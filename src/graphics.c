#include "includes.h"
#include "graphics.h"
#include "error.h"

BITMAP *buffer;
int screen_w, screen_h;

void graphics_init(){
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

void buffer_show(){
    stretch_blit(buffer, screen, 0, 0, GAME_HRES, GAME_VRES, 0, 0, SCREEN_W, SCREEN_H);
}
