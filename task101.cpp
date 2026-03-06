//Draw a circle defining its xy-coordinates(center of the window) & radius.

#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

// function to draw a circle
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    int x = 0;
    int y = radius;

    int decision = 1 - radius;

    while (y >= x)
    {
        // drawing 8 symmetric points
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);

        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

        x++;

        if (decision < 0)
        {
            decision += 2 * x + 1;
        }
        else
        {
            y--;
            decision += 2 * (x - y) + 1;
        }
    }
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    int width = 800;
    int height = 600;

    // create window
    SDL_Window* window = SDL_CreateWindow(
        "Circle",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

  
    int cx = width / 2;
    int cy = height / 2;
    int radius = 120;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        
        SDL_SetRenderDrawColor(renderer, 0, 139, 139, 0);

        drawCircle(renderer, cx, cy, radius);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
