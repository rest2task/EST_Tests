#include "WMain.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "privdef.h"
#include "imagewgt.h"
#include <math.h>
#include "editwgt.h"
#include "subpagewgt.h"
#include "keymod.h"
#include "key.h"
#include "texttbl.h"
#include "buttonwgt.h"
#include "tabpagewgt.h"
#include "kjump.h"
#include "ioredef.h"
#include "tablectl.h"
#include "usermod.h"
#include "stdio.h"
#include "oprintf.h"
#include "prodpower.h"
#include "time.h"
#include "labelwgt.h"
#include "dbenum.h"
#include "keyprot.h"

DEFINE_MAIN_MAP

CONNECT_MAIN_EVENT

#define ACT_FILTER_COUNTER    2   //动作滤波周期
#define A_PAGE_SEL            TMPVAL(472)  //A温度切换
#define B_PAGE_SEL            TMPVAL(473)  //B温度切换
#define TMP_REAL1            TMPSTR(110)   //温度实际值1显示
#define TMP_REAL2            TMPSTR(120)   //温度实际值1显示
#define TMP_OIL			     TMPSTR(130)   //油温显示
#define TMP_MOTOR			 TMPSTR(131)   //电机温度显示
#define TMP_MOUTH            TMPSTR(132)   //落料口温度

//20241028 chj 威力士主画面显示锁模力
#define TMP_LOCKPRS          TMPVAL(150)   //锁模力

#define ACTIVE_LABLE TMPVAL(475) //动作标签是否激活

/**********Act-TimeLine-Start*********************************************************************************/
#define CLOSE_END_TM  TMPVAL(476)
#define NOZADV_END_TM  TMPVAL(477)
#define INJECT_END_TM  TMPVAL(478)
#define HOLD_END_TM  TMPVAL(479)
#define NOZRET_END_TM  TMPVAL(480)
#define CHARGE_END_TM  TMPVAL(481)
#define SUCK_END_TM  TMPVAL(482)
#define COOL_END_TM  TMPVAL(483)
#define OPEN_END_TM  TMPVAL(484)
#define EJECT_END_TM  TMPVAL(485)

#define INJECT_START_TM     TMPVAL(498)
#define HOLD_START_TM       TMPVAL(499)

#define COOL_RUN_TM   TMPVAL(486)

#define TIME_LINE_Y0  TMPVAL(487)
#define TIME_LINE_Y1  TMPVAL(488)
#define TIME_LINE_Y2  TMPVAL(489)
#define TIME_LINE_Y3  TMPVAL(490)
#define TIME_LINE_Y4  TMPVAL(491)

#define CURRENT_TIME  TMPVAL(492)

#define TIME_LINE_MAX  TMPVAL(493)

#define ACTIVE_IMG    TMPVAL(460)


static const char* ActCurrentImg[] = {
    "STclosey.png",
    "STnozzleadvy.png",
    "STinjecty.png",
    "STholdy.png",
    "STnozzlerety.png",
    "STchargey.png",
    "STsucky.png",
    "STcooly.png",
    "STopeny.png",
    "STejecty.png",

    "STcorey.png",
    "STairy.png"
};

/**********MachineActStart*******************************************************************/
enum        ITEMID_ACTION {
    ACT_OPEN_BEGIN  = 0x201310,
    ACT_OPEN_1      = 0x201310,
    ACT_OPEN_END    = 0x2013FE,

    ACT_CLOSE_BEGIN = 0x201100,
    ACT_CLOSE_1     = 0x201110,
    ACT_CLOSE_END   = 0x2012FE,

    ACT_INJECT_BEGIN= 0x101100,
    ACT_INJECT_1    = 0x101110,
    ACT_INJECT_END  = 0x101119,

    ACT_CHARGE_BEGIN= 0x101200,
    ACT_CHARGE_END  = 0x1012FE,

    ACT_HOLD_BEGIN  = 0x10111C,
    ACT_HOLD_END    = 0x101125,

    ACT_SKBACK_BEGIN= 0x101300,
    ACT_SKBACK_END  = 0x1013FE,

    ACT_NOZADV_BEGIN= 0x111100,
    ACT_NOZADV_END  = 0x1111FE,

    ACT_NOZRET_BEGIN= 0x111200,
    ACT_NOZRET_END  = 0x1112FE,

    ACT_LEJECTADV_BEGIN= 0x211200,
    ACT_LEJECTADV_END  = 0x2112FE,
    ACT_LEJECTRET_BEGIN= 0x211300,
    ACT_LEJECTRET_END  = 0x2113FE,

    ACT_REJECTADV_BEGIN= 0x212200,
    ACT_REJECTADV_END  = 0x212211,
    ACT_REJECTRET_BEGIN= 0x212300,
    ACT_REJECTRET_END  = 0x212312,

    ACT_CORE_BEGIN  = 0x221100,
    ACT_CORE_END    = 0x226410,
    ACT_AIR_BEGIN  = 0x231110,
    ACT_AIR_END    = 0x23E200,
    ACT_COOL_BEGIN  = 0x111500,
    ACT_COOL_END    = 0x111510 //0x111610

};
//动作表 对应TimeLine
enum ActProcEnum {
    ACTPROC_CLOSE,
    ACTPROC_NOZZLEADV,
    ACTPROC_INJECT,
    ACTPROC_HOLD,
    ACTPROC_NOZZLERET,
    ACTPROC_CHARGE,
    ACTPROC_SUCK,
    ACTPROC_COOL,
    ACTPROC_OPEN,
    ACTPROC_EJECT,

    ACTPROC_CORE,
    ACTPROC_AIR
};

/***********I/O-Start**********************************************************************/


#define TMP_NAME0		TMPSTR(51)
#define TMP_COLOR0		TMPVAL(300)
#define SN_START		0
#define TMP_CHECK0		TMPVAL(51)//20190514.cyx
#define MASK_BIT0       0x01
#define CHECKBOXIN2_NAME(NUM,pbuff,size)		joinStrAndNum("tabin_in2_cb",NUM,pbuff,size)

#define SN_COUNT	48
#define SN_IV5000_ADD_COUNT	8
#define SN_IV5200_ADD_COUNT	16
#define TMP_NAME49		TMPSTR(10)
#define TMP_CHECK49		TMPVAL(110)
#define MASK_BIT0       0x01

static UI16 s_wIONum = 2;
static void IO_InputShow()
{
    PWGT pwgt;
    UI16 i;
    char buff[80];

    if(IV5000 || IV5200 || IV5300)
    {
        s_wIONum = 3;
    }
    else
    {
        s_wIONum = 2;
    }

    if(s_wIONum == 2)
    {
        for (i = 33; i <= 48; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOXIN2_NAME(i,buff,100));
            WGTSetVisible(pwgt, FALSE);
        }
    }
    else if(s_wIONum == 3)
    {
        for (i = 33; i <= 48; i++)
        {
            pwgt = PageFramCurWgtByName(CHECKBOXIN2_NAME(i,buff,100));
            WGTSetVisible(pwgt, TRUE);
        }
    }
}

static void Input__UpdateNames(PPROT_TBL_HD tbl_hd)
{
    int i,j;
    char strid[255];
    UI16 checkstate;
    TBL_INPUT* pin;

    for(i = 0; i<s_wIONum; i++)
    {
        for(j = 0; j < 16; ++j)
        {
            if(tbl_hd && (pin = TableFindInput(tbl_hd, 16*i+j+SN_START+1)))
            {
                UI32 logic_addr = (((UI32)pin->part_id+1) << 16) | pin->ind;

                if(pmap_input != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d  %s",16*i+j+1,GetMapTran(pmap_input,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d  ",16*i+j+1);
                }
                if(GetIPoint(16*i+j))//反逻辑
                {
                    strcat(strid,"(");
                    strcat(strid,GetTextTran(TEXT_ANTI_LOGIC));
                    strcat(strid,")");
                }

                VarAdrSetStr(TMP_NAME0+16*i+j, strid);

                if(GetUseForceRec(16*i+j))//输入点逻辑判断，强制激活设置或物理状态
                {
                    VarAdrSetInt(TMP_CHECK0+i*16+j,!GetForceInputRec(16*i+j));
                    VarAdrSetInt(TMP_COLOR0+16*i+j,GUI_RED);
                }
                else
                {
                    checkstate = VarAdrToInt(PI_INPUT1+i)>>j & MASK_BIT0;
                    VarAdrSetInt(TMP_CHECK0+i*16+j,checkstate);
                    VarAdrSetInt(TMP_COLOR0+16*i+j,GUI_BLACK);
                }

            }
            else
            {
                VarAdrSetInt(TMP_CHECK0+16*i+j,0);//20190517.cyx
                VarAdrSetInt(TMP_COLOR0+16*i+j,GUI_BLACK);//20190517.cyx
                snprintf(strid,sizeof(strid)-1,"%d  ",16*i+j+1);
                VarAdrSetStr(TMP_NAME0+16*i+j, strid);
            }

        }
    }
}


static void Output__UpdateNames(PPROT_TBL_HD tbl_hd)
{
    int i;
    char strid[255];
    UI16 checkstate;
    TBL_OUTPUT* pout;
    PWGT pwgt = NULL;
    for(i = 0; i < SN_COUNT; ++i)
    {
        if(tbl_hd && (pout = TableFindOutput(tbl_hd, i+SN_START+1)))
        {
            UI32 logic_addr = ((UI32)pout->part_id << 16) | pout->ind;

            if(pmap_output != NULL)
            {
                snprintf(strid,sizeof(strid)-1,"%d  %s",i+1,GetMapTran(pmap_output,logic_addr));
            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d  ",i+1);
            }

            VarAdrSetStr(TMP_NAME0+i, strid);

        }
        else
        {
            snprintf(strid,sizeof(strid)-1,"%d  ",i+1);
            VarAdrSetStr(TMP_NAME0+i, strid);
        }

        checkstate = VarAdrToInt(PO_OUTPUT1+(i/16))>>(i%16) & MASK_BIT0;
        VarAdrSetInt(TMP_CHECK0+i,checkstate);
    }

    if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5000)
    {
        for(i = 0; i < SN_IV5000_ADD_COUNT; ++i)
        {
            if(tbl_hd && (pout = TableFindOutput(tbl_hd, i+SN_START+1+SN_COUNT)))
            {
                UI32 logic_addr = ((UI32)pout->part_id << 16) | pout->ind;

                if(pmap_output != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d  %s",i+49,GetMapTran(pmap_output,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d  ",i+49);
                }

                VarAdrSetStr(TMP_NAME49+i, strid);

            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d  ",i+49);
                VarAdrSetStr(TMP_NAME49+i, strid);
            }
            checkstate = VarAdrToInt(PO_OUTPUT1+3)>>(i%16) & MASK_BIT0;
            VarAdrSetInt(TMP_CHECK49+i,checkstate);
        }
        TabPageSetSubVisble(main_pg.tabout, 2, TRUE);
        ImageSetPicName(main_pg.tabout_out2_image1, "right.png");

        for(i = 0; i < SN_IV5200_ADD_COUNT-SN_IV5000_ADD_COUNT; i++)
        {
            pwgt = PageFramCurWgtByIndex(i+157);
            WGTSetVisible(pwgt,FALSE);
        }
    }
    else if(IV5200 || IV5300)
    {
        for(i = 0; i < SN_IV5200_ADD_COUNT; ++i)
        {
            if(tbl_hd && (pout = TableFindOutput(tbl_hd, i+SN_START+1+SN_COUNT)))
            {
                UI32 logic_addr = ((UI32)pout->part_id << 16) | pout->ind;

                if(pmap_output != NULL)
                {
                    snprintf(strid,sizeof(strid)-1,"%d  %s",i+49,GetMapTran(pmap_output,logic_addr));
                }
                else
                {
                    snprintf(strid,sizeof(strid)-1,"%d  ",i+49);
                }

                VarAdrSetStr(TMP_NAME49+i, strid);

            }
            else
            {
                snprintf(strid,sizeof(strid)-1,"%d  ",i+49);
                VarAdrSetStr(TMP_NAME49+i, strid);
            }
            checkstate = VarAdrToInt(PO_OUTPUT1+3)>>(i%16) & MASK_BIT0;
            VarAdrSetInt(TMP_CHECK49+i,checkstate);
        }
        TabPageSetSubVisble(main_pg.tabout, 2, TRUE);
        ImageSetPicName(main_pg.tabout_out2_image1, "right.png");

        for(i = 0; i < SN_IV5200_ADD_COUNT-SN_IV5000_ADD_COUNT; i++)
        {
            pwgt = PageFramCurWgtByIndex(i+157);
            WGTSetVisible(pwgt,TRUE);
        }
    }
    else
    {
        TabPageSetSubVisble(main_pg.tabout, 2, FALSE);
        ImageSetPicName(main_pg.tabout_out2_image1, "right1.png");
    }
}




