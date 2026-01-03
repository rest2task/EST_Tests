#include "WData_Operation.h"
#include "typedef.h"
#include "information.h"
#include "dataCfgtb.h"
#include "database.h"
#include "file.h"
#include "dataOperation.h"
#include "editwgt.h"
#include "checkboxwgt.h"
#include "dropwgt.h"
#include "oprintf.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch

#define ADDR_OPERATEMODE		0x33150001		//操作模式对应地址
#define ADDR_MACCOMB			0x331501f4		//机器列表名（dropbox）对应临时地址
#define ADDR_MACCOMB_VAL		0x33150002		//机器列表（dropbox）值对应地址
#define ADDR_TYPEALL			0x33150003		//数据类型 ALL
#define ADDR_TYPEHOST			(ADDR_TYPEALL+1)
#define ADDR_TYPEHMI			(ADDR_TYPEALL+2)
#define ADDR_TYPEMOLD			(ADDR_TYPEALL+3)
#define ADDR_TYPESERVOR			(ADDR_TYPEALL+4)
#define ADDR_TYPEMACHSET        TMPVAL(8)       //数据类型 结构参数
#define ADDR_PASSWD				TMPVAL(9)		//密码对应地址
#define ADDR_STATE		        0x33150000		//状态


typedef enum enVIS_USE_ABLE
{
	UNCHECKED=0,
	CHECKED
}VIS_USE_ABLE;


DEFINE_DATA_OPERATION_MAP

CONNECT_DATA_OPERATION_EVENT

UI16 FileIsExist;
static void ReadUdiskList();
static void UpdateAllCombos();

static void TempAddrInit()
{
	UI32 i=0;
	FileIsExist = 0;
	
	for(i=ADDR_OPERATEMODE;i<=ADDR_PASSWD;i++)
	{
		VarAdrSetInt(i,0);
	}

	EditSetText(data_operation_pg.edit1,VarAdrToStr(SYSTEM_ADDR));
	WGTSetVisible(data_operation_pg.label110,FALSE);
	WGTSetVisible(data_operation_pg.label20,FALSE);
	WGTSetVisible(data_operation_pg.edit1,TRUE);
	WGTSetVisible(data_operation_pg.dropdown3,FALSE);
	WGTSetEnable(data_operation_pg.edit1,TRUE);
	WGTSetEnable(data_operation_pg.button43,TRUE);
	WGTSetEnable(data_operation_pg.button47,FALSE);

	WGTSetEnable(data_operation_pg.checkbox2,TRUE);
	WGTSetEnable(data_operation_pg.checkbox3,TRUE);

	ModInit();
	if(FileExist(MOLD_PATH))
	{
		WGTSetEnable(data_operation_pg.checkbox4,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox4,FALSE);
	}

	if(getServoOperateEnable())
	{
		WGTSetEnable(data_operation_pg.checkbox5,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox5,FALSE);
	}

    if(FileExist(MACHSET_PATH))//20201210
    {
        WGTSetEnable(data_operation_pg.checkbox6,TRUE);
    }
    else
    {
        WGTSetEnable(data_operation_pg.checkbox6,FALSE);
    }
}

BOOL DataOpInit()
{
	TempAddrInit();
	return TRUE;
}

BOOL DataOpUpdate()
{
	UI32 value;

    if(WGTGetEnable(data_operation_pg.checkbox2)&WGTGetEnable(data_operation_pg.checkbox3)&WGTGetEnable(data_operation_pg.checkbox4)
            &WGTGetEnable(data_operation_pg.checkbox5)&WGTGetEnable(data_operation_pg.checkbox6))//20201210
	{
		WGTSetEnable(data_operation_pg.checkbox1,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox1,FALSE);
	}
	
	if(!DoOPMonitor())
	{
        VarAdrSetStr(SETUP_SERVO_MSG,"");
        VarAdrSetStr(SETUP_SERVO_MSGDEF,"");
	}
	
	value = VarAdrToInt(ADDR_OPERATEMODE);

	if(getServoOperateEnable() && (value==EXPORT || value == IMPORT))
	{
		WGTSetEnable(data_operation_pg.checkbox5,TRUE);
		if(value == IMPORT)//20181122
		{
			if(check_usb() || ((FileIsExist & 0x10)==0))
			{
				WGTSetEnable(data_operation_pg.checkbox5,FALSE);
			}

		}
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox5,FALSE);
	}

	if(value == EXPORT || value == IMPORT)
	{
		if(check_usb())//!FileExist(USB_PATH)
		{
			WGTSetVisible(data_operation_pg.label20,TRUE);
			if(value == IMPORT && FileIsExist == 0)
			{
				WGTSetVisible(data_operation_pg.label110,TRUE);
				VarAdrSetStr(ADDR_MACCOMB,"");
			}
			else
			{
				WGTSetVisible(data_operation_pg.label110,FALSE);
			}
		}
		else
		{
			WGTSetVisible(data_operation_pg.label20,FALSE);
			WGTSetVisible(data_operation_pg.label110,FALSE);
			
		}
	}
	else
	{
		WGTSetVisible(data_operation_pg.label20,FALSE);
	}
	return TRUE;
}

