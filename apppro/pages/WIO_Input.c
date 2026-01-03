#include "WIO_Input.h"
#include "typedef.h"
#include "pagefram.h"
#include "oprintf.h"
#include "protcmd.h"
#include "tablectl.h"
#include "mod.h"
#include "panel.h"
#include "ioredef.h"
#include "usermod.h"
#include "checkboxwgt.h"
#include "texttbl.h"
#include "privdef.h"
#include "database.h"
#include "stdio.h"
#include "ioredef.h"
#include "kjump.h"
#include "verinfo.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_IO_INPUT_MAP

CONNECT_IO_INPUT_EVENT

#define TMP_CKBX_WIDTH  TMPVAL(0) //复选框宽度
#define TMP_NAME0		TMPSTR(51)
#define TMP_COLOR0		TMPVAL(300)
#define SN_START		0

#define TMP_CHECK0		TMPVAL(51)//20190514.cyx
#define MASK_BIT0       0x01
#define CHECKBOX_NAME(NUM,pbuff,size)		joinStrAndNum("cb",NUM,pbuff,size)

static UI16 s_wIONum = 2;

static void IO_InputShow()
{
    PWGT pwgt;
    UI16 i;
    char buff[10];

    if(IV5000 || IV5200 || IV5300)
    {
        s_wIONum = 3;
        if(IsG15Type())
        {
            VarAdrSetInt(TMP_CKBX_WIDTH,236);
        }
        else
        {
            VarAdrSetInt(TMP_CKBX_WIDTH,185);
        }
    }
    else
    {
        s_wIONum = 2;
        if(IsG15Type())
        {
            VarAdrSetInt(TMP_CKBX_WIDTH,300);
        }
        else
        {
            VarAdrSetInt(TMP_CKBX_WIDTH,225);
        }
    }

    if(s_wIONum == 2)
    {
        for (i =  1; i <= 16; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOX_NAME(i,buff,10));
            if(IsG15Type())
            {
                WI32(pwgt->x_pos,38);
            }
            else
            {
                WI32(pwgt->x_pos,30);
            }
        }

        for (i = 17; i <= 32; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOX_NAME(i,buff,10));
            if(IsG15Type())
            {
                WI32(pwgt->x_pos,384);
            }
            else
            {
                WI32(pwgt->x_pos,300);
            }
        }

        for (i = 33; i <= 48; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOX_NAME(i,buff,10));
            WGTSetVisible(pwgt, FALSE);
        }
    }
    else if(s_wIONum == 3)
    {
        for (i =  1; i <= 16; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOX_NAME(i,buff,10));
            if(IsG15Type())
            {
                WI32(pwgt->x_pos,25);
            }
            else
            {
                WI32(pwgt->x_pos,20);
            }
        }

        for (i = 17; i <= 32; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOX_NAME(i,buff,10));
            if(IsG15Type())
            {
                WI32(pwgt->x_pos,266);
            }
            else
            {
                WI32(pwgt->x_pos,208);
            }
        }

        for (i = 33; i <= 48; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOX_NAME(i,buff,10));
            WGTSetVisible(pwgt, TRUE);
        }
    }
}

static void Input__UpdateNames(PPROT_TBL_HD tbl_hd)
{
    int i,j;
    char strid[255];
    UI16 checkstate;
    TBL_INPUT* pin;

    for(i = 0; i<s_wIONum; i++)
    {
        for(j = 0; j < 16; ++j)
        {
            if(tbl_hd && (pin = TableFindInput(tbl_hd, 16*i+j+SN_START+1)))
            {
                UI32 logic_addr = (((UI32)pin->part_id+1) << 16) | pin->ind;

                if(pmap_input != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d  %s",16*i+j+1,GetMapTran(pmap_input,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d  ",16*i+j+1);
                }
                if(GetIPoint(16*i+j))//反逻辑
                {
                    strcat(strid,"(");
                    strcat(strid,GetTextTran(TEXT_ANTI_LOGIC));
                    strcat(strid,")");
                }

                VarAdrSetStr(TMP_NAME0+16*i+j, strid);
                //ODEBUG("Find input sn=%d, addr=%x:", 16*i+j+SN_START+1, logic_addr);
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d  ",16*i+j+1);
                VarAdrSetStr(TMP_NAME0+16*i+j, strid);
            }

            if(GetUseForceRec(16*i+j))//输入点逻辑判断，强制激活设置或物理状态
            {
                VarAdrSetInt(TMP_CHECK0+i*16+j,!GetForceInputRec(16*i+j));
                VarAdrSetInt(TMP_COLOR0+16*i+j,GUI_RED);
            }
            else
            {
                checkstate = VarAdrToInt(PI_INPUT1+i)>>j & MASK_BIT0;
                VarAdrSetInt(TMP_CHECK0+i*16+j,checkstate);
                VarAdrSetInt(TMP_COLOR0+16*i+j,GUI_BLACK);
            }
        }
    }

    if((GetCurrentUserId() >= USER_MANUFACTURER) && (IV3200 || IV3300 || IV5000 || IV5200 || IV5300 || IV3100))
    {
        WGTSetVisible(io_input_pg.lblinfo,FALSE);
    }
    else
    {
        WGTSetVisible(io_input_pg.lblinfo,TRUE);
    }
}

static int button1OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(io_input_pg.button1, WGT_CLICK_MSG, button1OnClick);
}
static int IO_InputOnInit(IO_Input_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int IO_InputOnShow(IO_Input_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USER_MODELOCK);//20210914 dyl touch

    G10IOInputGuideSwitch();
    IO_InputShow();//20190514.cyx 显示32或48个输入点
    Input__UpdateNames(GetTableByType(TB_I_LOGIC, TB_CUR));

    return 0;
}
static int IO_InputOnHide(IO_Input_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int IO_InputOnUpdate(IO_Input_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    Input__UpdateNames(GetTableByType(TB_I_LOGIC, TB_CUR));

    return 0;
}
static int IO_InputOnChange(IO_Input_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 goff;

    goff = WGTGetGOffset(pwgt);

    if(goff <= 48 && goff >=1)
    {
        if((GetCurrentUserId() >= USER_MANUFACTURER) && (IV3200 || IV3300 || IV5000 || IV5200 || IV5300 || IV3100))
        {
            SetCurrentIoNum(goff-1);
            PanelShowDialogByName(DIALOG_IO_FORCE);
            return TRUE;
        }
    }

    return 0;
}
static int button1OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ClearFoceInPoint();
    return 0;
}
