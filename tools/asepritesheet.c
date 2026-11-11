#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>

#pragma pack(push, 1)
typedef struct {
    uint8_t blue, green, red, reserved;
} BmpPaletteEntry;

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1, reserved2;
    uint32_t off_bits;
} BmpFileHeader;

typedef struct {
    uint32_t size;
    int32_t width, height;
    uint16_t planes, bit_count;
    uint32_t compression, size_image;
    int32_t x_pels_per_meter, y_pels_per_meter;
    uint32_t clr_used, clr_important;
} BmpInfoHeader;
#pragma pack(pop)

// ...huh

static uint16_t read16(FILE *f) {
    int a = fgetc(f), b = fgetc(f);
    if (a == EOF || b == EOF) return 0;
    return (uint16_t)(a | (b << 8));
}

static uint32_t read32(FILE *f) {
    uint32_t a = read16(f);
    uint32_t b = read16(f);
    return a | (b << 16);
}

static int decompress_cel(
    const uint8_t *src, size_t src_len,
    uint8_t *dst, size_t dst_len
) {
    if (src_len > UINT_MAX || dst_len > UINT_MAX)
        return 0;

    z_stream s;
    memset(&s, 0, sizeof(s));

    s.next_in = (Bytef *)src;
    s.avail_in = (uInt)src_len;
    s.next_out = dst;
    s.avail_out = (uInt)dst_len;

    if (inflateInit(&s) != Z_OK)
        return 0;

    int ret = inflate(&s, Z_FINISH);
    int ok = (ret == Z_STREAM_END &&
              s.total_out == dst_len);

    inflateEnd(&s);
    return ok;
}

static void init_palette(BmpPaletteEntry *p) {
    for (int i = 0; i < 256; i++) {
        p[i].red = i;
        p[i].green = i;
        p[i].blue = i;
        p[i].reserved = 0;
    }
}

static void print_palette(BmpPaletteEntry *p) {
    printf("\nPalette preview:\n");

    for (int i = 0; i < 256; i++) {
        printf("\033[48;2;%u;%u;%um  ",
               p[i].red, p[i].green, p[i].blue);

        if ((i + 1) % 16 == 0)
            printf("\033[0m\n");
    }

    printf("\033[0m\n");
}

// Aseprite modern palette chunk 0x2019
static void parse_palette_2019(
    FILE *f,
    long chunk_end,
    BmpPaletteEntry *palette
) {
    uint32_t palette_size = read32(f);
    uint32_t first = read32(f);
    uint32_t last = read32(f);

    fseek(f, 8, SEEK_CUR);

    if (palette_size > 256 || first > last || last >= palette_size) {
        fseek(f, chunk_end, SEEK_SET);
        return;
    }

    for (uint32_t i = first; i <= last; i++) {
        uint16_t flags = read16(f);

        int r = fgetc(f);
        int g = fgetc(f);
        int b = fgetc(f);
        int a = fgetc(f);

        if (r == EOF || g == EOF || b == EOF || a == EOF)
            break;

        palette[i].red = (uint8_t)r;
        palette[i].green = (uint8_t)g;
        palette[i].blue = (uint8_t)b;
        palette[i].reserved = 0;

        if (flags & 1) {
            uint16_t name_len = read16(f);
            fseek(f, name_len, SEEK_CUR);
        }
    }

    fseek(f, chunk_end, SEEK_SET);
}

// Aseprite legacy palette chunk (0x04/0x11)
static void parse_palette_0011(
    FILE *f,
    long chunk_end,
    BmpPaletteEntry *palette
) {
    uint16_t packets = read16(f);
    uint32_t index = 0;

    for (uint16_t p = 0; p < packets; p++) {
        int skip = fgetc(f);
        int count = fgetc(f);

        if (skip == EOF || count == EOF)
            break;

        index += (uint32_t)skip;

        int colors = count ? count : 256;

        for (int c = 0; c < colors; c++) {
            int r = fgetc(f);
            int g = fgetc(f);
            int b = fgetc(f);

            if (r == EOF || g == EOF || b == EOF)
                break;

            if (index < 256) {
                palette[index].red = (uint8_t)r;
                palette[index].green = (uint8_t)g;
                palette[index].blue = (uint8_t)b;
            }

            index++;
        }
    }

    fseek(f, chunk_end, SEEK_SET);
}

static void put_pixel(
    uint8_t *sheet,
    int sheet_w,
    int sheet_h,
    int frame,
    int frame_w,
    int x,
    int y,
    uint8_t color
) {
    int dx = frame * frame_w + x;

    if (dx < 0 || dx >= sheet_w ||
        y < 0 || y >= sheet_h)
        return;

    sheet[(size_t)y * sheet_w + dx] = color;
}