/***********I/O-End**********************************************************************/



/**********MachineActStart*******************************************************************/

static UI16 m_currentAction;
static UI16 scanCurrentAct()
{
    UI32 dwAct1 = VarAdrToInt(ACT_ACTNAME1) >> 8;
    UI32 dwAct2 = VarAdrToInt(ACT_ACTNAME2) >> 8;
    UI32 dwAct3 = VarAdrToInt(ACT_ACTNAME3) >> 8;
    UI32 dwAct4 = VarAdrToInt(ACT_ACTNAME4) >> 8;
    UI16 currentAction = 0;
    UI8 ActFilters[12]={3,3,3,3,3,3,3,3,3,3,3,3};//动作切换滤波，防止闪烁

    //开模
    if((dwAct1 >= ACT_OPEN_BEGIN && dwAct1 <= ACT_OPEN_END)
            ||(dwAct2 >= ACT_OPEN_BEGIN && dwAct2 <= ACT_OPEN_END)
            ||(dwAct3 >= ACT_OPEN_BEGIN && dwAct3 <= ACT_OPEN_END)
            ||(dwAct4 >= ACT_OPEN_BEGIN && dwAct4 <= ACT_OPEN_END)){
        currentAction |= (1 << ACTPROC_OPEN);
        ActFilters[ACTPROC_OPEN] = ACT_FILTER_COUNTER;
    }

    //关模
    if((dwAct1 >= ACT_CLOSE_BEGIN && dwAct1 <= ACT_CLOSE_END)
            ||(dwAct2 >= ACT_CLOSE_BEGIN && dwAct2 <= ACT_CLOSE_END)
            ||(dwAct3 >= ACT_CLOSE_BEGIN && dwAct3 <= ACT_CLOSE_END)
            ||(dwAct4 >= ACT_CLOSE_BEGIN && dwAct4 <= ACT_CLOSE_END)){
        currentAction |= (1 << ACTPROC_CLOSE);
        ActFilters[ACTPROC_CLOSE] = ACT_FILTER_COUNTER;
    }

    //射出
    if((dwAct1 >= ACT_INJECT_BEGIN && dwAct1 <= ACT_INJECT_END)
            ||(dwAct2 >= ACT_INJECT_BEGIN && dwAct2 <= ACT_INJECT_END)
            ||(dwAct3 >= ACT_INJECT_BEGIN && dwAct3 <= ACT_INJECT_END)
            ||(dwAct4 >= ACT_INJECT_BEGIN && dwAct4 <= ACT_INJECT_END)){
        currentAction |= (1 << ACTPROC_INJECT);
        ActFilters[ACTPROC_INJECT] = ACT_FILTER_COUNTER;
    }

    //储料
    if(((dwAct1 >= ACT_CHARGE_BEGIN && dwAct1 <= ACT_CHARGE_END) || (dwAct1 >= 0x101600 && dwAct1 <= 0x101810))
            ||((dwAct2 >= ACT_CHARGE_BEGIN && dwAct2 <= ACT_CHARGE_END) || (dwAct2 >= 0x101600 && dwAct2 <= 0x101810))
            ||((dwAct3 >= ACT_CHARGE_BEGIN && dwAct3 <= ACT_CHARGE_END) || (dwAct3 >= 0x101600 && dwAct3 <= 0x101810))
            ||((dwAct4 >= ACT_CHARGE_BEGIN && dwAct4 <= ACT_CHARGE_END) || (dwAct4 >= 0x101600 && dwAct4 <= 0x101810))){
        currentAction |= (1 << ACTPROC_CHARGE);
        ActFilters[ACTPROC_CHARGE] = ACT_FILTER_COUNTER;
    }

    //保压
    if((dwAct1 >= ACT_HOLD_BEGIN && dwAct1 <= ACT_HOLD_END)
            ||(dwAct2 >= ACT_HOLD_BEGIN && dwAct2 <= ACT_HOLD_END)
            ||(dwAct3 >= ACT_HOLD_BEGIN && dwAct3 <= ACT_HOLD_END)
            ||(dwAct4 >= ACT_HOLD_BEGIN && dwAct4 <= ACT_HOLD_END)){
        currentAction |= (1 << ACTPROC_HOLD);
        ActFilters[ACTPROC_HOLD] = ACT_FILTER_COUNTER;
    }

    //射退
    if((dwAct1 >= ACT_SKBACK_BEGIN && dwAct1 <= ACT_SKBACK_END)
            ||(dwAct2 >= ACT_SKBACK_BEGIN && dwAct2 <= ACT_SKBACK_END)
            ||(dwAct3 >= ACT_SKBACK_BEGIN && dwAct3 <= ACT_SKBACK_END)
            ||(dwAct4 >= ACT_SKBACK_BEGIN && dwAct4 <= ACT_SKBACK_END)){
        currentAction |= (1 << ACTPROC_SUCK);
        ActFilters[ACTPROC_SUCK] = ACT_FILTER_COUNTER;
    }

    //座进
    if((dwAct1 >= ACT_NOZADV_BEGIN && dwAct1 <= ACT_NOZADV_END)
            ||(dwAct2 >= ACT_NOZADV_BEGIN && dwAct2 <= ACT_NOZADV_END)
            ||(dwAct3 >= ACT_NOZADV_BEGIN && dwAct3 <= ACT_NOZADV_END)
            ||(dwAct4 >= ACT_NOZADV_BEGIN && dwAct4 <= ACT_NOZADV_END)){
        currentAction |= (1 << ACTPROC_NOZZLEADV);
        ActFilters[ACTPROC_NOZZLEADV] = ACT_FILTER_COUNTER;
    }

    //座退
    if((dwAct1 >= ACT_NOZRET_BEGIN && dwAct1 <= ACT_NOZRET_END)
            ||(dwAct2 >= ACT_NOZRET_BEGIN && dwAct2 <= ACT_NOZRET_END)
            ||(dwAct3 >= ACT_NOZRET_BEGIN && dwAct3 <= ACT_NOZRET_END)
            ||(dwAct4 >= ACT_NOZRET_BEGIN && dwAct4 <= ACT_NOZRET_END)){
        currentAction |= (1 << ACTPROC_NOZZLERET);
        ActFilters[ACTPROC_NOZZLERET] = ACT_FILTER_COUNTER;
    }

    //托模
    if(((dwAct1 >= ACT_LEJECTADV_BEGIN && dwAct1 <= ACT_LEJECTADV_END) ||(dwAct1 >= ACT_LEJECTRET_BEGIN && dwAct1 <= ACT_LEJECTRET_END)
        || (dwAct1 >= ACT_REJECTADV_BEGIN && dwAct1 <= ACT_REJECTADV_END) ||(dwAct1 >= ACT_REJECTRET_BEGIN && dwAct1 <= ACT_REJECTRET_END))
            ||((dwAct2 >= ACT_LEJECTADV_BEGIN && dwAct2 <= ACT_LEJECTADV_END) ||(dwAct2 >= ACT_LEJECTRET_BEGIN && dwAct2 <= ACT_LEJECTRET_END)
               || (dwAct2 >= ACT_REJECTADV_BEGIN && dwAct2 <= ACT_REJECTADV_END) ||(dwAct2 >= ACT_REJECTRET_BEGIN && dwAct2 <= ACT_REJECTRET_END))
            ||((dwAct3 >= ACT_LEJECTADV_BEGIN && dwAct3 <= ACT_LEJECTADV_END) ||(dwAct3 >= ACT_LEJECTRET_BEGIN && dwAct3 <= ACT_LEJECTRET_END)
               || (dwAct3 >= ACT_REJECTADV_BEGIN && dwAct3 <= ACT_REJECTADV_END) ||(dwAct3 >= ACT_REJECTRET_BEGIN && dwAct3 <= ACT_REJECTRET_END))
            ||((dwAct4 >= ACT_LEJECTADV_BEGIN && dwAct4 <= ACT_LEJECTADV_END) ||(dwAct4 >= ACT_LEJECTRET_BEGIN && dwAct4 <= ACT_LEJECTRET_END)
               || (dwAct4 >= ACT_REJECTADV_BEGIN && dwAct4 <= ACT_REJECTADV_END) ||(dwAct4 >= ACT_REJECTRET_BEGIN && dwAct4 <= ACT_REJECTRET_END)))
    {
        currentAction |= (1 << ACTPROC_EJECT);
        ActFilters[ACTPROC_EJECT] = ACT_FILTER_COUNTER;
    }

    //冷却
    if((dwAct1 >= ACT_COOL_BEGIN && dwAct1 <= ACT_COOL_END)
            ||(dwAct2 >= ACT_COOL_BEGIN && dwAct2 <= ACT_COOL_END)
            ||(dwAct3 >= ACT_COOL_BEGIN && dwAct3 <= ACT_COOL_END)
            ||(dwAct4 >= ACT_COOL_BEGIN && dwAct4 <= ACT_COOL_END)){
        currentAction |= (1 << ACTPROC_COOL);
        ActFilters[ACTPROC_COOL] = ACT_FILTER_COUNTER;
    }

    //中子
    if((dwAct1 >= ACT_CORE_BEGIN && dwAct1 <= ACT_CORE_END)
            ||(dwAct2 >= ACT_CORE_BEGIN && dwAct2 <= ACT_CORE_END)
            ||(dwAct3 >= ACT_CORE_BEGIN && dwAct3 <= ACT_CORE_END)
            ||(dwAct4 >= ACT_CORE_BEGIN && dwAct4 <= ACT_CORE_END)){
        currentAction |= (1 << ACTPROC_CORE);
        ActFilters[ACTPROC_CORE] = ACT_FILTER_COUNTER;
    }

    //吹气
    if((dwAct1 >= ACT_AIR_BEGIN && dwAct1 <= ACT_AIR_END)
            ||(dwAct2 >= ACT_AIR_BEGIN && dwAct2 <= ACT_AIR_END)
            ||(dwAct3 >= ACT_AIR_BEGIN && dwAct3 <= ACT_AIR_END)
            ||(dwAct4 >= ACT_AIR_BEGIN && dwAct4 <= ACT_AIR_END)){
        currentAction |= (1 << ACTPROC_AIR);
        ActFilters[ACTPROC_AIR] = ACT_FILTER_COUNTER;
    }

    return currentAction;
}

