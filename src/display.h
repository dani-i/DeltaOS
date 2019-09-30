// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

#ifndef _DISPLAY_H
#define _DISPLAY_H

// ================================================================================================================================
// ================================================================================================================================

#include "stdint.h"
#include "gdefs.h"

// ================================================================================================================================
// ================================================================================================================================

#define DISPLAY_BLACK                           0
#define DISPLAY_BLUE                            1
#define DISPLAY_GREEN                           2
#define DISPLAY_CYAN                            3
#define DISPLAY_RED                             4
#define DISPLAY_PURPLE                          5
#define DISPLAY_BROWN                           6
#define DISPLAY_GRAY                            7
#define DISPLAY_DARK_GRAY                       8
#define DISPLAY_LIGHT_BLUE                      9
#define DISPLAY_LIGHT_GREEN                     10
#define DISPLAY_LIGHT_CYAN                      11
#define DISPLAY_LIGHT_RED                       12
#define DISPLAY_LIGHT_PURPLE                    13
#define DISPLAY_YELLOW                          14
#define DISPLAY_WHITE                           15

// --------------------------------------------------------------------------------------------------------------------------------

#define DISPLAY_CHARACTER_COLOR(text, bg)       (0x0 | text | (bg << 4))

// --------------------------------------------------------------------------------------------------------------------------------

#define DISPLAY_COLOR                           DISPLAY_CHARACTER_COLOR(DISPLAY_BLACK, DISPLAY_GRAY)
#define DISPLAY_VALID                           DISPLAY_CHARACTER_COLOR(DISPLAY_LIGHT_GREEN, DISPLAY_GRAY)
#define DISPLAY_ERROR                           DISPLAY_CHARACTER_COLOR(DISPLAY_RED, DISPLAY_GRAY)

#define DISPLAY_STATUS_BAR                      DISPLAY_CHARACTER_COLOR(DISPLAY_GRAY, DISPLAY_DARK_GRAY)

#define DISPLAY_KEY_MODE_UNSET                  DISPLAY_STATUS_BAR
#define DISPLAY_KEY_MODE_SET                    DISPLAY_CHARACTER_COLOR(DISPLAY_GREEN, DISPLAY_DARK_GRAY)

#define DISPLAY_TIME                            DISPLAY_CHARACTER_COLOR(DISPLAY_GRAY, DISPLAY_DARK_GRAY)
#define DISPLAY_TIME_TICK                       DISPLAY_STATUS_BAR
#define DISPLAY_TIME_TOCK                       DISPLAY_CHARACTER_COLOR(DISPLAY_WHITE, DISPLAY_DARK_GRAY)

// --------------------------------------------------------------------------------------------------------------------------------

#define DISPLAY_TAB_SIZE                        4

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    unsetNumLock (void);
void    setNumLock (void);

void    unsetCapsLock (void);
void    setCapsLock (void);

void    unsetShift (void);
void    setShift (void);

void    unsetCtrl (void);
void    setCtrl (void);

void    unsetAlt (void);
void    setAlt (void);

// --------------------------------------------------------------------------------------------------------------------------------

void    displayTime (QWORD time);

void    clearScreen (void);

void    printNewLine (void);

void    print (const char *format, ...);

void    println (const char *format, ...);

void    printSuccess (const char *format, ...);

void    printError (const char *format, ...);

void    printHexDump (BYTE *buffer, DWORD buffer_size);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

#endif // _DISPLAY_H

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================
