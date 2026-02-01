#ifndef SOUND_H
#include <allegro.h>
void sound_init(void);
SAMPLE* allegro_sample_from_module(void* mod_data, size_t mod_size);
#define SOUND_H
#else
#warning "Multiple inclusions of sound.h"
#endif
