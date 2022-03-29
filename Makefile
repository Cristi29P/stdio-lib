CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -g

all: build

build: so_stdio

so_stdio: so_stdio.o

so_stdio.o: so_stdio.c so_stdio.h

.PHONY: clean

clean:
	rm -f *.o *~ so_stdio