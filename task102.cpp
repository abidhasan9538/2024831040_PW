//Expanding circle that resets when it touches any window edge.


#include <SDL2/SDL.h>

const int   SCREEN_W       = 800;
const int   SCREEN_H       = 600;
const float INITIAL_RADIUS = 20.0f;
const float GROWTH_RATE    = 80.0f;   // pixels per second

//Filled circle with midpoint scan-line algorithm
void drawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int r)
{
    int x = 0, y = r, d = 1 - r;

    // drawing one horizontal span
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

//Circle outline
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

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Expanding Circle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    const int   cx         = SCREEN_W / 2;
    const int   cy         = SCREEN_H / 2;
    const float MAX_RADIUS = (float)(cy < cx ? cy : cx);  // 300 for 800x600
    float       radius     = INITIAL_RADIUS;

    Uint64 prevTime = SDL_GetPerformanceCounter();
    bool   running  = true;

    while (running)
    {
        // delta time
        Uint64 now = SDL_GetPerformanceCounter();
        float  dt  = (float)(now - prevTime) / (float)SDL_GetPerformanceFrequency();
        prevTime   = now;

        
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)                              running = false;
            if (e.type == SDL_KEYDOWN &&
                e.key.keysym.sym == SDLK_ESCAPE)                running = false;
        }

    
        radius += GROWTH_RATE * dt;

        // Collision
        if (radius >= MAX_RADIUS)
            radius = INITIAL_RADIUS;

    
        SDL_SetRenderDrawColor(renderer, 15, 15, 25, 255);
        SDL_RenderClear(renderer);

        int r = (int)radius;

    
        SDL_SetRenderDrawColor(renderer, 40, 100, 200, 255);
        drawFilledCircle(renderer, cx, cy, r);

        SDL_SetRenderDrawColor(renderer, 100, 180, 255, 255);
        drawCircleOutline(renderer, cx, cy, r);
        drawCircleOutline(renderer, cx, cy, r + 1);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
