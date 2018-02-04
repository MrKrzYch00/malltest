CC = gcc

CFLAGS = -W -Wall -Wextra -ansi -pedantic -pthread

DEFOPT = -Ofast -fno-associative-math

BIN_SRC = src/malltest/malltest.c

.PHONY: malltest

malltest:
	$(CC) $(BIN_SRC) $(CFLAGS) $(DEFOPT) -o malltest

clean:
	rm -f malltest