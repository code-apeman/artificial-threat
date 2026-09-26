#include "includes.h"
#include "game.h"
#include "graphics.h"
#include "sound.h"
#include "error.h"
#include "input.h"
#include "physics.h"
#include "archive.h"
#include <string.h>

#define NATSUKI_FRAME_W 24
#define NATSUKI_FRAME_H 32
#define NATSUKI_FRAME_NUM_WALK 8
#define NATSUKI_FRAME_NUM_RUN 6
#define NATSUKI_FRAME_NUM_FALL 4
#define NATSUKI_FRAME_NUM_JUMP 1
#define NATSUKI_SPEED_NORMAL 1
#define NATSUKI_SPEED_SPRINT 3
#define NATSUKI_JUMP_SPEED 5

//DATAFILE *music, *sprites, *backgrounds, *tiles;
opened_anw_archive *music, *sprites, *backgrounds, *tiles;
//BITMAP *natsuki_spritesheet_walk, *natsuki_spritesheet_run, *natsuki_spritesheet_fall, *natsuki_spritesheet_jump, *natsuki_sprite;
//BITMAP *background;
//extern BITMAP *buffer;
extern bool game_exit_flag;
unsigned long long int frames = 0;

// Position, physics, etc.
hitbox natsuki_hitbox;
int natsuki_y_speed = 0;
bool natsuki_moving = false;

// Stats
unsigned int natsuki_mhp = 50, natsuki_atk = 2, natsuki_def = 0;
unsigned int natsuki_lv = 1, natsuki_exp = 0, natsuki_exp_to_next = 50;
unsigned int natsuki_max_air_jumps = 0;
int natsuki_hp = 50, natsuki_air_jumps = 0;
bool grounded = false;

// Scrolling
unsigned int camera_x = 0, camera_y = 0;

void game_init() {      // initialization routine
    graphics_init();
    sound_init();
    input_init();

    music = anw_open("music.anw");
    if (!music) handle_init_error("Could not open music.anw", "game_init() (game.c)");

    sprites = anw_open("sprites.anw");
    if (!sprites) handle_init_error("Could not open sprites.anw", "game_init() (game.c)");

    backgrounds = anw_open("bgs.anw");
    if (!backgrounds) handle_init_error("Could not open backgrounds.anw", "game_init() (game.c)");

    tiles = anw_open("tiles.anw");
    if (!tiles) handle_init_error("Could not open tiles.anw", "game_init() (game.c)");

//  DATAFILE *title_theme = find_datafile_object(music, "TITLE_XM");
//  if (!title_theme) handle_init_error("Could not load the title song from music.dat (is the file corrupt?)", "game_init() (game.c)");
    bool file_found = false;
    do if (strcmp(music->current_entry_header->filename, "title.xm") == 0) {
        file_found = true; break;
    } while (anw_next(music));
    if (!file_found) handle_init_error("Could not find the title song in music.anw (is the file corrupt?)", "game_init() (game.c)");
    void* title_song = anw_load(music);
    if (!title_song) handle_init_error("Could not load the title song from music.anw (is the file corrupt?)", "game_init() (game.c)");
    if (!load_module(title_song, music->current_entry_header->filesize)) handle_init_error("Could not load the title song from music.anw (is the file corrupt?)", "game_init() (game.c)");
//  natsuki_spritesheet_walk = find_datafile_object(sprites, "NATSUKI_WALK_BMP")->dat;
//  natsuki_spritesheet_run = find_datafile_object(sprites, "NATSUKI_RUN_BMP")->dat;
//  natsuki_spritesheet_fall = find_datafile_object(sprites, "NATSUKI_FALL_BMP")->dat;
//  natsuki_spritesheet_jump = find_datafile_object(sprites, "NATSUKI_JUMP_BMP")->dat;
//  natsuki_sprite = create_bitmap(NATSUKI_FRAME_W, NATSUKI_FRAME_H);
//  natsuki_hitbox = create_hitbox(160, 45, NATSUKI_FRAME_W, NATSUKI_FRAME_H, NATSUKI_FRAME_W / 2, NATSUKI_FRAME_H / 2, false, false, false);
//  create_hitbox(160, 135, 320, 45, 160, 0, false, false, false);
//  create_hitbox(32, 32, 16, 149, 16, 16, false, false, true);
//  background = find_datafile_object(backgrounds, "BG_TOKYO_BMP")->dat;
    play_module();
}

