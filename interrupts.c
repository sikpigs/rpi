#include <stddef.h>

#include "interrupts.h"
#include "timer.h"
#include "core.h"

#define INTERRUPT_CONTROLLER_BASE 0x2000B200

static void def_periph_isr(void*);

static struct
{
    isr_t isr;
    void* data;
} periph_isr_table[] = 
{
    { def_periph_isr, NULL }, // Timer
    { def_periph_isr, NULL }, // Mailbox
    { def_periph_isr, NULL }, // Doorbell 0
    { def_periph_isr, NULL }, // Doorbell 1
    { def_periph_isr, NULL }, // GPU0 Halted
    { def_periph_isr, NULL }, // GPU1 Halted
    { def_periph_isr, NULL }, // Illegal Access Type 1
    { def_periph_isr, NULL }  // Illegal Access Type 0
};

irq_cntlr_t* get_irq_cntlr(void)
{
    return (irq_cntlr_t*)(INTERRUPT_CONTROLLER_BASE);
}

void enable_basic_irqs(void)
{
    asm volatile(
        "mrs r0, cpsr;" 
        "bic r0, r0, #0x80;"
        "msr cpsr_c, r0;"
    : :);
}

void set_periph_isr(isr_t isr, void* data, uint32_t periph_type)
{
    if(isr && periph_type <= ILLEGAL_ACCESS_0_IRQ)
    {
        periph_isr_table[periph_type].isr = isr;
        periph_isr_table[periph_type].data = data;
    }
    else if(!isr)
    {
        periph_isr_table[periph_type].isr = def_periph_isr;
        periph_isr_table[periph_type].data = NULL;
    }
}

#define ISR(type, nm) void __attribute__((interrupt(#type))) nm(void)

ISR(UNDEF, undefined_instruction_vector)
{
    reset();
}

ISR(SWI, software_interrupt_vector)
{
    reset();
}

ISR(ABORT, prefetch_abort_vector)
{
    reset();
}

ISR(ABORT, data_abort_vector)
{
    reset();
}

ISR(FIQ, fast_interrupt_vector)
{
    reset();
}

ISR(IRQ, interrupt_vector)
{
    uint32_t const irq_basic = get_irq_cntlr()->irq_basic_pending;
    if(irq_basic & 0xFF)
    {
        for(uint32_t i = 0; i < 8; ++i)
        {
            if(irq_basic & (1 << i))
            {
                periph_isr_table[i].isr(periph_isr_table[i].data);
            }
        }
    }
}

static void def_periph_isr(void* data __attribute__((unused)))
{
}
