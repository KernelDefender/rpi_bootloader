OUTPUT_FORMAT("elf32-littlearm")
ENTRY(boot_start)  /* Changed from _start to boot_start */

SECTIONS {
    . = 0x80000;     /* Load address for the bootloader */

    .text : {
        *(.text)     /* Code section */
    }
    .data : {
        *(.data)     /* Data section */
    }
    .bss : {
        *(.bss)      /* BSS section */
    }
}
