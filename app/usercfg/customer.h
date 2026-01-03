#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#define M_MODEL_STD		0


/*原客户代码转换到新客户类型映射表*/
typedef struct tyCUS_TRAN_MAP
{
	UI16	old_cus_id;
	UI16	new_cus_id;
}CUS_TRAN_MAP, *PCUS_TRAN_MAP;

#define CUS_STD		0				//标准版本
#define CUS_HT		1				//海太
#define CUS_TY		2				//通用
#define CUS_SHGS	3				//上海光塑
#define CUS_ZW		4				//中微
#define CUS_ERD		5				//恩瑞德
#define CUS_HH		6				//海航

#if 0
#define CUS_SXFX			7				//山西汾西
#define CUS_MEIGE			8				//宁波美格
#define CUS_CHUANGHUI		9				//宁波创汇
#define CUS_ZHITIAN			10				//北仑智天
#define CUS_YUXIN			11				//鄞州宇兴
#define CUS_ZHENYU			12				//舟山震宇
#define CUS_YONGJIANG		13				//甬江
#define CUS_HAIBO			14				//海波
#define CUS_HUASHU			15				//浙江华塑
#define CUS_JITAI			16				//宁波捷泰#define CUS_SHUANGSHEN		17				//宁波双盛
#define CUS_ZHENJIE			18				//北仑震杰
#define CUS_ZHENYONG		19				//宁波震甬
#define CUS_NINGSHU			20				//宁波宁塑
#define CUS_TAIRUI			21				//泰瑞
#endif

