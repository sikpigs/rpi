#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include "bits.h"

#define GPIO_ALT_FUNC_0 4
#define GPIO_ALT_FUNC_1 5
#define GPIO_ALT_FUNC_2 6
#define GPIO_ALT_FUNC_3 7
#define GPIO_ALT_FUNC_4 3
#define GPIO_ALT_FUNC_5 2

#define GPIO_BASE  0x20200000
#define GPFSEL0    (GPIO_BASE | 0x00)
#define GPFSEL1    (GPIO_BASE | 0x04)
#define GPPUD      (GPIO_BASE | 0x94)
#define GPPUDCLK0  (GPIO_BASE | 0x98)
#define GPPUDCLK1  (GPIO_BASE | 0x9C)

#define GPPUD_PULL_UP   2
#define GPPUD_PULL_DOWN 1
#define GPPUD_OFF       0
#define gpio_set_gppud(v) write32(GPPUD, v)

void gpio_outp(uint32_t pin);
void gpio_clr(uint32_t pin);
void gpio_set(uint32_t pin);
void gpio_set_func(uint32_t pin, uint32_t function);
void gpio_set_gppudclk(uint32_t pin);
void gpio_clr_gppudclk(uint32_t pin);

#endif // __GPIO_H

