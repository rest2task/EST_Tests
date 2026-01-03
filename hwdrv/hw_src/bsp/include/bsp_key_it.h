#ifndef __BSP_KEY_IT_H
#define __BSP_KEY_IT_H

#include "fsl_common.h"
#include "types.h"

/******************************************************************
 * 宏
 ******************************************************************/
//AD_OK
#define AD_OK_GPIO          GPIO1
#define AD_OK_GPIO_PIN      (1U)
#define AD_OK_IOMUXC        IOMUXC_GPIO_AD_B0_01_GPIO1_IO01
#define AD_OK_NAME          "AD_OK"
#define AD_OK_ID            0
//中断相关，IRQ中断号及IRQHandler中断服务函数
#define AD_OK_IRQ           GPIO1_Combined_0_15_IRQn
#define AD_OK_IRQHandler    GPIO1_Combined_0_15_IRQHandler 

//EXT_AD_OK
#define EXT_AD_OK_GPIO          GPIO3
#define EXT_AD_OK_GPIO_PIN      (17U)
#define EXT_AD_OK_IOMUXC        IOMUXC_GPIO_SD_B0_05_GPIO3_IO17
#define EXT_AD_OK_NAME          "EXT_AD_OK"
#define EXT_AD_OK_ID            0
//中断相关，IRQ中断号及IRQHandler中断服务函数
#define EXT_AD_OK_IRQ           GPIO3_Combined_16_31_IRQn
#define EXT_AD_OK_IRQHandler    GPIO3_Combined_16_31_IRQHandler 

//SEMC_A8
#define SEMC_A8_GPIO          GPIO4
#define SEMC_A8_GPIO_PIN      (17U)
#define SEMC_A8_IOMUXC        IOMUXC_GPIO_EMC_17_GPIO4_IO17
#define SEMC_A8_NAME          "SEMC_A8"

//D7-CS1
#define SEMC_CSX01_GPIO          GPIO2
#define SEMC_CSX01_GPIO_PIN      (0U)
#define SEMC_CSX01_IOMUXC        IOMUXC_GPIO_B0_00_GPIO2_IO00
#define SEMC_CSX01_NAME          "SEMC_CSX01"
 
//MANUAL_KEY
#define MANUAL_KEY_GPIO          GPIO1
#define MANUAL_KEY_GPIO_PIN      (14U)
#define MANUAL_KEY_IOMUXC        IOMUXC_GPIO_AD_B0_14_GPIO1_IO14
#define MANUAL_KEY_NAME          "MANUAL_KEY"

//RESET_KEY
#define RESET_KEY_GPIO          GPIO1
#define RESET_KEY_GPIO_PIN      (15U)
#define RESET_KEY_IOMUXC        IOMUXC_GPIO_AD_B0_15_GPIO1_IO15
#define RESET_KEY_NAME          "RESET_KEY"

//DOWNLOAD_KEY
#define DOWNLOAD_KEY_GPIO       		GPIO1
#define DOWNLOAD_KEY_GPIO_PIN      	(17U)
#define DOWNLOAD_KEY_IOMUXC        	IOMUXC_GPIO_AD_B1_01_GPIO1_IO17
#define DOWNLOAD_KEY_NAME          	"DOWNLOAD_KEY"

//INRST_KEY
#define INRST_KEY_GPIO          GPIO1
#define INRST_KEY_GPIO_PIN      (29U)
#define INRST_KEY_IOMUXC        IOMUXC_GPIO_AD_B1_13_GPIO1_IO29
#define INRST_KEY_NAME          "INSET_KEY"

//DM9000
#define DM9000_INT_GPIO          GPIO1
#define DM9000_INT_GPIO_PIN      (19U)
#define DM9000_INT_IOMUXC        IOMUXC_GPIO_AD_B1_03_GPIO1_IO19
#define DM9000_INT_NAME          "DM9000_INT"

#define DM9000_IRQ          GPIO1_Combined_16_31_IRQn
#define DM9000_IRQHandler   GPIO1_Combined_16_31_IRQHandler


//WAUP按键
#define CORE_BOARD_WAUP_KEY_GPIO          GPIO5
#define CORE_BOARD_WAUP_KEY_GPIO_PIN      (0U)
#define CORE_BOARD_WAUP_KEY_IOMUXC        IOMUXC_SNVS_WAKEUP_GPIO5_IO00
#define CORE_BOARD_WAUP_KEY_NAME          "CORE_BORE_WAUP_KEY"
#define CORE_BOARD_WAUP_KEY_ID            0


//MODE按键
#define CORE_BOARD_MODE_KEY_GPIO          GPIO1
#define CORE_BOARD_MODE_KEY_GPIO_PIN      (5U)
#define CORE_BOARD_MODE_KEY_IOMUXC        IOMUXC_GPIO_AD_B0_05_GPIO1_IO05
#define CORE_BOARD_MODE_KEY_NAME          "CORE_BORE_MODE_KEY"
#define CORE_BOARD_MODE_KEY_ID            1
//中断相关，IRQ中断号及IRQHandler中断服务函数
#define CORE_BOARD_MODE_KEY_IRQ           GPIO1_Combined_0_15_IRQn
#define CORE_BOARD_MODE_KEY_IRQHandler    GPIO1_Combined_0_15_IRQHandler

/******************************************************************
 * 声明
 ******************************************************************/
 
               

/* 按键是否被按下的中断标志 */
extern __IO bool g_KeyDown[2];

typedef struct tag_IOVTEST //IO检测 2014-3-3
 {
   BYTE 	   ioovertime;		   //IO输出过载计时
   BOOL 	   iooverflag;		   //IO输出过载标志
   BYTE 	   iooverstep;		   //IO保护检测步骤
   WORD 	   iooverdelaytime;    //IO输出保护延迟时间
   BYTE 	   hvlowtime;		   //IO电源电压异常计时
   BOOL 	   hvlowflag;		   //IO电源电压异常标志
   WORD 		 checktime; 		 //IO电源电压异常标志
   WORD 		 manualflag;		 //IO电源电压异常标志
 } IOVTEST;   

void Key_IT_GPIO_Config(void);


   

#endif /* __BSP_KEY_IT_H */




