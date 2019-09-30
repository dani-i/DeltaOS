// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

#include "interrupts.h"

// ================================================================================================================================
// ================================================================================================================================

void    _clearIDT (void);

// ================================================================================================================================
// ================================================================================================================================

// --------------------------------------------------------------------------------------------------------------------------------

const   char                        *defaultInterrupts [] = {
    "#DE  -  Divide Error",
    "#DB  -  Debug Exception",
    "-MNI -  Nonmaskable external interrupt",
    "#BP  -  Breakpoint",
    "#OF  -  Overflow",
    "#BR  -  BOUND Range Exceeded",
    "#UD  -  Invalid Opcode (Undefined Opcode)",
    "#NM  -  Device Not Available (No Math Coprocessor)",
    "#DF  -  Double Fault",
    "-CSO -  Coprocessor Segment Overrun",
    "#TS  -  Invalid TSS",
    "#NP  -  Segment Not Present",
    "#SS  -  Stack-Segment Fault",
    "#GP  -  General protection",
    "#PF  -  Page Fault",
    "-IR  -  Intel reserved",
    "#MF  -  x87 FPU Floating-Point Error (Math Fault)",
    "#AC  -  Alignment Check",
    "#MC  -  Machine Check",
    "#XM  -  SIMD Floating-Point Exception",
    "#VE  -  Virtualization Exception",
    "#IE  -  Something went wrong! User defined interrupt caught by the default handler.",
    "$"
};

// --------------------------------------------------------------------------------------------------------------------------------

IDT_ENTRY                           idt [256];
IDTR                                idtr;

// --------------------------------------------------------------------------------------------------------------------------------

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    interruptInitialization (void)
{
    PIDTR   idtrPtr = &idtr;

    _clearIDT ();

    idtr.limit = 256 * (sizeof (IDT_ENTRY));
    idtr.baseAddress = (QWORD)(&idt);

    asm (
        ".intel_syntax noprefix\n\t"
        "MOV        RAX,        %0\n\t"
        "LIDT       [RAX]"
        :
        :"p"  (idtrPtr)
        :"rax"
    );

    addInterrupt (handlerDivideError,                       INTERRUPT_DE,           TRAP,           RING_0);
    addInterrupt (handlerDebugException,                    INTERRUPT_DB,           TRAP,           RING_0);
    addInterrupt (handlerNonmaskableExternalInterrupt,      INTERRUPT_NMI,          TRAP,           RING_0);
    addInterrupt (handlerBreakpoint,                        INTERRUPT_BP,           TRAP,           RING_0);
    addInterrupt (handlerOverflow,                          INTERRUPT_OF,           TRAP,           RING_0);
    addInterrupt (handlerBOUNDRangeExceeded,                INTERRUPT_BR,           TRAP,           RING_0);
    addInterrupt (handlerInvalidOpcode,                     INTERRUPT_UD,           TRAP,           RING_0);
    addInterrupt (handlerDeviceNotAvailable,                INTERRUPT_NM,           TRAP,           RING_0);
    addInterrupt (handlerDoubleFault,                       INTERRUPT_DF,           TRAP,           RING_0);
    addInterrupt (handlerCoprocessorSegmentOverrun,         INTERRUPT_CSO,          TRAP,           RING_0);
    addInterrupt (handlerInvalidTSS,                        INTERRUPT_TS,           TRAP,           RING_0);
    addInterrupt (handlerSegmentNotPresent,                 INTERRUPT_NP,           TRAP,           RING_0);
    addInterrupt (handlerStackSegmentFault,                 INTERRUPT_SS,           TRAP,           RING_0);
    addInterrupt (handlerGeneralprotection,                 INTERRUPT_GP,           TRAP,           RING_0);
    addInterrupt (handlerPageFault,                         INTERRUPT_PF,           TRAP,           RING_0);
    addInterrupt (handlerFloatingPointError,                INTERRUPT_MF,           TRAP,           RING_0);
    addInterrupt (handlerAlignmentCheck,                    INTERRUPT_AC,           TRAP,           RING_0);
    addInterrupt (handlerMachineCheck,                      INTERRUPT_MC,           TRAP,           RING_0);
    addInterrupt (handlerFloatingPointException,            INTERRUPT_XM,           TRAP,           RING_0);
    addInterrupt (handlerVirtualizationException,           INTERRUPT_VE,           TRAP,           RING_0);
}

// ================================================================================================================================
// ================================================================================================================================

void    addInterrupt (void (*handler)(void), DWORD number, BYTE type, BYTE ring)
{
    idt [number].offset_15_0               = (QWORD)handler & 0xFFFF;

    idt [number].segmentSelector           = 0x8;

    idt [number].interruptStackTable       = 0;
    idt [number].zero_1                    = 0;
    idt [number].type                      = type;
    idt [number].zero_1                    = 0;
    idt [number].privilegeLevelDescriptor  = ring;
    idt [number].segmentPresentFlag        = 1;

    idt [number].offset_31_16              = (WORD)((QWORD)handler >> 16) & 0xFFFF;
    idt [number].offset_63_32              = (DWORD)((QWORD)handler >> 32) & 0xFFFFFFFF;

    idt [number].reserved                  = 0;

    return;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    genericInterruptHandler (TRAP_FRAME trap_frame)
{
    clearScreen ();

    if ((WORD)trap_frame.handleIndex < 21)
    {
        printError ("\n        %s\n", defaultInterrupts [trap_frame.handleIndex]);
    }
    else
    {
        printError ("\n        %s\n", defaultInterrupts [21]);
    }

    displayTrapFrame (trap_frame);

    asmCLI ();
    asmHLT ();
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    displayTrapFrame (TRAP_FRAME trap_frame)
{
    printNewLine ();
    printNewLine ();
    printNewLine ();

    print ("\t\t\tRAX  %qh", trap_frame.RAX);
    print ("\t\t\tRBX  %qh\n", trap_frame.RBX);
    print ("\t\t\tRCX  %qh", trap_frame.RCX);
    print ("\t\t\tRDX  %qh\n\n\n", trap_frame.RDX);

    print ("\t\t\tR8   %qh", trap_frame.R8);
    print ("\t\t\tR9   %qh\n", trap_frame.R9);
    print ("\t\t\tR10  %qh", trap_frame.R10);
    print ("\t\t\tR11  %qh\n", trap_frame.R11);
    print ("\t\t\tR12  %qh", trap_frame.R12);
    print ("\t\t\tR13  %qh\n", trap_frame.R13);
    print ("\t\t\tR14  %qh", trap_frame.R14);
    print ("\t\t\tR15  %qh\n\n\n", trap_frame.R15);

    print ("\t\t\tCR0  %qh", trap_frame.CR0);
    print ("\t\t\tCR2  %qh\n", trap_frame.CR2);
    print ("\t\t\tCR3  %qh", trap_frame.CR3);
    print ("\t\t\tCR4  %qh\n\n\n", trap_frame.CR4);

    return;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _clearIDT (void)
{
    for (DWORD index = 0; index < 256; index++)
    {
        addInterrupt (handlerInterruptError, INTERRUPT_IE, TRAP, RING_0);
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================