BOOL DataOpOperate()
{
	UI32 value;

	CheckboxSetCheck(data_operation_pg.checkbox1,FALSE);
	CheckboxSetCheck(data_operation_pg.checkbox2,FALSE);
	CheckboxSetCheck(data_operation_pg.checkbox3,FALSE);
	CheckboxSetCheck(data_operation_pg.checkbox4,FALSE);
	CheckboxSetCheck(data_operation_pg.checkbox5,FALSE);
    CheckboxSetCheck(data_operation_pg.checkbox6,FALSE);//20201210

	value = VarAdrToInt(ADDR_OPERATEMODE);
	if(value == EXPORT)
	{
		WGTSetVisible(data_operation_pg.dropdown3,FALSE);
		WGTSetVisible(data_operation_pg.edit1,TRUE);
		WGTSetEnable(data_operation_pg.edit1,TRUE);
		WGTSetVisible(data_operation_pg.label110,FALSE);
		WGTSetEnable(data_operation_pg.checkbox2,TRUE);
		WGTSetEnable(data_operation_pg.checkbox3,TRUE);

		if(FileExist(MOLD_FILE))
		{
			WGTSetEnable(data_operation_pg.checkbox4,TRUE);
		}
		else
		{
			WGTSetEnable(data_operation_pg.checkbox4,FALSE);
		}

		if(getServoOperateEnable())
		{
			WGTSetEnable(data_operation_pg.checkbox5,TRUE);
		}
		else
		{
			WGTSetEnable(data_operation_pg.checkbox5,FALSE);
		}

        if(FileExist(MACHSET_PATH))//20201210
        {
            WGTSetEnable(data_operation_pg.checkbox6,TRUE);
        }
        else
        {
            WGTSetEnable(data_operation_pg.checkbox6,FALSE);
        }
	}
	else if(value == IMPORT)
	{
		WGTSetVisible(data_operation_pg.dropdown3,TRUE);
		WGTSetVisible(data_operation_pg.edit1,FALSE);
		WGTSetVisible(data_operation_pg.label110,FALSE);

		ReadUdiskList();
		UpdateAllCombos();
	}
	else if((value == RESTORTOSAVE) || (value == RESTORTODEF))
	{
		WGTSetEnable(data_operation_pg.edit1,FALSE);
		WGTSetVisible(data_operation_pg.edit1,TRUE);
		WGTSetVisible(data_operation_pg.dropdown3,FALSE);
		WGTSetVisible(data_operation_pg.label110,FALSE);

		WGTSetEnable(data_operation_pg.checkbox2,TRUE);
		WGTSetEnable(data_operation_pg.checkbox3,TRUE);
		WGTSetEnable(data_operation_pg.checkbox4,FALSE);

		/*if((value==RESTORTOSAVE) && getServoOperateEnable() && FileExist(PANEL_SERVO_PATH))
		{
			WGTSetEnable(data_operation_pg.checkbox5,TRUE);
		}
		else*/
		{
			WGTSetEnable(data_operation_pg.checkbox5,FALSE);
		}

        WGTSetEnable(data_operation_pg.checkbox6,FALSE);
	}
	return TRUE;
}
/************************************************************************/
/* CBMACHNAME 值改变                                             */
/************************************************************************/
BOOL DataOpNameChg()
{
	UpdateAllCombos();
	if(FileIsExist&0x100)
	{
		//WGTSetEnable(data_operation_pg.button43,FALSE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.button43,TRUE);
	}
	return TRUE;
}
/************************************************************************/
/* “所有选项按钮”的事件                                         */
/************************************************************************/
BOOL DataOpSelAll()
{
	int value = VarAdrToInt(ADDR_TYPEALL);
	if(value == DISABLE)
	{
		CheckboxSetCheck(data_operation_pg.checkbox2,FALSE);
		CheckboxSetCheck(data_operation_pg.checkbox3,FALSE);
		CheckboxSetCheck(data_operation_pg.checkbox4,FALSE);
		CheckboxSetCheck(data_operation_pg.checkbox5,FALSE);
        CheckboxSetCheck(data_operation_pg.checkbox6,FALSE);
	}
	else
	{
		if(getServoOperateEnable())
		{
			CheckboxSetCheck(data_operation_pg.checkbox5,TRUE);
		}

		CheckboxSetCheck(data_operation_pg.checkbox2,TRUE);
		CheckboxSetCheck(data_operation_pg.checkbox3,TRUE);
		CheckboxSetCheck(data_operation_pg.checkbox4,TRUE);
        CheckboxSetCheck(data_operation_pg.checkbox6,TRUE);
		
	}
	return TRUE;
}

