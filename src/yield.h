#ifndef YIELD_H
void timer_init(void);
void yield_frame(void);
#define YIELD_H
#else
#warning "Multiple inclusions of \"yield.h\""
#endif
