#include "../includes.h"
#include "../error.h"
#include "../graphics.h"
#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

void graphics_init(void){
    if (SDL_Init(SDL_INIT_EVERYTHING)) handle_init_failure("Failed to initialize SDL");
    window = SDL_CreateWindow("Artificial Threat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_HRES, GAME_VRES, 0);
    if (!window) handle_init_failure("Failed to create an SDL window");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) handle_init_failure("Failed to create an SDL renderer");
}
void draw_frame(void){

}
void graphics_cleanup(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
