#include "WProgUpdate.h"
#include "typedef.h"
#include "pagefram.h"
#include "file.h"
#include "vardb.h"
#include "updateprot.h"
#include "stdio.h"
#include "questionDlg.h"
#include "information.h"
#include "database.h"
#include "protcmd.h"
#include "oprintf.h"
#include "common.h"
#include "buttonwgt.h"
#include "texttbl.h"
#include "taskmoni.h"
#include "gux.h"
#include "kjump.h"
#include "keyprot.h"
#include "privdef.h"
#include "ctrlcmd.h"
#include "protmod.h"
#include "udisk.h"
#include "progbarwgt.h"
#include "string.h"
#include "labelwgt.h"
#include "canmod.h"//20211229 dyl IAP
#include "iapadmin.h"
#include "usermod.h"//20210914 dyl touch
#include "editwgt.h"
#include "panelrec.h"
#include "keymod.h"

DEFINE_PROGUPDATE_MAP 

CONNECT_PROGUPDATE_EVENT	

//#ifdef M788_PANEL
//#define FILE_PATH	USB_PATH"gaaxn"
//#else
//#define FILE_PATH	USB_PATH"gfxn"
//#endif
//20220729 dyl
#ifdef M788_PANEL
#ifdef A40I
#ifdef MPANEL_TC
#define FILE_PATH   USB_PATH"gdcxn"   //789B-TR/789B-TC
#define CODE        "gdcxn"
#else
#define FILE_PATH   USB_PATH"gdaxn"   //789B
#define CODE        "gdaxn"
#endif
#else
#define FILE_PATH	USB_PATH"gaaxn" //788
#define CODE        "gaaxn"
#endif
#else
#define FILE_PATH	USB_PATH"gfxn" //791
#define CODE        "gfxn"
#endif


#define MAIN_PATH	USB_BOOT_PATH"mainivent/iven-"     //20190329 hz
#define CFG_PATH	USB_BOOT_PATH"mainivent/cfg-"      //20190329 hz

#define		PANEL_MACHSET		"p.PP.MACHSET."

#define		PANELCODE_NAME		0x33120041 //PANEL_MACHSET"UPDATE_PANELCODE"
#define		CONFIGCODE_NAME		0x33120042 //PANEL_MACHSET"UPDATE_CONFIGCODE"
#define		MANUFCODE_NAME		0x33120043 //PANEL_MACHSET"UPDATE_MANUFCODE"
#define		PROGRAMCODE_NAME	0x33120044 //PANEL_MACHSET"UPDATE_PROGRAMCODE"

#define		ADR_PROGRESS_MAX	TMPVAL(6) //进度条最小值
#define		ADR_PROGRESS_VALUE	TMPVAL(5) //进度条值


#define    TMP_PANELCODE    TMPVAL(1)  //面板更新代码 临时变量1

#define    TMP_CONFIGCODE   TMPVAL(2)  //配置代码 临时变量2
#define    TMP_MANUFCODE    TMPVAL(3)  //厂商代码 临时变量3
#define    TMP_PROGRAMCODE  TMPVAL(4)  //程序代码 临时变量3

//20211229 dyl IAP
#define    TMP_IAPCODE    TMPVAL(10)  //扩展板代码 临时变量10
#define    TMP_IAPTYPE    0x331501fe  //扩展板类型 临时变量
#define    TMP_IAPTYPE_NEW    TMPVAL(20)  //扩展板类型(下拉框) 临时变量
#define    TMP_IAPNUM     TMPVAL(12)  //扩展板号   临时变量12
#define    TMP_IAPFORCE   TMPVAL(13)  //强制更新密码 临时变量13
#define	   ADR_PROGRESS2_MAX	TMPVAL(15) //进度条2最大值
#define	   ADR_PROGRESS2_VALUE	TMPVAL(14) //进度条2值
#define    IAP_PATH	      USB_BOOT_PATH"iap/iap-" //20211229 dyl

extern void QtEventLoop(); //20190329 hz
static void PacketFuncCallBack(void* pdat, int para1, int para2);
BOOL PanelUpdate();
BOOL MainUpdate();
BOOL PageShowInit();
void MainUpdateOK();


