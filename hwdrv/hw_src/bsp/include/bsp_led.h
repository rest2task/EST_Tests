#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "fsl_common.h"

#define CS0_GPIO     GPIO3
#define CS0_GPIO_PIN    27
#define CS0_GPIO_IOMUXC   IOMUXC_GPIO_EMC_41_GPIO3_IO27

#define RUN_LED_GPIO             GPIO2
#define RUN_LED_GPIO_PIN         (16U)
#define RUN_LED_IOMUXC           IOMUXC_GPIO_AD_B1_00_GPIO1_IO16

#define UART_LED_GPIO             GPIO2
#define UART_LED_GPIO_PIN         (17U)
#define UART_LED_IOMUXC           IOMUXC_GPIO_AD_B1_01_GPIO1_IO17

#define CANA_LED_GPIO             GPIO1
#define CANA_LED_GPIO_PIN         (24U)
#define CANA_LED_IOMUXC           IOMUXC_GPIO_AD_B1_08_GPIO1_IO24

#define CANB_LED_GPIO             GPIO1
#define CANB_LED_GPIO_PIN         (25U)
#define CANB_LED_IOMUXC           IOMUXC_GPIO_AD_B1_09_GPIO1_IO25

#define RS485_LED_GPIO             GPIO1
#define RS485_LED_GPIO_PIN         (28U)
#define RS485_LED_IOMUXC           IOMUXC_GPIO_AD_B1_12_GPIO1_IO28

#define CS_RS485_GPIO             GPIO1
#define CS_RS485_GPIO_PIN         (0U)
#define CS_RS485_IOMUXC           IOMUXC_GPIO_AD_B0_00_GPIO1_IO00

#define Free_Test_GPIO             GPIO1
#define Free_Test_GPIO_PIN         (26U)
#define Free_Test_IOMUXC           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26

#define CoreBard_Ver1_GPIO	     	GPIO2
#define CoreBard_Ver1_GPIO_PIN		(3u)
#define CoreBard_Ver1_IOMUXC		IOMUXC_GPIO_B0_03_GPIO2_IO03

#define CoreBard_Ver0_GPIO	     	GPIO2
#define CoreBard_Ver0_GPIO_PIN		(2u)
#define CoreBard_Ver0_IOMUXC		IOMUXC_GPIO_B0_02_GPIO2_IO02

/*********************************************************
 * LED GPIO端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
#define CORE_BOARD_LED_GPIO             GPIO2
#define CORE_BOARD_LED_GPIO_PIN         (16U)
#define CORE_BOARD_LED_IOMUXC           IOMUXC_GPIO_AD_B1_00_GPIO1_IO16

#define RGB_RED_LED_GPIO                GPIO1
#define RGB_RED_LED_GPIO_PIN            (24U)
#define RGB_RED_LED_IOMUXC              IOMUXC_GPIO_AD_B1_08_GPIO1_IO24

#define RGB_GREEN_LED_GPIO              GPIO1
#define RGB_GREEN_LED_GPIO_PIN          (25U)
#define RGB_GREEN_LED_IOMUXC            IOMUXC_GPIO_AD_B1_09_GPIO1_IO25

#define RGB_BLUE_LED_GPIO               GPIO1
#define RGB_BLUE_LED_GPIO_PIN           (10U)
#define RGB_BLUE_LED_IOMUXC             IOMUXC_GPIO_AD_B0_10_GPIO1_IO10

#define LED_DELAY_COUNT                 70000000

/** 控制LED灯亮灭的宏，
  * LED低电平亮，设置ON=0，OFF=1
  * 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
 */
#define ON  1
#define OFF 0

/* 使用标准的固件库控制IO 
   使用范例：CORE_BOARD_LED(ON); */
   
#define   CORE_BOARD_LED(a) if (a)  \
          GPIO_PinWrite(CORE_BOARD_LED_GPIO, CORE_BOARD_LED_GPIO_PIN, 0U);\
          else    \
          GPIO_PinWrite(CORE_BOARD_LED_GPIO, CORE_BOARD_LED_GPIO_PIN, 1U);

#define   RGB_RED_LED(a)  if (a)  \
          GPIO_PinWrite(RGB_RED_LED_GPIO, RGB_RED_LED_GPIO_PIN, 0U);\
          else    \
          GPIO_PinWrite(RGB_RED_LED_GPIO, RGB_RED_LED_GPIO_PIN, 1U);

#define   RGB_GREEN_LED(a)  if (a)  \
          GPIO_PinWrite(RGB_RGB_GREEN_LED_GPIO, RGB_RGB_GREEN_LED_GPIO_PIN, 0U);\
          else    \
          GPIO_PinWrite(RGB_GREEN_LED_GPIO, RGB_GREEN_LED_GPIO_PIN, 1U);

