#ifndef VECFLIP_H
typedef struct vector2 {
    int x, y;
} vector2;

typedef enum flipped {
    O = 0,
    H = 1,
    V = 2,
    VH = 3
} flipped;
#define VECFLIP_H
#endif
