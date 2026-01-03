#include "keydefine.h"
#include "database.h"
#include "oprintf.h"
#include "keydef.h"
#include "taskmoni.h"
#include "panelcfg.h"

//按键定义下拉表格对应值
UI16 key_define_list[] = {
    0,  //默认
    0x2013,     //开模
    0x2011,     //关模
    0x1011,     //射出
    0x1013,     //射退
    0x2113,     //托模退
    0x2111,     //托模进
    0x1012,     //加料
    0x101B,     //清料
    0x2212,     //中子出
    0x2211,     //中子入
    0,  //中子切换
    0x3001,     //润滑
    0x2311,     //吹气1-3-5
    0x2321,     //吹气2-4-6
    0,  //吹气切换
    0x0001,     //调模使用
    0x1111,     //座进
    0x1112,     //座退
    0x2412,     //调模退
    0x2411,     //调模进
    0x0411,     //马达
    0x0020,     //电热
    0   //自定义
};

/**
 * @brief      : 判断是否为相应键值
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20210118
 */
BOOL Judge_Define(UI16 num, KEY_DEFINE key_def)
{
    if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+num) == key_def)
    {
        return TRUE;
    }

    if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+num) != KEY_SELFDEFINE)
    {
        return FALSE;
    }

    //自定义发送储料键值即视为储料按键
    if((key_def < KEY_SELFDEFINE) && (key_def != KEY_DEFAULT) && (key_def != KEY_CORESELECT) && (key_def != KEY_AIRSELECT))
    {
        UI16 key_value = VarAdrToUI16(p_PP_MACHSET_SPARE1_VALUE+num);
        if(key_value == key_define_list[key_def])
        {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @brief      : 通过物理按键找到发送按键
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20210118
 */
UI16 KeyDefine_FindSendKey(UI32 oriKey)
{
    //20220805 dyl
    #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
    if(oriKey == _KEY_PANEL_SWX1)//备用1
    {
        return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY), 0);
    }
    else if(oriKey == _KEY_PANEL_SWX2)//备用2
    {
        return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE2_KEY), 1);
    }
    else if(oriKey == _KEY_PANEL_SWX3)//备用3
    {
        return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE3_KEY), 2);
    }
    else if(oriKey == _KEY_PANEL_SWX6)//备用4
    {
        return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE4_KEY), 3);
    }
    else if(oriKey == _KEY_PANEL_SWX7)//备用5
    {
        return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE5_KEY), 4);
    }
    else if(oriKey == _KEY_PANEL_SWX8)//备用6
    {
        return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE6_KEY), 5);
    }
    #else  //20220426
    if(PanelCfgSize()==DISPLAY_SIZE_15)//G15:KeyBoard
    {
        if(oriKey == _KEY_PANEL_PRINT)//备用1
        {
            return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY), 0);
        }
        else if(oriKey == _KEY_PANEL_CURVE)//备用2
        {
            return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE2_KEY), 1);
        }
        else if(oriKey == _KEY_PANEL_SW2)//备用3
        {
            return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE3_KEY), 2);
        }
        else if(oriKey == _KEY_PANEL_SW3)//备用4
        {
            return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE4_KEY), 3);
        }
    }
    else//G10
    {
        if(oriKey == _KEY_PANEL_SW2)//备用1
        {
            return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY), 0);
        }
        else if(oriKey == _KEY_PANEL_SW3)//备用2
        {
            return KeyDefine_GetSendKey(VarAdrToUI16(p_PP_MACHSET_SPARE2_KEY), 1);
        }
    }
    #endif

    return 0;
}

/**
 * @brief      : 通过按键选择值与备用键编号来获取发送键值
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20210118
 */
UI16 KeyDefine_GetSendKey(UI16 value, UI16 num)
{
    if(value == KEY_SELFDEFINE)
    {
        return VarAdrToUI16(p_PP_MACHSET_SPARE1_VALUE+num);
    }
    else
    {
        if(value < KEY_SELFDEFINE)
        {
            return key_define_list[value];
        }
    }

    return 0;
}

/**
 * @brief      : 备用键是否定义key_def
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20210118
 */
