.section .text
.global boot_start  /* Changed from _start to boot_start */

boot_start:
    bl main          /* Call the main function */
    b .              /* Infinite loop to prevent exiting */
