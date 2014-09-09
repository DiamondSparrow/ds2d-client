/*
 * options.c
 *
 *  Created on: Nov 27, 2013
 *      Author: DiamondS
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "options.h"
#include "types.h"

void OPTIONS_Init(options_t *options, int argc, char *argv[])
{
    int nextOption;
    const char *shortOptions = "hVdcijrt";
    const struct option longOptions[] =
    {
        { "debug", no_argument, NULL, 'd' },
        { "config", no_argument, NULL, 'c' },
        { "inputs", no_argument, NULL, 'i' },
        { "joystick", no_argument, NULL, 'j' },
        { "remote", no_argument, NULL, 'r' },
        { "tcp", no_argument, NULL, 't' },
        { "Version", no_argument, NULL, 'V' },
        { "help", no_argument, NULL, 'h' },
    { NULL, no_argument, NULL, 0 }
    };

    options->debug = FALSE;
    options->debugConfig = FALSE;
    options->debugInputs = FALSE;
    options->debugJoystick = FALSE;
    options->debugRemote = FALSE;
    options->debugTcpClient = FALSE;

    do
    {
        nextOption = getopt_long(argc, argv, shortOptions, longOptions, NULL);
        switch (nextOption)
        {
        case 'd':
            options->debug = TRUE;
            break;
        case 'c':
            options->debugConfig = TRUE;
            options->debug = TRUE;
            break;
        case 'i':
            options->debugInputs = TRUE;
            options->debug = TRUE;
            break;
        case 'j':
            options->debugJoystick = TRUE;
            options->debug = TRUE;
            break;
        case 'r':
            options->debugRemote = TRUE;
            options->debug = TRUE;
            break;
        case 't':
            options->debugTcpClient = TRUE;
            options->debug = TRUE;
            break;

        case 'V':
            OPTIONS_PrintVersion(stdout, 0);
            break;

        case 'h':
            OPTIONS_PrintUsage(stdout, 0, argv);
            break;

        case '?':
            OPTIONS_PrintUsage(stderr, 1, argv);
            break;

        case -1:
            break;

        default:
            abort();
            break;
        }
    } while (nextOption != -1);

    return;
}

void OPTIONS_PrintUsage(FILE *stream, int exitCode, char *argv[])
{
    fprintf(stream, "\nUsage:\n %s options\n", argv[0]);

    fprintf(stream, "\n"
            "Debug:\n"
            "  -d, --debug      Debug mode;\n"
            "  -c, --config     Debug config;\n"
            "  -i, --inputs     Debug inputs;\n"
            "  -i, --joystick   Debug joystick;\n"
            "  -r, --remote     Debug remote;\n"
            "  -t, --tcp        Debug TCP client.\n");
    fprintf(stream, "\n"
            "  -V, --version    Version;\n"
            "  -h, --help       Display this usage information.\n"
            "\n");

    exit(exitCode);
}

void OPTIONS_PrintVersion(FILE *stream, int exitCode)
{
    fprintf(stream, "Version: %s v%d.%d.%d.%d %s\n",
    OPTIONS_SOFTWARE_NAME,
    OPTIONS_SOFTWARE_MAJOR,
    OPTIONS_SOFTWARE_MINOR,
    OPTIONS_SOFTWARE_MAINTENANCE,
    OPTIONS_SOFTWARE_BUILD,
    OPTIONS_SOFTWARE_EDITION);

    exit(exitCode);
}

