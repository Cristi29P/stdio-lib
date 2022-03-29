CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -g

all: build

build: so_lib

so_lib: so_lib.o

so_lib.o: so_lib.c so_lib.h

.PHONY: clean

clean:
	rm -f *.o *~ so_lib