CC=g++
CPPFLAGS= -Wall -std=c++11 -ggdb
LDLIBS= -lm -lX11 -lpthread

main: main.o kdtree.o obj.o prim.o mtl.o setup.o
main.o: main.cc
obj.o: obj.cc obj.hh
kdtree.o: kdtree.cc kdtree.hh
prim.o: prim.cc prim.hh
mtl.o: mtl.cc mtl.hh
setup.o: setup.cc setup.hh
clean:
	rm -f *.o main
