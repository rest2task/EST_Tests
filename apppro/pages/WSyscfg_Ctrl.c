#include "WSyscfg_Ctrl.h"
#include "typedef.h"
#include "pagefram.h"
#include "stdio.h"
#include "database.h"
#include "information.h"
#include "file.h"
#include "common.h"
#include "protcmd.h"
#include "questionDlg.h"
#include "texttbl.h"
#include "labelwgt.h"
#include "oprintf.h"
#include "privdef.h"
#include "ctrlcmd.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_SYSCFG_CTRL_MAP

CONNECT_SYSCFG_CTRL_EVENT

#define TMP_CONFIGTYPE	TMPVAL(1)  //配置类型 临时变量1
#define TMP_CONFIGCODE	TMPVAL(2)  //配置代码 临时变量2
#define TMP_DELETEPWS	TMPVAL(3)  //删除密码 临时变量3

static QUESTION g_question;
static BOOL     bsavesuccess = FALSE;
static BOOL     brequest = FALSE;
static UI16     second=0;//时间

static void MainCfgDelete()
{
	ProtSysCtrl(CFG_ERASE_CMD,0,NULL);
}


/************************************************************************/
/*数据保存的回调函数                                                    */
/************************************************************************/
static void ReceivePackDataSave(void* pdata,UI16 len)
{
	FILEHD hfile;
	char cpath[255];
	UI16 index,index1,index2;
	int nwrite;

    index= VarAdrToUI16(TMP_CONFIGTYPE);//配置类型
	index1 = VarAdrToUI16(TMP_CONFIGCODE);//配置代码,路径
	index2 = index1 + 0x1000;

	if(!FileExist(USB_MAIN_PATH))
	{
		MkDir(USB_MAIN_PATH);
	}

	snprintf(cpath,sizeof(cpath)-1,USB_MAIN_PATH"cfg-%04d%04d%x.tbl",index,index,index2);

    if(FileExist(cpath))//保存之前先删除原有文件
    {
        FileDelete(cpath);
    }

    hfile = FileCreate(cpath);
    if(FILEHD_IS_OK(hfile))
    {
        FileSeek(hfile,0);
        FileWriteUdisk(hfile,pdata,len,&nwrite);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync
    }
	FileClose(hfile); 

	bsavesuccess = TRUE;

}


static void CtlConnect()
{
}
static int Syscfg_CtrlOnInit(Syscfg_Ctrl_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PackRxDataCbSet(ReceivePackDataSave);
    return 0;
}
static int Syscfg_CtrlOnShow(Syscfg_Ctrl_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

	VarAdrSetInt(TMP_CONFIGTYPE,1000);
	VarAdrSetInt(TMP_CONFIGCODE,0);
	VarAdrSetInt(TMP_DELETEPWS,0);

	WGTSetEnable(ppg->button32,FALSE);
	WGTSetVisible(ppg->stInfo,FALSE);
	WGTSetVisible(ppg->stInfo1,FALSE);

	bsavesuccess = FALSE;
    brequest = FALSE;
	second =0;

    return 0;
}
static int Syscfg_CtrlOnHide(Syscfg_Ctrl_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    return 0;
}
static int Syscfg_CtrlOnUpdate(Syscfg_Ctrl_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{

//    if ((OperateModeIndex() != MODE_MANUAL|| VarAdrToUI16(MOTOR_STATE_STATE)!=0) && (PageFrameCurFocusWgt() == ppg->edit21))
//	{
//		PageFramSetProtect(TRUE);
//	}
//	else
//	{
//		PageFramSetProtect(FALSE);
//	}
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

	if(bsavesuccess)
	{
		bsavesuccess = FALSE;
		brequest = FALSE;
		WGTSetVisible(ppg->stInfo1,FALSE);
		LabelSetText(ppg->stInfo,GetTextTran(TEXT_EXPORTSUCCESS));//配置导出成功
	}

	if(brequest && !bsavesuccess)
	{
		static UI32 time_1s=0;

		if(GetTick()-time_1s >1000)
		{
			second++;
			time_1s = GetTick();
			if(second>7)
			{
				second = 0;
				brequest = FALSE;
				bsavesuccess = FALSE;
				LabelSetText(ppg->stInfo,GetTextTran(TEXT_TIMEOUT));//导出超时失败，请先更换配置代码
			}
		}

	}

    return 0;
}

static int Syscfg_CtrlOnChange(Syscfg_Ctrl_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{

	UI32 addr; 
	PATTR pattr;
	UI16 value;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	switch (addr)
	{
	case TMP_DELETEPWS:
		value = VarAdrToUI16(TMP_DELETEPWS);
		if(value == 95)
		{
			WGTSetEnable(ppg->button32,TRUE);
		}
		else
		{
			WGTSetEnable(ppg->button32,FALSE);  //20181225 hz
		}
		break;

	default:
		if(pwgt == ppg->button31)
		{
		    UI16 index1,index2;

			/*need U disk exist*/
			if(check_usb())
			{
				ShowMsg(VW_MSG_UDISKNOTEXIST);
				return FALSE;
			}

            //20210707.cyx
            if((OperateModeIndex() == MODE_MANUAL) && (VarAdrToUI16(MOTOR_STATE_STATE)==0)) //全自动和马达开时不能操作
            {

            }
            else
            {
                ShowMsg(VM_MSG_MANUAL_CUTMOTOR);
                return FALSE;
            }

			index1 = VarAdrToUI16(TMP_CONFIGCODE);//配置代码,路径
			index2 = index1 + 0x1000;

			WGTSetVisible(ppg->stInfo,TRUE);
			LabelSetText(ppg->stInfo,GetTextTran(TEXT_EXPORTCFG));//导出配置中，请等待...
			WGTSetVisible(ppg->stInfo1,TRUE);

			if(index2>0x1004 || index2<0x1000)
			{
				WGTSetVisible(ppg->stInfo,TRUE);
				WGTSetVisible(ppg->stInfo1,FALSE);
				LabelSetText(ppg->stInfo,GetTextTran(TEXT_ERRCODE));//代码错误
			}
			else
			{
				PackRxTypeStart(index1+0x1000);//请求包类型
				brequest = TRUE;
				second = 0;
			}
		}
		else if(pwgt == ppg->button32)
		{
            if((OperateModeIndex() == MODE_MANUAL) && (VarAdrToUI16(MOTOR_STATE_STATE)==0)) //全自动和马达开时不能操作
			{
				g_question.pOkfunc = MainCfgDelete;//delete OK;
				g_question.pCancelfunc = NULL;
				ShowQuestion(VM_MSG_RESETCFG,g_question);
			}
			else 
			{
				ShowMsg(VM_MSG_MANUAL_CUTMOTOR);
				return FALSE;
			}
        }
		break;
	}

    return 0;
}
