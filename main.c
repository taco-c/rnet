#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"


static struct server *server;


static void sigint_handler() {
	if (server) {
		printf("quitting\n");
		server->is_listening = 0;
	}
}


int start_server() {
	int err;
	server = server_create("8080", 32U);

	if (server == NULL) {
		return -1;
	}

	err = server_listen(server);
	if (err < 0) {
		return err;
	}

	server_destroy(server);

	return 1;
}


int main(int argc, char **argv) {
	int err;
	int i;

	if (argc < 2) {
		return EXIT_FAILURE;
	}

	signal(SIGINT, sigint_handler);

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') continue;

		switch (argv[i][1]) {
		case 's':
			err = start_server();
			break;
		}
	}

	if (err < 0) {
		fprintf(stderr, "Error %d\n", err);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
