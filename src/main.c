#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>

#include "include/AppConfig.h"
#include "include/SnakeApp.h"

static SnakeApp_config_t config;

void init_config(SnakeApp_config_t *config);
void show_help();

void init_config(SnakeApp_config_t *config)
{
	memset(config, 0, sizeof(SnakeApp_config_t));
	memcpy(config->ip, DEFAULT_SERVER_IP, sizeof(DEFAULT_SERVER_IP));
	config->port = DEFAULT_SERVER_PORT;
	config->maxClients = MAX_CLIENTS;
}

void show_help(void)
{
	int fd;
	char b;
	fd = open("resources/messages/help.txt", O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "Could not find help.txt\n");
		return;
	}
	while (read(fd, &b, sizeof(char)))
	{
		if (b != '>' && b != '<')
		{
			printf("%c", b);
		}
	}
	close(fd);
}

int main(int argc, char *argv[])
{
	init_config(&config);

	int opt, opt_index;
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"remote", required_argument, NULL, 'r'},
		{NULL, 0, NULL, 0}};

	while ((opt = getopt_long(argc, argv, "hr:", long_opt, &opt_index)) != -1)
	{
		switch (opt)
		{
		case 'h':
			show_help();
			return 0;
			break;
		case 'r':
			config.maxClients = 0;
			strncpy(config.ip, optarg, 15);
			printf("Run as client, looking for server at \"%s\"\n", config.ip);
			break;
		case ':':
		case '?':
			printf("Type %s --help | %s -h for help\n", argv[0], argv[0]);
			exit(0);
			break;
		}
	}

	return SnakeApp_run(&config);
}
