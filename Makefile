include config.mk

OBJS = graphics.o error.o game.o main.o

all: $(addprefix obj/, $(OBJS))
	$(CCLD) $(LDFLAGS) -o $(OUTFILE) $^ $(ALLEGRO_LIBS)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o $(ALLEGRO_INCLUDES)

obj/game.o: src/game.c
	$(CC) $(CFLAGS) -c src/game.c -o obj/game.o $(ALLEGRO_INCLUDES)

obj/graphics.o: src/graphics.c
	$(CC) $(CFLAGS) -c src/graphics.c -o obj/graphics.o $(ALLEGRO_INCLUDES)

obj/error.o: src/error.c
	$(CC) $(CFLAGS) -c src/error.c -o obj/error.o $(ALLEGRO_INCLUDES)

clean:
	rm -f obj/* $(OUTFILE)

