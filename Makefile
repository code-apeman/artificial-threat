include config.mk

LIBS = $(ALLEGRO_LIBS) $(LIBOPENMPT_LIBS)
INCLUDES = $(ALLEGRO_INCLUDES) $(LIBOPENMPT_INCLUDES)

CODEFILES = graphics sound error game main
SPRITES = natsuki_walk
BACKGROUNDS = bg_tokyo
TILES = road road_slope pillar
MODFILES = title.it doomsday.it deathomen.it

all: no-cleanup
	rm -rf obj

no-cleanup: $(OUTFILE) music.dat sprites.dat bgs.dat tiles.dat

$(OUTFILE): $(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES)))
	$(CCLD) $(LDFLAGS) -o $(OUTFILE) $^ $(LIBS)
ifdef STRIP
	$(STRIP) $(OUTFILE)
endif

music.dat: $(addprefix music/, $(MODFILES))
	dat -a -c2 music.dat $^

sprites.dat: $(addprefix obj/spr/, $(addsuffix .bmp, $(SPRITES)))
	dat -a -c2 -t BMP sprites.dat $^

bgs.dat: $(addprefix obj/bg/, $(addsuffix .bmp, $(BACKGROUNDS)))
	dat -a -c2 -t BMP bgs.dat $^

tiles.dat: $(addprefix obj/tile/, $(addsuffix .bmp, $(TILES)))
	dat -a -c2 -t BMP tiles.dat $^

$(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES))): obj/bin
	$(CC) $(CFLAGS) -c $(subst obj/bin/,src/,$(subst .o,.c,$@)) -o $@ $(INCLUDES)

obj/spr/natsuki_walk.bmp: sprites/natsuki/natsuki_walk.ase obj/spr
	$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $^ >/dev/null

obj/bg/bg_tokyo.bmp: backgrounds/bg_tokyo.ase obj/bg
	$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $^ >/dev/null

$(addprefix obj/tile/, $(addsuffix .bmp, $(TILES))): obj/tile
	$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $(subst obj/tile/,tiles/,$(subst .bmp,.ase,$@)) >/dev/null

obj/bin: obj
	mkdir obj/bin

obj/spr: obj
	mkdir obj/spr

obj/bg: obj
	mkdir obj/bg

obj/tile: obj
	mkdir obj/tile

obj:
	mkdir obj

clean:
	rm -rf obj *.dat $(OUTFILE)
