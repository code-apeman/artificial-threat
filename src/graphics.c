/*
 * Mode 13h(-esque) graphics implementation
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "vecflip.h"
#include "error.h"

palette mode13h_palette = {
    // 4bpp colors
    {255,   0,   0,   0},
    {255,   0,   0, 168},
    {255,   0, 168,   0},
    {255,   0, 168, 168},
    {255, 168,   0,   0},
    {255, 168,   0, 168},
    {255, 168,  84,   0},
    {255, 168, 168, 168},
    {255,  84,  84,  84},
    {255,  84,  84, 252},
    {255,  84, 252,  84},
    {255,  84, 252, 252},
    {255, 252,  84,  84},
    {255, 252,  84, 252},
    {255, 252, 252,  84},
    {255, 252, 252, 252},

    // grayscale
    {255,   0,   0,   0},
    {255,  20,  20,  20},
    {255,  32,  32,  32},
    {255,  44,  44,  44},
    {255,  56,  56,  56},
    {255,  68,  68,  68},
    {255,  80,  80,  80},
    {255,  96,  96,  96},
    {255, 112, 112, 112},
    {255, 128, 128, 128},
    {255, 144, 144, 144},
    {255, 160, 160, 160},
    {255, 180, 180, 180},
    {255, 200, 200, 200},
    {255, 224, 224, 224},
    {255, 252, 252, 252},

    // light shades
    {255,   0,   0, 252},
    {255,  64,   0, 252},
    {255, 124,   0, 252},
    {255, 188,   0, 252},
    {255, 252,   0, 252},
    {255, 252,   0, 188},
    {255, 252,   0, 124},
    {255, 252,   0,  64},
    {255, 252,   0,   0},
    {255, 252,  64,   0},
    {255, 252, 124,   0},
    {255, 252, 188,   0},
    {255, 252, 252,   0},
    {255, 188, 252,   0},
    {255, 124, 252,   0},
    {255,  64, 252,   0},
    {255,   0, 252,   0},
    {255,   0, 252,  64},
    {255,   0, 252, 124},
    {255,   0, 252, 188},
    {255,   0, 252, 252},
    {255,   0, 188, 252},
    {255,   0, 124, 252},
    {255,   0,  64, 252},
    {255, 124, 124, 252},
    {255, 156, 124, 252},
    {255, 188, 124, 252},
    {255, 220, 124, 252},
    {255, 252, 124, 252},
    {255, 252, 124, 220},
    {255, 252, 124, 188},
    {255, 252, 124, 156},
    {255, 252, 124, 124},
    {255, 252, 156, 124},
    {255, 252, 188, 124},
    {255, 252, 220, 124},
    {255, 252, 252, 124},
    {255, 220, 252, 124},
    {255, 188, 252, 124},
    {255, 156, 252, 124},
    {255, 124, 252, 124},
    {255, 124, 252, 156},
    {255, 124, 252, 188},
    {255, 124, 252, 220},
    {255, 124, 252, 252},
    {255, 124, 220, 252},
    {255, 124, 188, 252},
    {255, 124, 156, 252},
    {255, 180, 180, 252},
    {255, 196, 180, 252},
    {255, 216, 180, 252},
    {255, 232, 180, 252},
    {255, 252, 180, 252},
    {255, 252, 180, 232},
    {255, 252, 180, 216},
    {255, 252, 180, 196},
    {255, 252, 180, 180},
    {255, 252, 196, 180},
    {255, 252, 216, 180},
    {255, 252, 232, 180},
    {255, 252, 252, 180},
    {255, 232, 252, 180},
    {255, 216, 252, 180},
    {255, 196, 252, 180},
    {255, 180, 252, 180},
    {255, 180, 252, 196},
    {255, 180, 252, 216},
    {255, 180, 252, 232},
    {255, 180, 252, 252},
    {255, 180, 232, 252},
    {255, 180, 216, 252},
    {255, 180, 196, 252},

    // mid shades
    {255,   0,   0, 180},
    {255,  44,   0, 180},
    {255,  88,   0, 180},
    {255, 132,   0, 180},
    {255, 180,   0, 180},
    {255, 180,   0, 132},
    {255, 180,   0,  88},
    {255, 180,   0,  44},
    {255, 180,   0,   0},
    {255, 180,  44,   0},
    {255, 180,  88,   0},
    {255, 180, 132,   0},
    {255, 180, 180,   0},
    {255, 132, 180,   0},
    {255,  88, 180,   0},
    {255,  44, 180,   0},
    {255,   0, 180,   0},
    {255,   0, 180,  44},
    {255,   0, 180,  88},
    {255,   0, 180, 132},
    {255,   0, 180, 180},
    {255,   0, 132, 180},
    {255,   0,  88, 180},
    {255,   0,  44, 180},
    {255,  88,  88, 180},
    {255, 108,  88, 180},
    {255, 132,  88, 180},
    {255, 156,  88, 180},
    {255, 180,  88, 180},
    {255, 180,  88, 156},
    {255, 180,  88, 132},
    {255, 180,  88, 108},
    {255, 180,  88,  88},
    {255, 180, 108,  88},
    {255, 180, 132,  88},
    {255, 180, 156,  88},
    {255, 180, 180,  88},
    {255, 156, 180,  88},
    {255, 132, 180,  88},
    {255, 108, 180,  88},
    {255,  88, 180,  88},
    {255,  88, 180, 108},
    {255,  88, 180, 132},
    {255,  88, 180, 156},
    {255,  88, 180, 180},
    {255,  88, 156, 180},
    {255,  88, 132, 180},
    {255,  88, 108, 180},
    {255, 128, 128, 180},
    {255, 140, 128, 180},
    {255, 152, 128, 180},
    {255, 164, 128, 180},
    {255, 180, 128, 180},
    {255, 180, 128, 164},
    {255, 180, 128, 152},
    {255, 180, 128, 140},
    {255, 180, 128, 128},
    {255, 180, 140, 128},
    {255, 180, 152, 128},
    {255, 180, 164, 128},
    {255, 180, 180, 128},
    {255, 164, 180, 128},
    {255, 152, 180, 128},
    {255, 140, 180, 128},
    {255, 128, 180, 128},
    {255, 128, 180, 140},
    {255, 128, 180, 152},
    {255, 128, 180, 164},
    {255, 128, 180, 180},
    {255, 128, 164, 180},
    {255, 128, 152, 180},
    {255, 128, 140, 180},

    // dark shades
    {255,   0,   0, 112},
    {255,  28,   0, 112},
    {255,  56,   0, 112},
    {255,  84,   0, 112},
    {255, 112,   0, 112},
    {255, 112,   0,  84},
    {255, 112,   0,  56},
    {255, 112,   0,  28},
    {255, 112,   0,   0},
    {255, 112,  28,   0},
    {255, 112,  56,   0},
    {255, 112,  84,   0},
    {255, 112, 112,   0},
    {255,  84, 112,   0},
    {255,  56, 112,   0},
    {255,  28, 112,   0},
    {255,   0, 112,   0},
    {255,   0, 112,  28},
    {255,   0, 112,  56},
    {255,   0, 112,  84},
    {255,   0, 112, 112},
    {255,   0,  84, 112},
    {255,   0,  56, 112},
    {255,   0,  28, 112},
    {255,  56,  56, 112},
    {255,  68,  56, 112},
    {255,  84,  56, 112},
    {255,  96,  56, 112},
    {255, 112,  56, 112},
    {255, 112,  56,  96},
    {255, 112,  56,  84},
    {255, 112,  56,  68},
    {255, 112,  56,  56},
    {255, 112,  68,  56},
    {255, 112,  84,  56},
    {255, 112,  96,  56},
    {255, 112, 112,  56},
    {255,  96, 112,  56},
    {255,  84, 112,  56},
    {255,  68, 112,  56},
    {255,  56, 112,  56},
    {255,  56, 112,  68},
    {255,  56, 112,  84},
    {255,  56, 112,  96},
    {255,  56, 112, 112},
    {255,  56,  96, 112},
    {255,  56,  84, 112},
    {255,  56,  68, 112},
    {255,  80,  80, 112},
    {255,  88,  80, 112},
    {255,  96,  80, 112},
    {255, 104,  80, 112},
    {255, 112,  80, 112},
    {255, 112,  80, 104},
    {255, 112,  80,  96},
    {255, 112,  80,  88},
    {255, 112,  80,  80},
    {255, 112,  88,  80},
    {255, 112,  96,  80},
    {255, 112,  104 ,80},
    {255, 112, 112,  80},
    {255, 104, 112,  80},
    {255,  96, 112,  80},
    {255,  88, 112,  80},
    {255,  80, 112,  80},
    {255,  80, 112,  88},
    {255,  80, 112,  96},
    {255,  80, 112, 104},
    {255,  80, 112, 112},
    {255,  80, 104, 112},
    {255,  80,  96, 112},
    {255,  80,  88, 112},

    // reserved 
    {255, 0, 0, 0},
    {255, 0, 0, 0},
    {255, 0, 0, 0},
    {255, 0, 0, 0},
    {255, 0, 0, 0},
    {255, 0, 0, 0},
    {255, 0, 0, 0},
    {255, 0, 0, 0}
};

palette *active_palette = &mode13h_palette;

buffer buf1, buf2;
buffer *front_buf = &buf1, *back_buf = &buf2;
bool frontbuf_is_2 = false;

buffer *background_ptr;
vector2 bg_offset = {0, 0};

void graphics_init(void){
    vector2 *bufsize = malloc(sizeof(vector2));
    bufsize->x = GAME_HRES;
    bufsize->y = GAME_VRES;
    buf1 = (buffer){bufsize, malloc(GAME_HRES * GAME_VRES * sizeof(pixel))};
    buf2 = (buffer){bufsize, malloc(GAME_HRES * GAME_VRES * sizeof(pixel))};
}

void swap_buffers(void){
    if(frontbuf_is_2){
        front_buf = &buf1;
        back_buf = &buf2;
        frontbuf_is_2 = false;
    } else {
        front_buf = &buf2;
        back_buf = &buf1;
        frontbuf_is_2 = true;
    }
}

// Simple BITmap BLock Transfer, the pos_* vars are using the upper left corner
// we also mirror stuff here
void bitblt(buffer *source, buffer *dest,
        const vector2 pos_src, const vector2 pos_dest, const vector2 blocksize,
        const flipped mirror){
    int soff, doff;
    for (int i = 0; i < blocksize.y; i++){
        // don't spill bytes in/from nonexistent lines
        if (pos_dest.y + i < 0) continue;
        if (pos_dest.y + i > dest->size->y) continue;
        if (pos_src.y + i < 0) continue;
        if (pos_src.y + i > source->size->y) continue;
        for (int j = 0; j < blocksize.x; j++){
            // don't spill bytes in/from nonexistent columns
            if (pos_dest.x + j < 0) continue;
            if (pos_dest.x + j > dest->size->x) continue;
            if (pos_src.x + j < 0) continue;
            if (pos_src.x + j > source->size->x) continue;
            // offsets (what do you mean i like ternary statements too much?)
            soff = (
                (pos_src.y + ((mirror & V) ? blocksize.y - i - 1 : i))
                * source->size->x +
                (pos_src.x + ((mirror & H) ? blocksize.x - j - 1 : j))
            );
            doff = (pos_dest.y + i) * dest->size->x + (pos_dest.x + j);
            // actual transfer
            dest->bitmap[doff] = source->bitmap[soff];
        }
    }
}

// 0-Masked BITmap BLock Transfer (e. g. for sprites)
void zmasked_bitblt(buffer *source, buffer *dest,
        const vector2 pos_src, const vector2 pos_dest, const vector2 blocksize,
        const flipped mirror){
    int soff, doff;
    for (int i = 0; i < blocksize.y; i++){
        // don't spill bytes in/from nonexistent lines
        if (pos_dest.y + i < 0) continue;
        if (pos_dest.y + i > dest->size->y) continue;
        if (pos_src.y + i < 0) continue;
        if (pos_src.y + i > source->size->y) continue;
        for (int j = 0; j < blocksize.x; j++){
            // don't spill bytes in/from nonexistent columns
            if (pos_dest.x + j < 0) continue;
            if (pos_dest.x + j > dest->size->x) continue;
            if (pos_src.x + j < 0) continue;
            if (pos_src.x + j > source->size->x) continue;
            // offsets (what do you mean i like ternary statements too much?)
            soff = (
                (pos_src.y + ((mirror & V) ? blocksize.y - i - 1 : i))
                * source->size->x +
                (pos_src.x + ((mirror & H) ? blocksize.x - j - 1 : j))
            );
            doff = (pos_dest.y + i) * dest->size->x + (pos_dest.x + j);
            // actual transfer
            if (source->bitmap[soff] != 0)
                dest->bitmap[doff] = source->bitmap[soff];
        }
    }
}

void draw_frame(void){
    swap_buffers();
    render_frame();
    if (background_ptr) bitblt(background_ptr, back_buf, bg_offset,
            (vector2){0, 0}, *back_buf->size, O);
}

buffer *load_graphics_asset(memchunk *bitmap_chunk){
    bmp_fheader *file_header = malloc(sizeof(bmp_fheader));
    memcpy(file_header, bitmap_chunk->pointer, sizeof(bmp_fheader));
    if (file_header->magic[0] != 'B' || file_header->magic[1] != 'M') {
        free(file_header);
        handle_load_error("Invalid magic",
                "load_graphics_asset (graphics.c)");
    }
    bmp_iheader *image_header = malloc(sizeof(bmp_iheader));
    memcpy(image_header, bitmap_chunk->pointer + sizeof(bmp_fheader),
            sizeof(bmp_iheader));
    if (image_header->compression > 2) {
        free(image_header); free(file_header);
        handle_load_error("Unsupported compression type",
                "load_graphics_asset (graphics.c)");
    }
    if (image_header->compression != 0) {
        free(image_header); free(file_header);
        handle_load_error("RLE compression is not supported yet",
                "load_graphics_asset (graphics.c)");
    }
    buffer *asset = malloc(sizeof(buffer));
    asset->size = malloc(sizeof(vector2));
    asset->size->x = image_header->width;
    asset->size->y = image_header->height;
    asset->bitmap = malloc(asset->size->x * asset->size->y);
    char *bitmap_data = (char*)bitmap_chunk->pointer + file_header->data_offset;
    switch(image_header->bitcount){
        case 1: {
            char bit, bitmask;
            unsigned int byte, pxpos, pxpos_bmp;
            for (int y = 0; y < asset->size->y; y++)
                for (int x = 0; x < asset->size->x; x++){
                    pxpos = y * asset->size->x + x;
                    pxpos_bmp = (asset->size->y - y - 1) * asset->size->x + x;
                    bit = pxpos_bmp % 8; byte = pxpos_bmp / 8;
                    bitmask = 1 << (bit - 1);
                    asset->bitmap[pxpos]
                        = (bitmap_data[byte] & bitmask) ? 15 : 0;
                }
        } break;
        case 4: { // TODO: implement RLE4
            const unsigned char
                lsn_mask = 0x0F,
                msn_mask = 0xF0;
            unsigned int pxpos, pxpos_bmp;
            for (int y = 0; y < asset->size->y; y++)
                for (int x = 0; x < asset->size->x; x++) {
                    pxpos = y * asset->size->x + x;
                    pxpos_bmp = (asset->size->y - y - 1) * asset->size->x + x;
                    asset->bitmap[pxpos] = (pxpos_bmp % 2 == 0)
                        ? (bitmap_data[pxpos_bmp / 2] & msn_mask) >> 4
                        : bitmap_data[pxpos_bmp / 2] & lsn_mask;
                }
        } break;
        case 8: { // TODO; implement RLE8
            unsigned int pxpos, pxpos_bmp;
            for (int y = 0; y < asset->size->y; y++)
                for (int x = 0; x < asset->size->x; x++) {
                    pxpos = y * asset->size->x + x;
                    pxpos_bmp = (asset->size->y - y - 1) * asset->size->x + x;
                    asset->bitmap[pxpos] = bitmap_data[pxpos_bmp];
                }
        } break;
        default: {
            char *error;
            asprintf(&error,
                    "Unsupported bit depth (%dbpp)", image_header->bitcount);
            free(asset->bitmap); free(asset->size);
            free(image_header); free(file_header);
            free(asset);
            handle_load_error(error, "load_graphics_asset (graphics.c)");
        } break;
    }
    free(image_header); free(file_header); return asset;
}
void set_background(buffer *asset){
    background_ptr = asset;
}
void background_offset_abs(const vector2 offset){
    bg_offset.x = offset.x;
    bg_offset.y = offset.y;
}
void background_offset_rel(const vector2 offset){
    bg_offset.x += offset.x;
    bg_offset.y += offset.y;
}
void draw_sprite(vector2 pos, vector2 size, flipped flip,
        char framecount, char framenum, buffer *asset){
    int frame_width = asset->size->x / framecount;
    zmasked_bitblt(asset, back_buf, (vector2){frame_width * framenum, 0}, pos,
            (vector2){frame_width, asset->size->y}, flip);
}

void graphics_cleanup(void){
    free(buf1.bitmap);
    free(buf2.bitmap);
}


