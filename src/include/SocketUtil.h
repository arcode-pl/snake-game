#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <arpa/inet.h>

typedef struct SocketUtil_config {
    char ipAddr[16];
    int port;
    int maxClients; 
} SocketUtil_config_t;

typedef struct SocketUtil {
    SocketUtil_config_t config;
    int socket;
    struct sockaddr_in sockAddrIn;
    socklen_t sockLen;
} SocketUtil_t;

int SocketUtil_initialize(SocketUtil_t *su);

#endif