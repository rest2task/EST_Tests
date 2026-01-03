#include "WmachTbCfg_5200.h"
#include "typedef.h"
#include "pagefram.h"
#include "dataCfgtb.h"
#include "mxml.h"
#include "file.h"
#include "information.h"
#include "stdio.h"
#include "dropwgt.h"
#include "protcmd.h"
#include "database.h"
#include "oprintf.h"
#include "machcfgtbl.h"
#include "questionDlg.h"
#include "ctrlcmd.h"
#include "labelwgt.h"
#include "texttbl.h"
#include "dataOperation.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_MACHTBCFG_5200_MAP

CONNECT_MACHTBCFG_5200_EVENT

#define     TMP_BEGIN				BASEADR_VAL
#define		TMP_END					(TMP_BEGIN+41)

#define		METER_BG_ADDR				(BASEADR_VAL+50)	//电尺部分
#define		METER_ED_ADDR				(BASEADR_VAL+70)
#define		CODE_BG_ADDR			(BASEADR_VAL+1)		//CODE 起始地址
#define		CODE_ED_ADDR			(BASEADR_VAL+39)	//CODE结束地址

#define		MACHTY_ID				(BASEADR_VAL+101)	//机型ID
#define     RADIO_USEBLE			(BASEADR_VAL+42)
#define     RADIO_CHECKED			(BASEADR_VAL+100)
#define		MAX_DIRCOUNT			(256)

#define		ADDR_COMB				0x331501F7			//下拉框数据对应地址
#define		VALUE_COMB				TMPVAL(0)			//下拉框值对应地址
#define		ROOTFILE				HMI_ROOT_PATH"iventinit/"

#define STAD_NAME(NUM,pbuff,size)		joinStrAndNum("stAD",NUM,pbuff,size)
#define STADUNIT_NAME(NUM,pbuff,size)	joinStrAndNum("stUnit",NUM,pbuff,size)//20211210 dyl 根据液压通道号、座台通道号设置AD名称


#define PUSH_TEMP_ADDR    TMPSTR(39) //十字头型号对应得临时地址 //20200408.cyx
#define PUSHTAB_CODE      TMPVAL(39) //20210820
static char cpushxbak[8] = "";

static char panelpath[128]={0};
static char mainpath[128]={0};
static char mainbinpath[128]={0};
static char opencalpath[128]={0};//20201210
static char machsetpath[128]={0};
static char dacfgpath[128]={0};//20210120
static MCCFG g_mccfg;
static FILEDIR g_filedir;

static QUESTION g_question;
static BOOL question_show = FALSE;
//static void PanelUpdateOK()
//{
//#ifdef WIN32
//	oprintf("reboot\n");
//#else
//	osystem("reboot");
//#endif
//}
static void MainUpdateOK()
{
    PanelMainRestart();
}

//20200408.cyx
static void SetPushWndTxt()
{
    UI16 wValue1 = VarAdrToInt(TMPVAL(22))/100;
    UI16 wValue2 = VarAdrToInt(TMPVAL(22))%100;
    UI16 g_pushcode = VarAdrToInt(TMPVAL(39));

    if(VarAdrToInt(TMPVAL(22)) > 100)
    {
        sprintf(cpushxbak, "%d%c%c", g_pushcode, wValue1, wValue2);
    }
    else
    {
        if(VarAdrToInt(TMPVAL(22)) == 0)
        {
            sprintf(cpushxbak, "%d", g_pushcode);
        }
        else
        {
            sprintf(cpushxbak, "%d%c", g_pushcode, wValue2);
        }
    }

    VarAdrSetStr(PUSH_TEMP_ADDR, cpushxbak);
}

