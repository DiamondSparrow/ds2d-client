/*
 * daemonize.h
 *
 *  Created on: Nov 27, 2013
 *      Author: DiamondS
 */

#ifndef DAEMONIZE_H_
#define DAEMONIZE_H_

#define DAEMONIZE_NAME 		"ds2d"
#define DAEMONIZE_LOCKFILE	"/run/lock/"
#define DAEMONIZE_USER		"root"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS	0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE	1
#endif

void DAEMONIZE_Init ( void );
void DAEMONIZE_Close ( void );

#endif /* DAEMONIZE_H_ */
