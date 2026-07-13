CONFIGFILE := config.mk
include ${CONFIGFILE}

LIBS = $(ALLEGRO_LIBS) $(LIBOPENMPT_LIBS)
INCLUDES = $(ALLEGRO_INCLUDES) $(LIBOPENMPT_INCLUDES)

CODEFILES = graphics sound error physics game main
SPRITES = natsuki_walk natsuki_run natsuki_jump natsuki_fall
BACKGROUNDS = bg_tokyo
TILES = road road_slope pillar
MODFILES = title.it doomsday.it deathomen.it zoloft.it

all: no-cleanup
	@echo "Cleaning up intermediate build artifacts... (use \"make no-cleanup\" to retain those)"
	@rm -rf obj

no-cleanup: $(OUTFILE) music.dat sprites.dat bgs.dat tiles.dat

$(OUTFILE): $(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES)))
	@echo "CCLD $@"
	@$(CCLD) $(LDFLAGS) -o $@ $^ $(LIBS)
ifdef STRIP
	@echo "STRIP $@"
	@$(STRIP) $(OUTFILE)
endif

music.dat: $(addprefix music/, $(MODFILES))
	@echo "DAT $@"
	@dat -a -c2 music.dat $^

sprites.dat: $(addprefix obj/spr/, $(addsuffix .bmp, $(SPRITES)))
	@echo "DAT $@"
	@dat -a -c2 -t BMP sprites.dat $^

bgs.dat: $(addprefix obj/bg/, $(addsuffix .bmp, $(BACKGROUNDS)))
	@echo "DAT $@"
	@dat -a -c2 -t BMP bgs.dat $^

tiles.dat: $(addprefix obj/tile/, $(addsuffix .bmp, $(TILES)))
	@echo "DAT $@"
	@dat -a -c2 -t BMP tiles.dat $^

$(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES))):
	@mkdir -p $(@D)
	@echo "CC $(subst obj/bin/,src/,$(subst .o,.c,$@))"
	@$(CC) $(CFLAGS) -c $(subst obj/bin/,src/,$(subst .o,.c,$@)) -o $@ $(INCLUDES)

$(addprefix obj/spr/, $(addsuffix .bmp, $(SPRITES))):
	@mkdir -p $(@D)
	@echo "SPRITE $@"
	@$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $(subst obj/spr/,sprites/,$(subst .bmp,.ase,$@)) >/dev/null

obj/bg/bg_tokyo.bmp: backgrounds/bg_tokyo.ase
	@mkdir -p $(@D)
	@echo "BACKGROUND $@"
	@$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $^ >/dev/null

$(addprefix obj/tile/, $(addsuffix .bmp, $(TILES))):
	@mkdir -p $(@D)
	@echo "TILE $@"
	@$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $(subst obj/tile/,tiles/,$(subst .bmp,.ase,$@)) >/dev/null

clean:
	@echo "Cleaning up"
	@rm -rf obj *.dat $(OUTFILE)

love:
	@echo "Not war?"
