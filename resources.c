/*
 * resources.c
 *
 *  Created on: 7 Dec 2013
 *      Author: mike
 */


#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>

#include "resources.h"

// FIXME: Is this portable?
#define MEDIA_DIR	"Media/"


// Globals
SDL_Surface *g_player1_img 		= NULL;
SDL_Surface *g_player2_img		= NULL;
SDL_Surface *g_test_enemy_img	= NULL;
SDL_Surface *g_player_ghost_img	= NULL;
SDL_Surface *g_curve_enemy_img	= NULL;
SDL_Surface *g_fast_enemy_img	= NULL;
SDL_Surface *g_boss_enemy_img	= NULL;
SDL_Surface *g_fire_img 		= NULL;
SDL_Surface *g_explosion_img	= NULL;
SDL_Surface *g_backdrop_img 	= NULL;
SDL_Surface *g_trails_img 		= NULL;
SDL_Surface *g_dark_img 		= NULL;
SDL_Surface *g_heart_img 		= NULL;
TTF_Font 	*g_overlay_font		= NULL;
TTF_Font 	*g_gameover_font	= NULL;
Mix_Chunk	*g_firstblood_snd	= NULL;
Mix_Chunk 	*g_megakill_snd		= NULL;
Mix_Chunk 	*g_ultrakill_snd	= NULL;
Mix_Chunk 	*g_rampage_snd		= NULL;
Mix_Chunk 	*g_godlike_snd		= NULL;
Mix_Chunk 	*g_holyshit_snd		= NULL;
Mix_Chunk 	*g_die_snd			= NULL;
Mix_Chunk	*g_explode_snd		= NULL;
Mix_Chunk	*g_laser_snd		= NULL;
Mix_Chunk	*g_hit_snd			= NULL;
Mix_Music	*g_music_snd		= NULL;
bool		g_firstblood_snd_played;
bool		g_megakill_snd_played;
bool		g_ultrakill_snd_played;
bool		g_rampage_snd_played;
bool		g_godlike_snd_played;
bool		g_holyshit_snd_played;
bool		g_die_snd_played;


// Preloads all SDL resources
void load_resources(void) {
	// Ships
	load_image(MEDIA_DIR "player1.png", &g_player1_img);
	load_image(MEDIA_DIR "player2.png", &g_player2_img);
	load_image(MEDIA_DIR "ghost.png", &g_player_ghost_img);
	load_image(MEDIA_DIR "enemy1.png", &g_test_enemy_img);
	load_image(MEDIA_DIR "enemy2.png", &g_curve_enemy_img);
	load_image(MEDIA_DIR "enemy3.png", &g_fast_enemy_img);
	load_image(MEDIA_DIR "boss.png", &g_boss_enemy_img);

	// Projectiles
	load_image(MEDIA_DIR "fire.png", &g_fire_img);
	load_image(MEDIA_DIR "explosion.png", &g_explosion_img);

	// Backgdrops
	load_image(MEDIA_DIR "backdrop.jpg", &g_backdrop_img);
	load_image(MEDIA_DIR "trails.png", &g_trails_img);
	load_image(MEDIA_DIR "dark.png", &g_dark_img);

	// Icons
	load_image(MEDIA_DIR "heart.png", &g_heart_img);

	// Sound effects
	load_sound(MEDIA_DIR "firstblood.wav", &g_firstblood_snd);
	load_sound(MEDIA_DIR "megakill.wav", &g_megakill_snd);
	load_sound(MEDIA_DIR "ultrakill.wav", &g_ultrakill_snd);
	load_sound(MEDIA_DIR "rampage.wav", &g_rampage_snd);
	load_sound(MEDIA_DIR "godlike.wav", &g_godlike_snd);
	load_sound(MEDIA_DIR "holyshit.wav", &g_holyshit_snd);
	load_sound(MEDIA_DIR "die.wav", &g_die_snd);
	load_sound(MEDIA_DIR "explode.wav", &g_explode_snd);
	load_sound(MEDIA_DIR "laser.wav", &g_laser_snd);
	load_sound(MEDIA_DIR "hit.wav", &g_hit_snd);

	// Music
	load_music(MEDIA_DIR "ymVST_demo.ogg", &g_music_snd);

	// Fonts
	load_font(MEDIA_DIR "Munro.ttf", 28, &g_overlay_font);
	load_font(MEDIA_DIR "Sega.ttf", 58, &g_gameover_font);
}


// Loads an image and checks to make sure it was loaded correctly
void load_image(const char *file, SDL_Surface **dst) {
	// Locals
	SDL_Surface *tmp = NULL;

	// Load and optimise the image
	tmp = IMG_Load(file);
	if(!tmp) {
		printf("Problem opening image (%s): %s", file, IMG_GetError());
		exit(EXIT_FAILURE);
	}
	*dst = SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface(tmp);
}


// Loads a font from disk and checks it loaded correctly
void load_font(const char *file, int size, TTF_Font **dst) {
	*dst = TTF_OpenFont(file, size);
	if(!(*dst)) {
		printf("Problem opening font (%s): %s", file, TTF_GetError());
		exit(EXIT_FAILURE);
	}
}


// Loads a sound from disk and checks it loaded correctly
void load_sound(const char *file, Mix_Chunk **dst) {
	*dst = Mix_LoadWAV(file);
	if(!(*dst)) {
		printf("Problem opening sound (%s): %s", file, Mix_GetError());
		exit(EXIT_FAILURE);
	}
}


// Loads a sound from disk and checks it loaded correctly
void load_music(const char *file, Mix_Music **dst) {
	*dst = Mix_LoadMUS(file);
	if(!(*dst)) {
		printf("Problem opening sound (%s): %s", file, Mix_GetError());
		exit(EXIT_FAILURE);
	}
}
