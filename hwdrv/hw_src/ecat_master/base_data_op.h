/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    base_data_op.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.23

Description:    bit&byte operation definition and functions

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef _BASE_DATA_OP_H_
#define _BASE_DATA_OP_H_
#include "types.h"
#include <stdlib.h>

#define BITS_PER_LONG 32
#define BIT(nr)				(1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITS_PER_BYTE		8
#define BITS_TO_LONGS(nr)	((nr + BITS_PER_BYTE * sizeof(long) -1) /(BITS_PER_BYTE * sizeof(long))
#define swap16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))
#if 0
#define swap32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffU) << 24) |			\
	(((__u32)(x) & (__u32)0x0000ff00U) << 8) |			\
	(((__u32)(x) & (__u32)0x00ff0000U) >> 8) |			\
	(((__u32)(x) & (__u16)0xff000000U) >> 24)))

#else
#define swap32(x) (x)	
#endif
#define le16_to_cpu(x) x
#define le32_to_cpu(x) x
#define le64_to_cpu(x) x

#define cpu_to_le16(x) x
#define cpu_to_le32(x) x
#define cpu_to_le64(x) x

#define __be16_to_cpu(x) swap16((__u16)(x))
#define __cpu_to_be16(x) ((__u16)swap16((x)))
#define __cpu_to_le32(x) ((__u32)(x))
#define __le32_to_cpu(x) ((__u32)(x))

#define le16_to_cpup(x) le16_to_cpu(*((uint16_t *)(x)))
#define le32_to_cpup(x) le32_to_cpu(*((uint32_t *)(x)))
#define le64_to_cpup(x) le64_to_cpu(*((uint64_t *)(x)))

#define ntohs(x) __be16_to_cpu(x)
#define htons(x) __cpu_to_be16(x)
static inline int test_bit(int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}
static inline void clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p &= ~mask;
}
static inline void set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p |= mask;
}

static inline int test_and_set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old;

	old = *p;
	*p = old | mask;

	return (old & mask) != 0;
}
static inline int test_and_clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old;
	old = *p;
	*p = old & ~mask;
	return (old & mask) != 0;
}
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

//register access
#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))
	
#define __arch_putb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))

#define __raw_writeb(v,a)		__arch_putb(v,a)
#define __raw_writew(v,a)		__arch_putw(v,a)
#define __raw_writel(v,a)		__arch_putl(v,a)
	
#define __raw_readb(a)			__arch_getb(a)
#define __raw_readw(a)			__arch_getw(a)
#define __raw_readl(a)			__arch_getl(a)

#define ether_crc_le(length, data) crc32_le(~0, data, length)
//number of data per L1 Cache line
#define L1_CACHE_BYTES		32
#define SKB_DATA_ALIGN(X)	(((X) + (L1_CACHE_BYTES - 1)) & \
				 ~(L1_CACHE_BYTES - 1))

#define UData(Data)	((unsigned long) (Data))
#define FSize(Field)	((Field) >> 16)
#define FShft(Field)	((Field) & 0x0000FFFF)
#define FMsk(Field)	(((UData (1) << FSize (Field)) - 1) << FShft (Field))
#define FClrBit(Data, Bit)	(Data = (Data & ~(Bit)))
#define FClrFld(Data, Field)	(Data = (Data & ~FMsk(Field)))
#define Fld(Size, Shft)	(((Size) << 16) + (Shft))

#define FInsrt(Value, Field) \
			(UData (Value) << FShft (Field))

u32 __div64_32(u64 *n, u32 base);
u32 do_div(u64 *n, u32 base);  //no gnu


//no gnu

#if 0
#define do_div(n,base) ({				\
	u32 __base = (base);			\
	u32 __rem;					\
	(void)(((typeof((n)) *)0) == ((u64 *)0));	\
	if (((n) >> 32) == 0) {			\
		__rem = (u32)(n) % __base;		\
		(n) = (u32)(n) / __base;		\
	} else						\
		__rem = __div64_32(&(n), __base);	\
	__rem;						\
 })
#endif
	/*
	 * min()/max()/clamp() macros that also do
	 * strict type-checking.. See the
	 * "unnecessary" pointer comparison.
	 */
 #if 0   //chenhao
#define min(x, y) ({				\
		typeof(x) _min1 = (x);			\
		typeof(y) _min2 = (y);			\
		(void) (&_min1 == &_min2);		\
		_min1 < _min2 ? _min1 : _min2; })
	
#define max(x, y) ({				\
		typeof(x) _max1 = (x);			\
		typeof(y) _max2 = (y);			\
		(void) (&_max1 == &_max2);		\
		_max1 > _max2 ? _max1 : _max2; })
#endif
		#define max(x,y) ((x) > (y) ? (x) : (y))
		#define min(x,y) ((x) > (y) ? (y) : (x))
		
		
static inline unsigned long
simple_strtoul(const char *nptr, char **endptr, int base)
{
	return strtoul(nptr, endptr, base);
}

/*
 * On ARMv5 and above those functions can be implemented around
 * the clz instruction for much better code efficiency.
 */

#if 0  //add by chenhao
#if 0 //gonggw,2015.01.12, Inline ASM in S3C6410 with GCC
static inline int fls(int x)
{
	int ret;

	asm("clz\t%0, %1" : "=r" (ret) : "r" (x));
       	ret = 32 - ret;
	return ret;
}
#else
static inline int fls(int x)
{
	int ret;

	__asm
	{
	    clz ret, x
	}
       	ret = 32 - ret;
	return ret;
}
    
#endif
#define __fls(x) (fls(x) - 1)
#define ffs(x) ({ unsigned long __t = (x); fls(__t & -__t); })
#define __ffs(x) (ffs(x) - 1)
#define ffz(x) __ffs( ~(x) )
#endif

#define min_t(type, x, y) ({			\
	type __min1 = (x);			\
	type __min2 = (y);			\
	__min1 < __min2 ? __min1: __min2; })

#define max_t(type, x, y) ({			\
	type __max1 = (x);			\
	type __max2 = (y);			\
	__max1 > __max2 ? __max1: __max2; })

#endif

