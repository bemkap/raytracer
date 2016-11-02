CC=g++
CPPFLAGS= -Wall -std=c++11 -ggdb `pkg-config --cflags glib-2.0 sdl2`
LDLIBS= `pkg-config --libs glib-2.0 sdl2` -lm

main: main.o kdtree.o parser.o
main.o: main.cc main.hh
parser.o: parser.cc parser.hh
kdtree.o: kdtree.cc kdtree.hh
clean:
	rm -f *.o main
