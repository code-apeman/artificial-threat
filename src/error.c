#include "includes.h"
#include "error.h"
#include "textcolors.h"

void handle_init_error(const char* error, const char* function_name){
    allegro_message("%s%sINITIALIZATION ERROR%s: %s%s%s%s at %s%s%s%s.\n",
			ANSI_COLOR_BRIGHT_RED,
			ANSI_BOLD,
			// ERROR
			ANSI_RESET,
			// :
			ANSI_BOLD,
			ANSI_COLOR_BRIGHT_WHITE,
			error,
			ANSI_RESET,
			// at
			ANSI_BOLD,
			ANSI_COLOR_BRIGHT_WHITE,
			function_name,
			ANSI_RESET);
	exit(-1);
}

void handle_init_failure(const char* function_name){
   handle_init_error(allegro_error, function_name); 
}
