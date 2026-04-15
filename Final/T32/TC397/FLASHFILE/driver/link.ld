ENTRY(_start)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x70100000, LENGTH = 4K
    RAM   (rwx) : ORIGIN = 0x70102000, LENGTH = 16K
}

__USTACK_SIZE = 0x1000;   /* 4 KB */
__ISTACK_SIZE = 0x1000;   /* 4 KB */
__CSA_SIZE    = 0x1000;   /* 4 KB Context Save Area */
_start        = 0x70100000;

SECTIONS
{
    /* Code & Read-only Data in Flash */
    .main 0x70100000 :
    {
        KEEP(*(.main))
    } > FLASH
    
    .data_block 0x70102000 :
    {
        KEEP(*(.data_block))
    } > RAM
    
    .text : {
        *(.startup_code*)
        *(.text*)
        *(.rodata*)
    } > FLASH

    /* Copyable Data — loads from Flash, runs in RAM */
    .data : {
        _data_start = .;
        *(.data*)
        _data_end = .;
    } > RAM AT > FLASH

    /* BSS in RAM */
    .bss (NOLOAD) : {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > RAM

    /* Context Save Area */
    .csa : {
        __CSA_BEGIN = .;
        . += __CSA_SIZE;
        __CSA_END = .;
    } > RAM

    /* Interrupt Stack */
    .istack : {
        . = ALIGN(8);
        __ISTACK = .;
        . += __ISTACK_SIZE;
        __ISTACK_END = .;
    } > RAM

    /* User Stack */
    .ustack : {
        . = ALIGN(8);
        __USTACK = .;
        . += __USTACK_SIZE;
        __USTACK_END = .;
    } > RAM
}

/* Provide the symbols the runtime expects */
PROVIDE(__copy_table = 0);
PROVIDE(__clear_table = 0);
