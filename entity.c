/*
 * entity.c
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */


#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <SDL/SDL_mixer.h>

#include "entity.h"
#include "resources.h"
#include "render.h"
#include "common.h"
#include "game.h"
#include "sound.h"


Entity g_entities[MAX_ENTITIES];		// Big array of entities
const Entity zero_entity = { 0 };		// Declare entity used to zero values


// Looks for free entity slot and creates entity there
Entity *create_entity(void) {
	int i;
	Entity *e;

	for (i=0; i<MAX_ENTITIES; i++) {
		e = &g_entities[i];
		if (!e->active && e != g_player1 && e != g_player2) {
			g_entities[i] = zero_entity;

			// Fill in some basic attributes in a linked list for easy access to neighbours
			if (i) {
				e->previous = &g_entities[i-1];
				e->previous->next = e;
			}
			e->active = true;
			e->lives = 1;
			e->frame_size = DEFAULT_FRAME_SIZE;
			e->frame_count = DEFAULT_FRAME_COUNT;
			return e;
		}
	}

	return NULL;
}


// Create blank projectile for firing
Entity *create_projectile(Entity *source) {
	// Make sure cooldown time has expired between shots to stop
	// a million entities being created at once
	if (SDL_GetTicks() < source->last_shot + WEAPON_COOLDOWN_TIME) {
		return NULL;
	}

	Entity *e = create_entity();
	assert(e);

	// Set attributes
	// Position in centre just above source ship
	e->type = E_PROJECTILE;
	e->owner = source;
	e->image = g_fire_img;
	e->position.x = source->position.x + (source->frame_size / 2 - e->image->w / 2);
	e->position.y = source->position.y - (e->image->h - 5);
	e->impulse = projectile_impulse;
	e->health = PROJECTILE_DAMAGE;
	e->damage = PROJECTILE_DAMAGE;

	// Reset the cooldown timer
	source->last_shot = SDL_GetTicks();

	// Play laser sound
	play_sound(g_laser_snd, NULL, true);

	return e;
}


// Projectile impulse (move in straight line)
void projectile_impulse(Entity *e) {
	e->position.y -= PROJECTILE_MOVE_SPEED;
}


// Create explosion
Entity *create_explosion(Entity *source) {
	Entity *e = create_entity();
	assert(e);

	// Set attributes
	// Position by source ship
	e->type = E_EXPLOSION;
	e->impulse = explosion_impulse;
	e->image = g_explosion_img;
	e->frame_count = EXPLOSION_FRAME_COUNT;
	e->frame_size = EXPLOSION_FRAME_SIZE;
	e->position.x = source->position.x + source->frame_size / 2 - e->frame_size / 2;
	e->position.y = source->position.y + source->frame_size / 2 - e->frame_size / 2;
	e->health = 0.0;
	e->damage = 0.0;
	e->last_shot = SDL_GetTicks();	// Use last_shot to figure out when it was created

	// Play explosion sound
	play_sound(g_explode_snd, NULL, true);

	return e;
}

// Kill explosion after it expires
// FIXME: Kind of hacky using an impulse for this
void explosion_impulse(Entity *e) {
	if (SDL_GetTicks() > e->last_shot + EXPLOSION_DURATION) {
		destroy_entity(e);
	}
}


// Create player
Entity *create_player(int player) {
	Entity *e = create_entity();
	assert(e);

	e->type = E_PLAYER;
	e->ghost = g_player_ghost_img;
	e->frame_count = PLAYER_FRAME_COUNT;
	e->lives = PLAYER_LIVES;
	e->position.x = WINDOW_WIDTH / 2 - e->frame_size / 2;
	e->position.y = WINDOW_HEIGHT - e->frame_size;
	switch (player) {
		case 1:
			e->image = g_player1_img;
			break;
		default:
			e->image = g_player2_img;
			e->active = false;
			break;
	}

	return e;
}


// Create basic test enemy - REMOVE BEFORE RELEASE!!!
Entity *create_test_enemy(void) {
	Entity *e = create_entity();
	assert(e);
	e->type = E_BASIC_ENEMY;
	e->is_enemy = true;

	// Position at top of screen and move down in a line

	e->impulse = test_enemy_impulse;

	// Other attribs
	e->image = g_test_enemy_img;
	e->frame_count = TEST_ENEMY_FRAME_COUNT;
	e->points = TEST_ENEMY_POINTS;
	e->health = TEST_ENEMY_HEALTH;
	e->damage = TEST_ENEMY_HEALTH;

	return e;
}


