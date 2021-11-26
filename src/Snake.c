#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/Snake.h"
#include "include/AppConfig.h"

void Snake_init(Snake_t *snake)
{
    snake->head = snake->tail = NULL;
    snake->speed = START_SPEED;
    snake->dir = ND;
    snake->player = -1;
    snake->size = SNAKE_NODE;
}

void Snake_spawn(Snake_t *snake, int x, int y)
{
    Snake_destroy(snake);
    snake->head = (Snake_node_t *)malloc(sizeof(Snake_node_t));
    snake->head->dir = snake->dir;
    snake->head->x = x;
    snake->head->y = y;
    snake->head->next = NULL;
    snake->head->prev = NULL;
    snake->tail = snake->head;
}

int Snake_grow(Snake_t *snake)
{
    Snake_node_t *node = (Snake_node_t *)malloc(sizeof(Snake_node_t));
    if (node == NULL)
    {
        printf("Not enough RAM!\n");
        return -1;
    }
    node->next = NULL;
    node->prev = NULL;

    Snake_node_t *tmp = snake->head;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = node;
    tmp->next->dir = tmp->dir;

    switch (tmp->dir)
    {
    case UP:
        tmp->next->x = tmp->x;
        tmp->next->y = tmp->y + BLOCK_SIZE;
        break;
    case DOWN:
        tmp->next->x = tmp->x;
        tmp->next->y = tmp->y - BLOCK_SIZE;
        break;
    case RIGHT:
        tmp->next->x = tmp->x - BLOCK_SIZE;
        tmp->next->y = tmp->y;
        break;
    case LEFT:
        tmp->next->x = tmp->x + BLOCK_SIZE;
        tmp->next->y = tmp->y;
        break;
    default:
        break;
    }
    tmp->next->prev = tmp;
    snake->tail = node;

    return 0;
}

static void snake_destroyAllNodes(Snake_node_t *node)
{
    if (node == NULL)
    {
        return;
    }

    snake_destroyAllNodes(node->next);
    free(node);
}

void Snake_destroy(Snake_t *snake)
{
    Snake_node_t *node = snake->head;
    snake_destroyAllNodes(node);
}