static QUESTION g_question;

static BOOL bmainsuccess=FALSE;
static BOOL bmainreboot=FALSE;//主机重启按下标记
static BOOL bpanelsuccess = TRUE;//20220228 dyl

//20211229 dyl IAP
static BOOL biapupdate_start = FALSE;//20211229 dyl iap更新按钮按下
static UI32  time_1s =0;//20211229 dyl
static UI32  time_1sx =0;
static UI16  second = 0;
static UI16  second2 = 0;
static UI16  Num_ForceUpdateFalse = 0;

//文件行数
static int FileRowCount(const char* value, int size)
{
	int row_count = 0;

	if(!value) return 0;

	while(*value && size--)
	{
		if(*value == '\n') row_count++;
		value++;
	}

	return row_count;
}

#ifdef IMX6
#define  READ_BUF 2000

#ifdef A40I //20230518 chj A40i板子更新失败，恢复文件
static char updatebak[] = "cp -r /pages /home/updatebak_hmi/\ncp -f /opt/HmiApp /home/updatebak_hmi/\n";
#else
static char updatebak[] = "cp -rf /pages/data /home/updatebak_hmi/pages/\n"
                          "cp -rf /pages/form /home/updatebak_hmi/pages/\n"
                          "cp -rf /pages/lua /home/updatebak_hmi/pages/\n"
                          "cp -rf /opt/HmiApp /home/updatebak_hmi/\n";

static char updatebak_pages_file[] = "/home/updatebak_hmi/pages";
#endif
static char updatebak_file[] = "/home/updatebak_hmi/";

int SetCmdList(const char* filepath)
{
    FILEHD hfile;
    int row, i;
    char cmd[255];
    int readcnt = 0;
    int exit_code;
    char* pstr = NULL;
    char* temp_str = NULL;

    #ifdef A40I
    MkDir(updatebak_file);
    #else
    MkDir(updatebak_file);
    MkDir(updatebak_pages_file);
    #endif

    hfile = FileOpen(filepath);
    if(FILEHD_IS_OK(hfile))
    {
        int size = 0;
        int fileSize = FileGetSize(hfile, filepath);
        size = fileSize + sizeof(updatebak);
        pstr = OBJMemMalloc(size);
        temp_str = pstr;

        strncpy(pstr, updatebak, sizeof(updatebak));
        if(pstr != NULL)
        {
            FileRead(hfile, pstr+sizeof(updatebak)-1, fileSize, &readcnt);
            FileClose(hfile);
        }
    }
    else
    {
        FileClose(hfile);
    }


    row = FileRowCount(pstr,readcnt+sizeof(updatebak));
    if(row<=0)
    {
        ShowMsg(VW_MSG_UDISKNOFILE);
        return FALSE;
    }


    ProgbarSetMaxValue(progupdate_pg.panelbar1, row);
    ProgbarSetCurValue(progupdate_pg.panelbar1, 0);

    for(i=0;i<row;i++)
    {
        ProgbarSetCurValue(progupdate_pg.panelbar1,i+1);
        if(*pstr =='\n') //过滤空行
        {
            pstr=pstr+1;
            continue;
        }
        sscanf(pstr, "%[^\n]", cmd);
        pstr+=strlen(cmd)+1; //1:回车
        oprintf("cmd=%s\r\n",cmd);

        if((cmd[0]=='#')) //过滤注销命令
        {
            continue;
        }
        else if((cmd[0]==' '))
        {
            continue;
        }
        while(system_deadline(cmd, 35, &exit_code,TRUE))//20230518 chj 面板更新比较
        {
            ;
        }
        if(exit_code == -1)
        {
            if(temp_str){
                OBJMemFree(temp_str);
            }

            BakToPanel();
            ShowMsg(VW_MSG_UDISKABNORMAL);

            return FALSE;
        }
        else if(exit_code == 1 && strstr(cmd, "cp"))
        {
            if(temp_str){
                OBJMemFree(temp_str);
            }

            BakToPanel();
            ShowMsg(VW_MSG_UDISKNOFILE);

            return FALSE;
        }
        PageFramReFresh(PanelCurPage(),TRUE);
    }
    if(temp_str){
        OBJMemFree(temp_str);
    }
    SysSync();
    return TRUE;
}


