gol: gol.c
	cc -o gol gol.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Wextra
