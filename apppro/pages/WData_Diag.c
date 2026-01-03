#include "WData_Diag.h"
#include "typedef.h"
#include "pagefram.h"
#include "vardb.h"
#include "dataRequest.h"
#include "oprecord.h"
#include "privdef.h"
#include "taskmoni.h"
#include "texttbl.h"
#include "protcmd.h"
#include "ctrlcmd.h"
#include "information.h"
#include "database.h"
#include "warndatarec.h"
#include "command.h"
#include "keymod.h"
#include "keydef.h"

DEFINE_DATA_DIAG_MAP

CONNECT_DATA_DIAG_EVENT

#define READ_RAM_ID		(100+0x100)

#define ADR_REQUEST		0x33150000
#define ADR_SHOWVALUE	0x33150001

#define TEMP_LOST_E5021  0x33150005
#define TEMP_LOST_E5022  0x33150006

#define TEMP_STR_E5021   0x331501f4
#define TEMP_STR_E5022   0x331501f5

//20200407HJM 模温功能
#define TEMP_STR_E5011   0x331501F7
#define TEMP_STR_E5012   0x331501F8
#define TEMP_STR_E5013   0x331501F9
#define TEMP_STR_E5014   0x331501FA
#define TEMP_STR_E5015   0x331501FB
#define TEMP_STR_E5016   0x331501FC
#define TEMP_STR_E5017   0x331501FD
#define TEMP_STR_E700B   0x331501FE

#define WRITE_CNT_E502_1  0x0A16004C	//E502_1通讯测定写入次数
#define READ_CNT_E502_1   0x0A16004D	//E502_1通讯测试读入次数
#define WRITE_CNT_E502_2  0x0A16004E	//E502_2通讯测定写入次数
#define READ_CNT_E502_2   0x0A16004F	//E502_2通讯测试读入次数

//20200407HJM 模温功能
#define WRITE_CNT_E501_1  0x2f160036	//E501_1通讯测定写入次数
#define READ_CNT_E501_1   0x2f160037	//E501_1通讯测试读入次数
#define TEMP_LOST_E5011   0x33150007
#define TEMP_LOST_E700B   0x3315000e

#define MAX_MT_CNT 7 //模温部件最大数量

#define E501_1_SHOW     TMPVAL(50) //E501_1可见
#define E700B_SHOW      TMPVAL(57) //E700B可见

static BOOL e5021 = FALSE;
static BOOL e5022 = FALSE;

//20200407HJM 模温功能
static BOOL e5011 = FALSE;
static BOOL e5012 = FALSE;
static BOOL e5013 = FALSE;
static BOOL e5014 = FALSE;
static BOOL e5015 = FALSE;
static BOOL e5016 = FALSE;
static BOOL e5017 = FALSE;

static BOOL e700b = FALSE;//20200603.cyx

