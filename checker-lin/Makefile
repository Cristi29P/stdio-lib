CC = gcc
CFLAGS = -Wall -Wextra -fPIC

all: build

build: libso_stdio.so

libso_stdio.so: so_stdio.o useful.o
		$(CC) so_stdio.o useful.o -shared -o libso_stdio.so
so_stdio.o: so_stdio.c

useful.o: useful.c

debug: so_stdio.c useful.c
		$(CC) testare.c so_stdio.c useful.c -o testare

.PHONY: clean

clean:
	rm -f *.o *~ libso_stdio.so