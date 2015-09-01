/*
 * entity.h
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */

#ifndef ENTITY_H_
#define ENTITY_H_

// Includes
#include <stdbool.h>
#include <SDL/SDL_image.h>

#include "common.h"


// Constants
#define MAX_ENTITIES	256

// Entity defines
#define PLAYER_MOVE_SPEED		5.0
#define PLAYER_LIVES			3

#define PROJECTILE_DAMAGE		20.0
#define PROJECTILE_MOVE_SPEED	12.0		// Make sure it's faster than player speed so we don't run into it

#define EXPLOSION_DURATION		1000

#define TEST_ENEMY_POINTS		10
#define TEST_ENEMY_HEALTH		100.0
#define TEST_ENEMY_MOVE_SPEED	1.0

#define CURVE_ENEMY_POINTS		100
#define CURVE_ENEMY_HEALTH		40.0
#define CURVE_ENEMY_MOVE_SPEED	3.0
#define CURVE_ENEMY_MOVE_FREQ	0.20

#define FAST_ENEMY_POINTS		400
#define FAST_ENEMY_HEALTH		40.0
#define FAST_ENEMY_MOVE_SPEED  9.0

#define BOSS_ENEMY_POINTS		20000
#define BOSS_ENEMY_HEALTH		20000.0
#define BOSS_ENEMY_MOVE_SPEED  0.2

// Structs & typedefs
struct entity_s;	// Forward declaration

typedef enum type_e {
	E_PLAYER,
	E_PROJECTILE,
	E_EXPLOSION,
	E_BASIC_ENEMY,
	E_CURVE_ENEMY,
	E_FAST_ENEMY,
	E_BOSS_ENEMY
} type_t;

typedef enum state_e {
	S_IDLE,
	S_MOVE,
	S_DIE
} state_t;

typedef void (*impulse_f)(struct entity_s *Entity);

typedef struct entity_s {
	struct		entity_s *previous;
	struct		entity_s *next;
	bool 		active;
	type_t		type;
	bool		is_enemy;
	struct 		entity_s *owner;
	state_t		state;
	impulse_f	impulse;
	SDL_Surface	*image;
	SDL_Surface *ghost;
	int			frame;
	int			last_frame;
	int 		frame_count;
	int			frame_size;
	vec2		position;
	float		angle;
	int 		points;
	int			lives;
	float		damage;
	float		health;
	int			last_shot;
	int			last_respawn;
	struct 		entity_s *shot_by;
} Entity;


// Globals
extern Entity g_entities[];
extern Entity *g_player1;
extern Entity *g_player2;
extern Entity *g_boss;


// Functions
Entity *create_entity(void);
Entity *create_projectile(Entity *);
void projectile_impulse(Entity *);
Entity *create_explosion(Entity *);
void explosion_impulse(Entity *);
Entity *create_player(int);
Entity *create_test_enemy(void);
void test_enemy_impulse(Entity *);
Entity *create_curve_enemy(void);
void curve_enemy_impulse(Entity *);
Entity *create_fast_enemy(void);
void fast_enemy_impulse(Entity *);
Entity *create_boss_enemy(void);
void boss_enemy_impulse(Entity *);
Entity *get_entity_collision(Entity *);
state_t damage_entity(Entity *, float);
void destroy_entity(Entity *);
void spawn_enemies(int);

#endif /* ENTITY_H_ */
