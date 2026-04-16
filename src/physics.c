#include "includes.h"
#include "physics.h"

size_t collider_count = 0;
hitbox* colliders = NULL;

hitbox create_hitbox(int position_x, int position_y, int width, int height, int origin_x, int origin_y, bool flipped_x, bool flipped_y, bool slope){
    hitbox collider = {collider_count, {position_x, position_y}, {width, height}, {origin_x, origin_y}, (flipped_y * 2) ^ flipped_x, slope};
    colliders = realloc(colliders, sizeof(hitbox) * (collider_count + 1));
    colliders[collider_count] = collider;
    collider_count++;
    return collider;
}

void remove_hitbox(hitbox collider){
    collider_count--;
    for (size_t i = collider.id; i < collider_count; i++) colliders[i] = colliders[i + 1];
    colliders = realloc(colliders, sizeof(hitbox) * collider_count);
}

void get_hitbox_points(hitbox collider, vector2 *points){
    points[0] = (vector2){collider.position.x - collider.origin.x, collider.position.y - collider.origin.y};
    points[1] = (vector2){collider.position.x + (collider.size.x - collider.origin.x), collider.position.y - collider.origin.y};
    points[2] = (vector2){collider.position.x - collider.origin.x, collider.position.y + (collider.size.y - collider.origin.y)};
    points[3] = (vector2){collider.position.x + (collider.size.x - collider.origin.x), collider.position.y + (collider.size.y - collider.origin.y)};
}

bool check_collision(hitbox collider_a, hitbox collider_b){
    vector2 points_a[4];
    get_hitbox_points(collider_a, points_a);
    vector2 points_b[4];
    get_hitbox_points(collider_b, points_b);
    if (
        (points_a[0].x < points_b[0].x && points_a[3].x < points_b[0].x) ||
        (points_a[0].x > points_b[3].x && points_a[3].x > points_b[3].x) ||
        (points_a[0].y < points_b[0].y && points_a[3].y < points_b[0].y) ||
        (points_a[0].y > points_b[3].y && points_a[3].y > points_b[3].y)
    ) return false;
    // if both aren't slopes they do in fact collide
    if ((!collider_a.is_slope) && (!collider_b.is_slope)) return true;
    // TODO: slope collision checks
    return true;
}

bool check_moving(hitbox collider, int delta_x, int delta_y){
    hitbox collider_copy = collider;
    collider_copy.position.x += delta_x;
    collider_copy.position.y += delta_y;
    for (size_t i = 0; i < collider_count; i++) {
        if (colliders[i].id == collider.id) continue;
        if (check_collision(collider_copy, colliders[i])) return false;
    } return true;
}
