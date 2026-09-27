#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes.h"
#include "../error.h"
#include "../graphics.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture **asset_vector;
graphics_asset_id asset_count = 0;
graphics_asset_id background_id = 65535;

void graphics_init(void){
    if (SDL_Init(SDL_INIT_EVERYTHING)) handle_init_failure("Failed to initialize SDL");
    window = SDL_CreateWindow("Artificial Threat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_HRES, GAME_VRES, 0);
    if (!window) handle_init_failure("Failed to create an SDL window");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) handle_init_failure("Failed to create an SDL renderer");
    asset_vector = malloc(sizeof(SDL_Texture*) * 65536);
    SDL_RenderClear(renderer);
}
graphics_asset_id load_graphics_asset(memchunk* bitmap_chunk){
    SDL_RWops *asset_buffer = SDL_RWFromConstMem(bitmap_chunk->pointer, bitmap_chunk->size);
    asset_count++;
    asset_vector[asset_count - 1] = IMG_LoadTexture_RW(renderer, asset_buffer, 1);
    return asset_count - 1;
}
void set_background(graphics_asset_id asset){
    background_id = asset;
}
void draw_sprite(vector2 pos, vector2 size, flipped flip, char framecount, char framenum, graphics_asset_id asset){
    SDL_Rect sprite_rect = {pos.x, pos.y, size.x, size.y};
    SDL_Rect frame_rect = {(framecount - 1 - framenum) * size.x, 0, size.x, size.y};
    SDL_RenderCopyEx(renderer, asset_vector[asset], &frame_rect, &sprite_rect, 0.0, NULL, (SDL_RendererFlip) flip); // "vecflip.h"'s flipped struct is, coincidentally, a carbon copy of SDL_RendererFlip
}
void draw_frame(void){
    if (asset_vector[background_id]) SDL_RenderCopy(renderer, asset_vector[background_id], NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}
void graphics_cleanup(void){
    for (int i = 0; i < asset_count; i++) SDL_DestroyTexture(asset_vector[i]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
