/*
 * common.c
 *
 *  Created on: 2 Dec 2013
 *      Author: mike
 */


#include <stdlib.h>
#include <math.h>


// Clamps a value to ensure x never goes below lower or above higher
float clamp(float x, float lower, float upper) {
	return x < lower ? lower : (x > upper ? upper : x);
}


// Returns distance between two points
float get_distance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
