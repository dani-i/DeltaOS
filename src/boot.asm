;================================================================================================================================
;================================================================================================================================
;================================================================================================================================
;================================================================================================================================

BITS 32

;================================================================================================================================
;================ Imports =======================================================================================================
;================================================================================================================================

EXTERN  kernel_main

EXTERN  print                                                                   ; (char *format, ...)
EXTERN  println                                                                 ; (char *format, ...)
EXTERN  printSuccess                                                            ; (char *format, ...)
EXTERN  printError                                                              ; (char *format, ...)
EXTERN  printHexDump                                                            ; (BYTE *buffer, DWORD buffer_size)
EXTERN  printUnsigned                                                           ; (DWORD number)
EXTERN  printUnsignedAsHex                                                      ; (DWORD number)

;================================================================================================================================
;================ Exports =======================================================================================================
;================================================================================================================================

; OS entry point.
GLOBAL  _start

; Display functions.
GLOBAL  asmCLI
GLOBAL  asmSTI
GLOBAL  asmHLT

GLOBAL  asmClearScreen
GLOBAL  okEnd
GLOBAL  errEnd

; Data
GLOBAL  MultiBootHeader

GLOBAL  debug_area

;================================================================================================================================
;================ MultiBoot =====================================================================================================
;================================================================================================================================

SECTION .multiboot
ALIGN   4

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    MULTIBOOT_FLAG_4KB_ALIGN            EQU     1 << 0                          ; 4KB alignment.
    MULTIBOOT_FLAG_MEM_INFO             EQU     1 << 1                          ; Memory info.
   ;MULTIBOOT_FLAG_VIDEO                EQU     1 << 2                          ; Video info.

    MULTIBOOT_MAGIC                     EQU     0x1BADB002                      ; Magic value for multiboot.
    MULTIBOOT_FLAGS                     EQU     MULTIBOOT_FLAG_4KB_ALIGN | MULTIBOOT_FLAG_MEM_INFO ;| MULTIBOOT_FLAG_VIDEO
    MULTIBOOT_CHECKSUM                  EQU     0 - MULTIBOOT_MAGIC - MULTIBOOT_FLAGS

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    MultiBootHeader:

        .magic                          DD      MULTIBOOT_MAGIC                 ; Required
        .flags                          DD      MULTIBOOT_FLAGS                 ; Required
        .checksum                       DD      MULTIBOOT_CHECKSUM              ; Required
        .header_addr                    DD      0                               ; If flags [16] is set
        .load_addr                      DD      0                               ; If flags [16] is set
        .load_end_addr                  DD      0                               ; If flags [16] is set
        .bss_end_addr                   DD      0                               ; If flags [16] is set
        .entry_addr                     DD      0                               ; If flags [16] is set
        .mode_type                      DD      0                               ; If flags [2] is set
        .width                          DD      0                               ; If flags [2] is set
        .height                         DD      0                               ; If flags [2] is set
        .depth                          DD      0                               ; If flags [2] is set

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

;================================================================================================================================
;================================================================================================================================

