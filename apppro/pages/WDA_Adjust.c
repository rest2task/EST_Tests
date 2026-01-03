#include "WDA_Adjust.h"
#include "typedef.h"
#include "database.h"
#include "texttbl.h"
#include "command.h"
#include "protcmd.h"
#include "information.h"
#include "questionDlg.h"
#include "taskmoni.h"
#include "oprecord.h"
#include "keymod.h"
#include "stdio.h"
#include "key.h"
#include "editwgt.h"
#include "oprintf.h"
#include "ctrlcmd.h"
#include "privdef.h"
#include "usermod.h"//20210914 dyl touch
#include "labelwgt.h"

DEFINE_DA_ADJUST_MAP

CONNECT_DA_ADJUST_EVENT

typedef enum enADJUST_MODE
{
	ADJ_PRESSZERO = 1,
	ADJ_PRESSMAX,
	ADJ_SPEEDZERO,
	ADJ_SPEEDMAX
}ADJUST_MODE ;

#define ADJ_BUTTON_MAX          (30)  //页面调按钮个数 

#define TMP_DA_DASELECT	        TMPVAL(0)  //da组数选择 临时变量0
#define TMP_DAOUTPUT_MAX	    TMPVAL(10) //DA输出最大值 //20210302
#define TMP_DAOUTPUT_DOT	    TMPVAL(11) //DA输出小数位 //20210302
#define TMP_DASPEED_DOT         TMPVAL(20) //DA速度小数位 //20210302

//da press or sppeed adjust
#define TMP_DA_ADJPRESS         TMPVAL(102)  //da压力调整 临时变量102
#define TMP_DA_STARTSPEED       TMPVAL(103)  //起始速度   临时变量103
#define TMP_DA_ADJSPEED         TMPVAL(106)  //da速度调整 临时变量106
#define TMP_DA_STARTPRESS       TMPVAL(107)  //起始压力   临时变量107

//speed
#define TMP_DA_SPEED            TMPVAL(110)  //速度       临时变量110

//password
#define TMP_DA_PASSWORD         TMPVAL(115)  //重置密码   临时变量115

//DA channel
#define TMP_DA_DACHANEL         TMPVAL(120)  //DA通道号   临时变量120

//DA button
#define TMP_DA_PRESSZERO        TMPSTR(30)  //DA压力调零 临时字符串变量 30 -59

//data addr
#define DAADJUST_MAXSPEED          0x3312004e  //DA最大速度
#define DAADJUST_DACHANEL          0x3312004f  //DA通道号

#define DA_OUTPUT0                 0x09160002  //DA压力输出
#define DA_OUTPUT1                 0x09160003  //DA流量输出

#define DASET_PRSOUTPUT0           0x09140090  //DA压力线性

#define DASET_FLOWOUTPUT0           0x091400a9  //DA流量线性

#define EDIT_LINEAR_NAME(NUM,pbuff,size)		joinStrAndNum("edit",NUM,pbuff,size)

static const char* trans;//按钮字符串变量

static QUESTION g_question;//询问框

#define BUTTONG_ADJUSTING(index)    \
	if(trans = GetMapTran(pmap_texttbl,TEXT_ADJUSTING)){ \
	VarAdrSetStr(TMP_DA_PRESSZERO+index-1,trans);    \
	} \

#define BUTTONG_ADJUST(index)    \
	if(trans = GetMapTran(pmap_texttbl,TEXT_ADJUST)){ \
	VarAdrSetStr(TMP_DA_PRESSZERO+index-1,trans);    \
	} \

static UI16 nDaIdx=0;
static UI16 nPress=0;
static UI16 nFlow=0;
static UI16 adjustMode=0;//调整哪个线性,eg:压力调零,最大压力,速度调零,最大速度,压力线性,速度线性
static UI16 nLineSelLast=0;//线性选择下拉框选择项修改前值 //20211101 dyl DA线性选择增加确认框

BOOL badjust=FALSE;// 调整与调整中
static PWGT old_pwgt = NULL;//20210914 dyl touch

extern BOOL g_bButtonPress;////按键长按标记
static void DaPrsAdjustReadOnly(BOOL read_only);
static void DaSpdAdjustReadOnly(BOOL read_only);

//根据CODE 油路控制 + 0x08 增加３位精度 //20210302
//20230518 chj DA最大值bug修复
static UI16 wDaMax_4095 = 4095;
static UI16 wDaMax_4090 = 4090;
static UI16 wDaMax_4065 = 4065;

static void ChangOutputPoint()
{  
    if(VarAdrToInt(d_machine1_MACHSET_FL_HYDRAUIC) & 0x08)
    {
        WGTSetVisible(da_adjust_pg.label193, TRUE);
        WGTSetVisible(da_adjust_pg.label194, TRUE);

        VarAdrSetInt(TMP_DAOUTPUT_DOT, 3);
        VarAdrSetInt(TMP_DAOUTPUT_MAX, 10000);
        //20230518 chj DA最大值bug修复
        wDaMax_4090 = 9995;
        wDaMax_4065 = 9970;
        wDaMax_4095 = 10000;
    }
    else
    {
        WGTSetVisible(da_adjust_pg.label193, FALSE);
        WGTSetVisible(da_adjust_pg.label194, FALSE);

        VarAdrSetInt(TMP_DAOUTPUT_DOT, 0);
        VarAdrSetInt(TMP_DAOUTPUT_MAX, 4095);
        //20230518 chj DA最大值bug修复
        wDaMax_4090 = 4090;
        wDaMax_4065 = 4065;
        wDaMax_4095 = 4095;
    }
}

/************************************************************************/
/* 变量地址切换                                                         */
/************************************************************************/
static void SetDASel()
{
	int i,j;
	char buff[10];
	PWGT pwgt;

	WGTSetAttrByName(da_adjust_pg.edit244,EDIT_TEXT_NAME,DA_OUTPUT0+nDaIdx*2);//压力强制输出
	
	WGTSetAttrByName(da_adjust_pg.edit246,EDIT_TEXT_NAME,DA_OUTPUT1+nDaIdx*2);//流量强制输出

	for(i=0;i<15;i++)
	{
		pwgt=PageFramCurWgtByName(EDIT_LINEAR_NAME(i,buff,10));
		WGTSetAttrByName(pwgt,EDIT_TEXT_NAME,DASET_PRSOUTPUT0+i+nDaIdx*36);//压力
		pwgt=PageFramCurWgtByName(EDIT_LINEAR_NAME(20+i,buff,10));
        WGTSetAttrByName(pwgt,EDIT_TEXT_NAME,DA_PRSOUT0_1+i+nDaIdx*36);//压力线性输出
	}

	for(j=0;j<11;j++)
	{
		pwgt=PageFramCurWgtByName(EDIT_LINEAR_NAME(40+j,buff,10));
		WGTSetAttrByName(pwgt,EDIT_TEXT_NAME,DASET_FLOWOUTPUT0+j+nDaIdx*36);//压力
		pwgt=PageFramCurWgtByName(EDIT_LINEAR_NAME(60+j,buff,10));
        WGTSetAttrByName(pwgt,EDIT_TEXT_NAME,DA_FLOWOUT0_1+j+nDaIdx*36);//压力线性输出
	}
}

