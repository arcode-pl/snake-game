#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include "include/AppConfig.h"
#include "include/Snake.h"
#include "include/SnakeApp.h"
#include "include/Sdl2Util.h"
#include "include/SocketUtil.h"

static Sdl2Util_t sdl;
static SocketUtil_t su;
static SDL_Color colors[4] = {RED, GREEN, BLUE, WHITE};

#define BOARD_RECT \
    (SDL_Rect) { 0, 0, SCREEN_X_SIZE, SCREEN_Y_SIZE }

typedef struct Snake_map
{
    Snake_t snakes[MAX_PLAYERS];
    Snake_action_t action;
} Snake_map_t;

static Snake_map_t snakeMap;
static bool isServer;

static void drawSnake(Sdl2Util_t *sdl, Snake_t *snake, SDL_Color color);

static int snakeApp_initSocket(SnakeApp_config_t *config)
{
    memset(&su, 0, sizeof(SocketUtil_t));
    memcpy(su.config.ipAddr, config->ip, sizeof(config->ip));
    su.config.port = config->port;
    su.config.maxClients = config->maxClients;

    isServer = config->maxClients > 0;
    if (isServer)
    {
        return SocketUtil_createServer(&su);
    }
    else
    {
        return SocketUtil_connectToServer(&su);
    }
}

static void snakeApp_initSnakes(void)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *s = &(snakeMap.snakes[i]);
        Snake_init(s);
    }
}

static int snakeApp_startPlayer(void)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *snake = &(snakeMap.snakes[i]);
        if (snake->player < 0)
        {
            int x = (i + 1) * (SCREEN_X_SIZE / (MAX_PLAYERS + 1));
            int y = SCREEN_Y_SIZE / 2;
            snake->size = SNAKE_NODE;
            snake->player = i;
            Snake_spawn(snake, x, y);
            return i;
        }
    }

    printf("Sorry, max players reached");
    return -1;
}

static void snakeApp_endPlayer(int player, bool restart)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *snake = &(snakeMap.snakes[i]);
        if (snake->player == player)
        {
            Snake_destroy(snake);
            Snake_init(snake);
            if (restart)
            {
                snake->player = i;
            }
        }
    }
}

static void snakeApp_moveAction(int player, int action)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *snake = &(snakeMap.snakes[i]);
        if (snake->player == player)
        {
            snake->head->dir = action;
        }
    }
}

static void snakeApp_eatFood(int player)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *snake = &(snakeMap.snakes[i]);
        if (snake->player == player)
        {
            Snake_grow(snake);
        }
    }
}

static void snakeApp_drawSnakes()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *s = &(snakeMap.snakes[i]);
        if (s->player >= 0)
        {
            drawSnake(&sdl, s, colors[i]);
        }
    }
}

static void snakeApp_processNetwork(int *action)
{
    // if (isServer)
    // {
    //     SocketUtil_listenAndSend(&su, &snakeMap, action);
    // }
    // else
    // {
    //     SocketUtil_sendAndReceive(&su, &snakeMap, action);
    // }
}

int SnakeApp_run(SnakeApp_config_t *config)
{
    if (snakeApp_initSocket(config) < 0)
    {
        return -1;
    }

    memset(&sdl, 0, sizeof(Sdl2Util_t));
    if (Sdl2Util_initialize(&sdl) < 0)
    {
        return -1;
    }

    snakeApp_initSnakes();
    int player = snakeApp_startPlayer();
    snakeApp_moveAction(player, MOVE_LEFT_ACTION);
    snakeApp_eatFood(player);
    snakeApp_eatFood(player);
    snakeApp_eatFood(player);
    snakeApp_drawSnakes();

    bool quit = false;
    while (quit == false)
    {
        int action = Sdl2Util_poolEvent(&sdl);
        if (action == QUIT_ACTION)
        {
            quit = true;
        }

        Sdl2Util_clean(&sdl, BLACK);
        Sdl2Util_drawBoard(&sdl, BOARD_RECT, BLOCK_SIZE, BLUE, RED);

        snakeApp_drawSnakes();
        //snakeApp_processNetwork(&action);

        //Sdl2Util_drawBoard(&sdl, BOARD_RECT, BLOCK_SIZE, BLUE, RED);

        //draw snakes from map
        // Sdl2Util_draw(&snakeMap);

        //update
        Sdl2Util_update(&sdl);
    }

    Sdl2Util_dispose(&sdl);

    return 0;
}

static void drawSnake(Sdl2Util_t *sdl, Snake_t *snake, SDL_Color color)
{
    Snake_node_t *node = snake->tail;

    Sdl2Util_setRendererColor(sdl, color);
    while (node != snake->head)
    {
        Sdl2Util_drawPoint(sdl, node->x, node->y, SNAKE_NODE);
        switch (node->dir)
        {
        case UP:
            node->y -= snake->speed;
            break;
        case DOWN:
            node->y += snake->speed;
            break;
        case RIGHT:
            node->x += snake->speed;
            break;
        case LEFT:
            node->x -= snake->speed;
            break;
        default:
            break;
        }
        node = node->prev;
    }

    SDL_Point point;
    point.x = node->x;
    point.y = node->y;
    Sdl2Util_drawCircle(sdl, point, BLOCK_SIZE * 1.5, color);
}