#include "WIO_Output_Ex.h"
#include "typedef.h"
#include "pagefram.h"
#include "protcmd.h"
#include "tablectl.h"
#include "mod.h"
#include "oprintf.h"
#include "database.h"
#include "ctrlcmd.h"
#include "questionDlg.h"
#include "information.h"
#include "checkboxwgt.h"
#include "stdio.h"
#include "ioredef.h"
#include "pagefram.h"
#include "keymod.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_IO_OUTPUT_EX_MAP

CONNECT_IO_OUTPUT_EX_EVENT

#define TMP_NAME0		TMPSTR(41)
#define TMP_CHECK0		TMPVAL(41)
#define PO_OUTPUT5      0x0616000E //输出点

#define OUT_VAL_CNT 8
#define SN_START	64
#define SN_COUNT	32
#define MASK_BIT0   0x01

UI16 m_forceValsx[OUT_VAL_CNT];

//20221111 dyl 当按下急停或通讯断开时，清除强制输出
extern UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
static BOOL bForceOutput_zero=FALSE;

extern BOOL b_RecMainNeedPO_TIMEOUT_EXIT_CMD; //IO强制输出超时，通知面板清除IO强制输出表 20250305 chj

static int GetBit1Num(UI8 value)
{
    int cnt1=0;
    for (cnt1 =0; value; ++cnt1)
    {
        value &= (value -1) ; // 清除最低位的1
    }
    return cnt1;
}

static void ForceOutput(int sn)
{
    UI16 data_tmp, bit_mask,npc;
    UI16* pdata;

    npc = sn/16;
    pdata = m_forceValsx + npc;
    bit_mask = 1 << sn%16;
    data_tmp = *pdata;

    //每8个点只能输出4个
    if(data_tmp & bit_mask)
    {
        *pdata &= ~bit_mask;
    }
    else
    {
        if(bit_mask >= 0x100)
        {
            data_tmp >>= 8;
        }
        if(GetBit1Num(data_tmp & 0xFF) < 4)
        {
            *pdata |= bit_mask;
        }
    }

    m_forceValsx[npc] = *pdata;

    VarAdrSetInt(PO_OUTPUT1+npc,*pdata);
    ProtSysCtrl(FORCE_PO_CMD, OUT_VAL_CNT, m_forceValsx);

    if(!m_isInForce)
    {
        m_isInForce = TRUE;
    }
}

static char* GetExNum(int index,char buff[])
{
    if((index%16) < 9)
    {
        sprintf(buff,"%d%d",0,index%16+1);
    }
    else
    {
        sprintf(buff,"%d",index%16+1);
    }

    return buff;
}

static void Output__UpdateNames(PPROT_TBL_HD tbl_hd)
{
    int i;
    char buff[10];
    char strid[255];
    UI16 checkstate;
    TBL_OUTPUT* pout;

    //20230518 chj  根据code码显示
    for(i = 0; i < (SN_COUNT/2); ++i)
    {
        if(VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x200)
        {
            if(tbl_hd && (pout = TableFindOutput(tbl_hd, i+SN_START+1)))
            {
                UI32 logic_addr = ((UI32)pout->part_id << 16) | pout->ind;

                if(pmap_output != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  %s",65+i,GetExNum(i,buff),GetMapTran(pmap_output,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",65+i,GetExNum(i,buff));
                }

                VarAdrSetStr(TMP_NAME0+i, strid);
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",65+i,GetExNum(i,buff));
                VarAdrSetStr(TMP_NAME0+i, strid);
            }
            checkstate = VarAdrToInt(PO_OUTPUT5+(i/16))>>(i%16) & MASK_BIT0;
            VarAdrSetInt(TMP_CHECK0+i,checkstate);
        }
        else
        {
            VarAdrSetInt(TMP_CHECK0+i,0);
            snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",65+i,GetExNum(i,buff));
            VarAdrSetStr(TMP_NAME0+i, strid);
        }


    }

    for(i = (SN_COUNT/2); i < SN_COUNT; ++i)
    {
        if(VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x2000)
        {
            if(tbl_hd && (pout = TableFindOutput(tbl_hd, i+SN_START+1)))
            {
                UI32 logic_addr = ((UI32)pout->part_id << 16) | pout->ind;

                if(pmap_output != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  %s",65+i,GetExNum(i,buff),GetMapTran(pmap_output,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",65+i,GetExNum(i,buff));
                }

                VarAdrSetStr(TMP_NAME0+i, strid);
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",65+i,GetExNum(i,buff));
                VarAdrSetStr(TMP_NAME0+i, strid);
            }
            checkstate = VarAdrToInt(PO_OUTPUT5+(i/16))>>(i%16) & MASK_BIT0;
            VarAdrSetInt(TMP_CHECK0+i,checkstate);
        }
        else
        {
            VarAdrSetInt(TMP_CHECK0+i,0);
            snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",65+i,GetExNum(i,buff));
            VarAdrSetStr(TMP_NAME0+i, strid);
        }


    }

}

