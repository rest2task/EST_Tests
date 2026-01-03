#include "pushtab.h"
#include "stdio.h"
#include "file.h"
#include "vardb.h"
#include "obj.h"
#include "oprintf.h" 
#include "monmcu.h"
//#include "questionDlg.h"
#include "protcmd.h"
#include "database.h"
#include "information.h"
#include "math.h"
#include "panelcfg.h" //20211210 dyl


#define  PUSHTAB_MAX_TRANSCNT   100//发送最大转换地址个数

TRANSMETER_ITEM g_dbTransMeterTable[NUM_PUSHTABLEDATA];
UI16 g_dbTransMeterCount = 0;
UI16 g_iMachine = 0;
BOOL g_recvmachcfg = FALSE; //配置表是否收到；
BOOL g_recvdatafinish = FALSE;//数据交互是否完成，用于是否可以对主机数据进行判断使用 + //20211210 dyl
BOOL g_recvdata_IsIng = FALSE;//数据交互是否进行中，用于在交互中一些操作不可进行需等待 + //20220106 dyl

static BOOL LoadPushTableCalcu(void* buf);


TRANSMETER_ITEM   g_dbTransMeterTableDefault[] = {
	//油缸行程 S0     模板行程SM       序号

	//广东思必拓行程：   移模行程：
	92  ,        47     ,
	0  ,          0     ,   //0
	65  ,      1        ,   //1
	130  ,     4       ,   //2
	195  ,     5       ,   //3
	260  ,     37       ,   //4
	324  ,     75       ,   //5
	389  ,     137      ,   //6
	454  ,     230      ,   //7
	519  ,     363      ,   //8
	584  ,     543      ,   //9
	649  ,     781      ,   //10
	714  ,     1083      ,   //11
	779  ,     1459      ,   //12
	844  ,     1915      ,   //13
	908  ,     2447      ,   //14
	973  ,     3076      ,   //15
	1038 ,     3794      ,   //16
	1103 ,     4600      ,   //17
	1168 ,     5489      ,   //18
	1233 ,     6453     ,   //19
	1298 ,     7484     ,   //20
	1363 ,     8571     ,   //21
	1428 ,     9702     ,   //22
	1492 ,     10847     ,   //23
	1557 ,     12031     ,   //24
	1622 ,     13225       ,  //25
	1687 ,     14420       ,  //26
	1752 ,     15607       ,  //27
	1817 ,     16780       ,  //28
	1882 ,     17934       ,  //29
	1947 ,     19065       ,  //30
	2012,      20170       ,  //31
	2076 ,     21229       ,  //32
	2141 ,     22277       ,  //33
	2206 ,     23294       ,  //34
	2271 ,     24282       ,  //35
	2336 ,     25241       ,  //36
	2401,      26172       ,  //37
	2466 ,     27077       ,  //38
	2531 ,     27956       ,  //39
	2596 ,     28812       ,  //40
	2660 ,     29636       ,  //41
	2725 ,     30457       ,  //42
	2790 ,     31265       ,  //43
	2855 ,     32069       ,  //44
	2920 ,     32878       ,  //45
	2985 ,     33711       ,  //46
	3050 ,     34611       ,  //47

	0,           0            //油缸型号
};

UI32 TransMeter(UI16 wPumpMeter)
{
    UI32 iMax,iMin,i;
	TRANSMETER_ITEM *item;

	if(g_iMachine == 0) return wPumpMeter;

	if(g_dbTransMeterCount == 0) return wPumpMeter;

    if(VarAdrToInt(USE_PUSHPARA))//20190813.cyx
    {
        item=(TRANSMETER_ITEM*)g_dbTransMeterTable2;
    }
    else
    {
        item=(TRANSMETER_ITEM*)g_dbTransMeterTable;
    }

	iMin=0;
	iMax=g_dbTransMeterCount-1;

    if (wPumpMeter==0xFFFF) return (UI32)item[iMax].wClamp/10;

	while (iMax-iMin>1){
		i=(iMax+iMin)/2;
		if (wPumpMeter < item[i].wPump) iMax=i;
		else iMin=i;
	}

	if (item[iMax].wPump==item[iMin].wPump)
        return (UI32)item[iMin].wClamp/10;
	else
        return (UI32)((float)(item[iMax].wClamp-item[iMin].wClamp)/(item[iMax].wPump-item[iMin].wPump)*(wPumpMeter-item[iMin].wPump)+item[iMin].wClamp)/10 ;
}

UI16 UnTransMeter(UI16 wClampMeter,BOOL bPoint2)
{
	UI16 iMax,iMin,i;
	UI32 wClamp_Meter;
	TRANSMETER_ITEM *item;

	if (wClampMeter==0xFFFF) return wClampMeter; 
	if (wClampMeter==0) return 0; 

	if (bPoint2) wClamp_Meter=wClampMeter;
	else wClamp_Meter=wClampMeter*10;

    if(VarAdrToInt(USE_PUSHPARA))//20190813.cyx
    {
        item=(TRANSMETER_ITEM*)g_dbTransMeterTable2;
    }
    else
    {
        item=(TRANSMETER_ITEM*)g_dbTransMeterTable;
    }

	iMin=0;
	iMax=g_dbTransMeterCount-1;

	while (iMax-iMin>1){
		i=(iMax+iMin)/2;
		if (wClamp_Meter < item[i].wClamp) iMax=i;
		else iMin=i;
	}

	if (item[iMax].wClamp==item[iMin].wClamp)
		return item[iMin].wPump;
	else
		return (UI16)( (float)(item[iMax].wPump-item[iMin].wPump)/(item[iMax].wClamp-item[iMin].wClamp)*(wClamp_Meter-item[iMin].wClamp)+item[iMin].wPump) ;
}

