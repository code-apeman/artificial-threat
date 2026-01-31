#include "includes.h"
#include "graphics.h"
#include "error.h"

void graphics_init(){
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0)
		handle_init_failure("graphics_init() (graphics.c)");
    set_window_title("Artificial Threat");
}
