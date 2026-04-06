#include <SDL2/SDL.h>


const int   SCREEN_W    = 800;
const int   SCREEN_H    = 600;

const float RADIUS_A    = 30.0f;   
const float RADIUS_B    = 30.0f;   

const float SPEED_A     = 220.0f;  
const float SPEED_B     = 200.0f;  

const float FLASH_DUR   = 0.30f;   

//Circle drawing 
void drawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int r)
{
    int x = 0, y = r, d = 1 - r;

    auto hline = [&](int x1, int x2, int ry) {
        SDL_RenderDrawLine(renderer, x1, ry, x2, ry);
    };

    hline(cx - r, cx + r, cy);
    while (x < y)
    {
        if (d < 0) { d += 2 * x + 3; }
        else       { d += 2 * (x - y) + 5; --y; }
        ++x;

        hline(cx - x, cx + x, cy + y);
        hline(cx - x, cx + x, cy - y);
        hline(cx - y, cx + y, cy + x);
        hline(cx - y, cx + y, cy - x);
    }
}

void drawCircleOutline(SDL_Renderer* renderer, int cx, int cy, int r)
{
    int x = 0, y = r, d = 1 - r;

    auto plot8 = [&]() {
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy + x);
        SDL_RenderDrawPoint(renderer, cx + y, cy - x);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);
    };

    plot8();
    while (x < y)
    {
        if (d < 0) { d += 2 * x + 3; }
        else       { d += 2 * (x - y) + 5; --y; }
        ++x;
        plot8();
    }
}


void drawCircle(SDL_Renderer* r,
                int cx, int cy, int radius,
                Uint8 fr, Uint8 fg, Uint8 fb,       // fill colour
                Uint8 or_, Uint8 og, Uint8 ob)       // outline colour
{
    SDL_SetRenderDrawColor(r, fr, fg, fb, 255);
    drawFilledCircle(r, cx, cy, radius);

    
    SDL_SetRenderDrawColor(r, or_, og, ob, 255);
    drawCircleOutline(r, cx, cy, radius);
    drawCircleOutline(r, cx, cy, radius + 1);
}

//squared distance 
inline float distSq(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1, dy = y2 - y1;
    return dx * dx + dy * dy;
}


inline float clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Task 103 - Two Circles + Collision",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // circle 1: auto-mover
    float ax   = RADIUS_A;                  
    float ay    = SCREEN_H / 2.0f;           
    float adirX = 1.0f;                      

    // circle 2: keyboard-controlled 
    float bx   = SCREEN_W / 2.0f;
    float by    = 0.0f + RADIUS_B;           

    //Collision and flash
    bool  wasColliding = false;
    float flashTimer   = 0.0f;

    Uint64 prevTime = SDL_GetPerformanceCounter();
    bool   running  = true;

    while (running)
    {
        //delta time
        Uint64 now = SDL_GetPerformanceCounter();
        float  dt  = (float)(now - prevTime) / (float)SDL_GetPerformanceFrequency();
        prevTime   = now;
        if (dt > 0.05f) dt = 0.05f;  

        
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)                          running = false;
            if (e.type == SDL_KEYDOWN &&
                e.key.keysym.sym == SDLK_ESCAPE)             running = false;
        }

        // keyboard input
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT])  bx -= SPEED_B * dt;
        if (keys[SDL_SCANCODE_RIGHT]) bx += SPEED_B * dt;
        if (keys[SDL_SCANCODE_UP])    by -= SPEED_B * dt;
        if (keys[SDL_SCANCODE_DOWN])  by += SPEED_B * dt;

        
        bx = clampf(bx, RADIUS_B,           SCREEN_W - RADIUS_B);
        by = clampf(by, RADIUS_B,           SCREEN_H - RADIUS_B);

        
        ax += SPEED_A * adirX * dt;

        if (ax - RADIUS_A <= 0.0f)          { ax = RADIUS_A;                adirX =  1.0f; }
        if (ax + RADIUS_A >= SCREEN_W)      { ax = SCREEN_W - RADIUS_A;    adirX = -1.0f; }

        //Collision detection
        float sumR    = RADIUS_A + RADIUS_B;
        bool  overlap = distSq(ax, ay, bx, by) <= sumR * sumR;

        if (overlap && !wasColliding)
        {
            
            flashTimer = FLASH_DUR;
        }
        wasColliding = overlap;

        if (flashTimer > 0.0f) flashTimer -= dt;

       
        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
        SDL_RenderClear(renderer);

        bool flash = (flashTimer > 0.0f);

        
        if (flash)
            drawCircle(renderer, (int)ax, (int)ay, (int)RADIUS_A,
                       255, 220,  30,          // fill: yellow
                       255, 255, 255);         // outline: white
        else
            drawCircle(renderer, (int)ax, (int)ay, (int)RADIUS_A,
                        40,  100, 210,         // fill: blue
                        90,  160, 255);        // outline: light blue

        
        if (flash)
            drawCircle(renderer, (int)bx, (int)by, (int)RADIUS_B,
                       255, 220,  30,
                       255, 255, 255);
        else
            drawCircle(renderer, (int)bx, (int)by, (int)RADIUS_B,
                        30,  180,  90,         // fill: green
                        70,  230, 130);        // outline: light green

        // red bar
        if (flash)
        {
            float frac = flashTimer / FLASH_DUR;         
            Uint8 alpha = (Uint8)(200 * frac);
            SDL_SetRenderDrawColor(renderer, 255, 60, 20, alpha);
            SDL_Rect bar = { 0, 0, SCREEN_W, 10 };
            SDL_RenderFillRect(renderer, &bar);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}