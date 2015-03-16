CC=g++
FLAGS=-std=c++1y -g -Werror -Wall -Wextra -O3

all:		reverse_c mmap_reverse buffered_reverse

reverse_c:	reverse.c
		gcc -std=c99 -Werror -Wall -O3 -o reverse_c reverse.c

mmap_reverse:	mmap_reverse.c
		gcc -std=c99 -Werror -Wall -O3 -o mmap_reverse mmap_reverse.c

buffered_reverse:	buffered_reverse.cc
		$(CC) $(FLAGS) -o buffered_reverse buffered_reverse.cc
