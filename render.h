/*
 * render.h
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */

#ifndef RENDER_H_
#define RENDER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


#define RENDER_FPS			60
#define FRAME_TIME			(1000 / RENDER_FPS)
#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
#define COLOUR_DEPTH		32


// Globals
extern int frame_start_time;

// Functions
void init_screen(void);
void update_screen(void);
void draw_backdrop(void);
void draw_stars(void);
void draw_entities(void);
void draw_overlay(void);
void print_string(int, int, char *, TTF_Font *, SDL_Colour *);


#endif /* RENDER_H_ */
