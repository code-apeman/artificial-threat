#ifndef SOUND_H
#include "includes.h"
#define BUFFER_SIZE (1024)
void sound_init(void);
SAMPLE* allegro_sample_from_module(void* mod_data, size_t mod_size);
bool load_module(void* mod_data, size_t mod_size);
bool play_module(void);
void buffer_check_callback(void);
#define SOUND_H
#else
#warning "Multiple inclusions of sound.h"
#endif