void PushToMaxMeter()
{
	static I16 adTempClamp = -1;
	//static I16 adAbsoluteClamp =-1;
    //static I16 adMaxClamp = -1;
    //static I16 adTransMaxClamp = -1;//推力座转换最大限制值
	static I16 wDatax= -1;
    //static I16 wDatax1= -1;
	//static I16 wDatax2= -1;
	UI16 wData;

	if(g_iMachine != 0)
	{
        if(adTempClamp != VarAdrToUI16(ADDR_ADPOSI_REAL2))
		{
			//20180621
			//if(VarAdrToUI16(ADMETER_ABSOLUTE2) >= VarAdrToUI16(ADMETER_ZSET2))
			//{
            //	adTempClamp = VarAdrToUI16(ADMETER_ZSET2) + VarAdrToUI16(ADDR_ADPOSI_REAL2);
			//	wData=TransMeter(adTempClamp)-TransMeter(VarAdrToUI16(ADMETER_ZSET2));
			//}
			//else
			//{
			//	adTempClamp = VarAdrToUI16(ADMETER_ABSOLUTE2);
			//	wData=TransMeter(VarAdrToUI16(ADMETER_ZSET2))-TransMeter(adTempClamp);
			//}
            adTempClamp = VarAdrToUI16(ADDR_ADPOSI_REAL2);
            VarAdrSetInt(AD_CLAMP_ABSOUTE,adTempClamp);//20211224 + //20220228 dyl
			wData = TransMeter(adTempClamp);
			wDatax = wData;
		}
        VarAdrSetInt(ADDR_ADPOSI_REAL2,wDatax);

		//20180621
		//if(adAbsoluteClamp != VarAdrToUI16(ADMETER_ABSOLUTE2))
		//{
		//	adAbsoluteClamp = VarAdrToUI16(ADMETER_ABSOLUTE2);
		//	wData = TransMeter(adAbsoluteClamp);
		//	wDatax2 = wData;
		//}
		//VarAdrSetInt(AD_CLAMP_ABSOUTE,wDatax2);

		//if(adMaxClamp != VarAdrToUI16(ADPOSI_METERMAXCLAMP))
		//{
		//	adMaxClamp = VarAdrToUI16(ADPOSI_METERMAXCLAMP);
		//	adTransMaxClamp = TransMeter(adMaxClamp);
		//	wData = TransMeter(0xffff);
		//	if(adTransMaxClamp<=wData)
		//		wDatax1 = adTransMaxClamp;
		//	else wDatax1 = wData;	
		//}
		//VarAdrSetInt(ADPOSI_METERMAXCLAMP,wDatax1);
	}
}

void MeterRecvData(UI16 set_id, int num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr)
{
	if(set_id == 129 + 0x100)//admeter data
	{
		PushToMaxMeter();
	}
}

//20241226 chj 威力士快速机模式下，部分参数变灰：关模高压二段,储料移动检测，储前电子尺检测，漏油检测功能,高压二段 数据赋值
void WLS_Fast_Spc()
{
    if(((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00) && VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE))
    {
        if(VarAdrToInt(d_clamp1_MACHSET_FL_CLSHIPRES2) != 1)
        {
            oprintf("set d_clamp1_MACHSET_FL_CLSHIPRES2\n");
            VarSendSaveSetIntByAdr(d_clamp1_MACHSET_FL_CLSHIPRES2,1);
        }

        if(VarAdrToInt(d_inject1_MOLDSET_FL_CHGMOVE) != 1)
        {
            oprintf("set d_inject1_MOLDSET_FL_CHGMOVE\n");
            VarSendSaveSetIntByAdr(d_inject1_MOLDSET_FL_CHGMOVE,1);
        }

        if(VarAdrToInt(d_inject1_MACHSET_FL_CHG_CHECKMM) != 0)
        {
            oprintf("set d_inject1_MOLDSET_PUMP_DEAD_INJ\n");
            VarSendSaveSetIntByAdr(d_inject1_MACHSET_FL_CHG_CHECKMM,0);
        }

        if(VarAdrToInt(d_oil1_MACHSET_FL_CHKOIL) != 1)
        {
            oprintf("set d_oil1_MACHSET_FL_CHKOIL\n");
            VarSendSaveSetIntByAdr(d_oil1_MACHSET_FL_CHKOIL,1);
        }

        if(VarAdrToInt(d_clamp1_ACTPARA_DAFLOW_CLSHIPRES2) > 30)
        {
            oprintf("set d_clamp1_ACTPARA_DAFLOW_CLSHIPRES2\n");
            VarSendSaveSetIntByAdr(d_clamp1_ACTPARA_DAFLOW_CLSHIPRES2,30);
        }

    }

    //20250109 fqh 威力士 关模高压压力上限最大值等初始化
    if(((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00) && VarAdrToInt(p_PP_MACHSET_WLSMACHINE_TYPE))
    {
        VarAdrSetInt(p_PP_MACHSET_CLSHIPRS_LIMIT_MAX_REAL,VarAdrToInt(p_PP_MACHSET_HIPRS_LIMIT_MAX)); //20250109 fqh 关模高压压力上限最大值
        VarAdrSetInt(p_PP_MACHSET_CHGBKPRS_LIMIT_MAX_REAL,VarAdrToInt(p_PP_MACHSET_CHGBKPRS_LIMIT_MAX)); //20250109 fqh 储料背压上限最大值
        VarAdrSetInt(p_PP_MACHSET_CHARGE_SPD_LIMIT_MAX_REAL,VarAdrToInt(p_PP_MACHSET_CHGSPD_LIMIT_MAX)); //20250109 fqh 储料速度上限最大值
    }
    else
    {
        VarAdrSetInt(p_PP_MACHSET_CLSHIPRS_LIMIT_MAX_REAL,VarAdrToInt(d_machine1_MACHSET_DAPRES_SYSLMT)); //20250109 fqh 关模高压压力上限最大值
        VarAdrSetInt(p_PP_MACHSET_CHGBKPRS_LIMIT_MAX_REAL,VarAdrToInt(d_machine1_MACHSET_DAPRES_SYSLMT)); //20250109 fqh 储料背压上限最大值
        VarAdrSetInt(p_PP_MACHSET_CHARGE_SPD_LIMIT_MAX_REAL,99); //20250109 fqh 储料速度上限最大值
    }
}

//20250109 fqh 海航增加润滑模式限制：润滑模式默认值为1，只有最高用户等级才可以修改润滑模式，其他等级不可修改
void HH_Spc()
{
    if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8F00)
    {
        oprintf("SYS_MACH_CUSTOMID:0x%x\n",VarAdrToInt(SYS_MACH_CUSTOMID));
        if(VarAdrToInt(d_lub1_MACHSET_FL_LUBMODE) != 1)
        {
            VarSendSaveSetIntByAdr(d_lub1_MACHSET_FL_LUBMODE,1);
        }

        if(VarAdrToInt(d_lub2_MACHSET_FL_LUBMODE) != 1)
        {
            VarSendSaveSetIntByAdr(d_lub2_MACHSET_FL_LUBMODE,1);
        }

        if(VarAdrToInt(d_lub3_MACHSET_FL_LUBMODE) != 1)
        {
            VarSendSaveSetIntByAdr(d_lub3_MACHSET_FL_LUBMODE,1);
        }
    }

}

