#include "WPushtable2.h"
#include "typedef.h"
#include "pagefram.h"
#include "questionDlg.h"
#include "pushtab.h"
#include "stdio.h"
#include "file.h"
#include "information.h"
#include "taskmoni.h"
#include "database.h"

DEFINE_PUSHTABLE2_MAP

CONNECT_PUSHTABLE2_EVENT

#define PUSHTAB_INDEX             48   //页面地址偏移

#define PUSHTABLE_PAGENUM		TMPVAL(99) //页码  临时变量99
#define PUSHTABLE_PUSHNUM		TMPVAL(100)//序号  临时变量100

#define TMP_PUSHPOS		TMPVAL(0)   //推力座     临时变量0-47
#define TMP_PUSHPOSEND	TMPVAL(47)  //推力座     临时变量47
#define TMP_MOLDPOS		TMPVAL(48)  //模板位置   临时变量48-95
#define TMP_MOLDPOSEND	TMPVAL(95)  //最后模板位置 临时变量95

#define TMP_PASSWORD            TMPVAL(301) //修改密码
#define TMP_PUSHDATAREADONLY	TMPVAL(300) //推力座数据只读
#define TMP_EDITBKCOLOR1        TMPVAL(302) //编辑框背景色1
#define TMP_EDITBKCOLOR2        TMPVAL(303) //编辑框背景色2

static QUESTION g_question;

static void LoadPushtabData()
{
    int i= 0;
    int index = 0;
    TRANSMETER_ITEM* pData=g_dbTransMeterTable2;

    index = VarAdrToUI16(PUSHTABLE_PAGENUM)*PUSHTAB_PAGE_GROUPNUM;
    pData = &pData[index];

    for(i=0;i<PUSHTAB_PAGE_GROUPNUM;i++)
    {
        UI32 dwaddr;
        dwaddr = TMP_PUSHPOS+i;
        VarAdrSetInt(dwaddr,(pData)->wPump);
        dwaddr = TMP_MOLDPOS+i;
        VarAdrSetInt(dwaddr,(pData++)->wClamp);
        //pushcod
        VarAdrSetInt(PUSHTABLE_PUSHNUM+i,VarAdrToUI16(PUSHTABLE_PAGENUM)*PUSHTAB_PAGE_GROUPNUM+i);
    }
}

static void WritePushPos(UI16 index)
{
    UI16 wData,wIndex;

    wIndex = index;

    if(/*wIndex >= 0 &&*/ wIndex <PUSHTAB_INDEX)
    { //十字头位置UI16
        wData = (UI16)VarAdrToInt(TMP_PUSHPOS+wIndex);
        g_dbTransMeterTable2[wIndex+PUSHTAB_PAGE_GROUPNUM*VarAdrToUI16(PUSHTABLE_PAGENUM)].wPump = wData;
        SavePushTableIndex(&g_dbTransMeterTable2[wIndex+PUSHTAB_PAGE_GROUPNUM*VarAdrToUI16(PUSHTABLE_PAGENUM)], wIndex+PUSHTAB_PAGE_GROUPNUM*VarAdrToUI16(PUSHTABLE_PAGENUM));
    }
}

static void WriteMoldPos(UI16 index)
{
    UI16 wIndex;
    UI32 dwData;

    wIndex = index;

    if(/*wIndex >= 0 && */wIndex <PUSHTAB_INDEX)
    { //模板位置UI16
        dwData = (UI32)VarAdrToInt(TMP_MOLDPOS+wIndex);
        g_dbTransMeterTable2[wIndex+PUSHTAB_PAGE_GROUPNUM*VarAdrToUI16(PUSHTABLE_PAGENUM)].wClamp = dwData;
        SavePushTableIndex(&g_dbTransMeterTable2[wIndex+PUSHTAB_PAGE_GROUPNUM*VarAdrToUI16(PUSHTABLE_PAGENUM)], wIndex+PUSHTAB_PAGE_GROUPNUM*VarAdrToUI16(PUSHTABLE_PAGENUM));
    }
}

