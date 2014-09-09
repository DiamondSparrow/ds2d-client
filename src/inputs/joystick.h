/*
 * joystick.h
 *
 *  Created on: Sep 1, 2014
 *      Author: DiamondS
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <linux/joystick.h>

typedef struct _joystick
{
    int stream;
    int debug;
    struct js_event event;
} joystick_t;

int JOYSTICK_Init(joystick_t *js, unsigned char debug, const char *dev);
void JOYSTICK_Close(joystick_t *js);

int JOYSTICK_ReadEvent(joystick_t *js);

#endif /* JOYSTICK_H_ */
