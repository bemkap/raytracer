CFLAGS= -std=c11

main: main.o kdtree.o parser.o
main.o: main.c
parser.o: parser.c parser.h
kdtree.o: kdtree.c kdtree.h
clean:
	rm -f *.o main