//动作循环显示
static void ActCyclDisply()
{
    UI16 currentAct = scanCurrentAct();
    int i;
    UI32 bit = 0;
    static UI32 last_bit = 0;
    if(last_bit != VarAdrToInt(ACTIVE_LABLE))
    {
        VarAdrSetInt(ACTIVE_LABLE, bit);
    }
    if(currentAct != m_currentAction)
    {
        m_currentAction = currentAct;

        if(m_currentAction == 0)
        {
            VarAdrSetInt(ACTIVE_LABLE, 0);
//            WGTSetVisible(main_pg.subCurAct, FALSE);
            return;
        }
//        WGTSetVisible(main_pg.subCurAct, TRUE);

        //Time line 是否激活
        for(i = 0; i < 10; ++i)
        {
            if(m_currentAction & (1 << i))
            {
                VarAdrSetInt(ACTIVE_LABLE, setone(bit, i));
                last_bit = VarAdrToInt(ACTIVE_LABLE);
            }
        }
    }
}

/**
 * @brief      : 座台位置切换
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200325
 */
static UI16 SetNozzlePosValueAndShow() //20200325
{
    UI16 wNozzleValue = VarAdrToInt(d_nozzle1_MACHSET_NZL_METERCH);
    UI16 wValue = 0;

//    if (IV5200 || IV5300)
    if (g_mainresource_current.nADNum == 8)//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
    {
        if((wNozzleValue>=4)&&(wNozzleValue<=7))  //4-7路做座台切换
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_4+(wNozzleValue-4));
        }
    }
//    else if (IV3100 && ((VarAdrToUI16(d_machine1_VERSION_model_and_cat)&0x1FFF)==3101))//20220120 dyl 电子尺通道统一处理
//    {
//        if(wNozzleValue == 7)
//        {
//            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_EXT1);
//        }
//        else if((wNozzleValue>=4)&&(wNozzleValue<=6))
//        {
//            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_4+(wNozzleValue-4));
//        }
//    }
    else if (g_mainresource_current.nADNum == 6)//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择
    {
        if(wNozzleValue == 7)
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_EXT1);
        }
        else if((wNozzleValue>=4)&&(wNozzleValue<=6))
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_4+(wNozzleValue-4));
        }
    }
    else
    {
        if(wNozzleValue == 4)
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_4);
        }
        else if((wNozzleValue>=5)&&(wNozzleValue<=7))  //4-7路做座台切换
        {
            wValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_EXT1+(wNozzleValue-5));
        }
    }

    return wValue;
}

/*
* @brief     :机器状态图片刷新
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191204
*/
static void Machineshow()
{
    static UI16 s_wClampMove = 50;
    static UI16 s_wInject1Move = 80;
    static UI16 s_wEjectMove = 20;
    static UI16 s_wEjectMoveMax = 0;
    static UI16 s_wNozelMove = 20;
    static int s_nwide;
    static BOOL bOnce = FALSE;
    static UI16 s_wFill1start=0;
    static UI16 s_wFill1Move=0;
    static UI16 s_wFill2Move=0;
    UI32 wMeterValue,wMeterMax,wMeterEndPos;
    int ClampAngle=50;
    int pixleft_x=73;
    int pixleft2_x=95;
    int pixright_x=151;
    int pixopen1_x=215;
    int pixopen2_x=186;
    int pixeject_x=196;
    int pixleftpie_x=34;
    int pixrightpie_x=180;
    int pixcrosspie_x=104;
    int travel_inje=60;
    int travel_eject=10;
    int travel_nozl=16;

    int pixnozl_x=342+travel_nozl;//pixMachineNozPic->x();//342
    int pixpattle_x=326+travel_nozl;//pixMachinePattlePic->x();//326
    int pixinje_x=342+travel_nozl;//pixMachineInject2Pic->x();//342

    int fill1_Rx=308;//填充1右边横坐标=填充1的横坐标+填充1的宽度
    int travel_fill1=42;//填充1宽度
    int fill2_Ry=179;//填充2起始纵坐标=填充1的纵坐标
    int travel_fill2=26;//填充23的动作行程
    int product_Rx=258;//产品横坐标
    int bNozl=0;
    int max_press = 0;
    int pressad_ch = 4;

    //开合模动作
    wMeterValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_2);//电子尺实时位置
    wMeterMax = VarAdrToInt(d_clamp1_ACTPARA_ADPOSI_OPENENDEND);//开模终止位置
    if(wMeterMax==0)
    {
        wMeterMax = ClampAngle;
    }
    if(wMeterValue > wMeterMax)
    {
        wMeterValue = wMeterMax;
    }
    s_wClampMove = ClampAngle * wMeterValue / wMeterMax;
    s_nwide = (pixright_x - pixleft_x)/2 ;
    //上轴杆联动
    //WI32(main_pg.subMach_pixleftup->x_pos,pixleft_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)));
    ImageSetRotateDeg(main_pg.subMach_pixleftup,s_wClampMove);
    WI32(main_pg.subMach_pixrightup->x_pos,pixright_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)));
    ImageSetRotateDeg(main_pg.subMach_pixrightup,360-s_wClampMove);

    //下轴杆联动
