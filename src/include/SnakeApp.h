#ifndef SNAKE_APP_H
#define SNAKE_APP_H

typedef struct SnakeApp_config {
    char ip[16];
    int port;
    int maxClients;
} SnakeApp_config_t;

int SnakeApp_run(SnakeApp_config_t *config);

#endif