#include<raylib.h>
#include<iostream>
#include<deque>
#include<raymath.h>
using namespace std;
//struct color(red, green blue, alpha) value rang 0-255
Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
//creating a grid for placing objects
int cellSize = 25;
int cellCount = 20;
int offset = 65;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for(unsigned int i = 0; i<deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
        
    }
    return false;
}

bool eventTriggered(double interval) 
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

//snake class
class Snake {
    public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addSegment = false;
    void draw()
    {
        for(unsigned int i = 0; i<body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset+x*cellSize, offset+y*cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);
        }
    }
    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true)
        {
            
            addSegment = false;

        }
        else {
          body.pop_back();
         
        }
        
    }

    void Reset()
    {
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
    }

};
//food class
class Food {
    public:
    Vector2 position; // raylib struct data structure for storing two datas, ideal for coordinate positions
                              // can be accessed by .x and .y
    Texture2D texture;                              
    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("images/slime.png");
        ImageResize(&image, 30,30);
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
        DrawTexture(texture, offset + position.x * cellSize, offset+position.y * cellSize, WHITE);

    }
    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);
        return Vector2{x,y};
    }
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        
        Vector2 position = GenerateRandomCell();
        while(ElementInDeque(position, snakeBody)){
            position = GenerateRandomCell();
        }
        return position;
    }

};

class Game {
    public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("sound/eat.mp3");
        wallSound = LoadSound("sound/wall.mp3");
    }
    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw(){
        food.draw();
        snake.draw();
    }
    void Update()
    {
        if(running)
        {
        snake.Update();
        CheckCollisionWithFood();
        CheckCollisionWithWall();
        CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], food.position)) 
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithWall()
    {
        if(snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
        
    }

    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(ElementInDeque(snake.body[0], headlessBody))
        {
            GameOver();
        }
    }
};

int main()
{
    cout << "Starting the game...." << endl;
    //create window
    InitWindow(2 * offset + cellSize * cellCount,2 * offset + cellSize * cellCount, "SNAKE"); //width, height and title
    SetTargetFPS(60); //determines how fast the game will run, specific fps for all machines, runs at the same speed in every computer
    Game game = Game();
    //game loop
    while(WindowShouldClose() == false) // esc or cross button tapped or not check, if true, game loop ends
    {
        BeginDrawing(); //creates a blank canvas that we will draw the game objects on
        if(eventTriggered(0.3)){
            game.Update();
            
        }
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y !=-1){
            game.snake.direction = {0,1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1,0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
            game.running = true;
        }
        ClearBackground(green); //clear the background every frame with the color green
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize * cellCount+10, (float)cellSize * cellCount +10}, 5, darkGreen);
        game.Draw();
        DrawText("SNAKE", offset+180, 15, 35,darkGreen);
        DrawText(TextFormat("%i", game.score), offset-5, offset+cellSize*cellCount+10, 30, darkGreen);
        EndDrawing(); // ends the canvas drawing
    }
    //destroy window
    CloseWindow();
    return 0;

}