/************************************************************************/
/* "HOST按钮"事件                                                */
/************************************************************************/
BOOL DataOpSelHost()
{
	if(getServoOperateEnable())
	{
        if((VarAdrToInt(ADDR_TYPEHOST)==1) && (VarAdrToInt(ADDR_TYPEHMI)==1) && (VarAdrToInt(ADDR_TYPEMOLD)==1)
                && (VarAdrToInt(ADDR_TYPESERVOR)==1) && (VarAdrToInt(ADDR_TYPEMACHSET)==1))//20201210
		{
			CheckboxSetCheck(data_operation_pg.checkbox1,TRUE);
		}
		else
		{
			CheckboxSetCheck(data_operation_pg.checkbox1,FALSE);
		}
	}
	else
	{
        if((VarAdrToInt(ADDR_TYPEHOST)==1) && (VarAdrToInt(ADDR_TYPEHMI)==1) && (VarAdrToInt(ADDR_TYPEMOLD)==1) && (VarAdrToInt(ADDR_TYPEMACHSET)==1))//20201210
		{
			CheckboxSetCheck(data_operation_pg.checkbox1,TRUE);
		}
		else
		{
			CheckboxSetCheck(data_operation_pg.checkbox1,FALSE);
		}
	}
	return TRUE;
}

/************************************************************************/
/* "hmi按钮"（选择面板按钮）事件                                 */
/************************************************************************/
BOOL DataOpSelHMI()
{
	return DataOpSelHost();
}

/************************************************************************/
/* "MOLD按钮"（选择模组按钮）事件                                */
/************************************************************************/
BOOL DataOpSelMOLD()
{
	return DataOpSelHost();
}

/************************************************************************/
/* "servo按钮"（选择伺服按钮）事件                               */
/************************************************************************/
BOOL DataOpSelServo()
{
	return DataOpSelHost();
}

/************************************************************************/
/* "Machset按钮"（选择结构参数按钮）事件                               */
/************************************************************************/
BOOL DataOpSelMachset()
{
    return DataOpSelHost();
}

/************************************************************************/
/* "PASSWORD"（密码输入）事件                                    */
/************************************************************************/
BOOL DataOpPwd()
{
	if(VarAdrToInt(ADDR_PASSWD) == 95)
	{
		WGTSetEnable(data_operation_pg.button47,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.button47,FALSE);
	}
	return TRUE;
}

static int HasSelect()
{
	int select=0;
	if(CheckboxGetCheck(data_operation_pg.checkbox2))
	{
		select |= TYPEHOST;
	}
	if(CheckboxGetCheck(data_operation_pg.checkbox3))
	{
		select |= TYPEHMI;
	}
	if(CheckboxGetCheck(data_operation_pg.checkbox4))
	{
		select |= TYPEMOLD;
	}
	if(CheckboxGetCheck(data_operation_pg.checkbox5))
	{
		select |= TYPESERVOR;
	}
    if(CheckboxGetCheck(data_operation_pg.checkbox6))//20201210
    {
        select |= TYPEMACHSET;
    }
	return select;
}

BOOL BtnOkClick()
{
	UI16 wOperateMode;
	int select;
	char pbuff[64];
	if((select=HasSelect())==0){
		ShowMsg(VM_MSG_SELECTTYPE);
		return FALSE;
	}
	wOperateMode =VarAdrToUI16(ADDR_OPERATEMODE);
	if(wOperateMode == EXPORT)
	{
		ExportToUdisk(select);
	}
	else if(wOperateMode == IMPORT){
        char* pstr = NULL;//20191223.cyx 支持中英文
        if(check_usb())
        {
            ShowMsg(VW_MSG_UDISKNOTEXIST);
            return FALSE;
        }
        if(VarAdrToInt(MOTOR_STATE_STATE) != 0)
        {
            ShowMsg(VW_MSG_MOTOOPNOTOUTPUT);
            return FALSE;
        }
        DropGetCurItemText(data_operation_pg.dropdown3,pbuff,64);
        pstr = ConvToGBK_Dup(pbuff);
        ImportFromUdisk(select,pstr);
        OBJMemFree(pstr);
	}
	else if(wOperateMode == RESTORTOSAVE){
		RestoreToSave(select);
	}
	else if(wOperateMode == RESTORTODEF){
		RestoreToDefault(select);
	}
	return TRUE;
}

