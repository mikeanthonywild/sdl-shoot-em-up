/*
 * sound.h
 *
 *  Created on: 12 Dec 2013
 *      Author: mike
 */

#ifndef SOUND_H_
#define SOUND_H_

// Includes
#include <SDL/SDL_mixer.h>
#include <stdbool.h>


// Functions
void play_sound(Mix_Chunk *, bool *, bool);

#endif /* SOUND_H_ */
