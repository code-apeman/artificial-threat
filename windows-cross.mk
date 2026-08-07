#############################################################################################################################
# Makefile configuration for a Windows cross-build on a generic UNIX-like OS (GNU/Linux, BSD, Solaris, etc.) or Cygwin host #
#############################################################################################################################

# Aseprite/LibreSprite binary
ASEPRITE = aseprite
# the version of strip from binutils, uncomment to use
STRIP = i486-w64-mingw32-strip
# the compiler to use for making the object files
CC = i486-w64-mingw32-cc
# the compiler to use for linking everything together
CCLD = $(CC)
# flags to use with $(CC)
CFLAGS = -g -O2 -pipe
# flags to use with $(CCLD)
LDFLAGS =
# output file name
OUTFILE = game
# additional header file director(y/ies) for Allegro
BACKEND_INCLUDES =
# additional parameter(s) for the linker to link against Allegro libraries
BACKEND_LIBS = -lalleg
