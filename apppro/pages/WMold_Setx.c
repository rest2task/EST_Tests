#include "WMold_Setx.h"
#include "typedef.h"
#include "pagefram.h"
#include "moldset.h"
#include "mod.h"
#include "oprintf.h"
#include "database.h"
#include "time.h"
#include "stdio.h"
#include "privdef.h"
#include "information.h"
#include "opencal.h"
#include "panelrec.h"
#include "questionDlg.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_MOLD_SETX_MAP

CONNECT_MOLD_SETX_EVENT

enum GOFFSET
{
    ID_READ		= 1,
    ID_READ_END	= 15,

    ID_SAVE		= 16,
    ID_SAVE_END	= 30,

    ID_DEL		= 31,
    ID_DEL_END	= 45,

    ID_SEL		= 46,
    ID_SEL_END	= 60,

    ID_IMPORT = 61,
    ID_EXPORT = 62,

    ID_ZH_START = 71,
    ID_ZH_END = 115
};

#define TMP_STARTNO		TMPVAL(0)
#define TMP_CURNO		TMPVAL(1)
#define TMP_SAVECNT		TMPSTR(99)
#define TMP_CURNAME		TMPSTR(0)
#define TMP_SELALL		TMPVAL(3)
//offset 0-9
#define TMP_INDEX0		TMPVAL(11)
#define TMP_NAME0		TMPSTR(1)
#define TMP_MATER0		TMPSTR(16)
#define TMP_COLOR0		TMPSTR(31)
#define TMP_SAVEDATE0	TMPSTR(46)
#define TMP_READ_EN0	TMPVAL(31)
#define TMP_SEL0		TMPVAL(46)
#define TMP_SEL_EN0		TMPVAL(61)

//define easy WR const
#define CMOD_WR_HEADSZ sizeof(MOLDSET_HEAD)
#define CMOD_WR_ITEMSZ sizeof(MOLDSET_ITEM)
#define CMOD_WR_PATH   SAVE_PATH"moldsetx.dat"

static QUESTION g_question;
static UI16 read_row = 0;

static void MoldSet__SaveHead()
{
    MOLDSET_HEAD head = {0, g_moldset.cur_no};
    CMOD_WHEAD_EX(&head);
}

static void MoldSet__ScanSaved()
{
    int i;
    MOLDSET_ITEM item;
    char buf[10];
    g_moldset.save_cnt = 0;
    g_moldset.is_selall = 1;

    for(i = 0; i < MAX_MOLDSET; i++)
    {
        if(CMOD_RITEM_EX(i,&item))
        {
            g_moldset.is_selall &= g_moldset.sel_state[i];
            g_moldset.save_cnt++;
        }
        else
            g_moldset.sel_state[i] = 0;
    }

    snprintf(buf,sizeof(buf)-1,"%d/%d",g_moldset.save_cnt,MAX_MOLDSET);
    VarAdrSetStr(TMP_SAVECNT, buf);
    VarAdrSetInt(TMP_SELALL, g_moldset.is_selall);
}

static void MoldSet__SelAll(BOOL bSelAll)
{
    MOLDSET_ITEM item;
    int i;

    g_moldset.is_selall = bSelAll;

    if(bSelAll)
    {
        for(i = 0; i < MAX_MOLDSET; i++)
        {
            g_moldset.sel_state[i] = CMOD_RITEM_EX(i,&item);
        }
    }
    else
    {
        for(i = 0; i < MAX_MOLDSET; i++)
        {
            g_moldset.sel_state[i] = 0;
        }
    }
}

static void MoldSet__UpdateRow(UI16 row)
{
    UI16 mold_id = g_moldset.start_no + row;
    MOLDSET_ITEM item;
        char sDate[256];

    if(CMOD_RITEM_EX(mold_id, &item))
    {
        //TimeToStr(sDate, "%d.MM.dd", item.dSave);
                snprintf(sDate,sizeof(sDate)-1,"%02d.%02d.%02d",item.dSave.year,item.dSave.month,item.dSave.day);
        VarAdrSetInt(TMP_READ_EN0+row, 1);
        VarAdrSetInt(TMP_SEL_EN0+row, 1);
    }
    else
    {
        sDate[0]='\0';
        item.sName[0]='\0';
        item.sMater[0]='\0';
        item.sColor[0]='\0';
        VarAdrSetInt(TMP_READ_EN0+row, 0);
        VarAdrSetInt(TMP_SEL_EN0+row, 0);
    }

    VarAdrSetInt(TMP_INDEX0+row, mold_id+1);
    VarAdrSetStr(TMP_NAME0+row, item.sName);
    VarAdrSetStr(TMP_MATER0+row, item.sMater);
    VarAdrSetStr(TMP_COLOR0+row, item.sColor);
    VarAdrSetStr(TMP_SAVEDATE0+row, sDate);
    VarAdrSetInt(TMP_SEL0+row, g_moldset.sel_state[mold_id]);
}

