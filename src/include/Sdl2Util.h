#ifndef SDL2_UTIL_H
#define SDL2_UTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_version.h>
#include <SDL2/SDL_ttf.h>

#define WHITE \
    (SDL_Color) { 0xff, 0xff, 0xff, 0x00 }
#define BLACK \
    (SDL_Color) { 0x00, 0x00, 0x00, 0x00 }
#define RED \
    (SDL_Color) { 0xff, 0x00, 0x00, 0x00 }
#define GREEN \
    (SDL_Color) { 0x00, 0xff, 0x00, 0x00 }
#define BLUE \
    (SDL_Color) { 0x00, 0x00, 0xff, 0x00 }

#define GREEN_T_LOW \
    (SDL_Color) { 0x00, 0xef, 0x00, 0x55 }
#define GREEN_T_MID \
    (SDL_Color) { 0x00, 0xef, 0x00, 0x88 }
#define GREEN_T_HIG \
    (SDL_Color) { 0x00, 0xef, 0x00, 0xee }

typedef struct Sdl2Util
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GLContext *glContext;
    TTF_Font *font;
} Sdl2Util_t;

int Sdl2Util_initialize(Sdl2Util_t *sdl);
int Sdl2Util_poolEvent(Sdl2Util_t *sdl);
void Sdl2Util_clean(Sdl2Util_t *sdl, SDL_Color color);
void Sdl2Util_update(Sdl2Util_t *sdl);
void Sdl2Util_dispose(Sdl2Util_t *sdl);

void Sdl2Util_setRendererColor(Sdl2Util_t *sdl, SDL_Color color);
void Sdl2Util_drawBoard(Sdl2Util_t *sdl, SDL_Rect rect, int blockSize, SDL_Color field, SDL_Color border);
void Sdl2Util_drawCircle(Sdl2Util_t *sdl, SDL_Point point, int radius, SDL_Color color);
void Sdl2Util_showText(Sdl2Util_t *sdl, char *text, SDL_Color textColor, SDL_Color backColor);
void Sdl2Util_drawPoint(Sdl2Util_t *sdl, int cx, int cy, int side);

#endif
