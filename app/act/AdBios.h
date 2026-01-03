#ifndef AD_H
#define AD_H

#include "common.h"
#include "hwcfg.h"


/*** API ***/
WORD GetADHW(int portno); 


/*** replacement define ***/
                                 

/*** constant define ***/
#define AD_TIMEOUT		1000 	
#define AD_ERROR_VALUE  0
#define AD_REPORT_ERROR(n)	{	}

#define AD_REGLO	portadlow	 		//低字节
//#define AD_REGMI	portadMiddle			//中字节
#define AD_REGHI	portadhigh			//高字节


#endif