static void MoldSet__UpdateCur()
{
    MOLDSET_ITEM item;
    if(CMOD_RITEM_EX(g_moldset.cur_no, &item))
    {
        VarAdrSetInt(TMP_CURNO, g_moldset.cur_no+1);
        VarAdrSetStr(TMP_CURNAME, item.sName);
    }
    else
    {
        VarAdrSetInt(TMP_CURNO, 0);
        VarAdrSetStr(TMP_CURNAME, "");
    }
}

//模具读入
static void MoldRead()
{
    g_moldset.cur_no = g_moldset.start_no + read_row;
    MoldSet__SaveHead();
    g_moldset_head.bchange = TRUE;  //20200113

    MoldSet__UpdateCur();
    MoldSet__UpdateRow(read_row);
    if(!ReadMoldSet(g_moldset.cur_no))
    {
        OWARN("ReadMoldSet failed!");
    }
    ReadOpenCalData_Mold(g_moldset.cur_no);//20200730.cyx

    //20160926
    VarSendSaveSetIntByAdr(CLAMP_MACHSET_FL_ADPOSROBOT,VarAdrToUI16(ROBOT_FL_ROBOTCLPMID));
    //20161008
    VarSendSaveSetIntByAdr(CLAMP_MACHSET_TM_MANUALOUT,VarAdrToUI16(ROBOT_TM_ROBOTMID));
    //20170411
    VarSendSaveSetIntByAdr(ADJUST_MACH_ADPOSI_ADJTARG,VarAdrToUI16(SYSTEM_MACH_ADPOSI_ADJTARG));
    PanelRecAdd(mold_setx_pg.button8, PNL_READ, 0);
}

static void CancelButton()
{

}

