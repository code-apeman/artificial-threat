#include <SDL2/SDL_timer.h>
#include "../yield.h"

// Frametime for 60 FPS is 16.(6) ms, we round that to 17
#define FRAMETIME_MS 17

uint64_t last_milliseconds = 0;

void yield_frame(void){
    while (SDL_GetTicks64() < (last_milliseconds + FRAMETIME_MS))
        SDL_Delay(1); // highly sophisticated thread yield function do not steal
    last_milliseconds = SDL_GetTicks64();
}
