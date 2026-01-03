#ifndef _AICLAMPOPN_H_
#define _AICLAMPOPN_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#define AICLP_ROWRAMP_NUM	10
#define AICLP_ROW_NUM	20
#define POS_DIFF pos_diff

typedef struct tyAICLAMPDATA_TBROW
{
	UI16		ramp[AICLP_ROWRAMP_NUM];	//	10个速度等级的ramp数据
	UI16		pos_k;						//对应位置的闭环系数
	UI16		pos_spdlimit;				//对应位置的速度上限
}AICLAMPDATA_TBROW, * PAICLAMPDATA_TBROW;


typedef struct tyAICLAMPDATA_ROW
{
	PAICLAMPDATA_TBROW		p_rampArray;	//	10个速度等级的ramp数据
	UI16		ramp_num;	//	ramp包含的斜率数量
	UI16		pos;		//	对应斜率组的位置	0.1mm
}AICLAMPDATA_ROW, * PAICLAMPDATA_ROW;


//extern UI16	FuncAiClampOpn = 0;


BOOL AiTBLReqRx(void);
void InitAiClamp(void);
UI16 AiClampRowDataGet(UI16 pos,UI16 spd);
void ClearAiClampTB(void);
void InitAiClampRampNum(void);
UI16 AiClampRampDataInput(UI16 pos,UI16 spd,UI16 ramp);






#ifdef __cplusplus
}
#endif

#endif

