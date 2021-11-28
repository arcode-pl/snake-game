#ifndef SNAKE_H
#define SNAKE_H

#define START_SPEED 2
#define BLOCK_SIZE 8
#define SNAKE_NODE BLOCK_SIZE - 2

#define SMALL_FOOD 1
#define MID_FOOD 3
#define BIG_FOOD 10

typedef enum Snake_direction
{
    ND,
    UP,
    DOWN,
    LEFT,
    RIGHT
} Snake_direction_t;

typedef struct node
{
    int x, y;              //x and y coordinates of the block
    Snake_direction_t dir; //direction of the block
    struct node *next, *prev;
} Snake_node_t;

typedef struct Snake
{
    int speed;             //speed of the snake
    Snake_direction_t dir; //diretion of the head of the snake
    Snake_node_t *head, *tail;
    int player;
    int size;
} Snake_t;

typedef enum Snake_action
{
    NONE_ACTION,
    QUIT_ACTION,
    PAUSE_ACTION,
    MOVE_LEFT_ACTION,
    MOVE_RIGHT_ACTION,
    MOVE_UP_ACTION,
    MOVE_DOWN_ACTION,
    MOVE_LEFT_ACTION_2,
    MOVE_RIGHT_ACTION_2,
    MOVE_UP_ACTION_2,
    MOVE_DOWN_ACTION_2,
    INCRASE_SNAKE_ACTION,
    REDUCE_SNAKE_ACTION,
    INCRASE_SNAKE_ACTION_2,
    REDUCE_SNAKE_ACTION_2,
    NEW_SNAKE_ACTION,
    END_SNAKE_ACTION,
} Snake_action_t;

void Snake_init(Snake_t *snake);
void Snake_spawn(Snake_t *snake, int x, int y);
int Snake_increase(Snake_t *snake, int size);
int Snake_decrease(Snake_t *snake, int size);
void Snake_destroy(Snake_t *snake);

void Snake_printNodesFromHead(Snake_t *snake);
void Snake_printNodesFromTail(Snake_t *snake);

#endif
