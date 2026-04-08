#include "includes.h"
#include "game.h"
#include "graphics.h"
#include "sound.h"
#include "error.h"
#include "physics.h"

#define NATSUKI_FRAME_W 24
#define NATSUKI_FRAME_H 32
#define NATSUKI_FRAME_NUM 8
#define NATSUKI_SPEED_NORMAL 1
#define NATSUKI_SPEED_SPRINT 3
#define NATSUKI_JUMP_SPEED 5

DATAFILE *music, *sprites, *backgrounds, *tiles;
BITMAP *natsuki_spritesheet, *natsuki_sprite;
BITMAP *background;
extern BITMAP *buffer;
extern bool game_exit_flag;
extern volatile bool frame_flag;
unsigned long long int frames = 0;

// Position, physics, etc.
hitbox natsuki_hitbox;
int natsuki_y_speed = 0;

// Stats
unsigned int natsuki_mhp = 50, natsuki_atk = 2, natsuki_def = 0;
unsigned int natsuki_lv = 1, natsuki_exp = 0, natsuki_exp_to_next = 50;
unsigned int natsuki_max_air_jumps = 0;
int natsuki_hp = 50, natsuki_air_jumps = 0;
bool grounded = false;

// Scrolling
unsigned int camera_x = 0, camera_y = 0;

// Controls
volatile unsigned char control_state = 0;

#define CONTROL_LEFT          0b00000001
#define CONTROL_RIGHT         0b00000010
#define CONTROL_UP            0b00000100
#define CONTROL_DOWN          0b00001000
#define CONTROL_JUMP          0b00010000
#define CONTROL_SPRINT        0b00100000
#define CONTROL_ATTACK_MELEE  0b01000000
#define CONTROL_ATTACK_RANGED 0b10000000

void keyboard_handler(int scancode) {
    // evil bitwise fuckery
    if (scancode & 0x80) switch (scancode & 0x7f) {
        case KEY_LEFT:  control_state &= ~CONTROL_LEFT;          break;
        case KEY_RIGHT: control_state &= ~CONTROL_RIGHT;         break;
        case KEY_UP:    control_state &= ~CONTROL_UP;            break;
        case KEY_DOWN:  control_state &= ~CONTROL_DOWN;          break;
        case KEY_Z:     control_state &= ~CONTROL_JUMP;          break;
        case KEY_X:     control_state &= ~CONTROL_SPRINT;        break;
        case KEY_C:     control_state &= ~CONTROL_ATTACK_MELEE;  break;
        case KEY_A:     control_state &= ~CONTROL_ATTACK_RANGED; break;
    } else switch (scancode & 0x7f) {
        case KEY_ESC: game_exit_flag = true; break;

        case KEY_LEFT:  control_state |=  CONTROL_LEFT;          break;
        case KEY_RIGHT: control_state |=  CONTROL_RIGHT;         break;
        case KEY_UP:    control_state |=  CONTROL_UP;            break;
        case KEY_DOWN:  control_state |=  CONTROL_DOWN;          break;
        case KEY_Z:     control_state |=  CONTROL_JUMP;          break;
        case KEY_X:     control_state |=  CONTROL_SPRINT;        break;
        case KEY_C:     control_state |=  CONTROL_ATTACK_MELEE;  break;
        case KEY_A:     control_state |=  CONTROL_ATTACK_RANGED; break;
    }
}
END_OF_FUNCTION(keyboard_handler)

void frame_timer_callback(){
    frame_flag = true;
}
END_OF_FUNCTION(frame_timer_callback)