#endif

BOOL PanelUpdate(ProgUpdate_PG* ppg)
{
	UI32 value;
	char filename[64];
    BOOL cmd_ok;
    cmd_ok = FALSE;
    bneedCompare = FALSE;//20230518 chj 面板更新比较

	if(check_usb())  //!FileExist(USB_PATH)
	{
		ShowMsg(VW_MSG_UDISKNOTEXIST);
		return FALSE;

	}
	//value = VarNameToInt(PANELCODE_NAME);
	value = VarAdrToUI16(TMP_PANELCODE);
#ifdef WIN32
	snprintf(filename,sizeof(filename)-1,"%s%03d.bat",FILE_PATH,value);
#else
	snprintf(filename,sizeof(filename)-1,"%s%03d",FILE_PATH,value);
#endif

	if(!FileExist(filename))
	{
		ShowMsg(VW_MSG_UDISKNOFILE);
		return FALSE;
	}

	ButtonSetText(ppg->button42,GetTextTran(TEXT_UPDATING));
	WGTSetVisible(ppg->label50,TRUE);
    WGTSetVisible(ppg->label51,TRUE);//20201214

    ProtRebootWait();//更新时面板只发送数据，不接收主机数据 + //20220228 dyl

	//force refresh 
	PageFramReFresh(PanelCurPage(),TRUE);
#ifdef USE_QT
    QtEventLoop();  //20190329 hz
#else
	GUX_Exec();
#endif

    if(value != 6666)//20211231 dyl
    {
    #ifdef IMX6
        cmd_ok = SetCmdList(filename);
    #endif //end IMX6

        if(cmd_ok == TRUE)
        {
            #ifdef USE_QT
            #ifndef WIN32
            if(bneedCompare)//20230518 chj 面板更新比较
            {
                if(Compare_filename())
                {
                    CompareToPanel();
                    VarAdrSetInt(PANELCODE_NAME,value);
                    PanelRecAdd(ppg->button42, PNL_UPDATE, 0);
                    VarAdrSaveInt(PANELCODE_NAME);
                    {
                        g_question.pOkfunc = PanelMainRestart;
                        g_question.pCancelfunc = ProtRebootRecover; //20230518 chj 更新完面板后，按取消，需要恢复通讯
                        ShowQuestion(PANEL_UPOK,g_question);
                    }
                }
                else
                {
                    BakToPanel();
                    ShowMsg(VW_MSG_UPDATEPACKFAIL);
                    PanelRecAdd(ppg->button42, PNL_UPDATE_PACK_FAIL, 0);
                }
            }
            #else
            if(0)
            {
                ;
            }
            #endif
            #endif
            else {
                VarAdrSetInt(PANELCODE_NAME,value);
                VarAdrSaveInt(PANELCODE_NAME);
                {
                    g_question.pOkfunc = PanelMainRestart;
                    g_question.pCancelfunc = ProtRebootRecover;//20220228 dyl
                    ShowQuestion(PANEL_UPOK,g_question);
                }
            }

        }
    }
    else
    {
        //特殊程序处理：开机将开模数（不清零）数据清零
        VarAdrSetInt(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL,0);
        VarAdrSaveInt(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL);
        VarAdrSetInt(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL,0);
        VarAdrSaveInt(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL);
        VarAdrSetInt(p_PP_MACHSET_SHOTCNT_TOTAL, 0);
        VarAdrSaveInt(p_PP_MACHSET_SHOTCNT_TOTAL);
        //2015.12.22 dyl 开模数不清零
        VarAdrSetInt(p_PP_STATE_SHOT_TOTAL_CNT, 0);
    }

    bpanelsuccess = FALSE;//20220228 dyl //20230518 chj 更新完面板后，按取消，需要恢复通讯

    WGTSetVisible(ppg->label50,FALSE);
    WGTSetVisible(ppg->label51,FALSE);//20201214
    ButtonSetText(ppg->button42,GetTextTran(TEXT_UPDATE));

    ClearCompareFile();//20230518 chj 面板更新比较

	return TRUE;
}

