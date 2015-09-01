/*
 * game.c
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */


#include <stdbool.h>
#include <stdlib.h>

#include "entity.h"
#include "game.h"
#include "render.h"
#include "resources.h"


// Vars
Entity *g_player1;
Entity *g_player2;
Entity *g_boss = NULL;
int g_score1;
int g_score2;
int game_start_time;
bool g_gameover = false;
float g_starx = 0.0;
float g_stary = 0.0;
float g_backdropx;
float g_backdropy;


// Sets up all entities etc.
void init_game(void) {
	g_player1 = create_player(1);
	g_player2 = create_player(2);

	// Record the init time so we know how much game time has elapsed
	game_start_time = SDL_GetTicks();

	// Seed random number generator
	srand(SDL_GetTicks());

	// Start the music
	Mix_PlayMusic(g_music_snd,-1);

	// Position the backdrop
	g_backdropx = WINDOW_WIDTH / 2 - g_backdrop_img->w / 2;
	g_backdropy = -(g_backdrop_img->h - WINDOW_HEIGHT);
}


// Iterates through all game entities and updates state machines
void update_game(void) {
	int i;
	int elapsed;
	Entity *e;

	for (i=0; i<MAX_ENTITIES; i++) {
		e = &g_entities[i];
		if (e->active) {
			e->state = next_state(e);
		}
	}

	// Tmp spawn some enemies randomly (remove after Monday)
	elapsed = SDL_GetTicks() - game_start_time;
	spawn_enemies(elapsed);
}


// Figures out the next state for an entity
state_t next_state(Entity *e) {
	switch(e->state) {
		case S_IDLE:
			return do_idle(e);
		case S_MOVE:
			return do_move(e);
		case S_DIE:
			return do_die(e);
		default:
			return S_IDLE;
	}
}


// Handle idle state
state_t do_idle(Entity *e) {
	// Reset respawn timer
	if (SDL_GetTicks() > e->last_respawn + INVULN_TIME) {
		e->last_respawn = 0;
	}

	// Check if there's an impulse function applied and push into move state
	if (e->impulse) {
		return S_MOVE;
	}

	return S_IDLE;
}


// Handle movement state
state_t do_move(Entity *e) {
	// Locals
	Entity *other_ent;

	// Kill entity if it moves off screen
	if (e->position.x < 0.0 || e->position.x > WINDOW_WIDTH || e->position.y < -e->frame_size || e->position.y > WINDOW_HEIGHT) {
		// Clear the shot_by var
		e->shot_by = NULL;
		return S_DIE;
	}

	// Execute the impulse function
	e->impulse(e);

	// Detect collisions and apply damage
	other_ent = get_entity_collision(e);
	if (other_ent) {
		other_ent->state = damage_entity(other_ent, e->damage);
	}

	return S_MOVE;
}


// Handle death state
state_t do_die(Entity *e) {
	// Take away a life and destroy the entity if no more lives left
	if (--e->lives < 1) {
		destroy_entity(e);
	} else {
		// Position at bottom of screen and set respawn flags
		e->last_respawn = SDL_GetTicks();
		e->position.x = WINDOW_WIDTH / 2 - e->frame_size / 2;
		e->position.y = WINDOW_HEIGHT - e->frame_size;
	}

	return S_IDLE;
}
