
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "pad_config.h"  
#include "bsp_led.h"   

/*******************************************************************************
 * 宏
 ******************************************************************************/
/* 所有引脚均使用同样的PAD配置 */
#define LED_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   

    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 关闭
      拉/保持器选择: 保持器（上面已关闭，配置无效）
      上拉/下拉选择: 100K欧姆下拉（上面已关闭，配置无效）
      滞回器配置: 关闭 */ 

#define VERSION_PAD_CONFIG_DATA        (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   

    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 使能
      拉/保持器选择: 上下拉使能
      上拉/下拉选择: 100K欧姆下拉
      滞回器配置: 关闭 */    

/*******************************************************************************
 * 声明
 ******************************************************************************/
static void LED_IOMUXC_MUX_Config(void);
static void LED_IOMUXC_PAD_Config(void);
static void LED_GPIO_Mode_Config(void);

/**
* @brief  初始化LED相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void LED_IOMUXC_MUX_Config(void)
{
	IOMUXC_SetPinMux(RUN_LED_IOMUXC, 0U);
	IOMUXC_SetPinMux(UART_LED_IOMUXC, 0U);
	IOMUXC_SetPinMux(CANA_LED_IOMUXC, 0U);
	IOMUXC_SetPinMux(CANB_LED_IOMUXC, 0U);
	IOMUXC_SetPinMux(RS485_LED_IOMUXC, 0U);
	IOMUXC_SetPinMux(CS_RS485_IOMUXC, 0U);
	IOMUXC_SetPinMux(Free_Test_IOMUXC, 0U);
	IOMUXC_SetPinMux(CoreBard_Ver1_IOMUXC, 0U);
	IOMUXC_SetPinMux(CoreBard_Ver0_IOMUXC, 0U);
}

/**
* @brief  初始化LED相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void LED_IOMUXC_PAD_Config(void)
{
	IOMUXC_SetPinConfig(RUN_LED_IOMUXC, LED_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(UART_LED_IOMUXC, LED_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(CANA_LED_IOMUXC, LED_PAD_CONFIG_DATA);  
	IOMUXC_SetPinConfig(CANB_LED_IOMUXC, LED_PAD_CONFIG_DATA);  
	IOMUXC_SetPinConfig(RS485_LED_IOMUXC, LED_PAD_CONFIG_DATA);  
	IOMUXC_SetPinConfig(CS_RS485_IOMUXC, LED_PAD_CONFIG_DATA);  
	IOMUXC_SetPinConfig(Free_Test_IOMUXC, LED_PAD_CONFIG_DATA);  

    /***************************************************************************************
    网络VER0（MCU的管脚：GPIO_B0_02）和VER1（MCU的管脚：GPIO_B0_03）表示核心板版本，
    VER1 VER0 = 11B 表示为U503核心板，VER1 VER0 = 10B表示为U506核心板。

    前面版本对这两个引脚的配置有问题，第一批IV5200A可以识别出U506，
    第二批生产的IV5200A会误识别成U503，导致ECAT无法连接。 2023-9-13 ccz
    ***************************************************************************************/
	IOMUXC_SetPinConfig(CoreBard_Ver1_IOMUXC, VERSION_PAD_CONFIG_DATA);  
	IOMUXC_SetPinConfig(CoreBard_Ver0_IOMUXC, VERSION_PAD_CONFIG_DATA);  
}


 /**
  * @brief  初始化LED相关的GPIO模式
  * @param  无
  * @retval 无
  */
static void LED_GPIO_Mode_Config(void)
{     
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t led_config;      
    
   /** 核心板的LED灯，GPIO配置 **/       
  led_config.direction = kGPIO_DigitalOutput; //输出模式
  led_config.outputLogic =  1;                //默认高电平    
  led_config.interruptMode = kGPIO_NoIntmode; //不使用中断
	
  GPIO_PinInit(RUN_LED_GPIO, RUN_LED_GPIO_PIN, &led_config);
  GPIO_PinInit(UART_LED_GPIO, UART_LED_GPIO_PIN, &led_config);
  GPIO_PinInit(CANA_LED_GPIO, CANA_LED_GPIO_PIN, &led_config);
  GPIO_PinInit(CANB_LED_GPIO, CANB_LED_GPIO_PIN, &led_config);
  GPIO_PinInit(RS485_LED_GPIO, RS485_LED_GPIO_PIN, &led_config);
  GPIO_PinInit(CS_RS485_GPIO, CS_RS485_GPIO_PIN, &led_config);
  GPIO_PinInit(Free_Test_GPIO, Free_Test_GPIO_PIN, &led_config);
	
  led_config.direction = kGPIO_DigitalInput; //输入模式
  GPIO_PinInit(CoreBard_Ver1_GPIO, CoreBard_Ver1_GPIO_PIN, &led_config);
  GPIO_PinInit(CoreBard_Ver0_GPIO, CoreBard_Ver0_GPIO_PIN, &led_config);
  CS_RS485_RXD
	
}

/**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{
  /* 初始化GPIO复用、属性、模式 */
  LED_IOMUXC_MUX_Config();
  LED_IOMUXC_PAD_Config();
  LED_GPIO_Mode_Config();
}


/*********************************************END OF FILE**********************/