static void NameChangeAndShow()  //20200325
{
    PWGT pwgt;
    char buff[10];
//    UI16 wNozzleValue = VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH);

    LabelSetText(machtbcfg_5200_pg.stAD4,GetTextTran(TEXT_AD_ADHYDR1));
    LabelSetText(machtbcfg_5200_pg.stUnit4,"(bar)");
    LabelSetText(machtbcfg_5200_pg.stAD5,"AD5");
    LabelSetText(machtbcfg_5200_pg.stAD6,"AD6");
    LabelSetText(machtbcfg_5200_pg.stAD7,"AD7");

//    if(VarAdrToInt(TMPVAL(29)) == 0x5200 || VarAdrToInt(TMPVAL(29)) == 0x5300)
//    {
//        VarAdrSetInt(TMPVAL(300), 1);//8路电子尺可见
//    }
//    else
//    {
//        VarAdrSetInt(TMPVAL(300), 0);//4路电子尺可见
//    }
    //20211210 dyl 根据液压通道号、座台通道号设置AD名称
    LabelSetText(machtbcfg_5200_pg.stAD4,"AD4");
    LabelSetText(machtbcfg_5200_pg.stAD5,"AD5");
    LabelSetText(machtbcfg_5200_pg.stAD6,"AD6");
    LabelSetText(machtbcfg_5200_pg.stAD7,"AD7");
    LabelSetText(machtbcfg_5200_pg.stAD8,"AD8");
    LabelSetText(machtbcfg_5200_pg.stUnit4,"(mm)");
    LabelSetText(machtbcfg_5200_pg.stUnit5,"(mm)");
    LabelSetText(machtbcfg_5200_pg.stUnit6,"(mm)");
    LabelSetText(machtbcfg_5200_pg.stUnit7,"(mm)");
    LabelSetText(machtbcfg_5200_pg.stUnit8,"(mm)");
    UI16 wChanel_Sensor1 = VarAdrToInt(d_Pump1_MACHSET_PRESSAD_CH);
    UI16 wChanel_Sensor2 = VarAdrToInt(d_Pump2_MACHSET_PRESSAD_CH);
    if(VarAdrToUI16(SYS_FL_MACH_CODE1)&0x04)//20220928 dyl ECAT模式
    {
        if ((wChanel_Sensor1>=4)&&(wChanel_Sensor1 <= g_mainresource_current.nADNum))
        {
            pwgt = PageFramCurWgtByName(STAD_NAME(wChanel_Sensor1,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR1));
            pwgt = PageFramCurWgtByName(STADUNIT_NAME(wChanel_Sensor1,buff,10));
            LabelSetText(pwgt,"(bar)");
        }
        if ((wChanel_Sensor2>=4)&&(wChanel_Sensor2 <= g_mainresource_current.nADNum)
                &&(wChanel_Sensor2 != wChanel_Sensor1))
        {
            pwgt = PageFramCurWgtByName(STAD_NAME(wChanel_Sensor2,buff,10));
            LabelSetText(pwgt,GetTextTran(TEXT_AD_SENSOR2));
            pwgt = PageFramCurWgtByName(STADUNIT_NAME(wChanel_Sensor2,buff,10));
            LabelSetText(pwgt,"(bar)");
        }
    }

//    if (VarAdrToInt(SYS_FL_METER) & 0x0004)
//    {
//        if (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5200)
//        {
//            if((wNozzleValue>=4)&&(wNozzleValue<=7))  //4-7路做座台切换
//            {
//                pwgt = PageFramCurWgtByName(STAD_NAME(wNozzleValue,buff,10));
//                LabelSetText(pwgt,GetTextTran(TEXT_AD_NOZZLE));
//                if(wNozzleValue == 4)
//                {
//                    LabelSetText(machtbcfg_5200_pg.stUnit4,"(mm)");
//                }
//            }
//        }
//        else
//        {
//            if(wNozzleValue == 4)  //4做座台切换
//            {
//                LabelSetText(machtbcfg_5200_pg.stAD4,GetTextTran(TEXT_AD_NOZZLE));
//                LabelSetText(machtbcfg_5200_pg.stUnit4,"(mm)");
//            }
//        }
//    }
}