//20190528 hz
static void LostRate()
{
    UI32 send, recive;
    int i;

    //E502-1
    if(VarAdrToInt(TEMP_CTRL_PART))
    {
        WGTSetAttrByName(data_diag_pg.edit41,EDIT_TEXT_NAME,d_TempCtrlComm1_STATE_WRITE_CNT_E502);
        WGTSetAttrByName(data_diag_pg.edit42,EDIT_TEXT_NAME,d_TempCtrlComm1_STATE_READ_CNT_E502);
        send = VarAdrToInt(d_TempCtrlComm1_STATE_WRITE_CNT_E502);
        recive = VarAdrToInt(d_TempCtrlComm1_STATE_READ_CNT_E502);
    }
    else
    {
        WGTSetAttrByName(data_diag_pg.edit41,EDIT_TEXT_NAME,WRITE_CNT_E502_1);
        WGTSetAttrByName(data_diag_pg.edit42,EDIT_TEXT_NAME,READ_CNT_E502_1);
        send = VarAdrToInt(WRITE_CNT_E502_1);
        recive = VarAdrToInt(READ_CNT_E502_1);
    }

    if(send == 0)
    {
        VarAdrSetInt(TEMP_LOST_E5021, 0);
    }
    else
    {
        VarAdrSetInt(TEMP_LOST_E5021, abs(send - recive)* 100 / send);
    }

    //E502-2
    if(VarAdrToInt(TEMP_CTRL_PART))
    {
        WGTSetAttrByName(data_diag_pg.edit44,EDIT_TEXT_NAME,d_TempCtrlComm2_STATE_WRITE_CNT_E502);
        WGTSetAttrByName(data_diag_pg.edit45,EDIT_TEXT_NAME,d_TempCtrlComm2_STATE_READ_CNT_E502);
        send = VarAdrToInt(d_TempCtrlComm2_STATE_WRITE_CNT_E502);
        recive = VarAdrToInt(d_TempCtrlComm2_STATE_READ_CNT_E502);
    }
    else
    {
        WGTSetAttrByName(data_diag_pg.edit44,EDIT_TEXT_NAME,WRITE_CNT_E502_2);
        WGTSetAttrByName(data_diag_pg.edit45,EDIT_TEXT_NAME,READ_CNT_E502_2);
        send = VarAdrToInt(WRITE_CNT_E502_2);
        recive = VarAdrToInt(READ_CNT_E502_2);
    }

    if(send == 0)
    {
        VarAdrSetInt(TEMP_LOST_E5022, 0);
    }
    else
    {
        VarAdrSetInt(TEMP_LOST_E5022, abs(send - recive)* 100 / send);
    }

    //20200407HJM 模温功能
    //E501
    for(i = 0; i < MAX_MT_CNT; i++)
    {
        send = VarAdrToInt(WRITE_CNT_E501_1 + i*0x100000);
        recive = VarAdrToInt(READ_CNT_E501_1 + i*0x100000);

        if(send == 0)
        {
            VarAdrSetInt(TEMP_LOST_E5011+i, 0);
        }
        else
        {
            VarAdrSetInt(TEMP_LOST_E5011+i, abs(send - recive)* 100 / send);
        }
    }

    //20200603.cyx E700B
    if(VarAdrToInt(SYS_FL_MACH_CODE54)&0x800)
    {
        send = VarAdrToInt(d_machine1_STATE_WRITE_CNT_E700B);
        recive = VarAdrToInt(d_machine1_STATE_READ_CNT_E700B);

        if(send == 0)
        {
            VarAdrSetInt(TEMP_LOST_E700B, 0);
        }
        else
        {
            VarAdrSetInt(TEMP_LOST_E700B, abs(send - recive)* 100 / send);
        }
    }
}

BOOL DataInit()
{
	int i=0;
	VarAdrSetInt(ADR_REQUEST,0);
	for(i=0;i<4;i++)
	{
		VarAdrSetInt(ADR_SHOWVALUE+i,0);
	}

	return TRUE;
}

static void ProdRecvData(UI16 set_id, int num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr)
{
	if(set_id == READ_RAM_ID)
	{
		int i=0;
		for(i=0;i<num;i++)
		{
			VarAdrSetInt(ADR_SHOWVALUE+i,pdata[i]);
		}
	}
}

