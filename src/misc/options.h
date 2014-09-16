/*
 * options.h
 *
 *  Created on: Nov 27, 2013
 *      Author: DiamondS
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stdbool.h>

#define OPTIONS_SOFTWARE_MAJOR          (0)
#define OPTIONS_SOFTWARE_MINOR          (1)
#define OPTIONS_SOFTWARE_MAINTENANCE    (1)
#define OPTIONS_SOFTWARE_BUILD          (1)

#define OPTIONS_SOFTWARE_EDITION        "Alpha Edition"
#define OPTIONS_SOFTWARE_NAME           "ds2d-client"

#define OPTIONS_DEFAULT_DEBUG           0
#define OPTIONS_DEFAULT_TCP_PORT        30003
#define OPTIONS_DEFAULT_TCP_SERVER      "127.0.0.1"
#define OPTIONS_DEFAULT_JOYSTICK_DEV    "/dev/input/js0"

typedef struct _options
{
    bool debug;
    bool debugConfig;
    bool debugInputs;
    bool debugJoystick;
    bool debugRemote;
    bool debugTcpClient;
} options_t;

options_t options;

void OPTIONS_Init(options_t *options, int argc, char *argv[]);
void OPTIONS_PrintUsage(FILE *stream, int exitCode, char *argv[]);
void OPTIONS_PrintVersion(FILE *stream, int exitCode);

#endif /* OPTIONS_H_ */
