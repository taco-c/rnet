# HELP

## socket.h

### Functions
```c
/* Create a new socket of type TYPE in domain DOMAIN, using
   protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
   Returns a file descriptor for the new socket, or -1 for errors.  */

int socket(int domain, int type, int protocol);


/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors. */

int connect(int fd, const struct sockaddr *addr, socklen_t addrlen);


/* When a socket is created with socket(2), it exists in a name space
   (address family) but has no address assigned to it. bind() assigns
   the address specified by addr to the socket referred to by
   the file descriptor sockfd. addrlen specifies the size, in bytes, of
   the address structure pointed to by addr. Traditionally, this operation is
   called “assigning a name to a socket”. */

int bind (int fd, const struct sockaddr *addr, socklen_t len);



ssize_t recv(int fd, void *buf, size_t len, int flags);


/* listen() marks the socket referred to by sockfd as a passive socket,
   that is, as a socket that will be used to accept incoming connection
   requests using accept(2).
   
   The sockfd argument is a file descriptor that refers to a socket of type
   SOCK_STREAM or SOCK_SEQPACKET.
   
   The backlog argument defines the maximum length to which the queue of
   pending connections for sockfd may grow. If a connection request arrives
   when the queue is full, the client may receive an error with an indication
   of ECONNREFUSED or, if the underlying protocol supports retransmission,
   the request may be ignored so that a later reattempt at connection
   succeeds. */

int listen(int fd, int backlog);


int accept(int fd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
```

### Definitions
```c
/* Address families.  */
...
#define AF_INET 2  /* IP Address family. */
```

```c
/* Types of sockets.  */
enum __socket_type {
  SOCK_STREAM    =  1, /* Sequenced, reliable, connection-based byte streams. */
  SOCK_DGRAM     =  2, /* Connectionless, unreliable datagrams
                       of fixed maximum length.  */
  SOCK_RAW       =  3, /* Raw protocol interface.  */
  SOCK_RDM       =  4, /* Reliably-delivered messages.  */
  SOCK_SEQPACKET =  5, /* Sequenced, reliable, connection-based,
                          datagrams of fixed maximum length.  */
  SOCK_DCCP      =  6, /* Datagram Congestion Control Protocol.  */
  SOCK_PACKET    = 10, /* Linux specific way of getting packets
                          at the dev level.  For writing rarp and
                          other similar things on the user level. */

  /* Flags to be ORed into the type parameter of socket and socketpair and
     used for the flags parameter of paccept.  */

  SOCK_CLOEXEC  = 02000000, /* Atomically set close-on-exec flag for the
                              new descriptor(s). */
  SOCK_NONBLOCK = 00004000  /* Atomically mark descriptor(s) as non-blocking. */
};

#define SOCK_STREAM    SOCK_STREAM
#define SOCK_DGRAM     SOCK_DGRAM
#define SOCK_RAW       SOCK_RAW
#define SOCK_RDM       SOCK_RDM
#define SOCK_SEQPACKET SOCK_SEQPACKET
#define SOCK_DCCP      SOCK_DCCP
#define SOCK_PACKET    SOCK_PACKET
#define SOCK_CLOEXEC   SOCK_CLOEXEC
#define SOCK_NONBLOCK  SOCK_NONBLOCK
```

## netinet/in.h
### Definitions
```c
typedef unsigned short sa_family_t;
typedef unsigned short in_port_t;
typedef uint32_t       in_addr_t;

/* Address to accept any incoming messages. */
#define INADDR_ANY ((in_addr_t) 0x00000000)

```

### Structures
```c
struct in_addr {
    in_addr_t s_addr; /* load with inet_pton() */
};


/* Structure describing an Internet socket address.  */

struct sockaddr {
    sa_family_t sa_family;   /* Address family, AF_xxx */
    char        sa_data[14]; /* 14 bytes of protocol address */
};


/* Structure describing an Internet socket address.  */

struct sockaddr_in {
    short          sin_family;  /* e.g. AF_INET, AF_INET6 */
    in_port_t      sin_port;    /* e.g. htons(3490) */
    struct in_addr sin_addr;    /* see struct in_addr, below */
    unsigned char  sin_zero[8]; /* Pad to size of `struct sockaddr'.  */
};

struct sockaddr_in {
    short          sin_family;  /* e.g. AF_INET, AF_INET6 */
    in_port_t      sin_port;    /* e.g. htons(3490) */
    struct in_addr sin_addr {   /* see struct in_addr, below */
        in_addr_t s_addr;       /* load with inet_pton() */
    };
    unsigned char  sin_zero[8]; /* Pad to size of `struct sockaddr'.  */
};
```

## netdb.h
### Functions

```c
/* Translate name of a service location and/or a service name to set of
   socket addresses. */

int getaddrinfo (const char *restrict            node,
                 const char *restrict            service,
                 const struct addrinfo *restrict hints,
                 struct addrinfo **restrict      res);


/* Translate a socket address to a location and service name. */

int getnameinfo(const struct sockaddr *restrict addr, socklen_t addrlen,
                char *restrict host, socklen_t hostlen,
                char *restrict serv, socklen_t servlen, int flags);

/* Convert error return from getaddrinfo() to a string.  */
const char *gai_strerror (int errcode);
```

### Definitions
```c
/* Possible values for `ai_flags' field in `addrinfo' structure.  */
...
#define AI_PASSIVE 0x0001 /* Socket address is intended for `bind'. */
```

### Structures
```c
/* Structure to contain information about address of a service provider. */

struct addrinfo {
    int              ai_flags;     /* Input flags. */
    int              ai_family;    /* Protocol family for socket. */
    int              ai_socktype;  /* Socket type. */
    int              ai_protocol;  /* Protocol for socket. */
    socklen_t        ai_addrlen;   /* Length of socket address. */
    struct sockaddr *ai_addr;      /* Socket address for socket. */
    char            *ai_canonname; /* Canonical name for service location. */
    struct addrinfo *ai_next;      /* Pointer to next in list. */
};
```
