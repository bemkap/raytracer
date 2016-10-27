CFLAGS= -Wall -std=c11 -ggdb `pkg-config --cflags glib-2.0 sdl2`
LDLIBS= `pkg-config --libs glib-2.0 sdl2` -lm

main: main.o kdtree.o parser.o vec3.o
main.o: main.c
vec3.o: vec3.c vec3.h
parser.o: parser.c parser.h
kdtree.o: kdtree.c kdtree.h
clean:
	rm -f *.o main
