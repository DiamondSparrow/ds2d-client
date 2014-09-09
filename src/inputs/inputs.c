/*
 * inputs.c
 *
 *  Created on: Sep 9, 2014
 *      Author: DiamondS
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "inputs.h"
#include "joystick.h"

#include "options.h"
#include "sleep.h"
#include "types.h"
#include "debug.h"
#include "config.h"


joystick_t js;
pthread_t INPUTS_Thread = {0};
unsigned int INPUTS_Run = FALSE;

void *INPUTS_Handler();
static void INPUTS_GetVector(short x, short y, double *velocity, double *angle);

int INPUTS_Init(const char *device)
{
    int ret = 0;

    if(JOYSTICK_Init(&js, options.debugJoystick, configuration.inputs.joystick) != 0)
    {
        return -1;
    }

    INPUTS_Run = TRUE;
    if( (ret = pthread_create(&INPUTS_Thread, NULL, &INPUTS_Handler, NULL)) )
    {
        fprintf(stderr,"ERROR: Failed to create inputs thread (%d). %s.\n", ret, strerror(errno));
        return -1;
    }

    return 0;
}

int INPUTS_Close(void)
{
    INPUTS_Run = FALSE;

    pthread_join(INPUTS_Thread, NULL);

    JOYSTICK_Close(&js);

    return 0;
}

void *INPUTS_Handler()
{
    int ret = 0;
    double vector = 0;
    double angle = 0;
    double jseX = 0;
    double jseY = 0;

    pthread_setname_np(INPUTS_Thread, "ds2c-inputs");

    DEBUG_Print(options.debugInputs, debugInputs, "* started.");

    while (INPUTS_Run)
    {
        ret = JOYSTICK_ReadEvent(&js);
        switch (ret)
        {
        case 0:
            SLEEP_Delay(0.1);
            break;
        case -1:
            SLEEP_Delay(1.0);
            break;
        case 1:
            if (js.event.type == JS_EVENT_AXIS
                    && (js.event.number == 1 || js.event.number == 0))
            {
                if (js.event.number == 1)
                {
                    jseY = (float) js.event.value / -10.2396875;
                }
                if (js.event.number == 0)
                {
                    jseX = (float) js.event.value / 10.2396875;
                }
                INPUTS_GetVector(jseX, jseY, &vector, &angle);
                DEBUG_Print(options.debugInputs, debugInputs,
                        "Vector: %+05.0f; Angle: %+05.0f;", vector, angle);
            }
            break;
        }
    }

    DEBUG_Print(options.debugInputs, debugInputs, "x stopped.");

    return (NULL);
}

static void INPUTS_GetVector(short x, short y, double *velocity, double *angle)
{
    *velocity = sqrt(pow(x, 2) + pow(y, 2));
    if (*velocity > 3200)
    {
        *velocity = 3200;
    }

    *angle = atan2(y, x);
    if (angle < 0)
    {
        *angle += 2 * M_PI;
    }
    *angle = *angle * 180 / M_PI;

    return;
}

