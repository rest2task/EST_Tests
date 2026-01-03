#include "WIO_Output.h"
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

DEFINE_IO_OUTPUT_MAP

CONNECT_IO_OUTPUT_EVENT

#define TMP_NAME0		TMPSTR(51)
#define TMP_CHECK0		TMPVAL(51)//20190514.cyx
#define TMP_NAME49		TMPSTR(10)
#define TMP_CHECK49		TMPVAL(110)
#define MASK_BIT0       0x01

#define OUT_VAL_CNT 8
#define SN_START	0
#define SN_COUNT	48
#define SN_IV5000_ADD_COUNT	8

#define CKBK1_OFFSET 1 //subpage2第一个控件的索引值
#define CKBK1_5000_OFFSET 101 //subpage1第一个控件的索引值

UI16 m_forceVals[OUT_VAL_CNT];
static int m_index = 1;

//20221111 dyl 当按下急停或通讯断开时，清除强制输出
extern UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
static BOOL bForceOutput_zero=FALSE;

static BOOL page_in = TRUE;

extern BOOL b_RecMainNeedPO_TIMEOUT_EXIT_CMD; //IO强制输出超时，通知面板清除IO强制输出表 20250305 chj

static void IO_OutputShow()
{
    if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5000)
    {
        WGTSetVisible(io_output_pg.subpage1,TRUE);
        WGTSetVisible(io_output_pg.subpage2,FALSE);
        PageWgtSetFocus(PageFramCurWgtByIndex(CKBK1_5000_OFFSET));
    }
    else
    {
        WGTSetVisible(io_output_pg.subpage1,FALSE);
        WGTSetVisible(io_output_pg.subpage2,TRUE);
        PageWgtSetFocus(PageFramCurWgtByIndex(CKBK1_OFFSET));
    }
}

static int GetBit1Num(UI8 valuex)
{
    int cnt1=0;
    for (cnt1 =0; valuex; ++cnt1)
    {
        valuex &= (valuex -1) ; // 清除最低位的1
    }
    return cnt1;
}

static void ForceOutput(int sn)
{
    UI16 data_tmp, bit_mask,npc;
    UI16* pdata;

    npc = sn/16;
    pdata = m_forceVals + npc;
    bit_mask = 1 << sn%16;
    data_tmp = *pdata;

    //每8个点只能输出4个
    if(data_tmp & bit_mask)
    {
        *pdata &= ~bit_mask;
        ODEBUG("pdata=%d\n", *pdata);
    }
    else
    {
        if(bit_mask >= 0x100)//0x100
        {
            data_tmp >>= 8;
        }
        if(GetBit1Num(data_tmp & 0xFF) < 4)
        {
            *pdata |= bit_mask;
        }
        ODEBUG("m_forceVals=%x\n", *pdata);
    }

    m_forceVals[npc] = *pdata;

    VarAdrSetInt(PO_OUTPUT1+npc,*pdata);
    ProtSysCtrl(FORCE_PO_CMD, OUT_VAL_CNT, m_forceVals);

    if(!m_isInForce)
    {
        m_isInForce = TRUE;
    }
}

static void Output__UpdateNames(PPROT_TBL_HD tbl_hd)
{
    int i;
    char strid[255];
    UI16 checkstate;
    TBL_OUTPUT* pout;
    for(i = 0; i < SN_COUNT; ++i)
    {
        if(tbl_hd && (pout = TableFindOutput(tbl_hd, i+SN_START+1)))
        {
            UI32 logic_addr = ((UI32)pout->part_id << 16) | pout->ind;

            if(pmap_output != NULL)
            {
                snprintf(strid,sizeof(strid)-1,"%d  %s",i+1,GetMapTran(pmap_output,logic_addr));
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d  ",i+1);
            }

            VarAdrSetStr(TMP_NAME0+i, strid);

            //ODEBUG("Find output sn=%d, addr=%x:", i+SN_START+1, logic_addr);
        }
        else
        {
            snprintf(strid,sizeof(strid)-1,"%d  ",i+1);
            VarAdrSetStr(TMP_NAME0+i, strid);
        }

        checkstate = VarAdrToInt(PO_OUTPUT1+(i/16))>>(i%16) & MASK_BIT0;
        VarAdrSetInt(TMP_CHECK0+i,checkstate);
    }

    if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5000)
    {
        for(i = 0; i < SN_IV5000_ADD_COUNT; ++i)
        {
            if(tbl_hd && (pout = TableFindOutput(tbl_hd, i+SN_START+1+SN_COUNT)))
            {
                UI32 logic_addr = ((UI32)pout->part_id << 16) | pout->ind;

                if(pmap_output != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d  %s",i+49,GetMapTran(pmap_output,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d  ",i+49);
                }

                VarAdrSetStr(TMP_NAME49+i, strid);

                //ODEBUG("Find output sn=%d, addr=%x:", i+SN_START+1, logic_addr);
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d  ",i+49);
                VarAdrSetStr(TMP_NAME49+i, strid);
            }
            checkstate = VarAdrToInt(PO_OUTPUT1+3)>>(i%16) & MASK_BIT0;
            VarAdrSetInt(TMP_CHECK49+i,checkstate);
        }
    }
}

void ForceZero()
{
    memset(m_forceVals, 0, sizeof(m_forceVals));
}

static void CtlConnect()
{
}
static int IO_OutputOnInit(IO_Output_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetIOOutZeroCB(ForceZero); //20190510 hz
    return 0;
}
static int IO_OutputOnShow(IO_Output_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USER_MODELOCK);//20210914 dyl touch

    page_in = TRUE;
    m_isInForce = FALSE;
    bForceOutput_zero=FALSE;//20221111 dyl 当按下急停或通讯断开时，清除强制输出
    memset(m_forceVals, 0, sizeof(m_forceVals));

    return 0;
}
static int IO_OutputOnHide(IO_Output_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
static int IO_OutputOnUpdate(IO_Output_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    if(page_in)
    {
        IO_OutputShow();
        page_in = FALSE;
    }

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
            memset(m_forceVals, 0, sizeof(m_forceVals));
            m_isInForce = FALSE;
            bForceOutput_zero = FALSE;
        }
    }

    return 0;
}
static int IO_OutputOnChange(IO_Output_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 index = WGTGetGOffset(pwgt);

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
            if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5000)
            {
                m_index = index - CKBK1_5000_OFFSET;
            }
            else
            {
                m_index = index - CKBK1_OFFSET;
            }

            ForceOutput(m_index);
        }
    }
    return 0;
}
