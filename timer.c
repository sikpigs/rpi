#include "timer.h"

#define ARM_TIMER_BASE 0x2000B400

arm_timer_t* get_arm_timer(void)
{
    return (arm_timer_t*)(ARM_TIMER_BASE);
}
