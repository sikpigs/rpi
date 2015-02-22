.section ".text"

/*
 * Name:
 *   gpio_set_func, set a pin function
 * Parameters:
 *   r0 = PIN#
 *   r1 = Function
 * Return:
 *   None
 */
.globl gpio_set_func
gpio_set_func:
    push   {r4, lr}
    ldr    r3, =0x20200000
1:  // loop until we find which function select (r1) and which pin in bank (r0)
    cmp    r0, #9
    subhi  r0, r0, #10 // pin /= 10
    addhi  r3, r3, #4  // gpio += 4 * (pin % 10)
    bhi    1b

    // prepare for RMW on GPIO

    add    r0, r0, lsl #1   // shift *= 3
    lsl    r1, r1, r0       // func <<= shift

    mov    r4, #7
    lsl    r4, r0           // mask <<= shift

    ldr    r2, [r3]         // load current value
    bic    r2, r2, r4       // val &= ~mask
    orr    r2, r2, r1       // val |= func
    str    r2, [r3]         // store updated value from r0

    pop    {r4, pc}

/*
 * Name:
 *   gpio_outp, set a pin to output mode
 * Parameters:
 *   r0 = PIN#
 *   r1 = Function
 * Return:
 *   None
 */
.globl gpio_outp
gpio_outp:
    mov   r1, #1
    b     gpio_set_func

/*
 * Name:
 *   gpio_inp, set a pin to input mode
 * Parameters:
 *   r0 = PIN#
 * Return:
 *   None
 */
.globl gpio_inp
gpio_inp:
    push  {lr}
    ldr   r3, =0x20200000
1:  // loop until we find which function select (r1) and which pin in in bank (r0)
    cmp   r0, #9
    subhi r0, #10           // r0 /= 10
    addhi r3, #4            // r1 = 4 * (r0 % 10)
    bhi   1b
    add   r0, r0, lsl #1    // r0 *= 3
    lsl   r0, r0, #7        // put 111b into position
    ldr   r2, [r3]          // load current value
    bic   r2, r0            // and current value with r0
    str   r2, [r3]          // store the updated value
    pop   {pc}

/*
 * Name:
 *   gpio_clr, set pin to ground (clear)
 * Parameters:
 *   r0 = PIN#
 * Return:
 *   None
 */
.globl gpio_clr
gpio_clr:
    push  {lr}
    ldr   r3, =0x20200000
    lsr   r2, r0, #5        // find pin bank by / 32, pins 0-31 are @ 0x20200000 and 32-53 are @ 0x20200004
    lsl   r2, #2            // r2 now is either 0 or 4
    add   r3, r2            // add bank to base address
    mov   r2, #1
    and   r0, #31           // get remainder of PIN# / 32
    lsl   r2, r0            // r2 is now the bit of the PIN
    str   r2, [r3, #0x28]   // turn on the bit to CLR
    pop   {pc}

/*
 * Name:
 *   gpio_set, set pin to high (set)
 * Parameters:
 *   r0 = PIN#
 * Return:
 *   None
 */
.globl gpio_set
gpio_set:
    push  {lr}
    ldr   r3, =0x20200000
    lsr   r2, r0, #5        // find pin bank by / 32, pins 0-31 are @ 0x20200000 and 32-53 are @ 0x20200004
    lsl   r2, #2            // r2 now is either 0 or 4
    add   r3, r2            // add bank to base address
    mov   r2, #1
    and   r0, #31           // get remainder of PIN# / 32
    lsl   r2, r0            // r2 is now the bit of the PIN
    str   r2, [r3, #0x1C]   // turn on the bit to SET
    pop   {pc}

/*
 * Name:
 *   gpio_set_gppudclk
 * Parameters:
 *   r0 = PIN#
 * Return:
 *   None
 */
.globl gpio_set_gppudclk
gpio_set_gppudclk:
    push   {lr}
    ldr   r3, =0x20200000
    lsr   r2, r0, #5        // find pin bank by / 32, pins 0-31 are @ 0x20200098 and 32-53 are @ 0x2020009C
    lsl   r2, #2            // r2 now is either 0 or 4
    add   r3, r2            // add bank to base address
    mov   r2, #1
    and   r0, #31           // get remainder of PIN# / 32
    lsl   r2, r0            // r2 is now the bit of the PIN
    str   r2, [r3, #0x98]
    pop   {pc}

/*
 * Name:
 *   gpio_clr_gppudclk
 * Parameters:
 *   r0 = PIN#
 * Return:
 *   None
 */
.globl gpio_clr_gppudclk
gpio_clr_gppudclk:
    push   {lr}
    ldr   r3, =0x20200000
    lsr   r2, r0, #5        // find pin bank by / 32, pins 0-31 are @ 0x20200098 and 32-53 are @ 0x2020009C
    lsl   r2, #2            // r2 now is either 0 or 4
    add   r3, r2            // add bank to base address
    mov   r2, #0
    str   r2, [r3, #0x98]
    pop   {pc}

