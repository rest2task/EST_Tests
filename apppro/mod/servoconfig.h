#ifndef _SERVOCONFIG_h
#define _SERVOCONFIG_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

#define SERVOCONFIG_NUM_LEN      10
#define SERVOCONFIG_NAME_LEN     255
#define SERVOCONFIG_UNIT_LEN     25
#define SERVOCONFIG_DESC_LEN     255

typedef enum enSERVOCONFIG_DATATYPE
{
    TYPE_H00 = 1,
    TYPE_H01,
    TYPE_H02,
    TYPE_H03,
    TYPE_H04,
    TYPE_H05,
    TYPE_H06,
    TYPE_H07,
    TYPE_H08,
    TYPE_H09,
    TYPE_H0A,
    TYPE_H0B,
    TYPE_H0C,
    TYPE_H0D,
    TYPE_H17,
    SERVOCONFIG_DATATYPE_END
}SERVOCONFIG_DATATYPE;

typedef struct tySERVOCONFIG * PSERVOCONFIG;
typedef struct tySERVOCONFIG{
    char num[SERVOCONFIG_NUM_LEN];          //参考编码
    char name[SERVOCONFIG_NAME_LEN];        //参考名称
    UI32 curvalue;                          //参考值
    UI32 defvalue;                          //出厂值
    char unit[SERVOCONFIG_UNIT_LEN];        //单位
    UI16 width;                             //长度
    PSERVOCONFIG pnext;
}SERVOCONFIG;

typedef struct tyCONFIGSELECT * PCONFIGSELECT;
typedef struct tyCONFIGSELECT{
    char desc[SERVOCONFIG_DESC_LEN];   //描述信息
    PCONFIGSELECT pnext;
}CONFIGSELECT;

BOOL LoadServoConfig();
UI16 GetHParaNum(int data_type);
char* GetHPara(int data_type, char* data_name, UI16 data_id, char* buff);
void ClearHPara();
void SetHPara_CurValue(int data_type, UI16 data_id, UI32 servo_val);

#define SERVOCONFIG_FILE_CH_NAME HMI_ROOT_PATH"save/650N_CH.csv"
#define SERVOCONFIG_FILE_EN_NAME HMI_ROOT_PATH"save/650N_EN.csv"
#define SERVOCONFIG_FILE_SELECT_CH_NAME HMI_ROOT_PATH"save/650N_SELECT_CH.csv"
#define SERVOCONFIG_FILE_SELECT_EN_NAME HMI_ROOT_PATH"save/650N_SELECT_EN.csv"

#ifdef __cplusplus
}
#endif

#endif



