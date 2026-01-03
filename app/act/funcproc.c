#include "admeter.h"
#include "definelist.h"
#include "cfgdb.h"
#include "adjust.h"
#include "machine.h"
#include "EtherCATProc.h"

int GetPositoPulseVal(UI16 ch)
{
    PADMETER pad = &m_ad;
    return pad->positopulse[ch];
}
//获取位置脉冲值:由0.01mm精度的位置转换而来
int	GetPulsePos_2PmmPos(int CurPos_2Pmm, UI16 ch)
{
    PADMETER pad = &m_ad;
    return (CurPos_2Pmm * pad->positopulse[ch]);
}
//获取0.01mm精度的位置值:由位置脉冲值转换而来
int	Get2PmmPos_PulsePos(int CurPos_PulsePos, UI16 ch)
{
    PADMETER pad = &m_ad;
    return (CurPos_PulsePos / pad->positopulse[ch]);
}
//获取脉冲值:由1度精度的角度值转换而来
int	GetPulse_Angle(int angle, UI16 ch)
{
    PADMETER pad = &m_ad;
    return (angle * pad->positopulse[ch] / 360);

}
//获取转速(脉冲数/s):由RPM转速转换而来
int	GetSpdPulse_SpdRPM(WORD ServoNo, int ActSpdRPM)
{
    PADMETER pad = &m_ad;

    int ActSpdPulse = 0;
    ActSpdPulse = ActSpdRPM * ((I32)(1 << pad->mh->ENCODERESOLUTION[ServoNo]) / 60.0f);
    return (ActSpdPulse);
}
int GetSpdRPM_SpdPulse(WORD ServoNo, int ActSpdPulse)
{
    PADMETER pad = &m_ad;

    int ActSpdRPM = 0;
    ActSpdRPM = ActSpdPulse / ((I32)(1 << pad->mh->ENCODERESOLUTION[ServoNo]) / 60.0f);
    return (ActSpdRPM);
}
int GetSpd1Pmms_SpdRPM(WORD ServoNo, int ActSpdRPM)
{
    PADMETER pp = &m_ad;
    float Rpmto1Pmms;
    int   tmp_1Pmms;
    Rpmto1Pmms =  (float)pp->mh->ENCODECYCLENTH[ServoNo] * (float)pp->mh->SMALLROTARY_D[ServoNo] / ((float)pp->mh->BIGROTARY_D[ServoNo] * 10 * 60);
    tmp_1Pmms = ActSpdRPM * Rpmto1Pmms;
    //tmp_1Pmms = ActSpdRPM;
    return (tmp_1Pmms);
}
void GetPosiToPulse()		//计算0.01mm对应的编码器脉冲数
{
    PADMETER pp = &m_ad;
    UI16 ch;

    for (ch = 0; ch < ENCODE_CH_NUM - 1; ch ++)
    {
        //储料密封反转功能会用到，但是储料是没有丝杆的，所以在这里丝杆齿距默认1
        if (ch == CHARGEPORT)
        {
            pp->mh->ENCODECYCLENTH[ch] = 1;
        }

        pp->positopulse[ch] =
            (int)(((I32)(1 << pp->mh->ENCODERESOLUTION[ch]) * (float)pp->mh->BIGROTARY_D[ch] ) / ((float)pp->mh->ENCODECYCLENTH[ch] * (float)pp->mh->SMALLROTARY_D[ch]));
    }
}




