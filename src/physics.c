#include "includes.h"
#include "physics.h"

size_t collider_count = 0;
hitbox* colliders = NULL;

hitbox create_hitbox(int position_x, int position_y, int width, int height, int origin_x, int origin_y, void *mask){
    hitbox collider = {{position_x, position_y}, {width, height}, {origin_x, origin_y}, mask};
    colliders = realloc(colliders, sizeof(hitbox) * (collider_count + 1));
    colliders[collider_count] = collider;
    collider_count++;
    return collider;
}

void remove_hitbox(hitbox collider){
    size_t index = -1;
    for (size_t i = 0; i < collider_count; i++)
        if (
            colliders[i].position.x == collider.position.x &&
            colliders[i].position.y == collider.position.y &&
            colliders[i].size.x == collider.size.x &&
            colliders[i].size.y == collider.size.y &&
            colliders[i].origin.x == collider.origin.x &&
            colliders[i].origin.y == collider.origin.y &&
            colliders[i].mask == collider.mask) {
        index = i; break;
    } if (index == -1) return;
    collider_count--;
    for (size_t i = index; i < collider_count; i++) colliders[i] = colliders[i + 1];
    colliders = realloc(colliders, sizeof(hitbox) * collider_count);
}

vector2 *get_hitbox_points(hitbox collider){
    vector2 points[4] = {
        {collider.position.x - collider.origin.x, collider.position.y - collider.origin.y},
        {collider.position.x + (collider.size.x - collider.origin.x), collider.position.y - collider.origin.y},
        {collider.position.x - collider.origin.x, collider.position.y + (collider.size.y - collider.origin.y)},
        {collider.position.x + (collider.size.x - collider.origin.x), collider.position.y + (collider.size.y - collider.origin.y)},
    }; return points;
}

bool check_collision(hitbox collider_a, hitbox collider_b){
    vector2 *points_a = get_hitbox_points(collider_a);
    vector2 *points_b = get_hitbox_points(collider_b);
    if (
        points_a[1].x < points_b[0].x ||
        points_a[0].x > points_b[1].x ||
        points_a[2].y < points_b[0].y ||
        points_a[0].y > points_b[2].y
    ) {free(points_a); free(points_b); return false;}
    if (collider_a.mask == NULL && collider_b.mask == NULL) {free(points_a); free(points_b); return true;}
    // TODO: add an actual collision checking algorithm for masked hitboxes
    return true;
}
