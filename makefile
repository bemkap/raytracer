CC= g++
CPPFLAGS= -Wall -std=c++11 -ggdb
LDLIBS= -lX11 -lpthread
SRCDIR= src
OBJDIR= obj

main: $(addprefix $(OBJDIR)/,main.o kdtree.o obj.o prim.o mtl.o setup.o)
	$(CC) $(LDLIBS) -o main $^
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) $(CPPFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJDIR)/*.o main
