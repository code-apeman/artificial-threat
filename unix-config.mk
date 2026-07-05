# Aseprite/LibreSprite binary
ASEPRITE = aseprite
# the version of strip from binutils, uncomment to use
#STRIP = strip
# the compiler to use for making the object files
CC = cc
# the compiler to use for linking everything together
CCLD = $(CC)
# flags to use with $(CC)
#CFLAGS = -g -O2 -pipe # for a release build, also enable strip
CFLAGS = -g -O0 -Wall # for a debug build
# flags to use with $(CCLD)
LDFLAGS =
# output file name
OUTFILE = game
# additional header file director(y/ies) for Allegro
ALLEGRO_INCLUDES =
# additional parameter(s) for the linker to link against Allegro libraries
ALLEGRO_LIBS = -lalleg
# additional header file director(y/ies) for libopenmpt
LIBOPENMPT_INCLUDES =
# additional parameter(s) for the linker to link against libopenmpt libraries
LIBOPENMPT_LIBS = -lopenmpt
