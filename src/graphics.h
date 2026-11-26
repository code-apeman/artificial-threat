// TODO: make the header more readable; clean up all the fluff
#ifndef GRAPHICS_H
#include <stdint.h>
#include "memchunk.h"
#include "vecflip.h"
#define GAME_HRES 320
#define GAME_VRES 200
#define PALSIZE 256

typedef struct indexed_palette_entry {
    uint8_t padding, r, g, b;
} indexed_palette_entry;
typedef indexed_palette_entry palette[PALSIZE];
typedef uint8_t pixel;
typedef struct buffer {
    vector2* size;
    pixel* bitmap;
} buffer;
void bitblt(buffer *source, buffer *dest,
        const vector2 pos_src, const vector2 pos_dest, const vector2 blocksize,
        const flipped mirror);
void zmasked_bitblt(buffer *source, buffer *dest,
        const vector2 pos_src, const vector2 pos_dest, const vector2 blocksize,
        const flipped mirror);
void graphics_init(void);
buffer *load_graphics_asset(memchunk *bitmap_chunk);
void set_background(buffer *asset);
void draw_sprite(vector2 pos, vector2 size, flipped flip,
        char framecount, char framenum, buffer *asset);
void draw_frame(void);
void graphics_cleanup(void);
void background_offset_abs(const vector2 offset);
void background_offset_rel(const vector2 offset);
#pragma pack(push)
#pragma pack(1)
typedef struct bmp_fheader {
    const char magic[2];
    uint32_t size;
    uint32_t reserved;
    uint32_t data_offset;
} bmp_fheader;
typedef struct bmp_iheader {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitcount;
    uint32_t compression;
    uint32_t image_size;
    uint32_t ppm_h;
    uint32_t ppm_v;
    uint32_t colmap_count;
    uint32_t colmap_significant;
} bmp_iheader;
#pragma pack(pop)
#define GRAPHICS_H
#else
#warning "Multiple inclusions of graphics.h"
#endif
