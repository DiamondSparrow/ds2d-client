/*
 * display.h
 *
 *  Created on: Feb 22, 2015
 *      Author: DiamondS
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DISPLAY_RESET   "\033[0m"
#define DISPLAY_BLACK   "\033[30m"     // Black
#define DISPLAY_RED     "\033[31m"     // Red
#define DISPLAY_GREEN   "\033[32m"     // Green
#define DISPLAY_YELLOW  "\033[33m"     // Yellow
#define DISPLAY_BLUE    "\033[34m"     // Blue
#define DISPLAY_MAGENTA "\033[35m"     // Magenta
#define DISPLAY_CYAN    "\033[36m"     // Cyan
#define DISPLAY_WHITE   "\033[37m"     // White
#define DISPLAY_BOLD    "\033[1m"      // Bold
#define DISPLAY_ITALIC  "\033[3m"      // Italic

#define DISPLAY_DEBUG_Y_START   25
#define DISPLAY_DEBUG_LIMIT     25

typedef enum _display_debug
{
    displayDebugMain,
    displayDebugError,
    displayDebugConfig,
    displayDebugInputs,
    displayDebugJoystick,
    displayDebugRemote,
    displayDebugTcpClient,
    displayDebugNone
} display_debug_e;

void DISPLAY_Init(void);
void DISPLAY_Show(void);
void DISPLAY_Debug(int debugFlag, display_debug_e debugType, void *str, ...);
void DISPLAY_Close(void);

#endif /* DISPLAY_H_ */
