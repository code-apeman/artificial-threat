#include "includes.h"
#include "game.h"

bool game_exit_flag = false;

int main(int argc, char** argv){
	allegro_init();			// initialize the Allegro library
	game_init();			// initialize the game

	do {				// loop
		game_input();		// get and process input
		game_logic();		// process the game logic (movement, enemies, etc.)
		game_draw();		// it's pronounced "jraphics"
	} while (!game_exit_flag);	// until the flag is set
	
	game_shutdown();		// saying goodbyes
	allegro_exit();			// final farewells
	return 0;			// tell the OS we're all good
}
END_OF_MAIN()
