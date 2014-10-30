/*
 * inputs.c
 *
 *  Created on: Sep 9, 2014
 *      Author: DiamondS
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "inputs.h"
#include "joystick.h"
#include "ds2.h"
#include "options.h"
#include "sleep.h"
#include "types.h"
#include "display.h"
#include "config.h"


pthread_t INPUTS_Thread = {0};
bool INPUTS_Run = false;

void *INPUTS_Handler();
static void INPUTS_GetVector(short x, short y, double *velocity, double *angle);

int INPUTS_Init(const char *device)
{
    int ret = 0;

    INPUTS_Run = true;
    if( (ret = pthread_create(&INPUTS_Thread, NULL, &INPUTS_Handler, NULL)) )
    {
        DISPLAY_Debug(TRUE, displayDebugError,"Failed to create inputs thread (%d). %s.\n", ret, strerror(errno));
        return -1;
    }

    return 0;
}

int INPUTS_Close(void)
{
    INPUTS_Run = false;

    pthread_join(INPUTS_Thread, NULL);

    return 0;
}

void *INPUTS_Handler()
{
    bool connected = false;
    joystick_t js;
    int i = 0;
    int ret = 0;
    double vector = 0;
    double angle = 0;
    double jseX = 0;
    double jseY = 0;

    pthread_setname_np(INPUTS_Thread, "ds2c-inputs");

    DISPLAY_Debug(options.debugInputs, displayDebugInputs, "* started.");
    SLEEP_Delay(1.0);

    while (INPUTS_Run)
    {
        if(!connected)
        {
            if(JOYSTICK_Init(&js, options.debugJoystick, configuration.inputs.joystick) != 0)
            {
                connected = false;
                i = 10;
                while(INPUTS_Run)
                {
                    if(i-- < 1)
                    {
                        break;
                    }
                    SLEEP_Delay(1.0);
                }
            }
            else
            {
                connected = true;
                DISPLAY_Debug(options.debugInputs, displayDebugInputs, "joystick connected.");
                ds2_data.joystickState = true;
            }
        }
        else
        {
            ret = JOYSTICK_ReadEvent(&js);
            switch (ret)
            {
            case 0:
                SLEEP_Delay(0.01);
                break;
            case -1:
                JOYSTICK_Close(&js);
                connected = false;
                DISPLAY_Debug(options.debugInputs, displayDebugInputs, "joystick disconnected.");
                break;
            case 1:
                if (js.event.type == JS_EVENT_AXIS)
                {
                    switch(js.event.number)
                    {
                    case 0:
                    case 1:
                        if(js.event.value < configuration.inputs.offset && js.event.value > -(configuration.inputs.offset))
                        {
                            js.event.value = 0;
                        }
                        if(js.event.value > configuration.inputs.offset)
                        {
                            js.event.value -= configuration.inputs.offset;
                        }
                        else if(js.event.value < -(configuration.inputs.offset))
                        {
                            js.event.value += configuration.inputs.offset;
                        }

                        if (js.event.number == 1)
                        {
                            jseY = (float) (js.event.value - 0) / ((JOYSTICK_AXIS_MIN + configuration.inputs.offset) / 100);
                        }
                        if (js.event.number == 0)
                        {
                            jseX = (float) (js.event.value - 0) / ((JOYSTICK_AXIS_MAX - configuration.inputs.offset) / 100);
                        }
                        INPUTS_GetVector(jseX, jseY, &vector, &angle);
                        ds2_data.speed = (int)vector;
                        ds2_data.angle = (int)angle;
                        DISPLAY_Debug(options.debugInputs, displayDebugInputs, "Vector: %+05.0f; Angle: %+05.0f;", vector, angle);
                        break;
                    case 2:
                        ds2_data.brake = ((js.event.value + 32767) / 655.34);
                        break;
                    case 6:
                        if(js.event.value < 0)
                        {
                            if((ds2_data.pan + 5) <= 90)
                            {
                                ds2_data.pan += 5;
                            }
                        }
                        else if(js.event.value > 0)
                        {
                            if((ds2_data.pan - 5) >= -90)
                            {
                                ds2_data.pan -= 5;
                            }
                        }
                        break;
                    case 7:
                        if(js.event.value < 0)
                        {
                            if((ds2_data.tilt + 5) <= 90)
                            {
                                ds2_data.tilt += 5;
                            }
                        }
                        else if(js.event.value > 0)
                        {
                            if((ds2_data.tilt - 5) >= -90)
                            {
                                ds2_data.tilt -= 5;
                            }
                        }
                        break;
                    }
                }
                if(js.event.type == JS_EVENT_BUTTON)
                {
                    switch(js.event.number)
                    {
                    case 2:
                        if(js.event.value == 1)
                        {
                            ds2_data.speed = 0;
                            ds2_data.angle = 0;
                        }
                        break;
                    }
                }
                break;
            default:
                SLEEP_Delay(1.0);
                break;
            }
        }
    }

    if(connected)
    {
        JOYSTICK_Close(&js);
    }

    DISPLAY_Debug(options.debugInputs, displayDebugInputs, "x stopped.");

    return (NULL);
}

static void INPUTS_GetVector(short x, short y, double *velocity, double *angle)
{
    *velocity = sqrt(pow(x, 2) + pow(y, 2));
    if (*velocity > 100)
    {
        *velocity = 100;
    }

    *angle = atan2(y, x);
    if (angle < 0)
    {
        *angle += 2 * M_PI;
    }
    *angle = *angle * 180 / M_PI;

    return;
}

