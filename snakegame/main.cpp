#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>


using namespace std;

// ---------- Color Palette ----------
const Color backgroundColor = {24, 28, 24, 255};   // soft warm charcoal
const Color borderColor     = {94, 120, 101, 255}; // muted sage
const Color snakeColor      = {0, 100, 88, 255}; // deep desaturated green
const Color textColor       = {230, 224, 212, 255}; // soft ivory
const Color popupColor      = {0, 0, 0, 0};    // graphite
const Color gameOverColor   = {180, 96, 82, 255};   // muted terracotta
const Color restartColor    = {170, 196, 170, 255}; // soft sage highlight
const Color exitColor       = {196, 180, 140, 255};  // muted sand
const Color boardShadow     = {18, 20, 18, 255};    // near-black green tint
const Color borderColor2     = {0, 0, 0, 0}; 

// ---------- Grid Settings ----------
int cellSize = 25;
int cellCount = 20;
int offset = 65;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, const deque<Vector2>& body)
{
    for (unsigned int i = 0; i < body.size(); i++)
    {
        if (Vector2Equals(body[i], element))
        {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
    Vector2 direction = { 1, 0 };
    bool addSegment = false;

    void draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;

            Rectangle segment = Rectangle{
                offset + x * cellSize,
                offset + y * cellSize,
                (float)cellSize,
                (float)cellSize
            };

            DrawRectangleRounded(segment, 0.5f, 6, snakeColor);
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));

        if (addSegment)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }
    }

    void Reset()
    {
        body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
        direction = { 1, 0 };
        addSegment = false;
    }
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food(const deque<Vector2>& snakeBody)
    {
        Image image = LoadImage("images/emoji_cyclops.png");
        ImageResize(&image, 25, 25);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);

        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{ x, y };
    }

    Vector2 GenerateRandomPos(const deque<Vector2>& snakeBody)
    {
        Vector2 pos = GenerateRandomCell();
        while (ElementInDeque(pos, snakeBody))
        {
            pos = GenerateRandomCell();
        }
        return pos;
    }
};

enum GameState
{
    RUNNING,
    GAME_OVER
};

class Game
{
public:
    Snake snake;
    Food food;
    GameState state;
    int score;
    Sound eatSound;
    Sound wallSound;

    Game() : snake(), food(snake.body)
    {
        InitAudioDevice();
        eatSound = LoadSound("sound/eat.mp3");
        wallSound = LoadSound("sound/wall.mp3");
        score = 0;
        state = RUNNING;
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.draw();
        snake.draw();
    }

    void Update()
    {
        if (state == RUNNING)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithWall();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    /*void CheckCollisionWithWall()
    {
        if (snake.body[0].x < 0 || snake.body[0].x >= cellCount)
        {
            GameOver();
        }

        if (snake.body[0].y < 0 || snake.body[0].y >= cellCount)
        {
            GameOver();
        }
    }*/
   void CheckCollisionWithWall()
{
    Vector2 head = snake.body[0];

    if (head.x < 0 || head.x >= cellCount || head.y < 0 || head.y >= cellCount)
    {
        snake.body.pop_front();
        GameOver();
    }
}

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();

        if (ElementInDeque(snake.body[0], headlessBody))
        {
            GameOver();
        }
    }

    void GameOver()
    {
        state = GAME_OVER;
        lastUpdateTime = GetTime();
        PlaySound(wallSound);
    }

    void RestartGame()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        score = 0;
        state = RUNNING;
        lastUpdateTime = GetTime();
    }

    void DrawGameOverPopup()
    {
        int screenWidth = 2 * offset + cellSize * cellCount;
        int screenHeight = 2 * offset + cellSize * cellCount;

        //DrawRectangle(0, 0, screenWidth, screenHeight, Fade(boardShadow, 0.55f));

        float boxWidth = 360;
        float boxHeight = 230;

        float boxX = screenWidth / 2.0f - boxWidth / 2.0f;
        float boxY = screenHeight / 2.0f - boxHeight / 2.0f;

        /*DrawRectangleRounded(
            Rectangle{ boxX, boxY, boxWidth, boxHeight },
            0.2f,
            10,
            popupColor
        );

        DrawRectangleLinesEx(
            Rectangle{ boxX, boxY, boxWidth, boxHeight },
            2,
            borderColor2
        ); */

        DrawText("GAME OVER", boxX + 50, boxY + 24, 42, gameOverColor);

        DrawText(
            TextFormat("Final Score: %i", score),
            boxX + 70,
            boxY + 92,
            30,
            textColor
        );

        DrawText("[R] Restart", boxX + 15, boxY + 170, 25, restartColor);
        DrawText("[ESC] Exit", boxX + 225, boxY + 170, 25, exitColor);
    }
};

int main()
{
    cout << "Starting the game...." << endl;

    int screenWidth = 2 * offset + cellSize * cellCount;
    int screenHeight = 2 * offset + cellSize * cellCount;

    InitWindow(screenWidth, screenHeight, "SNAKE");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Game game;
    bool exitRequested = false;

    while (WindowShouldClose() == false && exitRequested == false)
    {
        if (eventTriggered(0.3))
        {
            game.Update();
        }

        if (game.state == RUNNING)
        {
            if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
            {
                game.snake.direction = { 0, -1 };
            }
            if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
            {
                game.snake.direction = { 0, 1 };
            }
            if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
            {
                game.snake.direction = { 1, 0 };
            }
            if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
            {
                game.snake.direction = { -1, 0 };
            }
        }
        else if (game.state == GAME_OVER)
        {
            if (IsKeyPressed(KEY_R))
            {
                game.RestartGame();
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                exitRequested = true;
            }
        }

        BeginDrawing();
        ClearBackground(backgroundColor);

        DrawRectangle(
            offset - 5,
            offset - 5,
            cellSize * cellCount + 10,
            cellSize * cellCount + 10,
            boardShadow
        );

        DrawRectangleLinesEx(
            Rectangle{
                (float)offset - 5,
                (float)offset - 5,
                (float)cellSize * cellCount + 10,
                (float)cellSize * cellCount + 10
            },
            5,
            borderColor
        );

        game.Draw();

        DrawText("SNAKE", offset + 180, 15, 35, textColor);
        DrawText(TextFormat("%i", game.score), offset - 5, offset + cellSize * cellCount + 10, 30, textColor);

        if (game.state == GAME_OVER)
        {
            game.DrawGameOverPopup();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}