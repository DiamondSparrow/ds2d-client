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
#include <errno.h>
#include <linux/joystick.h>
#include <sys/ioctl.h>

#include "joystick.h"
#include "display.h"
#include "types.h"

static void JOYSTICK_DebugEvent(joystick_t *js);

int JOYSTICK_Init(joystick_t *js, unsigned char debug, const char *dev)
{
    //char name[128];
    //struct js_corr corr[10];
    js->debug = debug;

    if ( (js->stream = open(dev, O_RDONLY | O_NONBLOCK)) < 0)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed to open joystick %s.", dev);
        return -1;
    }
    else
    {
        /*
        if (ioctl(js->stream, JSIOCGNAME(sizeof(name)), name) < 0)
        {
            strncpy(name, "Unknown", sizeof(name));
        }
        DISPLAY_Debug(TRUE, displayDebugError, "js name = %s.", name);


        if (ioctl(js->stream, JSIOCSCORR, corr) < 0)
        {
            DISPLAY_Debug(TRUE, displayDebugError, "JSIOCSCORR.");
        }
        else
        {
            DISPLAY_Debug(TRUE, displayDebugError, "js A0 %f %d, %d.",
                    corr[0].coef,
                    corr[0].prec,
                    corr[0].type);
        }
        */
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
    if (bytes == -1)
    {
        if(errno == ENODEV)
        {
            return -1;
        }
        return 0;
    }

    if (bytes != sizeof(js->event))
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Unexpected bytes from joystick: %d\n", bytes);
        return -2;
    }

    JOYSTICK_DebugEvent(js);

    return 1;
}

static void JOYSTICK_DebugEvent(joystick_t *js)
{
    DISPLAY_Debug(js->debug, displayDebugJoystick, "time %8u, value %8hd, type: %3u, axis/button: %u",
                js->event.time,
                js->event.value,
                js->event.type,
                js->event.number);

    return;
}
