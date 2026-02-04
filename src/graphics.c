#include "includes.h"
#include "graphics.h"
#include "error.h"

BITMAP *page1, *page2;
BITMAP *active_page, *inactive_page;
bool second_page_active = false;

void graphics_init(){
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, GAME_HRES, GAME_VRES, GAME_HRES * 4, GAME_VRES * 4) != 0)
		handle_init_failure("graphics_init() (graphics.c)");
    set_window_title("Artificial Threat");
    set_palette(default_palette);
    page1 = create_video_bitmap(GAME_HRES * 2, GAME_VRES * 2);
    if (!page1)
        handle_init_error("Failed to allocate the first screen page", "graphics_init() (graphics.c)");
    page2 = create_video_bitmap(GAME_HRES * 2, GAME_VRES * 2);
    if (!page2)
        handle_init_error("Failed to allocate the second screen page", "graphics_init() (graphics.c)");
    active_page = page1;
    inactive_page = page2;
}

void page_flip(){
    if (second_page_active) {
        active_page = page1;
        inactive_page = page2;
    } else {
        active_page = page2;
        inactive_page = page1;
    } second_page_active = !second_page_active;
    show_video_bitmap(active_page);
}
