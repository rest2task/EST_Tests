/*===========================================================================+
|  Author   : chenhao	                                                     |
|  Version  : V1.00                                                          |
|  Creation : 09/30/2014                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include "typedef.h"
#include "types.h"
#include "config.h"

#define TIMER_10US_COUNT                (0xF0u)
#define TIMER_1MS_COUNT                 (240000)
#define TIMER_OVERFLOW                  (0xFFFFFFFFu)


/******************** added by gonggw, 2015.01.20 ********************/
volatile u64 jiffies_64;
volatile u32 jiffies;

u64 timestamp;
void timer_irq_en(void *TIMx, u32 us_val)
{
    static int i=0;
    i++;
}
void sys_time_init()
{
    static int i=0;
    i++;
}
void timer_init_spec(void *TIMx, u32 irq_en, u32 us_val)
{
    static int i=0;
    i++;
}
#ifndef SOC_DMTIMER_4_REGS
#define SOC_DMTIMER_4_REGS                   (0x48044000)
#endif
ulong read_timer()
{
    static int i=0;
    i++;
    return i;
}
unsigned long long get_ticks(void)
{
         return jiffies_64;
}
/*
timer_adjust: adjust timer counter for Synchronization with EtherCAT Slave
rate: negative for faster positive for slower; abs(rate), 1 for 10%,
*/
void timer_adjust(int rate)
{
    static int i=0;
    i++;
}