//设置编码器归零，并通知驱动器归零完成
void SET_EncodeRZ(int RZ, WORD ch, BOOL IsSaveData)
{
    PADMETER pp = &m_ad;

    pp->mh->ENCODERZSETL[ch] = 0xFFFF & RZ;
    pp->mh->ENCODERZSETH[ch] = 0xFFFF & (RZ >> 16);

    if (IsSaveData)
    {
        if (pp->mh->POINTZERO_FLAG[ch] != 1)
        {
            pp->mh->POINTZERO_FLAG[ch] = 1;
        }

        if (GetSlaveZeroFlag(ch) == 0)
        {
            SdoDtDataUpdata(ch, 0x2000, 1, 0x0583, 16);
        }
        PartCfgSave((PPART)pp, MH_ID);
        PartDataTx((PPART)pp, MH_ID, CAL_LEN(pp->mh->ENCODERZSETL) * 12, GET_IND(PADMETER_MACHSET, ENCODERZSETL[0]), TRUE);
        PartDataTx((PPART)pp, MH_ID, CAL_LEN(pp->mh->POINTZERO_FLAG) * 6, GET_IND(PADMETER_MACHSET, POINTZERO_FLAG[0]), TRUE);
    }
    else
    {
        if (pp->mh->POINTZERO_FLAG[ch] == 1)
        {
            pp->mh->POINTZERO_FLAG[ch] = 0;
        }
        PartCfgSave((PPART)pp, MH_ID);
        PartDataTx((PPART)pp, MH_ID, CAL_LEN(pp->mh->POINTZERO_FLAG) * 6, GET_IND(PADMETER_MACHSET, POINTZERO_FLAG[0]), TRUE);
    }
}


//计算1RPM=x*0.1mm/s
float GetRPMtoMMSpd(UI8 ch)
{
    PADMETER pp = &m_ad;

    return (float)pp->mh->ENCODECYCLENTH[ch] * (float)pp->mh->SMALLROTARY_D[ch] / ((float)pp->mh->BIGROTARY_D[ch] * 10 * 60);
}

//转换 锁模力(1KN)转换为丝杆距离(0.01mm)
int GetClampPresToMM()
{
    PMACHINE pma = &m_machine;

    PADJUST padj = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
    int i, ret;
    int Press_Value = padj->mh->CLAMPCLSPRES;
    if (Press_Value > pma->mh->DAPRES_HIGHCLSLIMT)
    {
        Press_Value = pma->mh->DAPRES_HIGHCLSLIMT;
    }

    InitClsHighTbl();
    for (i = 0; i < 16; i++)
    {
        if (i < 15 && pma->mh->CLSHITB1[i + 1] != 0)
        {
            if (pma->mh->CLSHITB1[i] <= Press_Value && pma->mh->CLSHITB1[i + 1] > Press_Value)
            {
                ret = (int)((double)((Press_Value - pma->mh->CLSHITB1[i]) * (pma->mh->CLSHITB2[i + 1] - pma->mh->CLSHITB2[i])) / (double)(pma->mh->CLSHITB1[i + 1] - pma->mh->CLSHITB1[i])
                            + pma->mh->CLSHITB2[i]);
                return ret;
            }
        }
        else  //这里是锁模力表的最后一个数据
        {
            if (Press_Value >= pma->mh->CLSHITB1[i] ) //设定锁模力大于最大锁模力表，则直接取最大数据
            {
                return pma->mh->CLSHITB2[i];
            }
            /*if(pma->mh->CLSHITB1[i] <= Press_Value)
            {
            	ret = (int)((double)((Press_Value - pma->mh->CLSHITB1[i])*(pma->mh->CLSHITB2[i] - pma->mh->CLSHITB2[i-1]))/(double)(pma->mh->CLSHITB1[i] - pma->mh->CLSHITB1[i-1])
            	+pma->mh->CLSHITB2[i]);
            	return ret;
            }	*/
        }
    }


    return 0;
}

//根据当前丝杆位置(0.01mm)计算当前锁模力(1KN)
int GetClampMMToPres(int Line_D_Value)
{
    PMACHINE pma = &m_machine;
    int i, ret;

    InitClsHighTbl();
    for (i = 0; i < 16; i++)
    {
        if (i < 15 && pma->mh->CLSHITB2[i + 1] != 0)
        {
            if (pma->mh->CLSHITB2[i] <= Line_D_Value && pma->mh->CLSHITB2[i + 1] > Line_D_Value)
            {
                ret = (int)((double)((Line_D_Value - pma->mh->CLSHITB2[i]) * (pma->mh->CLSHITB1[i + 1] - pma->mh->CLSHITB1[i])) / (double)(pma->mh->CLSHITB2[i + 1] - pma->mh->CLSHITB2[i])
                            + pma->mh->CLSHITB1[i]);
                return ret;
            }
        }
        else
        {
            if (pma->mh->CLSHITB2[i] <= Line_D_Value)
            {
                ret = (int)((double)((Line_D_Value - pma->mh->CLSHITB2[i]) * (pma->mh->CLSHITB1[i] - pma->mh->CLSHITB1[i - 1])) / (double)(pma->mh->CLSHITB2[i] - pma->mh->CLSHITB2[i - 1])
                            + pma->mh->CLSHITB1[i]);
                return ret;
            }
        }
    }


    return 0;
}


