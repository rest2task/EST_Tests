/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    uart_printf.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2013.09.10

Description:    Dynamic memory allocation

Others:

Function List:  
                    uart_printf          :  same with printf, print data to UART
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "types.h"
#include "config.h"
#include "base_data_op.h"

#include "debug.h"

#define NUM_TYPE long long


#define uart_printf printf

u32 __div64_32(u64 *n, u32 base)
{
	u64 rem = *n;
	u64 b = base;
	u64 res, d = 1;
	u32 high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (u64) high << 32;
		rem -= (u64) (high*base) << 32;
	}

	while ((s64)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

u32 do_div ( u64 *n, u32 base)
{
	u32 __base = (base);	
	u32 __rem;	
	if (((*n) >> 32) == 0) {			
		__rem = (u32)(*n) % __base;		
		(*n) = (u32)(*n) / __base;		
	} else						
		__rem = __div64_32(n, __base);
	return __rem;		
}



