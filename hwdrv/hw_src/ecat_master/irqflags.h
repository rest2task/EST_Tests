/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    irqflags.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.24

Description:     IRQ OP MACROs

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/


#ifndef __ASM_ARM_IRQFLAGS_H
#define __ASM_ARM_IRQFLAGS_H

/*
 * CPU interrupt mask handling.
 */
 #if 0 //gonggw, 2015.01.20, OK for GCC
#define local_irq_save_hw_notrace(x)					\
	({							\
	__asm__ __volatile__(					\
	"mrs	%0, cpsr		@ local_irq_save_hw\n"	\
	"cpsid	i"						\
	: "=r" (x) : : "memory", "cc");				\
	})
#define local_irq_enable_hw_notrace()  __asm__("cpsie i	@ __sti" : : : "memory", "cc")
#define local_irq_disable_hw_notrace() __asm__("cpsid i	@ __cli" : : : "memory", "cc")
#define local_fiq_enable_hw_notrace()  __asm__("cpsie f	@ __stf" : : : "memory", "cc")
#define local_fiq_disable_hw_notrace() __asm__("cpsid f	@ __clf" : : : "memory", "cc")
/*
* Save the current interrupt enable state.
*/
#define local_save_flags_hw(x)					\
	({							\
	__asm__ __volatile__(					\
	"mrs	%0, cpsr		@ local_save_flags_hw"	\
	: "=r" (x) : : "memory", "cc");				\
	})

/*
* restore saved IRQ & FIQ state
*/
#define local_irq_restore_hw_notrace(x)				\
	__asm__ __volatile__(					\
	"msr	cpsr_c, %0		@ local_irq_restore_hw\n"	\
	:							\
	: "r" (x)						\
	: "memory", "cc")


#else //Keil ARM format
#define local_irq_save_hw_notrace(x)	\
		do								\
		{								\
			__asm						\
			{							\
				mrs x, cpsr				\
				cpsid i					\
			}							\
		}while(0)
#define local_irq_enable_hw_notrace()	\
		do								\
		{								\
			__asm						\
			{							\
				cpsie i					\
			}							\
		}while(0)
		
#define local_irq_disable_hw_notrace()	\
		do								\
		{								\
			__asm						\
			{							\
				cpsid i					\
			}							\
		}while(0)
		
#define local_fiq_enable_hw_notrace()	\
		do								\
		{								\
			__asm						\
			{							\
				cpsie f					\
			}							\
		}while(0)
		
#define local_fiq_disable_hw_notrace()	\
		do								\
		{								\
			__asm						\
			{							\
				cpsid f					\
			}							\
		}while(0)

#define local_save_flags_hw(x)			\
		do								\
		{								\
			__asm						\
			{							\
				mrs x, cpsr				\
			}							\
		}while(0)

/*
* restore saved IRQ & FIQ state
*/
#define local_irq_restore_hw_notrace(x)	\
		do								\
		{								\
			__asm						\
			{							\
				msr cpsr_c, x			\
			}							\
		}while(0)

#endif
		
#define local_irq_save_hw(flags)	local_irq_save_hw_notrace(flags)
#define local_irq_enable_hw()		local_irq_enable_hw_notrace()
#define local_irq_disable_hw()		local_irq_disable_hw_notrace()
#define local_fiq_enable_hw()		local_fiq_enable_hw_notrace()
#define local_fiq_disable_hw()		local_fiq_disable_hw_notrace()
#define local_irq_restore_hw(flags)	local_irq_restore_hw_notrace(flags)

/**
 * enum irqreturn
 * @IRQ_NONE		interrupt was not from this device
 * @IRQ_HANDLED		interrupt was handled by this device
 * @IRQ_WAKE_THREAD	handler requests to wake the handler thread
 */
enum irqreturn {
	IRQ_NONE		= (0 << 0),
	IRQ_HANDLED		= (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};

typedef enum irqreturn irqreturn_t;
#define IRQ_RETVAL(x)	((x) != IRQ_NONE)

#endif

