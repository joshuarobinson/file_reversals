CC=g++
FLAGS=-std=c++1y -g -Werror -Wall -Wextra -O3

all:		fwrite_reverse mmap_reverse pwrite_reverse

fwrite_reverse:	fwrite_reverse.cc
		$(CC) $(FLAGS) -o fwrite_reverse fwrite_reverse.cc

mmap_reverse:	mmap_reverse.cc
		$(CC) $(FLAGS) -o mmap_reverse mmap_reverse.cc

pwrite_reverse:	pwrite_reverse.cc
		$(CC) $(FLAGS) -o pwrite_reverse pwrite_reverse.cc
