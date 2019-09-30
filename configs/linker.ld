ENTRY (_start)


SECTIONS
{
    /* This is where the kernel is loaded. */
    . = 1M;

    /* All text sections. */
    .text BLOCK (0x1000) : ALIGN (0x1000)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data. */
    .rodata BLOCK (0x1000) : ALIGN (0x1000)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK (0x1000) : ALIGN (0x1000)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK (0x1000) : ALIGN (0x1000)
    {
        *(COMMON)
        *(.bss)
    }
}

