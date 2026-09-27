#ifndef SOUND_H
#include <stddef.h>
#include "memchunk.h"
#define BUFFER_SIZE (1024)
void sound_init(void);
bool load_module(memchunk *mod_chunk);
bool play_module(void);
#define SOUND_H
#else
#warning "Multiple inclusions of sound.h"
#endif