/************************************************************************/
/* 显示路径下的文件，到dropbox里                                 */
/************************************************************************/
static void UpdateList()
{
    char str[20];
    char path[64];
    int wcode6 = VarAdrToInt(SYS_MACH_CUSTOMID);
    if(IntToHexStrEX(str,wcode6,4,TRUE))
    {
        snprintf(path,sizeof(path)-1,ROOTFILE"%s/mach/",str);
        if(FileExist(path))
        {
            if(GetDir(&g_filedir,path,GETDIR_SUB))
            {
                char* pinfo =NULL;
                if((pinfo=ConvertDirInfoToStr(&g_filedir)) != NULL)
                {
                    DropSetItems(machtbcfg_5200_pg.dropdown2,pinfo);//VarAdrSetStr(ADDR_COMB,pinfo);
                    OBJMemFree((void *)pinfo);
                    pinfo=NULL;
                    return ;
                }
            }
        }
    }
    VarAdrSetStr(ADDR_COMB,"");
}

static void setShowData(MCCFG* tblcfg)
{
    int addr;
    int i=0;
    UI16 *pvalue = (UI16 *)(&tblcfg->codetbl);
    for(addr=CODE_BG_ADDR;addr<=CODE_ED_ADDR;addr++,pvalue++)
    {
        VarAdrSetInt(addr,*pvalue);
    }

    addr = METER_BG_ADDR;
    for(i=0 ; i<METER_CNT ; i++)
    {
        VarAdrSetInt(addr+i,tblcfg->meter[i].meterpos);
        VarAdrSetInt(addr+i+10,tblcfg->meter[i].pos);
    }
}

/************************************************************************/
/* 读取指定的xml文件                                             */
/************************************************************************/
static void LoadData(const char * sysname)
{
    char str[8];
    int i;
    int wcode6 = VarAdrToInt(SYS_MACH_CUSTOMID);

    if(IntToHexStrEX(str,wcode6,4,TRUE))
    {
        snprintf(mainpath,sizeof(mainpath)-1,ROOTFILE"%s/mach/%s/main.xml",str,sysname);
        snprintf(panelpath,sizeof(panelpath)-1,ROOTFILE"%s/mach/%s/est_cur.tag",str,sysname);
        snprintf(mainbinpath,sizeof(mainbinpath)-1,ROOTFILE"%s/mach/%s/main.bin",str,sysname);
        snprintf(opencalpath,sizeof(opencalpath)-1,ROOTFILE"%s/mach/%s/opentabcal.dat",str,sysname);//20201210
        snprintf(machsetpath,sizeof(machsetpath)-1,ROOTFILE"%s/mach/%s/MachValue.csv",str,sysname);
        snprintf(dacfgpath,sizeof(dacfgpath)-1,ROOTFILE"%s/mach/%s/dacfg.xda",str,sysname);//20210120
    }
    oprintf("mainpath=%s\n",mainpath);
    oprintf("panelpath=%s\n",panelpath);
    VarAdrSetInt(TMP_BEGIN+1,0);
    if(FileExist(panelpath) || FileExist(opencalpath) || FileExist(machsetpath) || FileExist(dacfgpath))//20210120
    {
        VarAdrSetInt(RADIO_USEBLE,1);
    }
    else
    {
        VarAdrSetInt(RADIO_USEBLE,0);
    }
    if(!FileExist(mainpath))
    {
        for(i=CODE_BG_ADDR+1;i<CODE_ED_ADDR+1;i++)
        {
            VarAdrSetInt(i,0);
        }

        for(i=METER_BG_ADDR;i<METER_ED_ADDR;i++)
        {
            VarAdrSetInt(i,0);
        }

        NameChangeAndShow();//20200325
    }
    else
    {
        GetMainCfg(mainpath,&g_mccfg);
        setShowData(&g_mccfg);
    }

    {
        UI16 wcode0 = VarAdrToUI16(CODE_BG_ADDR);
        UI16 wcode1 = VarAdrToUI16(CODE_BG_ADDR+1);
        UI16 wcode54 = VarAdrToUI16(CODE_BG_ADDR+14);
        if(wcode1 & 0x0004)
        {
            VarAdrSetInt(MACHTY_ID,1);
        }
        else if(((wcode0 & 0x0004)==0) && (wcode54 & 0x1020))
        {
            VarAdrSetInt(MACHTY_ID,2);
        }
        else
        {
            VarAdrSetInt(MACHTY_ID,0);
        }
    }

    SetPushWndTxt();//20200408.cyx
}
/************************************************************************/
/* 传输的回调函数                                                */
/************************************************************************/
static void PacketFuncCallBack(void* pdat, int para1, int para2)
{
    char cmd[128]="\0";
    if(para2 == 0xffff)
    {
        OBJMemFree(pdat);
        ShowMsg(VM_MS_IMPORTSUCSS);
        g_question.pOkfunc = MainUpdateOK;
        g_question.pCancelfunc = NULL;
        question_show = TRUE;
        //ShowQuestion(VW_MSG_RESTART,g_question);
        //20210820
        snprintf(cmd,sizeof(cmd)-1, "cp -f /pages/save/pushtab%s.dat  %spushtab.dat",VarAdrToStr(PUSH_TEMP_ADDR),SAVE_PATH);
        osystem(cmd);
    }
}

