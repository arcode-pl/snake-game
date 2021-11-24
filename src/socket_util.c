#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "include/app_confg.h"

int sock = 0;
int server_sock = 0;
int clients_sock[APP_CONFIG_MAX_CLIENTS];
struct sockaddr_in address;

#define OPT_TRUE 1

int SocketUtil_create_server(void) {

    int i;
    int opt;
	
	for (i = 0; i < APP_CONFIG_MAX_CLIENTS; i++)
	{
		clients_sock[i] = 0;
	}
		
	if( (server_sock = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		return -1;
	}
	
    opt = OPT_TRUE;
	if( setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		return -1;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( APP_CONFIG_SERVER_PORT );
	if (bind(server_sock, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		return -1;
	}
	printf("Listener on port %d \n", APP_CONFIG_SERVER_PORT);
		
	if (listen(server_sock, 3) < 0)
	{
		perror("listen");
		return -1;
	}
		
	puts("Server configured");

    return 0;
}

int SocketUtil_connect_to_server(char *ipaddr) {

    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(APP_CONFIG_SERVER_PORT);

    if(inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    puts("Connected to server");

    return 0;
}

