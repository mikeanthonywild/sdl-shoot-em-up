/*
 * input.c
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */


#include <stdbool.h>
#include <SDL/SDL.h>

#include "input.h"
#include "entity.h"
#include "render.h"
#include "common.h"
#include "game.h"


// Handle keyboard input and events, update player
bool handle_input(void) {
	// Vars
	SDL_Event event;
	Uint8 *keystate;

	// Update the game time at the start of this frame (needed to lock framerate)
	frame_start_time = SDL_GetTicks();

	// SDL Events
	if (SDL_PollEvent(&event)) {
		// Terminate the program if cross is clicked, or some other type
		// of termination command is issued (SIGTERM etc.)
		if (event.type == SDL_QUIT) {
			return true;
		}

		// Keydown events (just used for player drop-in / drop-out
		if(event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
				case SDLK_F1:
					if (g_player1->lives && !g_gameover) {
						g_player1->active = !g_player1->active;
					}
					break;
				case SDLK_F2:
					if (g_player2->lives && !g_gameover) {
						g_player2->active = !g_player2->active;
					}
					break;
				/*
				case SDLK_l:
					(void) create_projectile(g_player1);
					break;
				case SDLK_h:
					(void) create_projectile(g_player2);
					break;
				*/
				default:
					break;
			}
		}
	}

	// Keyboard input
	// player impulses depending on whether the key was just pressed or released
	keystate = SDL_GetKeyState(NULL);

	// Player 1 movement
	if (g_player1->active) {
		g_player1->frame = 1;					// Reset the frame
		(void) create_projectile(g_player1);	// Auto-fire projectiles
		if (keystate[SDLK_LEFT]) {
			move_player(g_player1, -PLAYER_MOVE_SPEED, 0.0);
		}
		if (keystate[SDLK_RIGHT]		) {
			move_player(g_player1, PLAYER_MOVE_SPEED, 0.0);
		}
		if(keystate[SDLK_UP])
		{
			move_player(g_player1, 0.0, -PLAYER_MOVE_SPEED);
		}
		if(keystate[SDLK_DOWN]) {
			move_player(g_player1, 0.0, PLAYER_MOVE_SPEED);
		}
	}

	// Player 2 movement
	if (g_player2->active) {
		g_player2->frame = 1;					// Reset the frame
		(void) create_projectile(g_player2);	// Auto-fire projectiles
		if (keystate[SDLK_a]) {
			move_player(g_player2, -PLAYER_MOVE_SPEED, 0.0);
		}
		if (keystate[SDLK_d]		) {
			move_player(g_player2, PLAYER_MOVE_SPEED, 0.0);
		}
		if(keystate[SDLK_w])
		{
			move_player(g_player2, 0.0, -PLAYER_MOVE_SPEED);
		}
		if(keystate[SDLK_s]) {
			move_player(g_player2, 0.0, PLAYER_MOVE_SPEED);
		}
	}

	// Miscellaneous keys
	if (keystate[SDLK_ESCAPE]) {
		return true;
	}

	// Don't terminate unless SDL_QUIT event received
	return false;
}


// Moves player, making sure it doesn't go off screen
void move_player(Entity *player, float dx, float dy) {
	// Make sure we don't move off the screen
	if (player->position.x + dx < 0.0 || player->position.x + dx > WINDOW_WIDTH - player->frame_size ||
		player->position.y + dy < 0.0 || player->position.y + dy > WINDOW_HEIGHT - player->frame_size) {
		return;
	}


	// Animation
	if (dx < 0) {
		player->frame = 0;
		g_backdropx -= 0.1;
	} else if (dx > 0) {
		player->frame = 2;
		g_backdropx += 0.1;
	}

	player->position.x += dx;
	player->position.y += dy;

	// Scroll the stars a bit in the direction we're travelling
	g_stary += -dy * STAR_SPEED / 10;

	// Update the state machine
	//player->state = S_MOVE;
}