SECTION .text
ALIGN   4

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    _start:

        ;------------------------------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Disable interrupts.

        CLI

        ;------------------------------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Set the top of the stack.

        MOV         ESP,                kernel_stack_top

        ;------------------------------------------------------------------------------------------------------------------------

        CALL        asmClearScreen

        ;------------------------------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Check whether the value in EAX is 0x2badb002 or not.

        CMP         EAX,                0x2badb002
        JNE         .badBoot

        JMP         .goodBoot

        .badBoot:

            MOV     DWORD [0xB8000],    'E|r|'
            MOV     DWORD [0xB8004],    'r| |'
            MOV     DWORD [0xB8008],    'B|o|'
            MOV     DWORD [0xB800C],    'o|t|'

            CALL    errEnd

        .goodBoot:

            MOV     DWORD [0xB8000],    'Ozkz'
            MOV     DWORD [0xB8004],    ' z z'
            MOV     DWORD [0xB8008],    'Bzoz'
            MOV     DWORD [0xB800C],    'oztz'

        ;------------------------------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Save in the display memory area the address of various "variables" of interest.

        MOV         ECX,                0xB809C
        MOV         EAX,                kernel_stack_top
        MOV         [ECX],              EAX

        ADD         ECX,                0xA0
        MOV         EAX,                debug_area
        MOV         [ECX],              EAX

        ADD         ECX,                0xA0
        MOV         EAX,                PML4T
        MOV         [ECX],              EAX

        ADD         ECX,                0xA0
        MOV         EAX,                PDPT
        MOV         [ECX],              EAX

        ADD         ECX,                0xA0
        MOV         EAX,                PDT
        MOV         [ECX],              EAX

        ADD         ECX,                0xA0
        MOV         EAX,                PT
        MOV         [ECX],              EAX

        ADD         ECX,                0xA0
        MOV         EAX,                paging_end
        MOV         [ECX],              EAX

        ;------------------------------------------------------------------------------------------------------------------------
        ;---------------- Checks ------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Check whether the bootloader got us into protected mod.

        MOV         EAX,                CR0
        AND         EAX,                CR0_PE

        CMP         EAX,                CR0_PE
        JNE         .notInProtectedMode

        JMP         .inProtectedMode

        .notInProtectedMode:

            MOV     DWORD [0xB80A0],    'E|r|'
            MOV     DWORD [0xB80A4],    'r| |'
            MOV     DWORD [0xB80A8],    'P|r|'
            MOV     DWORD [0xB80AC],    'o|t|'
            MOV     DWORD [0xB80B0],    'e|c|'
            MOV     DWORD [0xB80B4],    't|e|'
            MOV     DWORD [0xB80B8],    'd| |'

            CALL    errEnd

        .inProtectedMode:

            MOV     DWORD [0xB80A0],    'Ozkz'
            MOV     DWORD [0xB80A4],    ' z z'
            MOV     DWORD [0xB80A8],    'Pzrz'
            MOV     DWORD [0xB80AC],    'oztz'
            MOV     DWORD [0xB80B0],    'ezcz'
            MOV     DWORD [0xB80B4],    'tzez'
            MOV     DWORD [0xB80B8],    'dz z'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Check if CPUID is available.

        PUSHFD
        POP         EAX
        MOV         ECX,                EAX

        XOR         EAX,                1 << 21
        PUSH        EAX
        POPFD

        PUSHFD
        POP         EAX

        PUSH        ECX
        POPFD

        XOR         EAX,                ECX
        JZ          .noCPUID

        JMP         .okCPUID

        .noCPUID:

            MOV     DWORD [0xB8140],    'E|r|'
            MOV     DWORD [0xB8144],    'r| |'
            MOV     DWORD [0xB8148],    'C|P|'
            MOV     DWORD [0xB814C],    'U|I|'
            MOV     DWORD [0xB8150],    'D| |'

            CALL    errEnd

        .okCPUID:

            MOV     DWORD [0xB8140],    'Ozkz'
            MOV     DWORD [0xB8144],    ' z z'
            MOV     DWORD [0xB8148],    'CzPz'
            MOV     DWORD [0xB814C],    'UzIz'
            MOV     DWORD [0xB8150],    'Dz z'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Check if the extended CPUID features are present.

        MOV         EAX,                0x80000000
        CPUID

        CMP         EAX,                0x80000001
        JE          .noExtendedCPUID

        JMP         .extendedCPUID

        .noExtendedCPUID:

            MOV     DWORD [0xB81E0],    'E|r|'
            MOV     DWORD [0xB81E4],    'r| |'
            MOV     DWORD [0xB81E8],    'E|x|'
            MOV     DWORD [0xB81EC],    't|e|'
            MOV     DWORD [0xB81F0],    'n|d|'
            MOV     DWORD [0xB81F4],    'e|d|'
            MOV     DWORD [0xB81F8],    ' |C|'
            MOV     DWORD [0xB81FC],    'P|U|'
            MOV     DWORD [0xB8200],    'I|D|'

            CALL    errEnd

        .extendedCPUID:

            MOV     DWORD [0xB81E0],    'Ozkz'
            MOV     DWORD [0xB81E4],    ' z z'
            MOV     DWORD [0xB81E8],    'Ezxz'
            MOV     DWORD [0xB81EC],    'tzez'
            MOV     DWORD [0xB81F0],    'nzdz'
            MOV     DWORD [0xB81F4],    'ezdz'
            MOV     DWORD [0xB81F8],    ' zCz'
            MOV     DWORD [0xB81FC],    'PzUz'
            MOV     DWORD [0xB8200],    'IzDz'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Check if long mode is supported.

        MOV         EAX,                0x80000001
        CPUID

        TEST        EDX,                1 << 29                         ; Tests if the LM-bit (29th) is set.
        JZ          .noLongMode

        JMP         .longMode

        .noLongMode:

            MOV     DWORD [0xB8280],    'E|r|'
            MOV     DWORD [0xB8284],    'r| |'
            MOV     DWORD [0xB8288],    'L|o|'
            MOV     DWORD [0xB828C],    'n|g|'
            MOV     DWORD [0xB8290],    ' |m|'
            MOV     DWORD [0xB8294],    'o|d|'
            MOV     DWORD [0xB8298],    'e| |'

            CALL    errEnd

        .longMode:

            MOV     DWORD [0xB8280],    'Ozkz'
            MOV     DWORD [0xB8284],    ' z z'
            MOV     DWORD [0xB8288],    'Lzoz'
            MOV     DWORD [0xB828C],    'nzgz'
            MOV     DWORD [0xB8290],    ' zmz'
            MOV     DWORD [0xB8294],    'ozdz'
            MOV     DWORD [0xB8298],    'ez z'

        ;------------------------------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Enable A20 line    -    https://wiki.osdev.org/A20

        IN          EAX,                0x92
        OR          EAX,                0x2
        OUT         0x92,               EAX

        MOV         DWORD [0xB8320],    ' p p'
        MOV         DWORD [0xB8324],    ' p p'
        MOV         DWORD [0xB8328],    'Ap2p'
        MOV         DWORD [0xB832C],    '0p p'
        MOV         DWORD [0xB8330],    'Lpip'
        MOV         DWORD [0xB8334],    'npep'
        MOV         DWORD [0xB8338],    ' pep'
        MOV         DWORD [0xB833C],    'npap'
        MOV         DWORD [0xB8340],    'bplp'
        MOV         DWORD [0xB8344],    'epdp'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Disable paging by reseting the 31th bit in CR0.     -    Should be cleared, just for good measures.

        MOV         EAX,                CR0

        MOV         EBX,                CR0_PG
        NOT         EBX
        AND         EAX,                EBX

        MOV         CR0,                EAX

        MOV         DWORD [0xB83C0],    ' p p'
        MOV         DWORD [0xB83C4],    ' p p'
        MOV         DWORD [0xB83C8],    'Ppap'
        MOV         DWORD [0xB83CC],    'gpip'
        MOV         DWORD [0xB83D0],    'npgp'
        MOV         DWORD [0xB83D4],    ' pdp'
        MOV         DWORD [0xB83D8],    'ipsp'
        MOV         DWORD [0xB83DC],    'apbp'
        MOV         DWORD [0xB83E0],    'lpep'
        MOV         DWORD [0xB83E4],    'dp p'

        ;------------------------------------------------------------------------------------------------------------------------
        ;---------------- Paging ------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Set PML4T    -    1 entry = 512 GiB

        ;   For now only the first entry is used, as a result no code is needed since it's easier to have it set at
        ; compilation.

        MOV         DWORD [0xB8460],    ' p p'
        MOV         DWORD [0xB8464],    ' p p'
        MOV         DWORD [0xB8468],    'PpMp'
        MOV         DWORD [0xB846C],    'Lp4p'
        MOV         DWORD [0xB8470],    'Tp p'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Set PDPT     -    1 entry = 1 GiB

        ;   For now only the first entry is used, as a result no code is needed since it's easier to have it set at
        ; compilation.

        MOV         DWORD [0xB8500],    ' p p'
        MOV         DWORD [0xB8504],    ' p p'
        MOV         DWORD [0xB8508],    'PpDp'
        MOV         DWORD [0xB850C],    'PpTp'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Set PDT      -    1 entry = 2 MiB

        MOV         ECX,                0

        .loopPDT:

            MOV     EBX,                PAGE_SIZE
            MOV     EAX,                ECX
            MUL     EBX

            MOV     EBX,                PT
            ADD     EBX,                EAX

            OR      EBX,                INITIAL_PAGE_PERMS

            MOV     [PDT + ECX * 8],    EBX

            INC     ECX
            CMP     ECX,                512
            JL      .loopPDT

        MOV         DWORD [0xB85A0],    ' p p'
        MOV         DWORD [0xB85A4],    ' p p'
        MOV         DWORD [0xB85A8],    'PpDp'
        MOV         DWORD [0xB85AC],    'Tp p'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Set PT       -    1 entry = 4 KiB

        ; 512 * 512 entries => 262144 entries

        MOV         ECX,                0
        MOV         EAX,                INITIAL_PAGE_PERMS

        .loopPT:

            MOV     [PT + ECX * 8],     EAX

            ADD     EAX,                PAGE_SIZE

            INC     ECX
            CMP     ECX,                262144
            JL      .loopPT

        MOV         DWORD [0xB8640],    ' p p'
        MOV         DWORD [0xB8644],    ' p p'
        MOV         DWORD [0xB8648],    'PpTp'

        ;------------------------------------------------------------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------
        ; Enable PAE & PSE bits in CR4.

        MOV         EAX,                CR4
        OR          EAX,                CR4_PSE
        OR          EAX,                CR4_PAE
        MOV         CR4,                EAX

        MOV         DWORD [0xB86E0],    ' p p'
        MOV         DWORD [0xB86E4],    ' p p'
        MOV         DWORD [0xB86E8],    'Spep'
        MOV         DWORD [0xB86EC],    'tp p'
        MOV         DWORD [0xB86F0],    'CpRp'
        MOV         DWORD [0xB86F4],    '4p p'
        MOV         DWORD [0xB86F8],    'PpAp'
        MOV         DWORD [0xB86FC],    'Ep p'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Load the physical address of PML4T in CR3.

        MOV         EAX,                PML4T
        MOV         CR3,                EAX

        MOV         DWORD [0xB8780],    ' p p'
        MOV         DWORD [0xB8784],    ' p p'
        MOV         DWORD [0xB8788],    'Lpop'
        MOV         DWORD [0xB878C],    'apdp'
        MOV         DWORD [0xB8790],    'epdp'
        MOV         DWORD [0xB8794],    ' pPp'
        MOV         DWORD [0xB8798],    'MpLp'
        MOV         DWORD [0xB879C],    '4pTp'
        MOV         DWORD [0xB87A0],    ' pip'
        MOV         DWORD [0xB87A4],    'np p'
        MOV         DWORD [0xB87A8],    'CpRp'
        MOV         DWORD [0xB87AC],    '3p p'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Set bit 8 in MSR 0x0000080 to enable long mode.

        MOV         ECX,                IA32_EFER
        RDMSR
        OR          EAX,                IA32_EFER_LME
        WRMSR

        MOV         DWORD [0xB8820],    ' p p'
        MOV         DWORD [0xB8824],    ' p p'
        MOV         DWORD [0xB8828],    'Epnp'
        MOV         DWORD [0xB882C],    'apbp'
        MOV         DWORD [0xB8830],    'lpep'
        MOV         DWORD [0xB8834],    'dp p'
        MOV         DWORD [0xB8838],    'Lpop'
        MOV         DWORD [0xB883C],    'npgp'
        MOV         DWORD [0xB8840],    ' pmp'
        MOV         DWORD [0xB8844],    'opdp'
        MOV         DWORD [0xB8848],    'ep p'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Activate paging by setting the PG (31th) bit in CR0.

        MOV         EAX,                CR0
        OR          EAX,                CR0_PG                      ; 0x80000000

        MOV         CR0,                EAX

        MOV         DWORD [0xB88C0],    ' p p'
        MOV         DWORD [0xB88C4],    ' p p'
        MOV         DWORD [0xB88C8],    'Ppap'
        MOV         DWORD [0xB88CC],    'gpip'
        MOV         DWORD [0xB88D0],    'npgp'
        MOV         DWORD [0xB88D4],    ' pep'
        MOV         DWORD [0xB88D8],    'npap'
        MOV         DWORD [0xB88DC],    'bplp'
        MOV         DWORD [0xB88E0],    'epdp'

        ;------------------------------------------------------------------------------------------------------------------------
        ;---------------- The CPU is now in Compatibility Mode ------------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------

        ;------------------------------------------------------------------------------------------------------------------------
        ; Set 64-bit GDT (code and data segment).

        LGDT        [GDT64.pointer]

        MOV         DWORD [0xB8960],    ' p p'
        MOV         DWORD [0xB8964],    ' p p'
        MOV         DWORD [0xB8968],    'GpDp'
        MOV         DWORD [0xB896C],    'Tp p'
        MOV         DWORD [0xB8970],    'spep'
        MOV         DWORD [0xB8974],    'tp p'

        ;------------------------------------------------------------------------------------------------------------------------
        ; Enter long mode by doing a far jump to the kernel entry point.

        MOV         DWORD [0xB8A00],    ' p p'
        MOV         DWORD [0xB8A04],    ' p p'
        MOV         DWORD [0xB8A08],    'Fpap'
        MOV         DWORD [0xB8A0C],    'rp p'
        MOV         DWORD [0xB8A10],    'jpup'
        MOV         DWORD [0xB8A14],    'mppp'
        MOV         DWORD [0xB8A18],    ' ptp'
        MOV         DWORD [0xB8A1C],    'op p'
        MOV         DWORD [0xB8A20],    'kpep'
        MOV         DWORD [0xB8A24],    'rpnp'
        MOV         DWORD [0xB8A28],    'eplp'
        MOV         DWORD [0xB8A2C],    ' pep'
        MOV         DWORD [0xB8A30],    'nptp'
        MOV         DWORD [0xB8A34],    'rpyp'
        MOV         DWORD [0xB8A38],    ' ppp'
        MOV         DWORD [0xB8A3C],    'opip'
        MOV         DWORD [0xB8A40],    'nptp'

        JMP         GDT64.code:toLongMode

        ;------------------------------------------------------------------------------------------------------------------------
        ;---------------- The OS ended its execution successfully ---------------------------------------------------------------
        ;------------------------------------------------------------------------------------------------------------------------

        CALL        okEnd

        ;------------------------------------------------------------------------------------------------------------------------

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    toLongMode:

        MOV         EAX,                0x0

        MOV         SS,                 EAX
        MOV         DS,                 EAX
        MOV         ES,                 EAX
        MOV         FS,                 EAX
        MOV         GS,                 EAX

        CALL        kernel_main

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    asmClearScreen:

        MOV         ECX,                0x000B8000

        .loop:

            MOV     DWORD [ECX],        0x70007000
            ADD     ECX,                4

            CMP     ECX,                0x000B8fa0
            JLE     .loop

        RET

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    asmCLI:

        CLI
        RET

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    asmSTI:

        STI
        RET

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    asmHLT:

        HLT

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    okEnd:

        MOV         BYTE [0x000B8F9F],  0xa0

        CLI
        HLT

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ; Call this if something goes bad.

    errEnd:

        MOV         BYTE [0x000B8F9F],  0xc0

        CLI
        HLT

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

