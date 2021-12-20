ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 1M;
    .text :
    {
        *(.text)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096)
    {
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    .asm : ALIGN(4096)
    {
        *(.asm)
    }
}