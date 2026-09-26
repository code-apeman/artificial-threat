###############################################################################################################################################
# Makefile configuration for a generic UNIX-like OS (GNU/Linux, BSD, Solaris, etc.) default build (dynamically-linked on most UNIX-like OSes) #
###############################################################################################################################################

# Aseprite/LibreSprite binary
ASEPRITE = aseprite
# the version of strip from binutils, uncomment the commented line and
# comment the uncommented line to use
#STRIP = strip
STRIP = printf "Debug build; let's give %s some privacy, eh?\n"
# the compiler to use for making the object files
CC = gcc
# the compiler to use for linking everything together
CCLD = $(CC)
# Backend to use (SDL (SDL2) and ALLEGRO (Allegro v4.2.3.1 + IBXM) are supported)
BACKEND = SDL
# flags to use with $(CC)
#CFLAGS = -g -O2 -pipe # for a release build, also enable strip
CFLAGS = -g -O0 -Wall -pipe # for a debug build
# flags to use with $(CCLD)
LDFLAGS =
# output file name
OUTFILE = game
# additional parameter(s) for the compiler to find backend header files
INCLUDES = $(shell sdl2-config --cflags)
# additional parameter(s) for the linker to link against backend libraries
#LIBS = $(shell sdl2-config --libs)
LIBS = -L/nix/store/da69qm9bail6gvla94gdx29kk7gvlmvj-sdl2-compat-2.32.70/lib -lSDL2 -L/nix/store/31820n440745n9lx1vnrvqcbnw3w6nib-SDL2_mixer-2.8.2/lib -lSDL2_mixer -L/nix/store/0v0ysy5zl692gmybfxcmrsijnxjgvsnm-SDL2_image-2.8.12/lib -lSDL2_image # fuck nixos, all my homies hate nixos
