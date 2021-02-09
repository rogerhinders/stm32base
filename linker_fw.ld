MEMORY
{
  rom (rx) : ORIGIN = 0x08002000, LENGTH = 62K
  ram (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

STACK_SIZE = 0x2000;

SECTIONS
{
    .text 0x08002000:
    {
        KEEP(*(.vectors .vectors.*))
        *(.text*)
        *(.rodata*)
    } > rom

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
    } > ram

    .data :
    {
        *(.data*);
    } > rom
}
