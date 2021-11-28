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
static SDL_Color colors[4] = {GREEN, RED, BLUE, WHITE};

#define BOARD_RECT \
    (SDL_Rect) { 0, 0, SCREEN_X_SIZE, SCREEN_Y_SIZE }

typedef struct Snake_map
{
    Snake_t snakes[MAX_PLAYERS];
    Snake_action_t action;
} Snake_map_t;

static Snake_map_t snakeMap;
static bool isServer;

static int player1;
static int player2;

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

static bool snakeApp_isPlayerValid(int player)
{
    return (player >= 0 && player < MAX_PLAYERS);
}

static bool snakeApp_isSnakeValid(Snake_t *snake)
{
    if (snake->head == NULL || snake->tail == NULL || !snakeApp_isPlayerValid(snake->player))
    {
        printf("Snake invisible now");
        return false;
    }

    return true;
}

static void snakeApp_initSnakes(void)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *snake = &(snakeMap.snakes[i]);
        Snake_init(snake);
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
            printf("Player %d start game at %d x %d\n", i, x, y);
            return i;
        }
    }

    printf("Sorry, max players reached");
    return -1;
}

static void snakeApp_endPlayer(int player)
{
    if (snakeApp_isPlayerValid(player))
    {
        Snake_t *snake = &(snakeMap.snakes[player]);
        if (snake->player == player)
        {
            Snake_destroy(snake);
            Snake_init(snake);
        }
    }
}

static void snakeApp_moveAction(int player, int direction)
{
    if (snakeApp_isPlayerValid(player))
    {
        snakeMap.snakes[player].head->dir = direction;
    }
}

static void snakeApp_increaseSnake(int player, int size)
{
    if (snakeApp_isPlayerValid(player))
    {
        Snake_t *snake = &(snakeMap.snakes[player]);
        if (snake->player == player)
        {
            Snake_increase(snake, size);
            Snake_printNodesFromHead(snake);
            Snake_printNodesFromTail(snake);
        }
    }
}

static void snakeApp_decreaseSnake(int player, int size)
{
    if (snakeApp_isPlayerValid(player))
    {
        Snake_t *snake = &(snakeMap.snakes[player]);
        if (snake->player == player)
        {
            Snake_decrease(snake, size);
            Snake_printNodesFromHead(snake);
            Snake_printNodesFromTail(snake);
        }
    }
}

static void snakeApp_drawSnakes()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Snake_t *snake = &(snakeMap.snakes[i]);
        if (snake->player >= 0)
        {
            drawSnake(&sdl, snake, colors[i]);
        }
    }
}

void SnakeApp_processAction(int player, int action)
{
    switch (action)
    {
    case MOVE_UP_ACTION:
    case MOVE_UP_ACTION_2:
        snakeApp_moveAction(player, UP);
        break;
    case MOVE_DOWN_ACTION:
    case MOVE_DOWN_ACTION_2:
        snakeApp_moveAction(player, DOWN);
        break;
    case MOVE_LEFT_ACTION:
    case MOVE_LEFT_ACTION_2:
        snakeApp_moveAction(player, LEFT);
        break;
    case MOVE_RIGHT_ACTION:
    case MOVE_RIGHT_ACTION_2:
        snakeApp_moveAction(player, RIGHT);
        break;
    case INCRASE_SNAKE_ACTION:
        snakeApp_increaseSnake(player, SMALL_FOOD);
        break;
    case REDUCE_SNAKE_ACTION:
        snakeApp_decreaseSnake(player, SMALL_FOOD);
        break;
    case END_SNAKE_ACTION:
        snakeApp_endPlayer(player);
        player = -1;
        break;
    case NEW_SNAKE_ACTION:
        if (player < 0)
        {
            player = snakeApp_startPlayer();
        }
        else
        {
            printf("Player already exist\n");
        }
        break;
    default:
        break;
    }
}

static inline bool isPlayer1(int action)
{
    switch (action)
    {
    case MOVE_UP_ACTION:
    case MOVE_DOWN_ACTION:
    case MOVE_LEFT_ACTION:
    case MOVE_RIGHT_ACTION:
        return true;
    default:
        return false;
    }
}

static inline bool isPlayer2(int action)
{
    switch (action)
    {
    case MOVE_UP_ACTION_2:
    case MOVE_DOWN_ACTION_2:
    case MOVE_LEFT_ACTION_2:
    case MOVE_RIGHT_ACTION_2:
        return true;
    default:
        return false;
    }
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

    player1 = -1;
    player2 = -1;
    snakeApp_initSnakes();

    bool quit = false;
    while (quit == false)
    {
        int action = Sdl2Util_poolEvent(&sdl);
        if (action == QUIT_ACTION)
        {
            printf("Quit action\n");
            quit = true;
        }

        if (isPlayer1(action))
        {
            if (player1 == -1)
            {
                player1 = snakeApp_startPlayer();
            }
            SnakeApp_processAction(player1, action);
        }

        if (isPlayer2(action))
        {
            if (player2 == -1)
            {
                player2 = snakeApp_startPlayer();
            }
            SnakeApp_processAction(player2, action);
        }

        Sdl2Util_clean(&sdl, BLACK);
        Sdl2Util_drawBoard(&sdl, BOARD_RECT, BLOCK_SIZE, BLUE, RED);
        snakeApp_drawSnakes();
        Sdl2Util_update(&sdl, DEFAULT_RENDERING_DELAY_MS);
    }

    Sdl2Util_dispose(&sdl);

    return 0;
}

static void drawSnake(Sdl2Util_t *sdl, Snake_t *snake, SDL_Color color)
{
    if (!snakeApp_isSnakeValid(snake))
    {
        return;
    }

    Snake_node_t *node = snake->head;

    SDL_Point point;
    point.x = node->x;
    point.y = node->y;
    Sdl2Util_drawCircle(sdl, point, BLOCK_SIZE * 1, color);

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
}