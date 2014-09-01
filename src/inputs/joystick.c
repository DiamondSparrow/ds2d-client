/*
 * joystick.c
 *
 *  Created on: Sep 1, 2014
 *      Author: DiamondS
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/joystick.h>

#include "joystick.h"
#include "debug.h"

static void JOYSTICK_DebugEvent(joystick_t *js);

int JOYSTICK_Init(joystick_t *js, unsigned char debug, char *dev)
{
    js->debug = debug;

    if ( (js->stream = open(dev, O_RDONLY | O_NONBLOCK)) < 0)
    {
        fprintf(stderr, "ERROR: Failed to open joystick %s.\n", dev);
        return -1;
    }

    return 0;
}

void JOYSTICK_Close(joystick_t *js)
{
    close(js->stream);

    return;
}


int JOYSTICK_ReadEvent(joystick_t *js)
{
    int bytes = 0;

    bytes = read(js->stream, &js->event, sizeof(js->event));
    if ( bytes == -1)
    {
        return 0;
    }

    if (bytes != sizeof(js->event))
    {
        fprintf(stderr, "ERROR: Unexpected bytes from joystick:%d\n", bytes);
        return -1;
    }

    JOYSTICK_DebugEvent(js);

    return 1;
}

static void JOYSTICK_DebugEvent(joystick_t *js)
{
    DEBUG_Print(js->debug, debugJoystick, "time %8u, value %8hd, type: %3u, axis/button: %u",
                js->event.time,
                js->event.value,
                js->event.type,
                js->event.number);

    return;
}
