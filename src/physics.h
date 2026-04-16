#ifndef PHYSICS_H
typedef struct vector2 {
    int x, y;
} vector2;

typedef enum flipped {
    O = 0,
    H = 1,
    V = 2,
    VH = 3
} flipped;

// mask (if not NULL) should always have (((size.x * size.y) / 8) + ((size.x % 8 > 0) * size.y)) bytes allocated in memory, otherwise it would segfault
typedef struct hitbox {
    unsigned int id;
    vector2 position, size, origin;
    flipped flipped;
    bool is_slope;
} hitbox;

#define GRAVITY 0.2f

hitbox create_hitbox(int position_x, int position_y, int width, int height, int origin_x, int origin_y, bool flipped_x, bool flipped_y, bool slope);
bool same_hitbox(hitbox collider_a, hitbox collider_b);
void remove_hitbox(hitbox collider);
void get_hitbox_points(hitbox collider, vector2 *points);
bool check_collision(hitbox collider_a, hitbox collider_b);
bool check_moving(hitbox collider, int delta_x, int delta_y);
#define PHYSICS_H
#else
#warning "Multiple inclusions of physics.h"
#endif
