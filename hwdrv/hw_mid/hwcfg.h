#ifndef _HWCFG_H_
#define _HWCFG_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "common.h"

extern UI16    g_max_loop_100us;


#define MODEL_I3100  	0x3100
#define MODEL_I3200  	0x3200
#define MODEL_I5200  	0x5200

#define MODEL_I3101		0x3101
#define MODEL_I3201		0x3201
#define MODEL_I3202		0x3202
#define MODEL_I5201		0x5201
#define MODEL_I3521		0x3521

#ifndef HW_MODEL
#define HW_MODEL  	MODEL_I3521
#endif

#define EXT_PBMAX    		16
#define EXT_PCMAX    		16

#define MAX_AD16	 0x10000
#define MAX_AD14     0x4000
#define MAX_AD12     0x1000
#define MAX_AD10     0x0400

/*硬件类型，通过寄存器读取，共 4位，识别16种*/
#define PCB_NAME_3201	0x07 //光塑特殊使用
#define PCB_NAME_5201	0x06
#define PCB_NAME_3101	0x05
#define PCB_NAME_3202	0x04						

#define I3101_PIMAX					32
#define I3101_POMAX					48
#define I3101_MAXDAPORT				4
#define I3101_MAXADPORT				6
#define I3101_MAX_ADVAULE			MAX_AD16
#define I3101_INN_MAXTEMPCHNL	    10				//A1000实际为9+ 1段，暂时改成8段, *温度通道数,应根据机型来设定该值*/

#define I3201_PIMAX					32
#define I3201_POMAX					48
#define I3201_MAXDAPORT			    4
#define I3201_MAXADPORT			    4
#define I3201_MAX_ADVAULE			MAX_AD16
#define I3201_INN_MAXTEMPCHNL	    10		

#define I3202_PIMAX					32
#define I3202_POMAX					48
#define I3202_MAXDAPORT			    4
#define I3202_MAXADPORT			    6
#define I3202_MAX_ADVAULE			MAX_AD16
#define I3202_INN_MAXTEMPCHNL	    10		

#define I5201_PIMAX					48
#define I5201_POMAX					64
#define I5201_MAXDAPORT			8
#define I5201_MAXADPORT			8
#define I5201_MAX_ADVAULE			MAX_AD16
#define I5201_INN_MAXTEMPCHNL	       10				/*IV5000实际为8段*/

//以下宏定义为应用层需要用到的，且比较固定，一般不需要修改

#define HW_DA_PORT_NUM 		8

#define DEF_CANADJ_METER_CH     1
#define DEF_INJECT_METER_CH		1
#define DEF_CLAMP_METER_CH		2
#define DEF_EJECT_METER_CH		3
#define DEF_PRESSENS_METER_CH	3
#define DEF_NOZZEL_METER_CH		4
#define DEF_PRESSENS2_METER_CH	5


#define FLASH_IAP_BASE		0x00000000
#define FLASH_W_ALGN		0x200				//FLASH写长度和起始地址必须是256的倍数

#define MAX_LOOP_100US		g_max_loop_100us
#define MAX_LOOP_10US		(MAX_LOOP_100US*10)
#define	CYCTM_US		(MAX_LOOP_100US*100)		/*周期时间：500us	*/

#define SPC_CLAMP_MAX_LOOP_100US		2



#define RS232_BR				57600
#define TEMP_AD_MAX_VALUE		MAX_AD12

typedef struct tyHW_CFG_VAR
{
	UI16 hw_model;				//主板型号
	UI16 hw_maxadport;			//主板最大AD通道数
	UI32 hw_max_advalue;		//主板AD芯片最大值，16位，14位，12位。。
	UI16 hw_pimax;				//主板最大输入点数
	UI16 hw_pomax;				//主板最大输出点数
	UI16 hw_inn_maxtempchnl;	//主板最大温度通道数	
	UI16 hw_da_port_num;		//主板最大DA输出通道
	UI16 press_senor_ch;		//主板的压力通道口，暂时没啥用  20210810   yxh
	UI16 hw_I3I5;				//区分主板是属于IV3201，IV5201，IV3101中的哪一种
	UI16 hw_ADS8688;			//预留
	BOOL hw_surtempnew;					/* 0 : 老的室温芯片 LM73   1 :新的室温芯片 NST112 */
}HW_CFG_VAR, *PHW_CFG_VAR;

extern HW_CFG_VAR	hw_cfg;


#define MAX_TEMPCHNL 	(hw_cfg.hw_inn_maxtempchnl)			
#define HW_PIMAX	(hw_cfg.hw_pimax)
#define HW_POMAX	(hw_cfg.hw_pomax)
#define MAXADPORT	(hw_cfg.hw_maxadport)
#define MAX_ADVAULE	(hw_cfg.hw_max_advalue)
#define DEF_PRSSENS_METR_CH (hw_cfg.press_senor_ch)
#define HW_DAPORTMAX	(hw_cfg.hw_da_port_num)
#define GetHwModel() (hw_cfg.hw_model)
#define HW_SURTEMPNEW (hw_cfg.hw_surtempnew)

#ifdef __cplusplus
}
#endif

#endif

