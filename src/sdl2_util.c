#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "include/snake.h"

SDL_Renderer *graphic_ren;

void SDL2Util_init(SDL_Renderer **renderer) {
	graphic_ren = *renderer;
}

void SDL2Util_set_renderer_color(SDL_Color color) {
	SDL_SetRenderDrawColor(graphic_ren, color.r, color.g, color.b, color.a);
}

void SDL2Util_draw_board(SDL_Rect rect, SDL_Color field, SDL_Color border) {
	int i;
	int b = BLOCK_SIZE * 1.5;

	SDL2Util_set_renderer_color(field);		
	for(i = BLOCK_SIZE / 2; i < b; i++) {
		SDL_RenderDrawLine(graphic_ren, i, rect.y, i, rect.h);
		SDL_RenderDrawLine(graphic_ren, rect.w - i, rect.y, rect.w - i, rect.h);
		SDL_RenderDrawLine(graphic_ren, rect.x, i, rect.w, i);
		SDL_RenderDrawLine(graphic_ren, rect.x, rect.h - i, rect.w, rect.h - i);
	}

	SDL2Util_set_renderer_color(border);
	for(i = 0; i < BLOCK_SIZE / 2; i++) {
		SDL_RenderDrawLine(graphic_ren, i, rect.y, i, rect.h);
		SDL_RenderDrawLine(graphic_ren, rect.w - i, rect.y, rect.w - i, rect.h);
		SDL_RenderDrawLine(graphic_ren, rect.x, i, rect.w, i);
		SDL_RenderDrawLine(graphic_ren, rect.x, rect.h - i, rect.w, rect.h - i);
	}
}

void SDL2Util_draw_circle(SDL_Point point, int radius, SDL_Color color) {
	SDL2Util_set_renderer_color(color);
	int x;
	float y;
	for(x = 0; x <= radius; x++) {
		y = sqrt(radius * radius - (float)(x * x));
		SDL_RenderDrawLine(graphic_ren, point.x + x, point.y + y, point.x + x, point.y - y);
		SDL_RenderDrawLine(graphic_ren, point.x - x, point.y + y, point.x - x, point.y - y);
	}
}