include config.mk

CODEFILES = graphics error game main
SPRITES = natsuki_walk

all: game $(addprefix obj/spr/, $(addsuffix .bmp, $(SPRITES)))

game: $(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES)))
	$(CCLD) $(LDFLAGS) -o $(OUTFILE) $^ $(ALLEGRO_LIBS)

obj/bin/main.o: src/main.c obj
	$(CC) $(CFLAGS) -c src/main.c -o obj/bin/main.o $(ALLEGRO_INCLUDES)

obj/bin/game.o: src/game.c obj
	$(CC) $(CFLAGS) -c src/game.c -o obj/bin/game.o $(ALLEGRO_INCLUDES)

obj/bin/graphics.o: src/graphics.c obj
	$(CC) $(CFLAGS) -c src/graphics.c -o obj/bin/graphics.o $(ALLEGRO_INCLUDES)

obj/bin/error.o: src/error.c obj
	$(CC) $(CFLAGS) -c src/error.c -o obj/bin/error.o $(ALLEGRO_INCLUDES)

obj/spr/natsuki_walk.bmp: sprites/natsuki/natsuki_walk.ase
	$(ASEPRITE) -b --sheet obj/spr/natsuki_walk.bmp --sheet-type horizontal $^

obj/bin: obj
	mkdir obj/bin

obj/spr: obj
	mkdir obj/spr

obj:
	mkdir obj

clean:
	rm -f obj/* $(OUTFILE)

