#include "WKey_Define.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "keydefine.h"
#include "editwgt.h"
#include "panelcfg.h"

DEFINE_KEY_DEFINE_MAP

CONNECT_KEY_DEFINE_EVENT

#define SPARE_KEY_NUM 2//总备用键数量

#define TMP_SENDKEY1 TMPVAL(1)//备用1发送键值
#define TMP_SENDKEY2 TMPVAL(2)//备用2发送键值

extern UI16 g_airkey_active;
extern UI16 g_corekey_active;

static void CtlConnect()
{
}
static int Key_DefineOnInit(Key_Define_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Key_DefineOnShow(Key_Define_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;
    PWGT pwgt_sendkey;

    for(i = 0; i < SPARE_KEY_NUM; i++)
    {
        VarAdrSetInt(TMP_SENDKEY1+i, KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+i), i));

        pwgt_sendkey = PageFramCurWgtByIndex(i+1);
        if(pwgt_sendkey != NULL)
        {
            if(VarAdrToInt(p_PP_MACHSET_SPARE1_KEY + i) == KEY_SELFDEFINE)
            {
                EditSetReadOnly(pwgt_sendkey, FALSE);
                EditSetBKColor(pwgt_sendkey, 0xFFFFFF);
                EditSetTextColor(pwgt_sendkey, 0x000000);
            }
            else
            {
                EditSetReadOnly(pwgt_sendkey, TRUE);
                EditSetBKColor(pwgt_sendkey, 0xdedede);
                EditSetTextColor(pwgt_sendkey, 0x000000);
            }
        }
    }

    //20241218 chj 如果备用按键被使用了，进行变灰处理
    //金鹰快速机模式下，按键自定义不可用
    if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400))
    {
        WGTSetVisible(ppg->labelinfo1,FALSE);
        WGTSetVisible(ppg->labelinfo2,FALSE);

        WGTSetEnable(ppg->dropexcel1,FALSE);
        WGTSetEnable(ppg->edit35,FALSE);

        WGTSetEnable(ppg->dropexcel2,FALSE);
        WGTSetEnable(ppg->edit36,FALSE);
    }
    else
    {
        if(VarAdrToInt(d_autoctrl1_MOLDSET_KEY_LUBR2_INDEPEND))
        {
            WGTSetEnable(ppg->dropexcel1,FALSE);
            WGTSetEnable(ppg->edit35,FALSE);
            WGTSetVisible(ppg->labelinfo1,TRUE);
        }
        else
        {
            WGTSetEnable(ppg->dropexcel1,TRUE);
            WGTSetEnable(ppg->edit35,TRUE);
            WGTSetVisible(ppg->labelinfo1,FALSE);
        }

        if(VarAdrToInt(p_PP_MACHSET_MT_BOARD_COUNT))
        {
            WGTSetEnable(ppg->dropexcel2,FALSE);
            WGTSetEnable(ppg->edit36,FALSE);
            WGTSetVisible(ppg->labelinfo2,TRUE);
        }
        else
        {
            WGTSetEnable(ppg->dropexcel2,TRUE);
            WGTSetEnable(ppg->edit36,TRUE);
            WGTSetVisible(ppg->labelinfo2,FALSE);
        }
    }

    return 0;
}
static int Key_DefineOnHide(Key_Define_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Key_DefineOnUpdate(Key_Define_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    for(i = 0; i < SPARE_KEY_NUM; i++)
    {
        if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+i) == KEY_COREOUT)
        {
            if(g_corekey_active == 0)
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0);
            }
            else
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0x2212 + ((g_corekey_active-1)<<4));
            }
        }
        else if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+i) == KEY_COREIN)
        {
            if(g_corekey_active == 0)
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0);
            }
            else
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0x2211 + ((g_corekey_active-1)<<4));
            }
        }
        else if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+i) == KEY_AIRBLOW135)
        {
            if(g_airkey_active == 0)
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0);
            }
            else
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0x2311 + (((g_airkey_active-1)*2)<<4));
            }
        }
        else if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+i) == KEY_AIRBLOW246)
        {
            if(g_airkey_active == 0)
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0);
            }
            else
            {
                VarAdrSetInt(TMP_SENDKEY1+i, 0x2321 + (((g_airkey_active-1)*2)<<4));
            }
        }
    }

    return 0;
}
static int Key_DefineOnChange(Key_Define_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,nId;
    PATTR pattr;
    PWGT pwgt_sendkey;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch(addr)
    {
    case p_PP_MACHSET_SPARE1_KEY:
    case p_PP_MACHSET_SPARE2_KEY:
        nId = addr - p_PP_MACHSET_SPARE1_KEY;
        VarAdrSetInt(TMP_SENDKEY1+nId, KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+nId), nId));

        pwgt_sendkey = PageFramCurWgtByIndex(nId+1);
        if(pwgt_sendkey != NULL)
        {
            if(VarAdrToInt(p_PP_MACHSET_SPARE1_KEY + nId) == KEY_SELFDEFINE)
            {
                EditSetReadOnly(pwgt_sendkey, FALSE);
                EditSetBKColor(pwgt_sendkey, 0xFFFFFF);
                EditSetTextColor(pwgt_sendkey, 0x000000);
            }
            else
            {
                EditSetReadOnly(pwgt_sendkey, TRUE);
                EditSetBKColor(pwgt_sendkey, 0xdedede);
                EditSetTextColor(pwgt_sendkey, 0x000000);
            }
        }
        break;
    case TMP_SENDKEY1:
    case TMP_SENDKEY2:
        nId = addr - TMP_SENDKEY1;
        VarAdrSetInt(p_PP_MACHSET_SPARE1_VALUE+nId, VarAdrToInt(TMP_SENDKEY1+nId));
        VarAdrSaveInt(p_PP_MACHSET_SPARE1_VALUE+nId);
    default:
        break;
    }
    return 0;
}