/************************************************************************/
/* 背压线性重新生成                                                     */
/************************************************************************/
static void BkPrsLinera()
{
	UI16 pdata[3];

	pdata[0]=VarAdrToUI16(DAADJUST_DACHANEL);//
	pdata[1]=VarAdrToUI16(INJECT_PORT_CHRGBACK);
	pdata[2]=VarAdrToUI16(SYSTEM_DAPRES_BACKLIMIT);
	ProtSysCtrl(CHGBACK_ADJ_CMD,3,pdata);
}
/************************************************************************/
/* DA通达的设定                                                         */
/************************************************************************/
static void DaChanelSet()
{
	if(VarAdrToUI16(INJECT_PORT_CHRGBACK)==0)
	{
		VarAdrSetInt(TMP_DA_DACHANEL,0);
		VarAdrSetInt(DAADJUST_DACHANEL,0);
	}
	else if(VarAdrToUI16(INJECT_PORT_CHRGBACK)==1)
	{
		VarAdrSetInt(TMP_DA_DACHANEL,1);
		VarAdrSetInt(DAADJUST_DACHANEL,1);
	}
	else if(VarAdrToUI16(INJECT_PORT_CHRGBACK)==2)
	{
		VarAdrSetInt(TMP_DA_DACHANEL,2);
		VarAdrSetInt(DAADJUST_DACHANEL,2);
	}
	else if(VarAdrToUI16(INJECT_PORT_CHRGBACK)==4)
	{
		VarAdrSetInt(TMP_DA_DACHANEL,3);
		VarAdrSetInt(DAADJUST_DACHANEL,4);
	}
	else if(VarAdrToUI16(INJECT_PORT_CHRGBACK)==8)
	{
		VarAdrSetInt(TMP_DA_DACHANEL,4);
		VarAdrSetInt(DAADJUST_DACHANEL,8);
	}
	VarAdrSaveInt(DAADJUST_DACHANEL);

	//general machine channel
	if((VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020)==0)
	{
		if(VarAdrToUI16(INJECT_PORT_CHRGBACK)==1)
		{
			VarAdrSetInt(INJECT_PORT_CHRGBACK,0);
			VarAdrSaveInt(INJECT_PORT_CHRGBACK);
			VarSendByAdr(INJECT_PORT_CHRGBACK);

			BkPrsLinera();//背压线性重新生成

			VarAdrSetInt(TMP_DA_DACHANEL,0);
			VarAdrSetInt(DAADJUST_DACHANEL,0);
			VarAdrSaveInt(DAADJUST_DACHANEL);
		}
	}
}
/************************************************************************/
/* 机器DA调整页面进入                                                   */
/************************************************************************/
static void DaAdjustPageIn(DA_Adjust_PG* ppg)
{
	//DA chanel
	DaChanelSet();

	VarAdrSetInt(TMP_DA_DASELECT,nDaIdx);//DA select
	VarAdrSetInt(TMP_DA_ADJPRESS,nPress);//DA press adj
	VarAdrSetInt(TMP_DA_ADJSPEED,nFlow);//DA speed adj
	VarAdrSetInt(TMP_DA_STARTSPEED,30);//起流
	VarAdrSetInt(TMP_DA_STARTPRESS,100);//起压
	VarAdrSetInt(TMP_DA_PASSWORD,0);

	WGTSetEnable(ppg->btReset, FALSE);
	DaPrsAdjustReadOnly(TRUE);
	DaSpdAdjustReadOnly(TRUE);


	SetDASel();
    ChangOutputPoint();//20210302

	//data request
    ProtSysCtrl(ENTER_DA_CMD,0,NULL);//进入DA曲线调试 20190107 hz
	ProtReqPartSubData(PART1_ID(HYDR_ID),ACTPARA,0,GetSubLenByID(PART1_SUB_ID(HYDR_ID,ACTPARA)));//液压部件

	//g_daAdjustIn=TRUE;
	//按钮字符串变量 调整or 调整中
	if(trans =  GetMapTran(pmap_texttbl,TEXT_ADJUST))//显示调整
	{
		int i;
		for(i=0;i<ADJ_BUTTON_MAX;i++)
		{
			VarAdrSetStr(TMP_DA_PRESSZERO+i,trans);
		}
	}

	//是否有数据修改标记还原
	VarAdrSetInt(DAADJ_CHANGE_FLAG,0);

    nLineSelLast=VarAdrToInt(SYS_DA_TYPE);//线性选择下拉框选择项修改前赋值 //20211101 dyl DA线性选择增加确认框

	badjust=FALSE;
}
/************************************************************************/
/* 机器DA调整页面退出                                                   */
/************************************************************************/
static void DaAdjustPageOut()
{
	//g_daAdjustIn=FALSE;
	nPress=0;
	nFlow=0;
	VarAdrSetInt(DAADJUST_ADJMOD,0);//变回调整
    if (nLineSelLast != VarAdrToInt(SYS_DA_TYPE))//20211101 dyl DA线性选择增加确认框
    {
        VarAdrSetInt(SYS_DA_TYPE,nLineSelLast);//线性选择下拉框选择后未点确认赋回原值
    }

	ProtSysCtrl(EXIT_DA_CMD,0,NULL);//退出DA曲线调试状态
}
/************************************************************************/
/* 机器DA调整页面刷新                                                   */
/************************************************************************/
static void DaAdjustUpdate()
{
	int wSpeed;

	//speed
	if(VarAdrToUI16(DAADJUST_MAXSPEED)!=0)
	{
		if ((VarAdrToUI16(SYS_CN_CHGRPMHOLE)!=0)&&(VarAdrToUI16(INJECT_CN_CHRGSPD)>0))
		{
			wSpeed=(int)((VarAdrToUI16(INJECT_CN_CHRGSPD)*1000/VarAdrToUI16(DAADJUST_MAXSPEED)));
			VarAdrSetInt(TMP_DA_SPEED,wSpeed);
		}
	}

    //速度小数点控制改为根据操作状态来显示 非手动下显示一位小数 //20210507
    if(VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL)
    {
        VarAdrSetInt(TMP_DASPEED_DOT, 0);
    }
    else
    {
        VarAdrSetInt(TMP_DASPEED_DOT, 1);
    }

	//focus 
	//if(GetTick()-time_30ms>=100 && bDaButtonChg)
	//{
	//	bDaButtonChg=FALSE;
	//	time_30ms = GetTick();
	//	PageFramSetOrderFocus(pf_daadj, pf_daadj->pyo_tbl, FALSE, WGTGetYOrder(pf_daadj->plast_focus));
	//	PageFramSetOrderFocus(pf_daadj, pf_daadj->pyo_tbl, TRUE, WGTGetYOrder(pf_daadj->plast_focus));
	//}
	//if((bAdjSave || bDaSave) && g_bCfmConfirmation){
	//	SaveAdj();
	//	SendKey(g_wCfmKey);
	//	CfmN_ResetStatus(); 
	//}
}
/************************************************************************/
/* 压力流量强制输出                                                     */
/************************************************************************/
static void PrsSpdForcedOutput()
{
	UI16 wPress,wSpeed;
	UI16 pdata[3];

	wPress = VarAdrToUI16(DA_OUTPUT0+nDaIdx*2);
	wSpeed=  VarAdrToUI16(DA_OUTPUT1+nDaIdx*2);
	pdata[0]=nDaIdx;
	pdata[1]=wPress;
	pdata[2]=wSpeed;
	ProtSysCtrl(DA_PAIR_CUR_CMD,3,pdata);
}
/************************************************************************/
/* DA调整组数选择                                                       */
/************************************************************************/
static void DaGroupSel()
{
	nDaIdx = VarAdrToUI16(TMP_DA_DASELECT);
	SetDASel();
}
/************************************************************************/
/* 背压压力上限修改                                                     */
/************************************************************************/
static void BkPressLimit()
{
	BkPrsLinera();//背压线性重新生成
}
/************************************************************************/
/* 背压通道修改                                                         */
/************************************************************************/
static void BkPressChannel()
{
	UI16 value;

	value = VarAdrToUI16(TMP_DA_DACHANEL);

	if(value==0)
	{
		VarAdrSetInt(INJECT_PORT_CHRGBACK,0);
	}
	else if(value==1)
	{
		VarAdrSetInt(INJECT_PORT_CHRGBACK,1);
	}
	else if(value==2)
	{
		VarAdrSetInt(INJECT_PORT_CHRGBACK,2);
	}
	else if(value==3)
	{
		VarAdrSetInt(INJECT_PORT_CHRGBACK,4);
	}
	else if(value==4)
	{
		VarAdrSetInt(INJECT_PORT_CHRGBACK,8);
	}
	VarAdrSaveInt(INJECT_PORT_CHRGBACK);
	VarSendByAdr(INJECT_PORT_CHRGBACK);

	//general machine
    if((VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020) || (VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04))
    {
        BkPrsLinera();//背压线性重新生成
    }
    else
    {
        if(value==1)
        {
            VarAdrSetInt(INJECT_PORT_CHRGBACK,0);
            VarAdrSaveInt(INJECT_PORT_CHRGBACK);
            VarSendByAdr(INJECT_PORT_CHRGBACK);
            VarAdrSetInt(TMP_DA_DACHANEL,0);
            BkPrsLinera();//背压线性重新生成
            ShowMsg(VW_MSG_FIRSTNOTBK);//普通机第一路不作背压
        }
        else
        {
            BkPrsLinera();//背压线性重新生成
        }
    }

	VarAdrSetInt(DAADJUST_DACHANEL,VarAdrToUI16(INJECT_PORT_CHRGBACK));
	VarAdrSaveInt(DAADJUST_DACHANEL);
}
/************************************************************************/
/* 线性选择修改                                                         */
/************************************************************************/
static void LinearSelect()
{
	UI16 wValue;
	UI16 pdata[2];

	//if(dbsys.cIndexMode != MODE_MANUAL){
	//	CancelEditChange(hwnd);
	//	MsgN_View(VW_MSG_MANUAL);
	//	return;
	//}
	wValue=VarAdrToUI16(SYS_DA_TYPE);
	if(wValue)
	{
		VarAdrSetInt(SYS_FL_HYDRAUIC,VarAdrToUI16(SYS_FL_HYDRAUIC)|0x10);
	}
	else
	{
		VarAdrSetInt(SYS_FL_HYDRAUIC,VarAdrToUI16(SYS_FL_HYDRAUIC)&0xFFEF);
	}
	CodeSendSave();//配置发送保存

	//DA校正线性选择 0-标准含底流,1-标准直线
	pdata[0]=wValue;
	pdata[1]=nDaIdx;
	ProtSysCtrl(ZEROLINE_ADJ_CMD,2,pdata);//20160519
}
/************************************************************************/
/* DA压力调整选择      20181019hz                                         */
/************************************************************************/
static void DaPrsAdjustReadOnly(BOOL read_only)
{
	PWGT pwgt;
	UI16 i;
	char buff[10];
	UI16 num_pressbar = 14;
	UI16 num_pressout = 35;
    UI32 bkcolor,textcolor;

    if(read_only)
    {
        bkcolor = 0xdedede;
        textcolor = 0x000000;
    }
    else
    {
        bkcolor = 0xFFFFFF;
        textcolor = 0x000000;
    }

	for (i = 0; i < num_pressbar; ++i)
	{
		pwgt = PageFramCurWgtByName(EDIT_LINEAR_NAME(i,buff,10));
		EditSetReadOnly(pwgt, read_only);
        EditSetBKColor(pwgt, bkcolor);
        EditSetTextColor(pwgt, textcolor);
	}

	for (i = 20; i < num_pressout; ++i)
	{
		pwgt = PageFramCurWgtByName(EDIT_LINEAR_NAME(i,buff,10));
		EditSetReadOnly(pwgt, read_only);
        EditSetBKColor(pwgt, bkcolor);
        EditSetTextColor(pwgt, textcolor);
	}

    //20210513
    EditSetReadOnly(da_adjust_pg.edit129, read_only);
    EditSetBKColor(da_adjust_pg.edit129, bkcolor);
    EditSetTextColor(da_adjust_pg.edit129, textcolor);
}
/************************************************************************/
/* DA压力最大值      20181019hz                                         */
/************************************************************************/
static void DaPrsAdjustMax()
{
	PWGT pwgt;
	UI16 i;
	char buff[10];
	UI16 num_pressbar_max = 15;

	for (i = 0; i < num_pressbar_max; ++i)
	{
		pwgt = PageFramCurWgtByName(EDIT_LINEAR_NAME(i,buff,10));
		if ((VarAdrToInt(TMP_DA_DASELECT) > 0) && ((VarAdrToInt(TMP_DA_DASELECT) + 1) == VarAdrToInt(TMP_DA_DACHANEL)))
		{
            EditSetMax(pwgt, VarAdrToInt(SYSTEM_DAPRES_BACKLIMIT));
		} 
		else
		{
            EditSetMax(pwgt, VarAdrToInt(SYS_MACH_DAPRES_SYSLMT));
		}
	}

	if ((VarAdrToInt(TMP_DA_DASELECT) > 0) && ((VarAdrToInt(TMP_DA_DASELECT) + 1) == VarAdrToInt(TMP_DA_DACHANEL)))
	{
        EditSetMax(PageFramCurWgtByName(EDIT_LINEAR_NAME(244,buff,10)), VarAdrToInt(SYSTEM_DAPRES_BACKLIMIT));
	} 
	else
	{
        EditSetMax(PageFramCurWgtByName(EDIT_LINEAR_NAME(244,buff,10)), VarAdrToInt(SYS_MACH_DAPRES_SYSLMT));
	}

}


