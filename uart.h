#ifndef __UART_H
#define __UART_H

#include "bits.h"
#include <stdint.h>

typedef uint16_t baud_rate_t;

#define B1200   ((baud_rate_t)26041)
#define B2400   ((baud_rate_t)13020)
#define B4800   ((baud_rate_t)6509)
#define B9600   ((baud_rate_t)3254)
#define B19200  ((baud_rate_t)1627)
#define B38400  ((baud_rate_t)813)
#define B57600  ((baud_rate_t)542)
#define B115200 ((baud_rate_t)270)

#define BITS7 0
#define BITS8 3

typedef struct
{
    baud_rate_t baud;
    uint8_t bits;
} uart_cfg_t;

typedef void (*uart_write_t)(uint8_t const* p, uint32_t count);
typedef struct
{
    uart_write_t write;
} uart_funcs_t;

void mini_uart_init(uart_cfg_t*, uart_funcs_t*);

#endif // __UART_H
