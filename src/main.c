#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_version.h>

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

void show_help();
int SDL_game_loop();

int main(int argc, char *argv[]) {

	int opt, opt_index;
	const struct option optarr[] = {
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

    while((opt = getopt_long(argc, argv, "h", optarr, &opt_index)) != -1) {
		switch(opt) {
			case 'h':
				show_help();
				return 0;
				break;
			case '?':
				printf("Type %s --help | %s -h for help\n", argv[0], argv[0]);
				exit(0);
				break;			
		}		
	}

	return SDL_game_loop();
}

int SDL_game_loop() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL could not initialize\n");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Error creating window.\n");
        return 2;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);

    bool quit = false;
    while (quit == false)
    {
        SDL_Event windowEvent;
        while (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }

        /*
            do drawing here
        */

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);

    return 0;
}

void show_help(void) {
	int fd;
	char b;
	fd = open("info/help.txt", O_RDONLY);
	if(fd == -1) {
		fprintf(stderr, "Could not find help.txt\n");
		return;
	}
	while(read(fd, &b, sizeof(char)))
		if(b != '>' && b != '<')
		printf("%c", b);
	close(fd);
}
