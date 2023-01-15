struct client {
	int socket;
};

struct client *client_create() {
	struct client *client = malloc(sizeof (struct client));
	client->socket = socket(AF_INET, SOCK_STREAM, 0);
	return client;
}


int client_connect(struct client *client, struct server *server) {
	struct sockaddr_in addr;
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(server->port);
	addr.sin_addr.s_addr = INADDR_ANY;
	return connect(client->socket, (struct sockaddr *)&addr, sizeof (addr));
}


int start_client() {
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr;
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof server_addr) < 0) {
		fprintf(stderr, "Error connecting\n");
		return -3;
	}

	char recv_buffer[256];
	int len = recv(socket_fd, &recv_buffer, sizeof recv_buffer, 0);

	printf("%s\n", recv_buffer);
	close(socket_fd);

	return 1;
}


char *get_ip(struct sockaddr *sockaddr) {
	struct sockaddr_storage *ip = (struct sockaddr_storage *)sockaddr;
	void *addr;
	char *ipstr = malloc(INET6_ADDRSTRLEN);

	if (sockaddr->sa_family == AF_INET) {
		addr = &(((struct sockaddr_in *)ip)->sin_addr);
	} else {
		addr = &(((struct sockaddr_in6 *)ip)->sin6_addr);
	}

	if (inet_ntop(sockaddr->sa_family, addr, ipstr, sizeof ipstr) == NULL) {
		fprintf(stderr, "Could not find ip.");
		return NULL;
	}

	return ipstr;
}
