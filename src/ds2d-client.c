/*
 ============================================================================
 Name        : ds2d-client.c
 Author      : DiamondS
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

#include "daemonize.h"
#include "options.h"
#include "sleep.h"
#include "types.h"
#include "debug.h"

#include "joystick.h"

volatile unsigned int run = 1;

void terminate(int signalNumber);
static void getVector ( short x, short y, double *velocity, double *angle);

int main(int argc, char *argv[])
{
    int ret = 0;
    joystick_t js;
    double vector = 0;
    double angle = 0;
    double jseX = 0;
    double jseY = 0;

    OPTIONS_Init(&options, argc, argv);

    if (!options.debug)
    {
        DAEMONIZE_Init();
    }
    else
    {
        printf("\033[2J\033[1;1H\033[37m");
        printf("\033[1;15H .-'/ ,_  \'-.   ");
        printf("\033[2;15H/  (  ( >  )  \\ ");
        printf("\033[3;15H\\   '-' '-'   / ");
        printf("\033[4;15H'-..__ __..-'    ");
        printf("\033[5;15H      /_\\       ");
        printf("\033[0m");
        printf("\n\t%s %d.%d.%d.%d (%s)\n\n",
        OPTIONS_SOFTWARE_NAME,
        OPTIONS_SOFTWARE_MAJOR,
        OPTIONS_SOFTWARE_MINOR,
        OPTIONS_SOFTWARE_MAINTENANCE,
        OPTIONS_SOFTWARE_BUILD,
        OPTIONS_SOFTWARE_EDITION);
    }

    signal(SIGINT, terminate);
    signal(SIGTERM, terminate);

    JOYSTICK_Init(&js, options.debugJoystick, options.joystickDevice);

    SLEEP_Delay(0.1);

    DEBUG_Print(options.debug, debugMain, "# Running.");

    while (run)
    {
        ret = JOYSTICK_ReadEvent(&js);
        switch(ret)
        {
        case 0:
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
                getVector(jseX, jseY, &vector, &angle);
                DEBUG_Print(options.debug, debugMain, "Vector: %+05.0f; Angle: %+05.0f;", vector, angle);
            }
            break;
        }
    }

    JOYSTICK_Close(&js);

    if (!options.debug)
    {
        DAEMONIZE_Close();
    }

    return EXIT_SUCCESS;
}

void terminate(int signalNumber)
{
    if (options.debug)
    {
        printf("\n");
    }
    DEBUG_Print(options.debug, debugMain, "Terminate.");
    run = 0;

    return;
}

static void getVector(short x, short y, double *velocity, double *angle)
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
