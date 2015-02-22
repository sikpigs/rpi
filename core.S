.section ".text"

/* Name:
 *   read32
 * Description:
 *   Reads 32 bits from some address (r0)
 * Parameters:
 *   r0 - Address from which to read 32 bit
 * Return:
 *   r0 - 32 bits of data
 */
.globl read32
read32:
    ldr r0, [r0]
    bx  lr

/*
 * Name:
 *   reset
 * Description:
 *   Resets the CPU
 * Parameters:
 *   None
 * Return:
 *   Nothing, really, this is a reset function
 */
.globl reset
reset:
    ldr r0, =0x20100000     // Base address of PM region
    mov r1, #0x5a000000     // PM Password

    orr r2, r1, #0x10       // 16 ticks to reboot
    str r2, [r0, #0x24]

    ldr r2, [r0, #0x1c]
    mvn r3, #0x30
    and r2, r3
    orr r2, r1
    orr r2, #0x20           // r2 = (r2 & ~0x30) | 0x5A000020 (0x20 means full reset)
    str r2, [r0, #0x1c]

    bx  lr

/*
 * Name:
 *   halt
 * Description:
 *   'Halts' the CPU
 * Parameters:
 *   None
 * Return:
 *   Nothing, really, this is a halt function
 */
.globl halt
halt:
    ldr r0, =0x20100000     // Base address of PM region
    mov r1, #0x5a000000     // PM Password

    ldr r2, [r0, #0x20]
    mvn r3, #0x30
    and r2, r3
    orr r2, r1
    orr r2, #0x40           // r2 = (r2 & ~0x30) | 0x5A000040 (0x40 tells bootcode.bin not to reload)
    str r2, [r0, #0x40]

    bl reset
    bx  lr
