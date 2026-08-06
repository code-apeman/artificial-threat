#ifndef GRAPHICS_H
void graphics_init(void);
void draw_frame(void);
#define GAME_HRES 320
#define GAME_VRES 180
#define GRAPHICS_H
#else
#warning "Multiple inclusions of graphics.h"
#endif
