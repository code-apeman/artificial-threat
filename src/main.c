#include "includes.h"
#include "yield.h"
#include "game.h"
#ifdef USE_ALLEGRO
#include <allegro.h>
#endif

bool game_exit_flag = false;

int main(int argc, char** argv){
    game_init();                // initialize the game

    do {                        // loop...:
        game_input();           // get and process input
        game_logic();           // process the game logic (movement, enemies, etc.)
        game_draw();            // it's pronounced "jraphics"
        yield_frame();          // ..otherwise, don't waste cpu time that much
    } while (!game_exit_flag);    // ...until the exit flag is set

    game_shutdown();            // saying goodbyes
    return 0;                   // tell the OS we're all good
}
#ifdef USE_ALLEGRO
END_OF_MAIN()
#endif