//    WI32(main_pg.subMach_pixleftdown->x_pos,pixleft_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)));
    ImageSetRotateDeg(main_pg.subMach_pixleftdown ,360 - (s_wClampMove));
    WI32(main_pg.subMach_pixrightdown->x_pos,pixright_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)));
    ImageSetRotateDeg(main_pg.subMach_pixrightdown,s_wClampMove);

    //内轴杆联动
    ImageSetRotateDeg(main_pg.subMach_pixleftup2,s_wClampMove*0.8);//1.5为模糊补偿数
    ImageSetRotateDeg(main_pg.subMach_pixleftdown2,360-s_wClampMove*0.8);//1.5为模糊补偿数

    //动模板移动
    WI32(main_pg.subMach_pixMachineOpen1Pic->x_pos,pixopen1_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)));
    WI32(main_pg.subMach_pixMachineOpen2Pic->x_pos,pixopen2_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)));
    WI32(main_pg.subMach_pixMachineRightPiePic->x_pos,pixrightpie_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)));

    //十字头移动
    WI32(main_pg.subMach_pixMachineLeftPiePic->x_pos,pixleftpie_x-s_wClampMove*0.5+5);//0.5 5为模糊补偿数
    WI32(main_pg.subMach_pixMachineClpPic3->x_pos,pixcrosspie_x-s_wClampMove*0.5+5);
    WI32(main_pg.subMach_pixleftup2->x_pos,pixleft2_x-s_wClampMove*0.5+5);//0.5 5为模糊补偿数
    WI32(main_pg.subMach_pixleftdown2->x_pos,pixleft2_x-s_wClampMove*0.5+5);//0.5 5为模糊补偿数

    if(VarAdrToInt(d_machine1_MACHSET_FL_SERVOMODE) & 0x0008) //坐台走位置//电子尺控制+4000改为code16+8 20250310 jhh
    {
        wMeterValue = SetNozzlePosValueAndShow();//电子尺实时位置 //20200325
        wMeterMax = VarAdrToInt(d_nozzle1_ACTPARA_ADPOSI_NOZADVEND);
        if(wMeterMax==0) wMeterMax=travel_nozl;
        if((wMeterValue > wMeterMax) || (((wMeterMax-wMeterValue)<10) && (wMeterValue> 10))) wMeterValue=wMeterMax;//1.0mm防震动
        s_wNozelMove=travel_nozl * wMeterValue/wMeterMax;
    }
    else if(VarAdrToInt(d_machine1_MACHSET_FL_SERVOMODE) & 0x0001)  //座台走时间//座台走时间code3+4改为code16+1 20250310 jhh
    {
        if(m_currentAction & (1 << ACTPROC_NOZZLEADV))
        {
            bNozl = 1;
            wMeterValue = VarAdrToInt(d_nozzle1_STATE_TM_NOZZLEF_RT);//动作实时计时
            wMeterMax = VarAdrToInt(d_nozzle1_ACTPARA_TM_NOZADVACT)+VarAdrToInt(d_nozzle1_MACHSET_TM_NOZADVSLOW);//设定时间
            if(wMeterMax==0) wMeterMax=travel_nozl;
            if(wMeterValue > wMeterMax) wMeterValue=wMeterMax;
            s_wNozelMove=travel_nozl * wMeterValue/wMeterMax;
        }
        else if(m_currentAction & (1 << ACTPROC_NOZZLERET))
        {
            bNozl =2;
            wMeterValue = VarAdrToInt(d_nozzle1_STATE_TM_NOZZLEB_RT);//动作实时计时
            wMeterMax = VarAdrToInt(d_nozzle1_ACTPARA_TM_NOZRETACT);//设定时间
            if(wMeterMax==0) wMeterMax=travel_nozl;
            if(wMeterValue > wMeterMax) wMeterValue=wMeterMax;
            s_wNozelMove=travel_nozl * wMeterValue/wMeterMax;
        }
        else
            bNozl = 0;
    }
    else
    {
        s_wNozelMove = travel_nozl;
    }
    WI32(main_pg.subMach_pixMachineNozPic->x_pos, pixnozl_x-s_wNozelMove);
    WI32(main_pg.subMach_pixMachinePattlePic->x_pos, pixpattle_x-s_wNozelMove);


    //螺杆动作  终止位置判断
    switch (VarAdrToInt(d_inject1_MOLDSET_SEC_INJ)) {
    case 6:
        wMeterEndPos = VarAdrToInt(d_inject1_ACTPARA_ADPOSI_INJ6END);
        break;
    case 5:
        wMeterEndPos = VarAdrToInt(d_inject1_ACTPARA_ADPOSI_INJ5END);
        break;
    case 4:
        wMeterEndPos = VarAdrToInt(d_inject1_ACTPARA_ADPOSI_INJ4END);
        break;
    case 3:
        wMeterEndPos = VarAdrToInt(d_inject1_ACTPARA_ADPOSI_INJ3END);
        break;
    case 2:
        wMeterEndPos = VarAdrToInt(d_inject1_ACTPARA_ADPOSI_INJ2END);
        break;
    case 1:
        wMeterEndPos = VarAdrToInt(d_inject1_ACTPARA_ADPOSI_INJ1END);
        break;
    default:
        break;
    }
    if(VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_1) > wMeterEndPos)
        wMeterValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_1) - wMeterEndPos;
    else
        wMeterValue = 0;
    wMeterMax = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_1) - wMeterEndPos;//射出终止位置
    if(wMeterMax<=0) wMeterMax=travel_inje;
    if(wMeterValue > wMeterMax) wMeterValue=0;
    s_wInject1Move=travel_inje * wMeterValue/wMeterMax;

    //射出
    if(VarAdrToInt(SYS_FL_METER) & 0x0004)
        WI32(main_pg.subMach_pixMachineInject2Pic->x_pos, pixinje_x-s_wNozelMove+s_wInject1Move);
    else if(VarAdrToInt(SYS_FL_MACH_CODE3) & 0x4000)
        WI32(main_pg.subMach_pixMachineInject2Pic->x_pos, pixinje_x-(pixnozl_x-RU32(main_pg.subMach_pixMachineNozPic->x_pos))+s_wInject1Move);
    else
        WI32(main_pg.subMach_pixMachineInject2Pic->x_pos, pixinje_x-s_wNozelMove+s_wInject1Move);


    //托模动作
    wMeterValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_3);
    wMeterMax = VarAdrToInt(d_eject1_ACTPARA_ADPOSI_EJTADVFASTEND);
    if(wMeterMax==0) wMeterMax=travel_eject;
    if((wMeterValue > wMeterMax) || (((wMeterMax - wMeterValue)<10)&&(wMeterValue>10))) wMeterValue=wMeterMax;//1.0mm防震动
    s_wEjectMove=travel_eject * wMeterValue/wMeterMax;
    if(s_wEjectMove>s_wEjectMoveMax)
        s_wEjectMoveMax=s_wEjectMove;//---------------产品使用
    WI32(main_pg.subMach_pixMachineEjectPic->x_pos, pixeject_x-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)) + s_wEjectMove);


    if(VarAdrToUI16(OPMODE_INDEX) == MODE_AUTO || VarAdrToUI16(OPMODE_INDEX) == 3)
    {
        if(m_currentAction & (1 << ACTPROC_INJECT))//-----------------射出状态
        {
            if(!bOnce)
            {
                s_wFill1start=s_wInject1Move/2;//20191105.dxl 改起点标志为终点标志
                bOnce=TRUE;
                s_wEjectMoveMax=0;
            }
            wMeterValue = VarAdrToInt(d_admeter1_STATE_ADPOSI_RELATIVE_1);//电子尺实时位置
            if(s_wInject1Move < s_wFill1start)//20191105.dxl 改起点标志为终点标志
            {
                s_wFill1Move=travel_fill1;
                if(!s_wFill1start)
                {
                    s_wFill2Move=travel_fill2;
                }
                else
                {
                    s_wFill2Move=travel_fill2 - travel_fill2*s_wInject1Move/s_wFill1start;//20191105.dxl 改起点标志为终点标志
                }
            }
            else
            {
                if(s_wFill1start==0)
                    s_wFill1Move=travel_fill1;
                else if((2*s_wFill1start) >= s_wInject1Move)//20191105.dxl 改起点标志为终点标志
                    s_wFill1Move=travel_fill1*((2*s_wFill1start)-s_wInject1Move)/s_wFill1start;//20191105.dxl 改起点标志为终点标志
                s_wFill2Move=0;
            }
            if (WGTGetVisible(main_pg.subMach_pixproduct))
                WGTSetVisible(main_pg.subMach_pixproduct, FALSE);
            if (!WGTGetVisible(main_pg.subMach_pixfill1))
                WGTSetVisible(main_pg.subMach_pixfill1, TRUE);
            if (!WGTGetVisible(main_pg.subMach_pixfill2))
                WGTSetVisible(main_pg.subMach_pixfill2, TRUE);
            if (!WGTGetVisible(main_pg.subMach_pixfill3))
                WGTSetVisible(main_pg.subMach_pixfill3, TRUE);
            WI32(main_pg.subMach_pixfill1->x_pos, fill1_Rx-s_wFill1Move);
            WI32(main_pg.subMach_pixfill1->x_size, s_wFill1Move);
            WI32(main_pg.subMach_pixfill2->y_pos, fill2_Ry-s_wFill2Move);
            WI32(main_pg.subMach_pixfill2->y_size, s_wFill2Move);
            WI32(main_pg.subMach_pixfill3->y_size, s_wFill2Move);
        }
        else
        {
            if(bOnce)
                bOnce = FALSE;
            if(m_currentAction & ((1 << ACTPROC_CHARGE)|(1 << ACTPROC_COOL)))
            {
                WI32(main_pg.subMach_pixproduct->x_pos, product_Rx);
                WGTSetVisible(main_pg.subMach_pixproduct, TRUE);
                if (WGTGetVisible(main_pg.subMach_pixfill1))
                    WGTSetVisible(main_pg.subMach_pixfill1, FALSE);
                if (WGTGetVisible(main_pg.subMach_pixfill2))
                    WGTSetVisible(main_pg.subMach_pixfill2, FALSE);
                if (WGTGetVisible(main_pg.subMach_pixfill3))
                    WGTSetVisible(main_pg.subMach_pixfill3, FALSE);
            }
            else if(m_currentAction & (1 << ACTPROC_OPEN) || m_currentAction & (1 << ACTPROC_EJECT))
            {
                if (!WGTGetVisible(main_pg.subMach_pixproduct))
                    WGTSetVisible(main_pg.subMach_pixproduct, TRUE);
                WI32(main_pg.subMach_pixproduct->x_pos, product_Rx-s_nwide*2*(1-cos(s_wClampMove*3.14/180.0)) + s_wEjectMoveMax);
                if (WGTGetVisible(main_pg.subMach_pixfill1))
                    WGTSetVisible(main_pg.subMach_pixfill1, FALSE);
                if (WGTGetVisible(main_pg.subMach_pixfill2))
                    WGTSetVisible(main_pg.subMach_pixfill2, FALSE);
                if (WGTGetVisible(main_pg.subMach_pixfill3))
                    WGTSetVisible(main_pg.subMach_pixfill3, FALSE);

                if(abs(s_wEjectMoveMax-travel_eject) < 3)//完成产品脱落
                {
                    WGTSetVisible(main_pg.subMach_pixproduct, FALSE);
                    WI32(main_pg.subMach_pixproduct->x_pos, product_Rx);
                }
            }
            else if (m_currentAction & (1 << ACTPROC_CLOSE))
            {
                if (WGTGetVisible(main_pg.subMach_pixproduct)){
                    WGTSetVisible(main_pg.subMach_pixproduct, FALSE);
                    WI32(main_pg.subMach_pixproduct->x_pos, product_Rx);
                }
                if (WGTGetVisible(main_pg.subMach_pixfill1))
                    WGTSetVisible(main_pg.subMach_pixfill1, FALSE);
                if (WGTGetVisible(main_pg.subMach_pixfill2))
                    WGTSetVisible(main_pg.subMach_pixfill2, FALSE);
                if (WGTGetVisible(main_pg.subMach_pixfill3))
                    WGTSetVisible(main_pg.subMach_pixfill3, FALSE);
            }
        }
    }
    else
    {
        if (WGTGetVisible(main_pg.subMach_pixproduct)){
            WGTSetVisible(main_pg.subMach_pixproduct, FALSE);
            WI32(main_pg.subMach_pixproduct->x_pos, product_Rx);
        }
        if (WGTGetVisible(main_pg.subMach_pixfill1))
            WGTSetVisible(main_pg.subMach_pixfill1, FALSE);
        if (WGTGetVisible(main_pg.subMach_pixfill2))
            WGTSetVisible(main_pg.subMach_pixfill2, FALSE);
        if (WGTGetVisible(main_pg.subMach_pixfill3))
            WGTSetVisible(main_pg.subMach_pixfill3, FALSE);
    }

     //压力表显示
    pressad_ch = VarAdrToInt(d_Pump1_MACHSET_PRESSAD_CH);
//    if (IV5200 || IV5300)
//    {
//        if((pressad_ch>=4)&&(pressad_ch<=8))
//        {
//            max_press = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_4+(pressad_ch-4));
//        }
//        else if((pressad_ch>=9)&&(pressad_ch<=12))
//        {
//            max_press = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_EXT1+(pressad_ch-9));
//        }
//    }
//    else
//    {
//        if(pressad_ch == 4)
//        {
//            max_press = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_4);
//        }
//        else if((pressad_ch>=5)&&(pressad_ch<=12))
//        {
//            max_press = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_EXT1+(pressad_ch-5));
//        }
//    }
    if((pressad_ch>=4)&&(pressad_ch<=g_mainresource_current.nADNum))//20220727 dyl
    {
        max_press = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_4+(pressad_ch-4));
    }
    else
    {
        max_press = VarAdrToInt(d_admeter1_STATE_ADPOSI_METERMAX_EXT1+(pressad_ch-g_mainresource_current.nADNum));
    }

    if(max_press > 0)
    {
        if(EditGetValue((PEDIT_WGT)main_pg.subMach_subpage5_dtclock) > max_press)
        {
            ImageSetRotateDeg(main_pg.subMach_subpage5_widget_prs, 250);
        }
        else
        {
            ImageSetRotateDeg(main_pg.subMach_subpage5_widget_prs, EditGetValue((PEDIT_WGT)main_pg.subMach_subpage5_dtclock)*250/max_press);
        }
    }
    else
    {
        if(EditGetValue((PEDIT_WGT)main_pg.subMach_subpage5_dtclock) > 250)
        {
            ImageSetRotateDeg(main_pg.subMach_subpage5_widget_prs, 250);
        }
        else
        {
            ImageSetRotateDeg(main_pg.subMach_subpage5_widget_prs, EditGetValue((PEDIT_WGT)main_pg.subMach_subpage5_dtclock)/10);
        }
    }
    //20211122 dyl 系统压力2压力表
    if ((VarAdrToInt(d_machine1_MOLDSET_FL_HYDY_WAY_SET) > 0))
    {
        if(max_press > 0)
        {
            if(EditGetValue((PEDIT_WGT)main_pg.subMach_subpage4_dtclock2) > max_press)
            {
                ImageSetRotateDeg(main_pg.subMach_subpage4_widget_prs2, 250);
            }
            else
            {
                ImageSetRotateDeg(main_pg.subMach_subpage4_widget_prs2, EditGetValue((PEDIT_WGT)main_pg.subMach_subpage4_dtclock2)*250/max_press);
            }
        }
        else
        {
            if(EditGetValue((PEDIT_WGT)main_pg.subMach_subpage4_dtclock2) > 250)
            {
                ImageSetRotateDeg(main_pg.subMach_subpage4_widget_prs2, 250);
            }
            else
            {
                ImageSetRotateDeg(main_pg.subMach_subpage4_widget_prs2, EditGetValue((PEDIT_WGT)main_pg.subMach_subpage4_dtclock2)/10);
            }
        }
    }

}
/**********MachineActStart*******************************************************************/


