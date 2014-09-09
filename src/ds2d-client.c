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
#include "config.h"

#include "inputs.h"

volatile unsigned int run = 1;

void terminate(int signalNumber);

int main(int argc, char *argv[])
{
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

    if(CONFIG_Init("../config.ini", options.debugConfig) != 0)
    {
        exit(EXIT_FAILURE);
    }

    if(INPUTS_Init(configuration.inputs.joystick) != 0)
    {
        exit(EXIT_FAILURE);
    }

    SLEEP_Delay(0.1);

    DEBUG_Print(options.debug, debugMain, "# Running.");

    while (run)
    {
        SLEEP_Delay(0.1);
    }

    INPUTS_Close();

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
