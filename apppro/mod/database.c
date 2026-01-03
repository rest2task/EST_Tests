#include "vardb.h"
#include "database.h"
#include "oprintf.h"
#include "command.h"
#include "protcmd.h"
#include "language.h"
#include "verinfo.h"
#include <stdio.h>
#include "prodmanage.h"
#include "time.h"
#include "panelcfg.h"
#include "file.h"

extern  LANG_ID m_currentLangId; //全局当前语言ID

//20211210 dyl 资源分配：IO输入输出点，AD电子尺，DA输出
MAINRESOURCE_RECD m_mainresource_recd;
DB_MAINRESOURCE g_mainresource_current;

MAINRESOURCE_RECD   g_dbmainresourceDefault = {
    {
    //主机型号  输入点个数  扩展板输入点个数  输出点个数  扩展板输出点个数  AD路数  AD扩展板路数  DA路数  DA扩展板路数
    {3100    ,  32,       0,              48,       0,             4,     8,           7,     0},//0
    {3101    ,  32,       0,              48,       0,             6,     8,           7,     0},//1
    {3200    ,  32,       0,              48,       0,             4,     8,           8,     0},//2
    {3201    ,  32,       0,              48,       0,             4,     8,           8,     0},//3
    {5200    ,  48,       0,              64,       0,             8,     8,           16,    0},//4
    {5201    ,  48,       0,              64,       0,             8,     8,           16,    0},//5
    {3202    ,  32,       0,              48,       0,             6,     8,           8,     0},//6
    }
};
/********* END 资源分配：IO输入输出点，AD电子尺，DA输出 *********/

