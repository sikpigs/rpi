#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <stdint.h>
#include "bits.h"

#define ARM_TIMER_IRQ        0
#define ARM_MAILBOX _IRQ     1
#define ARM_DOORBELL_0_IRQ   2
#define ARM_DOORBELL_1_IRQ   3
#define GPU0_HALTED_IRQ      4 
#define GPU1_HALTED_IRQ      5
#define ILLEGAL_ACCESS_1_IRQ 6
#define ILLEGAL_ACCESS_0_IRQ 7

typedef uint8_t periph_type_t;

typedef struct
{
    volatile uint32_t irq_basic_pending;
    volatile uint32_t irq_pending_1;
    volatile uint32_t irq_pending_2;
    volatile uint32_t fiq_cntl;
    volatile uint32_t enable_irqs_1;
    volatile uint32_t enable_irqs_2;
    volatile uint32_t enable_basic_irqs;
    volatile uint32_t disable_irqs_1;
    volatile uint32_t disable_irqs_2;
    volatile uint32_t disable_basic_irqs;
} irq_cntlr_t;

typedef void (*isr_t)(void*);

irq_cntlr_t* get_irq_cntlr(void);
void enable_basic_irqs(void);
void set_periph_isr(isr_t hndlr, void* data, periph_type_t periph_type);

#endif // __INTERRUPTS_H
