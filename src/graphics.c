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
    {0,   0,   0,   0},
    {0,   0,   0, 168},
    {0,   0, 168,   0},
    {0,   0, 168, 168},
    {0, 168,   0,   0},
    {0, 168,   0, 168},
    {0, 168,  84,   0},
    {0, 168, 168, 168},
    {0,  84,  84,  84},
    {0,  84,  84, 252},
    {0,  84, 252,  84},
    {0,  84, 252, 252},
    {0, 252,  84,  84},
    {0, 252,  84, 252},
    {0, 252, 252,  84},
    {0, 252, 252, 252},

    // grayscale
    {0,   0,   0,   0},
    {0,  20,  20,  20},
    {0,  32,  32,  32},
    {0,  44,  44,  44},
    {0,  56,  56,  56},
    {0,  68,  68,  68},
    {0,  80,  80,  80},
    {0,  96,  96,  96},
    {0, 112, 112, 112},
    {0, 128, 128, 128},
    {0, 144, 144, 144},
    {0, 160, 160, 160},
    {0, 180, 180, 180},
    {0, 200, 200, 200},
    {0, 224, 224, 224},
    {0, 252, 252, 252},

    // light shades
    {0,   0,   0, 252},
    {0,  64,   0, 252},
    {0, 124,   0, 252},
    {0, 188,   0, 252},
    {0, 252,   0, 252},
    {0, 252,   0, 188},
    {0, 252,   0, 124},
    {0, 252,   0,  64},
    {0, 252,   0,   0},
    {0, 252,  64,   0},
    {0, 252, 124,   0},
    {0, 252, 188,   0},
    {0, 252, 252,   0},
    {0, 188, 252,   0},
    {0, 124, 252,   0},
    {0,  64, 252,   0},
    {0,   0, 252,   0},
    {0,   0, 252,  64},
    {0,   0, 252, 124},
    {0,   0, 252, 188},
    {0,   0, 252, 252},
    {0,   0, 188, 252},
    {0,   0, 124, 252},
    {0,   0,  64, 252},
    {0, 124, 124, 252},
    {0, 156, 124, 252},
    {0, 188, 124, 252},
    {0, 220, 124, 252},
    {0, 252, 124, 252},
    {0, 252, 124, 220},
    {0, 252, 124, 188},
    {0, 252, 124, 156},
    {0, 252, 124, 124},
    {0, 252, 156, 124},
    {0, 252, 188, 124},
    {0, 252, 220, 124},
    {0, 252, 252, 124},
    {0, 220, 252, 124},
    {0, 188, 252, 124},
    {0, 156, 252, 124},
    {0, 124, 252, 124},
    {0, 124, 252, 156},
    {0, 124, 252, 188},
    {0, 124, 252, 220},
    {0, 124, 252, 252},
    {0, 124, 220, 252},
    {0, 124, 188, 252},
    {0, 124, 156, 252},
    {0, 180, 180, 252},
    {0, 196, 180, 252},
    {0, 216, 180, 252},
    {0, 232, 180, 252},
    {0, 252, 180, 252},
    {0, 252, 180, 232},
    {0, 252, 180, 216},
    {0, 252, 180, 196},
    {0, 252, 180, 180},
    {0, 252, 196, 180},
    {0, 252, 216, 180},
    {0, 252, 232, 180},
    {0, 252, 252, 180},
    {0, 232, 252, 180},
    {0, 216, 252, 180},
    {0, 196, 252, 180},
    {0, 180, 252, 180},
    {0, 180, 252, 196},
    {0, 180, 252, 216},
    {0, 180, 252, 232},
    {0, 180, 252, 252},
    {0, 180, 232, 252},
    {0, 180, 216, 252},
    {0, 180, 196, 252},

    // mid shades
    {0,   0,   0, 180},
    {0,  44,   0, 180},
    {0,  88,   0, 180},
    {0, 132,   0, 180},
    {0, 180,   0, 180},
    {0, 180,   0, 132},
    {0, 180,   0,  88},
    {0, 180,   0,  44},
    {0, 180,   0,   0},
    {0, 180,  44,   0},
    {0, 180,  88,   0},
    {0, 180, 132,   0},
    {0, 180, 180,   0},
    {0, 132, 180,   0},
    {0,  88, 180,   0},
    {0,  44, 180,   0},
    {0,   0, 180,   0},
    {0,   0, 180,  44},
    {0,   0, 180,  88},
    {0,   0, 180, 132},
    {0,   0, 180, 180},
    {0,   0, 132, 180},
    {0,   0,  88, 180},
    {0,   0,  44, 180},
    {0,  88,  88, 180},
    {0, 108,  88, 180},
    {0, 132,  88, 180},
    {0, 156,  88, 180},
    {0, 180,  88, 180},
    {0, 180,  88, 156},
    {0, 180,  88, 132},
    {0, 180,  88, 108},
    {0, 180,  88,  88},
    {0, 180, 108,  88},
    {0, 180, 132,  88},
    {0, 180, 156,  88},
    {0, 180, 180,  88},
    {0, 156, 180,  88},
    {0, 132, 180,  88},
    {0, 108, 180,  88},
    {0,  88, 180,  88},
    {0,  88, 180, 108},
    {0,  88, 180, 132},
    {0,  88, 180, 156},
    {0,  88, 180, 180},
    {0,  88, 156, 180},
    {0,  88, 132, 180},
    {0,  88, 108, 180},
    {0, 128, 128, 180},
    {0, 140, 128, 180},
    {0, 152, 128, 180},
    {0, 164, 128, 180},
    {0, 180, 128, 180},
    {0, 180, 128, 164},
    {0, 180, 128, 152},
    {0, 180, 128, 140},
    {0, 180, 128, 128},
    {0, 180, 140, 128},
    {0, 180, 152, 128},
    {0, 180, 164, 128},
    {0, 180, 180, 128},
    {0, 164, 180, 128},
    {0, 152, 180, 128},
    {0, 140, 180, 128},
    {0, 128, 180, 128},
    {0, 128, 180, 140},
    {0, 128, 180, 152},
    {0, 128, 180, 164},
    {0, 128, 180, 180},
    {0, 128, 164, 180},
    {0, 128, 152, 180},
    {0, 128, 140, 180},

    // dark shades
    {0,   0,   0, 112},
    {0,  28,   0, 112},
    {0,  56,   0, 112},
    {0,  84,   0, 112},
    {0, 112,   0, 112},
    {0, 112,   0,  84},
    {0, 112,   0,  56},
    {0, 112,   0,  28},
    {0, 112,   0,   0},
    {0, 112,  28,   0},
    {0, 112,  56,   0},
    {0, 112,  84,   0},
    {0, 112, 112,   0},
    {0,  84, 112,   0},
    {0,  56, 112,   0},
    {0,  28, 112,   0},
    {0,   0, 112,   0},
    {0,   0, 112,  28},
    {0,   0, 112,  56},
    {0,   0, 112,  84},
    {0,   0, 112, 112},
    {0,   0,  84, 112},
    {0,   0,  56, 112},
    {0,   0,  28, 112},
    {0,  56,  56, 112},
    {0,  68,  56, 112},
    {0,  84,  56, 112},
    {0,  96,  56, 112},
    {0, 112,  56, 112},
    {0, 112,  56,  96},
    {0, 112,  56,  84},
    {0, 112,  56,  68},
    {0, 112,  56,  56},
    {0, 112,  68,  56},
    {0, 112,  84,  56},
    {0, 112,  96,  56},
    {0, 112, 112,  56},
    {0,  96, 112,  56},
    {0,  84, 112,  56},
    {0,  68, 112,  56},
    {0,  56, 112,  56},
    {0,  56, 112,  68},
    {0,  56, 112,  84},
    {0,  56, 112,  96},
    {0,  56, 112, 112},
    {0,  56,  96, 112},
    {0,  56,  84, 112},
    {0,  56,  68, 112},
    {0,  80,  80, 112},
    {0,  88,  80, 112},
    {0,  96,  80, 112},
    {0, 104,  80, 112},
    {0, 112,  80, 112},
    {0, 112,  80, 104},
    {0, 112,  80,  96},
    {0, 112,  80,  88},
    {0, 112,  80,  80},
    {0, 112,  88,  80},
    {0, 112,  96,  80},
    {0, 112,  104 ,80},
    {0, 112, 112,  80},
    {0, 104, 112,  80},
    {0,  96, 112,  80},
    {0,  88, 112,  80},
    {0,  80, 112,  80},
    {0,  80, 112,  88},
    {0,  80, 112,  96},
    {0,  80, 112, 104},
    {0,  80, 112, 112},
    {0,  80, 104, 112},
    {0,  80,  96, 112},
    {0,  80,  88, 112},

    // reserved 
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
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