void MainMachRecvData(UI32 address, UI16 len, UI16* pdat, UI8 rx_type)
{
	int i;
	UI32 addr;

	for(i=0;i<len;i++)
	{
		addr = address+i;
		if(addr == SYS_FL_MACHINEMODEL)
		{
            g_iMachine = VarAdrToUI16(SYS_FL_MACHINEMODEL);
			g_recvmachcfg = TRUE;
            WLS_Fast_Spc();
            //20250109 fqh 海航增加润滑模式限制：润滑模式默认值为1，只有最高用户等级才可以修改润滑模式，其他等级不可修改
            HH_Spc();
		}
		else if((addr >= ADDR_TEMP_RAMP1 && addr <= ADDR_TEMP_RAMP16) || (addr == ADPOSI_ADJCUR)
                || (addr >= TEMP_DAPID_KP_1 && addr <= TEMP_DAPID_TC_16) //20190121 温度优化
                || (addr >= d_hydr1_MACHSET_PRESSELF_P && addr <= d_hydr1_MACHSET_PRESSELF_I) //20200731
                )
		{
			VarAdrSaveInt(addr);//20190605
		}

        //20191212 新温度A缓存区保存
        if(addr>=d_TempCtrlComm1_MACHSET_DATEMP_RAMP_1 && addr<=d_TempCtrlComm1_MACHSET_DAPID_TD_16)
        {
            VarAdrSaveInt(addr);
        }
        //20191212 新温度B缓存区保存
        if(addr>=d_TempCtrlComm2_MACHSET_DATEMP_RAMP_1 && addr<=d_TempCtrlComm2_MACHSET_DAPID_TD_16)
        {
            VarAdrSaveInt(addr);
        }

        //20191212
        if((addr==d_clamp1_MOLDSET_SLOPE_MAX_CLS_SPD)||(addr==d_clamp1_MOLDSET_SLOPE_MAX_OPN_SPD)||(addr==d_clamp1_MOLDSET_PUMP_DEAD_CLS)||(addr==d_clamp1_MOLDSET_PUMP_DEAD))
        {
            VarAdrSaveInt(addr);
        }
        if((addr==d_eject1_MOLDSET_EJEF_MAX_SPD)||(addr==d_eject1_MOLDSET_PUMP_DEAD_EJEFWD)||(addr==d_eject1_MOLDSET_EJEB_MAX_SPD)||(addr==d_eject1_MOLDSET_PUMP_DEAD_EJEBWD))
        {
            VarAdrSaveInt(addr);
        }
        if((addr==d_inject1_MOLDSET_SLOPE_MAX_SUCK_SPD)||(addr==d_inject1_MOLDSET_PUMP_DEAD_SUCK))
        {
            VarAdrSaveInt(addr);
        }

        //20200407HJM 模温功能

        //MT1-加温缓冲区
        if(addr>=d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW0 && addr<=d_TempCtrlMW1_MOLDSET_DATEMP_RAMP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT1-输出周期时间
        if(addr>=d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW0 && addr<=d_TempCtrlMW1_MOLDSET_TM_RESPONSELESS_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT1-KP
        if(addr>=d_TempCtrlMW1_MOLDSET_DAPID_KP_MW0 && addr<=d_TempCtrlMW1_MOLDSET_DAPID_KP_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT1-KI
        if(addr>=d_TempCtrlMW1_MOLDSET_DAPID_TI_MW0 && addr<=d_TempCtrlMW1_MOLDSET_DAPID_TI_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT1-KD
        if(addr>=d_TempCtrlMW1_MOLDSET_DAPID_TD_MW0 && addr<=d_TempCtrlMW1_MOLDSET_DAPID_TD_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT2-加温缓冲区
        if(addr>=d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW0 && addr<=d_TempCtrlMW2_MOLDSET_DATEMP_RAMP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT2-输出周期时间
        if(addr>=d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW0 && addr<=d_TempCtrlMW2_MOLDSET_TM_RESPONSELESS_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT2-KP
        if(addr>=d_TempCtrlMW2_MOLDSET_DAPID_KP_MW0 && addr<=d_TempCtrlMW2_MOLDSET_DAPID_KP_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT2-KI
        if(addr>=d_TempCtrlMW2_MOLDSET_DAPID_TI_MW0 && addr<=d_TempCtrlMW2_MOLDSET_DAPID_TI_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT2-KD
        if(addr>=d_TempCtrlMW3_MOLDSET_DAPID_TD_MW0 && addr<=d_TempCtrlMW3_MOLDSET_DAPID_TD_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT3-加温缓冲区
        if(addr>=d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW0 && addr<=d_TempCtrlMW3_MOLDSET_DATEMP_RAMP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT3-输出周期时间
        if(addr>=d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW0 && addr<=d_TempCtrlMW3_MOLDSET_TM_RESPONSELESS_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT3-KP
        if(addr>=d_TempCtrlMW3_MOLDSET_DAPID_KP_MW0 && addr<=d_TempCtrlMW3_MOLDSET_DAPID_KP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT3-KI
        if(addr>=d_TempCtrlMW3_MOLDSET_DAPID_TI_MW0 && addr<=d_TempCtrlMW3_MOLDSET_DAPID_TI_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT3-KD
        if(addr>=d_TempCtrlMW3_MOLDSET_DAPID_TD_MW0 && addr<=d_TempCtrlMW3_MOLDSET_DAPID_TD_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT4-加温缓冲区
        if(addr>=d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW0 && addr<=d_TempCtrlMW4_MOLDSET_DATEMP_RAMP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT4-输出周期时间
        if(addr>=d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW0 && addr<=d_TempCtrlMW4_MOLDSET_TM_RESPONSELESS_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT4-KP
        if(addr>=d_TempCtrlMW4_MOLDSET_DAPID_KP_MW0 && addr<=d_TempCtrlMW4_MOLDSET_DAPID_KP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT4-KI
        if(addr>=d_TempCtrlMW4_MOLDSET_DAPID_TI_MW0 && addr<=d_TempCtrlMW4_MOLDSET_DAPID_TI_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT4-KD
        if(addr>=d_TempCtrlMW4_MOLDSET_DAPID_TD_MW0 && addr<=d_TempCtrlMW4_MOLDSET_DAPID_TD_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT5-加温缓冲区
        if(addr>=d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW0 && addr<=d_TempCtrlMW5_MOLDSET_DATEMP_RAMP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT5-输出周期时间
        if(addr>=d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW0 && addr<=d_TempCtrlMW5_MOLDSET_TM_RESPONSELESS_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT5-KP
        if(addr>=d_TempCtrlMW5_MOLDSET_DAPID_KP_MW0 && addr<=d_TempCtrlMW5_MOLDSET_DAPID_KP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT5-KI
        if(addr>=d_TempCtrlMW5_MOLDSET_DAPID_TI_MW0 && addr<=d_TempCtrlMW5_MOLDSET_DAPID_TI_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT5-KD
        if(addr>=d_TempCtrlMW5_MOLDSET_DAPID_TD_MW0 && addr<=d_TempCtrlMW5_MOLDSET_DAPID_TD_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT6-加温缓冲区
        if(addr>=d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW0 && addr<=d_TempCtrlMW6_MOLDSET_DATEMP_RAMP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT6-输出周期时间
        if(addr>=d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW0 && addr<=d_TempCtrlMW6_MOLDSET_TM_RESPONSELESS_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT6-KP
        if(addr>=d_TempCtrlMW6_MOLDSET_DAPID_KP_MW0 && addr<=d_TempCtrlMW6_MOLDSET_DAPID_KP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT6-KI
        if(addr>=d_TempCtrlMW6_MOLDSET_DAPID_TI_MW0 && addr<=d_TempCtrlMW6_MOLDSET_DAPID_TI_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT6-KD
        if(addr>=d_TempCtrlMW6_MOLDSET_DAPID_TD_MW0 && addr<=d_TempCtrlMW6_MOLDSET_DAPID_TD_MW15)
        {
            VarAdrSaveInt(addr);
        }

        //MT7-加温缓冲区
        if(addr>=d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW0 && addr<=d_TempCtrlMW7_MOLDSET_DATEMP_RAMP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT7-输出周期时间
        if(addr>=d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW0 && addr<=d_TempCtrlMW7_MOLDSET_TM_RESPONSELESS_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT7-KP
        if(addr>=d_TempCtrlMW7_MOLDSET_DAPID_KP_MW0 && addr<=d_TempCtrlMW7_MOLDSET_DAPID_KP_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT7-KI
        if(addr>=d_TempCtrlMW7_MOLDSET_DAPID_TI_MW0 && addr<=d_TempCtrlMW7_MOLDSET_DAPID_TI_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //MT7-KD
        if(addr>=d_TempCtrlMW7_MOLDSET_DAPID_TD_MW0 && addr<=d_TempCtrlMW7_MOLDSET_DAPID_TD_MW15)
        {
            VarAdrSaveInt(addr);
        }
        //20211129 ybc + //20211210 dyl
        if((PanelCfgStyle()==G_TYPE) ||(PanelCfgStyle()==P_TYPE)) //卓越
        {
            if(addr == d_Pump1_MACHSET_OIL_PUMP_DISPLACEMENT)
            {
                g_recvdatafinish = TRUE;
                g_recvdata_IsIng = FALSE;
            }
        }
        else
        {
            if(addr == d_hydr1_ZDSET_DA_PRSOUT0_1)
            {
                g_recvdatafinish = TRUE;
                g_recvdata_IsIng = FALSE;
            }
        }
	}
}

static BOOL LoadPushTable(void* buf)
{
	FILEHD filehd;
	int readbytes;

	filehd = FileOpen(PUSHTABDB_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
        if(VarAdrToInt(USE_PUSHPARA))//20190813.cyx
        {
            FileRead(filehd,(char*)buf,MAX_PUSHTABLENUMA*sizeof(TRANSMETER_ITEM),&readbytes);
        }
        else
        {
            FileRead(filehd,(char*)buf,NUM_PUSHTABLEDATA*sizeof(TRANSMETER_ITEM),&readbytes);
        }

		FileClose(filehd);
		return  TRUE;
	}
	return FALSE;
}

static void SavePushTable(void* buf,UI16 nCount)
{
	FILEHD filehd;
	int writebytes;

	filehd = FileOpen(PUSHTABDB_INIT_FILE);
	if(!FILEHD_IS_OK(filehd))
	{
		filehd = FileCreate(PUSHTABDB_INIT_FILE);
	}
	if(FILEHD_IS_OK(filehd))
	{
		FileWrite(filehd,(char*)buf,nCount*TRANSMETER_ITEM_SIZE,&writebytes);
	}
	FileClose(filehd);
}

void SavePushTableIndex(void* buf,UI16 index)
{
	int noff,writebytes;
	FILEHD filehd;

	filehd = FileOpen(PUSHTABDB_INIT_FILE);   
    //20190910 创建pushtab.dat文件
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(PUSHTABDB_INIT_FILE);
    }
	if(FILEHD_IS_OK(filehd))
	{
        noff = 0;
		FileSeek(filehd,noff+index*TRANSMETER_ITEM_SIZE);
		FileWrite(filehd,(char*)buf,TRANSMETER_ITEM_SIZE,&writebytes);
	}
    FileClose(filehd);
}

//UI32 SetClampPosPush(PATTR pdata, UI16 wData)
//{
//    BOOL bTrans = FALSE;
//    BOOL bPoint2 = FALSE;
//	UI32 wAddr;
//
//	if(pdata == NULL || pdata->pvar == NULL)
//		return 0;
//	if(g_iMachine == 0) 
//		return 0;
//	
//
//	wAddr = pdata->pvar->address;
//
//    if(wAddr>= ADPOSI_CLOSE1 && wAddr <= ADPOSI_CLOSE3) bTrans = TRUE;
//    else if(wAddr >= ADPOSI_OPEN2 && wAddr <= ADPOSI_OPEN5) bTrans = TRUE;
//    else if(wAddr == ADPOSI_CLOSELOW || wAddr == ADPOSI_OPEN1 || ADPOSI_CLOSEHIGH){
//        bTrans = TRUE;
//        bPoint2 = TRUE;
//    }
//	else if(wAddr == ADDR_ADPOSI_REAL2) bTrans = TRUE;//开关模电子尺
//    if(bTrans){
//        wData=UnTransMeter(wData,bPoint2);
//    }
//	return wData;
//}

void TransTxPack(UI32 address, UI16 len, UI16* pdat, BOOL brsp)
{
	static PQST_TBL pp = NULL;
	static int addrcnt = 0;
	static UI32 dwaddr[PUSHTAB_MAX_TRANSCNT];
	PQ_NODE  pn;
	int i,j,k;
	UI32 dwaddrs;

	if (g_iMachine==0) return ;
	
	if(pp == NULL)
	{
		pp = VarDBGetVarAttrTbl(VAR_IS_PUSHTBL_BIT);
		if(pp != NULL)
		{
			addrcnt = pp->num;
			if(addrcnt>PUSHTAB_MAX_TRANSCNT)
			{
				addrcnt = PUSHTAB_MAX_TRANSCNT;
			}
			dwaddr[0] = QSTFirst(pp)->key;
			for(i = 1;i<addrcnt;i++)
			{
				pn = QSTNext(pp);
				if(pn !=NULL)
				{
					dwaddr[i] = pn->key;
				}
				else
				{
					break;
				}
			}		
		}
	}

	dwaddrs = address;//(sub_id<<16)+offset;

    for(j=0;j<len;j++)
	{
		for(k=0; k<addrcnt; k++)
		{
			if(dwaddr[k] ==(dwaddrs+j))
			{
                //20210518.cyx
                if((dwaddr[k] == ADPOSI_CLOSELOW) || (dwaddr[k] == ADPOSI_CLOSEHIGH) || (dwaddr[k] == ADPOSI_OPEN1) || (dwaddr[k] == ADPOS_OPENSPD1) || (dwaddr[k] == d_clamp1_ACTPARA_ADPOS_ULT_CLPOPEN)) //two point
				{
					pdat[j] = UnTransMeter(pdat[j], TRUE); 
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART1)
				{
					if(VarAdrToUI16(FL_BLAST1)==1 || VarAdrToUI16(FL_BLAST1)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART2)
				{
					if(VarAdrToUI16(FL_BLAST2)==1 || VarAdrToUI16(FL_BLAST2)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART3)
				{
					if(VarAdrToUI16(FL_BLAST3)==1 || VarAdrToUI16(FL_BLAST3)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART4)
				{
					if(VarAdrToUI16(FL_BLAST4)==1 || VarAdrToUI16(FL_BLAST4)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART5)
				{
					if(VarAdrToUI16(FL_BLAST5)==1 || VarAdrToUI16(FL_BLAST5)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART6)
				{
					if(VarAdrToUI16(FL_BLAST6)==1 || VarAdrToUI16(FL_BLAST6)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART7)
				{
					if(VarAdrToUI16(FL_BLAST7)==1 || VarAdrToUI16(FL_BLAST7)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART8)
				{
					if(VarAdrToUI16(FL_BLAST8)==1 || VarAdrToUI16(FL_BLAST8)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART9)
				{
					if(VarAdrToUI16(FL_BLAST9)==1 || VarAdrToUI16(FL_BLAST9)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART10)
				{
					if(VarAdrToUI16(FL_BLAST10)==1 || VarAdrToUI16(FL_BLAST10)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART11)
				{
					if(VarAdrToUI16(FL_BLAST11)==1 || VarAdrToUI16(FL_BLAST11)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART12)
				{
					if(VarAdrToUI16(FL_BLAST12)==1 || VarAdrToUI16(FL_BLAST12)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART13)
				{
					if(VarAdrToUI16(FL_BLAST13)==1 || VarAdrToUI16(FL_BLAST13)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				else if(dwaddr[k] == ADPOSI_BLASTSTART14)
				{
					if(VarAdrToUI16(FL_BLAST14)==1 || VarAdrToUI16(FL_BLAST14)==5)
						pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
                else if(dwaddr[k] == d_airblow1_MOLDSET_ADPOSI_MOVINGBLASTSTART2)//20241210 chj 28组吹气
                {
                    if(VarAdrToUI16(d_airblow1_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow1_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow2_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow2_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow2_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow3_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow3_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow3_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow4_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow4_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow4_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow5_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow5_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow5_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow6_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow6_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow6_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow7_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow7_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow7_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow8_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow8_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow8_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow9_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow9_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow9_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow10_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow10_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow10_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow11_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow11_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow11_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow12_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow12_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow12_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow13_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow13_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow13_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
                else if(dwaddr[k] == d_airblow14_MOLDSET_ADPOSI_MOVINGBLASTSTART2)
                {
                    if(VarAdrToUI16(d_airblow14_MOLDSET_FL_BLAST2)==1 || VarAdrToUI16(d_airblow14_MOLDSET_FL_BLAST2)==5)
                        pdat[j] = UnTransMeter(pdat[j],FALSE);
                }
				else 
				{
					pdat[j] = UnTransMeter(pdat[j],FALSE);
				}
				break;
			}
		}
    }
}

void InitPushTable()
{
	TRANSMETER_ITEM *item;
	int i=0;

    if(VarAdrToInt(USE_PUSHPARA))//20190813.cyx
    {
        memset (g_dbTransMeterTableCalcu, 0, MAX_PUSHTABLENUM*sizeof(TRANSMETER_ITEM));
        LoadPushTableCalcu((char*)g_dbTransMeterTableCalcu);

        memset (g_dbTransMeterTable2, 0, MAX_PUSHTABLENUMA*sizeof(TRANSMETER_ITEM));
        LoadPushTable((char*)g_dbTransMeterTable2);

        item=(TRANSMETER_ITEM*)g_dbTransMeterTable2;

        i=1;
        while ((++item)->wPump && (i<MAX_PUSHTABLENUMA))	i++;
    }
    else
    {
        memset (g_dbTransMeterTable, 0, NUM_PUSHTABLEDATA*sizeof(TRANSMETER_ITEM));
        LoadPushTable((char*)g_dbTransMeterTable);

        item=(TRANSMETER_ITEM*)g_dbTransMeterTable;

        i=1;
        while ((++item)->wPump && (i<NUM_PUSHTABLEDATA))	i++;
    }

	g_dbTransMeterCount=i;
}

BOOL SetMachineType(UI16 wType)
{
	UI16 i;
	BOOL bSuccess;
	TRANSMETER_ITEM *item;
	g_iMachine = wType;

    if(VarAdrToInt(USE_PUSHPARA))//20190813.cyx
    {
        memset(g_dbTransMeterTable2, 0, MAX_PUSHTABLENUMA*sizeof(TRANSMETER_ITEM));
        bSuccess=LoadPushTable((char*)g_dbTransMeterTable2);
        if(!bSuccess){
            item=(TRANSMETER_ITEM*)&g_dbTransMeterTableDefault;
            while (item->wPump) {
                if (item->wPump==wType) {
                    SavePushTable((char*)(item+1),(UI16)item->wClamp+1);
                    memcpy(g_dbTransMeterTable2,(item+1),(item->wClamp+1)*sizeof(TRANSMETER_ITEM));
                    break;
                }
                item+=item->wClamp+2;
            }
        }
        //find max
        item=(TRANSMETER_ITEM*)g_dbTransMeterTable2;

        i=1;
        while ((++item)->wPump && (i<MAX_PUSHTABLENUMA))	i++;
    }
    else
    {
        memset(g_dbTransMeterTable, 0, NUM_PUSHTABLEDATA*sizeof(TRANSMETER_ITEM));
        bSuccess=LoadPushTable((char*)g_dbTransMeterTable);
        if(!bSuccess){
            item=(TRANSMETER_ITEM*)&g_dbTransMeterTableDefault;
            while (item->wPump) {
                if (item->wPump==wType) {
                    SavePushTable((char*)(item+1),(UI16)item->wClamp+1);
                    memcpy(g_dbTransMeterTable,(item+1),(item->wClamp+1)*sizeof(TRANSMETER_ITEM));
                    break;
                }
                item+=item->wClamp+2;
            }
        }
        //find max
        item=(TRANSMETER_ITEM*)g_dbTransMeterTable;

        i=1;
        while ((++item)->wPump && (i<NUM_PUSHTABLEDATA))	i++;
    }

	g_dbTransMeterCount=i;

	return TRUE;
}

void PushtabInital(PCMOD pmod)
{
	DataSetCbSet(129+0x100,MeterRecvData);//admeter data 设置回调函数
	PartDataRxCbSet(MainMachRecvData);// mach cfg 设置回调函数
	PartDataTxCbSet(TransTxPack);//
}

UI16 PushtableType()
{
	return g_iMachine;
}

BOOL MachCfgRecv()
{
	return g_recvmachcfg;
}

void SetPushtableType(UI16  push_type)
{
	g_iMachine = push_type;
}

UI32 SetClampPosPush(PATTR pdata, UI16 wData)
{
	BOOL bTrans = FALSE;
	BOOL bPoint2 = FALSE;
	UI32 wAddr;
	PVAR pvar;

	pvar = OBJAttrGetVar(pdata);
	if(pdata == NULL || pvar == NULL)
		return 0;
	if(PushtableType() == 0) 
		return 0;


	wAddr = VarToVarAdr(pvar);

	if(wAddr>= ADPOSI_CLOSE1 && wAddr <= ADPOSI_CLOSE3) bTrans = TRUE;
	else if(wAddr >= ADPOSI_OPEN2 && wAddr <= ADPOSI_OPEN5) bTrans = TRUE;
    else if(wAddr == ADPOSI_CLOSELOW || wAddr == ADPOSI_OPEN1 || wAddr == ADPOSI_CLOSEHIGH || wAddr == ADPOS_OPENSPD1)//20200317
	{
		bTrans = TRUE;
		bPoint2 = TRUE;
	}
    else if(wAddr == ADDR_ADPOSI_REAL2) bTrans = TRUE;//开关模电子尺
    else if(wAddr == ADPOS_OPENSPD3) bTrans = TRUE;//20200317
	if(bTrans){
		wData=UnTransMeter(wData,bPoint2);
	}
	return wData;
}

//20211210 dyl
BOOL DataRecvFinish()
{
    return g_recvdatafinish;
}
void SetDataRecvFlag(BOOL bRecv)
{
    g_recvdatafinish = bRecv;
}

//数据交互是否进行中 + //20220106 dyl
BOOL IsDataRecvIng()
{
    return g_recvdata_IsIng;
}
void SetDataRecv_IngFlag(BOOL bRecv)
{
    g_recvdata_IsIng = bRecv;
}

/********************五支点新增*****************/
TRANSMETER_ITEM	g_dbTransMeterTable2[MAX_PUSHTABLENUMA];
TRANSMETER_ITEM	g_dbTransMeterTableCalcu[MAX_PUSHTABLENUM];

static BOOL LoadPushTableCalcu(void* buf)
{
    FILEHD filehd;
    int readbytes;

    filehd = FileOpen(PUSHTAB_CALCU_FILE);
    if(FILEHD_IS_OK(filehd))
    {
        FileRead(filehd,(char*)buf,MAX_PUSHTABLENUM*sizeof(TRANSMETER_ITEM),&readbytes);
        FileClose(filehd);
        return  TRUE;
    }
    return FALSE;
}

static void SavePushTableCalcu(void* buf,UI16 nCount)
{
    FILEHD filehd;
    int writebytes;

    filehd = FileOpen(PUSHTAB_CALCU_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(PUSHTAB_CALCU_FILE);
    }
    if(FILEHD_IS_OK(filehd))
    {
        FileWrite(filehd,(char*)buf,nCount*TRANSMETER_ITEM_SIZE,&writebytes);
    }
    FileClose(filehd);
}

//五支点计算推力座
void CalculateData(double L1,double L2,double L3,double L4,double L5,double H,double h,double Hm,double limitangle)
{
    double omega = acos((L1*L1+L5*L5-L3*L3)/(2*L1*L5));
//    double limitangle = asin((H-Hm)/(L1+L2))*180/M_PI;
    double Smmax = L1*cos(limitangle*M_PI/180)+L2*cos((2*M_PI)+asin(((H-Hm)/L2)-(L1/L2)*sin(limitangle*M_PI/180)));
    double phimax = asin(((H-h)-L5*sin(omega+limitangle*M_PI/180))/L4)*180/M_PI;
    double Sgmax,SmOld,SmOldCalcu;
    int wpushcount,k,i,MAX_LOOPNUM,wpushcalcucount;

    oprintf("omega = %f,phimax = %f\n",omega,phimax);
    //加phimax判断防止用户输入错误出现复数,出现复数自动更正
    if(isnan(phimax) || (limitangle==0))
    {
        double k;
        VarAdrSetInt(SHOW_LMTAG_ERROR,1);//提示极限夹角输入有误
        limitangle = asin((H-Hm)/(L1+L2))*180/M_PI;
        for(k = 0;k<10;k=k+0.01)
        {
            limitangle = limitangle + k;
            phimax = asin(((H-h)-L5*sin(omega+limitangle*M_PI/180))/L4)*180/M_PI;
            if(isnan(phimax))
            {
                continue;
            }
            else
            {
                VarAdrSetInt(p_PP_MACHSET_PUSHPARA_LMTAG, limitangle*10000);//20200811
                VarAdrSaveInt(p_PP_MACHSET_PUSHPARA_LMTAG);//20200811
                break;
            }
        }
        oprintf("omega = %f,phimax = %f,limitangle = %f\n",omega,phimax,limitangle);
        Smmax = L1*cos(limitangle*M_PI/180)+L2*cos((2*M_PI)+asin(((H-Hm)/L2)-(L1/L2)*sin(limitangle*M_PI/180)));
    }
    Sgmax = L5*cos(limitangle*M_PI/180+omega) - L4*cos(phimax*M_PI/180);

    //oprintf("Smmax = %f,Sgmax = %f\n",Smmax,Sgmax);

    memset (g_dbTransMeterTable2, 0, MAX_PUSHTABLENUMA*sizeof(TRANSMETER_ITEM));
    memset (g_dbTransMeterTableCalcu, 0, MAX_PUSHTABLENUM*sizeof(TRANSMETER_ITEM));

    wpushcount = 0;
    k = 1;//循环次数
    MAX_LOOPNUM = 10000;//最大循环数
    SmOld = 0;//推力座上一个值,用于比较
    //计算生成表一,两位小数
    for(i = 0;i < MAX_LOOPNUM;i = i+k){
        double Sg = Sgmax - i;
        double theta1 = asin((H-Hm)/(L1+L2));
        double phi = acos( (-Sg)/sqrt(Sg*Sg+(H-h)*(H-h)) ) - acos((Sg*Sg+(H-h)*(H-h)+L4*L4-L5*L5)/(2*L4*sqrt(Sg*Sg+(H-h)*(H-h))));
        double alpha = acos((Sg+L4*cos(phi))/L5)-omega-theta1;
        double alpha2 = (2*M_PI)+asin(((H-Hm)/L2)-(L1/L2)*sin(alpha+theta1))*(-1);
        double Sm = Smmax - (L1*cos(alpha+theta1)+L2*cos(alpha2));

        //oprintf("input = %d,Sm = %.8f\n",i,Sm);

        //Sm为nan值退出循环
        if(isnan(Sm))
        {
            break;
        }

        if(i == 0)
        {
            SmOld = Sm;
            wpushcount = 0;
            g_dbTransMeterTable2[wpushcount].wPump  = i*10;
            g_dbTransMeterTable2[wpushcount].wClamp = (UI32)(Sm*100);
            wpushcount++;
        }

        if(wpushcount < MAX_PUSHTABLENUMA)
        {
            if(abs(Sm*1000-SmOld*1000) >= 10)   //相差0.01取值
            {
                SmOld = Sm;
                g_dbTransMeterTable2[wpushcount].wPump  = i*10;
                g_dbTransMeterTable2[wpushcount].wClamp = (UI32)((Sm+0.005)*100);//四舍五入
                wpushcount++;
                //oprintf("wpushcount = %d\n",wpushcount);
                //若推力座最大个数大于MAX_PUSHTABLENUMA,则进入下一个循环,下个循环间距扩大一倍
                if(wpushcount == MAX_PUSHTABLENUMA)
                {
                    //验证当前Sm是否为最后一个数
                    i = i+k;
                    Sg = Sgmax - i;
                    theta1 = asin((H-Hm)/(L1+L2));
                    phi = acos( (-Sg)/sqrt(Sg*Sg+(H-h)*(H-h)) ) - acos((Sg*Sg+(H-h)*(H-h)+L4*L4-L5*L5)/(2*L4*sqrt(Sg*Sg+(H-h)*(H-h))));
                    alpha = acos((Sg+L4*cos(phi))/L5)-omega-theta1;
                    alpha2 = (2*M_PI)+asin(((H-Hm)/L2)-(L1/L2)*sin(alpha+theta1))*(-1);
                    Sm = Smmax - (L1*cos(alpha+theta1)+L2*cos(alpha2));
                    if(isnan(Sm))
                    {
                        break;
                    }
                    //若不是最后一个值,则进入下一次循环
                    memset (g_dbTransMeterTable2, 0, MAX_PUSHTABLENUMA*sizeof(TRANSMETER_ITEM));
                    wpushcount = 0;
                    k = 2*k;
                    i = 0-k;//下次循环从零开始
                    oprintf("/*******************Enter next loop*******************/\n");
                    oprintf("i = %d,k = %d\n",i,k);
                    continue;
                }
            }
        }

        //计算速比si
//        double i1=(L1*sin(alpha+theta1+alpha2))/(L5*cos(alpha2)*sin(alpha+theta1+omega));
//        double i2=(cos(phi)*sin(alpha+theta1+omega))/(sin(alpha+theta1+omega+phi));
//        double si=i2*i1;

    }

    wpushcalcucount = 0;
    k = 1;//重置循环次数
    SmOldCalcu = 0;//推力座上一个值,用于比较
    //计算生成表二,四位小数
    for(i = 0;i < MAX_LOOPNUM;i = i+k){
        double Sg = Sgmax - i;
        double theta1 = asin((H-Hm)/(L1+L2));
        double phi = acos( (-Sg)/sqrt(Sg*Sg+(H-h)*(H-h)) ) - acos((Sg*Sg+(H-h)*(H-h)+L4*L4-L5*L5)/(2*L4*sqrt(Sg*Sg+(H-h)*(H-h))));
        double alpha = acos((Sg+L4*cos(phi))/L5)-omega-theta1;
        double alpha2 = (2*M_PI)+asin(((H-Hm)/L2)-(L1/L2)*sin(alpha+theta1))*(-1);
        double Sm = Smmax - (L1*cos(alpha+theta1)+L2*cos(alpha2));

        //Sm为nan值退出循环
        if(isnan(Sm))
        {
            break;
        }

        if(i == 0)
        {
            SmOldCalcu = Sm;
            wpushcalcucount = 0;
            g_dbTransMeterTableCalcu[wpushcalcucount].wPump  = i*10;
            g_dbTransMeterTableCalcu[wpushcalcucount].wClamp = (UI32)(Sm*10000);
            wpushcalcucount++;
        }


        if(wpushcalcucount < MAX_PUSHTABLENUM)  //相差0.0001取值
        {
            if(abs(Sm*100000-SmOldCalcu*100000) >= 10)
            {
                SmOldCalcu = Sm;
                g_dbTransMeterTableCalcu[wpushcalcucount].wPump  = i*10;
                g_dbTransMeterTableCalcu[wpushcalcucount].wClamp = (UI32)((Sm+0.00005)*10000);//四舍五入
                wpushcalcucount++;
                //若推力座最大个数大于MAX_PUSHTABLENUM,则进入下一个循环,下个循环间距扩大一杯
                if(wpushcalcucount == MAX_PUSHTABLENUM)
                {
                    //验证当前Sm是否为最后一个数
                    i = i+k;
                    Sg = Sgmax - i;
                    theta1 = asin((H-Hm)/(L1+L2));
                    phi = acos( (-Sg)/sqrt(Sg*Sg+(H-h)*(H-h)) ) - acos((Sg*Sg+(H-h)*(H-h)+L4*L4-L5*L5)/(2*L4*sqrt(Sg*Sg+(H-h)*(H-h))));
                    alpha = acos((Sg+L4*cos(phi))/L5)-omega-theta1;
                    alpha2 = (2*M_PI)+asin(((H-Hm)/L2)-(L1/L2)*sin(alpha+theta1))*(-1);
                    Sm = Smmax - (L1*cos(alpha+theta1)+L2*cos(alpha2));
                    if(isnan(Sm))
                    {
                        break;
                    }
                    //若不是最后一个值,则进入下一次循环
                    memset (g_dbTransMeterTableCalcu, 0, MAX_PUSHTABLENUM*sizeof(TRANSMETER_ITEM));
                    wpushcalcucount = 0;
                    k = 2*k;
                    i = 0-k;//下次循环从零开始
                    continue;
                }
            }
        }


        //计算速比si
//        double i1=(L1*sin(alpha+theta1+alpha2))/(L5*cos(alpha2)*sin(alpha+theta1+omega));
//        double i2=(cos(phi)*sin(alpha+theta1+omega))/(sin(alpha+theta1+omega+phi));
//        double si=i2*i1;

    }

    //保存两张表
    SavePushTable((char*)g_dbTransMeterTable2,MAX_PUSHTABLENUMA);
    SavePushTableCalcu((char*)g_dbTransMeterTableCalcu,MAX_PUSHTABLENUM);

}
/*********************************************/
