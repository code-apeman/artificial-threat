CONFIGFILE := config.mk
include ${CONFIGFILE}

# Backend-independent code files
CODEFILES = physics game main archive
# Backend Dependent code files
BD_CODEFILES = graphics sound error input yield
ifeq ($(BACKEND),ALLEGRO)
  BD_CODEFILES += ibxm
endif
# Self-explanatory
SPRITES = natsuki_walk natsuki_run natsuki_jump natsuki_fall
BACKGROUNDS = bg_tokyo
TILES = road road_slope pillar
# Music module files
MODFILES = title.xm void.xm deathomen.xm zoloft.xm

all: no-cleanup
	@echo "Cleaning up intermediate build artifacts... (use \"make no-cleanup\" to retain those)"
	@rm -rf obj

no-cleanup: $(OUTFILE) music.anw sprites.anw bgs.anw tiles.anw

$(OUTFILE): $(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES))) $(addprefix obj/bin/, $(addsuffix .o, $(BD_CODEFILES)))
	@echo "CCLD $@"
	@$(CCLD) $(LDFLAGS) -o $@ $^ $(LIBS)
ifdef STRIP
	@echo "STRIP $@"
	@$(STRIP) $(OUTFILE)
endif

music.anw: $(addprefix music/, $(MODFILES)) obj/tools/anw_pack
	@cd music && echo "ANWPACK $@" && ../obj/tools/anw_pack ../music.anw $(MODFILES)

sprites.anw: $(addprefix obj/spr/, $(addsuffix .bmp, $(SPRITES))) obj/tools/anw_pack
	@cd obj/spr && echo "ANWPACK $@" && ../tools/anw_pack ../../sprites.anw $(addsuffix .bmp, $(SPRITES))

bgs.anw: $(addprefix obj/bg/, $(addsuffix .bmp, $(BACKGROUNDS))) obj/tools/anw_pack
	@cd obj/bg && echo "ANWPACK $@" && ../tools/anw_pack ../../bgs.anw $(addsuffix .bmp, $(BACKGROUNDS))

tiles.anw: $(addprefix obj/tile/, $(addsuffix .bmp, $(TILES))) obj/tools/anw_pack
	@cd obj/tile && echo "ANWPACK $@" && ../tools/anw_pack ../../tiles.anw $(addsuffix .bmp, $(TILES))

$(addprefix obj/bin/, $(addsuffix .o, $(CODEFILES))):
	@mkdir -p $(@D)
	@echo "CC $(subst obj/bin/,src/,$(subst .o,.c,$@))"
	@$(CC) $(CFLAGS) -c $(subst obj/bin/,src/,$(subst .o,.c,$@)) -o $@ $(INCLUDES) -DUSE_$(BACKEND)

$(addprefix obj/bin/, $(addsuffix .o, $(BD_CODEFILES))):
	@mkdir -p $(@D)
	@echo "CC $(subst obj/bin/,src/$(BACKEND)/,$(subst .o,.c,$@))"
	@$(CC) $(CFLAGS) -c $(subst obj/bin/,src/$(BACKEND)/,$(subst .o,.c,$@)) -o $@ $(INCLUDES)

$(addprefix obj/spr/, $(addsuffix .bmp, $(SPRITES))):
	@mkdir -p $(@D)
	@echo "ASEPRITE $@"
	@$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $(subst obj/spr/,sprites/,$(subst .bmp,.ase,$@)) >/dev/null

$(addprefix obj/bg/, $(addsuffix .bmp, $(BACKGROUNDS))):
	@mkdir -p $(@D)
	@echo "ASEPRITE $@"
	@$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $(subst obj/bg/,backgrounds/,$(subst .bmp,.ase,$@)) >/dev/null

$(addprefix obj/tile/, $(addsuffix .bmp, $(TILES))):
	@mkdir -p $(@D)
	@echo "ASEPRITE $@"
	@$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $(subst obj/tile/,tiles/,$(subst .bmp,.ase,$@)) >/dev/null

obj/tools/anw_pack:
	@mkdir -p $(@D)
	@echo "CC $@"
	@$(CC) -g -O3 -pipe tools/anw_pack.c -o obj/tools/anw_pack

clean:
	@echo "Cleaning up"
	@rm -rf obj *.anw $(OUTFILE)

love:
	@echo "Why be a war criminal when you can be GAY?"
