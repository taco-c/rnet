#ifndef RNET_SERVER_H
#define RNET_SERVER_H

#include <netdb.h> /* struct addrinfo */

/*typedef void server_t;*/
struct server {
	int                   fd;
	struct sockaddr_in    addr;
	int                   backlog;
	volatile unsigned int is_listening;
};


/* Creates a TCP server socket and binds it to _port_. */
struct server *server_create(char *port, unsigned int backlog);


/* Closes and frees the server structure. */
void server_destroy(struct server *server);
int server_listen(struct server *server);

#endif /* RNET_SERVER_H */
