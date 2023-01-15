struct http_header {
	char           version[9];
	unsigned short status;
}

struct http_message {
	struct http_header *header;
	char                body[1];
};
