#ifndef PHYSICS_H
typedef struct vector2 {
    int x, y;
} vector2;
typedef struct hitbox {
    vector2 position;
    const vector2 size;
    void *mask;
} hitbox;

#define GRAVITY 15

bool check_collision(hitbox collider_a, hitbox collider_b);
#define PHYSICS_H
#else
#warning "Multiple inclusions of physics.h"
#endif
