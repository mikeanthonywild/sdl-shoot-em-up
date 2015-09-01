/*
 * resources.h
 *
 *  Created on: 7 Dec 2013
 *      Author: mike
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_


// Includes
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>


#define DEFAULT_FRAME_SIZE		32
#define DEFAULT_FRAME_COUNT 	1

#define PLAYER_FRAME_COUNT		3
#define EXPLOSION_FRAME_COUNT 	81
#define EXPLOSION_COLUMNS		9
#define EXPLOSION_FRAME_SIZE	100
#define TEST_ENEMY_FRAME_COUNT	3
#define BOSS_ENEMY_FRAME_COUNT	5
#define BOSS_ENEMY_FRAME_SIZE	256


// Globals
extern SDL_Surface *g_player1_img;
extern SDL_Surface *g_player2_img;
extern SDL_Surface *g_player_ghost_img;
extern SDL_Surface *g_test_enemy_img;
extern SDL_Surface *g_curve_enemy_img;
extern SDL_Surface *g_fast_enemy_img;
extern SDL_Surface *g_boss_enemy_img;
extern SDL_Surface *g_fire_img;
extern SDL_Surface *g_explosion_img;
extern SDL_Surface *g_backdrop_img;
extern SDL_Surface *g_trails_img;
extern SDL_Surface *g_dark_img;
extern SDL_Surface *g_heart_img;
extern TTF_Font 	*g_overlay_font;
extern TTF_Font 	*g_gameover_font;
extern Mix_Chunk 	*g_firstblood_snd;
extern Mix_Chunk 	*g_megakill_snd;
extern Mix_Chunk 	*g_ultrakill_snd;
extern Mix_Chunk 	*g_rampage_snd;
extern Mix_Chunk 	*g_godlike_snd;
extern Mix_Chunk 	*g_holyshit_snd;
extern Mix_Chunk 	*g_die_snd;
extern Mix_Chunk	*g_explode_snd;
extern Mix_Chunk	*g_laser_snd;
extern Mix_Chunk	*g_hit_snd;
extern Mix_Music	*g_music_snd;
extern bool		 	g_firstblood_snd_played;
extern bool		 	g_megakill_snd_played;
extern bool		 	g_ultrakill_snd_played;
extern bool		 	g_rampage_snd_played;
extern bool		 	g_godlike_snd_played;
extern bool		 	g_holyshit_snd_played;
extern bool			g_die_snd_played;


// Functions
void load_resources(void);
void load_image(const char *, SDL_Surface **);
void load_font(const char *, int, TTF_Font **);
void load_sound(const char*, Mix_Chunk **);
void load_music(const char*, Mix_Music **);

#endif /* RESOURCES_H_ */
