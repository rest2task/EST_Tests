#ifndef _OIL__DIC_H_
#define _OIL_DIC_H_

#include "typedef.h"

#pragma pack(1)

/*油温控制和油检测放在一起*/
typedef struct EN_OIL_PO
{
    UI16 O_OILHEAT1 ;                   //0x0C100000 油温1 
    UI16 O_OILCOOL1;                    //0x0C100001 油温冷却  
	UI16 O_OIL_RSV;                     //0x0C100002 备用
	UI16 O_OILCOOL2;                    //0x0C100003 水温冷却
}OIL_PO, *POIL_PO;

typedef struct EN_OIL_PI
{
    UI16 I_FIL_BLK;                     //0x0C110000 滤油网检知
    UI16 I_OIL_LVL;                     //0x0C110001 油压油位检知 
    UI16 I_OIL_RSV;                     //0x0C110002 备用
}OIL_PI, *POIL_PI;

typedef struct  EN_OIL_MACHSET
{
    UI16 OIL_TEMPCH;					  /* 0x0C120000 油温温度通道, 范围1-16     默认1   未使用*/
	UI16 FL_OILWARM;                      /* 0x0C120001 油温预热使用  0不使用1使用 默认0   未使用*/
	UI16 DATEMP_OILMIN;                   /* 0x0C120002 油箱油温下限	 单位:摄氏度	默认0	范围0-100	*/
	UI16 DATEMP_OILMAX;                   /* 0x0C120003 油箱油温上限     单位:摄氏度	默认55	范围0-100	*/
    UI16 MIN_PRESS;						  /* 0x0C120004 最小系统压力，当实际系统压力小于最小系统压力且持续时间超过检测时间时警报  0—系统压力上限  默认5.0bar	20190409*/
    UI16 TM_PROTECT;					  /* 0x0C120005 系统压力检测时间，当实际系统压力小于最小系统压力且持续时间超过检测时间时警报  0.00—99.99s  默认5.00s   20200219 */
    UI16 FL_CHKOIL;						  /* 0x0C120006 漏油检测功能，0：不使用，1：使用，默认0：不使用 20190409*/
    //20190531.dxl
    UI16 FL_TEMP_OIL;					  /* 0x0C120007	油温功能	默认1	范围0:不使用	1:使用	*/
    UI16 DATEMP_COOLERONFOROIL;           /* 0x0C120008 油温冷却开启温度	单位:摄氏度	默认50	范围0-100	*/
    UI16 DATEMP_COOLEROFFFOROIL;          /* 0x0C120009 油温冷却关闭温度	单位:摄氏度	默认35	范围0-100	*/

    UI16 DATEMP_OILMIN2;                  /* 0x0C12000A 水温下限  默认0度 范围0-100 */
	UI16 DATEMP_OILMAX2;                  /* 0x0C12000B 水温上限  默认55度  范围0-100 */
	UI16 DATEMP_COOLERONFORWATER2;        /* 0x0C12000C 水温冷却开启温度  默认50度  范围0-100 */
    UI16 DATEMP_COOLEROFFFORWATER2;       /* 0x0C12000D 水温冷却关闭温度  默认35度  范围0-100 */
}OIL_MACHSET, *POIL_MACHSET;

typedef struct EN_OIL_STATE
{
	UI16  FL_ERROR0;						//0x0C160000 警报1
	UI16  FL_ERROR1;						//0x0C160001 警报2
	UI16  DATEMP_OIL;                       //0x0C160002 油温实际             
	UI16  DATEMP_OIL2;                      //0x0C160003 水温实际
}OIL_STATE, *POIL_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

#define	  ER1_FILT	     0x0001             //#0C100  液压油滤网阻塞
#define	  ER1_OILV	     0x0002             //#0C101  液压油位低 
#define	  ER0_OILT	     0x0004             //#0C102  油温偏差 
#define	  ER0_OILLEAK	 0x0008         	//#0C103  漏油    'OIL PRESSURE. TOO LOW  '   20190409
#define	  ER0_OILT2	     0x0010             //#0C104  水温偏差

#endif


#endif