BOOL KeyDefine_Def(int cur_key, KEY_DEFINE key_def)
{
    //20220805 dyl
    #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
    if((cur_key == _KEY_PANEL_SWX1) && Judge_Define(0, key_def))
    {
        return TRUE;
    }
    else if((cur_key == _KEY_PANEL_SWX2) && Judge_Define(1, key_def))
    {
        return TRUE;
    }
    else if((cur_key == _KEY_PANEL_SWX3) && Judge_Define(2, key_def))
    {
        return TRUE;
    }
    else if((cur_key == _KEY_PANEL_SWX6) && Judge_Define(3, key_def))
    {
        return TRUE;
    }
    else if((cur_key == _KEY_PANEL_SWX7) && Judge_Define(4, key_def))
    {
        return TRUE;
    }
    else if((cur_key == _KEY_PANEL_SWX8) && Judge_Define(5, key_def))
    {
        return TRUE;
    }
    #else  //20220426
    if(PanelCfgSize()==DISPLAY_SIZE_15)//G15:KeyBoard
    {
        if((cur_key == _KEY_PANEL_PRINT) && Judge_Define(0, key_def))
        {
            return TRUE;
        }
        else if((cur_key == _KEY_PANEL_CURVE) && Judge_Define(1, key_def))
        {
            return TRUE;
        }
        else if((cur_key == _KEY_PANEL_SW2) && Judge_Define(2, key_def))
        {
            return TRUE;
        }
        else if((cur_key == _KEY_PANEL_SW3) && Judge_Define(3, key_def))
        {
            return TRUE;
        }
    }
    else//G10
    {
        if((cur_key == _KEY_PANEL_SW2) && Judge_Define(0, key_def))
        {
            return TRUE;
        }
        else if((cur_key == _KEY_PANEL_SW3) && Judge_Define(1, key_def))
        {
            return TRUE;
        }
    }
    #endif

    return FALSE;
}

/**
 * @brief      : 获取按键灯状态
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20210118
 */
