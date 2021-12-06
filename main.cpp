
#include <iostream>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#if __linux__
	#include <SDL2/SDL2_gfxPrimitives.h>
#elif __unix__
	#include <SDL2/SDL2_gfxPrimitives.h>
#endif

#ifdef _WIN32
    #include <SDL2/SDL2_gfx.h>
#endif

#ifdef _WIN64
    #include <SDL2/SDL2_gfx.h>
#endif
#include <cmath>

#define WINDOW_TITLE    "HW4_mHBr"
#define WIDTH           1500
#define HEIGHT          850
#define RECT_WIDTH      400
#define RECT_HEIGHT     400

#define DELTA_L         6

int init_window_and_renderer(SDL_Window** window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error in initialization: %s", SDL_GetError());
        return -1;
    }
    *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_HIDDEN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (!*window)
    {
        printf("Error with SDL_Window: %s", SDL_GetError());
        return -1;
    }

    if (!*renderer)
    {
        printf("Error with renderer: %s", SDL_GetError());
        return -1;
    }
    return 1;
}

void draw_rect(SDL_Rect *rect, SDL_Renderer** renderer, SDL_Texture** texture, Uint32 color)
{
    SDL_SetRenderTarget(*renderer, *texture);
    //SDL_SetRenderDrawColor(*renderer , (Uint8)color , (Uint8)color<<8 , (Uint8)color<<16 , (Uint8)color<<24);
    SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(*renderer, rect);
    SDL_RenderCopy(*renderer, *texture, NULL, NULL);
    SDL_RenderPresent(*renderer);
}

int main(int argc, char *argv[])
{

    SDL_Window *win_main = NULL;
    SDL_Renderer *renderer_main = NULL;
    bool should_loop = true;

    if (init_window_and_renderer(&win_main, &renderer_main) < 0)
    {
        return -1;
    }

    SDL_Event event;
    int x1 = 0, y1 = 0, x2 = WIDTH-RECT_WIDTH, y2 = HEIGHT-RECT_HEIGHT;
    int dx1 = DELTA_L, dy1 = DELTA_L, dx2 = -DELTA_L, dy2 = -DELTA_L;

    SDL_ShowWindow(win_main);

    while (should_loop)
    {

        if (x1+RECT_WIDTH > WIDTH || x1 < 0)
        {
            dx1 *= -1;
        }
        if (x2+RECT_WIDTH > WIDTH || x2 < 0)
        {
            dx2 *= -1;
        }
        if (y2+RECT_HEIGHT > HEIGHT || y2 < 0)
        {
            dy2 *= -1;
        }
        if (y1+RECT_HEIGHT > HEIGHT || y1 < 0)
        {
            dy1 *= -1;
        }

        x1 += dx1;
        y1 += dy1;
        x2 += dx2;
        y2 += dy2;

        SDL_Texture* rect1_texture = SDL_CreateTexture(renderer_main, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,  RECT_WIDTH, RECT_HEIGHT);
        SDL_Texture* rect2_texture = SDL_CreateTexture(renderer_main, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RECT_WIDTH, RECT_HEIGHT);

        SDL_Rect rect1, rect2;
        rect1.x = x1;
        rect2.x=x2;
        rect1.y = y1;
        rect2.y=y2;
        rect1.h = RECT_HEIGHT;
        rect2.h=RECT_HEIGHT;
        rect1.w = RECT_WIDTH;
        rect2.w=RECT_WIDTH;


        SDL_Texture * intersect_texture = SDL_CreateTexture(renderer_main , SDL_PIXELFORMAT_RGBA8888 , SDL_TEXTUREACCESS_TARGET , RECT_WIDTH , RECT_HEIGHT);



        SDL_SetRenderDrawColor(renderer_main, 30,40, 50, 255);
        SDL_RenderClear(renderer_main);

        SDL_Rect intersect;
        SDL_IntersectRect(&rect1 , &rect2 ,&intersect);
        double c = intersect.w * intersect.h;
        c /=(RECT_HEIGHT*RECT_WIDTH);
        c*=255;
        int intersect_s = c;

        SDL_SetRenderTarget(renderer_main, rect2_texture);
        SDL_SetRenderDrawColor(renderer_main, 180, 150, 100, 255);
        SDL_RenderClear(renderer_main);
        SDL_SetRenderTarget(renderer_main, NULL);


        SDL_SetRenderTarget(renderer_main, rect1_texture);
        SDL_SetRenderDrawColor(renderer_main, 100, 100, 250, 255);
        SDL_RenderClear(renderer_main);
        SDL_SetRenderTarget(renderer_main, NULL);

        SDL_SetRenderTarget(renderer_main , intersect_texture);
        SDL_SetRenderDrawColor(renderer_main , 255 , 255-intersect_s , 255-intersect_s , 255);
        SDL_RenderClear(renderer_main);
        SDL_SetRenderTarget(renderer_main , NULL);

        SDL_RenderCopy(renderer_main, rect1_texture, NULL, &rect1);
        SDL_RenderPresent(renderer_main);
        SDL_RenderCopy(renderer_main, rect2_texture, NULL, &rect2);
        SDL_RenderPresent(renderer_main);
        SDL_RenderCopy(renderer_main , intersect_texture , NULL , &intersect);
        SDL_RenderPresent(renderer_main);


        SDL_Delay(20);


        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == 'q')
                {
                    should_loop=false;
                    break;
                }
            case SDL_QUIT:
                should_loop = false;
                break;
            }
        }


        // SDL_RenderClear(renderer_main);
        // SDL_SetRenderDrawColor(renderer_main, 30, 40, 50, 255);
        // SDL_RenderFillRect(renderer_main, NULL);
        // SDL_RenderPresent(renderer_main);
    }

    SDL_DestroyRenderer(renderer_main);
    renderer_main = NULL;
    SDL_DestroyWindow(win_main);
    win_main = NULL;
    SDL_Quit();

    return 0;
}