/************************************************************************/
/* DA压力调整选择                                                       */
/************************************************************************/
static void DaPrsAdjust()
{
	nPress = VarAdrToUI16(TMP_DA_ADJPRESS);
	
	if(nPress)
	{
		VarAdrSetInt(TMP_DA_ADJSPEED,0);
		DaSpdAdjustReadOnly(TRUE);
		DaPrsAdjustReadOnly(FALSE);
	}
	else
	{
		DaPrsAdjustReadOnly(TRUE);
	}
}
/************************************************************************/
/* DA流量调整选择    20181019hz                                           */
/************************************************************************/
static void DaSpdAdjustReadOnly(BOOL read_only)
{
	PWGT pwgt;
	UI16 i;
	char buff[10];
	UI16 num_spd = 50;
	UI16 num_spdout = 71;
    UI32 bkcolor,textcolor;

    if(read_only)
    {
        bkcolor = 0xdedede;
        textcolor = 0x000000;
    }
    else
    {
        bkcolor = 0xFFFFFF;
        textcolor = 0x000000;
    }

	for (i = 40; i < num_spd; ++i)
	{
		pwgt = PageFramCurWgtByName(EDIT_LINEAR_NAME(i,buff,10));
		EditSetReadOnly(pwgt, read_only);
        EditSetBKColor(pwgt, bkcolor);
        EditSetTextColor(pwgt, textcolor);
	}

	for (i = 60; i < num_spdout; ++i)
	{
		pwgt = PageFramCurWgtByName(EDIT_LINEAR_NAME(i,buff,10));
		EditSetReadOnly(pwgt, read_only);
        EditSetBKColor(pwgt, bkcolor);
        EditSetTextColor(pwgt, textcolor);
	}

    //20210513
    EditSetReadOnly(da_adjust_pg.edit130, read_only);
    EditSetBKColor(da_adjust_pg.edit130, bkcolor);
    EditSetTextColor(da_adjust_pg.edit130, textcolor);
}
/************************************************************************/
/* DA流量调整选择                                                       */
/************************************************************************/
static void DaSpdAdjust()
{
	nFlow = VarAdrToUI16(TMP_DA_ADJSPEED);
	if(nFlow)
	{
		VarAdrSetInt(TMP_DA_ADJPRESS,0);
		DaSpdAdjustReadOnly(FALSE);
		DaPrsAdjustReadOnly(TRUE);
	}
	else
	{
		DaSpdAdjustReadOnly(TRUE);
	}
}
/************************************************************************/
/* DA曲线保存                                                           */
/************************************************************************/
static void DaAdjustSave()
{
	ProtSysCtrl(DA_CUR_SAVE_CMD,0,NULL);
	VarAdrSetInt(DAADJ_CHANGE_FLAG,0);//线性数据修改标记还原
}
/************************************************************************/
/* DA曲线及最大值 最小值重置                                            */
/************************************************************************/
static void DaAdjustReset()
{
	ProtSysCtrl(DA_CFG_RESET_CMD,0,NULL);
}
/************************************************************************/
/* 压力调零发送                                                         */
/************************************************************************/
static void DaPressZero()
{

	UI16 wGroup; //组数
	UI16 pdata[3];

	wGroup = nDaIdx;
	pdata[0]=wGroup;
	if(wGroup==3)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN7);
	}
	else if(wGroup==2)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN5);
	}
	else if(wGroup==1)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN3);
	}
	else if(wGroup==0)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN1);
	}
	pdata[2]=VarAdrToUI16(TMP_DA_STARTSPEED);
	ProtSysCtrl(PRESS_DAMIN_ADJ_CMD, 3, pdata);
}
/************************************************************************/
/* 最大压力发送                                                         */
/************************************************************************/
static void DaPressMax()
{
	UI16 wGroup; //组数
	UI16 pdata[3];

	wGroup = nDaIdx;
	pdata[0]=wGroup;
	if(wGroup==3)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX7);
	}
	else if(wGroup==2)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX5);
	}
	else if(wGroup==1)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX3);
	}
	else if(wGroup==0)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX1);
	}
	pdata[2]=VarAdrToUI16(TMP_DA_STARTSPEED);
	ProtSysCtrl(PRESS_MAX_ADJ_CMD, 3, pdata);
}
/************************************************************************/
/* 速度调零发送                                                         */
/************************************************************************/
static void DaSpeedZero()
{

	UI16 wGroup; //组数
	UI16 pdata[3];

	wGroup = nDaIdx;
	pdata[0]=wGroup;
	if(wGroup==3)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN8);
	}
	else if(wGroup==2)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN6);
	}
	else if(wGroup==1)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN4);
	}
	else if(wGroup==0)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMIN2);
	}
	pdata[2]=VarAdrToUI16(TMP_DA_STARTPRESS);//起压
	ProtSysCtrl(FLOW_DAMIN_ADJ_CMD, 3, pdata);
}
/************************************************************************/
/* 最大速度发送                                                         */
/************************************************************************/
static void DaSpeedMax()
{
	UI16 wGroup; //组数
	UI16 pdata[3];

	wGroup = nDaIdx;
	pdata[0]=wGroup;
	if(wGroup==3)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX8);
	}
	else if(wGroup==2)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX6);
	}
	else if(wGroup==1)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX4);
	}
	else if(wGroup==0)
	{
		pdata[1]=VarAdrToUI16(DA_STATE_TEMPMAX2);
	}
	pdata[2]=VarAdrToUI16(TMP_DA_STARTPRESS);
	ProtSysCtrl(FLOW_MAX_ADJ_CMD, 3, pdata);
}
/************************************************************************/
/* 压力线性发送                                                         */
/************************************************************************/
static void DaPressLinear(UI16 index)
{
	UI16 wGroup; //组数
	UI16 pdata[3];

	wGroup = nDaIdx;
	pdata[0]=wGroup;
	if(wGroup==3)
	{
		pdata[1]=VarAdrToUI16(DA_PRSOUT0_4+index);
	}
	else if(wGroup==2)
	{
		pdata[1]=VarAdrToUI16(DA_PRSOUT0_3+index);
	}
	else if(wGroup==1)
	{
		pdata[1]=VarAdrToUI16(DA_PRSOUT0_2+index);
	}
	else if(wGroup==0)
	{
		pdata[1]=VarAdrToUI16(DA_PRSOUT0_1+index);
	}
	pdata[2]=VarAdrToUI16(TMP_DA_STARTSPEED); //起流
	ProtSysCtrl(PRESS_CUR_CMD, 3, pdata);//DA压力曲线调整
}
/************************************************************************/
/* 流量线性发送                                                         */
/************************************************************************/
static void DaSpeedLinear(UI16 index)
{
	UI16 wGroup; //组数
	UI16 pdata[3];

	wGroup = nDaIdx;
	pdata[0]=wGroup;
	if(wGroup==3)
	{
		pdata[1]=VarAdrToUI16(DA_FLOWOUT0_4+index);
	}
	else if(wGroup==2)
	{
		pdata[1]=VarAdrToUI16(DA_FLOWOUT0_3+index);
	}
	else if(wGroup==1)
	{
		pdata[1]=VarAdrToUI16(DA_FLOWOUT0_2+index);
	}
	else if(wGroup==0)
	{
		pdata[1]=VarAdrToUI16(DA_FLOWOUT0_1+index);
	}
	pdata[2]=VarAdrToUI16(TMP_DA_STARTPRESS); //起压
	ProtSysCtrl(FLOW_CUR_CMD, 3, pdata);////DA流量曲线调整
}
/************************************************************************/
/* DA线性调整关闭                                                       */
/************************************************************************/
static void DaAdjustCls()
{
	ProtSysCtrl(DA_MAX_ADJOVER_CMD, 1, &nDaIdx);////DA调整关闭
}