/**********Act-TimeLine-Start*********************************************************************************/
#define MIN_TIME_RANGE 250 //动作图表时间范围为2.5s的倍数
static void setActionMaxTime(UI32 maxTime)//20210108
{
    int maxRange, retValue, i;
    static int m_timeMaxRange = 0;
    static int m_timeRangeDownDly = 3;
    UI32 y_step_time = 0;
    static UI16 nProd = 0;

    if(nProd != OPGetCurRecordNum())//操作记录增加3个刷新时间最大值
    {
        nProd = OPGetCurRecordNum();
        if(m_timeRangeDownDly > 0) --m_timeRangeDownDly;
    }

    if(maxTime < MIN_TIME_RANGE) {
        maxRange = MIN_TIME_RANGE;
    }
    else {
        retValue = maxTime%MIN_TIME_RANGE;
        maxRange = maxTime-retValue;
        if(retValue > maxRange/12) {
            //超出可留一定余量
            maxRange += MIN_TIME_RANGE;
        }
    }

    if((m_timeMaxRange < maxRange) || ((m_timeMaxRange > maxRange) && (m_timeRangeDownDly <= 0))) {
        m_timeRangeDownDly = 3;
        m_timeMaxRange = maxRange;

        VarAdrSetInt(TIME_LINE_MAX, maxRange);//Line最大值

        VarAdrSetInt(TIME_LINE_Y0, maxRange/10);//设置Y轴坐标
        y_step_time = maxRange / 5;
        for(i = 0; i < 4; ++i)
        {
            VarAdrSetInt(TIME_LINE_Y1+i, (maxRange - y_step_time*(i+1))/10);
        }
    }
}

static void ActTime()
{
    int i;
    UI32 end_time[10];
    UI32 run_time[10];
    UI32 max_time = 0; //20210108
    //cool time 特殊处理  其余直接绑定变量

    //Time Line 结束时间计算。 开始时间直接绑定变量
    run_time[0] = VarAdrToInt(d_clamp1_STATE_TM_CLAMPCLS_RT);
    end_time[0] = VarAdrToInt(d_clamp1_STATE_TM_CLAMPCLS_START) + run_time[0];
    VarAdrSetInt(CLOSE_END_TM, end_time[0]);
    run_time[1] = VarAdrToInt(d_nozzle1_STATE_TM_NOZZLEF_RT);
    end_time[1] = VarAdrToInt(d_nozzle1_STATE_TM_NOZZLEF_START) + run_time[1];
    VarAdrSetInt(NOZADV_END_TM, end_time[1]);
    //20200604 射出保压小数位3位是仍显示2位小数
    if(VarAdrToInt(p_PP_OBJ_INJ_AND_HOLD_TM_DOT) == 3)
    {
        VarAdrSetInt(INJECT_START_TM, VarAdrToInt(d_inject1_STATE_HOLD_START)/10);
        run_time[2] = VarAdrToInt(d_inject1_STATE_TM_INJ_RT)/10;
        end_time[2] = VarAdrToInt(d_inject1_STATE_HOLD_START)/10 + run_time[2];
    }
    else
    {
        VarAdrSetInt(INJECT_START_TM, VarAdrToInt(d_inject1_STATE_HOLD_START));
        run_time[2] = VarAdrToInt(d_inject1_STATE_TM_INJ_RT);
        end_time[2] = VarAdrToInt(d_inject1_STATE_HOLD_START) + run_time[2];
    }
    VarAdrSetInt(INJECT_END_TM, end_time[2]);
    //20200604 射出保压小数位3位是仍显示2位小数
    if(VarAdrToInt(p_PP_OBJ_INJ_AND_HOLD_TM_DOT) == 3)
    {
        VarAdrSetInt(HOLD_START_TM, VarAdrToInt(d_inject1_STATE_HOLD_RT)/10);
        run_time[3] = VarAdrToInt(d_inject1_STATE_TM_HOLD_RT)/10;
        end_time[3] = VarAdrToInt(d_inject1_STATE_HOLD_RT)/10 + run_time[3];
    }
    else
    {
        VarAdrSetInt(HOLD_START_TM, VarAdrToInt(d_inject1_STATE_HOLD_RT));
        run_time[3] = VarAdrToInt(d_inject1_STATE_TM_HOLD_RT);
        end_time[3] = VarAdrToInt(d_inject1_STATE_HOLD_RT) + run_time[3];
    }
    VarAdrSetInt(HOLD_END_TM, end_time[3]);
    run_time[4] = VarAdrToInt(d_nozzle1_STATE_TM_NOZZLEB_RT);
    end_time[4] = VarAdrToInt(d_nozzle1_STATE_TM_NOZZLEB_START) + run_time[4];
    VarAdrSetInt(NOZRET_END_TM, end_time[4]);
    run_time[5] = VarAdrToInt(d_inject1_STATE_TM_CHARGE_RT);
    end_time[5] = VarAdrToInt(d_inject1_STATE_SUCKBACK_START) + run_time[5];
    VarAdrSetInt(CHARGE_END_TM, end_time[5]);
    run_time[6] = VarAdrToInt(d_inject1_STATE_TM_SUCKBK_RT);
    end_time[6] = VarAdrToInt(d_inject1_STATE_SUCKBK_RT) + run_time[6];
    VarAdrSetInt(SUCK_END_TM, end_time[6]);
    run_time[7] = VarAdrToInt(d_nozzle1_STATE_TM_COOL_RT); // | (VarAdrToInt(d_nozzle1_STATE_TM_COOL_RTH) << 16);
    end_time[7] = VarAdrToInt(d_nozzle1_STATE_TM_COOL_START) + run_time[7];
    VarAdrSetInt(COOL_END_TM, end_time[7]);
    run_time[8] = VarAdrToInt(d_clamp1_STATE_TM_CLAMPOPN_RT);
    end_time[8] = VarAdrToInt(d_clamp1_STATE_TM_CLAMPOPN_START) + run_time[8];
    VarAdrSetInt(OPEN_END_TM, end_time[8]);
    run_time[9] = VarAdrToInt(d_eject1_STATE_TM_EJECT_RT);
    end_time[9] = VarAdrToInt(d_eject1_STATE_TM_EJECT_START) + run_time[9];
    VarAdrSetInt(EJECT_END_TM, end_time[9]);


    for(i = 0; i < 10; ++i)
    {
        if(max_time < end_time[i])
            max_time = end_time[i];
        VarAdrSetInt(ACTIVE_IMG+i, 1);
        //VarAdrSetInt(CURRENT_TIME, run_time[i]);//时间同时刷新
        if(m_currentAction & (1 << i))
        {
            //需要修改图片filename的接口
//            ImageSetPicName(main_pg.subCurAct_PicAct_Current, ActCurrentImg[i]);
            VarAdrSetInt(ACTIVE_IMG+i, 2);
        }
    }

    setActionMaxTime(max_time);//20210108

    //20200604 射出保压小数位3位是仍显示2位小数
    if(VarAdrToInt(p_PP_OBJ_INJ_AND_HOLD_TM_DOT) == 3)
    {
        EditSetNum(main_pg.ActTm_3,VarAdrToInt(d_inject1_STATE_TM_INJ_RT)/10,2);
        EditSetNum(main_pg.edit3,VarAdrToInt(d_inject1_STATE_TM_INJ)/10,2);

        EditSetNum(main_pg.ActTm_4,VarAdrToInt(d_inject1_STATE_TM_HOLD_RT)/10,2);
        EditSetNum(main_pg.edit4,VarAdrToInt(d_inject1_STATE_TM_HOLD_L)/10,2);
    }
    else
    {
        EditSetNum(main_pg.ActTm_3,VarAdrToInt(d_inject1_STATE_TM_INJ_RT),2);
        EditSetNum(main_pg.edit3,VarAdrToInt(d_inject1_STATE_TM_INJ),2);

        EditSetNum(main_pg.ActTm_4,VarAdrToInt(d_inject1_STATE_TM_HOLD_RT),2);
        EditSetNum(main_pg.edit4,VarAdrToInt(d_inject1_STATE_TM_HOLD_L),2);
    }
}



/**********Act-TimeLine-End*********************************************************************************/


/***************TempStart*****************************************************************/
static BOOL GetMotorState()
{
    int i, choose;
    for(i = 0; i < 9; ++i)
    {
        choose = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_FUNC_2 + i);
        if(choose == 5)
            return TRUE;
    }
    return FALSE;
}
/**
* @brief     :扩展温度显示
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191204
*/
static UI16 TempxSegments(UI16 seg, BOOL isb)
{
    UI16 bit = 0;
    switch (seg) {
    case 5:
        setone(bit, 0);
        break;
    case 6:
        setone(bit, 0);
        setone(bit, 1);
        break;
    case 7:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        break;
    case 8:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        break;
    case 9:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        setone(bit, 4);
        break;
    case 10:
        setone(bit, 0);
        setone(bit, 1);
        setone(bit, 2);
        setone(bit, 3);
        setone(bit, 4);
        setone(bit, 5);
        break;
    default:
        break;
    }
//    if(VarAdrToInt(p_PP_STATE_TEMP_CTRL_PART) > 1)
//    {
//        if(isb)
//        {
//            if(GetMotorState())
//            {
//                setzero(bit, seg-5);
//                setone(bit, 6);
//            }
//        }
//    }
//    else
//    {
//        if(GetMotorState())
//        {
//            setzero(bit, seg-5);
//            setone(bit, 6);
//        }
//    }
    return bit;
}
static void TempxShow()
{
    UI16 seg1, seg2;
    UI16 bit1, bit2;
    seg1 = VarAdrToInt(d_TempCtrlComm1_MACHSET_DATEMP_CH_NUM);
    seg2 = VarAdrToInt(d_TempCtrlComm2_MACHSET_DATEMP_CH_NUM);

    bit1 = TempxSegments(seg1, FALSE);
    bit2 = TempxSegments(seg2, TRUE);
    VarAdrSetInt(A_PAGE_SEL, 0xffff & bit1);
    VarAdrSetInt(B_PAGE_SEL, 0xffff & bit2);
}

static void SubPageTempShow(Main_PG* ppg)
{
    UI16 temp_part;
    temp_part = VarAdrToInt(TEMP_CTRL_PART);

    if(temp_part >= 2)
    {
        WGTSetVisible(ppg->subTemp, FALSE);
        WGTSetVisible(ppg->subTempx, TRUE);
        WGTSetVisible(ppg->subTempxA, FALSE);
        TempxShow();
    }
    else if(temp_part == 1)
    {
        WGTSetVisible(ppg->subTemp, FALSE);
        WGTSetVisible(ppg->subTempx, FALSE);
        WGTSetVisible(ppg->subTempxA, TRUE);
        TempxShow();
    }
    else {
        WGTSetVisible(ppg->subTemp, TRUE);
        WGTSetVisible(ppg->subTempx, FALSE);
        WGTSetVisible(ppg->subTempxA, FALSE);
    }

}

