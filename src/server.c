#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> /* close */
#include <string.h> /* strlen */

/* Socket specific */
#include <arpa/inet.h>  /* inet_ntop */
#include <netinet/in.h> /* htons */
#include <netdb.h>      /* getaddrinfo, gai_strerror, struct addrinfo */
#include <sys/socket.h> /* socket, connect, bind */

#include "server.h"

#define READ_BUFFER_SIZE 4096

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif

typedef struct client {
	int                     fd;
	struct sockaddr_storage addr;
	socklen_t               addr_len;
} Client;


struct server *server_create(char *port, unsigned int backlog) {
	struct addrinfo hints;
	int status;
	struct addrinfo *addrinfos, *addrinfo;
	struct server *server = malloc(sizeof (struct server));
	server->backlog      = (int)backlog;
	server->is_listening = 0;

	memset(&hints, 0, sizeof (hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;

	status = getaddrinfo(NULL, port, &hints, &addrinfos);
	if(status < 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return NULL;
	}

	for (addrinfo = addrinfos; addrinfo != NULL; addrinfo = addrinfo->ai_next) {
		server->fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
		if (server->fd < 0) {
			continue; /* Try again */
		}
		if (bind(server->fd, addrinfo->ai_addr, addrinfo->ai_addrlen) == 0) {
			break; /* Success */
		}
		close(server->fd);
	}
	freeaddrinfo(addrinfos);

	if (addrinfo == NULL) { /* No address succeeded */
		fprintf(stderr, "Could not start server on %d:%s\n",
				(unsigned int)((struct sockaddr_in *)addrinfo->ai_addr)->sin_addr.s_addr,
				port);
		return NULL;
	}

	printf("Successfully started server on port %s\n", port);

	return server;
}


void server_destroy(struct server *server) {
	close(server->fd);
	free(server);
}


int server_listen(struct server *server) {
	Client client;
	char read_buffer[READ_BUFFER_SIZE];
	int read;
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	int status;
	char *server_message = "HTTP/1.1 200 OK\r\n"
	                       "Server: rnet\r\n"
	                       "Content-Type: text/html\r\n"
	                       "\r\n"
	                       "<b>rnet</b>\r\n"
	                       "\r\n";

	/* Listen and accept */
	server->is_listening = 1;
	if (listen(server->fd, server->backlog) < 0) {
		return -1;
	}
	client.addr_len = sizeof client.addr;

	while(server->is_listening) {
		client.fd = accept(server->fd,
		                   (struct sockaddr *)&client.addr,
		                   &client.addr_len);

		if (client.fd < 0) {
			fprintf(stderr , "could not accept client socket\n");
			continue;
		}

		/* Read */
		read = recv(client.fd, read_buffer, sizeof read_buffer, 0);

		/* Get client nameinfo */
		status = getnameinfo((struct sockaddr *)&client.addr, client.addr_len,
							 host,                            NI_MAXHOST,
							 service,                         NI_MAXSERV,
							 NI_NUMERICSERV);
		if (status < 0) {
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(status));
		}

		/* Action */
		printf("Received %d bytes from %s:%s\n", read, host, service);
		printf("%s\n", read_buffer);
		printf("%s\n", server_message);

		send(client.fd, server_message, strlen(server_message), 0);
		close(client.fd);
	}

	return 1;
}
