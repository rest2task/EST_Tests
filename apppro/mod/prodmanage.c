#include "prodmanage.h"
#include "stdio.h"
#include "oprecord.h"
#include "monmcu.h"
#include "prodmoni.h"
#include "database.h"
#include "power.h"
#include "tablectl.h"
#include "servo.h"
#include "pushtab.h"
#include "prodpower.h"
#include "verinfo.h"
#include "ctrlcmd.h"
#include "panelcfg.h"
#include "lubr.h"
#include "texttbl.h"//20211112 dyl 开机总时间、运行总计时统计
#include "privdef.h"

typedef enum enCLAMP_MODE{
	MODE_OPENCNT=0,
	MODE_PRODCNT
}CLAMPMODE;

//20211112 dyl 开机总时间、运行总计时统计
TIMECOUNT m_dbTimeCount;

TIMECOUNT m_dbTimeCountDefault = {
    0xEB90,              //已使用标志 EB90 wMarkUsed
    0,//                 WORD wTotalBootTime_H;           //1 历史累计开机总时间 high 16//NET_FUNC 2019.5.8 csj
    0,//                 WORD wTotalBootTime_L;           //2 历史累计开机总时间 low 16
    0,//                 WORD wBootRunTime_H;             //3 本次开机时间 high 16
    0,//                 WORD wBootRunTime_L;             //4 本次开机时间 low 16
    0,//                 WORD wTotalProdTime_H;           //5 历史生产运行总时间 high 16
    0,//                 WORD wTotalProdTime_L;           //6 历史生产运行总时间 low 16
    0,//                 WORD wTotalMotorTime_H;          //7 历史马达开累计时间 high 16
    0,//                 WORD wTotalMotorTime_L;          //8 历史马达开累计时间 low 16
    {0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0},//  WORD wReserve[RESERVE2_NUM];     //预留16个数据地址
};
/***************end //20211112 dyl************/

static void ProdRecvData(UI16 set_id, int num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr);
static void ProdRecvData128(UI16 set_id, int num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr);
//static CPROC_INFO ProdManageProcs[] = 
//{
//	{"clearCnt",	ProdClrCnt},
//	{"clearPkg",	ProdClrPkg},
//	{"setGoalCnt",	ProdSetGoalCnt},
//	{"setPerMold",   ProdSetPerMold},
//	{"changeMode",  ProdChgMode}
//};

void ProdInit()
{
	DataSetCbSet(1,ProdRecvData);//周期数据设置回调函数
	DataSetCbSet(128 + 0x100,ProdRecvData128);//设置回调函数
}

/************************************************************************/
/* 开模数清零                                                   */
/************************************************************************/
#include "iotnet.h" //NET_FUNC
BOOL ProdClrCnt()
{
#ifndef WIN32
	IOT_SaveTotalClampOpen();//NET_FUNC
#endif
    OPWRRecord(OP_CLEANSHOT);
    VarAdrSetInt(PROD_TOTAL_SHOTCNT,0);
    VarAdrSetInt(PROD_TOTALPRODCNT,0);
    VarAdrSetInt(p_PP_STATE_goodCntTotal,0);
    VarAdrSetInt(p_PP_STATE_badShot,0);
    //VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING1,0);
    //VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING2,0);
    VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM0,0);
    VarSendByAdr(CLAMP_STATE_MOLDOPNNUM0);
    VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM1,0);
    VarSendByAdr(CLAMP_STATE_MOLDOPNNUM1);
    VarAdrSetInt(CLAMP_STATE_CURPACKMOLD,0);
    VarSendByAdr(CLAMP_STATE_CURPACKMOLD);
    VarAdrSetInt(CLAMP_STATE_VALIDMOLD,0);
    VarSendByAdr(CLAMP_STATE_VALIDMOLD);

	VarAdrSetInt(PROD_SHOTCNT,0);
	VarAdrSaveInt(PROD_SHOTCNT);
    VarAdrSetInt(PROD_PKGCNT, 0);   //20190614 hz
    VarAdrSaveInt(PROD_PKGCNT);
    VarAdrSetInt(PROD_GOODCNT,0);//20220923 dyl 增加良品数保存
    VarAdrSaveInt(PROD_GOODCNT);

    ShotCntSave();
	return TRUE;
}

/************************************************************************/
/* 本包开模数清零                                               */
/************************************************************************/
BOOL ProdClrPkg()
{
    VarAdrSetInt(CLAMP_STATE_CURPACKMOLD,0);
    VarAdrSetInt(PROD_PKGCNT, 0);   //20190614 hz
    VarAdrSaveInt(PROD_PKGCNT);
    VarSendByAdr(CLAMP_STATE_CURPACKMOLD);
    ShotCntSave();
	return TRUE;
}

