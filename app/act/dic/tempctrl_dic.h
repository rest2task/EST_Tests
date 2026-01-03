#ifndef _TEMPCTRL__DIC_H_
#define _TEMPCTRL_DIC_H_


#include "typedef.h"
#include "admeter.h"

#pragma pack(1)

#define	TEMP_CH_NUM	16	//A960只使用9+1;温度扩展板不可用


typedef struct tyTEMP_ZDSET
{
	//	由保存在下位机的数据获取
	UI16 WTEMP_CHZERO[TEMP_CH_NUM];		 	// 0x05140000 [全电] 温度归零值[18]
	UI16 WTEMP_CHB[TEMP_CH_NUM];		 	// 0x05140010 [全电] 温度校准值[18]
	UI16 WTEMP_OFFSET;					 	// 0x05140020 [全电] 室温校准	
}TEMP_ZDSET, *PTEMP_ZDSET;

typedef struct EN_TEMPCTRL_STATE
{
	/*DB_TEMP_ID 560*/
	UI16  FL_ERROR0;						/*	0x05160000	警报1*/
	UI16  FL_ERROR1;						/*	0x05160001	警报2*/

	UI16  WTEMP_LM73;					    /*	0x05160002  室温LM73的温度*/
	UI16  WTEMP_LM73UV;					    /*	0x05160003  室温对应uV值*/
	UI16  WTEMP_ADJAMB;			  			/*	0x05160004  室温校准值(上位机发下来) 范围0.0~50.0*/
	UI16  WTEMP_ADJ;						/*	0x05160005  室温系数 */
}TEMPCTRL_STATE, *PTEMPCTRL_STATE;

#ifdef DIC_ENUM_NEED

#define     ER0_WTEMPEXT		0x0001      // #05100；外部室温异常  
#endif

#pragma pack()

#endif

