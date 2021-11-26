#include <stdio.h>
#include <string.h> //strlen
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "include/AppConfig.h"
#include "include/SocketUtil.h"

static int clientSocketList[MAX_CLIENTS];

int SocketUtil_createServer(SocketUtil_t *su)
{
	int optTrue = OPT_TRUE;

	if (su == NULL)
	{
		perror("su ptr is NULL");
		return -1;
	}

	memset(clientSocketList, 0, MAX_CLIENTS * sizeof(int));

	if ((su->socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		return -1;
	}

	if (setsockopt(su->socket, SOL_SOCKET, SO_REUSEADDR, (char *)&optTrue, sizeof(optTrue)) < 0)
	{
		perror("setsockopt failed");
		return -1;
	}

	su->sockAddrIn.sin_family = AF_INET;
	su->sockAddrIn.sin_addr.s_addr = INADDR_ANY;
	su->sockAddrIn.sin_port = htons(su->config.port);
	if (bind(su->socket, (struct sockaddr *)&(su->sockAddrIn), sizeof(su->sockAddrIn)) < 0)
	{
		perror("bind failed");
		return -1;
	}

	if (listen(su->socket, 3) < 0)
	{
		perror("listen");
		return -1;
	}

	su->sockLen = sizeof(su->sockAddrIn);

	puts("Server configured, waiting for connections ...");

	return 0;
}

int SocketUtil_connectToServer(SocketUtil_t *su)
{
	if ((su->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	su->sockAddrIn.sin_family = AF_INET;
	su->sockAddrIn.sin_port = htons(su->config.port);

	if (inet_pton(AF_INET, su->config.ipAddr, &(su->sockAddrIn.sin_addr)) <= 0)
	{
		printf("\nInvalid address | Address not supported \n");
		return -1;
	}

	if (connect(su->socket, (struct sockaddr *)&(su->sockAddrIn), sizeof(su->sockAddrIn)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	puts("Connected to server");

	return 0;
}
