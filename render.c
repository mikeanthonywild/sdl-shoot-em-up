/*
 * render.c
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */


#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "render.h"
#include "entity.h"
#include "resources.h"
#include "game.h"


// Vars
SDL_Surface *screen = NULL;
int frame_start_time = 0;
int frame = 0;


// Set up SDL
// TODO: Investigate hw accel - SDL_lockSurface()
// TODO: Doublebuffering
void init_screen(void){
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOUR_DEPTH, SDL_HWSURFACE);

	// Fall back to software rendering if hardware rendering fails
	if (!screen) {
		printf("Error initialising hardware rendering, resorting to SW renderer: %s", SDL_GetError());
		screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOUR_DEPTH, SDL_SWSURFACE);
	}
	if (!screen) {
		printf("Error initialising software rendering: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	//Initialise SDL_mixer
	if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1 ) {
		printf("Error initialising audio mixer: %s", Mix_GetError());
		exit(EXIT_FAILURE);
	}

	// Set a title
	SDL_WM_SetCaption( "GIVE ME A PROPER NAME :(", NULL );

	// Initialise font engine
	if(TTF_Init() == -1) {
		printf("Error initialising font engine: %s", TTF_GetError());
		exit(EXIT_FAILURE);
	}
}


// Render the game to the screen
void update_screen(void) {
	// Locals
	int elapsed;

	frame++;
	draw_backdrop();
	draw_entities();
	draw_overlay();

	// Lock the refresh rate to 60 FPS
	SDL_Flip(screen);
	elapsed = frame_start_time - SDL_GetTicks();
	if (elapsed < FRAME_TIME) {
		SDL_Delay(FRAME_TIME - elapsed);
	}
}


// Draw background image with parallax effect
void draw_backdrop(void) {
	SDL_Rect position;
	if (g_backdropy < 0) {
		g_backdropy += BACKDROP_SPEED;
	}
	position.x = g_backdropx;
	position.y = g_backdropy;
	SDL_BlitSurface(g_backdrop_img, NULL, screen, &position);

	draw_stars();
}


// Draw the star layer on top
void draw_stars(void) {
	// Vars
	SDL_Rect position;
	int x;
	int y;

	// Scroll the stars down a bit
	g_stary += STAR_SPEED;

	if (g_stary > 0.0) {
		g_stary = -g_trails_img->h;
	}

	for (x=g_starx; x<WINDOW_WIDTH; x+=g_trails_img->w) {
		position.x = x;
		position.w = g_trails_img->w;
		for (y=g_stary; y<WINDOW_WIDTH; y+=g_trails_img->h) {
			position.y = y;
			position.h = g_trails_img->h;
			SDL_BlitSurface(g_trails_img, NULL, screen, &position);
		}
	}

}


// Draw all game entities to the screen
void draw_entities(void) {
	int i;
	Entity *e;
	SDL_Rect draw_rect;
	SDL_Rect frame_rect;
	const float pi = 3.14159;

	// Loop through all entities
	for (i=0; i<MAX_ENTITIES; i++) {
		e = &g_entities[i];
		if (e->active) {
			// Set the position on screen
			draw_rect.x = e->position.x;
			draw_rect.y = e->position.y;

			// Advance the animation frame (non-player only)
			if (e->type != E_PLAYER && e->type != E_CURVE_ENEMY) {
				if (SDL_GetTicks() > e->last_frame + 50) {
					e->frame++;
					e->last_frame = SDL_GetTicks();
				}
				if (e->frame == e->frame_count) {
					e->frame = 0;
				}
			}

			else if (e->type == E_CURVE_ENEMY) {
				//e->angle > 0 && e->angle < pi / 8
				if (e->angle == 0) { // point left
						e->frame = 4;
					}
				else if( e->angle > 0 && e->angle < pi/2) {  // bottom left corner
						e->frame = 3;
					}
				else if( e->angle == pi/2) {  // bottom
						e->frame = 2;
					}
				else if( e->angle > pi/2 && e->angle < pi) {   // bottom right corner
					e->frame = 1;
				}
				else {   // right
					e->frame = 0;
				}
			}

			// Set the correct animation frame to be drawn
			frame_rect.x = e->frame * e->frame_size;
			frame_rect.y = 0;
			frame_rect.w = frame_rect.h = e->frame_size;

			// Correct frame for explosions is different
			if (e->type == E_EXPLOSION) {
				frame_rect.y = e->frame / EXPLOSION_COLUMNS * e->frame_size;
				frame_rect.y = (e->frame % EXPLOSION_COLUMNS) * e->frame_size;
			}

			// Transparency
			if (e->last_respawn) {
				SDL_BlitSurface(e->ghost, &frame_rect, screen, &draw_rect);
				continue;
			}

			// Draw it
			SDL_BlitSurface(e->image, &frame_rect, screen, &draw_rect);
		}
	}
}


