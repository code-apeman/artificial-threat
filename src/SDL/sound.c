#include <SDL2/SDL_mixer.h>
#include "../includes.h"
#include "../sound.h"
#include "../error.h"

Mix_Music *mixer_music;

void sound_init(){
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
                      MIX_DEFAULT_CHANNELS, 1024)){
        handle_init_error("Failed to initialize audio.", "sound.c");
        exit(-1);
    }
}

bool load_module(void* mod_data, size_t mod_size){
    if (mixer_music) Mix_FreeMusic(mixer_music);
    SDL_RWops *music_buffer = SDL_RWFromMem(mod_data, mod_size);
    if (!music_buffer) return false;
    mixer_music = Mix_LoadMUS_RW(music_buffer, 0);
    if (!mixer_music) return false;
    return true;
}

bool play_module(void){
    Mix_PlayMusic(mixer_music, -1);
    return true;
}
