/*
 * sound.c
 *
 *  Created on: 12 Dec 2013
 *      Author: mike
 */


#include <SDL/SDL_mixer.h>
#include <stdbool.h>


// Plays a single sound, making sure we only play it once
void play_sound(Mix_Chunk *sound, bool *played, bool repeat) {
	if ((played != NULL && *played != true) || repeat) {
		Mix_PlayChannel(-1, sound, 0);
		if (played) {
			*played = true;
		}
	}
}