//开始测试时禁止切自动,调模,清料
static BOOL ChangeModeForbid(int key,int press)
{
    BOOL is_test = FALSE;
    if(e5021 || e5022 || e5011 || e5012 || e5013 || e5014 || e5015 || e5016 || e5017 || e700b)
    {
        is_test = TRUE;
    }

    if(press && is_test && ((ProtRxCnt() > 60) && MainProtComIsOK()))
    {
        if((key== _KEY_PANEL_SEMIAUTO) || (key==_KEY_PANEL_SENSORAUTO) || (key==_KEY_PANEL_TIMEAUTO)
                || (key==_KEY_PANEL_MOLDADJ) || (key==_KEY_PANEL_AUTOMOLDADJ) || (key==_KEY_PANEL_MOLDADJNEW)
                || (key==_KEY_PANEL_AUTOPURGE))
        {
            if(VarAdrToInt(INFORMATION_INDEX) != VW_MSG_SHUTDOWNTEST)//当前未弹出提示框
            {
                ShowMsg(VW_MSG_SHUTDOWNTEST);
            }

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button45OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button46OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button47OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button48OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button49OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button50OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button51OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button52OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);

static void CtlConnect()
{
    WgtMsgConnect(data_diag_pg.button42, WGT_CLICK_MSG, button42OnClick);
    WgtMsgConnect(data_diag_pg.button43, WGT_CLICK_MSG, button43OnClick);

    WgtMsgConnect(data_diag_pg.button45, WGT_CLICK_MSG, button45OnClick);
    WgtMsgConnect(data_diag_pg.button46, WGT_CLICK_MSG, button46OnClick);
    WgtMsgConnect(data_diag_pg.button47, WGT_CLICK_MSG, button47OnClick);
    WgtMsgConnect(data_diag_pg.button48, WGT_CLICK_MSG, button48OnClick);
    WgtMsgConnect(data_diag_pg.button49, WGT_CLICK_MSG, button49OnClick);
    WgtMsgConnect(data_diag_pg.button50, WGT_CLICK_MSG, button50OnClick);
    WgtMsgConnect(data_diag_pg.button51, WGT_CLICK_MSG, button51OnClick);
    WgtMsgConnect(data_diag_pg.button52, WGT_CLICK_MSG, button52OnClick);

}
static int Data_DiagOnInit(Data_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	DataInit();
    SetDataDiagCB(ChangeModeForbid);
    return 0;
}
static int Data_DiagOnShow(Data_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;

    e5021 = FALSE;
    e5022 = FALSE;
    //20200407HJM 模温功能
    e5011 = FALSE;
    e5012 = FALSE;
    e5013 = FALSE;
    e5014 = FALSE;
    e5015 = FALSE;
    e5016 = FALSE;
    e5017 = FALSE;

    e700b = FALSE;//20200603.cyx


    VarAdrSetInt(TEMP_LOST_E5021, 0);
    VarAdrSetInt(TEMP_LOST_E5022, 0);
    //20200407HJM 模温功能
    for(i = 0; i < MAX_MT_CNT; i++)
    {
        if(i < VarAdrToInt(MT_BOARD_COUNT))
        {
            VarAdrSetInt(E501_1_SHOW + i, 1);
        }
        else
        {
            VarAdrSetInt(E501_1_SHOW + i, 0);
        }
    }

    for(i = 0; i < MAX_MT_CNT; i++)
    {
        VarAdrSetInt(TEMP_LOST_E5011 + i, 0);
    }

    VarAdrSetStr(TEMP_STR_E5022, GetTextTran(TEXT_TESTBEGIN));
    VarAdrSetStr(TEMP_STR_E5021, GetTextTran(TEXT_TESTBEGIN));
    //20200407HJM 模温功能
    for(i = 0; i < MAX_MT_CNT; i++)
    {
        VarAdrSetStr(TEMP_STR_E5011 + i, GetTextTran(TEXT_TESTBEGIN));
    }

    //20200603.cyx
    if(VarAdrToInt(SYS_FL_MACH_CODE54)&0x800)
    {
        VarAdrSetInt(E700B_SHOW, 1);
    }
    else
    {
        VarAdrSetInt(E700B_SHOW, 0);
    }
    VarAdrSetStr(TEMP_STR_E700B, GetTextTran(TEXT_TESTBEGIN));

    WGTSetEnable(data_diag_pg.button42, TRUE);
    WGTSetEnable(data_diag_pg.button43, TRUE);
    //20200407HJM 模温功能
    WGTSetEnable(data_diag_pg.button45, TRUE);
    WGTSetEnable(data_diag_pg.button46, TRUE);
    WGTSetEnable(data_diag_pg.button47, TRUE);
    WGTSetEnable(data_diag_pg.button48, TRUE);
    WGTSetEnable(data_diag_pg.button49, TRUE);
    WGTSetEnable(data_diag_pg.button50, TRUE);
    WGTSetEnable(data_diag_pg.button51, TRUE);
    WGTSetEnable(data_diag_pg.button52, TRUE);//20200603.cyx

    WGTSetEnable(data_diag_pg.edit35, TRUE);

	DataInit();

    return 0;
}
static int Data_DiagOnHide(Data_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    UI16 pdatax[3];//e502

    //20200407HJM 模温功能
    UI16 pdata_mt[3];//e501
    int i;

    pdata[0] = 0;
    pdata[1] = 0;
    pdata[2] = 0;
    ProtSysCtrl(CAN_TEST_EXIT_CMD,3,pdata);
    pdatax[0] = 0;
    pdatax[1] = 1;
    pdatax[2] = 0;
    ProtSysCtrl(CAN_TEST_E502_CMD,3,pdatax);
    pdatax[0] = 0;
    pdatax[1] = 2;
    pdatax[2] = 0;
    ProtSysCtrl(CAN_TEST_E502_CMD,3,pdatax);

    //20200407HJM 模温功能
    for(i = 1; i <= VarAdrToInt(MT_CTRL_PART); i++)//20201218
    {
        pdata_mt[0] = 0;
        pdata_mt[1] = i;
        pdata_mt[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata_mt);
    }

    //20200603.cyx E700B
    pdata[0] = 0;
    pdata[1] = 1;
    pdata[2] = 0;
    ProtSysCtrl(CAN_TEST_E700B_CMD,3,pdata);

    return 0;
}
static int Data_DiagOnUpdate(Data_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    LostRate();

    return 0;
}
static int Data_DiagOnChange(Data_Diag_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr; 
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	switch (addr)
	{
	case ADR_REQUEST:
		{
			UI32 addr = VarAdrToInt(ADR_REQUEST);
			DataRequest(addr,ProdRecvData);
		}	
		break;
	default :		
		break;
	}


    return 0;
}

static int button42OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5021 = !e5021;
    if(e5021)
    {
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        WGTSetEnable(data_diag_pg.edit35, FALSE);

        VarAdrSetStr(TEMP_STR_E5021, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 1;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E502_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);

        WGTSetEnable(data_diag_pg.edit35, TRUE);

        VarAdrSetStr(TEMP_STR_E5021, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 1;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E502_CMD,3,pdata);
    }
}
static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5022 = !e5022;
    if(e5022)
    {
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        WGTSetEnable(data_diag_pg.edit35, FALSE);

        VarAdrSetStr(TEMP_STR_E5022, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 2;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E502_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);
        WGTSetEnable(data_diag_pg.edit35, TRUE);

        VarAdrSetStr(TEMP_STR_E5022, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 2;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E502_CMD,3,pdata);
    }

}

