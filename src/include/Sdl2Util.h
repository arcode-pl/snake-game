#ifndef SDL2_UTIL_H
#define SDL2_UTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_version.h>
#include <SDL2/SDL_ttf.h>

#define WHITE (SDL_Color){0xff,0xff,0xff,0x00}
#define BLACK (SDL_Color){0x00,0x00,0x00,0x00}
#define RED   (SDL_Color){0xff,0x00,0x00,0x00}
#define GREE  (SDL_Color){0x00,0xff,0x00,0x00}
#define BLUE  (SDL_Color){0x00,0x00,0xff,0x00}

typedef enum Sdl2Util_action {
    NONE_ACTION = 0,
    QUIT_ACTION = 1,
    MOVE_LEFT_ACTION = 2,
    MOVE_RIGHT_ACTION = 3,
    MOVE_UP_ACTION = 4,
    MOVE_DOWN_ACTION = 5,
} Sdl2Util_action_t;

typedef struct Sdl2Util {
    SDL_Window* window;
    SDL_Renderer *renderer;
    SDL_GLContext *glContext;
    TTF_Font *font;
} Sdl2Util_t;

int Sdl2Util_initialize(Sdl2Util_t *sdl);
Sdl2Util_action_t Sdl2Util_poolEvent(Sdl2Util_t *sdl);
void Sdl2Util_update(Sdl2Util_t *sdl);
void Sdl2Util_dispose(Sdl2Util_t *sdl);

void Sdl2Util_drawBoard(Sdl2Util_t *sdl, SDL_Rect rect, int blockSize, SDL_Color field, SDL_Color border);
void Sdl2Util_drawCircle(Sdl2Util_t *sdl, SDL_Point point, int radius, SDL_Color color);

#endif
