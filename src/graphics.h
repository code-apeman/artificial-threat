#ifndef GRAPHICS_H
#include <stdint.h>
typedef uint16_t graphics_asset_id;
void graphics_init(void);
void set_background(graphics_asset_id asset);
void draw_sprite(int x, int y, int w, int h, char framecount, char framenum, graphics_asset_id asset);
void draw_frame(void);
void graphics_cleanup(void);
#define GAME_HRES 320
#define GAME_VRES 180
#define GRAPHICS_H
#else
#warning "Multiple inclusions of graphics.h"
#endif