static const char* TempRealStr(char* buf, UI32 adr, UI16 size)
{
    UI32 num;
    num = VarAdrToInt(adr);
    if(num == 999){
        if(GetTextTran(TEXT_HEATALARM999) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM999), size);
    }
    else if(num == 777){
        if(GetTextTran(TEXT_HEATALARM777) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM777), size);
    }
    else if(num == 788){
        if(GetTextTran(TEXT_HEATALARM788) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM788), size);
    }
    else if(num == 779){
        if(GetTextTran(TEXT_HEATALARM779) != NULL)
            strncpy(buf, GetTextTran(TEXT_HEATALARM779), size);
    }
    else {
        snprintf(buf, size-1, "%d", num);
    }
    return buf;
}
static void TempRealShowUpdate()
{
    char buf[20]= "";
    UI16 i, size;
    UI16 part;
    UI16 oil, motor, mouth;
    part = VarAdrToInt(p_PP_STATE_TEMP_CTRL_PART);
    size = sizeof(buf)-1;
    for(i = 0; i < 9; ++i){
        if(part >=2){
            strncpy(buf, TempRealStr(buf, d_TempCtrlComm2_STATE_DATEMP_REAL_2 + i, size), size);
            VarAdrSetStr(TMP_REAL2 + i, buf);
        }
            strncpy(buf, TempRealStr(buf, d_TempCtrlComm1_STATE_DATEMP_REAL_2 + i, size), size);
            VarAdrSetStr(TMP_REAL1 + i, buf);
    }
    //油温显示
    if(part)
    {
        strncpy(buf, TempRealStr(buf, d_oil1_STATE_DATEMP_OIL, size), size);
        oil = VarAdrToInt(d_oil1_STATE_DATEMP_OIL);
        if(oil == 999 || oil == 777 || oil == 788 || oil == 779){
            WGTSetVisible(main_pg.subMach_subpage1_lboil, FALSE);
        }
        else{
            WGTSetVisible(main_pg.subMach_subpage1_lboil, TRUE);
        }
    }
    else
    {
        strncpy(buf, TempRealStr(buf, d_tempctrl1_STATE_DATEMP_REAL_1, size), size);
        oil = VarAdrToInt(d_tempctrl1_STATE_DATEMP_REAL_1);
        if(oil == 999 || oil == 777 || oil == 788 || oil == 779){
            WGTSetVisible(main_pg.subMach_subpage1_lboil, FALSE);
        }
        else{
            WGTSetVisible(main_pg.subMach_subpage1_lboil, TRUE);
        }
    }
    VarAdrSetStr(TMP_OIL, buf);

    //motor temp
    if(GetMotorState())
    {
        strncpy(buf, TempRealStr(buf, d_motor1_STATE_DATEMP_REALMOTOR, size), size);
        motor = VarAdrToInt(d_motor1_STATE_DATEMP_REALMOTOR);
        if(motor == 999 || motor == 777 || motor == 788 || motor == 779){
            WGTSetVisible(main_pg.subMach_subpage2_lbmotor, FALSE);
        }
        else{
            WGTSetVisible(main_pg.subMach_subpage2_lbmotor, TRUE);
        }
        VarAdrSetStr(TMP_MOTOR, buf);
    }

    //mouth temp
    if(VarAdrToInt(p_PP_OBJ_MOUTH_VISIBLE))
    {
        mouth = VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_CHARGEPOS);
        if(mouth == 999 || mouth == 777 || mouth == 788 || mouth == 779)
            WGTSetVisible(main_pg.mouth_unit, FALSE);
        else
            WGTSetVisible(main_pg.mouth_unit, TRUE);
        strncpy(buf, TempRealStr(buf, d_TempCtrlComm1_STATE_DATEMP_CHARGEPOS, size), size);
        VarAdrSetStr(TMP_MOUTH, buf);
    }
    else
    {
        WGTSetVisible(main_pg.mouth_unit, FALSE);
    }
}
/***************TempEnd*****************************************************************/

/****************MainKeyStart***********************************************************************/
#define MACH_RELEASE_IMAGE   "btpart1.png"
#define MACH_FOCUS_IMAGE     "btpart1x.png"
#define IO_RELEASE_IMAGE   "btpart2.png"
#define IO_FOCUS_IMAGE     "btpart2x.png"
#define PRODUCT_RELEASE_IMAGE   "btpart3.png"
#define PRODUCT_FOCUS_IMAGE     "btpart3x.png"
#define OUTPUT_RELEASE_IMAGE   "btpart4.png"
#define OUTPUT_FOCUS_IMAGE     "btpart4x.png"
static void ButtonSetTextColor(PWGT pwgt,int value_int)
{
    //20210914 dyl touch
    //OBJSetValueByAttrName((POBJ)pwgt,"textColor",value_int,0,NULL,ATTR_UINT);
    POBJ po;
    po = (POBJ)pwgt;
    OBJSetValueByAttrName(po, "textColor", value_int, 0, 0, ATTR_INT);
    OBJSetValueByAttrName(po, "focusFTColor", value_int, 0, 0, ATTR_INT);
}

static void ButtonSetReleasedImage(PWGT pwgt,char* value_str)
{
    //20210914 dyl touch
    //OBJSetValueByAttrName((POBJ)pwgt,"releasedImage",0,0,value_str,ATTR_STR);
    POBJ po;
    po = (POBJ)pwgt;
    OBJSetValueByAttrName(po, "releasedImage", 0, 0, value_str, ATTR_STR);
    OBJSetValueByAttrName(po, "pressedImage", 0, 0, value_str, ATTR_STR);
    OBJSetValueByAttrName(po, "focusImage", 0, 0, value_str, ATTR_STR);
}

static UI16 main_key;
static void MainKeyCb(UI32 key,UI32 press)
{
    UI16 bit = 0;
    if(key == GUI_KEY_MAINPAGE)
    {
        main_key++;
        switch (main_key) {
        case 1:    //in
            bit = 1;
            ButtonSetText(main_pg.btio, GetTextTran(TEXT_INPUT));
            ButtonSetReleasedImage(main_pg.btMachine,MACH_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btio,IO_FOCUS_IMAGE);
            ButtonSetReleasedImage(main_pg.btProduct,PRODUCT_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btOutput,OUTPUT_RELEASE_IMAGE);
            ButtonSetTextColor(main_pg.btMachine,GUI_BLACK);
            ButtonSetTextColor(main_pg.btio,GUI_WHITE);
            ButtonSetTextColor(main_pg.btProduct,GUI_BLACK);
            ButtonSetTextColor(main_pg.btOutput,GUI_BLACK);
            break;
        case 2:    //out
            ButtonSetText(main_pg.btio, GetTextTran(TEXT_OUTPUT));
            ButtonSetReleasedImage(main_pg.btMachine,MACH_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btio,IO_FOCUS_IMAGE);
            ButtonSetReleasedImage(main_pg.btProduct,PRODUCT_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btOutput,OUTPUT_RELEASE_IMAGE);
            ButtonSetTextColor(main_pg.btMachine,GUI_BLACK);
            ButtonSetTextColor(main_pg.btio,GUI_WHITE);
            ButtonSetTextColor(main_pg.btProduct,GUI_BLACK);
            ButtonSetTextColor(main_pg.btOutput,GUI_BLACK);
            bit = 2;
            break;
        case 3:    //prod
            ButtonSetText(main_pg.btio, "I/O");
            ButtonSetReleasedImage(main_pg.btMachine,MACH_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btio,IO_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btProduct,PRODUCT_FOCUS_IMAGE);
            ButtonSetReleasedImage(main_pg.btOutput,OUTPUT_RELEASE_IMAGE);
            ButtonSetTextColor(main_pg.btMachine,GUI_BLACK);
            ButtonSetTextColor(main_pg.btio,GUI_BLACK);
            ButtonSetTextColor(main_pg.btProduct,GUI_WHITE);
            ButtonSetTextColor(main_pg.btOutput,GUI_BLACK);
            bit = 4;
            break;
        case 4:    //output //20210223
            ButtonSetText(main_pg.btio, "I/O");
            ButtonSetReleasedImage(main_pg.btMachine,MACH_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btio,IO_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btProduct,PRODUCT_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btOutput,OUTPUT_FOCUS_IMAGE);
            ButtonSetTextColor(main_pg.btMachine,GUI_BLACK);
            ButtonSetTextColor(main_pg.btio,GUI_BLACK);
            ButtonSetTextColor(main_pg.btProduct,GUI_BLACK);
            ButtonSetTextColor(main_pg.btOutput,GUI_WHITE);
            bit = 8;
            break;
        case 5:    //mach
            ButtonSetText(main_pg.btio, "I/O");
            ButtonSetReleasedImage(main_pg.btMachine,MACH_FOCUS_IMAGE);
            ButtonSetReleasedImage(main_pg.btio,IO_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btProduct,PRODUCT_RELEASE_IMAGE);
            ButtonSetReleasedImage(main_pg.btOutput,OUTPUT_RELEASE_IMAGE);
            ButtonSetTextColor(main_pg.btMachine,GUI_WHITE);
            ButtonSetTextColor(main_pg.btio,GUI_BLACK);
            ButtonSetTextColor(main_pg.btProduct,GUI_BLACK);
            ButtonSetTextColor(main_pg.btOutput,GUI_BLACK);
            bit = 16;
            main_key = 0;
            break;
        default:
            break;
        }
            WGTSetVisible(main_pg.subMach, getbit(bit, 4));
            WGTSetVisible(main_pg.tabin, getbit(bit, 0));
            WGTSetVisible(main_pg.tabout, getbit(bit, 1));
            WGTSetVisible(main_pg.subProduct, getbit(bit, 2));
            WGTSetVisible(main_pg.subOutput, getbit(bit, 3));
            WGTSetVisible(main_pg.leftbt, getbit(bit, 0)||getbit(bit, 1));//20210914 dyl touch
            WGTSetVisible(main_pg.rightbt, getbit(bit, 0)||getbit(bit, 1));//20210914 dyl touch
    }
    else if(key == GUI_KEY_LEFT)
    {
        if(main_key == 1)
            TabPageSelPrev(main_pg.tabin, FALSE);
        else if(main_key == 2)
            TabPageSelPrev(main_pg.tabout, FALSE);
    }
    else if(key == GUI_KEY_RIGHT)
    {
        if(main_key == 1)
            TabPageSelNext(main_pg.tabin, FALSE);
        else if(main_key == 2)
            TabPageSelNext(main_pg.tabout, FALSE);
    }
#ifdef M788_PANEL
    else if(key == GUI_KEY_UP)
    {
        UI16 lcdbright = VarAdrToInt(p_PP_MACH2_Bright);
        if(lcdbright <= 90)
        {
            VarAdrSetInt(p_PP_MACH2_Bright, lcdbright+10);
            VarAdrSaveInt(p_PP_MACH2_Bright);
            SetLCD(TRUE);
        }
    }
    else if(key == GUI_KEY_DOWN)
    {
        UI16 lcdbright = VarAdrToInt(p_PP_MACH2_Bright);
        if(lcdbright > 20)
        {
            VarAdrSetInt(p_PP_MACH2_Bright, lcdbright-10);
            VarAdrSaveInt(p_PP_MACH2_Bright);
            SetLCD(TRUE);
        }
    }
#endif
}

