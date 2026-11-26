// TODO: make the header more readable; clean up all the fluff
#ifndef RENDER_H
#include "graphics.h"
void render_init(void);
void render_frame(void);
void render_cleanup(void);
#define RENDER_H
#else
#warning "Multiple inclusions of graphics.h"
#endif
