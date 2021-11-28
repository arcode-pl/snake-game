#include <stdio.h>
#include <string.h> //strlen
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>

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

void SocketUtil_printComm(SocketUtil_comm_t *comm)
{
	printf("Command: %d, Len: %d \nData:", comm->command, comm->len);
	if (comm->len > COMM_DATA_LEN)
	{
		printf("Communication: Wrong len !");
	}
	else
	{
		for (int i = 0; i < comm->len; i++)
		{
			printf("0x%08x ", comm->data[i]);
		}
	}
	printf("\n");
}

int SocketUtil_send(SocketUtil_t *su, SocketUtil_comm_t *comm)
{
	int len = send(su->socket, comm, sizeof(SocketUtil_comm_t), 0);
	printf("Send %d bytes:\n", len);
	SocketUtil_printComm(comm);

	return len;
}

int SocketUtil_receive(SocketUtil_t *su, SocketUtil_comm_t *comm)
{
	int len = recv(su->socket, comm, sizeof(SocketUtil_comm_t), 0);
	printf("Received %d bytes:\n", len);
	SocketUtil_printComm(comm);

	return len;
}

static int sd, max_sd, activity, new_socket;
static fd_set readfds;
static struct timeval timeout_tv;

int SocketUtil_listenClients(SocketUtil_t *su, SocketUtil_comm_t *comm, int timeout_us)
{
	int valRead;

	timeout_tv.tv_sec = 0;
	timeout_tv.tv_usec = timeout_us;

	//clear the socket set
	FD_ZERO(&readfds);

	//add master socket to set
	FD_SET(su->socket, &readfds);
	max_sd = su->socket;

	//add child sockets to set
	for (int i = 0; i < su->config.maxClients; i++)
	{
		//socket descriptor
		sd = clientSocketList[i];

		//if valid socket descriptor then add to read list
		if (sd > 0)
		{
			FD_SET(sd, &readfds);
		}
		//highest file descriptor number, need it for the select function
		if (sd > max_sd)
		{
			max_sd = sd;
		}
	}

	//wait for an activity on one of the sockets , timeout is NULL ,
	//so wait indefinitely
	activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout_tv);
	if (activity <= 0)
	{
		return 0;
	}

	//If something happened on the master socket ,
	//then its an incoming connection
	if (FD_ISSET(su->socket, &readfds))
	{
		if ((new_socket = accept(su->socket, (struct sockaddr *)&(su->sockAddrIn),
								 (socklen_t *)&(su->sockLen))) < 0)
		{
			printf("accept error");
			return 0;
		}

		//inform user of socket number - used in send and receive commands
		printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(su->sockAddrIn.sin_addr), ntohs(su->sockAddrIn.sin_port));

		//send new connection greeting message
		// if (send(new_socket, "", strlen(message), 0) != strlen(message))
		// {
		// 	perror("send");
		// }

		puts("Welcome message sent successfully");

		//add new socket to array of sockets
		for (int i = 0; i < su->config.maxClients; i++)
		{
			//if position is empty
			if (clientSocketList[i] == 0)
			{
				clientSocketList[i] = new_socket;
				printf("Adding to list of sockets as %d\n", i);

				return 0;
			}
		}
	}

	//else its some IO operation on some other socket
	for (int i = 0; i < su->config.maxClients; i++)
	{
		sd = clientSocketList[i];

		if (FD_ISSET(sd, &readfds))
		{
			//Check if it was for closing , and also read the
			//incoming message
			if ((valRead = recv(sd, comm, sizeof(SocketUtil_comm_t), 0)) == 0)
			{
				//Somebody disconnected , get his details and print
				getpeername(sd, (struct sockaddr *)&(su->sockAddrIn),
							(socklen_t *)&(su->sockLen));
				printf("Host disconnected , ip %s , port %d \n",
					   inet_ntoa(su->sockAddrIn.sin_addr), ntohs(su->sockAddrIn.sin_port));

				//Close the socket and mark as 0 in list for reuse
				close(sd);
				clientSocketList[i] = 0;
			}

			//Echo back the message that came in
			else
			{
				send(sd, comm, sizeof(SocketUtil_comm_t), 0);
			}
		}
	}

	return 0;
}