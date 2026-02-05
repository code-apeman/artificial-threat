#include "includes.h"
#include "physics.h"
#include <stdio.h>

size_t collider_count = 0;
hitbox* colliders = NULL;

hitbox create_hitbox(int position_x, int position_y, int width, int height, int origin_x, int origin_y, void *mask){
    hitbox collider = {collider_count, {position_x, position_y}, {width, height}, {origin_x, origin_y}, mask};
    colliders = realloc(colliders, sizeof(hitbox) * (collider_count + 1));
    colliders[collider_count] = collider;
    collider_count++;
    return collider;
}

void remove_hitbox(hitbox collider){
    collider_count--;
    for (size_t i = collider.id; i < collider_count; i++) colliders[i] = colliders[i + 1];
    colliders = realloc(colliders, sizeof(hitbox) * collider_count);
    if (collider.mask) free(collider.mask);
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
    // if both are NULL, we don't bother assigning masks since we know already that the hitboxes overlap
    if ((!collider_a.mask) && (!collider_b.mask)) return true;
    // and now for the fun part: collision check of bitmasked hitboxes
    // first we assign an all-1 mask to the NULL-mask hitbox
    void *mask_a, *mask_b;
    size_t mask_a_bytesize = ((collider_a.size.x * collider_a.size.y) / 8) + (collider_a.size.x * collider_a.size.y % 8 > 0);
    size_t mask_b_bytesize = ((collider_b.size.x * collider_b.size.y) / 8) + (collider_b.size.x * collider_b.size.y % 8 > 0);
    if (!collider_a.mask) {
        mask_a = malloc(mask_a_bytesize);
        for (int i = 0; i < mask_a_bytesize; i++) ((char*)mask_a)[i] = 0xFF;
    } else mask_a = collider_a.mask; 
    if (!collider_b.mask) {
        mask_b = malloc(mask_b_bytesize);
        for (int i = 0; i < mask_b_bytesize; i++) ((char*)mask_b)[i] = 0xFF;
    } else mask_b = collider_b.mask;
    // and now we calculate the size and the coords of the overlap
    // some checks we'll need the results of later
    bool a_righter_than_b = points_a[0].x > points_b[0].x;
    bool a_lower_than_b = points_a[0].y > points_b[0].y;
    // corner inside bounding box A, relative to points_a[0]
    vector2 overlap_corner_a = {
        ((a_righter_than_b)
            ? points_b[3].x - points_a[0].x
            : points_b[0].x - points_a[0].x),
        ((a_lower_than_b)
            ? points_b[3].y - points_a[0].y
            : points_b[0].y - points_a[0].y)
    };
    // corner inside bounding box B, relative to points_b[0]
    vector2 overlap_corner_b = {
        ((a_righter_than_b)
            ? points_a[0].x - points_a[0].x
            : points_a[3].x - points_a[0].x),
        ((a_lower_than_b)
            ? points_a[0].y - points_b[0].y
            : points_a[3].y - points_b[0].y)
    };
    // second corner index, as in:
    // 0--1
    // |  |
    // 2--3
    unsigned char corner2index_a = (((char)  a_lower_than_b) << 1) | ((char)  a_righter_than_b);
    unsigned char corner2index_b = (((char) !a_lower_than_b) << 1) | ((char) !a_righter_than_b);
    // and now the fun (/s) part: evil bitwise fuckery
    if (!collider_a.mask) free(mask_a);
    if (!collider_b.mask) free(mask_b);
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
