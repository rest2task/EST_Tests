#ifndef _GPT_TIMER_H_
#define _GPT_TIMER_H_
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpt.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "Typedef.h"

extern int count;

typedef struct tag_CLOCK
{
		WORD 			 temp100us;
    WORD       temp1ms;
    WORD       temp10ms;
    WORD       temp100ms;	
    WORD       temp1s;  
} CLOCK_STRU; 

void GPT_Config(void); 
#endif