void CBUpdate(void* pdat, int state, int progress) // state   ：更新状态，progress更新进度
{
	if(state !=UPDATE_TMOUT)
	{
		VarAdrSetInt(ADR_PROGRESS_VALUE,progress);
	}

    if(state != UPDATE_START && state != UPDATE_ING) //20250306 chj 更新开始时，也不需要Free内存
	{
        OBJMemFree(pdat);
	}

	if(state == UPDATE_OK)
	{
		MainProtReset();
		ShowQuestion(MAIN_UPOK,g_question);
		bmainsuccess = TRUE;
        VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
	}
	if(state == UPDATE_TMOUT)
	{
        MainProtReset();
		ShowMsg(UPDATE_FAIL);
        VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
		bmainsuccess = TRUE;
	}
}

BOOL MainUpdate(ProgUpdate_PG* ppg)
{
	UI32 value1,value2,value3;
	char filename[64];

	if(check_usb())  //!FileExist(USB_PATH)
	{
		ShowMsg(VW_MSG_UDISKNOTEXIST);
		return FALSE;
	}

	//value1 = VarNameToInt(CONFIGCODE_NAME);
	//value2 = VarNameToInt(MANUFCODE_NAME);
	//value3 = VarNameToInt(PROGRAMCODE_NAME);
	value1 = VarAdrToUI16(TMP_CONFIGCODE);
	value2 = VarAdrToUI16(TMP_MANUFCODE);
	value3 = VarAdrToUI16(TMP_PROGRAMCODE);
	VarAdrSetInt(ADR_PROGRESS_VALUE,0);
	snprintf(filename,sizeof(filename)-1,"%s%04d%04d%04d.bin",MAIN_PATH,value1,value2,value3);
	if(FileExist(filename))
	{
		FILEHD filehd;
		int size;
		UI8* buf = NULL;
		filehd = FileOpen(filename);
		size = FileGetSize(filehd, filename);
		buf = OBJMemMalloc(size);
		if(buf != NULL)
		{
			FileRead(filehd,buf,size,NULL);
			FileClose(filehd);
			ProtUpgradReset(1);//重启
			ButtonSetText(ppg->button43,GetTextTran(TEXT_UPDATING));
			if(StartUpdate(buf,size,CBUpdate))
			{
				//send prog
				VarAdrSetInt(ADR_PROGRESS_MAX,100);
				g_question.pOkfunc = MainUpdateOK;
				g_question.pCancelfunc = NULL;
				WGTSetVisible(ppg->label51,TRUE);
				//save code
				VarAdrSetInt(CONFIGCODE_NAME,value1);
				VarAdrSaveInt(CONFIGCODE_NAME);
				VarAdrSetInt(MANUFCODE_NAME,value2);
				VarAdrSaveInt(MANUFCODE_NAME);
				VarAdrSetInt(PROGRAMCODE_NAME,value3);
				VarAdrSaveInt(PROGRAMCODE_NAME);
			}
			else
			{
				ShowMsg(UPDATE_FAIL);
				VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
			}

            PageWgtSetFocus(ppg->edit20); //  20190110 hz
            VarAdrSetInt(PROGUPDATE_FLAG,1);//update flag
		}
	}
	else
	{
		snprintf(filename,sizeof(filename)-1,"%s%04d%04d%04d.tbl",CFG_PATH,value1,value2,value3);
		if(FileExist(filename))	//send cfg
		{
			FILEHD filehd;
			int size;
			UI8* buf = NULL;
			filehd = FileOpen(filename);
			size = FileGetSize(filehd, filename);
			buf = OBJMemMalloc(size);
			if(buf != NULL)
			{
				FileRead(filehd,buf,size,NULL);
				FileClose(filehd);
				ButtonSetText(ppg->button43,GetTextTran(TEXT_UPDATING));
				PackTxTypeStart(TYPE_TABLE_PART,buf,size,PacketFuncCallBack);
				g_question.pOkfunc = MainUpdateOK;
				g_question.pCancelfunc = NULL;
				//save code
				VarAdrSetInt(CONFIGCODE_NAME,value1);
				VarAdrSaveInt(CONFIGCODE_NAME);
				VarAdrSetInt(MANUFCODE_NAME,value2);
				VarAdrSaveInt(MANUFCODE_NAME);
				VarAdrSetInt(PROGRAMCODE_NAME,value3);
				VarAdrSaveInt(PROGRAMCODE_NAME);

                PageWgtSetFocus(ppg->edit20);//  20190110 hz
                VarAdrSetInt(PROGUPDATE_FLAG,1);//update flag
			}
		}
		else
		{
			ButtonSetText(ppg->button43,GetTextTran(TEXT_UPDATE));
			ShowMsg(VW_MSG_UDISKNOFILE);
			return FALSE;
		}
	}
	return TRUE;
}

