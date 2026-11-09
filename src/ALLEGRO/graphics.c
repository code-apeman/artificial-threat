#include <allegro.h>
#include "includes.h"
#include "error.h"
#include "graphics.h"

BITMAP *buffer;
int screen_w, screen_h;

void graphics_init(){
    allegro_init();  // graphics always inits first, so backend is always initialized there
    if (get_desktop_resolution(&screen_w, &screen_h) != 0){
        screen_w = 320; screen_h = 180;
    }
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, screen_w, screen_h, 0, 0) != 0)
		handle_init_failure("graphics_init() (graphics.c)");
    set_window_title("Artificial Threat");
    set_palette(default_palette);
    buffer = create_bitmap(GAME_HRES, GAME_VRES);
    if (!buffer)
        handle_init_error("Failed to allocate the frame buffer", "graphics_init() (graphics.c)");
}

void draw_frame(){
    stretch_blit(buffer, screen, 0, 0, GAME_HRES, GAME_VRES, 0, 0, SCREEN_W, SCREEN_H);
}

void graphics_cleanup(){
	allegro_exit();
}
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