/*********** Touch start ***********/ //20210914 dyl
static void MachInfoButton()
{
    main_key = 0;
    ButtonSetText(main_pg.btio, "I/O");
    WGTSetVisible(main_pg.subMach, TRUE);
    WGTSetVisible(main_pg.tabin, FALSE);
    WGTSetVisible(main_pg.tabout, FALSE);
    WGTSetVisible(main_pg.subProduct, FALSE);
    WGTSetVisible(main_pg.leftbt, FALSE);
    WGTSetVisible(main_pg.rightbt, FALSE);
    WGTSetVisible(main_pg.subOutput, FALSE);
}
static void IOButton()
{
    if(!strcmp(ButtonGetText(main_pg.btio), GetTextTran(TEXT_OUTPUT))
            || !strcmp(ButtonGetText(main_pg.btio), "I/O"))
    {
        ButtonSetText(main_pg.btio, GetTextTran(TEXT_INPUT));
        TabPageSelPage(main_pg.tabin, 0);
        WGTSetVisible(main_pg.tabin, TRUE);
        WGTSetVisible(main_pg.tabout, FALSE);
        main_key = 1;
    }
    else
    {
        ButtonSetText(main_pg.btio, GetTextTran(TEXT_OUTPUT));
        TabPageSelPage(main_pg.tabout, 0);
        WGTSetVisible(main_pg.tabin, FALSE);
        WGTSetVisible(main_pg.tabout, TRUE);
        main_key = 2;
    }

    WGTSetVisible(main_pg.subMach, FALSE);
    WGTSetVisible(main_pg.subProduct, FALSE);
    WGTSetVisible(main_pg.leftbt, TRUE);
    WGTSetVisible(main_pg.rightbt, TRUE);
    WGTSetVisible(main_pg.subOutput, FALSE);
}
static void ProdCntButton()
{
    main_key = 3;
    ButtonSetText(main_pg.btio, "I/O");
    WGTSetVisible(main_pg.subMach, FALSE);
    WGTSetVisible(main_pg.tabin, FALSE);
    WGTSetVisible(main_pg.tabout, FALSE);
    WGTSetVisible(main_pg.subProduct, TRUE);
    WGTSetVisible(main_pg.leftbt, FALSE);
    WGTSetVisible(main_pg.rightbt, FALSE);
    WGTSetVisible(main_pg.subOutput, FALSE);
}
static void OutputButton()
{
    main_key = 4;
    ButtonSetText(main_pg.btio, "I/O");
    WGTSetVisible(main_pg.subMach, FALSE);
    WGTSetVisible(main_pg.tabin, FALSE);
    WGTSetVisible(main_pg.tabout, FALSE);
    WGTSetVisible(main_pg.subProduct, FALSE);
    WGTSetVisible(main_pg.leftbt, FALSE);
    WGTSetVisible(main_pg.rightbt, FALSE);
    WGTSetVisible(main_pg.subOutput, TRUE);
}
/*********** Touch end   ***********/
/****************MainKeyEnd***********************************************************************/

/***********PageOnShowStart********************************************************************************/
/**
* @brief     :进入画面时的 一些控件的属性
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191205
*/
static void MainWgtShow(Main_PG* ppg)
{
    WGTSetVisible(ppg->subMach, TRUE);
    WGTSetVisible(ppg->tabin, FALSE);
    WGTSetVisible(ppg->tabout, FALSE);
    WGTSetVisible(ppg->subProduct, FALSE);
    WGTSetVisible(ppg->subOutput, FALSE);
    TabPageSelPage(ppg->tabin, 0);
    /*********** Touch start ***********/ //20210914 dyl
    TabPageSelPage(main_pg.tabin, 4);
    Focus(ppg->btMachine);
//    TabPageSelPage(ppg->tabout, 0);
    /*********** Touch end   ***********/
    ButtonSetReleasedImage(main_pg.btMachine,MACH_FOCUS_IMAGE);
    ButtonSetReleasedImage(main_pg.btio,IO_RELEASE_IMAGE);
    ButtonSetReleasedImage(main_pg.btProduct,PRODUCT_RELEASE_IMAGE);
    ButtonSetReleasedImage(main_pg.btOutput,OUTPUT_RELEASE_IMAGE);
    ButtonSetTextColor(main_pg.btMachine,GUI_WHITE);
    ButtonSetTextColor(main_pg.btio,GUI_BLACK);
    ButtonSetTextColor(main_pg.btProduct,GUI_BLACK);
    ButtonSetTextColor(main_pg.btOutput,GUI_BLACK);
    ButtonSetText(main_pg.btio, "I/O");

    //20211122 dyl 系统压力2显示
    if ((VarAdrToInt(d_machine1_MOLDSET_FL_HYDY_WAY_SET) > 0)
            && ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04) || ((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0 && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))))
    {
        WGTSetVisible(ppg->subMach_subpage4,TRUE);
    }
    else
    {
        WGTSetVisible(ppg->subMach_subpage4,FALSE);
    }
//    //系统压力
//    if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x04)
//    {
//        WGTSetAttrByName(ppg->subMach_dtclock, "text", d_Pump1_STATE_SERVO_PRESS);  //反馈压力
//    }
//    else if((VarAdrToInt(SYS_FL_MACH_CODE0) & 0x0004) == 0 && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020))
//    {
////        if(VarAdrToInt(SYS_FL_METER) & 0x0004)
////        {
//            WGTSetAttrByName(ppg->subMach_dtclock, "text", d_servo1_STATE_SERVO_PRESS_1);  //反馈压力
////        }
////        else
////        {
////            if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0xa620 || VarAdrToInt(SYS_FL_CTRLMODEL) == 0x2100
////                    || VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100)
////                WGTSetAttrByName(ppg->subMach_dtclock, "text", d_admeter1_STATE_ADPOSI_RELATIVE_4);  //ad4
////            else if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0xa920 || VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5000)
////                WGTSetAttrByName(ppg->subMach_dtclock, "text", d_admeter1_STATE_ADPOSI_RELATIVE_4);  //ad6
////        }
//    }
//    else
//    {
//        if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0xa620 || VarAdrToInt(SYS_FL_CTRLMODEL) == 0x2100
//            || VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100)
//            WGTSetAttrByName(ppg->subMach_dtclock, "text", d_admeter1_STATE_ADPOSI_RELATIVE_4);  //ad4
//        else if(VarAdrToInt(SYS_FL_CTRLMODEL) == 0xa920 || VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5000)
//            WGTSetAttrByName(ppg->subMach_dtclock, "text", d_admeter1_STATE_ADPOSI_RELATIVE_4);  //ad6
//    }


    //机械手显示
    if(VarAdrToInt(d_robot1_MOLDSET_FL_ROBOTCTRL) > 0)
    {
        WGTSetVisible(ppg->subMach_subpage3,TRUE);
        if( VarAdrToInt(d_robot1_MOLDSET_FL_ROBOTCLPMID) == 1)
        {
            WGTSetVisible(ppg->subMach_subpage3_Robot, TRUE);
            WGTSetVisible(ppg->subMach_subpage3_edrobot, TRUE);
            WGTSetVisible(ppg->subMach_subpage3_lbrobot, TRUE);
        }
        else
        {
            WGTSetVisible(ppg->subMach_subpage3_Robot, FALSE);
            WGTSetVisible(ppg->subMach_subpage3_edrobot, FALSE);
            WGTSetVisible(ppg->subMach_subpage3_lbrobot, FALSE);
        }

        WGTSetVisible(ppg->subMach_robotimg, TRUE);
    }
    else
    {
        WGTSetVisible(ppg->subMach_subpage3,FALSE);
        //WGTSetVisible(ppg->subMach_Robot, FALSE);
        //WGTSetVisible(ppg->subMach_edrobot, FALSE);
        //WGTSetVisible(ppg->subMach_lbrobot, FALSE);
        WGTSetVisible(ppg->subMach_robotimg, FALSE);
    }

    // 电机
    if(GetMotorState())
    {
        WGTSetVisible(ppg->subMach_motorimg, TRUE);
        //WGTSetVisible(ppg->subMach_MotrTemp, TRUE);
        //WGTSetVisible(ppg->subMach_edmotor, TRUE);
        //WGTSetVisible(ppg->subMach_lbmotor, TRUE);
        WGTSetVisible(ppg->subMach_subpage2,TRUE);
    }
    else
    {
        WGTSetVisible(ppg->subMach_motorimg, FALSE);
        //WGTSetVisible(ppg->subMach_MotrTemp, FALSE);
        //WGTSetVisible(ppg->subMach_edmotor, FALSE);
        //WGTSetVisible(ppg->subMach_lbmotor, FALSE);
        WGTSetVisible(ppg->subMach_subpage2,FALSE);
    }

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
    WGTSetVisible(ppg->subOutput_lblpv1, bShowVol);
    WGTSetVisible(ppg->subOutput_lblpv2, bShowVol);
    WGTSetVisible(ppg->subOutput_lblfv1, bShowVol);
    WGTSetVisible(ppg->subOutput_lblfv2, bShowVol);

    WGTSetVisible(ppg->subOutput_pv1sign, bShowVol);
    WGTSetVisible(ppg->subOutput_pv2sign, bShowVol);
    WGTSetVisible(ppg->subOutput_fv1sign, bShowVol);
    WGTSetVisible(ppg->subOutput_fv2sign, bShowVol);

    WGTSetVisible(ppg->subOutput_eddavol1, bShowVol);
    WGTSetVisible(ppg->subOutput_eddavol2, bShowVol);
    WGTSetVisible(ppg->subOutput_eddavol3, bShowVol);
    WGTSetVisible(ppg->subOutput_eddavol4, bShowVol);

    WGTSetVisible(ppg->subOutput_lblpv1_unit, bShowVol);
    WGTSetVisible(ppg->subOutput_lblpv2_unit, bShowVol);
    WGTSetVisible(ppg->subOutput_lblfv1_unit, bShowVol);
    WGTSetVisible(ppg->subOutput_lblfv2_unit, bShowVol);
}
/***********PageOnShowEnd********************************************************************************/


/***********DayProdStart********************************************************************************/
#define HOUR_0  TMPVAL(200)
static void DayHistogram()
{
    int i;
    UI32 year,month, day;
    UI32 per_hour;
    char date[20];

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    year = t->tm_year;
    month = t->tm_mon;
    day = t->tm_mday;

    for(i = 0; i < 24; ++i)
    {
        per_hour = g_G10ProdCnt.prod_cnt[month][day-1][i];
        VarAdrSetInt(HOUR_0+i, per_hour);
    }

    snprintf(date, sizeof(date), "%d-%d-%d", year+1900, month+1, day);
    LabelSetText(main_pg.subProduct_stDate, date);
}
/***********DayProdEnd********************************************************************************/