/**
* @brief     :机器上电全周期相关数据初始化
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void DataBaseInit()
{
	UI32 value;

	value = VarAdrToInt(PROD_SHOTCNT);
	VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM0,(value>>16));//开模数一
	VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM1,(value&0xFFFF));//开模数二
	VarAdrSetInt(CLAMP_STATE_CURPACKMOLD,VarAdrToUI16(PROD_PKGCNT)); //本报良品数
	VarAdrSetInt(CLAMP_STATE_VALIDMOLD,VarAdrToUI16(PROD_GOODCNT));  //良品数
    if((PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))//20200410
    {
        VarAdrSetInt(LUB1_STATE_CN_CLAMPLUBRIC,VarAdrToUI16(PROD_CLAMPLUBRIC));//自动润滑模数
        VarAdrSetInt(LUB2_STATE_CN_CLAMPLUBRIC,VarAdrToUI16(PROD_CLAMPLUBRIC2));//自动润滑模数2
        VarAdrSetInt(LUB3_STATE_CN_CLAMPLUBRIC,VarAdrToUI16(PROD_CLAMPLUBRIC3));//自动润滑模数3
    }
    else
    {
        VarAdrSetInt(CLAMP_STATE_CLAMPLUBRIC,VarAdrToUI16(PROD_CLAMPLUBRIC));//自动润滑模数
    }
	VarAdrSetInt(SYS_STATE_FL_AUTOALARM,VarAdrToUI16(AUTOALARM_STATE));//自动警报状态
    //20211207 dyl 开模总数不清零
    VarAdrSetInt(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL,(VarAdrToInt(p_PP_MACHSET_SHOTCNT_TOTAL)>>16));//开模数高
    VarAdrSetInt(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL,(VarAdrToInt(p_PP_MACHSET_SHOTCNT_TOTAL)&0xFFFF));//开模数低
}

/**
* @brief     :开模数一、二，本包良品数，良品数，自动润滑模数, 发送给下位机
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void SendPartSateData()
{
	UI8 i;

	for(i=0;i<5;i++)
	{
		VarSendByAdr(CLAMP_STATE_MOLDOPNNUM0+i);
	}
    VarSendByAdr(LUB1_STATE_CN_CLAMPLUBRIC);//20200410
    VarSendByAdr(LUB2_STATE_CN_CLAMPLUBRIC);
    VarSendByAdr(LUB3_STATE_CN_CLAMPLUBRIC);

    VarSendByAdr(d_machine1_STATE_MODOPENNUM0);//20201104
    VarSendByAdr(d_machine1_STATE_MODOPENNUM1);

    VarSendByAdr(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL);//20211207 dyl 开模总数不清零
    VarSendByAdr(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL);
}

/**
* @brief     : 主动请求部件数据，可在多处被调用
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void RequestCfgData()
{
    int i;
    UI16 part_sub_id;
    UI16 part_id;
    UI16 len;
    ProtReqPartSubData(PART1_ID(MACHINE_ID),MACHSET,0,GetSubLenByID(PART1_SUB_ID(MACHINE_ID,MACHSET)));//
	ProtReqPartSubData(PART1_ID(MACHINE_ID),ACTPARA,0,GetSubLenByID(PART1_SUB_ID(MACHINE_ID,ACTPARA)));//
	ProtReqPartSubData(PART1_ID(TEMPCTRL_ID),ACTPARA,0,GetSubLenByID(PART1_SUB_ID(TEMPCTRL_ID,ACTPARA)));//
	ProtReqPartSubData(PART1_ID(METER_ID),MACHSET,0,GetSubLenByID(PART1_SUB_ID(METER_ID,MACHSET)));//
	ProtReqPartSubData(PART1_ID(DA_ID),MACHSET,0,GetSubLenByID(PART1_SUB_ID(DA_ID,MACHSET)));//
	ProtReqPartSubData(PART1_ID(DA_ID),STATE,0,GetSubLenByID(PART1_SUB_ID(DA_ID,STATE)));//
	ProtReqPartSubData(PART1_ID(HYDR_ID),ACTPARA,0,GetSubLenByID(PART1_SUB_ID(HYDR_ID,ACTPARA)));//
    //todo
    if((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))
    {
        for(i = 0; i < 8; ++i) //20191231 驱动目前最大只配8个 预留4个
        {
            part_id = (UI8)PUMP_ID<<8 | (i+1)<<4;
            part_sub_id = part_id | MACHSET;
            len = GetSubLenByID(part_sub_id);
            ProtReqPartSubData(part_id, MACHSET, 0, len);
        }
    }
}

/**
* @brief     : 当前语言类型初始化, 设置到上次关机前设的语言
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void LanguageSetInit()
{
	UI16 id;

	id = VarAdrToUI16(LANGE_SELECT);

	m_currentLangId = (LANG_ID)id;
}
/************************************************************************/
/* 画面数据保护功能                                                     */
/************************************************************************/
UI16 IsDataProtect()
{
	return VarAdrToUI16(SAFETY_DATAPROTECT);
}
/************************************************************************/
/* 画面显示保护功能                                                     */
/************************************************************************/
UI16 IsViewProtect()
{
	return VarAdrToUI16(SAFETY_VIEWPROTECT);
}
/************************************************************************/
/* 参数画面隐藏功能                                                     */
/************************************************************************/
UI16 IsParamViewHide()
{
	return VarAdrToUI16(SAFETY_PARAMHIDE);
}
/************************************************************************/
/* 密码框提示功能                                                       */
/************************************************************************/
UI16 IsPwdPrompt()
{
	return VarAdrToUI16(SAFETY_PWDPROMPT);
}
//保存手动保存数据
BOOL SaveManualSaveData(void* data)
{
    FILEHD filehd;
    filehd = FileOpen(MANUAL_SAVE_PATH);

    if(FILEHD_IS_OK(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,data,sizeof(MANUALSAVEDATA),NULL);
    }
    FileClose(filehd);
}
//加载手动保存数据
void LoadManualSaveData()
{
    FILEHD filehd;
    filehd = FileOpen(MANUAL_SAVE_PATH);

    if(FILEHD_IS_OK(filehd))
    {
        MANUALSAVEDATA data;
        FileRead(filehd,&data,sizeof(MANUALSAVEDATA),NULL);
        if(data.flag == 0xeb90)
        {
            VarAdrSetInt(PROD_SHOTCNT, data.prod_shotcnt);
            VarAdrSaveInt(PROD_SHOTCNT);
//            VarAdrSetInt(AUTOALARM_STATE, data.autoalarm_state);
//            VarAdrSaveInt(AUTOALARM_STATE);
            VarAdrSetInt(PROD_PKGCNT, data.prod_pkgcnt);
            VarAdrSaveInt(PROD_PKGCNT);
            VarAdrSetInt(PROD_CLAMPLUBRIC, data.prod_clamplubric);
            VarAdrSaveInt(PROD_CLAMPLUBRIC);
            VarAdrSetInt(PROD_CLAMPLUBRIC2, data.prod_clamplubric2);
            VarAdrSaveInt(PROD_CLAMPLUBRIC2);
            VarAdrSetInt(PROD_CLAMPLUBRIC3, data.prod_clamplubric3);
            VarAdrSaveInt(PROD_CLAMPLUBRIC3);
            VarAdrSetInt(SWCYCLEMOLDCURRENTCNT, data.cyclemold_curcnt); 
            VarAdrSaveInt(SWCYCLEMOLDCURRENTCNT);
            VarAdrSetInt(p_PP_MACHSET_SHOTCNT_TOTAL, data.prod_shotcnt_noclear);//20211207 dyl 开模总数不清零
            VarAdrSaveInt(p_PP_MACHSET_SHOTCNT_TOTAL);
            VarAdrSetInt(PROD_GOODCNT, data.prod_goodcnt);//20220923 dyl 增加良品数保存
            VarAdrSaveInt(PROD_GOODCNT);

            data.flag = 0;
            FileClose(filehd);//读文件操作完后要先关闭文件才能进行写操作
            filehd = FileOpen(MANUAL_SAVE_PATH);
            FileWrite(filehd,&data,sizeof(MANUALSAVEDATA),NULL);
        }

        FileClose(filehd);
    }
    else
    {
        filehd = FileCreate(MANUAL_SAVE_PATH);
        FileClose(filehd);
    }

}