void game_input() {     // input collection and processing
    // SDL: poll events
    // Allegro: (semi-)stub (all collection done via timer)
    unsigned char control_state = input_collect();
    if ((control_state & CONTROL_LEFT) && check_moving(natsuki_hitbox, (control_state & CONTROL_SPRINT) ? -NATSUKI_SPEED_SPRINT : -NATSUKI_SPEED_NORMAL, 0)) {
        natsuki_hitbox.position.x -= (control_state & CONTROL_SPRINT) ? NATSUKI_SPEED_SPRINT : NATSUKI_SPEED_NORMAL;
        natsuki_hitbox.flipped |=  H;
    } if ((control_state & CONTROL_RIGHT) && check_moving(natsuki_hitbox, (control_state & CONTROL_SPRINT) ? NATSUKI_SPEED_SPRINT : NATSUKI_SPEED_NORMAL, 0)) {
        natsuki_hitbox.position.x += (control_state & CONTROL_SPRINT) ? NATSUKI_SPEED_SPRINT : NATSUKI_SPEED_NORMAL;
        natsuki_hitbox.flipped &= ~H;
    } grounded = !check_moving(natsuki_hitbox, 0, 1); if (grounded) natsuki_air_jumps = natsuki_max_air_jumps;
    if ((control_state & CONTROL_JUMP) && (grounded || (natsuki_air_jumps > 0))) {
        if (!grounded) natsuki_air_jumps--;
        natsuki_y_speed = -NATSUKI_JUMP_SPEED;
    }
    natsuki_moving = ((control_state & CONTROL_LEFT) || (control_state & CONTROL_RIGHT));
}

void game_logic() {     // everything else
    if (check_moving(natsuki_hitbox, 0, natsuki_y_speed)) {
        natsuki_hitbox.position.y += natsuki_y_speed;
        if ((frames % (int)(1/GRAVITY)) == 0) natsuki_y_speed++;
    } else natsuki_y_speed = 0;
}

void game_draw() {      // drawing the frame
//  blit(background, buffer, 0, 0, 0, 0, GAME_HRES, GAME_VRES);
//  blit((grounded
//              ? (((control_state & CONTROL_SPRINT) && natsuki_moving)
//                  ? natsuki_spritesheet_run
//                  : natsuki_spritesheet_walk)
//              : ((natsuki_y_speed > 0)
//                  ? natsuki_spritesheet_fall
//                  : natsuki_spritesheet_jump)),
//          natsuki_sprite, NATSUKI_FRAME_W * ((frames / 6) %
//              ((natsuki_moving && grounded)
//               ? ((control_state & CONTROL_SPRINT)
//                   ? NATSUKI_FRAME_NUM_RUN
//                   : NATSUKI_FRAME_NUM_WALK)
//               : ((natsuki_y_speed > 0)
//                   ? NATSUKI_FRAME_NUM_FALL
//                   : NATSUKI_FRAME_NUM_JUMP))),
//          0, 0, 0, NATSUKI_FRAME_W, NATSUKI_FRAME_H);
//  switch (natsuki_hitbox.flipped){
//      case  O:         draw_sprite(buffer, natsuki_sprite, natsuki_hitbox.position.x - (NATSUKI_FRAME_W / 2), natsuki_hitbox.position.y - (NATSUKI_FRAME_H / 2)); break;
//      case  H:  draw_sprite_h_flip(buffer, natsuki_sprite, natsuki_hitbox.position.x - (NATSUKI_FRAME_W / 2), natsuki_hitbox.position.y - (NATSUKI_FRAME_H / 2)); break;
//      case  V:  draw_sprite_v_flip(buffer, natsuki_sprite, natsuki_hitbox.position.x - (NATSUKI_FRAME_W / 2), natsuki_hitbox.position.y - (NATSUKI_FRAME_H / 2)); break;
//      case VH: draw_sprite_vh_flip(buffer, natsuki_sprite, natsuki_hitbox.position.x - (NATSUKI_FRAME_W / 2), natsuki_hitbox.position.y - (NATSUKI_FRAME_H / 2)); break;
//  }
    frames++;           // increment the frame counter
    draw_frame();       // do what the function says
}

void game_shutdown() {  // final farewells
//  destroy_bitmap(buffer);
//  unload_datafile(music);
//  unload_datafile(sprites);
//  unload_datafile(backgrounds);
    graphics_cleanup();
}
