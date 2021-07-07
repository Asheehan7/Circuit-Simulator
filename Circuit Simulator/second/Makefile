CC = gcc

all:second

second: second.c
	$(CC) -Wall -Werror -fsanitize=address second.c -o second -lm -O3

clean:
	rm -rf second
