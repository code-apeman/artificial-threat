#include "includes.h"
#include "sound.h"
#include "error.h"

AUDIOSTREAM* music_stream;
openmpt_module* loaded_module;
int freq, bits, chcount;
bool buffer_refill_flag, song_play_flag;

void sound_init(){
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0)
        handle_init_failure("sound_init() (sound.c)");
    freq = get_mixer_frequency();
    bits = get_mixer_bits();
    chcount = get_mixer_channels();
    music_stream = play_audio_stream(BUFFER_SIZE, 16, (chcount > 1) ? 1 : 0, freq, 255, 128);
}

// TODO: use AUDIOSTREAM instead of (VERY!) unreliably determining the module length and converting it to a regular sample
SAMPLE* allegro_sample_from_module(void* mod_data, size_t mod_size){
    handle_deprecation("allegro_sample_from_module");
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
    SAMPLE* resulting_sample = create_sample(16, (chcount > 1) ? 1 : 0, freq, pcm_length);
    if (chcount > 1) {
        int16_t left[pcm_length];
        int16_t right[pcm_length];
        if (openmpt_module_read_stereo(module,
                freq, pcm_length,
                left, right)
        != pcm_length) return NULL;
        for (int i = 0; i < pcm_length; i++) {
            ((int16_t*)resulting_sample->data)[i * 2] = left[i];
            ((uint16_t*)resulting_sample->data)[i * 2] ^= 0x8000; // signed to unsigned conversion
            ((int16_t*)resulting_sample->data)[(i * 2) + 1] = right[i];
            ((uint16_t*)resulting_sample->data)[(i * 2) + 1] ^= 0x8000;
        }
    } else {
        if (openmpt_module_read_mono(module,
                freq, pcm_length,
               resulting_sample->data)
        != pcm_length) return NULL;
        for (int i = 0; i < pcm_length; i++)
            ((uint16_t*)resulting_sample->data)[i] ^= 0x8000;
    } return resulting_sample;
}

bool load_module(void* mod_data, size_t mod_size){
    int error = OPENMPT_ERROR_OK;
    const char* error_message = NULL;
    loaded_module = openmpt_module_create_from_memory2(mod_data, mod_size, NULL, NULL, NULL, NULL, &error, &error_message, NULL);
    if (!loaded_module) {
        handle_load_error(error_message, "load_module");
        openmpt_free_string(error_message);
        error_message = NULL;
        return false;
    } openmpt_module_ctl_set_text(loaded_module, "play.at_end", "continue");
    return true;
}

void refill_buffer(void* buffer){
    if (!buffer) return;
    if (!loaded_module) return;
    if (chcount > 1) {
        int16_t left[BUFFER_SIZE];
        int16_t right[BUFFER_SIZE];
        song_play_flag = !(openmpt_module_read_stereo(loaded_module,
                    freq, BUFFER_SIZE, left, right)
                < BUFFER_SIZE);
        for (int i = 0; i < BUFFER_SIZE; i++) {
            ((int16_t*)buffer)[i * 2] = left[i];
            ((uint16_t*)buffer)[i * 2] ^= 0x8000; // signed to unsigned conversion
            ((int16_t*)buffer)[(i * 2) + 1] = right[i];
            ((uint16_t*)buffer)[(i * 2) + 1] ^= 0x8000;
        }
    } else {
        song_play_flag = !(openmpt_module_read_mono(loaded_module,
                    freq, BUFFER_SIZE, buffer)
                < BUFFER_SIZE);
        for (int i = 0; i < BUFFER_SIZE; i++)
            ((uint16_t*)buffer)[i] ^= 0x8000;
    } free_audio_stream_buffer(music_stream);
}

void buffer_check_callback(void){
    void* buffer = get_audio_stream_buffer(music_stream);
    if (buffer) refill_buffer(buffer);
}

bool play_module(void){
    if (!loaded_module) return false;
    refill_buffer(get_audio_stream_buffer(music_stream));
    return true;
}
