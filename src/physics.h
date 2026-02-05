#ifndef PHYSICS_H
typedef struct vector2 {
    int x, y;
} vector2;
typedef struct hitbox {
    vector2 position, size, origin;
    void *mask;
} hitbox;

#define GRAVITY 15

hitbox create_hitbox(int position_x, int position_y, int width, int height, int origin_x, int origin_y, void *mask);
void remove_hitbox(hitbox collider);
vector2 *get_hitbox_points(hitbox collider);
bool check_collision(hitbox collider_a, hitbox collider_b);
#define PHYSICS_H
#else
#warning "Multiple inclusions of physics.h"
#endif