/************************************************************************/
/* 设置的目标数                                                 */
/************************************************************************/
BOOL ProdSetGoalCnt()
{
	UI32 cnt;
	UI32 percnt,opencnt;

	cnt = VarAdrToInt(PROD_TARGETCNT);
	percnt = VarAdrToInt(PROD_PERCNT);

    if(VarAdrToInt(CLAMP_MOLD_MOLDREACHCHK) == MODE_PRODCNT)
    {
        if(cnt%percnt == 0)
        {
            opencnt = cnt/percnt;
        }
        else
        {
            opencnt = cnt/percnt+1;
        }
        VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING1,opencnt>>16);
        VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING2,opencnt&0xFFFF);
    }
    else
    {
        VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING1,cnt>>16);
        VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING2,cnt&0xFFFF);
    }
    VarSendByAdr(CLAMP_MOLD_PRODUCTSETTING1);
    VarAdrSaveInt(CLAMP_MOLD_PRODUCTSETTING1);
    VarSendByAdr(CLAMP_MOLD_PRODUCTSETTING2);
    VarAdrSaveInt(CLAMP_MOLD_PRODUCTSETTING2);

	return TRUE;
}

/************************************************************************/
/* 每模产品数                                                   */
/************************************************************************/
BOOL ProdSetPerMold()
{
	UI32 cnt;
	UI32 percnt,opencnt;

	cnt = VarAdrToInt(PROD_TARGETCNT);
	percnt = VarAdrToInt(PROD_PERCNT);

    if(VarAdrToInt(CLAMP_MOLD_MOLDREACHCHK) == MODE_PRODCNT)
    {
        if(cnt%percnt == 0)
        {
            opencnt = cnt/percnt;
        }
        else
        {
            opencnt = cnt/percnt+1;
        }
        VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING1,opencnt>>16);
        VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING2,opencnt&0xFFFF);
    }
    VarSendByAdr(CLAMP_MOLD_PRODUCTSETTING1);
    VarAdrSaveInt(CLAMP_MOLD_PRODUCTSETTING1);
    VarSendByAdr(CLAMP_MOLD_PRODUCTSETTING2);
    VarAdrSaveInt(CLAMP_MOLD_PRODUCTSETTING2);

	return TRUE;
}

/************************************************************************/
/* 计数方式改变                                                 */
/************************************************************************/
BOOL ProdChgMode()
{
	UI32 cnt,percnt;
	cnt = VarAdrToInt(PROD_TARGETCNT);
	percnt = VarAdrToInt(PROD_PERCNT);
	if(percnt == 0)
	{
		VarAdrSetInt(PROD_PERCNT,1);
		percnt = 1;
	}

    if(VarAdrToInt(CLAMP_MOLD_MOLDREACHCHK) == MODE_PRODCNT)
    {
        if(cnt%percnt == 0)
        {
            cnt = cnt/percnt;
        }
        else
        {
            cnt = cnt/percnt+1;
        }
    }
    VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING1,cnt>>16);
    VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING2,cnt&0xFFFF);
    VarSendByAdr(CLAMP_MOLD_PRODUCTSETTING1);
    VarAdrSaveInt(CLAMP_MOLD_PRODUCTSETTING1);
    VarSendByAdr(CLAMP_MOLD_PRODUCTSETTING2);
    VarAdrSaveInt(CLAMP_MOLD_PRODUCTSETTING2);

	return TRUE;
}

//void ProdManageInital(PCMOD pmod)
//{
//	RegisterModProcs(pmod, ProdManageProcs, ARRAY_NUM(ProdManageProcs));
//	ProdInit();
//}


