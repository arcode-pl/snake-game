#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "include/SnakeApp.h"
#include "include/Sdl2Util.h"
#include "include/SocketUtil.h"

static Sdl2Util_t sdl;
static SocketUtil_t su;

void SnakeApp_initConfig(SnakeApp_config_t *config) {
    memcpy(su.config.ipAddr, config->ip, sizeof(config->ip));
    su.config.port = config->port;
    su.config.maxClients = config->maxClients;
}

int SnakeApp_run(SnakeApp_config_t *config) 
{
    memset(&su, 0, sizeof(SocketUtil_t));
    SnakeApp_initConfig(config);
    if (SocketUtil_initialize(&su) < 0) {
        return -1;
    }

    memset(&sdl, 0, sizeof(Sdl2Util_t));
    if (Sdl2Util_initialize(&sdl) < 0) {
        return -1;
    }

    SDL_Point point;
    point.x = 400;
    point.y = 400;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 800;
    rect.h = 600;

    SDL_Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 0;

    int radius = 5;

    Sdl2Util_drawCircle(&sdl, point, radius, color);

    Sdl2Util_drawBoard(&sdl, rect, 8, RED, BLACK);

    bool quit = false;
    while (quit == false) {
        
        Sdl2Util_action_t action = Sdl2Util_poolEvent(&sdl);

        if(action == QUIT_ACTION) {
            quit = true;
        }

        //create_board(&ren);
        /*
            do drawing here
        */

        Sdl2Util_update(&sdl);
    }

    Sdl2Util_dispose(&sdl);

    return 0;
}