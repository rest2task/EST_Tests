#include "temp.h"
#include "oprintf.h" 
#include "part.h"   
#include "time.h"
#include "questionDlg.h"
#include "privdef.h"
#include "keydef.h"
#include "database.h"
#include "panelcfg.h"

static PER_HEATMONI_CB per_heatmoni_cb=NULL;

#define HEATING_TIME     0x5100101 //警报：预温计时未到
#define TC1_HEATING_TIME 0xe100101 //20191212警报：TC1 E502预温计时未到
#define TC2_HEATING_TIME 0xe200101 //20191212警报：TC2 E502预温计时未到

#define TEMP_OPTIMIZING      0x5100107  //温度优化中
#define TC1_TEMP_OPTIMIZING  0xe100107  //TC1  温度优化中
#define TC2_TEMP_OPTIMIZING  0xe200107  //TC2  温度优化中

//20200407HJM 模温功能
#define MT1_OPTIMIZING  0x2F100105       //模温1  温度优化中
#define MT2_OPTIMIZING  0x2F200105       //模温2  温度优化中
#define MT3_OPTIMIZING  0x2F300105       //模温3  温度优化中
#define MT4_OPTIMIZING  0x2F400105       //模温4  温度优化中
#define MT5_OPTIMIZING  0x2F500105       //模温5  温度优化中
#define MT6_OPTIMIZING  0x2F600105       //模温6  温度优化中
#define MT7_OPTIMIZING  0x2F700105       //模温7  温度优化中

#define TM_HEATERWARM               0x05130030 //预温计时
#define WSTATERSV6                  0x0516002C //保温倒计时
#define TMP_LEFTMIN                 0x33120014 //预温倒计分
#define TMP_LEFTSEC                 0x33120015 //预温倒计秒
#define TEMP_LEFT_TIME_SHOW         0x33160014 //剩余预温时间是否显示
#define OPTIMIZE_HOUR_LAST          0x3316002b //上一次温度优化小时
#define OPTIMIZE_MIN_LAST           0x3316002c //上一次温度优化分钟
#define OPTIMIZE_HOUR_CUR           0x3316002d //当前温度优化小时
#define OPTIMIZE_MIN_CUR            0x3316002e //当前温度优化分钟

extern  UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;

static QUESTION g_question;

static BOOL HaveAlarm(UI32 alarm)
{
    if(g_warm1==alarm || g_warm2==alarm || g_warm3==alarm || g_warm4==alarm || g_warm5==alarm
            || g_warm6==alarm || g_warm7==alarm || g_warm8==alarm || g_warm9==alarm || g_warm10==alarm)
    {
        return TRUE;
    }

    return FALSE;
}

/************************************************************************/
/*  获取星期数据                                                        */
/************************************************************************/
UI16 GetWeekData()
{
	UI8 nweek;
	struct tm* t;
	time_t m_tmNow;

	time(&m_tmNow);
	t = localtime(&m_tmNow);
	if(t == NULL)
		return 0;

	nweek=t->tm_wday;
	if(nweek == 7) nweek  = 0;

	return nweek;
}
/************************************************************************/
/*  温度优化时间      20190522 hz                                 */
/************************************************************************/
void TempOptimizeTm()
{
    static UI16 second = 0;
	static UI32 time_1s = 0;
    static UI32 lastOptState = 0;
    UI16 temp_value;

	if(GetTick() - time_1s >= 1000)
	{
		second ++;
		time_1s = GetTick();
	}

    if(second>=60) //1min
    {
		second = 0;
        if(VarAdrToInt(DATEMP_YOUHUAOK) == 1 || VarAdrToInt(DATEMP_YOUHUAOK) == 2
            || VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK) == 1 || VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK) == 2
            || VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK) == 1 || VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK) == 2)
        {
            VarAdrSetInt(OPTIMIZE_MIN_CUR, VarAdrToInt(OPTIMIZE_MIN_CUR) + 1);
        }
        if(VarAdrToInt(OPTIMIZE_MIN_CUR) >= 60)
        {
            VarAdrSetInt(OPTIMIZE_HOUR_CUR, VarAdrToInt(OPTIMIZE_HOUR_CUR) + 1);
            VarAdrSetInt(OPTIMIZE_MIN_CUR, 0);
        }
    }
    temp_value = VarAdrToInt(DATEMP_YOUHUAOK) + VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK) + VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK);
    if(lastOptState != temp_value)
	{
        if(VarAdrToInt(DATEMP_YOUHUAOK) == 3 || VarAdrToInt(DATEMP_YOUHUAOK) == 4
            || VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK) == 3 || VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK) == 4
            || VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK) == 3 || VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK) == 4)
        {
			VarAdrSetInt(OPTIMIZE_HOUR_LAST, VarAdrToInt(OPTIMIZE_HOUR_CUR));
			VarAdrSetInt(OPTIMIZE_MIN_LAST, VarAdrToInt(OPTIMIZE_MIN_CUR));
			VarAdrSaveInt(OPTIMIZE_HOUR_LAST);//20190605
			VarAdrSaveInt(OPTIMIZE_MIN_LAST);
			VarAdrSetInt(OPTIMIZE_HOUR_CUR, 0);
			VarAdrSetInt(OPTIMIZE_MIN_CUR, 0);
		}
        lastOptState = temp_value;
	}
}

