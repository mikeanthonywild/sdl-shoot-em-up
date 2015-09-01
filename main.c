/*
 * main.c
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "input.h"
#include "game.h"
#include "render.h"
#include "resources.h"


// Main entry point
int main(void) {
	// Local vars
	bool quit = false;

	// Initialise everything
	init_screen();
	load_resources();
	init_game();

	// Main loop
	while (!quit) {
		// TODO: Decouple input, logic and render loop timings
		// TODO: Replace polling architecture with SDL events
		quit = handle_input();
		update_game();
		update_screen();
	}

	// Clean up and terminate
	return EXIT_SUCCESS;
}
