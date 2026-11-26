###############################################################################################################################################
# Makefile configuration for a generic UNIX-like OS (GNU/Linux, BSD, Solaris, etc.) default build (dynamically-linked on most UNIX-like OSes) #
###############################################################################################################################################

# the version of strip from binutils, uncomment the commented line and
# comment the uncommented line to use
#STRIP = strip
STRIP = printf "Debug build; let's give %s some privacy, eh?\n"
# the compiler to use for making the object files
CC = gcc
# the compiler to use for linking everything together
CCLD = $(CC)
# Backend to use (SDL (SDL2) and ALLEGRO (Allegro v4.2.3.1 + IBXM) are supported)
BACKEND = WAYLAND
# flags to use with $(CC)
#CFLAGS = -g -O2 -pipe # for a release build, also enable strip
CFLAGS = -g -O0 -Wall -Wextra -Wpedantic -pipe # for a debug build
# flags to use with $(CCLD)
LDFLAGS =
# output file name
OUTFILE = game
# additional parameter(s) for the compiler to find backend header files
INCLUDES = $(shell pkgconf wayland-client --cflags)
# additional parameter(s) for the linker to link against backend libraries
LIBS = $(shell pkgconf wayland-client --libs)
