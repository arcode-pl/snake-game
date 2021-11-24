#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_version.h>
#include <SDL2/SDL_ttf.h>
#include "include/sdl2_util.h"
#include "include/app_confg.h"

TTF_Font *arial;
static bool run_as_server;

bool process_keys(SDL_Keycode key);

int SnakeApp_start(bool is_server) {

    run_as_server = is_server;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL could not initialize\n");
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Error creating window.\n");
        return -1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!ren) {
		SDL_DestroyWindow(window);
		fprintf(stderr, "Could not create renderer : %s\n", SDL_GetError());
		return -1;
	}

    if(TTF_Init()) {
			fprintf(stderr, "Could not Initiate Fonts : %s\n", TTF_GetError());
			exit(4);
	}
	arial = TTF_OpenFont("resources/fonts/font.ttf", 50);
	if(arial == NULL) {
		fprintf(stderr, "Could not find the font loader file : %s\n", TTF_GetError());
		exit(5);
	}

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);

    SDL_Point point;
    point.x = 400;
    point.y = 400;

    SDL_Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 0;

    int radius = 5;

    SDL2Util_init(&ren);
    SDL2Util_draw_circle(point, radius, color);

    bool quit = false;
    while (quit == false)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch(e.type) {
                case SDL_QUIT:
                    printf("SDL_QUIT\n");
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                    printf("SDL_KEYDOWN\n");
                    quit = process_keys(e.key.keysym.sym);
                    break;

                default:

                    break;
            }
        }

        //create_board(&ren);
        /*
            do drawing here
        */

        SDL_RenderPresent(ren);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    TTF_CloseFont(arial);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}

bool process_keys(SDL_Keycode key) {
    if(key == SDLK_q) {
        return true;
    }

    switch(key)
    {
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_LEFT:
        case SDLK_RIGHT:
            if(run_as_server) {
                // process in game
            } else {
                // send to server
            }
            break;

        default:
            break;
    }

    return false;
}