static void CtlConnect()
{
}
static int Mold_SetxOnInit(Mold_Setx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Mold_SetxOnShow(Mold_Setx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USER_MODELOCK);//20210914 dyl touch

    int i;
    g_moldset.start_no = 0;
    VarAdrSetInt(UDISK_WR_PRIVILEGE,1);
    VarAdrSetInt(TMP_STARTNO, g_moldset.start_no+1);

    MoldSet__SelAll(TRUE);
    MoldSet__UpdateCur();
    MoldSet__ScanSaved();
    for(i = 0; i < 15; ++i)
    {
        MoldSet__UpdateRow(i);
    }
    g_question.pOkfunc = MoldRead;
    g_question.pCancelfunc = CancelButton;

    //20241028 chj 增加威力士颜色和重量文字切换
    if(((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00) && VarAdrToInt(p_PP_MACHSET_MOLD_NAME))
    {
        WGTSetVisible(ppg->labelHeight,TRUE);
        WGTSetVisible(ppg->labelcolor,FALSE);
    }
    else {
        WGTSetVisible(ppg->labelHeight,FALSE);
        WGTSetVisible(ppg->labelcolor,TRUE);
    }
    return 0;
}
static int Mold_SetxOnHide(Mold_Setx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Mold_SetxOnUpdate(Mold_Setx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    PWGT focus_wgt;
    UI32 goff;
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    if(VarAdrToUI16(UDIKS_READ_FLAG))
    {
        int i;
        VarAdrSetInt(UDIKS_READ_FLAG,0);
        g_moldset.start_no = 0;
        VarAdrSetInt(UDISK_WR_PRIVILEGE,1);
        VarAdrSetInt(TMP_STARTNO, g_moldset.start_no+1);

        MoldSet__SelAll(TRUE);
        MoldSet__UpdateCur();
        MoldSet__ScanSaved();
        for(i = 0; i < 15; ++i)
        {
            MoldSet__UpdateRow(i);
        }
    }

    focus_wgt =  PageFrameCurFocusWgt();
    goff = WGTGetGOffset(focus_wgt);

    if(goff >= ID_ZH_START && goff <= ID_ZH_END){
        WGTSetVisible(ppg->label199, TRUE);
        WGTSetVisible(ppg->label200, TRUE);
        WGTSetVisible(ppg->label201, TRUE);
    }
    else{
        WGTSetVisible(ppg->label199, FALSE);
        WGTSetVisible(ppg->label200, FALSE);
        WGTSetVisible(ppg->label201, FALSE);
    }

    return 0;
}
static int Mold_SetxOnChange(Mold_Setx_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr, goff;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    goff = WGTGetGOffset(pwgt);

    if(goff >= ID_READ && goff <= ID_READ_END)
    {
        read_row = goff - ID_READ;
        ShowQuestion(VW_MSG_WHETHER_READ,g_question);
    }
    else if(goff >= ID_SAVE && goff <= ID_SAVE_END)
    {
                const time_t timecopy = (UI32)time(NULL);
                struct tm* ptm = localtime(&timecopy);

        int row = goff - ID_SAVE;
        MOLDSET_ITEM item;

        g_moldset.cur_no = g_moldset.start_no + row;
        MoldSet__SaveHead();
        g_moldset_head.bchange = TRUE; //20200113

        item.dSave.year = (ptm->tm_year+1900)%100;//保留年后两位
                item.dSave.month = ptm->tm_mon+1;
                item.dSave.day = ptm->tm_mday;

        strcpy(item.sName, VarAdrToStr(TMP_NAME0+row));
        strcpy(item.sMater, VarAdrToStr(TMP_MATER0+row));
        strcpy(item.sColor, VarAdrToStr(TMP_COLOR0+row));
        CMOD_WITEM_EX(g_moldset.cur_no, &item);
        SaveOpenCal_MoldInfo(g_moldset.cur_no, &item);//20200730.cyx

        g_moldset.sel_state[g_moldset.cur_no] = TRUE;
        MoldSet__UpdateCur();
        MoldSet__UpdateRow(row);
        MoldSet__ScanSaved();
        if(!SaveMoldSet(g_moldset.cur_no))
        {
            OWARN("SaveMoldSet failed!");
        }
        SaveOpenCalData_Mold(g_moldset.cur_no);//20200730.cyx
        PanelRecAdd(pwgt, PNL_SAVE, 0);
    }
    else if(goff >= ID_DEL && goff <= ID_DEL_END)
    {
        int row = goff - ID_DEL;
        int index = g_moldset.start_no + row;

        CMOD_DITEM(index);
        DeleteOpenCal_Mold(index);//20200730.cyx
        g_moldset_head.bchange = TRUE; //20200113

        g_moldset.sel_state[index] = FALSE;
        MoldSet__UpdateCur();
        MoldSet__UpdateRow(row);
        MoldSet__ScanSaved();
        PanelRecAdd(pwgt, PNL_DELETE, 0);
    }
    else if(goff >= ID_SEL && goff <= ID_SEL_END)
    {
        int row = goff - ID_SEL;
        int index = g_moldset.start_no + row;

        g_moldset.sel_state[index] = (UI8)VarAdrToUI16(TMP_SEL0+row);
        MoldSet__ScanSaved();
    }
    else if(goff == ID_IMPORT)
    {
        if(check_usb())
        {
           ShowMsg(VW_MSG_UDISKNOTEXIST);
           return 0;
        }
        PanelRecAdd(pwgt, PNL_READ_U, 0);
        PanelShowDialogByName(DIALOG_READUDISK);
    }
    else if(goff == ID_EXPORT)
    {
        if(check_usb())
        {
           ShowMsg(VW_MSG_UDISKNOTEXIST);
           return 0;
        }
        PanelRecAdd(pwgt, PNL_WRITE_U, 0);
        PanelShowDialogByName(DIALOG_WRITEUDISK);
    }
    else if(addr == TMP_STARTNO)
    {
        int i;
        g_moldset.start_no = RU16(*pattr) - 1;
        MoldSet__ScanSaved();
        for(i = 0; i < 15; ++i)
        {
            MoldSet__UpdateRow(i);
        }
    }
    else if(addr == TMP_SELALL)
    {
        int i;
        MoldSet__SelAll(RU32(*pattr));
        for(i = 0; i < 15; ++i)
        {
            MoldSet__UpdateRow(i);
        }
    }
    else if(pwgt == ppg->btlast)
    {
        int i;
        if(VarAdrToInt(TMP_STARTNO) > 15)
        {
            VarAdrSetInt(TMP_STARTNO, VarAdrToInt(TMP_STARTNO)-15);
        }
        else
        {
            VarAdrSetInt(TMP_STARTNO, 1);
        }

        g_moldset.start_no = VarAdrToInt(TMP_STARTNO) - 1;
        MoldSet__ScanSaved();
        for(i = 0; i < 15; ++i)
        {
            MoldSet__UpdateRow(i);
        }
    }
    else if(pwgt == ppg->btnext)
    {
        int i;
        if(VarAdrToInt(TMP_STARTNO) < (MAX_MOLDSET+1-15*2))
        {
            VarAdrSetInt(TMP_STARTNO, VarAdrToInt(TMP_STARTNO)+15);
        }
        else
        {
            VarAdrSetInt(TMP_STARTNO, MAX_MOLDSET+1-15);
        }

        g_moldset.start_no = VarAdrToInt(TMP_STARTNO) - 1;
        MoldSet__ScanSaved();
        for(i = 0; i < 15; ++i)
        {
            MoldSet__UpdateRow(i);
        }
    }

    return 0;
}
