#ifndef _INJSTOR_DIC_H_
#define _INJSTOR_DIC_H_

#ifdef __cplusplus
extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#pragma pack(1)

/*----------氮气辅助部件(射出增压)----------*/
typedef struct EN_INJSTOR_PO
{
    UI16  O_INJ_STO ;                 //0x12100000 储能
    UI16  O_STO_REL;                  //0x12100001 储能泄压
    UI16  O_INJ_STO_SRV;              //0x12100002 储能备用
}INJSTOR_PO, *PINJSTOR_PO;

typedef struct EN_INJSTOR_PI
{
    UI16  I_INJ_STO ;                 //0x12110000 射出储能
    UI16  I_INJ_STO_SRV;              //0x12110001 储能备用
}INJSTOR_PI, *PINJSTOR_PI;

typedef struct EN_INJSTOR_MACHSET
{
    UI16 INJSTOR_METERCH;				    // 0x12120000 蓄能器压力检测通道 范围4-7 默认6
}INJSTOR_MACHSET, *PINJSTOR_MACHSET;
typedef struct EN_INJSTOR_MOLDSET
{
    /*----------氮气辅助部件(射出增压)----------*/
    UI16  DAPRES_INJSTOR;                   // 0x12130000 射出增压蓄能器设定压力
    UI16  DAPRES_INJSMAX;                   // 0x12130001 蓄能器压力上限         范围0~250 默认160
    UI16  DAPRES_INJSMIN;                   // 0x12130002 蓄能器压力下限         范围0~250 默认140
}INJSTOR_MOLDSET, *PINJSTOR_MOLDSET;

typedef struct EN_INJSTOR_ACTPARA
{
    /*----------氮气辅助部件(射出增压)----------*/
    UI16  DAPRES_INJSTOR;             // 0x12140000 蓄能器充压压力 范围0-140 默认140
    UI16  DAFLOW_INJSTOR;             // 0x12140001 蓄能器充压速度 范围0-99           默认99
}INJSTOR_ACTPARA, *PINJSTOR_ACTPARA;

typedef struct EN_INJSTOR_HYDR
{
    /*+++++++++HYDR(PORT;PUMPS)+++++++++*/
    UI16  HYDR_PORT_INJSTOR;		 //0x12150000 氮气辅助部件DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_INJSTOR;		 //0x12150001 氮气辅助部件比例速度限制:位操作-->P1……P7

}INJSTOR_HYDR, *PINJSTOR_HYDR;

typedef struct EN_INJSTOR_STATE
{
    UI16  FL_ERROR0;				//0x12160000   警报1
    UI16  FL_ERROR1;				//0x12160001   警报2
    UI16  Step_Injor;               //0x12160002   射出储能实时动作步     成品画面显示
}INJSTOR_STATE, *PINJSTOR_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED


/*ACT SN start from 1*/
enum EN_INJSTOR_ACT_SN
{
    OS_INJSTO = 1,				// 0   ; '射出增压'

    INJSTOR_ACT_SN_END
};

#endif

#ifdef __cplusplus
}
#endif

#endif