#define   RGB_BLUE_LED(a) if (a)  \
          GPIO_PinWrite(RGB_BLUE_LED_GPIO, RGB_BLUE_LED_GPIO_PIN, 0U);\
          else    \
          GPIO_PinWrite(RGB_BLUE_LED_GPIO, RGB_BLUE_LED_GPIO_PIN, 1U);     


/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)      {p->DR |= (1U << i);}   //输出为高电平    
#define digitalLo(p,i)      {p->DR &= ~(1U << i);}  //输出低电平
#define digitalToggle(p,i)  {p->DR ^= (1U<<i);}     //输出反转状态

#define CS0_HIGH    digitalHi(CS0_GPIO,CS0_GPIO_PIN)
/* 定义控制IO的宏 */
#define RUN_LED_TOGGLE       digitalToggle(RUN_LED_GPIO,RUN_LED_GPIO_PIN)
#define RUN_LED_OFF          digitalHi(RUN_LED_GPIO,RUN_LED_GPIO_PIN)
#define RUN_LED_ON           digitalLo(RUN_LED_GPIO,RUN_LED_GPIO_PIN)

#define URAT_LED_TOGGLE       digitalToggle(UART_LED_GPIO,UART_LED_GPIO_PIN)
#define UART_LED_OFF          digitalHi(UART_LED_GPIO,UART_LED_GPIO_PIN)
#define UART_LED_ON           digitalLo(UART_LED_GPIO,UART_LED_GPIO_PIN)

#define CANA_LED_TOGGLE       digitalToggle(CANA_LED_GPIO,CANA_LED_GPIO_PIN)
#define CANA_LED_OFF          digitalHi(CANA_LED_GPIO,CANA_LED_GPIO_PIN)
#define CANA_LED_ON           digitalLo(CANA_LED_GPIO,CANA_LED_GPIO_PIN)

#define CANB_LED_TOGGLE       digitalToggle(CANB_LED_GPIO,CANB_LED_GPIO_PIN)
#define CANB_LED_OFF          digitalHi(CANB_LED_GPIO,CANB_LED_GPIO_PIN)
#define CANB_LED_ON           digitalLo(CANB_LED_GPIO,CANB_LED_GPIO_PIN)

#define RS485_LED_TOGGLE       digitalToggle(RS485_LED_GPIO,RS485_LED_GPIO_PIN)
#define RS485_LED_OFF          digitalHi(RS485_LED_GPIO,RS485_LED_GPIO_PIN)
#define RS485_LED_ON           digitalLo(RS485_LED_GPIO,RS485_LED_GPIO_PIN)

#define CS_RS485_TOGGLE       digitalToggle(CS_RS485_GPIO,CS_RS485_GPIO_PIN)
#define CS_RS485_TXD          digitalHi(CS_RS485_GPIO,CS_RS485_GPIO_PIN)
#define CS_RS485_RXD          digitalLo(CS_RS485_GPIO,CS_RS485_GPIO_PIN)

#define Free_Test_TOGGLE       digitalToggle(Free_Test_GPIO,Free_Test_GPIO_PIN)
#define Free_Test_High         digitalHi(Free_Test_GPIO,Free_Test_GPIO_PIN)
#define Free_Test_Low          digitalLo(Free_Test_GPIO,Free_Test_GPIO_PIN)

/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

//红
#define RGB_LED_COLOR_RED  \
          RGB_RED_LED_ON;\
          RGB_GREEN_LED_OFF;\
          RGB_BLUE_LED_OFF

//绿
#define RGB_LED_COLOR_GREEN   \
          RGB_RED_LED_OFF;\
          RGB_GREEN_LED_ON;\
          RGB_BLUE_LED_OFF

//蓝
#define RGB_LED_COLOR_BLUE  \
          RGB_RED_LED_OFF;\
          RGB_GREEN_LED_OFF;\
          RGB_BLUE_LED_ON
          
//黄(红+绿)          
#define RGB_LED_COLOR_YELLOW  \
          RGB_RED_LED_ON;\
          RGB_GREEN_LED_ON;\
          RGB_BLUE_LED_OFF
          
//紫(红+蓝)
#define RGB_LED_COLOR_PURPLE  \
          RGB_RED_LED_ON;\
          RGB_GREEN_LED_OFF;\
          RGB_BLUE_LED_ON

//青(绿+蓝)
#define RGB_LED_COLOR_CYAN \
          RGB_RED_LED_OFF;\
          RGB_GREEN_LED_ON;\
          RGB_BLUE_LED_ON
          
//白(红+绿+蓝)
#define RGB_LED_COLOR_WHITE \
          RGB_RED_LED_ON;\
          RGB_GREEN_LED_ON;\
          RGB_BLUE_LED_ON
          
//黑(全部关闭)
#define RGB_LED_COLOR_OFF \
          RGB_RED_LED_OFF;\
          RGB_GREEN_LED_OFF;\
          RGB_BLUE_LED_OFF  

/*******************************************************************************
 * 函数声明
 ******************************************************************************/
void LED_GPIO_Config(void);

#endif /* __BSP_LED_H */
