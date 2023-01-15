INCLUDE = include
CFLAGS = -I$(INCLUDE) -D_POSIX_C_SOURCE=200809L -std=c89 -pedantic-errors -Werror -Wall -Wextra

build:
	gcc -o bin/rnet main.c src/server.c $(CFLAGS)

