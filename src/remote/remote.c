/*
 * remote.c
 *
 *  Created on: Sep 9, 2014
 *      Author: DiamondS
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "remote.h"
#include "tcp_client.h"

#include "options.h"
#include "sleep.h"
#include "types.h"
#include "debug.h"
#include "config.h"

pthread_t REMOTE_Thread = {0};
bool REMOTE_Run = false;
tcp_client_t tcpClient = {0};

void *REMOTE_Handler();

int REMOTE_Init(void)
{
    int ret = 0;

    if(TCP_CLIENT_Init(&tcpClient, (char *)configuration.remote.ip, configuration.remote.port, options.debugTcpClient) != 0)
    {
        return -1;
    }

    REMOTE_Run = true;
    if( (ret = pthread_create(&REMOTE_Thread, NULL, &REMOTE_Handler, NULL)) )
    {
        fprintf(stderr,"ERROR: Failed to create inputs thread (%d). %s.\n", ret, strerror(errno));
        return -1;
    }

    return 0;
}

int REMOTE_Close(void)
{
    REMOTE_Run = false;

    pthread_join(REMOTE_Thread, NULL);

    return 0;
}

void *REMOTE_Handler()
{
    bool connected = false;
    int i = 0;

    pthread_setname_np(REMOTE_Thread, "ds2c-remote");

    DEBUG_Print(options.debugRemote, debugRemote, "* started.");

    SLEEP_Delay(1.0);

    while (REMOTE_Run)
    {
        if(!connected)
        {
            if(TCP_CLIENT_Connect(&tcpClient) == 0)
            {
                connected = true;
                DEBUG_Print(options.debugRemote, debugRemote, "connected.");
            }
            else
            {
                i = 10;
                while (REMOTE_Run)
                {
                    if(i-- < 1)
                    {
                        break;
                    }
                    SLEEP_Delay(1.0);
                }
            }
        }
        else
        {

        }
        SLEEP_Delay(0.1);
    }

    DEBUG_Print(options.debugRemote, debugRemote, "x stopped.");

    return (NULL);
}
