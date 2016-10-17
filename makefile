main: main.c kdtree.o
	gcc -std=c11 -o main main.c kdtree.o

kdtree.o: kdtree.c kdtree.h
	gcc -std=c11 -c kdtree.c