/*------------------------------------------------------------*/
//	名称:均值滤波复用函数
//	功能:将Width个Value数据，去最大值最小值后取均值返回
//	涉及参数:
//	Value,实际数据
//	Width,滤波宽度
//	ValueBuff,实际数据保存空间
//	i,数据地址位移标记
//	作者:周拓
/*------------------------------------------------------------*/

int MeanFiltering(int Value, int Width, int* ValueBuff, int *i)
{
    int j, Rel = 0, width;
    int max, min;
    int valuebuff;
    if (*i >= 2000)
    {
        *i = 0;
    }
    ValueBuff[(Width + *i) % Width] = Value;
    max = min = Value;
    if (*i < 0)
    {
        for (j = 0; j < (Width); j++)
        {
            ValueBuff[j] = Value;
            Rel = Rel + ValueBuff[j];
        }
        width = Width;
    }
    else
    {
        for (j = *i; j > (*i - Width); j--)
        {
            valuebuff = ValueBuff[(Width + j) % Width];
            Rel = Rel + valuebuff;

            if (valuebuff > max)
            {
                max = valuebuff;
            }
            if (valuebuff < min)
            {
                min = valuebuff;
            }
        }
        width = Width - 2;
    }
    (*i)++;
    return (int)((float)(Rel - min - max) / (float)(width) + 0.5f);
}

/*------------------------------------------------------------*/
//	名称:接近值滤波复用函数
//	功能:在Width个Value数据中查找最接近上一个返回值的数据后返回。
//	涉及参数:
//	Value,实际数据
//	Width,滤波宽度
//	ValueBuff,实际数据保存空间
//	i,数据地址位移标记
//	lastvalue,上一个返回值，用于当前数据的比较
//	作者:周拓
/*------------------------------------------------------------*/

int MeanFiltering_delay(int Value, int Width, int* ValueBuff, I16 *i, int *lastvalue)
{
    int j, Rel = 0;
    int min;
    int valuebuff;
    if (*i >= 2000)
    {
        *i = 0;
    }
    ValueBuff[(Width + *i) % Width] = Value;
    Rel = Value;
    min = Value;
    if (*i < 0)
    {
        for (j = 0; j < (Width); j++)
        {
            ValueBuff[j] = Value;
            Rel = Rel + ValueBuff[j];
        }
    }
    else
    {
        for (j = *i; j > (*i - Width); j--)
        {
            int deff ;
            valuebuff = ValueBuff[(Width + j) % Width];

            deff = (valuebuff - *lastvalue);

            if (deff < min)
            {
                min = deff;

                Rel = valuebuff;
            }
        }
    }
    (*i)++;
    *lastvalue = Rel;
    return Rel;
}

/*------------------------------------------------------------*/
/*------------------------------------------------------------*/



//判断调模进是否到位
BOOL IsAdjFwdEnd()
{
    PADJUST pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
    PADMETER pad = &m_ad;
    //if(CheckPIUsed(pp->pi->I_ADJ_FWD))
    return MoveLocationNMControlProc(pp->pi->I_ADJ_FWD) || (pp->st->ADPOSI_ADJREAL <= pad->mh->ADPOSI_ADJMIN);
}

//判断调模退是否到位
BOOL IsAdjBwdEnd()
{
    PADJUST pp = (PADJUST)GetPartByID(MK_PART(ADJUST_ID, 1));
    PADMETER pad = &m_ad;
    //if(CheckPIUsed(pp->pi->I_ADJ_BWD))
    return MoveLocationNMControlProc(pp->pi->I_ADJ_BWD) || (pp->st->ADPOSI_ADJREAL >= pad->mh->ADPOSI_ADJMAX);
}