BOOL KeyDefine_LED_State(UI16 num)
{
    BOOL state = FALSE;
    UI32 led = GetLED();

    if(Judge_Define(num, KEY_OPEN))
    {
        if(JudgeActStep(0x20130000, 0x2013FE01) || JudgeActStep(0x20170000, 0x2018FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_CLOSE))
    {
        if(JudgeActStep(0x20110000, 0x2012FE01) || JudgeActStep(0x20160000, 0x2016FE01)
                    || JudgeActStep(0x20190000, 0x2019FE01) || JudgeActStep(0x24180000, 0x2418FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_INJECT))
    {
        if(JudgeActStep(0x10110000, 0x1011FE01) || JudgeActStep(0x10140000, 0x1014FE01) || JudgeActStep(0x24160000, 0x2416FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_SUCKBACK))
    {
        if(JudgeActStep(0x10130000, 0x1013FE01) || JudgeActStep(0x10190000, 0x1019FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_EJECTRET))
    {
        if(JudgeActStep(0x21130000, 0x2113FE01) || JudgeActStep(0x21150000, 0x2115FE01) || JudgeActStep(0x24170000, 0x2417FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_EJECTADV))
    {
        if(JudgeActStep(0x21120000, 0x2112FE01) || JudgeActStep(0x21140000, 0x2114FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_CHARGE))
    {
        if(led & _LED_CHARGE)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AUTOPURGE))
    {
        if(led & _LED_AUTOPURGE)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_COREOUT))
    {
        if(JudgeActStep(0x22120000, 0x2212FE01) || JudgeActStep(0x22140000, 0x2214FE01)
                    || JudgeActStep(0x22220000, 0x2222FE01) || JudgeActStep(0x22240000, 0x2224FE01)
                    || JudgeActStep(0x22320000, 0x2232FE01) || JudgeActStep(0x22340000, 0x2234FE01)
                    || JudgeActStep(0x22420000, 0x2242FE01) || JudgeActStep(0x22440000, 0x2244FE01)
                    || JudgeActStep(0x22520000, 0x2252FE01) || JudgeActStep(0x22540000, 0x2254FE01)
                    || JudgeActStep(0x22620000, 0x2262FE01) || JudgeActStep(0x22640000, 0x2264FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_COREIN))
    {
        if(JudgeActStep(0x22110000, 0x2211FE01) || JudgeActStep(0x22130000, 0x2213FE01)
                    || JudgeActStep(0x22210000, 0x2221FE01) || JudgeActStep(0x22230000, 0x2223FE01)
                    || JudgeActStep(0x22310000, 0x2231FE01) || JudgeActStep(0x22330000, 0x2233FE01)
                    || JudgeActStep(0x22410000, 0x2241FE01) || JudgeActStep(0x22430000, 0x2243FE01)
                    || JudgeActStep(0x22510000, 0x2251FE01) || JudgeActStep(0x22530000, 0x2253FE01)
                    || JudgeActStep(0x22610000, 0x2261FE01) || JudgeActStep(0x22630000, 0x2263FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_CORESELECT))
    {
        if(led & _LED_CORESELECT)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_LUBRICATE))
    {
        if(led & _LED_LUBRICATION)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AIRBLOW135))
    {
        if(JudgeActStep(0x23110000, 0x2311FE01) || JudgeActStep(0x23310000, 0x2331FE01) || JudgeActStep(0x23510000, 0x2351FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AIRBLOW246))
    {
        if(JudgeActStep(0x23210000, 0x2321FE01) || JudgeActStep(0x23410000, 0x2341FE01) || JudgeActStep(0x23610000, 0x2361FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AIRSELECT))
    {
        if(led & _LED_AIRSELECT)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOLDADJ))
    {
        if(led & _LED_ADJMOLD)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_NOZZLEADV))
    {
        if(JudgeActStep(0x11110000, 0x1111FE01) || JudgeActStep(0x11130000, 0x1113FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_NOZZLERET))
    {
        if(JudgeActStep(0x11120000, 0x1112FE01) || JudgeActStep(0x11140000, 0x1114FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOLDTHICK))
    {
        if(JudgeActStep(0x24120000, 0x2412FE01) || JudgeActStep(0x24140000, 0x2414FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOLDTHIN))
    {
        if(JudgeActStep(0x24110000, 0x2411FE01) || JudgeActStep(0x24130000, 0x2413FE01))
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOTOR))
    {
        if(led & _LED_MOTORFLASH)
        {
            static UI32 time_300ms = 0;
            static BOOL bMotorFlash=FALSE;
            if(GetTick()-time_300ms >= 300)
            {
                time_300ms = GetTick();
                if(!bMotorFlash)
                {
                    bMotorFlash = TRUE;
                }
                else
                {
                    bMotorFlash = FALSE;
                }
            }
            state = bMotorFlash;
        }
        else if(led & _LED_MOTOR)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_HEATER))
    {
        if(led & _LED_HEATER)
        {
            state = TRUE;
        }
    }
    else if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+num) == KEY_SELFDEFINE)
    {
        if(VarAdrToUI16(p_PP_MACHSET_SPARE1_VALUE+num))
        {
            UI32 actstep1 = VarAdrToInt(p_PP_MACHSET_SPARE1_VALUE+num)<<16;
            UI32 actstep2 = VarAdrToInt(p_PP_MACHSET_SPARE1_VALUE+num)<<16 | 0xFFFF;
            if(JudgeActStep(actstep1, actstep2))
            {
                state = TRUE;
            }
        }
    }

    return state;
}

/**
 * @brief      : 获取按键灯状态 电容屏
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20210118
 */
BOOL KeyDefine_LED_State2(UI16 num)
{
    BOOL state = FALSE;
    UI32 led = GetLED();
    UI32 led2 = GetLED_2();

    if(Judge_Define(num, KEY_OPEN))
    {
        if(led & _LED_MOLDOPEN)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_CLOSE))
    {
        if(led & _LED_MOLDCLOSE)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_INJECT))
    {
        if(led & _LED_INJECT)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_SUCKBACK))
    {
        if(led & _LED_SUCKBACK)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_EJECTRET))
    {
        if(led & _LED_EJECTRET)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_EJECTADV))
    {
        if(led & _LED_EJECTADV)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_CHARGE))
    {
        if(led & _LED_CHARGE)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AUTOPURGE))
    {
        if(led & _LED_AUTOPURGE)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_COREOUT))
    {
        if(led & _LED_COREOUT)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_COREIN))
    {
        if(led & _LED_COREIN)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_CORESELECT))
    {
        if(led & _LED_CORESELECT)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_LUBRICATE))
    {
        if(led & _LED_LUBRICATION)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AIRBLOW135))
    {
        if(led2 & _LED_AIRBLASTMOV)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AIRBLOW246))
    {
        if(led2 & _LED_AIRBLASTSTAT)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_AIRSELECT))
    {
        if(led2 & _LED_AIRSELECT)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOLDADJ))
    {
        if(led2 & _LED_ADJMOLD)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_NOZZLEADV))
    {
        if(led & _LED_NOZZLEADV)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_NOZZLERET))
    {
        if(led & _LED_NOZZLERET)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOLDTHICK))
    {
        if(led2 & _LED_MOLDTHICK)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOLDTHIN))
    {
        if(led2 & _LED_MOLDTHIN)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_MOTOR))
    {
        if(led & _LED_MOTORFLASH)
        {
            static UI32 time_300ms = 0;
            static BOOL bMotorFlash=FALSE;
            if(GetTick()-time_300ms >= 300)
            {
                time_300ms = GetTick();
                if(!bMotorFlash)
                {
                    bMotorFlash = TRUE;
                }
                else
                {
                    bMotorFlash = FALSE;
                }
            }
            state = bMotorFlash;
        }
        else if(led & _LED_MOTOR)
        {
            state = TRUE;
        }
    }
    else if(Judge_Define(num, KEY_HEATER))
    {
        if(led & _LED_HEATER)
        {
            state = TRUE;
        }
    }
    else if(VarAdrToUI16(p_PP_MACHSET_SPARE1_KEY+num) == KEY_SELFDEFINE)
    {
        if(VarAdrToUI16(p_PP_MACHSET_SPARE1_VALUE+num))
        {
            UI32 actstep1 = VarAdrToInt(p_PP_MACHSET_SPARE1_VALUE+num)<<16;
            UI32 actstep2 = VarAdrToInt(p_PP_MACHSET_SPARE1_VALUE+num)<<16 | 0xFFFF;
            if(JudgeActStep(actstep1, actstep2))
            {
                state = TRUE;
            }
        }
    }

    return state;
}
