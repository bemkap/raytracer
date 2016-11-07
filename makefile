CC=g++
CPPFLAGS= -Wall -std=c++11 -ggdb `pkg-config --cflags sdl2`
LDLIBS= `pkg-config --libs sdl2` -lm

main: main.o kdtree.o obj.o prim.o sdl.o mtl.o
main.o: main.cc
obj.o: obj.cc obj.hh
kdtree.o: kdtree.cc kdtree.hh
prim.o: prim.cc prim.hh
sdl.o: sdl.cc sdl.hh
mtl.o: mtl.cc mtl.hh
clean:
	rm -f *.o main
