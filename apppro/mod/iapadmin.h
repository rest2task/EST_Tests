#ifndef _IAPADMIN_h
#define _IAPADMIN_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

typedef enum enIAPUPDATE_ST
{
    IAPUPDATE_FAIURE = -2,
    IAPUPDATE_CANCEL = -1,
    IAPUPDATE_START = 0,
    IAPUPDATE_OK = 1,
    IAPUPDATE_ING ,
    IAPUPDATE_WAIT
}IAPUPDATE_ST;


void Iap_Send(int nparam1);
void Iap_SendCancel();
void Monitor_Iap();
BOOL IsIapSending();//判断当前扩展板是否正在发送包
UI16 GetIapUpdateState();//获取IAP更新状态
UI16 GetSend_IAPPack();//获取当前IAP发送的包号
void ResetStart();//重置标记位归零


UI16 g_CurrentChoose_Iap;//扩展板类型对应的CAN ID 由枚举决定 //20211229 dyl IAP
UI16 g_CurrentChoose_IapNum;//需要扩展板的num，由拨码决定
UI32 Total_filesize;//读取到的文件长度

#ifdef __cplusplus
}
#endif

#endif