/************************************************************************/
/* DA线性调整                                                          */
/************************************************************************/
static void DaAdjust(UI16 index)
{
	int nIndex;
	//int index = para[0].v;
//oprintf("badjust=%d,index=%d,old_index=%d\n",badjust,index,old_index);
	badjust =!badjust;

	if((index ==1) && badjust)
		DaPressZero();
	else if((index ==2) && badjust)
		DaPressMax();
	else if((index ==3) && badjust)
		DaSpeedZero();
	else if((index ==4) && badjust)
		DaSpeedMax();
	else if((index >=5 && index <=19)  && badjust)
	{
		nIndex=index-5;
		DaPressLinear(nIndex);
	}
	else if((index >=20 && index <=30)  && badjust)
	{
		nIndex=index-20;
		DaSpeedLinear(nIndex);
	}

	if(badjust)
	{
		BUTTONG_ADJUSTING(index);
		VarAdrSetInt(DAADJUST_ADJMOD,1);//变调整中
		adjustMode = index;
	}
	else 
	{
        BUTTONG_ADJUST(index);
		VarAdrSetInt(DAADJUST_ADJMOD,0);//变调整
		DaAdjustCls();
	}	
}
/************************************************************************/
/* DA线性调整标记                                                       */
/************************************************************************/
static UI16 DaAdjustFlag()
{
	return VarAdrToUI16(DAADJUST_ADJMOD);
}
/************************************************************************/
/* DA线性调整                                                           */
/************************************************************************/
void DaLinearAdj(UI32 key,UI32 press)
{
	UI16 wGroup,wValue;
	UI32 wKey;

	if(adjustMode == ADJ_PRESSZERO)
	{
		wGroup=nDaIdx;
		if(wGroup==3) wValue=VarAdrToUI16(DA_STATE_TEMPMIN7);
		else if(wGroup==2) wValue=VarAdrToUI16(DA_STATE_TEMPMIN5);
		else if(wGroup==1) wValue=VarAdrToUI16(DA_STATE_TEMPMIN3);
		else if(wGroup==0) wValue=VarAdrToUI16(DA_STATE_TEMPMIN1);

		if(DaAdjustFlag())
		{
			//stInfo->show();
			wKey=key;
			if(wKey==GUI_KEY_DOWN && press==1){ //
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
				if(wValue>=5) {
					if(wGroup==3){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN7,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN7);
					}
					else if(wGroup==2){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN5,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN5);
					}
					else if(wGroup==1){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN3,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN3);
					}
					else if(wGroup==0){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN1,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN1);
					}
					//DaPressZero();
				}
				else{
					if(wGroup==3) {
						VarAdrSetInt(DA_STATE_TEMPMIN7,0);
						VarSendByAdr(DA_STATE_TEMPMIN7);
					}
					else if(wGroup==2) {
						VarAdrSetInt(DA_STATE_TEMPMIN5,0);
						VarSendByAdr(DA_STATE_TEMPMIN5);
					}
					else if(wGroup==1) {
						VarAdrSetInt(DA_STATE_TEMPMIN3,0);
						VarSendByAdr(DA_STATE_TEMPMIN3);
					}
					else if(wGroup==0) {
						VarAdrSetInt(DA_STATE_TEMPMIN1,0);
						VarSendByAdr(DA_STATE_TEMPMIN1);
					}
					//DaPressZero();
				}
				DaPressZero();
			}
			else if (wKey==GUI_KEY_UP && press==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
                if(wValue<=wDaMax_4090) {//20230518 chj DA最大值bug修复
					if(wGroup==3){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN7,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN7);
					}
					else if(wGroup==2){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN5,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN5);
					}
					else if(wGroup==1){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN3,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN3);
					}
					else if(wGroup==0){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN1,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN1);
					}
					//DaPressZero();
				}
				else{
					if(wGroup==3){
                        VarAdrSetInt(DA_STATE_TEMPMIN7,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN7);
					}
					else if(wGroup==2){
                        VarAdrSetInt(DA_STATE_TEMPMIN5,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN5);
					}
					else if(wGroup==1){
                        VarAdrSetInt(DA_STATE_TEMPMIN3,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN3);
					}
					else if(wGroup==0){
                        VarAdrSetInt(DA_STATE_TEMPMIN1,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN1);
					}
					//DaPressZero();
				}
				DaPressZero();
			}
		}
	}
	else if(adjustMode == ADJ_PRESSMAX)
	{
		wGroup = nDaIdx;
		if(wGroup==3) wValue=VarAdrToUI16(DA_STATE_TEMPMAX7);
		else if(wGroup==2) wValue=VarAdrToUI16(DA_STATE_TEMPMAX5);
		else if(wGroup==1) wValue=VarAdrToUI16(DA_STATE_TEMPMAX3);
		else if(wGroup==0) wValue=VarAdrToUI16(DA_STATE_TEMPMAX1);
		if(DaAdjustFlag()){
			//stInfo->show();
			wKey=key;
			if(wKey==GUI_KEY_DOWN && press ==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
				if(wValue>=5) {
					if(wGroup==3) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX7,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX7);
					}
					else if(wGroup==2) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX5,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX5);
					}
					else if(wGroup==1) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX3,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX3);
					}
					else if(wGroup==0){
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX1,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX1);
					}
					//DaPressMax();
				}
				else{
					if(wGroup==3) {
						VarAdrSetInt(DA_STATE_TEMPMAX7,0);
						VarSendByAdr(DA_STATE_TEMPMAX7);
					}
					else if(wGroup==2) {
						VarAdrSetInt(DA_STATE_TEMPMAX5,0);
						VarSendByAdr(DA_STATE_TEMPMAX5);
					}
					else if(wGroup==1) {
						VarAdrSetInt(DA_STATE_TEMPMAX3,0);
						VarSendByAdr(DA_STATE_TEMPMAX3);
					}
					else if(wGroup==0){
						VarAdrSetInt(DA_STATE_TEMPMAX1,0);
						VarSendByAdr(DA_STATE_TEMPMAX1);
					}
					//DaPressMax();
				}
				DaPressMax();
			}
			else if (wKey==GUI_KEY_UP && press ==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
                if(wValue<=wDaMax_4090) {//20230518 chj DA最大值bug修复
					if (wGroup==3){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX7,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX7);
					}
					else if(wGroup==2){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX5,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX5);
					}
					else if(wGroup==1){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX3,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX3);
					}
					else if(wGroup==0){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX1,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX1);
					}
					//DaPressMax();

				}
				else{
					if(wGroup==3){
                        VarAdrSetInt(DA_STATE_TEMPMAX7,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX7);
					}
					else if(wGroup==2){
                        VarAdrSetInt(DA_STATE_TEMPMAX5,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX5);
					}
					else if(wGroup==1){
                        VarAdrSetInt(DA_STATE_TEMPMAX3,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX3);
					}
					else if(wGroup==0){
                        VarAdrSetInt(DA_STATE_TEMPMAX1,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX1);
					}
					//DaPressMax();
				}
				DaPressMax();
			}
		}
	}
	else if(adjustMode == ADJ_SPEEDZERO)
	{
		wGroup=nDaIdx;
		if(wGroup==3) wValue=VarAdrToUI16(DA_STATE_TEMPMIN8);
		else if(wGroup==2) wValue=VarAdrToUI16(DA_STATE_TEMPMIN6);
		else if(wGroup==1) wValue=VarAdrToUI16(DA_STATE_TEMPMIN4);
		else if(wGroup==0) wValue=VarAdrToUI16(DA_STATE_TEMPMIN2);
		if(DaAdjustFlag()){
			//stInfo->show();
			wKey=key;
			if(wKey==GUI_KEY_DOWN && press ==1)
			{
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
				if(wValue>=5) {
					if(wGroup==3){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN8,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN8);
					}
					else if(wGroup==2){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN6,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN6);
					}
					else if(wGroup==1){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN4,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN4);
					}
					else if(wGroup==0){
						if((g_bButtonPress)&&(wValue>=30)){
							wValue-=30;
						}
						else{
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN2,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN2);
					}
					//DaSpeedZero();
				}
				else{
					if(wGroup==3) {
						VarAdrSetInt(DA_STATE_TEMPMIN8,0);
						VarSendByAdr(DA_STATE_TEMPMIN8);
					}
					else if(wGroup==2) {
						VarAdrSetInt(DA_STATE_TEMPMIN6,0);
						VarSendByAdr(DA_STATE_TEMPMIN6);
					}
					else if(wGroup==1) {
						VarAdrSetInt(DA_STATE_TEMPMIN4,0);
						VarSendByAdr(DA_STATE_TEMPMIN4);
					}
					else if(wGroup==0) {
						VarAdrSetInt(DA_STATE_TEMPMIN2,0);
						VarSendByAdr(DA_STATE_TEMPMIN2);
					}
					//DaSpeedZero();
				}
				DaSpeedZero();
			}
			else if (wKey==GUI_KEY_UP && press ==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
                if(wValue<=wDaMax_4090) {//20230518 chj DA最大值bug修复
					if(wGroup==3){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN8,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN8);
					}
					else if(wGroup==2){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN6,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN6);
					}
					else if(wGroup==1){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN4,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN4);
					}
					else if(wGroup==0){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMIN2,wValue);
						VarSendByAdr(DA_STATE_TEMPMIN2);
					}
					//DaSpeedZero();
				}
				else{
					if(wGroup==3){
                        VarAdrSetInt(DA_STATE_TEMPMIN8,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN8);
					}
					else if(wGroup==2){
                        VarAdrSetInt(DA_STATE_TEMPMIN6,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN6);
					}
					else if(wGroup==1){
                        VarAdrSetInt(DA_STATE_TEMPMIN4,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN4);
					}
					else if(wGroup==0){
                        VarAdrSetInt(DA_STATE_TEMPMIN2,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMIN2);
					}
					//DaSpeedZero();
				}
				DaSpeedZero();
			}
		}
	}
	else if (adjustMode == ADJ_SPEEDMAX)
	{
		wGroup=nDaIdx;
		if(wGroup==3) wValue=VarAdrToUI16(DA_STATE_TEMPMAX8);
		else if(wGroup==2) wValue=VarAdrToUI16(DA_STATE_TEMPMAX6);
		else if(wGroup==1) wValue=VarAdrToUI16(DA_STATE_TEMPMAX4);
		else if(wGroup==0) wValue=VarAdrToUI16(DA_STATE_TEMPMAX2);
		if(DaAdjustFlag())
		{
			wKey=key;
			//stInfo->show();
			if(wKey==GUI_KEY_DOWN && press ==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
				if(wValue>=5) {
					if(wGroup==3){
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX8,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX8);
					}
					else if(wGroup==2) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX6,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX6);
					}
					else if(wGroup==1) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX4,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX4);
					}
					else if(wGroup==0){
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX2,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX2);
					}
					//DaSpeedMax();
				}
				else {
					if(wGroup==3){
						VarAdrSetInt(DA_STATE_TEMPMAX8,0);
						VarSendByAdr(DA_STATE_TEMPMAX8);
					}
					else  if(wGroup==2) {
						VarAdrSetInt(DA_STATE_TEMPMAX6,0);
						VarSendByAdr(DA_STATE_TEMPMAX6);
					}
					else if(wGroup==1) {
						VarAdrSetInt(DA_STATE_TEMPMAX4,0);
						VarSendByAdr(DA_STATE_TEMPMAX4);
					}
					else if(wGroup==0){
						VarAdrSetInt(DA_STATE_TEMPMAX2,0);
						VarSendByAdr(DA_STATE_TEMPMAX2);
					}
					//DaSpeedMax();
				}
				DaSpeedMax();
			}
			else if (wKey==GUI_KEY_UP && press==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
                if(wValue<=wDaMax_4090) {//20230518 chj DA最大值bug修复
					if(wGroup==3){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX8,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX8);
					}
					else if(wGroup==2){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX6,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX6);
					}
					else if(wGroup==1){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX4,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX4);
					}
					else if(wGroup==0){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_STATE_TEMPMAX2,wValue);
						VarSendByAdr(DA_STATE_TEMPMAX2);
					}
					//DaSpeedMax();
				}
				else{
					if(wGroup==3){
                        VarAdrSetInt(DA_STATE_TEMPMAX8,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX8);
					}
					else if(wGroup==2){
                        VarAdrSetInt(DA_STATE_TEMPMAX6,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX6);
					}
					else if(wGroup==1){
                        VarAdrSetInt(DA_STATE_TEMPMAX4,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX4);
					}
					else if(wGroup==0){
                        VarAdrSetInt(DA_STATE_TEMPMAX2,wDaMax_4095);
						VarSendByAdr(DA_STATE_TEMPMAX2);
					}
					//DaSpeedMax();
				}
				DaSpeedMax();
			}
		}
	}
	else if(adjustMode >=5 && adjustMode <=19)
	{
		UI16 index;

		index = adjustMode-5;
		wGroup=nDaIdx;
		if(wGroup==3) wValue=VarAdrToUI16(DA_PRSOUT0_4+index);
		else if(wGroup==2) wValue=VarAdrToUI16(DA_PRSOUT0_3+index);
		else if(wGroup==1) wValue=VarAdrToUI16(DA_PRSOUT0_2+index);
		else if(wGroup==0) wValue=VarAdrToUI16(DA_PRSOUT0_1+index);
		if(DaAdjustFlag())
		{
			//stInfo->show();
			wKey=key;
			if(wKey==GUI_KEY_DOWN && press ==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
				if(wValue>=5) {
					if(wGroup==3) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_PRSOUT0_4+index,wValue);
						VarSendByAdr(DA_PRSOUT0_4+index);
					}
					else if(wGroup==2) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_PRSOUT0_3+index,wValue);
						VarSendByAdr(DA_PRSOUT0_3+index);
					}
					else if(wGroup==1) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_PRSOUT0_2+index,wValue);
						VarSendByAdr(DA_PRSOUT0_2+index);
					}
					else if(wGroup==0){
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_PRSOUT0_1+index,wValue);
						VarSendByAdr(DA_PRSOUT0_1+index);
					}
					//DaPressLinear(index);
				}
				else{
					if(wGroup==3) {
						VarAdrSetInt(DA_PRSOUT0_4+index,0);
						VarSendByAdr(DA_PRSOUT0_4+index);
					}
					else if(wGroup==2) {
						VarAdrSetInt(DA_PRSOUT0_3+index,0);
						VarSendByAdr(DA_PRSOUT0_3+index);
					}
					else if(wGroup==1) {
						VarAdrSetInt(DA_PRSOUT0_2+index,0);
						VarSendByAdr(DA_PRSOUT0_2+index);
					}
					else if(wGroup==0){
						VarAdrSetInt(DA_PRSOUT0_1+index,0);
						VarSendByAdr(DA_PRSOUT0_1+index);
					}
					//DaPressLinear(index);
				}
				DaPressLinear(index);
			}
			else if (wKey==GUI_KEY_UP && press ==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
                if(wValue<=wDaMax_4090) {//20230518 chj DA最大值bug修复
					if (wGroup==3){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_PRSOUT0_4+index,wValue);
						VarSendByAdr(DA_PRSOUT0_4+index);
					}
					else if(wGroup==2){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_PRSOUT0_3+index,wValue);
						VarSendByAdr(DA_PRSOUT0_3+index);
					}
					else if(wGroup==1){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_PRSOUT0_2+index,wValue);
						VarSendByAdr(DA_PRSOUT0_2+index);
					}
					else if(wGroup==0){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_PRSOUT0_1+index,wValue);
						VarSendByAdr(DA_PRSOUT0_1+index);
					}
					//DaPressLinear(index);
				}
				else{
					if(wGroup==3){
                        VarAdrSetInt(DA_PRSOUT0_4+index,wDaMax_4095);
						VarSendByAdr(DA_PRSOUT0_4+index);
					}
					else if(wGroup==2){
                        VarAdrSetInt(DA_PRSOUT0_3+index,wDaMax_4095);
						VarSendByAdr(DA_PRSOUT0_3+index);
					}
					else if(wGroup==1){
                        VarAdrSetInt(DA_PRSOUT0_2+index,wDaMax_4095);
						VarSendByAdr(DA_PRSOUT0_2+index);
					}
					else if(wGroup==0){
                        VarAdrSetInt(DA_PRSOUT0_1+index,wDaMax_4095);
						VarSendByAdr(DA_PRSOUT0_1+index);
					}
					//DaPressLinear(index);
				}
				DaPressLinear(index);
			}
		}
	}
	else if(adjustMode >=20 && adjustMode<=30)
	{
		UI16 index;

		index = adjustMode-20;
		wGroup=nDaIdx;
		if(wGroup==3) wValue=VarAdrToUI16(DA_FLOWOUT0_4+index);
		else if(wGroup==2) wValue=VarAdrToUI16(DA_FLOWOUT0_3+index);
		else if(wGroup==1) wValue=VarAdrToUI16(DA_FLOWOUT0_2+index);
		else if(wGroup==0) wValue=VarAdrToUI16(DA_FLOWOUT0_1+index);
		if(DaAdjustFlag())
		{
			//stInfo->show();
			wKey=key;
			if(wKey==GUI_KEY_DOWN && press==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
				if(wValue>=5) {
					if(wGroup==3) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_4+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_4+index);
					}
					else if(wGroup==2) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_3+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_3+index);
					}
					else if(wGroup==1) {
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_2+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_2+index);
					}
					else if(wGroup==0){
						if((g_bButtonPress)&&(wValue>=30)) {
							wValue-=30;
						}
						else {
							wValue-=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_1+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_1+index);
					}
					//DaSpeedLinear(index);
				}
				else{
					if(wGroup==3) {
						VarAdrSetInt(DA_FLOWOUT0_4+index,0);
						VarSendByAdr(DA_FLOWOUT0_4+index);
					}
					else if(wGroup==2) {
						VarAdrSetInt(DA_FLOWOUT0_3+index,0);
						VarSendByAdr(DA_FLOWOUT0_3+index);
					}
					else if(wGroup==1) {
						VarAdrSetInt(DA_FLOWOUT0_2+index,0);
						VarSendByAdr(DA_FLOWOUT0_2+index);
					}
					else if(wGroup==0){
						VarAdrSetInt(DA_FLOWOUT0_1+index,0);
						VarSendByAdr(DA_FLOWOUT0_1+index);
					}
					//DaSpeedLinear(index);
				}
				DaSpeedLinear(index);
			}
			else if (wKey==GUI_KEY_UP && press==1){
				//g_bCfmDaxSave = TRUE;
				VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
				//bAdjChange = TRUE;
                if(wValue<=wDaMax_4090) {//20230518 chj DA最大值bug修复
					if (wGroup==3){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_4+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_4+index);
					}
					else if(wGroup==2){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_3+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_3+index);
					}
					else if(wGroup==1){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_2+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_2+index);
					}
					else if(wGroup==0){
                        if((g_bButtonPress)&&(wValue<=wDaMax_4065)) {
							wValue+=30;
						}
						else {
							wValue+=5;
						}
						VarAdrSetInt(DA_FLOWOUT0_1+index,wValue);
						VarSendByAdr(DA_FLOWOUT0_1+index);
					}
					//DaSpeedLinear(index);
				}
				else{
					if(wGroup==3){
                        VarAdrSetInt(DA_FLOWOUT0_4+index,wDaMax_4095);
						VarSendByAdr(DA_FLOWOUT0_4+index);
					}
					else if(wGroup==2){
                        VarAdrSetInt(DA_FLOWOUT0_3+index,wDaMax_4095);
						VarSendByAdr(DA_FLOWOUT0_3+index);
					}
					else if(wGroup==1){
                        VarAdrSetInt(DA_FLOWOUT0_2+index,wDaMax_4095);
						VarSendByAdr(DA_FLOWOUT0_2+index);
					}
					else if(wGroup==0){
                        VarAdrSetInt(DA_FLOWOUT0_1+index,wDaMax_4095);
						VarSendByAdr(DA_FLOWOUT0_1+index);
					}
					//DaSpeedLinear(index);
				}
				DaSpeedLinear(index);
			}
		}
	}
}