/************************************************************************/
/*  温度优化中电热键处理      20190522 hz                                 */
/************************************************************************/
static BOOL isHeater = FALSE;  //确认后开启电热，不在弹框
static void OkButton()
{
    isHeater = TRUE;
	VarAdrSetInt(OPTIMIZE_HOUR_CUR, 0);
	VarAdrSetInt(OPTIMIZE_MIN_CUR, 0);
    SendKey(_KEY_PANEL_HEATER);
    SendKey(_KEY_NULL);
}

static void CancleButton()
{
    return;
}

BOOL TempOptimizeJudege()
{
    if(isHeater)
    {
        isHeater = FALSE;
        return FALSE;
    }
    g_question.pOkfunc = OkButton;
    g_question.pCancelfunc = CancleButton;
    ShowQuestion(VM_MSG_OPTIMIZE_EXIT, g_question);
    return TRUE;
}

/************************************************************************/
/*  是否在温度优化中          20190522 hz                                 */
/************************************************************************/
BOOL isTempOptimize()
{
    if(HaveAlarm(TEMP_OPTIMIZING) || VarAdrToInt(DATEMP_YOUHUAOK) == 1 || VarAdrToInt(DATEMP_YOUHUAOK) == 2)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/************************************************************************/
/*  是否在温度优化中          20191212 ybc                                 */
/************************************************************************/
BOOL isTempOptimizex()
{
    if(HaveAlarm(TC1_TEMP_OPTIMIZING) || HaveAlarm(TC2_TEMP_OPTIMIZING)
            || VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK) == 1 || VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_YOUHUAOK) == 2
            || VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK) == 1 || VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_YOUHUAOK) == 2)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*预温计时监控函数*/
