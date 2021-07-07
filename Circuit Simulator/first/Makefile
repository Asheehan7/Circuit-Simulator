CC = gcc

all:first

first: first.c
	$(CC) -Wall -Werror -fsanitize=address first.c -o first -lm -O3

clean:
	rm -rf first