/************************************************************************/
/* 动作步监控                                                   */
/************************************************************************/
static void ProdStepMonitor()
{

}
#include "oprintf.h"
/**
* @brief     :周期数据接收函数
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void ProdRecvData(UI16 set_id, int num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr)
{
	if(set_id == 1)//每模上传数据 id为1
	{
		static UI32 shotCount=0;
        static UI32 shotCountx=0;//20211207 dyl 开模总数不清零
        UI32 curCount= (VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16) | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1);
        UI32 curCountx= (VarAdrToInt(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL)<<16) | VarAdrToInt(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL);
        g_dbPower.dwOpenCnt = VarAdrToInt(d_machine1_STATE_MODOPENNUM1);//20201104
		GetHeaterPower(); //  20190418 hz
		if(shotCount != curCount)
		{
			shotCount = curCount;
            MoniContinueCntWrite();//20230518 chj 写文件次数监控
            VarAdrSetInt(PROD_TOTAL_SHOTCNT,shotCount);
			ProdMonitor();
			ProdEnergy();
			ProdStepMonitor();
            ProdPower(); //20190403 hz
            VarAdrSetInt(POWER_ON_MOLD, VarAdrToInt(POWER_ON_MOLD) + 1); //20190717 hz
            //CheckHMDLubCnt();//20230109 dyl//当润滑倒计数等于１时，润滑计时清零 //20210507

            //20190610 hz  真空发生器功能 兼容威力仕特殊机
            if(((GetPanelType() == PANEL_V10) || (PanelCfgStyle()==G_TYPE))&& ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00))
            {
                VarAdrSetInt(SWCYCLEMOLDCURRENTCNT, VarAdrToInt(SWCYCLEMOLDCURRENTCNT) + 1);
                if((VarAdrToInt(SWCYCLEMOLDCURRENTCNT) >= VarAdrToInt(SWCYCLEMOLDCNT)) && (VarAdrToInt(SWCYCLEMOLDCNT) > 0)) //20220228 dyl
                {
                    ProtSysCtrl(OPENRELVLV_CMD,0,NULL);
                    VarAdrSetInt(SWCYCLEMOLDCURRENTCNT, 0);
                }
            }
		}
        if(shotCountx != curCountx)//20211207 dyl 开模总数不清零
        {
            shotCountx = curCountx;
            VarAdrSetInt(p_PP_STATE_SHOT_TOTAL_CNT, shotCountx);
        }
		//上传开模数赋值到设定地址
        //VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING1,VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0));
        //VarAdrSetInt(CLAMP_MOLD_PRODUCTSETTING2,VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1));
        VarAdrSetInt(PROD_SHOTCNT,(VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16 | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1)));
		//OWARN("PROD_SHOTCNT =%d",VarAdrToInt(PROD_SHOTCNT));

        VarAdrSetInt(p_PP_MACHSET_SHOTCNT_TOTAL,(VarAdrToInt(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL)<<16 | VarAdrToInt(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL)));//20211207 dyl 开模总数不清零
	}
}

static void ProdRecvData128(UI16 set_id, int num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr)
{
	if(set_id == 128 + 0x100)
	{
        UI32 curCount= (VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16) | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1);
        VarAdrSetInt(PROD_TOTAL_SHOTCNT,curCount);
        VarAdrSetInt(PROD_TOTALPRODCNT,curCount*VarAdrToInt(PROD_PERCNT));

		//if(bMachcfgReceive){
		//if((VarAdrToUI16(SYS_FL_MACH_CODE48) & 0x0100)==0){
		//	if(VarAdrToUI16(SYS_STATE_FL_AUTOALARM) != VarAdrToUI16(AUTOALARM_STATE)){
		//		VarAdrSetInt(SYS_STATE_FL_AUTOALARM,VarAdrToUI16(AUTOALARM_STATE));
		//		VarSendByAdr(SYS_STATE_FL_AUTOALARM);
		//	}
		//}
		//}
		VarAdrSetInt(PROD_PKGCNT,VarAdrToUI16(CLAMP_STATE_CURPACKMOLD));
		VarAdrSetInt(PROD_GOODCNT,VarAdrToUI16(CLAMP_STATE_VALIDMOLD));
        if((PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))//20200410
        {
            VarAdrSetInt(PROD_CLAMPLUBRIC,VarAdrToUI16(LUB1_STATE_CN_CLAMPLUBRIC));
            VarAdrSetInt(PROD_CLAMPLUBRIC2,VarAdrToUI16(LUB2_STATE_CN_CLAMPLUBRIC));
            VarAdrSetInt(PROD_CLAMPLUBRIC3,VarAdrToUI16(LUB3_STATE_CN_CLAMPLUBRIC));
        }
        else
        {
            VarAdrSetInt(PROD_CLAMPLUBRIC,VarAdrToUI16(CLAMP_STATE_CLAMPLUBRIC));
        }

		ProcessIo();				

		//auto alarm
		if(MachCfgRecv())
		{
			if((VarAdrToUI16(SYS_FL_MACH_CODE48) & 0x0100)==0)
            {
                UI16 value = VarAdrToUI16(AUTOALARM_STATE);
                if(VarAdrToUI16(SYS_STATE_FL_AUTOALARM) != value)
				{
                    VarAdrSetInt(SYS_STATE_FL_AUTOALARM,value);
                    VarSendByAdr(SYS_STATE_FL_AUTOALARM);
				}
			}
		}
		
		//servo data need to do
		if((dbsvox[1].wData[ID_E000] == 5)|| (((VarAdrToUI16(SYS_FL_MACH_CODE0)&0x0004)==0) && (VarAdrToUI16(SYS_FL_MACH_CODE54) & 0x1020))){
				dbsvox[1].wData[ID_100F] = VarAdrToUI16(SERVO_STATE_SPEED1);
				dbsvox[1].wData[ID_1004] = VarAdrToUI16(SERVO_STATE_CURRENT1);
				dbsvox[1].wData[ID_1005] = VarAdrToUI16(SERVO_STATE_POWER1);
				dbsvox[1].wData[ID_1707] = VarAdrToUI16(SERVO_STATE_TEMP1);
		}
	}
}


//20211112 dyl 开机总时间、运行总计时统计
static BOOL CheckFileValid(FILEHD filehd)
{
    if(FILEHD_IS_OK(filehd))
    {
        return true;
    }
    else
    {
        FileClose(filehd);
        return false;
    }
}

void TimeCountInit()
{
    FILEHD filehd;
    memset(&m_dbTimeCount, 0, sizeof(TIMECOUNT));
    m_dbTimeCount.flag = 0;
    filehd = FileOpen(WD_TIMECNT_PATH);
    if(CheckFileValid(filehd))
    {
        FileRead(filehd, &m_dbTimeCount, sizeof(TIMECOUNT), NULL);
        FileClose(filehd);
    }
    else {
        filehd = FileCreate(WD_TIMECNT_PATH);
        memcpy(&m_dbTimeCount,&m_dbTimeCountDefault,sizeof(TIMECOUNT));
        FileWrite(filehd,&m_dbTimeCount,sizeof(TIMECOUNT),NULL);
        FileClose(filehd);
    }
}

static void saveTimeCount(UI32 dst, void* src, UI16 wCount)
{
    FILEHD filehd;
    int writebytes;

    filehd = FileOpen(WD_TIMECNT_PATH);
    if(FILEHD_IS_OK(filehd))
    {
        FileSeek(filehd, dst);
        FileWrite(filehd, src, wCount, &writebytes);
    }
    FileClose(filehd);
}

static void calTime()
{
    //20210825 dyl 开机总时间、运行总计时统计
    UI32 time = (m_dbTimeCount.wTotalBootTime_H<<16 | m_dbTimeCount.wTotalBootTime_L)+1;
    m_dbTimeCount.wTotalBootTime_H = time>>16;
    m_dbTimeCount.wTotalBootTime_L = time&0x0ffff;

    if(OperateModeIndex() == 1 || OperateModeIndex() == 2 || OperateModeIndex() == 3)
    {
        time = (m_dbTimeCount.wTotalProdTime_H<<16 | m_dbTimeCount.wTotalProdTime_L)+1;
        m_dbTimeCount.wTotalProdTime_H = time>>16;
        m_dbTimeCount.wTotalProdTime_L = time&0x0ffff;
    }
}

void Time_Save()
{
    //20210825 dyl 开机总时间、运行总计时统计
//    UI16 wpos;
//    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalBootTime_H-(UI32)&m_dbTimeCount);
//    saveTimeCount(wpos,&m_dbTimeCount.wTotalBootTime_H,sizeof(m_dbTimeCount.wTotalBootTime_H));
//    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalBootTime_L-(UI32)&m_dbTimeCount);
//    saveTimeCount(wpos,&m_dbTimeCount.wTotalBootTime_L,sizeof(m_dbTimeCount.wTotalBootTime_L));
//    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalProdTime_H-(UI32)&m_dbTimeCount);
//    saveTimeCount(wpos,&m_dbTimeCount.wTotalProdTime_H,sizeof(m_dbTimeCount.wTotalProdTime_H));
//    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalProdTime_L-(UI32)&m_dbTimeCount);
//    saveTimeCount(wpos,&m_dbTimeCount.wTotalProdTime_L,sizeof(m_dbTimeCount.wTotalProdTime_L));

    saveTimeCount(0,&m_dbTimeCount,sizeof(TIMECOUNT));//20230111 dyl
}

//20250414 jhh 优化面板内存使用率
#ifndef WIN32
    void clearCache()
    {
        if (system("echo 1 > /proc/sys/vm/drop_caches") == 0) {
            oprintf("clear used memory\n ");
        }
    }
#endif

void UpdateTimeCount()
{
    static UI32  time_1s =0;
    static UI16  second = 0;
    static UI16  time_5m =0;
    static UI32  time_startup = 0;
    static UI32  time_1m = 0;
#ifndef WIN32
    static UI32  time_3d = 0;
#endif

    if(GetTick() -  time_1m >= 60000)//20250306 chj 面板本次开机运行时间
    {
        time_1m = GetTick();
        VarAdrSetInt(p_PP_MACHSET_PANEL_STARTUP_TM,time_startup);
        time_startup ++;
    }

    if(GetTick()-time_1s >=1000)
    {
        calTime();
        time_1s =  GetTick();
        second++;
        if(second>=60)
        {
            second =0;
            time_5m ++;
            if(time_5m>=10)//20230518 chj 5->10 减少保存次数
            {
                Time_Save();
                time_5m =0;
            }
//20250414 jhh 优化面板内存使用率
#ifndef WIN32
            time_3d ++;
            if(time_3d >= 60*24*3)//超过三天4320min
            {
                clearCache();
                time_3d = 0;
            }
#endif
        }
    }
}

void ProdSetRealTimeCount()
{
    UI32 Currenttime_Boot = 0;
    UI32 Currenttime_Prod = 0;
    Currenttime_Boot = (m_dbTimeCount.wTotalBootTime_H<<16 | m_dbTimeCount.wTotalBootTime_L);
    Currenttime_Prod = (m_dbTimeCount.wTotalProdTime_H<<16 | m_dbTimeCount.wTotalProdTime_L);

    UI16 min,hour,day,sec;//20210302 CHZ
    char RunTime[255];
    day=Currenttime_Boot/86400;
    hour=Currenttime_Boot%86400/3600;
    min=Currenttime_Boot%86400%3600/60;
    sec=Currenttime_Boot%60;
    snprintf(RunTime,sizeof(RunTime)-1,"%d%s%02d%s%02d%s%02d%s",day,GetTextTran(TEXT_PRODUCT_DAY),hour,GetTextTran(TEXT_PRODUCT_HOUR),
             min,GetTextTran(TEXT_PRODUCT_MIN),sec,GetTextTran(TEXT_PRODUCT_SEC));
    VarAdrSetStr((p_PP_TMP_TMPSTR50),RunTime);

    day=Currenttime_Prod/86400;
    hour=Currenttime_Prod%86400/3600;
    min=Currenttime_Prod%86400%3600/60;
    sec=Currenttime_Prod%60;
    snprintf(RunTime,sizeof(RunTime)-1,"%d%s%02d%s%02d%s%02d%s",day,GetTextTran(TEXT_PRODUCT_DAY),hour,GetTextTran(TEXT_PRODUCT_HOUR),
             min,GetTextTran(TEXT_PRODUCT_MIN),sec,GetTextTran(TEXT_PRODUCT_SEC));
    VarAdrSetStr((p_PP_TMP_TMPSTR52),RunTime);
}

/************************************************************************/
/* 清零开机总运行时间                                                   */
/************************************************************************/
BOOL ProdClrBootTime()
{
    m_dbTimeCount.wTotalBootTime_H = 0;
    m_dbTimeCount.wTotalBootTime_L = 0;
    UI16 wpos;
    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalBootTime_H-(UI32)&m_dbTimeCount);
    saveTimeCount(wpos,&m_dbTimeCount.wTotalBootTime_H,sizeof(m_dbTimeCount.wTotalBootTime_H));
    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalBootTime_L-(UI32)&m_dbTimeCount);
    saveTimeCount(wpos,&m_dbTimeCount.wTotalBootTime_L,sizeof(m_dbTimeCount.wTotalBootTime_L));

    return TRUE;
}
/************************************************************************/
/* 清零生产总运行时间                                                 */
/************************************************************************/
BOOL ProdClrProdTime()
{
    m_dbTimeCount.wTotalProdTime_H = 0;
    m_dbTimeCount.wTotalProdTime_L = 0;
    UI16 wpos;
    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalProdTime_H-(UI32)&m_dbTimeCount);
    saveTimeCount(wpos,&m_dbTimeCount.wTotalProdTime_H,sizeof(m_dbTimeCount.wTotalProdTime_H));
    wpos =(UI16)((UI32)&m_dbTimeCount.wTotalProdTime_L-(UI32)&m_dbTimeCount);
    saveTimeCount(wpos,&m_dbTimeCount.wTotalProdTime_L,sizeof(m_dbTimeCount.wTotalProdTime_L));
}
/***************end //20211112 dyl************/