typedef enum enCUS_MAP
{
	CS_STD = 0x0100, 			/*EST标准*/
	CS_TJ   =0x0200,            /*海天天剑*/
	CS_DAAI = 0x0300,           /*大爱*/
	CS_GELINSIDUN = 0x0400,     /*格林斯顿*/
	CS_NAIXUN = 0x0400,         /*宁波耐迅*/
	CS_INOVANCE = 0x0900,		/*汇川标准*/
	CS_INOVA = 0x1000,			/*INOVA*/
	CS_ZW = 0x4000, 			/*北京中威*/
	CS_FENXI = 0x4100,			/*山西汾西*/
	CS_TAIRUI = 0x4200,			/*杭州泰瑞*/
	CS_DONGHUA = 0x4300,		/*广州东华*/
	CS_NBHAITIAN  = 0x4500,     /*宁波海田*/
	CS_NBDAMEI  = 0x4600,     	/*宁波达美(海石)*/
	CS_NBKLG  = 0x4700,     	/*宁波科隆阁*/
	CS_NBSHUANGMA = 0x4800,	    /*宁波双马(伯乐)*/
	CS_NBYS = 0x4900,			/*宁波永生*/
	CS_HAIJIANG = 0x4A00,		/*宁波海江*/
	CS_MAIDAO = 0x4B00,		    /*宁波迈道*/
	CS_MEIGE = 0x5100, 			/*宁波美格*/
	CS_CHUANGHUI = 0x5200, 		/*宁波创汇*/
	CS_ZHITIAN = 0x5300, 		/*北仑智天*/
	CS_YUXIN = 0x5400, 			/*鄞州宇兴*/
	CS_ZHENYU = 0x5500, 		/*舟山震宇*/
	CS_YONGJIANG = 0x5600, 		/*宁波甬江*/
	CS_ENRUIDE = 0X5700, 		/*宁波恩瑞德*/
	CS_HAIBO = 0X5800, 			/*宁波佳通(海波)*/
	CS_NBYISHU = 0x5900,		/*宁波亿塑*/
	CS_HUASHU = 0x6200, 		/*浙江华塑*/
	CS_JINYING = 0x6400, 		/*舟山金鹰*/
	CS_WEIDA = 0x6600,			/*广东伟达*/
	CS_YONGFENG = 0x6700,		/*宁波甬丰*/
	CS_JIETAI = 0x7100, 		/*宁波捷泰*/
	CS_SHUANGSHEN = 0x7200, 	/*宁波双盛*/
	CS_ZHENJIE = 0x7300, 		/*北仑震杰*/
	CS_ZHENYONG = 0x7400, 		/*宁波震甬*/
	CS_NINGSHU = 0x7500, 		/*宁波宁塑*/
	CS_ZHONGSHU = 0x7600, 		/*宁波中塑*/
	CS_BONA= 0x7700, 			/*宁波博纳*/
	CS_ENGE = 0x7800, 			/*宁波恩格*/
	CS_YONGDA = 0x7900, 		/*宁波永达*/
	CS_WEILISHI = 0x7A00, 		/*威力仕高科*/
	CS_YAOHUI = 0x7B00, 		/*广东耀威*/
	CS_HAIQIANG = 0x7C00, 		/*宁波海强*/
	CS_HAIJIN = 0x7D00, 		/*宁波海晶*/
	CS_HAIRUN = 0x7E00, 		/*宁波海润*/
	CS_MINJIE = 0x7F00, 		/*宁波铭捷*/
	CS_GUANGSHU = 0x8000, 		/*上海光塑*/
	CS_HENRUN = 0x8200, 		/*宁波亨润*/
	CS_HUAMEIDA = 0x8400, 		/*宁波华美达*/
	CS_HAITAI = 0x8600, 		/*宁波海太*/
	CS_HAIXIN = 0x8700, 		/*宁波海星*/
	CS_SHENDA = 0x8800,         /*德清申达*/
	CS_JINGQIONG = 0x8900,      /*宁波京琼*/
	CS_TONGYONG = 0x8B00, 		/*宁波通用*/
	CS_TAILIKE = 0x8E00, 		/*宁波泰立克*/
	CS_ZHUANHUI = 0x8F00, 		/*宁波展晖（海航)*/
	CS_JIAMING = 0x9000, 		/*广东佳明*/
	CS_ZHONGHUI = 0x9100, 		/*广东中汇*/
	CS_SHIBITUO = 0x9200, 		/*广东思必拓*/
	CS_YANGXIN = 0x9300, 		/*广东阳鑫*/
	CS_JINGDIWEI = 0x9500, 		/*广东金迪威*/
	CS_LIANSHENG = 0x9600,      /*广东联升*/
	CS_YOUZHU = 0x9700,         /*广东优铸*/
	CS_YIZHIMI = 0x9800,        /*广东伊之密*/
	CS_GUANGMIN = 0xB000, 		/*温州光明*/
	CS_SHENGBANG = 0xB200,		/*温州圣邦*/
	CS_ZHISHANG = 0xB300, 		/*温州至上*/
	CS_YUYAOLOG = 0xB400, 		/*宁波创基(朗格)*/
	CS_LIQUAN = 0xB500, 		/*广东力泉*/
	CS_HENHUI = 0xB600, 		/*广东恒辉*/
	CU_FUSHTY = 0xB700,          /*佛山天银*/
	CS_NBHD   = 0xB800,         /*宁波海达*/
	CS_ZXSJ = 0xB900,			/*震雄塑机*/
	CS_GDWA = 0xBA00,			/*广东威亚*/
	CS_GDYT = 0xBB00,			/*广东亚太*/
	CS_NBSS = 0xBC00,           /*宁波三顺*/
	CS_LIWEI = 0xBD00,			/*浙江力维*/
	CS_BSD = 0xBE00,			/*广东必盛达*/
	CS_BLK = 0xBF00,			/*宁波博洛克*/
	/*立式机厂商*/
	CS_TIANYI = 0xC000,			/*宁波天一*/	
	CS_GONGZENG = 0xC100,		/*工正立式*/
	CS_DECHUANG = 0xC200,		/*德创立式*/
	CS_JIASU = 0xC300,			/*佳塑立式*/
	CS_JINGJIA = 0xC400,		/*精佳立式*/
	CS_BAJISITAN = 0xC500,		/*巴基斯坦*/
	CS_YADEWEI = 0xC600,		/*雅迪威*/
	CS_MJLKH = 0xC700,			/*孟加拉*/
	CS_JINGDIAO = 0xC800,		/*常州精雕*/
	CS_SAIPULE = 0xC900,		/*宁波赛普乐*/
	CS_JIBOSHI = 0xCA00,		/*宁波机博士*/
	CS_RUSSIA = 0xCB00,		    /*俄罗斯*/
	CS_ZHISU = 0xCC00,          /*广东智塑*/
	CS_FENGTIE = 0xCD00,        /*苏州丰铁*/
}CUS_MAP;

/*客户选择*/
#define CUSTOMER 	CUS_STD
//#define CUSTOMER 	CUS_ERD
//#define CUSTOMER 	CUS_ZW
//#define CUSTOMER 	CUS_HH
//#define CUSTOMER 	CUS_TY
//#define CUSTOMER	CUS_TAIRUI

/*射出精度到1ms*/
#define INJ_TM_PRECIS		1			/*0 10ms, 1: 1ms*/

#if(CUSTOMER == CUS_ERD)
#include "enruide.h"
#elif(CUSTOMER == CUS_ZW)
#include "zhongwei.h"
#endif

#define CUS_MODEL_STD	((CUSTOMER) << 16)	

#ifndef CUS_MODEL
#define CUS_MODEL		CUS_MODEL_STD
#endif

#include "cfgdef.h"

#ifdef __cplusplus
}
#endif

#endif



