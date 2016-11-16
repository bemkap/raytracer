CC=g++
CPPFLAGS= -Wall -std=c++11 -pg -ggdb
LDLIBS= -lm -pg

main: main.o kdtree.o obj.o prim.o mtl.o
main.o: main.cc
obj.o: obj.cc obj.hh
kdtree.o: kdtree.cc kdtree.hh
prim.o: prim.cc prim.hh
mtl.o: mtl.cc mtl.hh
clean:
	rm -f *.o main
