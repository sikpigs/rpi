#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
#include "bits.h"

#define ARM_TIMER_CTRL_FREE_RUNNING_CNTR_EN (BIT(9))
#define ARM_TIMER_CTRL_HALT_CONT            (BIT(8))
#define ARM_TIMER_CTRL_TIMER_EN             (BIT(7))
#define ARM_TIMER_CTRL_IRQ_EN               (BIT(5))
#define ARM_TIMER_CTRL_PRE_SCALE(s)         ((s) << 2)
#define ARM_TIMER_CTRL_23BIT_CNTR           BIT(1)

#define ARM_TIMER_16BIT_CNTR     0
#define ARM_TIMER_23BIT_CNTR     1
#define ARM_TIMER_PRE_SCALE_OFF (0)
#define ARM_TIMER_PRE_SCALE_16  (1)
#define ARM_TIMER_PRE_SCALE_256 (2)

#define ARM_TIMER_ENABLE_TIMER  getarm_timer()->Ctrl.EnableTimer
#define ARM_TIMER_ENABLE_IRQ    getarm_timer()->Ctrl.EnableIRQ
#define ARM_TIMER_CLEAR         getarm_timer()->IRQClear
#define ARM_TIMER_TYPE          getarm_timer()->Ctrl.CntrType
#define ARM_TIMER_PRE_SCALE     getarm_timer()->Ctrl.Prescale
#define ARM_TIMER_LOAD          getarm_timer()->Load

typedef union 
{
    struct
    {
        volatile uint8_t                            : 1;
        volatile uint8_t cntr_type                  : 1;
        volatile uint8_t prescale                   : 2;
        volatile uint8_t                            : 1;
        volatile uint8_t enable_irq                 : 1;
        volatile uint8_t                            : 1;
        volatile uint8_t enable_timer               : 1;
        volatile uint8_t halt_continue              : 1;
        volatile uint8_t enable_free_running_cntr   : 1;
        volatile uint8_t                            : 6;
        volatile uint8_t prescaler                  : 8;
        volatile uint32_t                           : 8;
    };
    volatile uint32_t ctrl;
} arm_timer_ctrl_t;

typedef struct
{
    volatile uint32_t load;
    volatile uint32_t value;
    volatile arm_timer_ctrl_t ctrl;
    volatile uint32_t irq_clear;
    volatile uint32_t raw_irq;
    volatile uint32_t masked_irq;
    volatile uint32_t reload;
    volatile uint32_t predriver;
    volatile uint32_t free_running_cntr;
} arm_timer_t;

arm_timer_t* get_arm_timer(void);

#endif // __TIMER_H