static void ReadUdiskList()
{
	static FILEDIR g_filedir;
	VarAdrSetStr(ADDR_MACCOMB,"");
	if(!FileExist(ESTDATA_PATH))
	{
		return;
	}

	if(GetDir(&g_filedir,ESTDATA_PATH,GETDIR_SUB))//GETDIR_NORMAL
	{	
		char* pinfo =NULL;
		//char *pvalue=NULL;
		if((pinfo=ConvertDirInfoToStr(&g_filedir)) != NULL)
		{
			//VarAdrSetStr(ADDR_MACCOMB,pinfo);
			//pvalue=VarAdrToStr(ADDR_MACCOMB);
			DropSetItems(data_operation_pg.dropdown3,pinfo);
			OBJMemFree((void *)pinfo);
			pinfo=NULL;
		}
	}
}

static void UpdateAllCombos()
{
	char pbuff[64];
    char* pstr = NULL;//20191223.cyx 支持中英文
	DropGetCurItemText(data_operation_pg.dropdown3,pbuff,64);
    pstr = ConvToGBK_Dup(pbuff);
    FileIsExist = DataFileExist(((OP_MODE)VarAdrToInt(ADDR_OPERATEMODE)),pstr);
    OBJMemFree(pstr);
	if(FileIsExist & 0x01)
	{
		WGTSetEnable(data_operation_pg.checkbox1,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox1,FALSE);
        CheckboxSetCheck(data_operation_pg.checkbox1,FALSE);//20191223.cyx 变灰时复选框置FALSE状态
	}

	if(FileIsExist & 0x02)
	{
		WGTSetEnable(data_operation_pg.checkbox2,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox2,FALSE);
        CheckboxSetCheck(data_operation_pg.checkbox2,FALSE);//20191223.cyx 变灰时复选框置FALSE状态
	}

	if(FileIsExist & 0x04)
	{
		WGTSetEnable(data_operation_pg.checkbox3,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox3,FALSE);
        CheckboxSetCheck(data_operation_pg.checkbox3,FALSE);//20191223.cyx 变灰时复选框置FALSE状态
	}

	if(FileIsExist & 0x08)
	{
		WGTSetEnable(data_operation_pg.checkbox4,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox4,FALSE);
        CheckboxSetCheck(data_operation_pg.checkbox4,FALSE);//20191223.cyx 变灰时复选框置FALSE状态
	}

	if(FileIsExist & 0x10)
	{
		WGTSetEnable(data_operation_pg.checkbox5,TRUE);
	}
	else
	{
		WGTSetEnable(data_operation_pg.checkbox5,FALSE);
        CheckboxSetCheck(data_operation_pg.checkbox5,FALSE);//20191223.cyx 变灰时复选框置FALSE状态
	}

    if(FileIsExist & 0x20)//20201210
    {
        WGTSetEnable(data_operation_pg.checkbox6,TRUE);
    }
    else
    {
        WGTSetEnable(data_operation_pg.checkbox6,FALSE);
        CheckboxSetCheck(data_operation_pg.checkbox6,FALSE);//20191223.cyx 变灰时复选框置FALSE状态
    }

    DataOpSelHost();//20191223.cyx 全部选择完善
}


static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button47OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
	WgtMsgConnect(data_operation_pg.button43, WGT_CLICK_MSG, button43OnClick);
	WgtMsgConnect(data_operation_pg.button47, WGT_CLICK_MSG, button47OnClick);
}
static int Data_OperationOnInit(Data_Operation_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Data_OperationOnShow(Data_Operation_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

	DataOpInit();
	VarAdrSetInt(ADDR_STATE,0);
    return 0;
}
static int Data_OperationOnHide(Data_Operation_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Data_OperationOnUpdate(Data_Operation_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

	DataOpUpdate();

    return 0;
}
static int Data_OperationOnChange(Data_Operation_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr; 
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	switch (addr)
	{
	case ADDR_OPERATEMODE:
		DataOpOperate();
		break;
	case ADDR_TYPEALL:
		DataOpSelAll();
		break;
	case ADDR_TYPEHOST:
		DataOpSelHost();
		break;
	case ADDR_TYPEHMI:
		DataOpSelHMI();
		break;
	case ADDR_TYPEMOLD:
		DataOpSelMOLD();
		break;
	case ADDR_TYPESERVOR:
		DataOpSelServo();
		break;
    case ADDR_TYPEMACHSET://20201210
        DataOpSelMachset();
        break;
	case ADDR_MACCOMB_VAL:
		DataOpNameChg();
		break;
	case ADDR_PASSWD:
		DataOpPwd();
		break;
	default :
		break;
	}
    return 0;
}

static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	BtnOkClick();
	return 0;
}

static int button47OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	DataOpSave();
    PanelRecAdd(data_operation_pg.button47, PNL_SAVE, 0);
    ShowMsg(VW_MSG_SAVESUCCESS);
	return 1;
}
