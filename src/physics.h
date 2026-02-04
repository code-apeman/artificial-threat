#ifndef PHYSICS_H
typedef struct vector2 {
    int x, y;
} vector2;
typedef struct hitbox {
    vector2 position;
    const vector2 size;
} hitbox;
typedef struct masked_hitbox {
    hitbox bounds;
    void *mask;
} masked_hitbox;

#define GRAVITY 15

#define PHYSICS_H
#else
#warning "Multiple inclusions of physics.h"
#endif
