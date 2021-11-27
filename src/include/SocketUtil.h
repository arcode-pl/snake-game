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

typedef enum Snake_command
{
    ACTION_COMMAND,
    MAP_COMMAND,
} Snake_command_t;

#define COMM_DATA_LEN 1024
typedef struct SocketUtil_comm
{
    int command;
    int data[COMM_DATA_LEN];
    int len;
} SocketUtil_comm_t;

int SocketUtil_createServer(SocketUtil_t *su);
int SocketUtil_connectToServer(SocketUtil_t *su);

int SocketUtil_send(SocketUtil_t *su, SocketUtil_comm_t *comm);
int SocketUtil_receive(SocketUtil_t *su, SocketUtil_comm_t *comm);

int SocketUtil_sendAndReceiveAction(SocketUtil_t *su, SocketUtil_comm_t *comm);
int SocketUtil_listenClients(SocketUtil_t *su, SocketUtil_comm_t *comm, int timeout_us);

#endif