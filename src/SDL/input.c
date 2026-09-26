#include <SDL2/SDL.h>
#include "../input.h"

SDL_Event current_event;
extern bool game_exit_flag;

void input_init(void){
    // stub; no additional initialization needed for input on SDL
}
unsigned char input_collect(void){
    while(SDL_PollEvent(&current_event)){
        switch (current_event.type) {
            case SDL_KEYDOWN:
                switch (current_event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        game_exit_flag = true; break;
                    default: break;
                }
            default: break;
        }
    } return 0;
}
