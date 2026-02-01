include config.mk

LIBS = $(ALLEGRO_LIBS) $(LIBOPENMPT_LIBS)
INCLUDES = $(ALLEGRO_INCLUDES) $(LIBOPENMPT_INCLUDES)

CODEFILES = graphics sound error game main
SPRITES = natsuki_walk
MODFILES = title.it

all: game music.dat sprites.dat

game: $(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES)))
	$(CCLD) $(LDFLAGS) -o $(OUTFILE) $^ $(LIBS)

music.dat: $(addprefix music/, $(MODFILES))
	dat -a -c2 music.dat $^

sprites.dat: $(addprefix obj/spr/, $(addsuffix .bmp, $(SPRITES)))
	dat -a -c2 sprites.dat obj/spr/*

obj/bin/main.o: src/main.c obj/bin
	$(CC) $(CFLAGS) -c src/main.c -o obj/bin/main.o $(INCLUDES)

obj/bin/game.o: src/game.c obj/bin
	$(CC) $(CFLAGS) -c src/game.c -o obj/bin/game.o $(INCLUDES)

obj/bin/graphics.o: src/graphics.c obj/bin
	$(CC) $(CFLAGS) -c src/graphics.c -o obj/bin/graphics.o $(INCLUDES)

obj/bin/sound.o: src/sound.c obj/bin
	$(CC) $(CFLAGS) -c src/sound.c -o obj/bin/sound.o $(INCLUDES)

obj/bin/error.o: src/error.c obj/bin
	$(CC) $(CFLAGS) -c src/error.c -o obj/bin/error.o $(INCLUDES)

obj/spr/natsuki_walk.bmp: sprites/natsuki/natsuki_walk.ase obj/spr
	$(ASEPRITE) -b --sheet obj/spr/natsuki_walk.bmp --sheet-type horizontal $^ >/dev/null

obj/bin: obj
	mkdir obj/bin

obj/spr: obj
	mkdir obj/spr

obj:
	mkdir obj

clean:
	rm -rf obj *.dat $(OUTFILE)