static void PacketFuncCallBack(void* pdat, int para1, int para2)
{
	if(para2 == 0xffff) //结束
	{
		OBJMemFree(pdat);
		ShowQuestion(MAIN_UPOK,g_question);
		bmainsuccess = TRUE;
        VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
	}
	
}

BOOL PageShowInit(ProgUpdate_PG* ppg)
{
	WGTSetVisible(ppg->label50,FALSE);
	WGTSetVisible(ppg->label51,FALSE);
	VarAdrSetInt(ADR_PROGRESS_VALUE,0);
	VarAdrSetInt(ADR_PROGRESS_MAX,100);
	g_question.pOkfunc = PanelMainRestart;
	g_question.pCancelfunc = NULL;

	ButtonSetText(ppg->button42, GetTextTran(TEXT_UPDATE));
	ButtonSetText(ppg->button43, GetTextTran(TEXT_UPDATE));

	VarAdrSetInt(TMP_PANELCODE,VarAdrToUI16(PANELCODE_NAME));
	VarAdrSetInt(TMP_CONFIGCODE,VarAdrToUI16(CONFIGCODE_NAME));
	VarAdrSetInt(TMP_MANUFCODE,VarAdrToUI16(MANUFCODE_NAME));
	VarAdrSetInt(TMP_PROGRAMCODE,VarAdrToUI16(PROGRAMCODE_NAME));

    ProgbarSetCurValue(ppg->panelbar1,0);
    //KeyTestGuideSwtich();//面板按键引导  20181206 hz

    //20221111 dyl 最高等级增加更新代码前缀显示
    if (GetCurrentUserId() >= USER_SUPERVISOR)
    {
        WGTSetVisible(ppg->stcode,TRUE);
        char text[10];
        snprintf(text, sizeof(text), "%s", CODE);
        LabelSetText(ppg->stcode, text);
    }
    else
    {
        WGTSetVisible(ppg->stcode,FALSE);
    }

	return TRUE;
}

//void PanelUpdateOK()
//{
//#ifdef WIN32
//	oprintf("reboot\n");
//#else
//	osystem("reboot");
//#endif
//}

void MainUpdateOK()
{
	ProtUpgradReset(0);

    bmainreboot = TRUE;
    InitMainHand();
    WGTSetEnable(progupdate_pg.button43, FALSE);
}

//20240925 fqh IAP
/***********start************/
static UI16 GetCANIDByType_New(int IAP)
{
    UI16 can_id;
    if(IAP==0) //E400B
    {
        can_id = 7;
    }
    else if(IAP==1) //E500B
    {
        can_id = 9;
    }
    else if(IAP==2) //E501B2
    {
        can_id = 11;
    }
    else if(IAP==3) //E501B
    {
        can_id = 13;
    }
    else if(IAP==4) //E502
    {
        can_id = 12;
    }
    else if(IAP==5) //E700B
    {
        can_id = 5;
    }
    else if(IAP==6) //20240918 chj 增加液压阀ID
    {
        can_id = 8;
    }

    else
    {
        can_id = 0;
    }
    return can_id;
}