// Draw the UI and debug
void draw_overlay(void) {
	// Vars
	SDL_Color gold = {240, 230, 140};
	SDL_Color white = {255, 255, 255};
	SDL_Color red = {255, 0, 0};
	SDL_Rect position;
	char points1[10];
	char points2[10];
	int i;
	int width;
	int height;
	char *text = (char *) malloc(sizeof(char) * 10);	// Dyanmic memory
	char *msg;

	// Game over message - darken the screen
	if (g_gameover) {
		SDL_BlitSurface(g_dark_img, NULL, screen, NULL);
		TTF_SizeText(g_gameover_font, "GAME OVER", &width, &height);
		if (g_boss && g_boss->active == false) {
			msg = "  YOU WIN ";
		} else {
			msg = "GAME OVER";
		}
		print_string(WINDOW_WIDTH / 2 - width / 2, WINDOW_HEIGHT / 2 - height / 2, msg, g_gameover_font, &red);
	}

	// Player 1 score
	print_string(10, 10, "1P: ", g_overlay_font, &gold);
	if (g_player1->active || g_gameover) {
		sprintf(points1, "%d", g_player1->points);
		print_string(80, 10, points1, g_overlay_font, &white);

		// Health
		position.y = WINDOW_HEIGHT - g_heart_img->h - 10;
		for (i=0; i<g_player1->lives; i++) {
			position.x = 10 + i * (g_heart_img->w + 2);
			SDL_BlitSurface(g_heart_img, NULL, screen, &position);
		}
	} else {
		print_string(80, 10, "PRESS F1", g_overlay_font, &white);
	}


	// Player 2 score
	print_string(WINDOW_WIDTH - 220, 10, "2P: ", g_overlay_font, &gold);
	if (g_player2->active || g_gameover) {
		sprintf(points2, "%d", g_player2->points);
		print_string(WINDOW_WIDTH - 220 + 80, 10, points2, g_overlay_font, &white);

		// Health
		position.y = WINDOW_HEIGHT - g_heart_img->h - 10;
		for (i=0; i<g_player2->lives; i++) {
			position.x = WINDOW_WIDTH - 12 - (g_heart_img->w * PLAYER_LIVES) + i * (g_heart_img->w + 2);
			SDL_BlitSurface(g_heart_img, NULL, screen, &position);
		}
	} else {
		print_string(WINDOW_WIDTH - 220 + 80, 10, "PRESS F2", g_overlay_font, &white);
	}

	free(text);
}


// Prints text to screen using SDL
void print_string(int x, int y, char *str, TTF_Font *font, SDL_Color *colour) {
	SDL_Surface *msg;
	SDL_Rect position;
	position.x = x;
	position.y = y;
	msg = TTF_RenderText_Solid(font, str, *colour);
	SDL_BlitSurface(msg, NULL, screen, &position);
	SDL_FreeSurface(msg);
}
