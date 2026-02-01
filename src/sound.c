#include "includes.h"
#include "sound.h"
#include "error.h"

int freq, bits, chcount;

void sound_init(){
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0)
        handle_init_failure("sound_init() (sound.c)");
    freq = get_mixer_frequency();
    bits = get_mixer_bits();
    chcount = get_mixer_channels();
}

SAMPLE* allegro_sample_from_module(void* mod_data, size_t mod_size){
    int error = OPENMPT_ERROR_OK;
    const char* error_message = NULL;
    openmpt_module* module = openmpt_module_create_from_memory2(mod_data, mod_size, NULL, NULL, NULL, NULL, &error, &error_message, NULL);
    if (!module) {
        openmpt_free_string(error_message);
        error_message = NULL;
        return NULL;
    }
    double duration = openmpt_module_get_duration_seconds(module);
    int pcm_length = (int)((double)freq * duration);
    SAMPLE* resulting_sample = create_sample(bits, 0, freq, pcm_length);
    if (openmpt_module_read_mono(module,
                    freq, pcm_length,
                    resulting_sample->data) != pcm_length) return NULL;
    return resulting_sample;
}
