/*
 * config.h
 *
 *  Created on: Sep 9, 2014
 *      Author: DiamondS
 */

#ifndef CONFIG_H_
#define CONFIG_H_

typedef struct _config_remote
{
    const char* ip;
    unsigned int port;
} config_remote_t;

typedef struct _config_inputs
{
    const char* joystick;
    int offset;
} config_inputs_t;

typedef struct _config
{
    config_remote_t remote;
    config_inputs_t inputs;

} config_t;

extern config_t configuration;

int CONFIG_Init(char *configFile, int debug);
void CONFIG_Debug(void);

#endif /* CONFIG_H_ */
