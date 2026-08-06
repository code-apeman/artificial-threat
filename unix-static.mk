#############################################################################################################
# Makefile configuration for a generic UNIX-like OS (GNU/Linux, BSD, Solaris, etc.) statically-linked build #
#############################################################################################################

# Aseprite/LibreSprite binary
ASEPRITE = aseprite
# the version of strip from binutils, uncomment to use
STRIP = strip
# the compiler to use for making the object files
CC = cc
# the compiler to use for linking everything together
CCLD = $(CC)
# flags to use with $(CC)
CFLAGS = -g -O2 -pipe
# flags to use with $(CCLD)
LDFLAGS = -Lstatic
# output file name
OUTFILE = game
# backend being used (SDL or ALLEGRO)
BACKEND = SDL
# additional header file director(y/ies) for Allegro
ALLEGRO_INCLUDES =
# additional parameter(s) for the linker to link against Allegro libraries
ALLEGRO_LIBS = -lalleg
# additional header file director(y/ies) for libopenmpt
LIBOPENMPT_INCLUDES =
# additional parameter(s) for the linker to link against libopenmpt libraries
LIBOPENMPT_LIBS = -lopenmpt
