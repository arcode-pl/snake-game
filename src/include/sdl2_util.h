#ifndef SDL2_UTIL_H
#define SDL2_UTIL_H

#include <SDL2/SDL.h>

void SDL2Util_init(SDL_Renderer **renderer);
void SDL2Util_set_renderer_color(SDL_Color color);
void SDL2Util_draw_board(SDL_Rect rect, SDL_Color field, SDL_Color border);
void SDL2Util_draw_circle(SDL_Point point, int radius, SDL_Color color);

#endif
