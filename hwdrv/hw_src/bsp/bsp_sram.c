/**
  ******************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   SRAM驱动
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"  
#include "fsl_iomuxc.h"
#include "fsl_semc.h"  
	
#include "pad_config.h"  
#include "bsp_sram.h"  

/*******************************************************************************
 * 宏
 ******************************************************************************/

/* 所有引脚均使用同样的PAD配置 */
#define SRAM_PAD_CONFIG_DATA            (SRE_1_FAST_SLEW_RATE| \
                                            DSE_7_R0_7| \
                                            SPEED_3_MAX_200MHz| \
                                            ODE_0_OPEN_DRAIN_DISABLED| \
                                            PKE_1_PULL_KEEPER_ENABLED| \
                                            PUE_0_KEEPER_SELECTED| \
                                            PUS_0_100K_OHM_PULL_DOWN| \
                                            HYS_1_HYSTERESIS_ENABLED)   
                                            
                                            /* 所有引脚均使用同样的PAD配置 */
#define SRAM_CS_PAD_CONFIG_DATA            (SRE_1_FAST_SLEW_RATE| \
                                            DSE_7_R0_7| \
                                            SPEED_3_MAX_200MHz| \
                                            ODE_1_OPEN_DRAIN_ENABLED| \
                                            PKE_1_PULL_KEEPER_ENABLED| \
                                            PUE_1_PULL_SELECTED| \
                                            PUS_1_47K_OHM_PULL_UP| \
                                            HYS_1_HYSTERESIS_ENABLED) 
    /* 配置说明 : */
    /* 转换速率: 转换速率快
        驱动强度: R0/7 
        带宽配置 : max(200MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 100K欧姆下拉(选择了保持器此配置无效)
        滞回器配置: 使能 */



/*******************************************************************************
 * 声明
 ******************************************************************************/
static void SRAM_IOMUXC_MUX_Config(void);
static void SRAM_IOMUXC_PAD_Config(void);
static status_t SRAM_SEMC_Config(void);


/**
* @brief  初始化SRAM相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void SRAM_IOMUXC_MUX_Config(void)
{
  /* 除DQS信号外，其余引脚均可不开启SION功能 */
  /* DATA信号线DATA00~DATA15 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_00_SEMC_DATA00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_01_SEMC_DATA01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_02_SEMC_DATA02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_03_SEMC_DATA03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_SEMC_DATA04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_05_SEMC_DATA05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_SEMC_DATA06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_07_SEMC_DATA07, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_30_SEMC_DATA08, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_31_SEMC_DATA09, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_32_SEMC_DATA10, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_33_SEMC_DATA11, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_34_SEMC_DATA12, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_35_SEMC_DATA13, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_36_SEMC_DATA14, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_37_SEMC_DATA15, 0U);  
  
  /* ADDR信号线ADDR00~ADDR12 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_09_SEMC_ADDR00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_10_SEMC_ADDR01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_11_SEMC_ADDR02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_SEMC_ADDR03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_13_SEMC_ADDR04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_14_SEMC_ADDR05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_SEMC_ADDR06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_16_SEMC_ADDR07, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_17_SEMC_ADDR08, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, 0U);     //nWE                                   
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, 0U);     //nOE
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_41_SEMC_CSX00,0U);   //nGCS6_0
//  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_SEMC_CSX01,0U);   //nGCS6_1
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_SEMC_CSX02,0U);   //nGCS5_2
}

/**
* @brief  初始化SRAM相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
static void SRAM_IOMUXC_PAD_Config(void)
{
  /* 所有引脚均使用同样的PAD配置 */
   /* DATA信号线DATA00~DATA15 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_00_SEMC_DATA00, SRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_01_SEMC_DATA01, SRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_02_SEMC_DATA02, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_03_SEMC_DATA03, SRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_04_SEMC_DATA04, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_05_SEMC_DATA05, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_06_SEMC_DATA06, SRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_07_SEMC_DATA07, SRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_30_SEMC_DATA08, SRAM_PAD_CONFIG_DATA);         
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_31_SEMC_DATA09, SRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_32_SEMC_DATA10, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_33_SEMC_DATA11, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_34_SEMC_DATA12, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_35_SEMC_DATA13, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_36_SEMC_DATA14, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_37_SEMC_DATA15, SRAM_PAD_CONFIG_DATA);                             
  
  /* ADDR信号线ADDR00~ADDR12 */               
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_09_SEMC_ADDR00, SRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_10_SEMC_ADDR01, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_11_SEMC_ADDR02, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_12_SEMC_ADDR03, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_13_SEMC_ADDR04, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_14_SEMC_ADDR05, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_15_SEMC_ADDR06, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_16_SEMC_ADDR07, SRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_17_SEMC_ADDR08, SRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, SRAM_PAD_CONFIG_DATA);    //nWE                         
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, SRAM_PAD_CONFIG_DATA);    //nOE
  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_41_SEMC_CSX00,SRAM_PAD_CONFIG_DATA);   //nGCS6_0
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_SEMC_CSX01,SRAM_PAD_CONFIG_DATA);   //nGCS6_1
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_SEMC_CSX02,SRAM_PAD_CONFIG_DATA);   //nGCS5_2
}