void game_init() {      // initialization routine
    install_timer();
    graphics_init();
    sound_init();
    install_keyboard();

    LOCK_VARIABLE(control_state);
    LOCK_FUNCTION(keyboard_handler);
    keyboard_lowlevel_callback = keyboard_handler;

    LOCK_VARIABLE(frame_flag);
    LOCK_FUNCTION(frame_timer_callback);
    install_int_ex(frame_timer_callback, FRAMETIME_HWTICKS);

    music = load_datafile("music.dat");
    if (!music) handle_init_error("Could not load music.dat", "game_init() (game.c)");

    sprites = load_datafile("sprites.dat");
    if (!sprites) handle_init_error("Could not load sprites.dat", "game_init() (game.c)");

    backgrounds = load_datafile("bgs.dat");
    if (!backgrounds) handle_init_error("Could not load backgrounds.dat", "game_init() (game.c)");

    tiles = load_datafile("tiles.dat");
    if (!tiles) handle_init_error("Could not load tiles.dat", "game_init() (game.c)");

    DATAFILE *title_theme = find_datafile_object(music, "ZOLOFT_IT");
    if (!title_theme) handle_init_error("Could not load the title song from music.dat (is the file corrupt?)", "game_init() (game.c)");
    if (!load_module(title_theme->dat, title_theme->size)) handle_init_error("Could not load the title song from music.dat (is the file corrupt?)", "game_init() (game.c)");
    natsuki_spritesheet = find_datafile_object(sprites, "NATSUKI_WALK_BMP")->dat;
    natsuki_sprite = create_bitmap(NATSUKI_FRAME_W, NATSUKI_FRAME_H);
    natsuki_hitbox = create_hitbox(160, 45, NATSUKI_FRAME_W, NATSUKI_FRAME_H, NATSUKI_FRAME_W / 2, NATSUKI_FRAME_H / 2, NULL);
    create_hitbox(160, 135, 320, 45, 160, 0, NULL);
    background = find_datafile_object(backgrounds, "BG_TOKYO_BMP")->dat;
    play_module();
}

void game_input() {     // input collection and processing

}

void game_logic() {     // everything else
    if ((control_state & CONTROL_LEFT) && check_moving(natsuki_hitbox, (control_state & CONTROL_SPRINT) ? -NATSUKI_SPEED_SPRINT : -NATSUKI_SPEED_NORMAL, 0)) natsuki_hitbox.position.x -= (control_state & CONTROL_SPRINT) ? NATSUKI_SPEED_SPRINT : NATSUKI_SPEED_NORMAL;
    if ((control_state & CONTROL_RIGHT) && check_moving(natsuki_hitbox, (control_state & CONTROL_SPRINT) ? NATSUKI_SPEED_SPRINT : NATSUKI_SPEED_NORMAL, 0)) natsuki_hitbox.position.x += (control_state & CONTROL_SPRINT) ? NATSUKI_SPEED_SPRINT : NATSUKI_SPEED_NORMAL;
    grounded = !check_moving(natsuki_hitbox, 0, 1);
    if (grounded) natsuki_air_jumps = natsuki_max_air_jumps;
    if ((control_state & CONTROL_JUMP) && (grounded || (natsuki_air_jumps > 0))) {
        if (!grounded) natsuki_air_jumps--;
        natsuki_y_speed = -NATSUKI_JUMP_SPEED;
    }
    if (check_moving(natsuki_hitbox, 0, natsuki_y_speed)) {
        natsuki_hitbox.position.y += natsuki_y_speed;
        if ((frames % (int)(1/GRAVITY)) == 0) natsuki_y_speed++;
    } else natsuki_y_speed = 0;
    buffer_check_callback();
}

void game_draw() {      // drawing the frame
    blit(background, buffer, 0, 0, 0, 0, GAME_HRES, GAME_VRES);
    blit(natsuki_spritesheet, natsuki_sprite, NATSUKI_FRAME_W * ((frames / 6) % NATSUKI_FRAME_NUM), 0, 0, 0, NATSUKI_FRAME_W, NATSUKI_FRAME_H);
    draw_sprite(buffer, natsuki_sprite, natsuki_hitbox.position.x - (NATSUKI_FRAME_W / 2), natsuki_hitbox.position.y - (NATSUKI_FRAME_H / 2)); 
    frames++;           // increment the frame counter
    buffer_show();        // do what the function says
}

void game_shutdown() {  // final farewells
    destroy_bitmap(buffer);
    unload_datafile(music);
    unload_datafile(sprites);
    unload_datafile(backgrounds);
}
