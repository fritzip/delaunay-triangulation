CFLAGS = -g -Wall -std=gnu99
#GL_INCLUDE    = -I/usr/include/GL
GL_LIBDIR    = -L. -L/usr/X11R6/lib
GL_LIBRARIES = -lglut -lGL -lm -lGLU 
#-lXmu -lXext -lXi -lX11
CC=gcc
OS = "unknown"

# Add objectiveC library for MacOSX.
ifneq ($(strip $(shell $(CC) -v 2>&1 | grep -i "Apple")),)
 GL_INCLUDE    = -I/usr/X11R6/include
 GL_LIBRARIES += -lobjc
 BROWSER = dillo
 OS = "osx"
endif
# Nothing to be done for Linux :o)
ifneq ($(strip $(shell $(CC) -v 2>&1 | grep -i "Linux")),)
 OS = "linux"
 BROWSER = mozilla
endif

exe: main.o linked_list.o print_fn.o geoalgo.o math_fn.o 
	$(CC) -o $@ $^ $(GL_LIBRARIES) $(CFLAGS) $(GL_LIBDIR)

main.o: main.c main.h
	$(CC) -c main.c $(GL_INCLUDE) $(CFLAGS)

linked_list.o: linked_list.c linked_list.h
	$(CC) -c $< $(GL_INCLUDE) $(CFLAGS)

print_fn.o: print_fn.c print_fn.h
	$(CC) -c $< $(GL_INCLUDE) $(CFLAGS)

math_fn.o: math_fn.c math_fn.h
	$(CC) -c $< $(GL_INCLUDE) $(CFLAGS)

geoalgo.o: geoalgo.c geoalgo.h
	$(CC) -c $< $(GL_INCLUDE) $(CFLAGS)

#
clean:
	@echo "operating system = $(OS)"
	rm -rf *.o
