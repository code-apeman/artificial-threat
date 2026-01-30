#ifndef GAME_H
void game_init(void);
void game_input(void);
void game_logic(void);
void game_draw(void);
void game_shutdown(void);
#define GAME_H
#else
#warning "Multiple inclusions of game.h"
#endif
