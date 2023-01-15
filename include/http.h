#ifndef RNET_HTTP_H
#define RNET_HTTP_H


#define STATUS_OK 200


/* HandleFunc registers the handler function for the given pattern in
   the DefaultServeMux. The documentation for ServeMux explains how patterns
   are matched. */
void handle_func(struct server *server,
                 char *target,
                 void (*handler)(struct server *, struct client *));


/* ListenAndServe listens on the TCP network address addr and then calls
   Serve with handler to handle requests on incoming connections.
   Accepted connections are configured to enable TCP keep-alives. */
void listen_and_serve(struct server *server, char *port);


#endif
