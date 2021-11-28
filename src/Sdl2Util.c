#include <math.h>
#include "include/Sdl2Util.h"
#include "include/AppConfig.h"
#include "include/Snake.h"

void Sdl2Util_setRendererColor(Sdl2Util_t *sdl, SDL_Color color)
{
	SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, color.a);
}

void Sdl2Util_clean(Sdl2Util_t *sdl, SDL_Color color)
{
	Sdl2Util_setRendererColor(sdl, color);
	SDL_RenderClear(sdl->renderer);
}

void Sdl2Util_update(Sdl2Util_t *sdl, int delayMs)
{
	SDL_RenderPresent(sdl->renderer);
	SDL_GL_SwapWindow(sdl->window);
	SDL_Delay(delayMs);
}

int Sdl2Util_poolEvent(Sdl2Util_t *sdl)
{
	int action = NONE_ACTION;

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
		{
			printf("SDL_QUIT\n");
			action = QUIT_ACTION;
			break;
		}
		case SDL_KEYDOWN:
		{
			SDL_Keycode keyCode = e.key.keysym.sym;

			switch (keyCode)
			{
			case SDLK_q:
				printf("SDL_KEYDOWN:SDLK_q\n");
				action = QUIT_ACTION;
				break;
			case SDLK_UP:
				printf("MOVE_UP_ACTION\n");
				action = MOVE_UP_ACTION;
				break;
			case SDLK_DOWN:
				printf("MOVE_DOWN_ACTION\n");
				action = MOVE_DOWN_ACTION;
				break;
			case SDLK_LEFT:
				printf("MOVE_LEFT_ACTION\n");
				action = MOVE_LEFT_ACTION;
				break;
			case SDLK_RIGHT:
				printf("MOVE_RIGHT_ACTION\n");
				action = MOVE_RIGHT_ACTION;
				break;
			case SDLK_w:
				printf("MOVE_UP_ACTION_2\n");
				action = MOVE_UP_ACTION_2;
				break;
			case SDLK_s:
				printf("MOVE_DOWN_ACTION_2\n");
				action = MOVE_DOWN_ACTION_2;
				break;
			case SDLK_a:
				printf("MOVE_LEFT_ACTION_2\n");
				action = MOVE_LEFT_ACTION_2;
				break;
			case SDLK_d:
				printf("MOVE_RIGHT_ACTION_2\n");
				action = MOVE_RIGHT_ACTION_2;
				break;
			case SDLK_KP_PLUS:
				printf("INCRASE_SNAKE_ACTION\n");
				action = INCRASE_SNAKE_ACTION;
				break;
			case SDLK_KP_MINUS:
				printf("REDUCE_SNAKE_ACTION\n");
				action = REDUCE_SNAKE_ACTION;
				break;
			case SDLK_DELETE:
				printf("END_SNAKE_ACTION\n");
				action = END_SNAKE_ACTION;
				break;
			case SDLK_INSERT:
				printf("NEW_SNAKE_ACTION\n");
				action = NEW_SNAKE_ACTION;
				break;
			default:
				break;
			}

			printf("SDL_KEYDOWN\n");
			break;
		}
		default:
			break;
		}
	}

	return action;
}

void Sdl2Util_dispose(Sdl2Util_t *sdl)
{
	SDL_GL_DeleteContext(sdl->glContext);
	TTF_CloseFont(sdl->font);
	TTF_Quit();
	SDL_DestroyRenderer(sdl->renderer);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}

void Sdl2Util_drawBoard(Sdl2Util_t *sdl, SDL_Rect rect, int blockSize, SDL_Color field, SDL_Color border)
{
	int i, b = blockSize * 1.5;

	Sdl2Util_setRendererColor(sdl, field);
	for (i = blockSize / 2; i < b; i++)
	{
		SDL_RenderDrawLine(sdl->renderer, i, rect.y, i, rect.h);
		SDL_RenderDrawLine(sdl->renderer, rect.w - i, rect.y, rect.w - i, rect.h);
		SDL_RenderDrawLine(sdl->renderer, rect.x, i, rect.w, i);
		SDL_RenderDrawLine(sdl->renderer, rect.x, rect.h - i, rect.w, rect.h - i);
	}

	Sdl2Util_setRendererColor(sdl, border);
	for (i = 0; i < blockSize / 2; i++)
	{
		SDL_RenderDrawLine(sdl->renderer, i, rect.y, i, rect.h);
		SDL_RenderDrawLine(sdl->renderer, rect.w - i, rect.y, rect.w - i, rect.h);
		SDL_RenderDrawLine(sdl->renderer, rect.x, i, rect.w, i);
		SDL_RenderDrawLine(sdl->renderer, rect.x, rect.h - i, rect.w, rect.h - i);
	}
}

void Sdl2Util_drawCircle(Sdl2Util_t *sdl, SDL_Point point, int radius, SDL_Color color)
{
	Sdl2Util_setRendererColor(sdl, color);
	int x;
	float y;
	for (x = 0; x <= radius; x++)
	{
		y = sqrt(radius * radius - (float)(x * x));
		SDL_RenderDrawLine(sdl->renderer, point.x + x, point.y + y, point.x + x, point.y - y);
		SDL_RenderDrawLine(sdl->renderer, point.x - x, point.y + y, point.x - x, point.y - y);
	}
}

void Sdl2Util_showText(Sdl2Util_t *sdl, char *text, SDL_Color textColor, SDL_Color backColor)
{
	SDL_Rect R1 = {10, 2, 400, 70};
	SDL_Surface *sur;
	SDL_Texture *tex;
	char scoretext[64];
	strncpy(scoretext, text, 64);
	sur = TTF_RenderText_Solid(sdl->font, scoretext, textColor);
	tex = SDL_CreateTextureFromSurface(sdl->renderer, sur);
	SDL_FreeSurface(sur);

	Sdl2Util_setRendererColor(sdl, backColor);
	SDL_RenderClear(sdl->renderer);

	Sdl2Util_setRendererColor(sdl, BLACK);
	SDL_RenderCopy(sdl->renderer, tex, NULL, &R1);
}

void Sdl2Util_drawNode(Sdl2Util_t *sdl, int cx, int cy, int size, SDL_Color color)
{
	Sdl2Util_setRendererColor(sdl, color);
	int x = cx + size;
	int y1 = cy - size;
	int y2 = cy + size;
	for (int i = cx - size; i < x; i++)
	{
		SDL_RenderDrawLine(sdl->renderer, i, y1, i, y2);
	}
}

int Sdl2Util_initialize(Sdl2Util_t *sdl)
{
	if (sdl == NULL)
	{
		fprintf(stderr, "SDL null pointer\n");
		return -1;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "SDL could not initialize\n");
		return -1;
	}

	sdl->window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_X_SIZE, SCREEN_Y_SIZE, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!sdl->window)
	{
		fprintf(stderr, "Error creating window.\n");
		return -1;
	}

	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!sdl->renderer)
	{
		SDL_DestroyWindow(sdl->window);
		fprintf(stderr, "Could not create renderer : %s\n", SDL_GetError());
		return -1;
	}

	if (TTF_Init())
	{
		fprintf(stderr, "Could not Initiate Fonts : %s\n", TTF_GetError());
		return -1;
	}

	sdl->font = TTF_OpenFont("resources/fonts/font.ttf", 50);
	if (sdl->font == NULL)
	{
		fprintf(stderr, "Could not find the font loader file : %s\n", TTF_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	sdl->glContext = SDL_GL_CreateContext(sdl->window);

	SDL_GL_SetSwapInterval(1);

	return 0;
}