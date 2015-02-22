#include "uart.h"
#include "gpio.h"
#include "core.h"

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

#define AUX_ENABLE_MINI_UART BIT(0)

#define AUX_MU_IIR_REG_TX_FIFO_EN   BIT(7)
#define AUX_MU_IIR_REG_RX_FIFO_EN   BIT(6)
#define AUX_MU_IIR_REG_TX_FIFO_RST  BIT(2)
#define AUX_MU_IIR_REG_RX_FIFO_RST  BIT(1)
#define AUX_MU_IIR_REG_IRQ_PEND_RST BIT(0)

#define AUX_MU_IER_REG_RX_IRQ_EN    BIT(1)
#define AUX_MU_IER_REG_TX_IRQ_EN    BIT(0)

#define AUX_MU_CNTL_REG_TX_EN       BIT(1)
#define AUX_MU_CNTL_REG_RX_EN       BIT(0)

#define AUX_MU_TXD_PIN 14
#define AUX_MU_RXD_PIN 15

static void mini_uart_write(uint8_t const* p, uint32_t count);

void mini_uart_init(uart_cfg_t* cfg, uart_funcs_t* funcs)
{
    // Enable the mini UART w/o clobbering existing setup
    uint32_t auxEnables = read32(AUX_ENABLES);
    dmb();
    if((auxEnables & AUX_ENABLE_MINI_UART) != AUX_ENABLE_MINI_UART)
    {
        write32(AUX_ENABLES, auxEnables | AUX_ENABLE_MINI_UART);
    }
    
    // Enable IRQ's for RX and TX FIFO empty
    write32(AUX_MU_IER_REG, AUX_MU_IER_REG_RX_IRQ_EN | AUX_MU_IER_REG_TX_IRQ_EN);
    write32(AUX_MU_CNTL_REG, 0);                // Disable everything
    write32(AUX_MU_LCR_REG, cfg->bits & 0x03);
    write32(AUX_MU_MCR_REG, 0);                 // Make RTS High

    write32(AUX_MU_IIR_REG, AUX_MU_IIR_REG_TX_FIFO_EN  |    // Enable FIFOs
                            AUX_MU_IIR_REG_RX_FIFO_EN  |
                            AUX_MU_IIR_REG_TX_FIFO_RST |    // Reset FIFOs
                            AUX_MU_IIR_REG_RX_FIFO_RST |
                            AUX_MU_IIR_REG_IRQ_PEND_RST);   // Clear any pending interrupts
    write32(AUX_MU_BAUD_REG, cfg->baud & 0xFFFF);

    // Set GPIO pins 14 and 15 to alternate function 5 (TXD1, RXD1)
    gpio_set_func(AUX_MU_TXD_PIN, GPIO_ALT_FUNC_5);
    gpio_set_func(AUX_MU_RXD_PIN, GPIO_ALT_FUNC_5);

    // Disable pull up/down for pins 14 & 15, write 0 to GPPUD
    write32(GPPUD, GPPUD_OFF);
    spin(150);
    
    // Assert clock on pins 14 and 15
    write32(GPPUDCLK0, (1 << AUX_MU_TXD_PIN));
    write32(GPPUDCLK0, (1 << AUX_MU_RXD_PIN));
    spin(150);
    
    // Write 0 to GPPUD (not necessary here, already 0)
    // Write 0 to GPPUDCLKn to take effect
    write32(GPPUDCLK0, 0);

    write32(AUX_MU_CNTL_REG, AUX_MU_CNTL_REG_TX_EN |
                             AUX_MU_CNTL_REG_RX_EN);    // Enable RX & TX

    funcs->write = mini_uart_write;
}

static void mini_uart_write(uint8_t const* p, uint32_t count)
{
    uint8_t const* e = &p[count];
    while(e != p)
    {
        while(!(read32(AUX_MU_LSR_REG) & BIT(5)))
            continue;
        write32(AUX_MU_IO_REG, *p);
        ++p;
    }
}

