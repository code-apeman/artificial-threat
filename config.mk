# Aseprite/LibreSprite binary
ASEPRITE = libresprite
# the compiler to use for making the object files
CC = cc
# the compiler to use for linking everything together
CCLD = $(CC)
# flags to use with $(CC)
CFLAGS = -g -O2 -pipe
# flags to use with $(CCLD)
LDFLAGS =
# output file name
OUTFILE = game
# additional header file director(y/ies) for Allegro
ALLEGRO_INCLUDES =
# additional parameter(s) for Allegro libraries
ALLEGRO_LIBS = -lalleg
