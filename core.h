#ifndef __CORE_H
#define __CORE_H

#include <stdint.h>

#define write32(addr, data) \
    asm("str %[d], [%[a]]" : : [d]"r"(data), [a]"r"(addr) : )

#define spin(cycles) \
    asm volatile("1: subs %[c], %[c], #1; bne 1b;" : : [c]"r"(cycles) : "cc")

#define isb() __asm__ __volatile__ ("mcr p15, 0, %0, c7,  c5, 4" : : "r" (0) : "memory")
#define dmb() __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory")
#define dsb() __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory")

uint32_t read32(uint32_t addr);
void reset(void);
void halt(void);

#endif // __CORE_H