bool verbose = false;

int main(int argc, char **argv) {
    verbose = argc == 4 && (strcmp("-v", argv[1]) == 0 || strcmp("--verbose", argv[1]) == 0);

    if (!(argc == 3 || verbose)) {
        fprintf(stderr,
            "Usage: %s [-v|--verbose] input.ase output.bmp\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[verbose ? 2 : 1], "rb");

    if (!f) {
        perror("Could not open the input file");
        return 1;
    }

    // Aseprite file header
    uint32_t file_size = read32(f);
    uint16_t magic = read16(f);
    uint16_t frames = read16(f);
    uint16_t width = read16(f);
    uint16_t height = read16(f);
    uint16_t depth = read16(f);

    if (magic != 0xA5E0) {
        fprintf(stderr, "Invalid Aseprite magic.\n");
        fclose(f);
        return 1;
    }

    if (depth != 8) {
        fprintf(stderr,
            "Expected indexed 8bpp file; got %u bpp.\n", depth);
        fclose(f);
        return 1;
    }

    if (!frames || !width || !height) {
        fprintf(stderr, "Invalid image dimensions.\n");
        fclose(f);
        return 1;
    }

    if ((uint64_t)width * frames > INT32_MAX ||
        (uint64_t)width * frames * height > SIZE_MAX) {
        fprintf(stderr, "Image dimensions too large.\n");
        fclose(f);
        return 1;
    }

    int sheet_w = (int)width * frames;
    int sheet_h = height;

    size_t pixel_count = (size_t)sheet_w * sheet_h;

    uint8_t *sheet = calloc(pixel_count, 1);

    if (!sheet) {
        perror("Could not allocate a buffer for the bitmap");
        fclose(f);
        return 1;
    }

    BmpPaletteEntry palette[256];
    init_palette(palette);

    /* PASS 1: Parse palettes across all frames. */
    fseek(f, 128, SEEK_SET);

    for (int frame = 0; frame < frames; frame++) {
        long frame_start = ftell(f);

        uint32_t frame_size = read32(f);
        uint16_t frame_magic = read16(f);
        uint16_t old_chunks = read16(f);

        read16(f); /* duration */
        fseek(f, 2, SEEK_CUR);

        uint32_t new_chunks = read32(f);
        uint32_t chunk_count =
            new_chunks ? new_chunks : old_chunks;

        if (frame_magic != 0xF1FA ||
            frame_size < 16) {
            fprintf(stderr, "Invalid frame %d.\n", frame);
            goto fail;
        }

        long frame_end = frame_start + frame_size;

        for (uint32_t c = 0; c < chunk_count; c++) {
            long chunk_start = ftell(f);

            uint32_t chunk_size = read32(f);
            uint16_t chunk_type = read16(f);

	    if (verbose)
                printf("Palette scan: frame=%d chunk=0x%04X size=%u\n",
                    frame, chunk_type, chunk_size);

            if (chunk_size < 6 ||
                chunk_start + chunk_size > frame_end) {
                fprintf(stderr, "Invalid chunk size\n");
                goto fail;
            }

            long chunk_end = chunk_start + chunk_size;

            if (chunk_type == 0x2019) {
                if (verbose) printf("FOUND MODERN PALETTE CHUNK\n");
                parse_palette_2019(f, chunk_end, palette);
            }
            else if (chunk_type == 0x0004 || chunk_type == 0x0011) {
                if (verbose) printf("FOUND LEGACY PALETTE CHUNK\n");
                parse_palette_0011(f, chunk_end, palette);
            }

            fseek(f, chunk_end, SEEK_SET);
        }

        fseek(f, frame_end, SEEK_SET);
    }

    /* PASS 2: Extract pixel cels. */
    fseek(f, 128, SEEK_SET);

    for (int frame = 0; frame < frames; frame++) {
        long frame_start = ftell(f);

        uint32_t frame_size = read32(f);
        uint16_t frame_magic = read16(f);
        uint16_t old_chunks = read16(f);

        read16(f);
        fseek(f, 2, SEEK_CUR);

        uint32_t new_chunks = read32(f);
        uint32_t chunk_count =
            new_chunks ? new_chunks : old_chunks;

        if (frame_magic != 0xF1FA ||
            frame_size < 16) {
            fprintf(stderr, "Invalid frame header\n");
            goto fail;
        }

        long frame_end = frame_start + frame_size;

        for (uint32_t c = 0; c < chunk_count; c++) {
            long chunk_start = ftell(f);

            uint32_t chunk_size = read32(f);
            uint16_t chunk_type = read16(f);

            if (chunk_size < 6 ||
                chunk_start + chunk_size > frame_end) {
                fprintf(stderr, "Invalid chunk\n");
                goto fail;
            }

            long chunk_end = chunk_start + chunk_size;

            if (chunk_type != 0x2005) {
                fseek(f, chunk_end, SEEK_SET);
                continue;
            }

            uint16_t layer = read16(f);
            int16_t cel_x = (int16_t)read16(f);
            int16_t cel_y = (int16_t)read16(f);
            uint8_t opacity = (uint8_t)fgetc(f);
            uint16_t cel_type = read16(f);

            fseek(f, 7, SEEK_CUR);

            if (cel_type == 1) {
                // TODO: support linked cels
                printf("Frame %d: linked cel skipped.\n", frame);
                fseek(f, chunk_end, SEEK_SET);
                continue;
            }

            if (cel_type != 0 && cel_type != 2) {
                fseek(f, chunk_end, SEEK_SET);
                continue;
            }

            uint16_t cel_w = read16(f);
            uint16_t cel_h = read16(f);

            if (!cel_w || !cel_h) {
                fseek(f, chunk_end, SEEK_SET);
                continue;
            }

            size_t raw_len =
                (size_t)cel_w * (size_t)cel_h;

            if (raw_len > SIZE_MAX / sizeof(uint8_t)) {
                fseek(f, chunk_end, SEEK_SET);
                continue;
            }

            uint8_t *raw = malloc(raw_len);

            if (!raw) {
                perror("Cel allocation failed");
                goto fail;
            }

            int ready = 0;

            if (cel_type == 0) {
                if (raw_len <= (size_t)(chunk_end - ftell(f)) &&
                    fread(raw, 1, raw_len, f) == raw_len) {
                    ready = 1;
                }            }
            else if (cel_type == 2) {
                long data_start = ftell(f);
                size_t compressed_len =
                    (size_t)(chunk_end - data_start);

                uint8_t *compressed = malloc(compressed_len);

                if (compressed) {
                    if (fread(compressed, 1,
                              compressed_len, f) == compressed_len) {
                        ready = decompress_cel(
                            compressed, compressed_len,
                            raw, raw_len
                        );
                    }

                    free(compressed);
                }
            }

            if (verbose) printf(
                "Frame %d | Layer %u | Cel %ux%u | "
                "Position %d,%d | Type %u | Opacity %u | %s\n",
                frame, layer, cel_w, cel_h,
                cel_x, cel_y, cel_type, opacity,
                ready ? "OK" : "FAILED"
            );

            if (ready) {
                for (int y = 0; y < cel_h; y++) {
                    for (int x = 0; x < cel_w; x++) {
                        put_pixel(
                            sheet,
                            sheet_w,
                            sheet_h,
                            frame,
                            width,
                            cel_x + x,
                            cel_y + y,
                            raw[(size_t)y * cel_w + x]
                        );
                    }
                }
            }

            free(raw);
            fseek(f, chunk_end, SEEK_SET);
        }

        fseek(f, frame_end, SEEK_SET);
    }

    fclose(f);
    f = NULL;

    if (verbose) print_palette(palette);

    /* WRITE BMP */
    FILE *out = fopen(argv[verbose ? 3 : 2], "wb");

    if (!out) {
        perror("Could not open/create the output file");
        free(sheet);
        return 1;
    }

    int row_size = (sheet_w + 3) & ~3;
    uint32_t image_size = (uint32_t)row_size * sheet_h;

    BmpFileHeader bfh = {
        0x4D42,
        0,
        0,
        0,
        sizeof(BmpFileHeader) +
        sizeof(BmpInfoHeader) +
        sizeof(palette)
    };

    bfh.size = bfh.off_bits + image_size;

    BmpInfoHeader bih = {
        sizeof(BmpInfoHeader),
        sheet_w,
        sheet_h,
        1,
        8,
        0,
        image_size,
        2835,
        2835,
        256,
        256
    };

    fwrite(&bfh, sizeof(bfh), 1, out);
    fwrite(&bih, sizeof(bih), 1, out);
    fwrite(palette, sizeof(palette), 1, out);

    uint8_t *padding = calloc(4, 1);

    if (!padding) {
        fclose(out);
        free(sheet);
        return 1;
    }

    int padding_len = row_size - sheet_w;

    for (int y = sheet_h - 1; y >= 0; y--) {
        fwrite(
            sheet + (size_t)y * sheet_w,
            1,
            sheet_w,
            out
        );

        if (padding_len)
            fwrite(padding, 1, padding_len, out);
    }

    free(padding);
    free(sheet);
    fclose(out);

    return 0;

fail:
    if (f) fclose(f);
    free(sheet);
    return 1;
}

