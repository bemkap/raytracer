CC= g++
override CPPFLAGS+= -Wall -std=c++11 -ggdb
override LDLIBS+= -lX11 -lpthread
SRCDIR= src
OBJDIR= obj

main: $(addprefix $(OBJDIR)/,main.o kdtree.o obj.o ray.o mtl.o setup.o)
	$(CC) $(LDLIBS) -o main $^
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) $(CPPFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJDIR)/*.o main
