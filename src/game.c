#include "includes.h"
#include "game.h"
#include "graphics.h"
#include "sound.h"
#include "error.h"

DATAFILE *music, *sprites;
BITMAP *natsuki_spritesheet;
extern bool game_exit_flag;

void game_init() {      // initialization routine
    music = load_datafile("music.dat");
    sprites = load_datafile("sprites.dat");
    if (!music) handle_init_error("Could not load music.dat", "game_init() (game.c)");
    if (!sprites) handle_init_error("Could not load sprites.dat", "game_init() (game.c)");
    graphics_init();
    sound_init();
    DATAFILE *title_theme = find_datafile_object(music, "TITLE_IT");
    SAMPLE *title_theme_pcm = allegro_sample_from_module(title_theme->dat, title_theme->size);
    play_sample(title_theme_pcm, 75, 128, 1000, 1);
    natsuki_spritesheet = find_datafile_object(sprites, "NATSUKI_WALK_BMP")->dat;
    set_palette(default_palette);
}

void game_input() {     // input collection and processing

}

void game_logic() {     // everything else

}

void game_draw() {      // drawing the frame
    blit(natsuki_spritesheet, screen, 0, 0, (SCREEN_W-natsuki_spritesheet->w)/2,
        (SCREEN_H-natsuki_spritesheet->h)/2, natsuki_spritesheet->w, natsuki_spritesheet->h);
}

void game_shutdown() {  // final farewells
    unload_datafile(music);
    unload_datafile(sprites);
}