BOOL IAPInit(ProgUpdate_PG* ppg)
{
    VarAdrSetInt(ADR_PROGRESS2_VALUE,0);
    VarAdrSetInt(ADR_PROGRESS2_MAX,100);
    ButtonSetText(ppg->btIAP_update, GetTextTran(TEXT_UPDATE));
    ButtonSetText(ppg->btIAP_Force,  GetTextTran(TEXT_UPDATE));

    VarAdrSetInt(TMP_IAPCODE,VarAdrToUI16(p_PP_MACHSET_IAP_BOARD_CODE));
    VarAdrSetInt(TMP_IAPTYPE_NEW,0);
    VarAdrSetInt(TMP_IAPNUM, VarAdrToUI16(p_PP_MACHSET_IAP_NUMBER));
    VarAdrSetInt(TMP_IAPFORCE, 0);
    WGTSetEnable(PageGetWgtByName(progupdate_pg_name,"btIAP_Force"),FALSE);

    SetIapRebootFlag(FALSE);//扩展板重启标志置FALSE
    ResetStart();//重置标记位归零
    biapupdate_start = FALSE;
    Num_ForceUpdateFalse=0;//用于判断强制更新是否失败，超过2次则认为CAN模块需重启
}

BOOL IAPUpdate(ProgUpdate_PG* ppg)
{
    UI32 value1;
    char filename[64];

    if(check_usb())  //!FileExist(USB_PATH)
    {
        ShowMsg(VW_MSG_UDISKNOTEXIST);
        return FALSE;
    }

    value1 = VarAdrToUI16(TMP_IAPCODE);
    g_CurrentChoose_Iap = GetCANIDByType_New(VarAdrToInt(TMP_IAPTYPE_NEW));
    if(g_CurrentChoose_Iap == 9) //E500B
    {
        g_CurrentChoose_IapNum = 0;
    }
    else
    {
        g_CurrentChoose_IapNum = VarAdrToUI16(TMP_IAPNUM);
    }
    VarAdrSetInt(ADR_PROGRESS2_VALUE,0);
    ResetStart();//重置标记位归零
    snprintf(filename,sizeof(filename)-1,"%s%04d.bin",IAP_PATH,value1);
    if(FileExist(filename))
    {
        CreateTaskCan(CAN_TX_IAPREBOOT,(g_CurrentChoose_Iap<<3|g_CurrentChoose_IapNum),1,0);//发送扩展板重启命令
        biapupdate_start = TRUE;
        ButtonSetText(ppg->btIAP_update,GetTextTran(TEXT_UPDATING));
        VarAdrSetInt(PROGUPDATE_FLAG,1);//update flag
        time_1s = GetTick();
        time_1sx = GetTick();
        second=0;
        second2=0;
        return TRUE;
    }
    else
    {
        ButtonSetText(ppg->btIAP_update,GetTextTran(TEXT_UPDATE));
        VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
        ShowMsg(VW_MSG_UDISKNOFILE);
        return FALSE;
    }

}

BOOL IAPForceUpdate(ProgUpdate_PG* ppg)
{
    UI32 value1;
    char filename[64];
    if(check_usb())  //!FileExist(USB_PATH)
    {
        ShowMsg(VW_MSG_UDISKNOTEXIST);
        return FALSE;
    }

    value1 = VarAdrToUI16(TMP_IAPCODE);
    g_CurrentChoose_Iap = GetCANIDByType_New(VarAdrToInt(TMP_IAPTYPE_NEW));
    if(g_CurrentChoose_Iap == 9) //E500B
    {
        g_CurrentChoose_IapNum = 0;
    }
    else
    {
        g_CurrentChoose_IapNum = VarAdrToUI16(TMP_IAPNUM);
    }
    VarAdrSetInt(ADR_PROGRESS2_VALUE,0);
    ResetStart();//重置标记位归零
    snprintf(filename,sizeof(filename)-1,"%s%04d.bin",IAP_PATH,value1);
    if(FileExist(filename))
    {
        CreateTaskCan(CAN_TX_IAPREBOOTFORCE,(g_CurrentChoose_Iap<<3|g_CurrentChoose_IapNum),1,0);//发送扩展板重启命令
        biapupdate_start = TRUE;
        ButtonSetText(ppg->btIAP_Force,GetTextTran(TEXT_UPDATING));
        VarAdrSetInt(PROGUPDATE_FLAG,1);//update flag
        time_1s = GetTick();
        time_1sx = GetTick();
        second=0;
        second2=0;
        Num_ForceUpdateFalse++;//点击一次则记录一次，更新成功则清零，如果累计数大于等于2次，则认为CAN模块挂掉，需重启
        if(Num_ForceUpdateFalse>=2)
        {
            osystem("canconfig can0 stop");//用于CAN模块由于干扰挂掉 重启
            osystem("canconfig can0 start");
            Num_ForceUpdateFalse=0;
        }
        return TRUE;
    }
    else
    {
        ButtonSetText(ppg->btIAP_Force,GetTextTran(TEXT_UPDATE));
        VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
        ShowMsg(VW_MSG_UDISKNOFILE);
        return FALSE;
    }
}