static void PreHeatMoni()
{
	UI16 wvalue,wvalue1,wvalue2,wvalue3;
    UI16 tc1_wvalue,tc1_wvalue1,tc1_wvalue2,tc1_wvalue3;
    UI16 tc2_wvalue,tc2_wvalue1,tc2_wvalue2,tc2_wvalue3;

    //20191212 增加E502预温计时
    if(HaveAlarm(HEATING_TIME))
	{
        wvalue1 = VarAdrToUI16(TM_HEATERWARM);

        if(wvalue1 == 0)
        {
            VarAdrSetInt(TEMP_LEFT_TIME_SHOW, 0);
			return;
        }
        if(VarAdrToUI16(WSTATERSV6))
		{
			wvalue2 = VarAdrToUI16(WSTATERSV6)/60;
			wvalue3 = 60-VarAdrToUI16(WSTATERSV6)%60;
			wvalue  = wvalue1 - wvalue2-1;
            if(wvalue1 < wvalue2+1)
            {
                VarAdrSetInt(TMP_LEFTMIN,0);  //min
                VarAdrSetInt(TMP_LEFTSEC,0); //sec
            }
            else
            {
                VarAdrSetInt(TMP_LEFTMIN,wvalue);  //min
                VarAdrSetInt(TMP_LEFTSEC,wvalue3); //sec
            }
		}
        else //设定值和实际值的差值还未到范围内，还未开始倒计时
        {
            VarAdrSetInt(TMP_LEFTMIN,wvalue1);
            VarAdrSetInt(TMP_LEFTSEC,0);
        }

        if(VarAdrToInt(TMP_LEFTMIN) || VarAdrToInt(TMP_LEFTSEC))
        {
            VarAdrSetInt(TEMP_LEFT_TIME_SHOW, 1);
        }
        else
        {
            VarAdrSetInt(TEMP_LEFT_TIME_SHOW, 0);
        }
	}
    else
    {
        VarAdrSetInt(TEMP_LEFT_TIME_SHOW, 0); //20190429 hz
    }

    //E502预温计时
    if(HaveAlarm(TC1_HEATING_TIME) || HaveAlarm(TC2_HEATING_TIME))
    {
        tc1_wvalue1 = VarAdrToUI16(d_TempCtrlComm1_MOLDSET_TM_HEATERWARM);

        tc2_wvalue1 = VarAdrToUI16(d_TempCtrlComm2_MOLDSET_TM_HEATERWARM);

        if(tc1_wvalue1 ==0 && tc2_wvalue1 ==0)
        {
            VarAdrSetInt(p_PP_STATE_isRemain2TimeShow, 0);
            return;
        }

        if(VarAdrToUI16(d_TempCtrlComm1_STATE_WSTATERSV6))
        {
            tc1_wvalue2 = VarAdrToUI16(d_TempCtrlComm1_STATE_WSTATERSV6)/60;
            tc1_wvalue3 = 60-VarAdrToUI16(d_TempCtrlComm1_STATE_WSTATERSV6)%60;
            tc1_wvalue  =tc1_wvalue1 - tc1_wvalue2-1;
            if(tc1_wvalue1 < tc1_wvalue2+1)
            {
                VarAdrSetInt(p_PP_STATE_remainMin,0);  //min
                VarAdrSetInt(p_PP_STATE_remainSec,0); //sec
            }
            else
            {
                VarAdrSetInt(p_PP_STATE_remainMin,tc1_wvalue);  //min
                VarAdrSetInt(p_PP_STATE_remainSec,tc1_wvalue3); //sec
            }
        }
        else //设定值和实际值的差值还未到范围内，还未开始倒计时
        {
            VarAdrSetInt(p_PP_STATE_remainMin,tc1_wvalue1);
            VarAdrSetInt(p_PP_STATE_remainSec,0);
        }

        if(VarAdrToUI16(d_TempCtrlComm2_STATE_WSTATERSV6)) //e502 tc2
        {
            tc2_wvalue2 = VarAdrToUI16(d_TempCtrlComm2_STATE_WSTATERSV6)/60;
            tc2_wvalue3 = 60-VarAdrToUI16(d_TempCtrlComm2_STATE_WSTATERSV6)%60;
            tc2_wvalue  = tc2_wvalue1 - tc2_wvalue2-1;
            if(tc2_wvalue1 < tc2_wvalue2+1)
            {
                VarAdrSetInt(p_PP_STATE_remain2Min,0);  //min
                VarAdrSetInt(p_PP_STATE_remain2Sec,0); //sec
            }
            else
            {
                VarAdrSetInt(p_PP_STATE_remain2Min,tc2_wvalue);  //min
                VarAdrSetInt(p_PP_STATE_remain2Sec,tc2_wvalue3); //sec
            }
        }
        else //设定值和实际值的差值还未到范围内，还未开始倒计时
        {
            VarAdrSetInt(p_PP_STATE_remain2Min,tc2_wvalue1);
            VarAdrSetInt(p_PP_STATE_remain2Sec,0);
        }

        if(VarAdrToInt(p_PP_STATE_remainMin) || VarAdrToInt(p_PP_STATE_remainSec)
                || VarAdrToInt(p_PP_STATE_remain2Min) || VarAdrToInt(p_PP_STATE_remain2Sec))
        {
            UI16 tc_ab_swtich = VarAdrToUI16(p_PP_MACHSET_E502_TEMP_AB);
            if(tc_ab_swtich == 0 && (VarAdrToInt(p_PP_STATE_remainMin) || VarAdrToInt(p_PP_STATE_remainSec)))
            {
                VarAdrSetInt(p_PP_STATE_isRemain2TimeShow, 1);
            }
            else if(tc_ab_swtich ==1 && (VarAdrToInt(p_PP_STATE_remain2Min) || VarAdrToInt(p_PP_STATE_remain2Sec)))
            {
                VarAdrSetInt(p_PP_STATE_isRemain2TimeShow, 1);
            }
            else
            {
                VarAdrSetInt(p_PP_STATE_isRemain2TimeShow, 0);
            }
        }
        else
        {
            VarAdrSetInt(p_PP_STATE_isRemain2TimeShow, 0);
        }
    }
    else
    {
        VarAdrSetInt(p_PP_STATE_isRemain2TimeShow, 0);
    }
}

/************************************************************************/
/* 预温实时监控函数                                                     */
/************************************************************************/
void PreHeatMonitor()
{
	if(per_heatmoni_cb!=NULL)
		(*per_heatmoni_cb)();
}

//收到下位机发送的温度缓冲区或调模位置
//void TempBufRecvData(UI32 address, UI16 len, UI16* pdat, UI8 rx_type)
//{
//	PVAR pdata;
//
//	if(address >= DATEMP_RAMP1 && address <= DATEMP_RAMP16 || address == ADPOSI_ADJCUR)
//	{
//		if(pdata = VarAdrToVar(address))
//			VarSaveInt(pdata);
//	}
//}

void TempSetInital(PCMOD pmod)
{
	//PartDataRxCbSet(TempBufRecvData);// 设置温度缓冲回调函数
	PerHeatMoniCbSet(PreHeatMoni);
}

