#include <stdint.h>
#include "interrupts.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "strings.h"
#include "atags.h"

#define ACT_LED_PIN 16
static void timer_irq(void*);

void kmain(
        uint32_t r0 __attribute__((unused)),
        uint32_t r1 __attribute__((unused)),
        uint32_t atags)
{
    set_periph_isr(timer_irq, NULL, ARM_TIMER_IRQ);
    gpio_outp(ACT_LED_PIN);
    arm_timer_t* timer = get_arm_timer();
    timer->load = 0x400;
    timer->ctrl.ctrl = ARM_TIMER_CTRL_TIMER_EN |
                       ARM_TIMER_CTRL_IRQ_EN |
                       ARM_TIMER_CTRL_PRE_SCALE(ARM_TIMER_PRE_SCALE_256) |
                       ARM_TIMER_CTRL_23BIT_CNTR;
    get_irq_cntlr()->enable_basic_irqs = ARM_TIMER_IRQ;
    enable_basic_irqs();

    uart_cfg_t uart_cfg = { .baud = B115200, .bits = BITS8 };
    uart_funcs_t uart_funcs;
    mini_uart_init(&uart_cfg, &uart_funcs);

    if(atags)
    {
        print_atags(&uart_funcs, atags);
    }
}

static void timer_irq(void* data __attribute__((unused)))
{
    static uint32_t on = 1;
    if(on)
    {
        gpio_set(ACT_LED_PIN);
    }
    else
    {
        gpio_clr(ACT_LED_PIN);
    }
    on = !on;
    get_arm_timer()->irq_clear = 1;
}
