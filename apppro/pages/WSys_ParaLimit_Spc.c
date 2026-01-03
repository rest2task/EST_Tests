#include "WSys_ParaLimit_Spc.h"
#include "typedef.h"
#include "pagefram.h"
#include "time.h"
#include "stdio.h"
#include "database.h"
#include "oprintf.h"
#include <stdlib.h>
#include "pushtab.h"

DEFINE_SYS_PARALIMIT_SPC_MAP

CONNECT_SYS_PARALIMIT_SPC_EVENT

#define TMP_CALIBRATE_PASSWORD  TMPVAL(58)//威力士特殊计算出来的密码
#define TMP_ENABLE              TMPVAL(59)//威力士内容输入密码后是否可用
#define TMP_SHOW                TMPVAL(60)//威力士密码内容后是否显示
static void SetLimit()
{
    //20241017 chj 威力士增加压力上限限制 start -- CLSHIPRS_PRS_MAX CHGBKPRS_PRS_MAX
    //关模高压压力上限start
    int valueClsHiPrs = VarAdrToUI16(d_machine1_MACHSET_DAPRES_SYSLMT);
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00)
    {
        if(VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE) == 1)
        {
            valueClsHiPrs = VarAdrToUI16(p_PP_MACHSET_HIPRS_LIMIT_MAX);
        }
        else
        {
            valueClsHiPrs = VarAdrToUI16(d_machine1_MACHSET_DAPRES_SYSLMT);
        }
    }
    else
    {
        valueClsHiPrs = VarAdrToUI16(d_machine1_MACHSET_DAPRES_SYSLMT);
    }

    if(VarAdrToUI16(d_autoctrl1_MOLDSET_DAHIPRES_CLPLIMIT)>valueClsHiPrs)
    {
        VarAdrSetInt(d_autoctrl1_MOLDSET_DAHIPRES_CLPLIMIT,valueClsHiPrs);
        VarAdrSaveInt(d_autoctrl1_MOLDSET_DAHIPRES_CLPLIMIT);
    }

    VarAdrSetInt(p_PP_MACHSET_CLSHIPRS_LIMIT_MAX_REAL,valueClsHiPrs);
    //关模高压压力上限end

    //储料背压压力上限start
    int valueChgBkPrs = VarAdrToUI16(d_machine1_MACHSET_DAPRES_SYSLMT);
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00)
    {
        if(VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE) == 1)
        {
            valueChgBkPrs = VarAdrToUI16(p_PP_MACHSET_CHGBKPRS_LIMIT_MAX);
        }
        else
        {
            valueChgBkPrs = VarAdrToUI16(d_machine1_MACHSET_DAPRES_SYSLMT);
        }
    }
    else
    {
        valueChgBkPrs = VarAdrToUI16(d_machine1_MACHSET_DAPRES_SYSLMT);
    }

    if(VarAdrToUI16(d_autoctrl1_MOLDSET_DAPRES_BACKLIMIT)>valueChgBkPrs)
    {
        VarAdrSetInt(d_autoctrl1_MOLDSET_DAPRES_BACKLIMIT,valueChgBkPrs);
        VarAdrSaveInt(d_autoctrl1_MOLDSET_DAPRES_BACKLIMIT);
    }

    VarAdrSetInt(p_PP_MACHSET_CHGBKPRS_LIMIT_MAX_REAL,valueChgBkPrs);
    //储料背压压力上限end

    //储料速度上限--start
    int valueChgSpd = 99;
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00)
    {
        if(VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE) == 1)
        {
            valueChgSpd = VarAdrToUI16(p_PP_MACHSET_CHGSPD_LIMIT_MAX);
        }
        else
        {
            valueChgSpd = 99;
        }
    }
    else
    {
        valueChgSpd = 99;
    }

    if(VarAdrToUI16(d_autoctrl1_MOLDSET_DAFLOW_CHARGELIMIT)>valueChgSpd)
    {
        VarAdrSetInt(d_autoctrl1_MOLDSET_DAFLOW_CHARGELIMIT,valueChgSpd);
        VarAdrSaveInt(d_autoctrl1_MOLDSET_DAFLOW_CHARGELIMIT);
    }

    VarAdrSetInt(p_PP_MACHSET_CHARGE_SPD_LIMIT_MAX_REAL,valueChgSpd);
    //储料速度上限--end
    //20241017 chj 威力士增加压力上限限制 end
}
static void CtlConnect()
{
}
static int Sys_ParaLimit_SpcOnInit(Sys_ParaLimit_Spc_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_ParaLimit_SpcOnShow(Sys_ParaLimit_Spc_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(TMP_CALIBRATE_PASSWORD,0);
    VarAdrSetInt(TMP_ENABLE,0);
    if(VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE) == 1) //20250109 fqh 快速机时开放威力士密码内容
    {
        VarAdrSetInt(TMP_SHOW,1);
    }
    else
    {
        VarAdrSetInt(TMP_SHOW,0);
    }
    SetLimit();
    return 0;
}
static int Sys_ParaLimit_SpcOnHide(Sys_ParaLimit_Spc_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_ParaLimit_SpcOnUpdate(Sys_ParaLimit_Spc_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_ParaLimit_SpcOnChange(Sys_ParaLimit_Spc_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
   UI32 addr;
   UI32 month,day;
   struct tm* t;
   time_t tm_now;

   time(&tm_now);
   t = localtime(&tm_now);
   month=t->tm_mon+1;
   day=t->tm_mday;
   PATTR pattr;

   pattr = (PATTR)msg_para;

   addr = OBJAttrGetAdr(pattr);
   UI16 value;
   value = VarAdrToUI16(addr);

   switch(addr)
   {
       case TMP_CALIBRATE_PASSWORD:
           if(VarAdrToInt(TMP_CALIBRATE_PASSWORD) == month*1000+day*10)
           {
               VarAdrSetInt(TMP_ENABLE,1);
           }
           else
           {
               VarAdrSetInt(TMP_ENABLE,0);
           }
            break;
       case p_PP_MACHSET_WLSMACHINE_TYPE://20241226 chj 威力士快速机模式下，部分参数变灰：关模高压二段,储料移动检测，储前电子尺检测，漏油检测功能数据赋值
           if(value == 1)
           {
                WLS_Fast_Spc();
                VarAdrSetInt(TMP_SHOW,1);
           }
           else
           {
                VarAdrSetInt(TMP_SHOW,0);
           }

           SetLimit();
            break;
       case p_PP_MACHSET_HIPRS_LIMIT_MAX:
       case p_PP_MACHSET_CHGBKPRS_LIMIT_MAX:
       case p_PP_MACHSET_CHGSPD_LIMIT_MAX:
            SetLimit();
       break;
       default:
            break;
   }
   return 0;
}
