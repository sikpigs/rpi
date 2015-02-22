#ifndef __ATAGS_H
#define __ATAGS_H

#include <stdint.h>
#include "uart.h"

void print_atags(uart_funcs_t* funcs, uint32_t atags);

#endif // __ATAGS_H