// Test impulse (move in straight line)
void test_enemy_impulse(Entity *e) {
	e->position.y += TEST_ENEMY_MOVE_SPEED;
}


// Enemy moves from side to side
Entity *create_curve_enemy(void) {
    Entity *e = create_entity();
    assert(e);
    e->type = E_CURVE_ENEMY;
    e->is_enemy = true;

    // Position at top of screen and move down in a line
    e->image = g_curve_enemy_img;
    e->impulse = curve_enemy_impulse;
    e->points = CURVE_ENEMY_POINTS;
    e->health = CURVE_ENEMY_HEALTH;
    e->damage = CURVE_ENEMY_HEALTH;

    return e;
}


// Test impulse (move in straight line)
void curve_enemy_impulse(Entity *e) {
	const float pi = 3.14159;
	const float f = CURVE_ENEMY_MOVE_FREQ;
	const float t = SDL_GetTicks() / 1000.0;
	const float dx = WINDOW_WIDTH * (1 + sin(pi * f * t)) / 2 - e->position.x;
    const float dy = CURVE_ENEMY_MOVE_SPEED;
    float theta = atan(dy / dx);
	// Second and third quadrants
	if ((dx < 0 && dy > 0) || (dx < 0 && dy < 0)) {
		theta += pi;
	}

	e->position.x += dx;
	e->position.y += dy;
	e->angle = theta;
}


Entity *create_fast_enemy(void) {
    Entity *e = create_entity();
    assert(e);
    e->type = E_FAST_ENEMY;
    e->is_enemy = true;

    // Position at top of screen and move down in a line
    e->image = g_fast_enemy_img;
    e->impulse = fast_enemy_impulse;
    e->points = FAST_ENEMY_POINTS;
    e->health = FAST_ENEMY_HEALTH;
    e->damage = FAST_ENEMY_HEALTH;

    return e;
}


// Test impulse (move in straight line)
void fast_enemy_impulse(Entity *e) {
    e->position.y += FAST_ENEMY_MOVE_SPEED;
}


// Final boss enemy
Entity *create_boss_enemy(void) {
    Entity *e = create_entity();
    assert(e);
    e->type = E_BOSS_ENEMY;
    e->is_enemy = true;

    // Position at top of screen and move down in a line
    e->image = g_boss_enemy_img;
    e->frame_count = BOSS_ENEMY_FRAME_COUNT;
    e->frame_size = BOSS_ENEMY_FRAME_SIZE;
    e->impulse = boss_enemy_impulse;
    e->points = BOSS_ENEMY_POINTS;
    e->health = BOSS_ENEMY_HEALTH;
    e->damage = BOSS_ENEMY_HEALTH;

    return e;
}


// Boss impulse (move in straight line)
void boss_enemy_impulse(Entity *e) {
    e->position.y += BOSS_ENEMY_MOVE_SPEED;
}


// Returns any entities colliding with entity specified
Entity *get_entity_collision(Entity *src) {
	int i;
	Entity *dst;

	// Test each entity to see whether collision circles intersect
	for (i=0; i<MAX_ENTITIES; i++) {
		dst = &g_entities[i];

		// Skip if entity not active or we're testing against ourselves, don't test for collisions between two entities
		// of the same type either (to stop them colliding with themselves). Don't test on explosions either
		if (dst->active && src != dst && src->is_enemy != dst->is_enemy && !src->last_respawn && !dst->last_respawn && \
				src->type != E_EXPLOSION && dst->type != E_EXPLOSION) {
			// Check if collision circles intersect
			if (get_distance(src->position.x, src->position.y, dst->position.x, dst->position.y) <  sqrt(src->frame_size / 2) + dst->frame_size / 2 &&\
					src->position.y > 0.0 && dst->position.y > 0.0) {

				// Record who's hitting it so we can allocate points
				if (src->type == E_PROJECTILE) {
					dst->shot_by = src->owner;
				} else if (dst->type == E_PROJECTILE) {
					src->shot_by = dst->owner;
				}

				// Hit sound
				play_sound(g_hit_snd, NULL, true);

				return dst;
			}
		}
	}

	return NULL;
}


