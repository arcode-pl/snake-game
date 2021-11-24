#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include "include/snake_app.h"
#include "include/socket_util.h"

void show_help();

int main(int argc, char *argv[]) {

	int opt, opt_index;
    int is_server = 1;
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
        {"remote", required_argument, NULL, 'r'},
		{NULL, 0, NULL, 0}
	};

    while((opt = getopt_long(argc, argv, "hr:", long_opt, &opt_index)) != -1) {
		switch(opt) {
			case 'h':
				show_help();
				return 0;
				break;
            case 'r':
                printf("Run as client, looking for server at \"%s\"\n", optarg);
				if (SocketUtil_connect_to_server(optarg) != 0) {
                    exit(EXIT_FAILURE);
                }
                is_server = 0;
				break;
			case ':':
            case '?':
				printf("Type %s --help | %s -h for help\n", argv[0], argv[0]);
				exit(0);
				break;			
		}		
	}

    if (is_server) {
		printf("Run as server, looking for clients.\n");
        if (SocketUtil_create_server() != 0) {
            exit(EXIT_FAILURE);
        }
    }

	return SnakeApp_start(is_server);
}

void show_help(void) {
	int fd;
	char b;
	fd = open("resources/messages/help.txt", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Could not find help.txt\n");
		return;
	}
	while (read(fd, &b, sizeof(char))) {
		if(b != '>' && b != '<') {
			printf("%c", b);
		}
	}
	close(fd);
}