static int switch_key = 0;
/************************************************************************/
/* 切页面信息提示框“保存”按钮                                         */
/************************************************************************/
static void SaveButton()
{
	DaAdjustSave();
	//返回物理按键
	if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6)
	{
		switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
	}
	else if(switch_key ==GUI_KEY_FUN7)
	{
		switch_key = KEY_FUN7;
	}
	else if(switch_key == GUI_KEY_MAINPAGE)
	{
		switch_key = KEY_MAIN;
	}
	else if(switch_key == GUI_KEY_FUN8)
	{
		switch_key = KEY_PC;
	}
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
	SendKey(switch_key);
}
/************************************************************************/
/*  切页面信息提示框“取消”按钮                                        */
/************************************************************************/
static void CancelButton()
{
	VarAdrSetInt(DAADJ_CHANGE_FLAG,0);//数据修改标记还原
	//返回物理按键
	if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6)
	{
		switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
	}
	else if(switch_key ==GUI_KEY_FUN7)
	{
		switch_key = KEY_FUN7;
	}
	else if(switch_key == GUI_KEY_MAINPAGE)
	{
		switch_key = KEY_MAIN;
	}
	else if(switch_key == GUI_KEY_FUN8)
	{
		switch_key = KEY_PC;
	}
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
	SendKey(switch_key);
}
static void DaAjustChange(int key,int press)
{
	if(press)
	{
		g_question.pOkfunc = SaveButton;
		g_question.pCancelfunc = CancelButton;
		switch_key = key;

        //20200109.cyx 非手动，自动登出时默认不保存，不弹框
        if((VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL) || VarAdrToInt(USER_STATE))
        {
            CancelButton();
        }
        else
        {
            ShowQuestion(VW_MSG_SAVEDATA,g_question);
        }
	}
}