/************************************************************************/
/* 开模数保存                                                           */
/************************************************************************/
void ShotCntSave()
{
    MANUALSAVEDATA data;
    memset(&data, 0, sizeof(MANUALSAVEDATA));

    data.prod_shotcnt = VarAdrToInt(PROD_SHOTCNT);
    data.autoalarm_state = VarAdrToInt(AUTOALARM_STATE);
    data.prod_pkgcnt = VarAdrToInt(PROD_PKGCNT);
    data.prod_clamplubric = VarAdrToInt(PROD_CLAMPLUBRIC);
    data.prod_clamplubric2 = VarAdrToInt(PROD_CLAMPLUBRIC2);
    data.prod_clamplubric3 = VarAdrToInt(PROD_CLAMPLUBRIC3);
    data.prod_goodcnt = VarAdrToInt(PROD_GOODCNT);//20220923 dyl 增加良品数保存

    //20190610 hz  真空发生器功能 兼容威力仕特殊机
    if(((GetPanelType() == PANEL_V10) || (PanelCfgStyle()==G_TYPE)) && ((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00))
    {
        data.cyclemold_curcnt = VarAdrToInt(SWCYCLEMOLDCURRENTCNT);
    }

    data.prod_shotcnt_noclear = VarAdrToInt(p_PP_MACHSET_SHOTCNT_TOTAL);//20211207 dyl 开模总数不清零

    data.flag = 0xeb90;
    SaveManualSaveData(&data);
}

/**
 * @brief      : 判断是否为G15面板
 *
 * @param      :
 * @return     : 是返回TRUE 否返回FALSE
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20191219
 */
BOOL IsG15Type()
{
    if((GetPanelType() == PANEL_VT15) && (PanelCfgStyle() == G_TYPE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******
 * * @brief    : 是否是触摸屏（G10）
 *
 * @param      :
 * @return     : 是返回TRUE 否返回FALSE
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : dyl
 * @date       : 20220104
 * *****/
BOOL IsTouchType()
{
#ifdef MPANEL_TC
    return TRUE;
#else
    return FALSE;
#endif
}

/******
 * * @brief    : 是否是带数字按键板面板
 *
 * @param      :
 * @return     : 是返回TRUE 否返回FALSE
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : dyl
 * @date       : 20220805
 * *****/
BOOL IsHaveKeyBoard()//20220805 dyl
{
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
    return FALSE;
#else
    if ((PanelCfgSize()==DISPLAY_SIZE_15))//G15 KeyBoard
        return TRUE;
    else//G10
        return TRUE;
#endif
}

/**
 * @brief      : 是否使用模温功能
 *
 * @param      :
 * @return     : 是返回TRUE 否返回FALSE
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200409
 */
BOOL isUseMoldFunc()
{
    if((PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//判断是否有actstep1与actstep2区间的动作步
BOOL JudgeActStep(UI32 actstep1, UI32 actstep2)
{
    UI32 dwAct1 = VarAdrToInt(ACT_ACTNAME1);
    UI32 dwAct2 = VarAdrToInt(ACT_ACTNAME2);
    UI32 dwAct3 = VarAdrToInt(ACT_ACTNAME3);
    UI32 dwAct4 = VarAdrToInt(ACT_ACTNAME4);

    if(actstep1 > actstep2)
    {
        UI32 temp_actstep = actstep2;
        actstep2 = actstep1;
        actstep1 = temp_actstep;
    }

    if((dwAct1>=actstep1 && dwAct1<=actstep2) || (dwAct2>=actstep1 && dwAct2<=actstep2)
            || (dwAct3>=actstep1 && dwAct3<=actstep2) || (dwAct4>=actstep1 && dwAct4<=actstep2))
    {
        return TRUE;
    }

    return FALSE;
}

//判断是否有储料动作步
BOOL JudgeChargeActStep()
{
    if(JudgeActStep(0x10120000, 0x1012FE01) || JudgeActStep(0x10160000, 0x10181000))
    {
        return TRUE;
    }

    return FALSE;
}

//判断是否有射退动作步 //20230518 chj增加射退动作判断
BOOL JudgeSuckBackActStep()
{
    if(JudgeActStep(0x10130000, 0x1013FFFF))
    {
        return TRUE;
    }

    return FALSE;
}

//判断是否有调模动作步 //20230518 chj增加调模动作判断
BOOL JudgeAdjMoldAdvActStep()
{
    if(JudgeActStep(0x24110000, 0x2411FFFF))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL JudgeAdjMoldRetActStep()
{
    if(JudgeActStep(0x24120000, 0x2412FFFF))
    {
        return TRUE;
    }

    return FALSE;
}

//20211210 dyl 资源分配：IO输入输出点，AD电子尺，DA输出
/*****************************************
*Subject  :
*Condition:
*Object   :资源分配：IO输入输出点，AD电子尺，DA输出
*Result   :
******************************************/
void ResourceUse()
{
    int i;
    for(i=0;i<NUM_MAINRESOURCE_RECD;i++)
    {
        if (m_mainresource_recd.MainResourceitems[i].nCtrlModelId==(VarAdrToUI16(d_machine1_VERSION_model_and_cat)&0x1FFF))
        {
            g_mainresource_current.nCtrlModelId = m_mainresource_recd.MainResourceitems[i].nCtrlModelId;
            g_mainresource_current.nIOInNum = m_mainresource_recd.MainResourceitems[i].nIOInNum;
            g_mainresource_current.nIOOutNum = m_mainresource_recd.MainResourceitems[i].nIOOutNum;
            g_mainresource_current.nADNum = m_mainresource_recd.MainResourceitems[i].nADNum;
            g_mainresource_current.nDANum = m_mainresource_recd.MainResourceitems[i].nDANum;
            g_mainresource_current.nADNum_Ex = m_mainresource_recd.MainResourceitems[i].nADNum_Ex;
            break;
        }
    }
}

BOOL ResourceLoad()
{
    memset(&m_mainresource_recd, 0, sizeof(MAINRESOURCE_RECD));
    memcpy(&m_mainresource_recd,&g_dbmainresourceDefault,sizeof(MAINRESOURCE_RECD));

    memset(&g_mainresource_current, 0, sizeof(DB_MAINRESOURCE));

    return TRUE;
}

int GetADMaxNumber()//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
{
    return g_mainresource_current.nADNum;
}
/********* END 资源分配：IO输入输出点，AD电子尺，DA输出 *********/

//20210914 dyl touch
BOOL  g_bScreenChecking;
BOOL IsScreenChecking()
{
    return g_bScreenChecking;
}

void SetValOfScreenCheck(BOOL bCheck)
{
   g_bScreenChecking =  bCheck;
}
