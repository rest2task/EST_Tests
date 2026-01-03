#ifndef _MACHCFG_h
#define _MACHCFG_h
#include "typedef.h"
#ifdef __cplusplus

extern "C"
{ 
#endif
typedef struct tyCODETbl
{
	UI16 code0;
	UI16 code1;
	UI16 code2;
	UI16 code3;
	UI16 code4;
	UI16 code5;
	UI16 code43;
	UI16 code44;
	UI16 code48;
	UI16 code49;
	UI16 code50;
	UI16 code51;
	UI16 code52;
	UI16 code53;
	UI16 code54;
	UI16 oilctl;		//油路控制
	UI16 tempctl;		//温度控制
	UI16 postmtctl;		//位置尺控制
	UI16 reserved1;		//保留1
	UI16 qvr;			//国标
	UI16 reserved2;		//保留2
	UI16 reserved3;		//保留3
	UI16 reserved4;		//保留4
	UI16 reserved5;		//保留5
	UI16 manufact;		//厂商代码
	UI16 code7;
	UI16 code11;
	UI16 code12;
	UI16 syscode;		//系统代码
	UI16 version;		//版本
	UI16 verdate;		//版本日期
	UI16 slidermode;	//滑模选择
	UI16 segmentoutput;	//流量分段输出
	UI16 sysprsmax;		//系统压力上限
	UI16 setprsmax;		//设定压力上限
	UI16 prmhole;		//储料RPM孔数
	UI16 reserved6;		//保留6
	UI16 reserved7;		//保留7
	UI16 pushcode;		//推力座型号
}CODETbl;

typedef struct tyMeterTbl
{
	UI16 meterpos;	//电子尺行程
	UI16 pos;		//机械行程
	UI16 setzero;	//电子尺归零设置
}MeterTbl;

#define METER_CNT	8

typedef struct tyMCCFG
{
	CODETbl codetbl;
	MeterTbl meter[METER_CNT];
}MCCFG;

void GetMainCfg(const char* filename,MCCFG* tblcfg);
void LoadPanelData(const char *filename);
#ifdef __cplusplus
}
#endif

#endif



