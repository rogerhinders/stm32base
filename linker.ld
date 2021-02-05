MEMORY
{
  rom (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  ram (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

STACK_SIZE = 0x2000;

SECTIONS
{
    .text 0x08000000:
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