void OutputEx_ForceZero()
{
    memset(m_forceValsx, 0, sizeof(m_forceValsx));
}

static void CtlConnect()
{
}
static int IO_Output_ExOnInit(IO_Output_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetIOOut2ZeroCB(OutputEx_ForceZero);
    return 0;
}
static int IO_Output_ExOnShow(IO_Output_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USER_MODELOCK);//20210914 dyl touch

    m_isInForce = FALSE;
    bForceOutput_zero=FALSE;//20221111 dyl 当按下急停或通讯断开时，清除强制输出
    memset(m_forceValsx, 0, sizeof(m_forceValsx));

    return 0;
}
static int IO_Output_ExOnHide(IO_Output_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    if(m_isInForce)
    {
        ProtSysCtrl(EXIT_PO_CMD, 0, NULL);
    }
    m_isInForce = FALSE;
    bForceOutput_zero=FALSE;//20221111 dyl 当按下急停或通讯断开时，清除强制输出

    return 0;
}
static int IO_Output_ExOnUpdate(IO_Output_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    Output__UpdateNames(GetTableByType(TB_O_LOGIC, TB_CUR));

    //20221111 dyl 当按下急停或通讯断开时，清除强制输出
    UI32 dwWarn[10]={g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10};
    int i;
    if (m_isInForce)
    {
        for(i=0;i<10;i++)
        {
            if((dwWarn[i] == 0x100101) || (dwWarn[i] == 0x33100101))
            {
                bForceOutput_zero = TRUE;
                break;
            }
        }
        if(b_RecMainNeedPO_TIMEOUT_EXIT_CMD)//IO强制输出超时，通知面板清除IO强制输出表 20250305 chj
        {
            bForceOutput_zero = TRUE;
            b_RecMainNeedPO_TIMEOUT_EXIT_CMD = FALSE;
        }
    }
    if (bForceOutput_zero)
    {
        if(m_isInForce)
        {
            ProtSysCtrl(EXIT_PO_CMD, 0, NULL);

            //force out to zero
            memset(m_forceValsx, 0, sizeof(m_forceValsx));
            m_isInForce = FALSE;
            bForceOutput_zero = FALSE;
        }
    }

    return 0;
}
static int IO_Output_ExOnChange(IO_Output_Ex_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int index = WGTGetGOffset(pwgt);
    if(index)
    {
        if(VarAdrToInt(MOTOR_STATE_STATE) != 0 || VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS) != 0)//20190516.cyx
        {
            CheckboxSetCheck(pwgt,!CheckboxGetCheck(pwgt));
            ShowMsg(VW_MSG_CLSHEATERANDMOTOR);
            return -1;
        }
        else
        {
            ForceOutput(index+64-1);
        }
    }

    return 0;
}
