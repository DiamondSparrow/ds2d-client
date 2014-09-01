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
	const char *shortOptions = "hVditp:s:j:";
	const struct option longOptions[] =
	{
			{ "port", required_argument, NULL, 'p' },
            { "server", required_argument, NULL, 's' },
            { "joystick", required_argument, NULL, 'j' },
			{ "debug", no_argument, NULL, 'd' },
            { "input", no_argument, NULL, 'i' },
			{ "tcp", no_argument, NULL, 't' },
			{ "Version", no_argument, NULL, 'V' },
			{ "help", no_argument, NULL, 'h' },
			{ NULL, no_argument, NULL, 0 }
	};

	options->debug = FALSE;
    options->debugJoystick = FALSE;
	options->debugTcpClient = FALSE;

	options->tcpPort = OPTIONS_DEFAULT_TCP_PORT;
	memcpy(options->tcpServer, (char *)OPTIONS_DEFAULT_TCP_SERVER, strlen((char *)OPTIONS_DEFAULT_TCP_SERVER));
	memcpy(options->joystickDevice, (char *)OPTIONS_DEFAULT_JOYSTICK_DEV, strlen((char *)OPTIONS_DEFAULT_JOYSTICK_DEV));

	do
	{
		nextOption = getopt_long(argc, argv, shortOptions, longOptions, NULL);
		switch (nextOption)
		{
		case 'p':
			if (optarg > 0)
			{
				options->tcpPort = atoi(optarg);
			}
			break;
        case 's':
            if (optarg > 0)
            {
                memset(options->tcpServer, 0, sizeof options->tcpServer);
                memcpy(options->tcpServer, (char *)optarg, strlen((char *)optarg));
            }
            break;
        case 'j':
            if (optarg > 0)
            {
                memset(options->joystickDevice, 0, sizeof options->joystickDevice);
                memcpy(options->joystickDevice, (char *)optarg, strlen((char *)optarg));
            }
            break;

		case 'd':
			options->debug = TRUE;
			break;
        case 'i':
            options->debugJoystick = TRUE;
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

	fprintf(stream, "Options:\n");
    fprintf(stream, "  -p, --port       <port>  Socket Server Port (default: %d).\n", OPTIONS_DEFAULT_TCP_PORT);
	fprintf(stream, "  -s, --server     <ip>    Socket Server ip (default: %s).\n", (char *)OPTIONS_DEFAULT_TCP_SERVER);
    fprintf(stream, "  -j, --joystick   <dev>   Jostick device (default: %s).\n", (char *)OPTIONS_DEFAULT_JOYSTICK_DEV);
	fprintf(stream, "\n"
			"Debug:\n"
			"  -d, --debug      Debug mode;\n"
			"  -i, --input      Debug Joystick.\n"
			"  -t, --tcp        Debug TCP Server.\n");
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

