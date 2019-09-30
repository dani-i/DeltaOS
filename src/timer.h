// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

#ifndef _TIMER_H
#define _TIMER_H

// ================================================================================================================================
// ================================================================================================================================

#include "interrupts.h"
#include "gdefs.h"
#include "pic.h"

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void            timerInitialization (void);

// --------------------------------------------------------------------------------------------------------------------------------

extern void     asmHandlerTimer (void);

void            handlerTimer (void);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QWORD           getTime (void);

void    		sleep (QWORD wait_ticks);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define PIT_MODE_COMMAND_W                                          0x43

#define PIT_IO_PORT_CHANNEL_0_DATA_RW                               0x40
#define PIT_IO_PORT_CHANNEL_1_DATA_RW                               0x41
#define PIT_IO_PORT_CHANNEL_2_DATA_RW                               0x42

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define INTERRUPT_TIMER                                             40

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++    https://wiki.osdev.org/PIT#I.2FO_Ports    +++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------  0  ----------  bcd/bINARY MODE  --------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

#define PIT_16_BIT_MODE                                             0x0     //  0 = 16-BIT BINARY
#define PIT_4_DIGI_BCD                                              0x1     //  1 = FOUR-DIGIT bcd

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------  1 to 3  -----  Operating mode  ---------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

#define PIT_OPERATING_MODE_0_INTERRUPT_ON_TERMINAL_COUNT            0x0     //  000 = Mode 0 (interrupt on terminal count)
#define PIT_OPERATING_MODE_1_HARDWARE_RETRIGGERABLE                 0x2     //  001 = Mode 1 (hardware re-triggerable one-shot)
#define PIT_OPERATING_MODE_2_RATE_GENERATOR                         0x4     //  010 = Mode 2 (rate generator)
#define PIT_OPERATING_MODE_3_SQUARE_WAVE_GENERATOR                  0x6     //  011 = Mode 3 (square wave generator)
#define PIT_OPERATING_MODE_4_SOFTWARE_TRIGGERED_STROBE              0x8     //  100 = Mode 4 (software triggered strobe)
#define PIT_OPERATING_MODE_5_HARDWARE_TRIGGERED_STROBE              0xA     //  101 = Mode 5 (hardware triggered strobe)
#define PIT_OPERATING_MODE_2_RATE_GENERATOR_BIS                     0xC     //  110 = Mode 2 (rate generator, same as 010b)
#define PIT_OPERATING_MODE_3_SQUARE_WAVE_GENERATOR_BIS              0xE     //  111 = Mode 3 (square wave generator, same as 011b)

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------  4 and 5  ----  Access mode  ------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

#define PIT_ACCESS_MODE_LATCH_COUNT_VALUE                           0x00    //  00 = Latch count value command
#define PIT_ACCESS_MODE_LOBYTE_ONLY                                 0x10    //  01 = Access mode: lobyte only
#define PIT_ACCESS_MODE_HIBYTE_ONLY                                 0x20    //  10 = Access mode: hibyte only
#define PIT_ACCESS_MODE_LOBYTE_HIBYTE                               0x30    //  11 = Access mode: lobyte/hibyte

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------  6 and 7  ----  Select channel  ---------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

#define PIT_CHANNEL_SELECTOR_CHANNEL_0                              0x00    //  00 = Channel 0
#define PIT_CHANNEL_SELECTOR_CHANNEL_1                              0x40    //  01 = Channel 1
#define PIT_CHANNEL_SELECTOR_CHANNEL_2                              0x80    //  10 = Channel 2
#define PIT_CHANNEL_SELECTOR_READ_BACK_CMD                          0xC0    //  11 = Read-back command (8254 only)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

#endif // _TIMER_H

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================