static int button41OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(pushtable2_pg.button43, WGT_CLICK_MSG, button43OnClick);
    WgtMsgConnect(pushtable2_pg.button42, WGT_CLICK_MSG, button42OnClick);
    WgtMsgConnect(pushtable2_pg.button41, WGT_CLICK_MSG, button41OnClick);
}
static int Pushtable2OnInit(Pushtable2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pushtable2OnShow(Pushtable2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(PUSHTABLE_PAGENUM,0);//进入默认0页
    LoadPushtabData();
    g_question.pOkfunc = PanelMainRestart;
    g_question.pCancelfunc = NULL;

    VarAdrSetInt(TMP_PUSHDATAREADONLY, 1);
    VarAdrSetInt(TMP_PASSWORD,1234);
    VarAdrSetInt(TMP_EDITBKCOLOR1,0xdedede);
    VarAdrSetInt(TMP_EDITBKCOLOR2,0xdedede);

    if(VarAdrToInt(FL_CLSOPEN_MODE) == 2)
        NaviSetIndex(PanelCurPage(), 1);
    else if((VarAdrToInt(FL_CLSOPEN_MODE) == 3) || (VarAdrToInt(FL_CLSOPEN_MODE) == 4))
        NaviSetIndex(PanelCurPage(), 2);
    else
        NaviSetIndex(PanelCurPage(), 0);

    return 0;
}
static int Pushtable2OnHide(Pushtable2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Pushtable2OnUpdate(Pushtable2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //udisk import
    if(VarAdrToUI16(UDIKS_READ_FLAG) && (VarAdrToInt(INFORMATION_INDEX) == 0))
    {
        VarAdrSetInt(UDIKS_READ_FLAG,0);
        SetMachineType(PushtableType());//type
        VarAdrSetInt(PUSHTABLE_PAGENUM,0);//进入默认0页
        LoadPushtabData();

        ShowQuestion(VW_MSG_RESTART,g_question);
    }

    return 0;
}
static int Pushtable2OnChange(Pushtable2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //UI16 value;
    UI16 index;
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch(addr)
    {
    case PUSHTABLE_PAGENUM:
        LoadPushtabData();
        break;
    case TMP_PASSWORD:
        if(VarAdrToInt(TMP_PASSWORD) == 9595)
        {
            VarAdrSetInt(TMP_PUSHDATAREADONLY, 0);
            VarAdrSetInt(TMP_EDITBKCOLOR1,0xffffff);
            VarAdrSetInt(TMP_EDITBKCOLOR2,0x00e700);
        }
        else
        {
            VarAdrSetInt(TMP_PUSHDATAREADONLY, 1);
            VarAdrSetInt(TMP_EDITBKCOLOR1,0xdedede);
            VarAdrSetInt(TMP_EDITBKCOLOR2,0xdedede);
        }
        break;
    default:
        break;
    }

    if(addr >=TMP_PUSHPOS && addr<= TMP_PUSHPOSEND)
    {
        index =(UI16)(addr-TMP_PUSHPOS);
        WritePushPos(index);
    }
    else if(addr >=TMP_MOLDPOS && addr <= TMP_MOLDPOSEND)
    {
        index =(UI16)(addr-TMP_MOLDPOS);
        WriteMoldPos(index);
    }

    return 0;
}
static int button41OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(check_usb())
    {
        ShowMsg(VW_MSG_UDISKNOTEXIST);
        return 0;
    }
    else
    {
        PanelShowDialogByName(DIALOG_READUDISK);
        VarAdrSetInt(UDISK_WR_PRIVILEGE,2); //导入导出 权限
    }

    return 0;
}
static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(check_usb())
    {
        ShowMsg(VW_MSG_UDISKNOTEXIST);
        return 0;
    }
    else
    {
        PanelShowDialogByName(DIALOG_WRITEUDISK);
        VarAdrSetInt(UDISK_WR_PRIVILEGE,2); //导入导出 权限
    }

    return 0;
}
static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    char cPath[255];
    UI16 push_type;
    UI16 wValue1 = VarAdrToInt(d_machine1_MACHSET_FL_reserve_3)/100;
    UI16 wValue2 = VarAdrToInt(d_machine1_MACHSET_FL_reserve_3)%100;

    push_type = PushtableType();

    if(VarAdrToInt(d_machine1_MACHSET_FL_reserve_3) > 100)//20200408.cyx
    {
        snprintf(cPath, sizeof(cPath)-1, SAVE_PATH"pushtab%d%c%c.dat", push_type, wValue1, wValue2);
    }
    else
    {
        if(VarAdrToInt(d_machine1_MACHSET_FL_reserve_3) == 0)
        {
            snprintf(cPath, sizeof(cPath)-1, SAVE_PATH"pushtab%d.dat", push_type);
        }
        else
        {
            snprintf(cPath, sizeof(cPath)-1, SAVE_PATH"pushtab%d%c.dat", push_type, wValue2);
        }
    }

    FileDelete(PUSHTABDB_INIT_FILE);
    FileCopy(cPath,PUSHTABDB_INIT_FILE);
    SetMachineType(push_type);//92默认表
    LoadPushtabData();

    ShowQuestion(VW_MSG_RESTART,g_question);
    //ShowMsg(VW_MSG_RESTART);

    return 0;
}
