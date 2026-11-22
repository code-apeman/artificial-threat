#ifndef GRAPHICS_H
#include <stdint.h>
#include "memchunk.h"
#include "vecflip.h"
#define GAME_HRES 320
#define GAME_VRES 200
#define PALSIZE 256
#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;
typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;
typedef struct {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} BMPRGBQuad;
#pragma pack(pop)

typedef struct indexed_palette_entry {
    uint8_t r, g, b, padding;
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
typedef uint16_t graphics_asset_id;
void graphics_init(void);
graphics_asset_id load_graphics_asset(memchunk *bitmap_chunk);
void set_background(graphics_asset_id asset);
void draw_sprite(vector2 pos, vector2 size, flipped flip,
        char framecount, char framenum, graphics_asset_id asset);
void draw_frame(void);
void graphics_cleanup(void);
#define GRAPHICS_H
#else
#warning "Multiple inclusions of graphics.h"
#endif
