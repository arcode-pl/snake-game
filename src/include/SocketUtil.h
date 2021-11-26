#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <arpa/inet.h>

typedef struct SocketUtil_config
{
    char ipAddr[16];
    int port;
    int maxClients;
} SocketUtil_config_t;

typedef struct SocketUtil
{
    SocketUtil_config_t config;
    int socket;
    struct sockaddr_in sockAddrIn;
    socklen_t sockLen;
} SocketUtil_t;

int SocketUtil_createServer(SocketUtil_t *su);
int SocketUtil_connectToServer(SocketUtil_t *su);

// int SocketUtil_listenAndSend(SocketUtil_t *su, char *data, int len);
// int SocketUtil_sendAndReceive(SocketUtil_t *su, char *data, int *len);

#endif