// Handle take damage state
state_t damage_entity(Entity *e, float damage) {
	// Destroy the entity if it's out of health
	e->health -= damage;
	if (e->health < 0.0) {
		return S_DIE;
	}

	return e->state;
}


// Marks an entity inactive so it's not updated
void destroy_entity(Entity *e) {
	// Allocate points and play sounds
	if (e->shot_by) {
		e->shot_by->points += e->points;

		int points = g_player1->points;
		if (points == FIRSTBLOOD_POINTS && points < MEGAKILL_POINTS) {
			play_sound(g_firstblood_snd, &g_firstblood_snd_played, false);
		} else if (points > MEGAKILL_POINTS && points < ULTRAKILL_POINTS) {
			play_sound(g_megakill_snd, &g_megakill_snd_played, false);
		} else if (points > ULTRAKILL_POINTS && points < RAMPAGE_POINTS) {
			play_sound(g_ultrakill_snd, &g_ultrakill_snd_played, false);
		} else if (points > RAMPAGE_POINTS && points < GODLIKE_POINTS) {
			play_sound(g_rampage_snd, &g_rampage_snd_played, false);
		} else if (points > GODLIKE_POINTS && e != g_boss) {
			play_sound(g_godlike_snd, &g_godlike_snd_played, false);
		}

		// Explosion
		if (e->type != E_EXPLOSION) {
			(void) create_explosion(e);
			if (e->type == E_BOSS_ENEMY) {
				printf("hi");
			}
		}
	}

	// Endgame state if player is hit or boss dies
	if ((!g_player1->lives && g_player1->active && !g_player2->lives && g_player2->active) || \
			(!g_player1->lives && !g_player2->active || !g_player2->lives && !g_player1->active)) {
		g_gameover = true;
		play_sound(g_die_snd, &g_die_snd_played, false);
	}
	if (e == g_boss) {
		g_gameover = true;
		play_sound(g_holyshit_snd, &g_holyshit_snd_played, false);
		// Disable both players
		g_player1->active = false;
		g_player2->active = false;
	}

	// Mark inactive
	e->active = false;
}


// Randomly spawns enemies depending on time elapsed
// TODO: Tidy this obscenity up
void spawn_enemies(int elapsed) {
	// vars
	Entity *e;
	int rnd = rand();

	// Figure out which stage of the level we're at
	// Level 1
	if (elapsed < LEVEL2_START_TIME) {
		if (elapsed % LEVEL1_SPAWN_RATE < TIMER_TOLERANCE) {
			e = create_test_enemy();
			e->position.x = rand() % WINDOW_WIDTH;
			e->position.y = -e->frame_size;
		}

	// Level 2
	} else if (elapsed > LEVEL2_START_TIME && elapsed < LEVEL3_START_TIME) {
		if (elapsed % LEVEL2_SPAWN_RATE < TIMER_TOLERANCE) {
			switch (rnd % 2) {
				case 0:
					e = create_test_enemy();
					break;
				case 1:
					e = create_curve_enemy();
					break;
			}
			e->position.x = rand() % WINDOW_WIDTH;
			e->position.y = -e->frame_size;
		}

	// Level 3
	} else if (elapsed > LEVEL3_START_TIME) {
		if (elapsed % LEVEL3_SPAWN_RATE < TIMER_TOLERANCE) {
			switch (rnd % 3) {
				case 0:
					e = create_test_enemy();
					break;
				case 1:
					e = create_curve_enemy();
					break;
				case 2:
					e = create_fast_enemy();
					break;
			}
			e->position.x = rand() % WINDOW_WIDTH;
			e->position.y = -e->frame_size;
		}
	}

	// Boss
	if (elapsed > BOSS_START_TIME) {
		if (!g_boss) {
			g_boss = create_boss_enemy();
			g_boss->position.x = WINDOW_WIDTH / 2 - (g_boss->frame_size / 2);
			g_boss->position.y = -g_boss->frame_size;
		}
	}
}