/**
* @brief  初始化SRAM相关的SEMC配置
* @param  无
* @retval 无
*/
static status_t SRAM_SEMC_Config(void)
{  
    semc_config_t config;
    semc_sram_config_t sramconfig;
    uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;
//    uint32_t init_NS = 1;
    /* 结构体的内容全部清零 */
    memset(&config, 0, sizeof(semc_config_t));
    memset(&sramconfig, 0, sizeof(semc_sram_config_t));
    /* 使用默认配置初始化 SEMC. */
    SEMC_GetDefaultConfig(&config);
    /* ！注意！此处必须使用这个模式，
       否则SRAM驱动时钟不能跑到高频率 */
    config.dqsMode = kSEMC_Loopbackdqspad;
    SEMC_Init(SEMC, &config);
    sramconfig.cePinMux = kSEMC_MUXCSX0;
    sramconfig.addr27 = kSEMC_MORA27_NONE;
    sramconfig.address = 0xa0000000;
    sramconfig.memsize_kbytes = 32*1024;
    sramconfig.addrPortWidth = 24;
    sramconfig.advActivePolarity = kSEMC_AdvActiveLow;
    sramconfig.addrMode = kSEMC_AddrDataMux;
    sramconfig.burstLen = kSEMC_Nor_BurstLen32;
    sramconfig.portSize = kSEMC_PortSize16Bit;
    

    
    /* 使用以上配置初始化，并使用CS0，即用在Region 0区域 */
    SEMC_ConfigureSRAM(SEMC,&sramconfig,clockFrq);
    
    return 1;


}


/**
* @brief  初始化SRAM
* @param  无
* @retval 无
*/
status_t SRAM_Init(void)
{  
  CLOCK_SetMux(kCLOCK_SemcMux, 1);
  CLOCK_SetMux(kCLOCK_SemcAltMux, 0);  
  CLOCK_SetDiv(kCLOCK_SemcDiv, 6);

  SRAM_IOMUXC_MUX_Config();
  SRAM_IOMUXC_PAD_Config();
  return SRAM_SEMC_Config();
}


static status_t NOR_Config(void)
{  
//    semc_config_t config;
    semc_nor_config_t norconfig;
    uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;
 //   uint32_t init_NS = 1;
    /* 结构体的内容全部清零 */
  //  memset(&config, 0, sizeof(semc_config_t));
    memset(&norconfig, 0, sizeof(semc_nor_config_t));
//    /* 使用默认配置初始化 SEMC. */
//    SEMC_GetDefaultConfig(&config);
//    /* ！注意！此处必须使用这个模式，
//       否则SRAM驱动时钟不能跑到高频率 */
//    config.dqsMode = kSEMC_Loopbackdqspad;
//    SEMC_Init(SEMC, &config);
    
    /*! @brief SEMC NOR configuration structure. */
    
    norconfig.cePinMux = kSEMC_MUXCSX2;
    norconfig.addr27 = kSEMC_MORA27_NONE;
    norconfig.address = 0xb0000000;
    norconfig.memsize_kbytes = 32*1024;
    norconfig.addrPortWidth = 24;
    norconfig.rdyactivePolarity = kSEMC_RdyActiveLow;
    norconfig.advActivePolarity = kSEMC_AdvActiveLow;
    norconfig.columnAddrBitNum = kSEMC_NorColum_12bit;
    norconfig.addrMode = kSEMC_AddrDataMux;
    norconfig.burstLen = kSEMC_Nor_BurstLen32;
    norconfig.portSize = kSEMC_PortSize16Bit;
    
    norconfig.tCeSetup_Ns = 10;
    norconfig.tCeHold_Ns = 100;
    norconfig.tCeInterval_Ns = 10;
    norconfig.tAddrSetup_Ns = 10;
    norconfig.tAddrHold_Ns = 100;
    norconfig.tWeLow_Ns = 200;
    norconfig.tWeHigh_Ns = 50;
    norconfig.tReLow_Ns = 200;
    norconfig.tReHigh_Ns = 50;
    norconfig.tTurnAround_Ns = 10;
    norconfig.tAddr2WriteHold_Ns = 10;
    
    /* 使用以上配置初始化，并使用CS0，即用在Region 0区域 */
    return SEMC_ConfigureNOR(SEMC,&norconfig,clockFrq);
}

#include "fsl_semc.h"
status_t SEMC_ConfigureIPCommand(SEMC_Type *base, uint8_t size_bytes);
void NOR_Init(void)
{
//  CLOCK_SetMux(kCLOCK_SemcMux, 1);
//  CLOCK_SetMux(kCLOCK_SemcAltMux, 0);  
//  CLOCK_SetDiv(kCLOCK_SemcDiv, 2);

//  SRAM_IOMUXC_MUX_Config();
//  SRAM_IOMUXC_PAD_Config();
	NOR_Config();
	SEMC_ConfigureIPCommand(SEMC, 2);

}

/*********************************************END OF FILE**********************/