PER_HEATMONI_CB PerHeatMoniCbSet(PER_HEATMONI_CB perheat_cb)
{
	PER_HEATMONI_CB heat_cb;

	heat_cb = per_heatmoni_cb;
	per_heatmoni_cb = perheat_cb;

	return heat_cb;
}

BOOL isMTOptimizex()
{
    if(HaveAlarm(MT1_OPTIMIZING) ||
       VarAdrToInt(d_TempCtrlMW1_STATE_DATEMP_YOUHUAOK) == 1 ||
       VarAdrToInt(d_TempCtrlMW1_STATE_DATEMP_YOUHUAOK) == 2 ||
       HaveAlarm(MT2_OPTIMIZING) ||
       VarAdrToInt(d_TempCtrlMW2_STATE_DATEMP_YOUHUAOK) == 1 ||
       VarAdrToInt(d_TempCtrlMW2_STATE_DATEMP_YOUHUAOK) == 2 ||
       HaveAlarm(MT3_OPTIMIZING) ||
       VarAdrToInt(d_TempCtrlMW3_STATE_DATEMP_YOUHUAOK) == 1 ||
       VarAdrToInt(d_TempCtrlMW3_STATE_DATEMP_YOUHUAOK) == 2 ||
       HaveAlarm(MT4_OPTIMIZING) ||
       VarAdrToInt(d_TempCtrlMW4_STATE_DATEMP_YOUHUAOK) == 1 ||
       VarAdrToInt(d_TempCtrlMW4_STATE_DATEMP_YOUHUAOK) == 2 ||
       HaveAlarm(MT5_OPTIMIZING) ||
       VarAdrToInt(d_TempCtrlMW5_STATE_DATEMP_YOUHUAOK) == 1 ||
       VarAdrToInt(d_TempCtrlMW5_STATE_DATEMP_YOUHUAOK) == 2 ||
       HaveAlarm(MT6_OPTIMIZING) ||
       VarAdrToInt(d_TempCtrlMW6_STATE_DATEMP_YOUHUAOK) == 1 ||
       VarAdrToInt(d_TempCtrlMW6_STATE_DATEMP_YOUHUAOK) == 2 ||
       HaveAlarm(MT7_OPTIMIZING) ||
       VarAdrToInt(d_TempCtrlMW7_STATE_DATEMP_YOUHUAOK) == 1 ||
       VarAdrToInt(d_TempCtrlMW7_STATE_DATEMP_YOUHUAOK) == 2
            )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/**
* @brief     :根据当前选择模温数量，关闭多余的模温功能
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200417
*/
void MoldTempFuncClose()
{
    UI16 part, i;
    part = VarAdrToInt(MT_BOARD_COUNT);
    for(i = part; i < 7; ++i)
    {
        if(VarAdrToInt(d_TempCtrlMW1_MOLDSET_FL_USE_MW+i*0x100000) != 0)//20230518 chj 修复当模温数量设为1时，模温2，3，4，5的模温功能应变为不使用
        {
            VarSendSaveSetIntByAdr(d_TempCtrlMW1_MOLDSET_FL_USE_MW+i*0x100000, 0);
        }
    }
}

/**
* @brief     :判断模温电热状态
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :cyx
* @date      :20210319
*/
BOOL GetMtpTempHeatState()
{
    int i;
    BOOL bmtheateruse = false;//20210310.dxl
    for (i = 0; i < 7; i++)
    {
        if (1 == VarAdrToInt(d_TempCtrlMW1_STATE_FL_HEATERSTATUS_MW+i*0x100000))
        {
            bmtheateruse = true;
            break;
        }
    }
    return bmtheateruse;
}

/************************************************************************/
/*  模温温度优化中电热键处理      20200806                                   */
/************************************************************************/
static BOOL isMTHeater = FALSE;  //确认后开启电热，不在弹框
static void MoldTempOkButton()
{
    isMTHeater = TRUE;
    #ifdef PANEL_NOKEYBOARD//20220805 dyl G15:No_KeyBoard //#ifdef CAPACITIVESCREEN  //电容屏P15-5排新按键
    SendKey(_KEY_PANEL_SWX2);
    #else  //20220426
    if(PanelCfgSize()==DISPLAY_SIZE_15)//G15 KeyBoard
        SendKey(_KEY_PANEL_CURVE);
    else//G10
        SendKey(_KEY_PANEL_SW3);
    #endif
    SendKey(_KEY_NULL);
}

BOOL MoldTempOptimizeJudege()
{
    if(isMTHeater)
    {
        isMTHeater = FALSE;
        return FALSE;
    }
    g_question.pOkfunc = MoldTempOkButton;
    g_question.pCancelfunc = CancleButton;
    ShowQuestion(VM_MSG_OPTIMIZE_EXIT, g_question);
    return TRUE;
}
