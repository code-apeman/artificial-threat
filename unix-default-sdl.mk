###############################################################################################################################################
# Makefile configuration for a generic UNIX-like OS (GNU/Linux, BSD, Solaris, etc.) default build (dynamically-linked on most UNIX-like OSes) #
###############################################################################################################################################

# Aseprite/LibreSprite binary
ASEPRITE = aseprite
# the version of strip from binutils, uncomment to use
#STRIP = strip
# the compiler to use for making the object files
CC = cc
# the compiler to use for linking everything together
CCLD = $(CC)
# Backend to use (SDL (SDL2) and ALLEGRO (Allegro v4.2.3.1 + MikMod) are supported)
BACKEND = SDL
# flags to use with $(CC)
#CFLAGS = -g -O2 -pipe # for a release build, also enable strip
CFLAGS = -g -O0 -Wall # for a debug build
# flags to use with $(CCLD)
LDFLAGS =
# output file name
OUTFILE = game
# additional parameter(s) for the compiler to find backend header files
INCLUDES = 
# additional parameter(s) for the linker to link against backend libraries
LIBS = -lSDL2_gfx -lSDL2_image -lSDL2_ttf -lSDL2_mixer
