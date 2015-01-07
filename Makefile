# ------------------------------------------------
# Makefile
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = delaunay

# compiler
CC       = gcc

# compiling flags here
CFLAGS   = -std=gnu99
#-I.

# linking flags here
LFLAGS   = -Wall -I.

# using OpenGL lib
#GL_INCLUDE    = -I/usr/include/GL
GL_LIBDIR    = -L. -L/usr/X11R6/lib
GL_LIBRARIES = -lglut -lGL -lGLU -lm
#-lXmu -lXext -lXi -lX11

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

# change these to set the proper directories where each files shoould be
SRCDIR   = SRC
OBJDIR   = OBJ
BINDIR   = BIN
DOCDIR   = DOC
RAPDIR   = RAPPORT

RAPPORT  = rapport_delaunay_SAINLOT 

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -rf
ln       = ln -sf

$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo -n "Linking ... "
	@mkdir -p $(BINDIR)
	@$(CC) $(LFLAGS) $(OBJECTS) $(GL_LIBDIR) $(GL_LIBRARIES) -o $@ 
	@$(ln) $(BINDIR)/$(TARGET)
	@echo "OK"
	@echo "You can now run ./"$(TARGET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo -n "Compilation de "$<" ... "
	@mkdir -p $(OBJDIR) 
	@$(CC) $(CFLAGS) $(GL_INCLUDE) -c $< -o $@
	@echo "OK"

all: $(BINDIR)/$(TARGET) dox rapport

clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete !"

remove: clean
	@$(rm) $(BINDIR)
	@$(rm) $(TARGET)
	@$(rm) $(OBJDIR)
	@$(rm) $(DOCDIR)
	@echo "Executable removed !"

dox:
	@echo -n "Génération de la documentation ... "
	@$(rm) $(DOCDIR)
	@doxygen Doxyfile >/dev/null
	@echo "OK"

rapport:
	@echo -n "Génération du rapport ... "
	@cd $(RAPDIR) && pdflatex $(RAPPORT).tex >/dev/null
	@$(rm) $(RAPPORT).aux $(RAPPORT).log
	@echo "OK"