static BOOL PageConfirm()
{
    FILEHD filehd;
    int size;
    char * pdata = NULL;
    char scmd[128];

    filehd = FileOpen(mainbinpath);
    if(FILEHD_IS_OK(filehd))
    {
        size = FileGetSize(filehd, mainbinpath);
        pdata = (char *)OBJMemMalloc(size);
        if(FileRead(filehd,pdata,size,NULL))
        {
            PackTxTypeStart(TYPE_TABLE_PART,pdata,size,PacketFuncCallBack);
            ShowMsg(VM_MS_DATAIMPORT);
        }
        FileClose(filehd);
    }

    if(VarAdrToInt(RADIO_CHECKED))//20200423 面板数据导入
    {
        if(FileExist(panelpath))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s %sest_cur.tag", panelpath, DATA_PATH);
            osystem(scmd);
        }

        if(FileExist(opencalpath))//20201210
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s %sopentabcal.dat", opencalpath,SAVE_PATH);
            osystem(scmd);
        }

        if(FileExist(machsetpath))//20201210
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s %sMachValue.csv", machsetpath,SAVE_PATH);
            osystem(scmd);

            //20230518 chj 此处有个bug，当面板数据和结构参数一起导入的时候，如果est_cur.tag文件大小不同，
            //即中间加过参数，再结合结构参数内的dic，可能会导致数据错乱，所以改为如果导入过数据，开机读取结构参数
            char scmdMach[128];
            snprintf(scmdMach,sizeof(scmdMach)-1, "touch %s",PANEL_MACHVALUE_FILE);
            osystem(scmdMach);
            //LoadDataValueExcel(MACHSET_PATH, TRUE);
        }

        if(FileExist(dacfgpath))//20210120
        {
            snprintf(scmd,sizeof(scmd)-1,CMD_COPY" %s %sdacfg.dat", dacfgpath, SAVE_PATH);
            osystem(scmd);
            LoadDACfgData();
        }
    }
    return TRUE;
}

//20181022hz
static void ShowRefresh()
{
    UI16 wcode0 = VarAdrToUI16(CODE_BG_ADDR);
    UI16 wcode1 = VarAdrToUI16(CODE_BG_ADDR+1);
    UI16 wcode54 = VarAdrToUI16(CODE_BG_ADDR+14);

    if(wcode1 & 0x0004)
    {
        DropSetCurIndex(machtbcfg_5200_pg.dropdown1,1);
    }
    else if(((wcode0 & 0x0004)==0) && (wcode54 & 0x1020) && (wcode54 & 0x100) )//20241205 chj 金鹰增加麦格米特模式
    {
        DropSetCurIndex(machtbcfg_5200_pg.dropdown1,3);
    }
    else if(((wcode0 & 0x0004)==0) && (wcode54 & 0x1020))
    {
        DropSetCurIndex(machtbcfg_5200_pg.dropdown1,2);
    }
    else
    {
        DropSetCurIndex(machtbcfg_5200_pg.dropdown1,0);
    }
}

