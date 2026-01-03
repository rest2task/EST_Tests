#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "fsl_common.h"
#include "gpt_timer.h"
void SysTick_Init(void);
void Delay_ms(__IO uint32_t nTime);


void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);

extern volatile CLOCK_STRU		  g_clock;

#endif /* __SYSTICK_H */

