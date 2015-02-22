/*
 * display.c
 *
 *  Created on: Feb 22, 2015
 *      Author: DiamondS
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


#include "display.h"
#include "ds2.h"
#include "options.h"
#include "config.h"
#include "types.h"

pthread_mutex_t DISPLAY_Mutex = PTHREAD_MUTEX_INITIALIZER;
const char DISPLAY_DebugTypeNames[displayDebugNone + 1][11] =
{
        "APP",
        "ERR",
        "CFG",
        "INP",
        "JST",
        "RMT",
        "TCP",
        "???",
};
int DISPLAY_Debug_Index = -1;

void DISPLAY_Init(void)
{
    printf("\033[2J\033[1;1H\033[37m");
    printf("\033[1;19H .-'/ ,_  \'-.   ");
    printf("\033[2;19H/  (  ( >  )  \\ ");
    printf("\033[3;19H\\   '-' '-'   / ");
    printf("\033[4;19H'-..__ __..-'    ");
    printf("\033[5;19H      /_\\       ");
    printf("" DISPLAY_RESET);
    printf("\n\t%s %d.%d.%d.%d (%s)\n\n",
            OPTIONS_SOFTWARE_NAME,
            OPTIONS_SOFTWARE_MAJOR,
            OPTIONS_SOFTWARE_MINOR,
            OPTIONS_SOFTWARE_MAINTENANCE,
            OPTIONS_SOFTWARE_BUILD,
            OPTIONS_SOFTWARE_EDITION);

    // System
    printf("\033[08;01H+----------------------------------------------------------+" DISPLAY_RESET);
    printf("\033[09;01H| " DISPLAY_ITALIC "System " DISPLAY_RESET "                                                  |");
    printf("\033[10;01H+----------------------------------------------------------+" DISPLAY_RESET);
    printf("\033[11;01H| Joystick: %12s" DISPLAY_RESET,
            ds2_data.joystickState ? DISPLAY_GREEN "connected"               : DISPLAY_RED "disconnected");
    printf("\033[11;38H Remote: %12s |"  DISPLAY_RESET,
            ds2_data.remoteState   ? DISPLAY_GREEN "connected" DISPLAY_RESET : DISPLAY_RED "disconnected" DISPLAY_RESET);
    printf("\033[12;01H+----------------------------------------------------------+" DISPLAY_RESET);

    // Left Motor
    printf("\033[14;1H+------------------+"  DISPLAY_RESET);
    printf("\033[15;1H| " DISPLAY_ITALIC "Left Motor" DISPLAY_RESET "       |"  DISPLAY_RESET);
    printf("\033[16;1H+------------------+"  DISPLAY_RESET);
    printf("\033[17;1H| Speed:   %+4d    |"  DISPLAY_RESET, ds2_data.left.speed);
    printf("\033[18;1H| Brake:   %4d    |" DISPLAY_RESET, ds2_data.left.brake);
    printf("\033[19;1H| Current: %4d A. |"   DISPLAY_RESET, ds2_data.left.current);
    printf("\033[20;1H+------------------+"  DISPLAY_RESET);

    // Wheel
    printf("\033[14;23H+--------------+" DISPLAY_RESET);
    printf("\033[15;23H| "DISPLAY_ITALIC "Wheel" DISPLAY_RESET "        |" DISPLAY_RESET);
    printf("\033[16;23H+--------------+" DISPLAY_RESET);
    printf("\033[17;23H| Speed: %+4d  |" DISPLAY_RESET, ds2_data.speed);
    printf("\033[18;23H| Angle: %+4d  |" DISPLAY_RESET, ds2_data.angle);
    printf("\033[19;23H| Brake: %+4d  |" DISPLAY_RESET, ds2_data.brake);
    printf("\033[20;23H+--------------+" DISPLAY_RESET);

    // Right Motor
    printf("\033[14;41H+------------------+"  DISPLAY_RESET);
    printf("\033[15;41H| " DISPLAY_ITALIC "Right Motor" DISPLAY_RESET "      |"  DISPLAY_RESET);
    printf("\033[16;41H+------------------+"  DISPLAY_RESET);
    printf("\033[17;41H| Speed:   %+4d    |"  DISPLAY_RESET, ds2_data.right.speed);
    printf("\033[18;41H| Brake:   %4d    |" DISPLAY_RESET, ds2_data.right.brake);
    printf("\033[19;41H| Current: %4d A. |"   DISPLAY_RESET, ds2_data.right.current);
    printf("\033[20;41H+------------------+"  DISPLAY_RESET);

    printf("\n");
    printf("\033[22;01H+-----------------------------------------------------------" DISPLAY_RESET);
    printf("\033[23;01H| " DISPLAY_ITALIC "Debug " DISPLAY_RESET "                                                  ");
    printf("\033[24;01H+-----------------------------------------------------------" DISPLAY_RESET);
    printf("\n");

    return;
}

void DISPLAY_Show(void)
{
    pthread_mutex_lock(&DISPLAY_Mutex);

    // System
    printf("\033[11;12H %12s " DISPLAY_RESET,
            ds2_data.joystickState ? DISPLAY_GREEN "connected   "               : DISPLAY_RED "disconnected");
    printf("\033[11;46H %12s "  DISPLAY_RESET,
            ds2_data.remoteState   ? DISPLAY_GREEN "connected   " DISPLAY_RESET : DISPLAY_RED "disconnected" DISPLAY_RESET);

    // Left Motor
    printf("\033[17;11H %+4d "  DISPLAY_RESET, ds2_data.left.speed);
    printf("\033[18;11H %4d " DISPLAY_RESET, ds2_data.left.brake);
    printf("\033[19;11H %4d "   DISPLAY_RESET, ds2_data.left.current);

    // Wheel
    printf("\033[17;31H %+4d " DISPLAY_RESET, ds2_data.speed);
    printf("\033[18;31H %+4d " DISPLAY_RESET, ds2_data.angle);
    printf("\033[19;31H %+4d " DISPLAY_RESET, ds2_data.brake);

    // Right Motor
    printf("\033[17;51H %+4d "  DISPLAY_RESET, ds2_data.right.speed);
    printf("\033[18;51H %4d " DISPLAY_RESET, ds2_data.right.brake);
    printf("\033[19;51H %4d "   DISPLAY_RESET, ds2_data.right.current);

    printf("\n");

    pthread_mutex_unlock(&DISPLAY_Mutex);

    return;
}

void DISPLAY_Debug(int debugFlag, display_debug_e debugType, void *str, ...)
{
    int index = DISPLAY_Debug_Index;
    va_list args;
    struct timeval localTime;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    gettimeofday(&localTime, NULL);

    if (debugFlag == TRUE)
    {
        pthread_mutex_lock(&DISPLAY_Mutex);

        if(index == -1)
        {
            index = 0;
        }

        if(DISPLAY_Debug_Index != -1)
        {
            printf("\033[%d;01H|",
                    DISPLAY_Debug_Index == 0 ? (DISPLAY_DEBUG_LIMIT + DISPLAY_DEBUG_Y_START - 1) : (index + DISPLAY_DEBUG_Y_START - 1));
        }
        if(debugType == displayDebugError)
        {
            printf(DISPLAY_RED "");
        }
        printf("\033[%d;01H> %02d:%02d:%02d.%06ld %-3.3s: ",
                index + DISPLAY_DEBUG_Y_START,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                //localTime.tv_sec,
                localTime.tv_usec,
                DISPLAY_DebugTypeNames[debugType]);

        va_start(args, str);
        vprintf(str, args);
        va_end(args);
        printf("\n" DISPLAY_RESET);
        if(DISPLAY_Debug_Index == -1)
        {
            DISPLAY_Debug_Index = 0;
        }
        DISPLAY_Debug_Index++;
        if(DISPLAY_Debug_Index >= DISPLAY_DEBUG_LIMIT)
        {
            DISPLAY_Debug_Index = 0;
        }
        pthread_mutex_unlock(&DISPLAY_Mutex);
    }

    return;
}

void DISPLAY_Close(void)
{

    printf("\033[%d;01H \n", (DISPLAY_DEBUG_LIMIT + DISPLAY_DEBUG_Y_START));
    return;
}
