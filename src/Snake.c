#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/Snake.h"
#include "include/AppConfig.h"

void Snake_init(Snake_t *snake)
{
    snake->head = NULL;
    snake->tail = NULL;
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

static Snake_node_t *snake_createNode()
{
    Snake_node_t *node = (Snake_node_t *)malloc(sizeof(Snake_node_t));
    return node;
}

void Snake_printNodesFromHead(Snake_t *snake)
{
    Snake_node_t *node = snake->head;
    printf("from head:\n");
    while (node != NULL && node != snake->tail)
    {
        printf("node: 0x%p / next: 0x%p\n", node, node->next);
        printf("x: %d, y: %d, dir: %d\n", node->x, node->y, node->dir);
        node = node->next;
    }
}

void Snake_printNodesFromTail(Snake_t *snake)
{
    Snake_node_t *node = snake->tail;
    printf("from tail:\n");
    while (node != NULL && node != snake->head)
    {
        printf("node: 0x%p / prev: 0x%p\n", node, node->prev);
        printf("x: %d, y: %d, dir: %d\n", node->x, node->y, node->dir);
        node = node->prev;
    }
}

static void snake_calcNewTail(Snake_node_t *node)
{
    node->dir = node->prev->dir;

    switch (node->dir)
    {
    case UP:
        node->x = node->prev->x;
        node->y = node->prev->y + BLOCK_SIZE;
        break;
    case DOWN:
        node->x = node->prev->x;
        node->y = node->prev->y - BLOCK_SIZE;
        break;
    case LEFT:
        node->x = node->prev->x - BLOCK_SIZE;
        node->y = node->prev->y;
        break;
    case RIGHT:
        node->x = node->prev->x + BLOCK_SIZE;
        node->y = node->prev->y;
        break;
    default:
        break;
    }
}

int Snake_increase(Snake_t *snake, int size)
{
    if (size <= 0 || size > 10)
    {
        printf("Valid snake increase range is from 1 to 10\n");
        return -1;
    }

    if (snake->head->dir == ND)
    {
        printf("Can't add node when snake is paused\n");
        return -1;
    }

    for (int i = 0; i < size; i++)
    {
        Snake_node_t *new = snake_createNode();
        if (new == NULL)
        {
            printf("Not enough RAM!\n");
            return -1;
        }

        snake->tail->next = new;
        new->prev = snake->tail;
        new->next = NULL;
        snake->tail = new;

        snake_calcNewTail(new);
    }

    return 0;
}

int Snake_decrease(Snake_t *snake, int size)
{
    if (size <= 0 || size > 10)
    {
        printf("Valid snake increase range is from 1 to 10\n");
        return -1;
    }

    for (int i = 0; i < size; i++)
    {
        Snake_node_t *node = snake->tail;
        if (node->prev != NULL)
        {
            snake->tail = snake->tail->prev;
            snake->tail->next = NULL;
            free(node);
        }
    }

    return 0;
}

static void snake_destroyAllNextNodes(Snake_node_t *node)
{
    if (node == NULL)
    {
        return;
    }
    snake_destroyAllNextNodes(node->next);
    free(node);
}

void Snake_destroy(Snake_t *snake)
{
    Snake_node_t *node = snake->head;
    if (node == NULL)
    {
        printf("Snake already destroyed\n");
        return;
    }
    snake_destroyAllNextNodes(node);
}