#ifndef _ADMETER_DIC_H_
#define _ADMETER_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

#define 		ADMETER_CH_NUM			8
#define 		ENCODE_CH_NUM			6
#define 		ADMETER_CH_NUM_EXT		8

#define 		SPEED_MIN_CHG_TM			(2*MAX_LOOP_100US)	/*以程序的运行周期为单位， MAX_LOOP_100US，A620为0.4ms*/
#define 		SPEED_WIN_SIZE			16					/*需2的n次方*/				
#define 		SPEED_POS_MIN_CHG		4					/*电子尺最小有效变化*/
#define 		SPEED_TM_MAX_CHG 		(64*MAX_LOOP_100US)	/*以程序的运行周期为单位， MAX_LOOP_100US，A620为0.4ms , 51.2ms*/
#define 		SPEED_SAMPLE_NUM			(SPEED_WIN_SIZE/2)	/*速度采样计算次数*/

typedef struct EN_ADMETER_MACHSET
{
	UI16 	ADPOSI_PTMSETLEN[ADMETER_CH_NUM];                 // 07120000 - 07120007 位置尺长   	范围0.0-3000.0	默认2500  
    	UI16 	ADPOSI_MACHINELEN[ADMETER_CH_NUM];                // 07120008 - 0712000F  机械行程    	范围0.0-3000.0  	默认30000 
    	UI16 	ZSET[ADMETER_CH_NUM];                             // 07120010 - 07120017位置尺归零台 1~8	 		范围0.0-3000.0	默认0

	UI16 	ADPOSI_PTMSETLEN_EXT[ADMETER_CH_NUM_EXT];        //07120018-0712001F扩展板位置尺长                                  
	UI16 	ADPOSI_MACHINELEN_EXT[ADMETER_CH_NUM_EXT];       // 07120020-07120027  扩展板机械行程          
	UI16 	ZSET_EXT[ADMETER_CH_NUM_EXT];                    //07120028-0712002F  外扩电子尺位置尺归零台 
}ADMETER_MACHSET, *PADMETER_MACHSET;

typedef struct EN_ADMETER_STATE
{
    UI16 FL_ERROR0;								       //0x07160000 警报1
    UI16 FL_ERROR1;								       //0x07160001 警报2

   	UI16 ADPOSI_RELATIVE[ADMETER_CH_NUM];                   // 0x07160002 - 0x07160009 位置尺相对位置0
    UI16 ADPOSI_ABSOLUTE[ADMETER_CH_NUM];                   // 0x0716000A - 0x07160011 位置尺绝对位置0
    UI16 ADPOSI_METERMAX[ADMETER_CH_NUM];                   // 0x07160012 - 0x07160019 位置尺使用最大值
    UI16 ADPOSI_METER[ADMETER_CH_NUM];                      // 0x0716001A - 0x07160021 位置尺AD实际采集值
	
	UI16 ADPOSI_RELATIVE_EXT[ADMETER_CH_NUM_EXT];   		// 07160022- 07160029 扩展电子尺 位置尺相对位置 
	UI16 ADPOSI_ABSOLUTE_EXT[ADMETER_CH_NUM_EXT];   		// 0716002A-07160031 扩展电子尺位置尺绝对位置  
	UI16 ADPOSI_METERMAX_EXT[ADMETER_CH_NUM_EXT];   		// 07160032-07160039扩展电子尺位置尺使用最大值
}ADMETER_STATE, *PADMETER_STATE;

#define     ER0_ADMETER1      0x0001      //#07100   ; '电子尺1异常'
#define     ER0_ADMETER2      0x0002      //#07101   ; '电子尺2异常'
#define     ER0_ADMETER3      0x0004      //#07102   ; '电子尺3异常'
#define     ER0_ADMETER4      0x0008      //#07103   ; '电子尺4异常'
#define     ER0_ADMETER5      0x0010      //#07104   ; '电子尺5异常'
#define     ER0_ADMETER6      0x0020      //#07105   ; '电子尺6异常'
#define     ER0_ADMETER7      0x0040      //#07106   ; '电子尺7异常'
#define     ER0_ADMETER8      0x0080      //#07107   ; '电子尺8异常'

#define     ER0_ADMETER_EXT1      0x0100      //#07108   ; '扩展电子尺1异常'
#define     ER0_ADMETER_EXT2      0x0200      //#07109   ; '扩展电子尺2异常'
#define     ER0_ADMETER_EXT3      0x0400      //#0710A   ; '扩展电子尺3异常'
#define     ER0_ADMETER_EXT4      0x0800      //#0710B   ; '扩展电子尺4异常'
#define     ER0_ADMETER_EXT5      0x1000      //#0710C   ; '扩展电子尺5异常'
#define     ER0_ADMETER_EXT6      0x2000      //#0710D   ; '扩展电子尺6异常'
#define     ER0_ADMETER_EXT7      0x4000      //#0710E   ; '扩展电子尺7异常'
#define     ER0_ADMETER_EXT8      0x8000      //#0710F   ; '扩展电子尺8异常'


#pragma pack()

#ifdef DIC_ENUM_NEED

#endif

#ifdef __cplusplus
}
#endif

#endif

