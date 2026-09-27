#ifndef GRAPHICS_H
#include <stdint.h>
#include "memchunk.h"
#include "vecflip.h"
typedef uint16_t graphics_asset_id;
void graphics_init(void);
graphics_asset_id load_graphics_asset(memchunk *bitmap_chunk);
void set_background(graphics_asset_id asset);
void draw_sprite(vector2 pos, vector2 size, flipped flip, char framecount, char framenum, graphics_asset_id asset);
void draw_frame(void);
void graphics_cleanup(void);
#define GAME_HRES 320
#define GAME_VRES 200
#define GRAPHICS_H
#else
#warning "Multiple inclusions of graphics.h"
#endif