static int button45OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5011 = !e5011;
    if(e5011)
    {
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);

        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        VarAdrSetStr(TEMP_STR_E5011, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 1;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.edit35, TRUE);
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);
        VarAdrSetStr(TEMP_STR_E5011, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 1;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
}
static int button46OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5012 = !e5012;
    if(e5012)
    {
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        VarAdrSetStr(TEMP_STR_E5012, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 2;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.edit35, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);

        VarAdrSetStr(TEMP_STR_E5012, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 2;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
}
static int button47OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5013 = !e5013;
    if(e5013)
    {
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        VarAdrSetStr(TEMP_STR_E5013, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 3;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.edit35,   TRUE);
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);

        VarAdrSetStr(TEMP_STR_E5013, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 3;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
}
static int button48OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5014 = !e5014;
    if(e5014)
    {
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        VarAdrSetStr(TEMP_STR_E5014, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 4;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.edit35,   TRUE);
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);

        VarAdrSetStr(TEMP_STR_E5014, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 4;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
}
static int button49OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5015 = !e5015;
    if(e5015)
    {
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        VarAdrSetStr(TEMP_STR_E5015, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 5;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.edit35,   TRUE);
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);

        VarAdrSetStr(TEMP_STR_E5015, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 5;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
}
static int button50OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5016 = !e5016;
    if(e5016)
    {
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        VarAdrSetStr(TEMP_STR_E5016, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 6;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.edit35, TRUE);
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);

        VarAdrSetStr(TEMP_STR_E5016, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 6;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
}
static int button51OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e5017 = !e5017;
    if(e5017)
    {
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button52, FALSE);

        VarAdrSetStr(TEMP_STR_E5017, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 7;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.edit35, TRUE);
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button52, TRUE);

        VarAdrSetStr(TEMP_STR_E5017, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 7;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E501_CMD,3,pdata);
    }
}
//20200603.cyx E700B
static int button52OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[3];
    if(OperateModeIndex() != MODE_MANUAL)
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }
    e700b = !e700b;
    if(e700b)
    {
        WGTSetEnable(data_diag_pg.edit35, FALSE);
        WGTSetEnable(data_diag_pg.button42, FALSE);
        WGTSetEnable(data_diag_pg.button43, FALSE);
        WGTSetEnable(data_diag_pg.button45, FALSE);
        WGTSetEnable(data_diag_pg.button46, FALSE);
        WGTSetEnable(data_diag_pg.button47, FALSE);
        WGTSetEnable(data_diag_pg.button48, FALSE);
        WGTSetEnable(data_diag_pg.button49, FALSE);
        WGTSetEnable(data_diag_pg.button50, FALSE);
        WGTSetEnable(data_diag_pg.button51, FALSE);

        VarAdrSetStr(TEMP_STR_E700B, GetTextTran(TEXT_TESTEND));
        pdata[0] = 1;
        pdata[1] = 1;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E700B_CMD,3,pdata);
    }
    else
    {
        WGTSetEnable(data_diag_pg.edit35, TRUE);
        WGTSetEnable(data_diag_pg.button42, TRUE);
        WGTSetEnable(data_diag_pg.button43, TRUE);
        WGTSetEnable(data_diag_pg.button45, TRUE);
        WGTSetEnable(data_diag_pg.button46, TRUE);
        WGTSetEnable(data_diag_pg.button47, TRUE);
        WGTSetEnable(data_diag_pg.button48, TRUE);
        WGTSetEnable(data_diag_pg.button49, TRUE);
        WGTSetEnable(data_diag_pg.button50, TRUE);
        WGTSetEnable(data_diag_pg.button51, TRUE);

        VarAdrSetStr(TEMP_STR_E700B, GetTextTran(TEXT_TESTBEGIN));
        pdata[0] = 0;
        pdata[1] = 1;
        pdata[2] = 0;
        ProtSysCtrl(CAN_TEST_E700B_CMD,3,pdata);
    }
}