static BOOL PageInit(machTbCfg_5200_PG* ppg)
{
    char * pstr= NULL;
    //20220831 dyl 只赋值一次，不必每次进画面都读取
//    g_version = ReadVersion();
    VarAdrSetStr(SYSTEM_ADDR,g_version.system1);
    oprintf("---g_version.system1=%s",g_version.system1);

    UpdateList();
    pstr = VarAdrToStr(SYSTEM_ADDR);
    VarAdrSetInt(TMPVAL(100),0);
    if (DropSetSelByName(machtbcfg_5200_pg.dropdown2,pstr))//20220915 dyl 当找不到对应的下拉框文本时，下拉框赋为空白需重新选择
    {
        LoadData(g_version.system1);
        ShowRefresh();
    }
    else
    {
        LoadData(g_version.system1);//20220928 dyl
        DropSetCurIndex(machtbcfg_5200_pg.dropdown2, -1);
    }

    NameChangeAndShow();//20200325

    question_show = FALSE;

    return TRUE;
}

//下拉框值的选择  20181225 hz
static BOOL PickDownName()
{
    char  textBuf[MAX_UPDOWN_NAME_LEN];
    memset(textBuf, 0, sizeof(textBuf)); //20181123 hz
    DropGetCurItemText(machtbcfg_5200_pg.dropdown2,textBuf,21);

    if (textBuf[22] != '\0')
    {
        ShowMsg(VW_MSG_MACHINENAMETOOLONG);
        return FALSE;
    }

    strcpy(g_version.system1,textBuf);
    VarAdrSetStr(SYSTEM_ADDR,g_version.system1);
    return TRUE;
}


//只加载数据，不保存数据 20181221 hz
static BOOL PageRelode(machTbCfg_5200_PG* ppg)
{
    if(!PickDownName())
    {
        return FALSE;//20210127.cyx
    }

    LoadData(g_version.system1);
    ShowRefresh();
    return TRUE;
}

//20181221 hz
static BOOL PageSave()
{
    PickDownName();

    WriteVersion(g_version);
    return TRUE;
}

static void CtlConnect()
{
}
static int machTbCfg_5200OnInit(machTbCfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    g_version = ReadVersion();//20220915 dyl 只赋值一次，不必每次进画面都读取
    return 0;
}
static int machTbCfg_5200OnShow(machTbCfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

    PageInit(ppg);
    return 0;
}
static int machTbCfg_5200OnHide(machTbCfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    return 0;
}
static int machTbCfg_5200OnUpdate(machTbCfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    if(question_show)
    {
        if(!VarAdrToInt(INFORMATION_INDEX))
        {
            ShowQuestion(VW_MSG_RESTART,g_question);
            question_show = FALSE;
        }
    }

    return 0;
}
static int machTbCfg_5200OnChange(machTbCfg_5200_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    switch (addr)
    {
    case VALUE_COMB:
        if(!PageRelode(ppg))//20210127.cyx
        {
            VarAdrSetInt(addr, msg_value);
        }
        break;

    default:
        if(pwgt == ppg->button43)
        {
            if(VarAdrToInt(MOTOR_STATE_STATE) != 0)
            {
                ShowMsg(VW_MSG_MOTOOPNOTOUTPUT);
                return -1;
            }

            if(strcmp(VarAdrToStr(TMPSTR(3)),"") == 0)
            {
                ShowMsg(VW_MSG_CONFIGFILEREADFAIL);
                return -1;
            }

            PageSave();
            PageConfirm();
        }
        break;
    }
    return 0;
}
