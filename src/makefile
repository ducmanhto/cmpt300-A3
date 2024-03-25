CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build: list.o
	gcc $(CFLAGS) main.c pcb.c queues.c list.o -o simulator

run: build
	./simulator

valgrind: build
	valgrind --leak-check=full ./simulator

clean: 
	rm -f s-talk