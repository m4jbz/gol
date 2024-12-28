CC=gcc
CFLAGS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Wextra

gol: gol.c
	$(CC) -o gol gol.c $(CFLAGS)