;================================================================================================================================
;================================================================================================================================

SECTION .data
ALIGN   0x1000

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ;++++++++++++++++ Constants +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    KERNEL_STACK_SIZE                                   EQU     4096

    INITIAL_PAGE_PERMS                                  EQU     0x7

    PAGE_SIZE                                           EQU     0x1000

    CR0_PE                                              EQU     0x1
    CR0_PG                                              EQU     0x80000000

    CR4_PSE                                             EQU     0x00000010
    CR4_PAE                                             EQU     0x00000020

    IA32_EFER                                           EQU     0xC0000080
    IA32_EFER_LME                                       EQU     0x100

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ;++++++++++++++++ Data ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    ;----------------------------------------------------------------------------------------------------------------------------
    ;---------------- Memory area dedicated to debug purposes -------------------------------------------------------------------
    ;----------------------------------------------------------------------------------------------------------------------------

    debug_index                                         DD      0x0
    debug_area                  TIMES   4095            DD      0x44042047

    ;----------------------------------------------------------------------------------------------------------------------------
    ;---------------- Paging tables ---------------------------------------------------------------------------------------------
    ;----------------------------------------------------------------------------------------------------------------------------

    PML4T:
                                                        DQ      PDPT + INITIAL_PAGE_PERMS
                                TIMES   511             DQ      0

    ;----------------------------------------------------------------------------------------------------------------------------

    PDPT:
                                                        DQ      PDT + INITIAL_PAGE_PERMS
                                TIMES   511             DQ      0

    ;----------------------------------------------------------------------------------------------------------------------------

    PDT:
                                TIMES   512             DQ      0

    ;----------------------------------------------------------------------------------------------------------------------------

    PT:
                                TIMES   512 * 512       DQ      0

    ;----------------------------------------------------------------------------------------------------------------------------

    paging_end                                          DQ      0x1223334444555556

    ;----------------------------------------------------------------------------------------------------------------------------

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

;================================================================================================================================
;================================================================================================================================

SECTION .rodata
ALIGN   4

    ;----------------------------------------------------------------------------------------------------------------------------

    GDT64:
                                                        DQ      0x0

    .code:                                              EQU     $ - GDT64
                                                        DQ      (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment

    .pointer:
                                                        DW      $ - GDT64 - 1
                                                        DQ      GDT64

    ;----------------------------------------------------------------------------------------------------------------------------

;================================================================================================================================
;================================================================================================================================

SECTION .bss
ALIGN   4

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    ALIGN 4
    kernel_stack_bottom:

         RESB        KERNEL_STACK_SIZE

    kernel_stack_top:

    ;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

;================================================================================================================================
;================================================================================================================================
;================================================================================================================================
;================================================================================================================================