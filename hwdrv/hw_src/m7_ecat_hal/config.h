/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    config.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2015.01.20

Description:    LPC435X modules config parameters

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifndef __ASSEMBLY__


#endif
#define STACK_POINTER			0x10001800
#ifdef DYN_MEM_ALLOC
#define PLL1_120MHz
#else
//PLL1 parameters definition
//#define PLL1_72MHz
//#define PLL1_120MHz
//#define PLL1_132MHz
//#define PLL1_156MHz
#define PLL1_204MHz
#endif

/******************** CLOCK CONFIG ***********************/
#define RTC_CLK					(32768UL)		/* RTC oscillator frequency 			*/
#define IRC_OSC					(12000000UL)    /* Internal RC oscillator frequency 		*/
#define IRC_TRIM_VAL			0x350 			/* IRC trim value for 12MHz output 		*/
#define XTAL_FREQ				(12000000UL)	/* Frequency of external xtal 			*/
#define USE_XTAL				1				/* Select clock source 1=XTAL, 0=IRC 	*/
#if defined(PLL1_72MHz)
#define BASE_M4_CLK_FREQ		(72000000)	/* Target BASE_M4_CLK Frequency		*/
#elif defined(PLL1_120MHz)
#define BASE_M4_CLK_FREQ		(120000000)	/* Target BASE_M4_CLK Frequency		*/
#elif defined(PLL1_132MHz)
#define BASE_M4_CLK_FREQ		(132000000)	/* Target BASE_M4_CLK Frequency		*/
#elif defined(PLL1_156MHz)
#define BASE_M4_CLK_FREQ		(156000000)	/* Target BASE_M4_CLK Frequency		*/
#elif defined(PLL1_204MHz)
#define BASE_M4_CLK_FREQ		(204000000)	/* Target BASE_M4_CLK Frequency		*/
#endif
/********************************************************/

#define INTERNAL_FLASH

#if (BASE_M4_CLK_FREQ < 21000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x0
#elif (BASE_M4_CLK_FREQ < 43000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x1
#elif (BASE_M4_CLK_FREQ < 64000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x2
#elif (BASE_M4_CLK_FREQ < 86000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x3
#elif (BASE_M4_CLK_FREQ < 107000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x4
#elif (BASE_M4_CLK_FREQ < 129000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x5
#elif (BASE_M4_CLK_FREQ <= 150000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x6
#elif (BASE_M4_CLK_FREQ <= 172000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x7
#elif (BASE_M4_CLK_FREQ <= 193000000UL)
#define	FLASH_ACCELERATOR_SPEED	0x8
#else
#define	FLASH_ACCELERATOR_SPEED	0x9
#endif
//Options for DEBUG
//configure DRAM and SYstem control registers in lowlevel_init.S
//#define CONFIG_DRAM_CFG

/******************** SYSTEM MEMORY CONFIG ***************/
#define CONFIG_SYS_IRAM_BASE	0x10000000                                      /* Internal SRAM base address */
#define CONFIG_SYS_IRAM_SIZE	0x8000                                          /* 32 KB of internal SRAM memory */
#define CONFIG_SYS_IRAM_END		(CONFIG_SYS_IRAM_BASE + CONFIG_SYS_IRAM_SIZE)
#define CONFIG_SYS_DRAM_BASE	SDRAM_ADDR_BASE                                 /* SDRAM(DYNC0) Start address */
#define CONFIG_SYS_DRAM_SIZE	(1 << 25)                                       /* Total DDR SDRAM is 32M bytes in mini6410 */
#define CONFIG_SYS_DRAM_END		(CONFIG_SYS_DRAM_BASE+CONFIG_SYS_DRAM_SIZE)     /* Total DDR SDRAM is 32M bytes in mini6410 */
//Memory Allocation
//options for Memory Management
/* SDRAM Address Base for DYCS0*/
#define SDRAM_ADDR_BASE			0x28000000
#define APP_BASE				CONFIG_SYS_DRAM_BASE                            //start address of Application code run space
#define APP_STACK				(SDRAM_ADDR_BASE + 0x1A00000)                   //Stack pointer
#define CONFIG_MM_START_ADDR	APP_STACK
#define CONFIG_MM_LENGTH		0x100000                                        //reserve 1MB mmeory for dynamic memory allocation
#define CONFIG_MM_MASK			(CONFIG_MM_LENGTH - 1)                          //for address check
//Config area allocation
#define FAC_CFG_SIZE			(16 * 1024)                             		//Factory Configuration data size
#define USR_CFG_SIZE			(16 * 1024)                             		//User Configuration data size
#define APP_INFO_SIZE			(2	* 1024)
#define MACHINE_DATA_BASE		(CONFIG_MM_START_ADDR + CONFIG_MM_LENGTH)       //Machne Parameter Base address
#define MACHINE_DATA_BAK_BASE	(MACHINE_DATA_BASE + FAC_CFG_SIZE + USR_CFG_SIZE) //Machne Parameter Base address
#define MACHINE_DATA_LENGTH		0x100000                                        //Machne Parameter size in byte
//used for LPC4357 EMAC DESC
#if 0 //DRAM
#define EMAC_DESC_BASE			(MACHINE_DATA_BASE + MACHINE_DATA_LENGTH)
#define EMAC_DESC_LENGTH		0x100000                                        //Machne Parameter size in byte
#else //Internal SRAM
#define EMAC_DESC_BASE			0x2000F380
#define EMAC_DESC_LENGTH		0xC80                                        //Machne Parameter size in byte
#endif
/********************************************************/

/******************** UART_PRINTF CONFIG *****************/
#define CONFIG_SYS_PBSIZE		384		/* Print Buffer Size */
/********************************************************/

/******************** UART CONFIG ************************/
/*
#define DEBUG_UART				(LPC_UART1)
#define DEBUG_UART_NUM			1
#define DEBUG_UART_BR			115200

#define PANEL_UART				(LPC_USART3)
#define PANEL_UART_NUM			3
#define PANEL_UART_BR			19200

#define RS485_UART				(LPC_USART0)
#define RS485_UART_NUM			0
#define RS485_UART_BR			115200
*/
/********************************************************/

/******************** TIMER CONFIG ***********************/
#define CONFIG_SYS_HZ			100000 //Timer cycle, 10us
#define TICK_TIME				(1000000/CONFIG_SYS_HZ)
#define SYSTEM_TIMER			(LPC_TIMER0)
#define DEBUG_TIMER				(LPC_TIMER2)
#define CPLD_PWM_TIMER			(LPC_TIMER3)
#ifdef BOOT_BL1
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_IRAM_END-0x8)
#else
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_DRAM_END-0x8)
#endif
/********************************************************/

/******************** TIMER CONFIG ***********************/

/********************************************************/

/******************** CAN CONFIG ***********************/
#define	PANEL_CAN				0 //CAN module selection, 0 or 1
/********************************************************/
/******************** I2C CONFIG ***********************/
#define	MAIN_I2C				0//I2C module selection, 0 or 1
#define CONFIG_MAX_I2C_NUM		2
#define CONFIG_SYS_I2C_SPEED	100000  /*  100 kbps  */
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_SYS_I2C_SLAVE	0x0
/********************************************************/

#define CONFIG_USE_LIB_MEMCPY //gonggw, 2013.10.15, use memcpy in standard library; use my codes else

/********************** GPIO CONFIG **********************/

/********************************************************/

/********************** EXTENDED CONFIG *****************/
//ÔÝÊ±È¥µô
 #define EXT_ERULER_CAP				//E-Ruler Capturing
/********************************************************/


#endif