static int btSave0OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int btSave1OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int btSave2OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int btResetOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(da_adjust_pg.btReset, WGT_CLICK_MSG, btResetOnClick);
    WgtMsgConnect(da_adjust_pg.btSave2, WGT_CLICK_MSG, btSave2OnClick);
    WgtMsgConnect(da_adjust_pg.btSave1, WGT_CLICK_MSG, btSave1OnClick);
    WgtMsgConnect(da_adjust_pg.btSave0, WGT_CLICK_MSG, btSave0OnClick);
}
static int DA_AdjustOnInit(DA_Adjust_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	SetDaAdjustCB(DaLinearAdj);
	SetDaAdjCB(DaAjustChange);

    return 0;
}
static int DA_AdjustOnShow(DA_Adjust_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

#ifdef WIN32 //便于制作机型da数据
    EditSetReadOnly(ppg->edit14, FALSE);
    EditSetReadOnly(ppg->edit50, FALSE);
#endif

	DaAdjustPageIn(ppg);

    //BkPrsLinera();//背压线性重新生成 20190107 hz
	DaPrsAdjustMax();

    //20181116 因为数据是1位小数，速度显示改为0位小数
    //EditSetDot(ppg->edit246,0);

    if((VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020)&&((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x04) == 0))
    {
        WGTSetVisible(ppg->checkbox8,FALSE);
    }
    else
    {
        WGTSetVisible(ppg->checkbox8,TRUE);
    }

    //20230518 chj 修复DA压力流量调整中，退出页面，DA调整输出无法选择
    WGTSetEnable(ppg->checkbox1, TRUE);
    WGTSetEnable(ppg->checkbox2, TRUE);
    WGTSetEnable(ppg->checkbox3, TRUE);
    WGTSetEnable(ppg->checkbox4, TRUE);
    WGTSetEnable(ppg->checkbox5, TRUE);
    WGTSetEnable(ppg->checkbox6, TRUE);
    WGTSetEnable(ppg->checkbox7, TRUE);
    WGTSetEnable(ppg->checkbox8, TRUE);

    //20241205 chj 金鹰DA实际电压值显示
    BOOL bShowVol = FALSE;
    if((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)
    {
        bShowVol = TRUE;
    }
    else
    {
        bShowVol = FALSE;
    }
    WGTSetVisible(ppg->lblpv1, bShowVol);
    WGTSetVisible(ppg->lblpv2, bShowVol);
    WGTSetVisible(ppg->lblfv1, bShowVol);
    WGTSetVisible(ppg->lblfv2, bShowVol);

    WGTSetVisible(ppg->pv1sign, bShowVol);
    WGTSetVisible(ppg->pv2sign, bShowVol);
    WGTSetVisible(ppg->fv1sign, bShowVol);
    WGTSetVisible(ppg->fv2sign, bShowVol);

    WGTSetVisible(ppg->eddavol1, bShowVol);
    WGTSetVisible(ppg->eddavol2, bShowVol);
    WGTSetVisible(ppg->eddavol3, bShowVol);
    WGTSetVisible(ppg->eddavol4, bShowVol);

    WGTSetVisible(ppg->lblpv1_unit, bShowVol);
    WGTSetVisible(ppg->lblpv2_unit, bShowVol);
    WGTSetVisible(ppg->lblfv1_unit, bShowVol);
    WGTSetVisible(ppg->lblfv2_unit, bShowVol);

    return 0;
}
static int DA_AdjustOnHide(DA_Adjust_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

	DaAdjustPageOut();

    old_pwgt = NULL;//20221028 dyl
    return 0;
}
static int DA_AdjustOnUpdate(DA_Adjust_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT      //20181123 hz
	DaAdjustUpdate();

    //20241205 chj 金鹰DA实际电压值显示
    char text[10];
    if((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)
    {
        UI16 DA_VOL;
        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL1) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(ppg->eddavol1, text);
        LabelSetText(ppg->pv1sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL1)>>15 != 0) ? "-":" ");

        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL2) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(ppg->eddavol2, text);
        LabelSetText(ppg->fv1sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL2)>>15 != 0) ? "-":" ");

        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL3) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(ppg->eddavol3, text);
        LabelSetText(ppg->pv2sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL3)>>15 != 0) ? "-":" ");

        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL4) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(ppg->eddavol4, text);
        LabelSetText(ppg->fv2sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL4)>>15 != 0) ? "-":" ");

    }

    return 0;
}
static int DA_AdjustOnChange(DA_Adjust_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	//UI16 value;
	UI16 index;
	//UI16 pdata[2];
	UI32 addr;
	PATTR pattr;
//    static PWGT old_pwgt = NULL;//20210914 dyl touch

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

    //20221026 dyl 上一个按钮是调整按钮：在调整中，触摸点击其他控件无效，要退出调整才能使用
    if(badjust)//调整按钮在调整中
    {
        if ((old_pwgt != NULL)&&(old_pwgt != pwgt)//本次操作的控件是否跟上一次的控件一致
                &&(pwgt != ppg->btup)&&(pwgt != ppg->btdown)&&(pwgt != ppg->btenter))//是否是调整按钮
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            return -1;
        }
    }

	switch(addr)
	{
	case TMP_DA_DASELECT: //DA选择
		DaGroupSel();
        //BkPrsLinera();//背压线性重新生成 20190107 hz
		DaPrsAdjustMax();
		break;
	case SYS_DA_TYPE://线性选择      
		if(OperateModeIndex() == MODE_MANUAL)
        {
            //20211101 dyl DA线性选择增加确认框
//			LinearSelect();
		}
		else
		{
			VarAdrSetInt(SYS_DA_TYPE,msg_value);
			ShowMsg(VW_MSG_MANUAL);
			return -1;
		}
		break;
	case TMP_DA_DACHANEL:  //背压通道
		BkPressChannel();
		DaPrsAdjustMax();
		break;
    case SYSTEM_DAPRES_BACKLIMIT: //背压上限
		BkPressLimit();
		DaPrsAdjustMax(); //20190107 hz
		break;
	case TMP_DA_ADJPRESS://DA压力调整
		DaPrsAdjust();
		break;
	case TMP_DA_ADJSPEED://DA流量调整
		DaSpdAdjust();
		break;
	case TMP_DA_PASSWORD:
		if (VarAdrToInt(TMP_DA_PASSWORD) == 95)
		{
			WGTSetEnable(ppg->btReset, TRUE);
		} 
		else
		{
			WGTSetEnable(ppg->btReset, FALSE);
		}
		break;
	default:
		if(pwgt ==  ppg->edit244)
		{
			PrsSpdForcedOutput();
		}
		else if(pwgt ==  ppg->edit246)
		{
			PrsSpdForcedOutput();
		}
        else if (pwgt == ppg->btok1)//20211101 dyl DA线性选择增加确认框
        {
            if(OperateModeIndex() == MODE_MANUAL)
            {
                if (nLineSelLast != VarAdrToInt(SYS_DA_TYPE))
                {
                    nLineSelLast = VarAdrToInt(SYS_DA_TYPE);
                    LinearSelect();
                }
            }
            else
            {
                VarAdrSetInt(SYS_DA_TYPE,nLineSelLast);
                ShowMsg(VW_MSG_MANUAL);
                return -1;
            }
        }
        else if(pwgt ==  ppg->btup) //20220518 20200107.cyx + //20210914 dyl touch
        {
            SendKey(_UARROW);
            SendKey(KEY_RELEASE);//20220622 dyl 发送弹起键
        }
        else if(pwgt ==  ppg->btdown)
        {
            SendKey(_DARROW);
            SendKey(KEY_RELEASE);//20220622 dyl 发送弹起键
        }
        else if(pwgt ==  ppg->btenter)
        {
            if(old_pwgt != NULL)
            {
                PageWgtSetFocus(old_pwgt);
            }
            SendKey(_ENTER);
            SendKey(KEY_RELEASE);//20220622 dyl 发送弹起键
        }
		break;
	}
	//adjust g_offset
	index =(UI16)WGTGetGOffset(pwgt);
	if(index>=1 && index <=30)
	{
        SendKey(KEY_RELEASE);//20220622 dyl 发送弹起键
        DaAdjust(index);
        //20221026 dyl 上一个按钮是调整按钮：在调整中，触摸点击其他控件无效，要退出调整才能使用
        if (old_pwgt != pwgt)
        {
            old_pwgt = pwgt;
        }
        else
        {
            old_pwgt = NULL;
        }
        if (badjust)
        {
            WGTSetEnable(ppg->checkbox1, FALSE);
            WGTSetEnable(ppg->checkbox2, FALSE);
            WGTSetEnable(ppg->checkbox3, FALSE);
            WGTSetEnable(ppg->checkbox4, FALSE);
            WGTSetEnable(ppg->checkbox5, FALSE);
            WGTSetEnable(ppg->checkbox6, FALSE);
            WGTSetEnable(ppg->checkbox7, FALSE);
            WGTSetEnable(ppg->checkbox8, FALSE);
        }
        else
        {
            WGTSetEnable(ppg->checkbox1, TRUE);
            WGTSetEnable(ppg->checkbox2, TRUE);
            WGTSetEnable(ppg->checkbox3, TRUE);
            WGTSetEnable(ppg->checkbox4, TRUE);
            WGTSetEnable(ppg->checkbox5, TRUE);
            WGTSetEnable(ppg->checkbox6, TRUE);
            WGTSetEnable(ppg->checkbox7, TRUE);
            WGTSetEnable(ppg->checkbox8, TRUE);
        }
//        //20210914 dyl touch
//        old_pwgt = pwgt;//20220518 20200107.cyx
        //if(!DaAdjustFlag())
        //{
            if(old_pwgt != NULL)
            {
                PageWgtSetFocus(old_pwgt);
            }
        //}
	}

	//线性值输入有修改
	if(index>=50 && index <=130)
	{
		VarAdrSetInt(DAADJ_CHANGE_FLAG,1);
	}

    return 0;
}

static int btSave0OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20221026 dyl 上一个按钮是调整按钮：在调整中，触摸点击其他控件无效，要退出调整才能使用
    if(badjust)//调整按钮在调整中
    {
        return -1;
    }
	DaAdjustSave();

    return 0;
}
static int btSave1OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20221026 dyl 上一个按钮是调整按钮：在调整中，触摸点击其他控件无效，要退出调整才能使用
    if(badjust)//调整按钮在调整中
    {
        return -1;
    }
	DaAdjustSave();

    return 0;
}
static int btSave2OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20221026 dyl 上一个按钮是调整按钮：在调整中，触摸点击其他控件无效，要退出调整才能使用
    if(badjust)//调整按钮在调整中
    {
        return -1;
    }
	DaAdjustSave();

    return 0;
}
static int btResetOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20221026 dyl 上一个按钮是调整按钮：在调整中，触摸点击其他控件无效，要退出调整才能使用
    if(badjust)//调整按钮在调整中
    {
        return -1;
    }
	DaAdjustReset();

    return 0;
}
