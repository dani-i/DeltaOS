// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

// ================================================================================================================================
// ================================================================================================================================

#include "gdefs.h"

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define INTERRUPT_DE                            0
#define INTERRUPT_DB                            1
#define INTERRUPT_NMI                           2
#define INTERRUPT_BP                            3
#define INTERRUPT_OF                            4
#define INTERRUPT_BR                            5
#define INTERRUPT_UD                            6
#define INTERRUPT_NM                            7
#define INTERRUPT_DF                            8
#define INTERRUPT_CSO                           9
#define INTERRUPT_TS                            10
#define INTERRUPT_NP                            11
#define INTERRUPT_SS                            12
#define INTERRUPT_GP                            13
#define INTERRUPT_PF                            14

// --------------------------------------------------------------------------------------------------------------------------------

                                            //  15          Reserved by intel

// --------------------------------------------------------------------------------------------------------------------------------

#define INTERRUPT_MF                            16
#define INTERRUPT_AC                            17
#define INTERRUPT_MC                            18
#define INTERRUPT_XM                            19
#define INTERRUPT_VE                            20

// --------------------------------------------------------------------------------------------------------------------------------

                                            //  21 - 31     Reserved by intel

// --------------------------------------------------------------------------------------------------------------------------------

#define INTERRUPT_IE                            56

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define RING_0                                  0x0
#define RING_1                                  0x1
#define RING_2                                  0x2
#define RING_3                                  0x3

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define TRAP                                    0xF
#define INTERRUPT                               0xE

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

typedef     struct      _IDT_ENTRY {

    WORD                    offset_15_0;

    WORD                    segmentSelector;

    union {

        struct {

            WORD            interruptStackTable         : 3;
            WORD            zero_1                      : 5;
            WORD            type                        : 4;
            WORD            zero_2                      : 1;
            WORD            privilegeLevelDescriptor    : 2;
            WORD            segmentPresentFlag          : 1;

        };

        WORD    raw;

    };

    WORD                    offset_31_16;

    DWORD                   offset_63_32;

    DWORD                   reserved;

}   IDT_ENTRY,  *PIDT_ENTRY;

// --------------------------------------------------------------------------------------------------------------------------------

typedef     struct      _IDTR
{
    
    union {

        struct {

            QWORD           limit               : 16;
            QWORD           baseAddress         : 48;

        };

        QWORD    raw;

    };

}   IDTR,   *PIDTR;

// --------------------------------------------------------------------------------------------------------------------------------

typedef     struct  _TRAP_FRAME {

    QWORD               CR4;
    QWORD               CR3;
    QWORD               CR2;
    QWORD               CR0;

    QWORD               R15;
    QWORD               R14;
    QWORD               R13;
    QWORD               R12;
    QWORD               R11;
    QWORD               R10;
    QWORD               R9;
    QWORD               R8;

    QWORD               RDX;
    QWORD               RCX;
    QWORD               RBX;
    QWORD               RAX;

    QWORD               handleIndex;

}   TRAP_FRAME;

// ================================================================================================================================
// ================================================================================================================================

// --------------------------------------------------------------------------------------------------------------------------------

void    interruptInitialization (void);

// --------------------------------------------------------------------------------------------------------------------------------

void    addInterrupt (void (*handler)(void), DWORD number, BYTE type, BYTE ring);

// --------------------------------------------------------------------------------------------------------------------------------

void    displayTrapFrame (TRAP_FRAME trap_frame);
void    genericInterruptHandler (TRAP_FRAME trap_frame);

// --------------------------------------------------------------------------------------------------------------------------------

extern  void    handlerDivideError (void);
extern  void    handlerDebugException (void);
extern  void    handlerNonmaskableExternalInterrupt (void);
extern  void    handlerBreakpoint (void);

extern  void    handlerOverflow (void);
extern  void    handlerBOUNDRangeExceeded (void);
extern  void    handlerInvalidOpcode (void);
extern  void    handlerDeviceNotAvailable (void);

extern  void    handlerDoubleFault (void);
extern  void    handlerCoprocessorSegmentOverrun (void);
extern  void    handlerInvalidTSS (void);
extern  void    handlerSegmentNotPresent (void);

extern  void    handlerStackSegmentFault (void);
extern  void    handlerGeneralprotection (void);
extern  void    handlerPageFault (void);
extern  void    handlerFloatingPointError (void);

extern  void    handlerAlignmentCheck (void);
extern  void    handlerMachineCheck (void);
extern  void    handlerFloatingPointException (void);
extern  void    handlerVirtualizationException (void);

extern  void    handlerInterruptError (void);

// --------------------------------------------------------------------------------------------------------------------------------

// ================================================================================================================================
// ================================================================================================================================

#endif // _INTERRUPTS_H

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================
