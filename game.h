/*
 * game.h
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */

#ifndef GAME_H_
#define GAME_H_

// Includes
#include <stdbool.h>

#include "entity.h"


// Constants
#define LEVEL2_START_TIME	20000	// Time next level begins at
#define LEVEL3_START_TIME	50000
#define BOSS_START_TIME		120000

#define LEVEL1_SPAWN_RATE	2000	// Time between enemy spawns
#define LEVEL2_SPAWN_RATE	700
#define LEVEL3_SPAWN_RATE	250

/*
#define LEVEL2_START_TIME	1000	// Time next level begins at
#define LEVEL3_START_TIME	2000
#define BOSS_START_TIME		3000

#define LEVEL1_SPAWN_RATE	2000	// Time between enemy spawns
#define LEVEL2_SPAWN_RATE	700
#define LEVEL3_SPAWN_RATE	250
*/

#define FIRSTBLOOD_POINTS	10
#define MEGAKILL_POINTS		200
#define ULTRAKILL_POINTS	500
#define RAMPAGE_POINTS		1000
#define GODLIKE_POINTS		2000

#define TIMER_TOLERANCE		10		// Allow timer to be 10ms out or so

#define WEAPON_COOLDOWN_TIME	150	// Time between shots
#define INVULN_TIME			3000	// Time player is untouchable for after dying
#define STAR_SPEED			6.0
#define BACKDROP_SPEED		0.2;

// Globals
extern bool g_gameover;
extern float g_starx;
extern float g_stary;
extern float g_backdropx;
extern float g_backdropy;


// Functions
void init_game(void);
void update_game(void);
state_t next_state(Entity *);
state_t do_idle(Entity *);
state_t do_move(Entity*);
state_t do_die(Entity *);


#endif /* GAME_H_ */
