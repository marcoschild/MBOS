/* === linker.ld === */
ENTRY(start)

SECTIONS
{
    /* Kernel starts at 0x1000 in memory */
    . = 0x1000;

    /* Code/text segment */
    .text :
    {
        *(.multiboot)   /* if multiboot header is used later */
        *(.text*)       /* all code */
    }

    /* Read-only data (e.g. strings) */
    .rodata :
    {
        *(.rodata*)
    }

    /* Initialized data */
    .data :
    {
        *(.data*)
    }

    /* Uninitialized data */
    .bss :
    {
        *(.bss*)
        *(COMMON)
    }
}
