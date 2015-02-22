.section ".text.startup"
.globl _start
_start:

    ldr     pc, _reset
    ldr     pc, _undefined_instruction_vector
    ldr     pc, _software_interrupt_vector
    ldr     pc, _prefetch_abort_vector
    ldr     pc, _data_abort_vector
    ldr     pc, _reset
    ldr     pc, _interrupt_vector
    ldr     pc, _fast_interrupt_vector
1:
    mov     r0,  #0x8000
    mov     r1,  #0x0000
    ldmia   r0!, {r2-r9}
    stmia   r1!, {r2-r9}
    ldmia   r0!, {r2-r9}
    stmia   r1!, {r2-r9}
    mov     sp,  #0x8000
    ldr     r4,  =__bss_start__
    ldr     r9,  =__bss_end__
    eor     r5,  r5
    eor     r6,  r6
    eor     r7,  r7
    eor     r8,  r8
    b       3f
2:
    stmia   r4!, {r5-r8}
3:
    cmp     r4, r9
    blo     2b
4:
    ldr     r3, =kmain
    blx     r3
5:
    wfe
    b       5b

_reset:                         .word   1b
_undefined_instruction_vector:  .word   undefined_instruction_vector
_software_interrupt_vector:     .word   software_interrupt_vector
_prefetch_abort_vector:         .word   prefetch_abort_vector
_data_abort_vector:             .word   data_abort_vector
_unused_handler:                .word   1b
_interrupt_vector:              .word   interrupt_vector
_fast_interrupt_vector:         .word   fast_interrupt_vector

