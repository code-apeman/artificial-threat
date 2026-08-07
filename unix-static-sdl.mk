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
# Backend to use (SDL (SDL2) and ALLEGRO (Allegro v4.2.3.1 + MikMod) are supported)
BACKEND = SDL
# additional parameter(s) for the compiler to use backend headers
INCLUDES =
# additional parameter(s) for the linker to link against Allegro libraries
LIBS = -lSDL2_gfx -lSDL2_image -lSDL2_ttf -lSDL2_mixer
