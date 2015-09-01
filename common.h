/*
 * common.h
 *
 *  Created on: 2 Dec 2013
 *      Author: mike
 */

#ifndef COMMON_H_
#define COMMON_H_

typedef struct vec2_s {
	float x;
	float y;
} vec2;


// Functions
float clamp(float, float, float);
float get_distance(float, float, float, float);


#endif /* COMMON_H_ */