/**************end**************/

static void CtlConnect()
{
}
static int ProgUpdateOnInit(ProgUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	
    return 0;
}
static int ProgUpdateOnShow(ProgUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

	PageShowInit(ppg);

    IAPInit(ppg);//20211229 dyl IAP

    return 0;
}
static int ProgUpdateOnHide(ProgUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    if(!bpanelsuccess)//20220228 dyl
    {
        bpanelsuccess = TRUE;
        ProtRebootRecover();
    }

    return 0;
}
static int ProgUpdateOnUpdate(ProgUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    if(VarAdrToUI16(PROGUPDATE_FLAG)==1)//20250319 jhh 更新画面主机更新过程中不进入屏保
    {
        Reset_PanelNoKeyPress();
    }

    if(bmainsuccess)
	{
		bmainsuccess = FALSE;
		ButtonSetText(ppg->button43,GetTextTran(TEXT_UPDATE));
        WGTSetVisible(ppg->label51,FALSE);
	}

    if(bmainreboot && GetMainHand())
    {
        bmainreboot = FALSE;
        WGTSetEnable(progupdate_pg.button43, TRUE);
        ShowMsg(VW_MSG_HOSTSTARTOVER);
    }

    //20211229 dyl IAP
    //20201106 CHZ   更新过程中判断发送的包号与接受到的包号不相同的话，等待15S，如果还是不相同，则认为更新失败  失败后将标志清空
    if((g_nPrgPack != GetSend_IAPPack()) && IsIapSending())
    {
        if(GetSend_IAPPack()>1)//用于判断是否处于更新过程中
        {
            if (GetTick()-time_1sx >=1000){//防止更新过程中中断
                time_1sx =  GetTick();
                second2++;
                if(second2>=15)
                {
                    second2=0;
                    Iap_SendCancel();
                    biapupdate_start=FALSE;
                    VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
                    VarAdrSetInt(ADR_PROGRESS2_VALUE, 0);
                    ShowMsg(VW_MSG_IAPFAIL);//MsgN_View(VW_PROG_IAPFAIL);
                    ButtonSetText(ppg->btIAP_update,GetTextTran(TEXT_UPDATE));
                    ButtonSetText(ppg->btIAP_Force,GetTextTran(TEXT_UPDATE));
                }
            }
        }
        if (GetTick()-time_1s >=1000){
            time_1s =  GetTick();
            second++;
            if(second>=15)
            {
                Iap_SendCancel();
                biapupdate_start=FALSE;
                VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
                VarAdrSetInt(ADR_PROGRESS2_VALUE, 0);
                ShowMsg(VW_MSG_IAPFAIL);//MsgN_View(VW_PROG_IAPFAIL);
                ButtonSetText(ppg->btIAP_update,GetTextTran(TEXT_UPDATE));
                ButtonSetText(ppg->btIAP_Force,GetTextTran(TEXT_UPDATE));
            }
        }
    }
    else if(biapupdate_start==TRUE) //按下更新后 如果30秒内无法和扩展板通信，则认为更新失败
    {
        if (GetTick()-time_1s >=1000){
            time_1s =  GetTick();
            second++;
            if(second>=30)
            {
                Iap_SendCancel();
                biapupdate_start=FALSE;
                VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
                VarAdrSetInt(ADR_PROGRESS2_VALUE, 0);
                ShowMsg(VW_MSG_IAPFAIL);//MsgN_View(VW_PROG_IAPFAIL);
                ButtonSetText(ppg->btIAP_update,GetTextTran(TEXT_UPDATE));
                ButtonSetText(ppg->btIAP_Force,GetTextTran(TEXT_UPDATE));
            }
        }
    }

    if (IsIapRebootSuccess())//重启成功
    {
        SetIapRebootFlag(FALSE);
        UI32 nIndex = VarAdrToUI16(TMP_IAPCODE);
        if(IsIapSending()){//如果在发送状态终，则取消
            return 0;
        }
        Iap_Send(nIndex);
        VarAdrSetInt(ADR_PROGRESS2_MAX,100);
    }

    //20201106 CHZ
    if(/*IsIapUpdateOK()*/GetIapUpdateState()==IAPUPDATE_OK){ //扩展板更新成功标志 ，表明扩展板普通更新或者强制更新成功，将部分标志清除
        ResetStart();//重置标记位归零
        second=0;
        second2=0;
        time_1s = GetTick();
        time_1sx = GetTick();
        biapupdate_start=FALSE;
        VarAdrSetInt(PROGUPDATE_FLAG,0);//update flag
        Num_ForceUpdateFalse=0;
        ShowMsg(VW_MSG_IAPSUCCESS);
        ButtonSetText(ppg->btIAP_update,GetTextTran(TEXT_UPDATE));
        ButtonSetText(ppg->btIAP_Force,GetTextTran(TEXT_UPDATE));
    }
    else if(GetIapUpdateState()==IAPUPDATE_ING)//20220118 dyl 当前更新状态为等待
    {
        second2=0;
        time_1sx = GetTick();
    }

    //进度条显示
    if((!(IsIapSending()))&&(GetSend_IAPPack()==0))
    {
        ;
    }
    else
    {
        if(GetSend_IAPPack()!=1) second=0;
        UI16 total_iappack = (UI16)((Total_filesize+sizeof(UI16)*PKG_UPDATE_DATA_LEN - 1)/(sizeof(UI16)*PKG_UPDATE_DATA_LEN));
        UI16 progress_iap;
        if (total_iappack != 0)
        {
            progress_iap = (g_nPrgPack&0x7FFF)*100/total_iappack;
        }
        else {
            progress_iap = 100;
        }
        VarAdrSetInt(ADR_PROGRESS2_VALUE, progress_iap);
    }
    /**************end**************/

    return 0;
}
static int ProgUpdateOnChange(ProgUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr; 
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);
	
    //20220919 dyl progr update protect 程序更新时其他按键无效
    if(VarAdrToUI16(PROGUPDATE_FLAG))
    {
        VarSendSaveSetIntByAdr(addr,msg_value);//原始值
        return 0;
    }

	switch (addr)
	{

	default :
		if(pwgt == ppg->button42)
        {
            if(VarAdrToInt(MOTOR_STATE_STATE) != 0 || VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS) != 0)//2050117 chj 增加切马达电热提示
            {
                ShowMsg(VW_MSG_CLSHEATERANDMOTOR);
                return 0;
            }
            else
            {
                PanelUpdate(ppg);
            }

		}
		else if(pwgt == ppg->button43)
		{
            if(VarAdrToInt(MOTOR_STATE_STATE) != 0 || VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS) != 0)//2050117 chj 增加切马达电热提示
            {
                ShowMsg(VW_MSG_CLSHEATERANDMOTOR);
                return 0;
            }
            else
            {
                MainUpdate(ppg);
            }

		}
        else if(pwgt == ppg->btIAP_update)//20211229 dyl IAP
        {
            IAPUpdate(ppg);
        }
        else if(pwgt == ppg->btIAP_Force)//20211229 dyl IAP
        {
            IAPForceUpdate(ppg);
        }
        else if(pwgt == ppg->editpassword)//20211229 dyl IAP
        {
            if(VarAdrToInt(TMP_IAPFORCE) == 5858)
            {
                WGTSetEnable(PageGetWgtByName(progupdate_pg_name,"btIAP_Force"),TRUE);
            }
            else
            {
                WGTSetEnable(PageGetWgtByName(progupdate_pg_name,"btIAP_Force"),FALSE);
            }
        }
		break;
	}
    return 0;
}