/***********OutputStart********************************************************************************/
//20220705 dyl
static void CmdAndSpeedState()
{
    char text[10];
    UI16 cmd_val;
    cmd_val = VarAdrToUI16(d_Pump1_STATE_MOTOR_SPEED_SET);
    if(cmd_val > 32768)
    {
        snprintf(text, sizeof(text), "%d", cmd_val - 65536);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", cmd_val);
    }
    EditSetText(main_pg.subOutput_stCmd1, text);

    cmd_val = VarAdrToUI16(d_Pump2_STATE_MOTOR_SPEED_SET);
    if(cmd_val > 32768)
    {
        snprintf(text, sizeof(text), "%d", cmd_val - 65536);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", cmd_val);
    }
    EditSetText(main_pg.subOutput_stCmd2, text);

    cmd_val = VarAdrToUI16(d_Pump3_STATE_MOTOR_SPEED_SET);
    if(cmd_val > 32768)
    {
        snprintf(text, sizeof(text), "%d", cmd_val - 65536);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", cmd_val);
    }
    EditSetText(main_pg.subOutput_stCmd3, text);

    cmd_val = VarAdrToUI16(d_Pump4_STATE_MOTOR_SPEED_SET);
    if(cmd_val > 32768)
    {
        snprintf(text, sizeof(text), "%d", cmd_val - 65536);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", cmd_val);
    }
    EditSetText(main_pg.subOutput_stCmd4, text);

    //20221025 dyl
    UI16 DA_val,DA_singal;
    DA_val = VarAdrToUI16(d_da1_STATE_DA_E700B_VAL1);
    DA_singal = VarAdrToUI16(d_da1_STATE_DA_E700B_DIR1);
    if(DA_singal == 1)
    {
        snprintf(text, sizeof(text), "-%d", DA_val);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", DA_val);
    }
    EditSetText(main_pg.subOutput_stexdap1, text);

    DA_val = VarAdrToUI16(d_da1_STATE_DA_E700B_VAL2);
    DA_singal = VarAdrToUI16(d_da1_STATE_DA_E700B_DIR2);
    if(DA_singal == 1)
    {
        snprintf(text, sizeof(text), "-%d", DA_val);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", DA_val);
    }
    EditSetText(main_pg.subOutput_stexdaf1, text);

    DA_val = VarAdrToUI16(d_da1_STATE_DA_E700B_VAL3);
    DA_singal = VarAdrToUI16(d_da1_STATE_DA_E700B_DIR3);
    if(DA_singal == 1)
    {
        snprintf(text, sizeof(text), "-%d", DA_val);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", DA_val);
    }
    EditSetText(main_pg.subOutput_stexdap2, text);

    DA_val = VarAdrToUI16(d_da1_STATE_DA_E700B_VAL4);
    DA_singal = VarAdrToUI16(d_da1_STATE_DA_E700B_DIR4);
    if(DA_singal == 1)
    {
        snprintf(text, sizeof(text), "-%d", DA_val);
    }
    else
    {
        snprintf(text, sizeof(text), "%d", DA_val);
    }
    EditSetText(main_pg.subOutput_stexdaf2, text);

    //20241205 chj 金鹰DA实际电压值显示
    if((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)
    {
        UI16 DA_VOL;
        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL1) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(main_pg.subOutput_eddavol1, text);
        LabelSetText(main_pg.subOutput_pv1sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL1)>>15 != 0) ? "-":" ");

        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL2) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(main_pg.subOutput_eddavol2, text);
        LabelSetText(main_pg.subOutput_fv1sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL2)>>15 != 0) ? "-":" ");

        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL3) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(main_pg.subOutput_eddavol3, text);
        LabelSetText(main_pg.subOutput_pv2sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL3)>>15 != 0) ? "-":" ");

        DA_VOL = (VarAdrToUI16(d_da1_STATE_DA_VOL_VAL4) & 0x7FFF);
        snprintf(text, sizeof(text)-1, "%4.1f", DA_VOL/10.);
        LabelSetText(main_pg.subOutput_eddavol4, text);
        LabelSetText(main_pg.subOutput_fv2sign,(VarAdrToUI16(d_da1_STATE_DA_VOL_VAL4)>>15 != 0) ? "-":" ");

    }


}
/***********OutputEnd********************************************************************************/

static void CtlConnect()
{
}
static int MainOnInit(Main_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int MainOnShow(Main_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetStr(0x3317003d,"btnMAINx.png");//主画面常态按钮为按下状态
    VarAdrSetInt(0x331700b9,0x0000FFFF);//主画面文本色为按下状态

    m_currentAction = 0;
    main_key = 0;
    SetMainCB(MainKeyCb);
    SubPageTempShow(ppg);
    MainWgtShow(ppg);
    G10MainGuideSwitch();
    return 0;
}
static int MainOnHide(Main_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetStr(0x3317003d,"btnMAIN.png");//主画面常态按钮恢复
    VarAdrSetInt(0x331700b9,0x00000000);//主画面文本色恢复

    return 0;
}
static int MainOnUpdate(Main_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 temp_part;
    static int temp_change = -1;
    temp_part = VarAdrToInt(TEMP_CTRL_PART);
    if(temp_change != temp_part)
    {
        main_key = 0;//20210914 dyl touch
        SubPageTempShow(ppg);
        MainWgtShow(ppg);
        G10MainGuideSwitch();
        temp_change = VarAdrToInt(TEMP_CTRL_PART);
    }

    ActCyclDisply();
    ActTime();
//    DayHistogram();//20210914 dyl touch
    TempRealShowUpdate();

    if(main_key == 0)
    {
        Machineshow();
        /*********** Touch start ***********/ //20210914 dyl
        ButtonSetReleasedImage(ppg->btio, IO_RELEASE_IMAGE);
        ButtonSetReleasedImage(ppg->btMachine, MACH_FOCUS_IMAGE );
        ButtonSetReleasedImage(ppg->btProduct, PRODUCT_RELEASE_IMAGE);
        ButtonSetReleasedImage(ppg->btOutput, OUTPUT_RELEASE_IMAGE);
        ButtonSetTextColor(ppg->btio, 0x000000);
        ButtonSetTextColor(ppg->btMachine, 0xffffff);
        ButtonSetTextColor(ppg->btProduct, 0x000000);
        ButtonSetTextColor(ppg->btOutput, 0x000000);
        /*********** Touch end   ***********/

        //20241028 chj 威力士主画面显示锁模力
        if(((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x7A00))
        {
            WGTSetVisible(ppg->subMach_lblLockPrs_unit,TRUE);
            WGTSetVisible(ppg->subMach_lblLockPrs,TRUE);
            WGTSetVisible(ppg->subMach_edlockprs,TRUE);

            if(VarAdrToInt(d_clamp1_STATE_DAPRES_CLSHI_MAX)>0 && VarAdrToInt(d_autoctrl1_MOLDSET_DAPRES_CLPLIMIT)>0)
            {
                VarAdrSetInt(TMP_LOCKPRS , ((UI32)(VarAdrToInt(d_clamp1_STATE_DAPRES_CLSHI_MAX)*VarAdrToInt(p_PP_MACHSET_LOCK_SCALE)/(VarAdrToInt(d_autoctrl1_MOLDSET_DAPRES_CLPLIMIT)*10))) );
            }
            else
            {
                VarAdrSetInt(TMP_LOCKPRS , 0);
            }
        }
        else {
            WGTSetVisible(ppg->subMach_lblLockPrs_unit,FALSE);
            WGTSetVisible(ppg->subMach_lblLockPrs,FALSE);
            WGTSetVisible(ppg->subMach_edlockprs,FALSE);
        }
    }
    /*********** Touch start ***********/ //20210914 dyl
    else if((main_key == 1) || (main_key == 2))
    {
        IO_InputShow();

        if(WGTGetVisible(main_pg.tabin) || (main_key == 1))
        {
            Input__UpdateNames(GetTableByType(TB_I_LOGIC, TB_CUR));
        }
        else
        {
            Output__UpdateNames(GetTableByType(TB_O_LOGIC, TB_CUR));
        }

        ButtonSetReleasedImage(ppg->btio, IO_FOCUS_IMAGE);
        ButtonSetReleasedImage(ppg->btMachine, MACH_RELEASE_IMAGE );
        ButtonSetReleasedImage(ppg->btProduct, PRODUCT_RELEASE_IMAGE);
        ButtonSetReleasedImage(ppg->btOutput, OUTPUT_RELEASE_IMAGE);
        ButtonSetTextColor(ppg->btio, 0xffffff);
        ButtonSetTextColor(ppg->btMachine, 0x000000);
        ButtonSetTextColor(ppg->btProduct, 0x000000);
        ButtonSetTextColor(ppg->btOutput, 0x000000);
    }
    else if(main_key == 3)
    {
        DayHistogram();
        ButtonSetReleasedImage(ppg->btio, IO_RELEASE_IMAGE);
        ButtonSetReleasedImage(ppg->btMachine, MACH_RELEASE_IMAGE );
        ButtonSetReleasedImage(ppg->btProduct, PRODUCT_FOCUS_IMAGE);
        ButtonSetReleasedImage(ppg->btOutput, OUTPUT_RELEASE_IMAGE);
        ButtonSetTextColor(ppg->btio, 0x000000);
        ButtonSetTextColor(ppg->btMachine, 0x000000);
        ButtonSetTextColor(ppg->btProduct, 0xffffff);
        ButtonSetTextColor(ppg->btOutput, 0x000000);
    }
    else if(main_key == 4)
    {
        CmdAndSpeedState();//20220705 dyl Output
        ButtonSetReleasedImage(ppg->btio, IO_RELEASE_IMAGE);
        ButtonSetReleasedImage(ppg->btMachine, MACH_RELEASE_IMAGE );
        ButtonSetReleasedImage(ppg->btProduct, PRODUCT_RELEASE_IMAGE);
        ButtonSetReleasedImage(ppg->btOutput, OUTPUT_FOCUS_IMAGE);
        ButtonSetTextColor(ppg->btio, 0x000000);
        ButtonSetTextColor(ppg->btMachine, 0x000000);
        ButtonSetTextColor(ppg->btProduct, 0x000000);
        ButtonSetTextColor(ppg->btOutput, 0xffffff);
    }
//    else if(main_key == 1)
//    {
//        IO_InputShow();
//        Input__UpdateNames(GetTableByType(TB_I_LOGIC, TB_CUR));
//    }
//    else if(main_key == 2)
//    {
//        Output__UpdateNames(GetTableByType(TB_O_LOGIC, TB_CUR));
//    }
//    else if(main_key == 4)//20220705 dyl Output
//    {
//        CmdAndSpeedState();
//    }
    /*********** Touch end   ***********/
    return 0;
}
static int MainOnChange(Main_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    /*********** Touch start ***********/ //20210914 dyl
    if(pwgt == ppg->btMachine)
    {
        MachInfoButton();
    }
    else if(pwgt == ppg->btio)
    {
        IOButton();
    }
    else if(pwgt == ppg->btProduct)
    {
        ProdCntButton();
    }
    else if(pwgt == ppg->btOutput)
    {
        OutputButton();
    }
    else if(pwgt == ppg->leftbt)
    {
        if(strcmp(ButtonGetText(ppg->btio), GetTextTran(TEXT_INPUT)))
        {
            TabPageSelPrev(ppg->tabout, FALSE);
        }
        else
        {
            TabPageSelPrev(ppg->tabin, FALSE);
        }

    }
    else if(pwgt == ppg->rightbt)
    {
        if(strcmp(ButtonGetText(ppg->btio), GetTextTran(TEXT_INPUT)))
        {
            TabPageSelNext(ppg->tabout, FALSE);
        }
        else
        {
            TabPageSelNext(ppg->tabin, FALSE);
        }
    }
    /*********** Touch end   ***********/
    return 0;
}
