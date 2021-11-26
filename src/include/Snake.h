#ifndef SNAKE_H
#define SNAKE_H

#define START_SPEED 2
#define BLOCK_SIZE 8
#define SNAKE_NODE BLOCK_SIZE - 2

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
    NONE_ACTION = 0,
    QUIT_ACTION = 1,
    MOVE_LEFT_ACTION = 2,
    MOVE_RIGHT_ACTION = 3,
    MOVE_UP_ACTION = 4,
    MOVE_DOWN_ACTION = 5,
} Snake_action_t;

void Snake_init(Snake_t *snake);
void Snake_spawn(Snake_t *snake, int x, int y);
int Snake_grow(Snake_t *snake);
void Snake_destroy(Snake_t *snake);

#endif
