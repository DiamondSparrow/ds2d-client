/*
 * remote.c
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

#include "remote.h"

#include "options.h"
#include "sleep.h"
#include "types.h"
#include "debug.h"
#include "config.h"

pthread_t REMOTE_Thread = {0};
unsigned int REMOTE_Run = FALSE;

void *REMOTE_Handler();

int REMOTE_Init(void)
{
    int ret = 0;

    REMOTE_Run = TRUE;
    if( (ret = pthread_create(&REMOTE_Thread, NULL, &REMOTE_Handler, NULL)) )
    {
        fprintf(stderr,"ERROR: Failed to create inputs thread (%d). %s.\n", ret, strerror(errno));
        return -1;
    }

    return 0;
}

int REMOTE_Close(void)
{
    REMOTE_Run = FALSE;

    pthread_join(REMOTE_Thread, NULL);

    return 0;
}

void *REMOTE_Handler()
{
    pthread_setname_np(REMOTE_Thread, "ds2c-remote");

    DEBUG_Print(options.debugRemote, debugRemote, "* started.");

    while (REMOTE_Run)
    {
        SLEEP_Delay(0.1);
    }

    DEBUG_Print(options.debugRemote, debugRemote, "x stopped.");

    return (NULL);
}
