/*
 * input.h
 *
 *  Created on: 28 Nov 2013
 *      Author: mike
 */

#ifndef INPUT_H_
#define INPUT_H_

// Includes
#include <stdbool.h>

#include "entity.h"


// Functions
bool handle_input(void);
void move_player(Entity *, float, float);

#endif /* INPUT_H_ */
