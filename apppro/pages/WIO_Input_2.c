#include "WIO_Input_2.h"
#include "typedef.h"
#include "pagefram.h"
#include "oprintf.h"
#include "protcmd.h"
#include "tablectl.h"
#include "mod.h"
#include "database.h"
#include "usermod.h"
#include "checkboxwgt.h"
#include "texttbl.h"
#include "privdef.h"
#include "ioredef.h"
#include "stdio.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_IO_INPUT_2_MAP

CONNECT_IO_INPUT_2_EVENT

#define SN_COUNT		32
#define IO_STATE_pi_input3       0x6160004

#define TMP_NAME0		TMPSTR(41)
#define TMP_COLOR0		TMPVAL(300)
#define TMP_CHECK0		TMPVAL(41)//20190514.cyx
#define MASK_BIT0       0x01

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

static UI32 GetSnNum(int index)
{
    if(IV5000 || IV5200 || IV5300)
    {
        return (index+49);
    }
    else
    {
        return (index+33);
    }
}

static void Input__UpdateNames(PPROT_TBL_HD tbl_hd)
{
    int i;
    char buff[10];
    char strid[255];
    UI16 checkstate;
    TBL_INPUT* pin;

    for(i = 0; i < 16; ++i)
    {
        if(VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x200)
        {
            if(tbl_hd && (pin = TableFindInput(tbl_hd, i+GetSnNum(0))))
            {
                UI32 logic_addr = (((UI32)pin->part_id+1) << 16) | pin->ind;

                if(pmap_input != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  %s",GetSnNum(i),GetExNum(i,buff),GetMapTran(pmap_input,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",GetSnNum(i),GetExNum(i,buff));
                }
                if(GetIPoint(GetSnNum(0)-1+i))//反逻辑
                {
                    strcat(strid,"(");
                    strcat(strid,GetTextTran(TEXT_ANTI_LOGIC));
                    strcat(strid,")");
                }

                VarAdrSetStr(TMP_NAME0+i, strid);
                //ODEBUG("Find input sn=%d, addr=%x:", i+SN_START+1, logic_addr);
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",GetSnNum(i),GetExNum(i,buff));
                VarAdrSetStr(TMP_NAME0+i, strid);
            }

            if(GetUseForceRec(GetSnNum(0)-1+i))//输入点逻辑判断，强制激活设置或物理状态
            {
                VarAdrSetInt(TMP_CHECK0+i,!GetForceInputRec(GetSnNum(0)-1+i));
                VarAdrSetInt(TMP_COLOR0+i,GUI_RED);
            }
            else
            {
                if(IV5000 || IV5200 || IV5300)
                {
                    checkstate = VarAdrToInt(IO_STATE_pi_input3+1+(i/16))>>(i%16) & MASK_BIT0;
                }
                else
                {
                    checkstate = VarAdrToInt(IO_STATE_pi_input3+(i/16))>>(i%16) & MASK_BIT0;
                }

                VarAdrSetInt(TMP_CHECK0+i,checkstate);
                VarAdrSetInt(TMP_COLOR0+i,GUI_BLACK);
            }
        }
        else
        {
            VarAdrSetInt(TMP_CHECK0+i,0);//20190517.cyx
            VarAdrSetInt(TMP_COLOR0+i,GUI_BLACK);//20190517.cyx
            snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",GetSnNum(i),GetExNum(i,buff));
            VarAdrSetStr(TMP_NAME0+i, strid);
        }
    }

    for(i = 16; i < SN_COUNT; ++i)
    {
        if(VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x2000)
        {
            if(tbl_hd && (pin = TableFindInput(tbl_hd, i+GetSnNum(0))))
            {
                UI32 logic_addr = (((UI32)pin->part_id+1) << 16) | pin->ind;

                if(pmap_input != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  %s",GetSnNum(i),GetExNum(i,buff),GetMapTran(pmap_input,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",GetSnNum(i),GetExNum(i,buff));
                }
                if(GetIPoint(GetSnNum(0)-1+i))//反逻辑
                {
                    strcat(strid,"(");
                    strcat(strid,GetTextTran(TEXT_ANTI_LOGIC));
                    strcat(strid,")");
                }

                VarAdrSetStr(TMP_NAME0+i, strid);
                //ODEBUG("Find input sn=%d, addr=%x:", i+SN_START+1, logic_addr);
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",GetSnNum(i),GetExNum(i,buff));
                VarAdrSetStr(TMP_NAME0+i, strid);
            }

            if(GetUseForceRec(GetSnNum(0)-1+i))//输入点逻辑判断，强制激活设置或物理状态
            {
                VarAdrSetInt(TMP_CHECK0+i,!GetForceInputRec(GetSnNum(0)-1+i));
                VarAdrSetInt(TMP_COLOR0+i,GUI_RED);
            }
            else
            {
                if(IV5000 || IV5200 || IV5300)
                {
                    checkstate = VarAdrToInt(IO_STATE_pi_input3+1+(i/16))>>(i%16) & MASK_BIT0;
                }
                else
                {
                    checkstate = VarAdrToInt(IO_STATE_pi_input3+(i/16))>>(i%16) & MASK_BIT0;
                }

                VarAdrSetInt(TMP_CHECK0+i,checkstate);
                VarAdrSetInt(TMP_COLOR0+i,GUI_BLACK);
            }
        }
        else
        {
            VarAdrSetInt(TMP_CHECK0+i,0);//20190517.cyx
            VarAdrSetInt(TMP_COLOR0+i,GUI_BLACK);//20190517.cyx
            snprintf(strid,sizeof(strid)-1,"%d(E%s)  ",GetSnNum(i),GetExNum(i,buff));
            VarAdrSetStr(TMP_NAME0+i, strid);
        }
    }

    if((GetCurrentUserId() >= USER_MANUFACTURER) && (IV3200 || IV3300 || IV5000 || IV5200 || IV5300 || IV3100))
    {
        WGTSetVisible(io_input_2_pg.lblinfo,FALSE);
    }
    else
    {
        WGTSetVisible(io_input_2_pg.lblinfo,TRUE);
    }
}

static int button1OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(io_input_2_pg.button1, WGT_CLICK_MSG, button1OnClick);
}
static int IO_Input_2OnInit(IO_Input_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int IO_Input_2OnShow(IO_Input_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USER_MODELOCK);//20210914 dyl touch
    return 0;
}
static int IO_Input_2OnHide(IO_Input_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int IO_Input_2OnUpdate(IO_Input_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    Input__UpdateNames(GetTableByType(TB_I_LOGIC, TB_CUR));
    return 0;
}
static int IO_Input_2OnChange(IO_Input_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 goff;

    goff = WGTGetGOffset(pwgt);

    if(goff <= 32 && goff >=1)
    {
        if((GetCurrentUserId() >= USER_MANUFACTURER) && (IV3200 || IV3300 || IV5000 || IV5200 || IV5300 || IV3100))
        {
            if((VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x200) && (goff <= 16))
            {
                SetCurrentIoNum(GetSnNum(goff-1)-1);
                PanelShowDialogByName(DIALOG_IO_FORCE);
                return TRUE;
            }
            else if(VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x2000)
            {
                SetCurrentIoNum(GetSnNum(goff-1)-1);
                PanelShowDialogByName(DIALOG_IO_FORCE);
                return TRUE;
            }
        }
    }

    return 0;
}
static int button1OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ClearFoceInPoint();
    return 0;
}
