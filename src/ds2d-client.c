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

#include "options.h"
#include "sleep.h"
#include "types.h"
#include "debug.h"
#include "config.h"

#include "inputs.h"
#include "remote.h"
#include "display.h"
#include "ds2.h"

volatile unsigned int run = 1;

void terminate(int signalNumber);

int main(int argc, char *argv[])
{
    OPTIONS_Init(&options, argc, argv);


    signal(SIGINT, terminate);
    signal(SIGTERM, terminate);

    DISPLAY_Init();

    if(CONFIG_Init("../config.ini", options.debugConfig) != 0)
    {
        exit(EXIT_FAILURE);
    }

    if(INPUTS_Init(configuration.inputs.joystick) != 0)
    {
        exit(EXIT_FAILURE);
    }

    if(REMOTE_Init() != 0)
    {
        exit(EXIT_FAILURE);
    }

    SLEEP_Delay(0.1);

    DISPLAY_Debug(options.debug, displayDebugMain, "# Running.");

    while (run)
    {
        DISPLAY_Show();
        SLEEP_Delay(0.1);
    }

    INPUTS_Close();
    REMOTE_Close();
    DISPLAY_Close();

    return EXIT_SUCCESS;
}

void terminate(int signalNumber)
{
    if (options.debug)
    {
        printf("\n");
    }
    DISPLAY_Debug(options.debug, displayDebugMain, "Terminate.");
    run = 0;

    return;
}
