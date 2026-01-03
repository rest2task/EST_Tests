/******************************************************************************
  文 件 名   : TempCtrlComm.c
  版 本 号   : 初稿
  作    者   : zhou tuo
  生成日期   : 2019年5月17日
  最近修改   :
  功能描述   : 通讯温度部件(E502)
  函数列表   :
              
  修改历史   :
  1.日    期   : 2019年5月17日
    作    者   : zhou tuo
    修改内容   : 创建文件
  2.日    期   : 2019年11月29日
    作    者   : zhou tuo
    修改内容   : 删除原tempctrl部件的dic，并合并tempctrlstru.c及tempctrl。
    			并将comm后缀删除。


******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "canhead.h"
#include "TempCtrlComm.h"
#include "sram.h"
#include "can_e502.h"
#include "systempro.h"
#include "machapi.h"
#include "oil.h" 
#include "alarm.h"
#include "cfgdb.h"
#include "Memtbl.h"
#include "stdlib.h"
#include "string.h"
#include "ZeroDebug.h"
#include "Hwmid.h"
#include "math.h"


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern OIL		  m_oil;

extern WORD tempuplimit[TEMP_MAX_NUM];
extern MOTOR	  m_motor;
extern WORD g_outtempuv_cn;
extern TEMP_STRU	  	  g_temp;
extern MACHINE m_machine;
extern UI8	Spd_buf_num ;
extern UI16 zerodata_tempzero[TEMP_CH_NUM], zerodata_tempb[TEMP_CH_NUM];

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
void RealOilSet(PTEMPCTRLCOMM pp);
void Adaptive(PTEMPCTRLCOMM pp, UI16 curchnl, float cur_temp, UI16 set_temp);

void InitTempStucPara(PTEMPCTRLCOMM pp, UI16 curchnl);
int getcurtemp(void);


/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
UI16 ChkPidIsRF(PTEMPCTRLCOMM pp);
UI16* GetCacheWord(UI16* psize);
void FreeCacheWord(void* pcache);
WORD ADtoTemp(WORD ADVALUE);
float ADtoTemp_float(WORD ADVALUE);


void Get_TempTest(PTEMPCTRLCOMM pp);
void Get_TempControl(PTEMPCTRLCOMM pp);
void WarmOn_Comm(PTEMPCTRLCOMM pp);
void WarmOff_Comm(PTEMPCTRLCOMM pp);

void SetheatS(PTEMPCTRLCOMM pp,int ch, TEMP_HEAT_STATE flag);

void InitP(PTEMPCTRLCOMM pp, UI16 curchnl);
void CalTempRamp(PTEMPCTRLCOMM pp, UI16 curchnl);
void LeastSquareMethod(PTEMPCTRLCOMM pp, UI16 curchnl, float Ak[], UI16 lenOfAk, float b, float beta); 
WORD TempWarmOnConsist(PTEMPCTRLCOMM pp);
void SaveIk0(PTEMPCTRLCOMM pp);
void UpdateTempPara(PTEMPCTRLCOMM pp);
void SaveTempPara(PTEMPCTRLCOMM pp);
void SaveAdpative(PTEMPCTRLCOMM pp);
void InitPidParaPro(PTEMPCTRLCOMM pp);
void GetCurTempUpSpeed(PTEMPCTRLCOMM pp, UI16 curchnl);
WORD FindFirstCurchnl(PTEMPCTRLCOMM pp);
void WarmOnNewRampSet(PTEMPCTRLCOMM pp);
void PIDControl(PTEMPCTRLCOMM pp, UI16 curchnl);
void GetKi0(PTEMPCTRLCOMM pp, UI16 curchnl);
void TurnToRamp(PTEMPCTRLCOMM pp);
WORD STO_PID_Need(PTEMPCTRLCOMM pp);
WORD STO_RAMP_Need(PTEMPCTRLCOMM pp);
void STOStart(PTEMPCTRLCOMM pp);
WORD STOEffChk(PTEMPCTRLCOMM pp);
void RestorePID(PTEMPCTRLCOMM pp);
WORD Ramp_Optimize_OK(PTEMPCTRLCOMM pp);
void SetTempChgNewRampSet(PTEMPCTRLCOMM pp, UI16 curchnl);
void SpecialPidRF(PTEMPCTRLCOMM pp,UI16 curchnl);
void Cal_Spd_Buf_Num(PTEMPCTRLCOMM pp);
void TempDropProtect(PTEMPCTRLCOMM pp, UI16 curchnl);
void init_ramp_estimate(PTEMPCTRLCOMM pp, UI16 curchnl);


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
UI8 Spd_buf_num = 50;

/*-----------------------------------------------------------------------------------+
|	常量定义 
|	采用K型热电偶,此标为每隔5摄氏度所对应的微伏表(0 - 595)																		 |
+------------------------------------------------------------------------------------*/
const WORD Tk_tab[120]=
{
	0, 198, 397, 597, 798,1000,1203,1407,1612,1817,		  // 0-45
	2023,2230,2437,2644,2851,3059,3267,3474,3682,3889,		  // 50-95
	4096,4303,4509,4715,4920,5124,5328,5532,5735,5937,		  // 100-145
	6138,6340,6540,6741,6941,7140,7340,7540,7739,7939,		  // 150-195
	8139,8338,8539,8739,8940,9141,9343,9545,9747,9950,		  // 200-245
	10153,10357,10561,10766,10971,11176,11382,11588,11795,12002, // 250-295
	12209,12416,12624,12832,13040,13248,13457,13665,13875,14084, // 300-345
	14293,14503,14713,14923,15133,15343,15554,15764,15975,16186, // 350-395
	16397,16608,16820,17031,17243,17455,17667,17879,18091,18303, // 400-445
	18516,18728,18941,19154,19366,19579,19792,20005,20218,20431, // 450-495
	20644,20857,21071,21284,21497,21710,21924,22137,22350,22563, // 500-545
	22776,22990,23203,23416,23629,23842,24055,24268,24480,24693 // 550-595
};

/*-----------------------------------------------------------------------------------+
|	常量定义 
|	采用J型热电偶,此标为每隔5摄氏度所对应的微伏表(0 - 595)																		 |
+------------------------------------------------------------------------------------*/

const WORD TJ_tab[] =
{	
	0,	253,	507,	762, 1019, 1277, 1537, 1797, 2059, 2322,  //  0-45度*****J热电偶温度-微伏表
	2585, 2850, 3116, 3382, 3650, 3918, 4187, 4456, 4726, 4997, // 50-95
	5269, 5541, 5814, 6087, 6360, 6634, 6909, 7184, 7459, 7734, //100-145
	8010, 8286, 8562, 8839, 9115, 9392, 9669, 9947,10224,10501, //150-195
	10779,11056,11334,11612,11889,12167,12445,12722,13000,13278,	//200-245
	13555,13833,14110,14388,14665,14942,15219,15496,15773,16050,	//250-295
	16327,16604,16881,17157,17434,17710,17986,18262,18538,18814,	//300-345
	19090,19366,19642,19918,20194,20469,20745,21021,21297,21572,	//350-395
	21848,22124,22400,22676,22952,23228,23504,23780,24057,24333,	//400-445
	24610,24887,25164,25442,25720,25998,26276,26555,26834,27113,	//450-495
	27393,27673,27953,28234,28516,28798,29080,29363,29647,29931,	//500-545
	30216,30502,30788,31074,31362,31650,31939,32229,32519,32810 	//550-595
};

//K型热电偶负温度对应微伏值表
WORD T_tabN[10] = {0, 182, 379, 575, 768, 960, 1150, 1337, 1523, 1706};	//0 - -45


const WORD* T_tab = Tk_tab;

WORD tempcoolinit[TEMP_MAX_NUM] = {0};
WORD tempuplimit[TEMP_MAX_NUM] = {0};
PTEMPCTRLCOMM	g_tempcomm[TEMP_MAX_NUM] = {NULL};
UI16	g_tempcomm_num = 0;

//UI16 TempCommModeTb[3][3] = 
//	{
//		{7,0x5020,10},
//		{6,0x5021,8},
//		{4,0x5060,6}
//	};


/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
BOOL TempIOForceErr()
{
    PTEMPCTRLCOMM pp;
    int i,j;

    for(i = 0;i < g_tempcomm_num;i++)
    {
        pp = g_tempcomm[i];
        for(j = 1;j < pp->mh->DATEMP_CH_NUM;j++)
        {
            if(CheckPO(pp->po->O_TEMOP[j]) == PO_SET && pp->st->DATEMP_REAL[j] >= 300 && pp->st->DATEMP_REAL[j] < MAXTEMP)
                return TRUE; //强制输出状态下，温度超过300度还输出电热时强制退出
        }
    }

    return FALSE;
}

/*****************************************************************************************
功    能: 加载出厂调试区温度归零值和温度校准值
说    明: 此功能使用时，7474画面温度归零和校准功能失效。
          如外面有使用需求，则需要屏蔽此功能。
修改原因: 5656配置导出时会导出温度归零值和温度校准值，若将导出的配置导入其他主机
          会覆盖参数，有可能导致温度检测不准。目前测试400度情况下最多差5度。
*****************************************************************************************/
void LoadZeroDataTemp()
{
    PTEMPCTRL ptmp = &m_temp;
    UI16 i;

    if(ptmp != NULL)
    {
        for(i = 0; i < TEMP_CH_NUM; i++)
        {
			ptmp->zd->WTEMP_CHZERO[i] = zerodata_tempzero[i];
			ptmp->zd->WTEMP_CHB[i] = zerodata_tempb[i];
        }
    }
}

BOOL ChkTempHighErr(PTEMPCTRLCOMM pp, UI16 curchnl) //温度超保护上限检测
{
    if(!ChkSysCfg(XF4_TMPLIMIT))
	{
		if (pp->st->DATEMP_REAL[curchnl] < MAXTEMP)
		{
			if(tempuplimit[1] < 350 || tempuplimit[1] > 525)
			{
				tempuplimit[1] = 400;
			}
		
			if(pp->st->DATEMP_REAL[curchnl] >= tempuplimit[1])
			{						
				SET_PART_ERR(pp, ER0_TEMPHIGH_COMM);
				StandAlarm(TRUE);
				if(GetHeatingStatus())
				{
					GetSys()->md->CUSTOM_PARA[0] = 8;
					WarmOff();
				}
			
				return TRUE;
			}
		}		
	}	
    return FALSE;
}

BOOL ChkTempMachTypeE50X(void)
{
    if (ChkSysCfg(XF0_ACCFL)||((GetHwModel() == MODEL_I3201) || (GetHwModel() == MODEL_I3202) || (GetHwModel() == MODEL_I5201)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void InitWarmFreeTime()
{
	PTEMPCTRLCOMM pp = (PTEMPCTRLCOMM)GetPartByID(MK_PART(TEMPCTRLCOMM_ID,1));

	if (pp->mh->FL_FREETIME_WARMOFF > 0 || pp->mh->TM_FREETIME_WARMOFF > 0)
	{
		pp->warm_freetime_start = GetSecTick();
		pp->warm_free_flag = 0;	

		if (pp->mh->FL_FREETIME_WARMOFF == 2)
		{
			if (pp->Holdwarm_flag == 1)
			{
				pp->Holdwarm_flag = 0;
				InitTempDataStu(pp);
			}
		}
	}
}


BOOL WarmFreeTempErrChk(PTEMPCTRLCOMM pc)
{
	int	j,err = 0;

	if(pc != NULL)
	{
		for(j = 0;j < pc->mh->DATEMP_CH_NUM;j++)
		{
			if(CHK_TEMP_AI_CTRL(pc->mh->DATEMP_FUNC[j]))
		    {
		        if (pc->md->DATEMP_SETTING[j] > pc->st->DATEMP_REAL[j])
		        {
		            if ((pc->md->DATEMP_SETTING[j] - pc->st->DATEMP_REAL[j]) > pc->md->DATEMP_MIN[j])
					{
						err = 1;
						break;
					}
		        }
		    }
		}
	}							

	if(err == 1)
		return TRUE;
	else		
		return FALSE;
}

void WarmFreeTimeChk()
{
	PTEMPCTRLCOMM pp = (PTEMPCTRLCOMM)GetPartByID(MK_PART(TEMPCTRLCOMM_ID,1));

	//功能关闭
	if (pp->mh->FL_FREETIME_WARMOFF == 0 || pp->mh->TM_FREETIME_WARMOFF == 0 || pp->warm_free_flag == 1 || pp->st->FL_HEATERSTATUS == 0 || WarmFreeTempErrChk(pp))
	{
		pp->warm_freetime_start = GetSecTick();
		pp->st->TM_COUNTDOWNTOWARM = pp->mh->TM_FREETIME_WARMOFF;
		return;
	}

	if (!ActTskIsEmpty() && !ActIsInTsk(MK_ACT(MOTOR_ID,1, OS_MOTON))&&!ActIsInTsk(MK_ACT(MOTOR_ID,1, OS_MOTOFF)))
	{	
		pp->warm_freetime_start = GetSecTick();
		pp->st->TM_COUNTDOWNTOWARM = pp->mh->TM_FREETIME_WARMOFF;
		pp->warm_free_flag = 1;
		return;
	}
	
	pp->st->TM_COUNTDOWNTOWARM = pp->mh->TM_FREETIME_WARMOFF - (((UI16)GetSecTick() - (UI16)pp->warm_freetime_start)/60);

	if (((UI16)GetSecTick() - (UI16)pp->warm_freetime_start)/60 >= pp->mh->TM_FREETIME_WARMOFF)
	{		
		pp->st->TM_COUNTDOWNTOWARM = 0;
		
		if (pp->mh->FL_FREETIME_WARMOFF == 1)
		{
			SET_PART_ERR(pp,ER5_WARMOFF);
			WarmOff();
		}
		else
		{
			pp->Holdwarm_flag = 1;
			InitTempDataStu(pp);
			SET_PART_ERR(pp,ER5_TEMP_TO_HOLD);
		}
	}


	if (pp->Holdwarm_flag == 1)
	{
		SET_PART_ERR(pp,ER5_TEMP_TO_HOLD);
	}
}



/*-----------------------------------------------------------------------------------+
|      关闭当前通道电热函数            ccc                                              |
+------------------------------------------------------------------------------------*/
void	ClosCurheat(PTEMPCTRLCOMM pp)
{	
	pp->tempctrldatastu.heatonsts[pp->curchnl]  =0;		
	pp->tempctrldatastu.temppwr[pp->curchnl].pk =0;

	if (pp->curchnl<TEMP_E502_CH_NUM_DIC)
	{
		SetPOBit(pp->po->O_TEMOP[pp->curchnl],PC_OFFD);
	}
}

void Get_Temp(BOOL bforce)
{	
	UI16 i;
	InitTempTkTj();

	if (g_tempcomm_num > 0)
    {
        for (i = 0; i < g_tempcomm_num; i++)
        {
            Get_TempControl(g_tempcomm[i]);
        }
    }	
}	

/*-----------------------------------------------------------------------------------+
|      温度转换成脉冲函数                                                            |
+-----------------------------------------------------------------------------------*/
unsigned	T2P(int temp)
{
  int i,j,pulse;
	
  if(temp<0)       temp =   0;
  else if(temp>MAX_TEMPSET) temp = MAX_TEMPSET;
	
  i = temp/5;
  j = temp%5;
	
  pulse = T_tab[i]+(T_tab[i+1]-T_tab[i])*j/5;
  return pulse;
}

void WarmOn()
{
  	int i;

  	if (g_tempcomm_num > 0)
    {
        for (i = 0; i < g_tempcomm_num; i++)
        {
            WarmOn_Comm(g_tempcomm[i]);
        }
    }
}

void WarmOff()
{
  	int i;

    for (i = 0; i < g_tempcomm_num; i++)
    {
    	if(g_tempcomm[i] != NULL)
    	{
			WarmOff_Comm(g_tempcomm[i]);
		}            
    } 
}

//=======================================================
// 功能: AD值查表得到温度值 -WD1011
//=======================================================
WORD ADtoTemp(WORD ADVALUE)
{
	WORD   i,imin,imax,lp,xtemp;
	DWORD  tp1,tp2;

	/*条件判读从> 改成>=,解决ADVALUE == T_tab[MAX_TEMPTBL]时，
	下面的while语句死循环的问题*/
	if(ADVALUE >= T_tab[MAX_TEMPTBL])
	{
	  	xtemp = 999;
	}
	else
	{
		imax = MAX_TEMPTBL;     //96
    	imin = 0;
    	i = imax / 2;   		//48
    	lp = 1;
    	while (lp)
    	{	  
  			if (T_tab[i] > ADVALUE)
 			{
   	  			imax = i;
  	  			i = imin + (imax-imin) / 2;
  			}
  			else if (T_tab[i+1] <= ADVALUE)
  			{
  	  			imin = i;
  	  			i = imin + (imax-imin)/2;
  			}	
  			else
  	  			lp = 0;
		}	
		tp1 = (WORD)(ADVALUE - T_tab[i]);
		tp2 = (WORD)(T_tab[i+1] - T_tab[i]);
	 	xtemp = (WORD)(((i * 50 + tp1 * 50 / tp2) + 5) / 10);   		//5度范围内插值补偿后得到温度 
	}
	
    return(xtemp);
}  

BOOL GetHeatingStatus()
{
	if(g_tempcomm_num > 0)
	{
		//第一个温度部件的状态代表所有温度部件
		return g_tempcomm[0]->st->FL_HEATERSTATUS;
	}
	
	return FALSE;
}

/*=============================================================*/  

/*******************************************************************************************
 *Brief			:手动清除温度优化状态
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190701
********************************************************************************************/
void InitManualTempRF(PTEMPCTRLCOMM pp)
{	
	if(pp->st->FL_HEATERSTATUS == FALSE)
	{
		pp->g_newtemp.tempRFFunc = 0;
		pp->st->DATEMP_YOUHUAXUQIU = 0;
		pp->st->DATEMP_YOUHUAOK = 0;
	}	
}


//首次开机初始化
void initNewTempPara(PTEMPCTRLCOMM pp)
{
    int i = 0;

    pp->g_newtemp.tempRFFunc = 0;

    for (i = 0; i < TEMP_E502_CH_NUM; i++)
    {
        pp->g_newtemp.tempRFMode[i] = 0;
        pp->g_newtemp.tempdata[i].RFend_flag = 0;
        pp->g_newtemp.temppwr[i].pk0  = 0;
    }
}

/*******************************************************************************************
 *Brief			:同步升温初始化
 *Param			:func 0:关闭，1:开启
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190131
********************************************************************************************/
void InitConstraint(PTEMPCTRLCOMM pp,WORD func)
{	
	UI16 ret = 0;
    if (func)
    {
        pp->g_newtemp.tempConstraintFunc = TRUE;
		ret = ChkPidIsRF(pp);
		if(ret == 0)
		{
			//未自学习 关闭同步升温 并提示
			if(pp->g_newtemp.tempRFFunc == 0)
			{
				SET_PART_ERR(pp,ER0_SYNCER_COMM);
			}
			
			pp->g_newtemp.tempConstraintFunc = FALSE;
		}
		else if(ret == 2)
		{
			//无PID控制 关闭同步升温
			pp->g_newtemp.tempConstraintFunc = FALSE;
		}
    }
    else
    {
        pp->g_newtemp.tempConstraintFunc = FALSE;
    }
}


/*******************************************************************************************
 *Brief			:开启电热时初始化温度数据
 *Param			:
 				ConstraintFunc:同步升温功能，0:关闭,1:开启
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190131
********************************************************************************************/
void InitNewTempByWarmOn(PTEMPCTRLCOMM pp,WORD ConstraintFunc)
{
    int iii = 0;
    if (pp->g_newtemp.tempRFFunc == 1)
    {
        pp->g_newtemp.temp_starttime_S = GetSecTick();

		if(pp->YouHuaFlag == 0x01)	//强制优化时
    		pp->st->DATEMP_YOUHUAOK = 0x02;
		else
			pp->st->DATEMP_YOUHUAOK = 0x01;
    }

    for (iii = 0; iii < TEMP_E502_CH_NUM; iii++)
    {
        pp->g_newtemp.fnextheatmodenew[iii] =  0;

        /*每次加热清除积分等数据*/
        pp->tempctrldatastu.tempdata[iii].last     = 0;
        pp->tempctrldatastu.tempdata[iii].test     = 0;
        pp->tempctrldatastu.heatcheck[iii].lastheatonsts = 0;
        pp->tempctrldatastu.heatonsts[iii] =  0;
        pp->tempctrldatastu.heatstat[iii]  =  0;

        pp->tempctrldatastu.temppwr[iii].ek  = 0;
        pp->tempctrldatastu.temppwr[iii].ek0 = 0;
        pp->tempctrldatastu.temppwr[iii].pk  = 0;
        pp->tempctrldatastu.temppwr[iii].ik  = 0;

        pp->g_newtemp.tempdata[iii].last0	 = 0;

		pp->cycleheat[iii].heatstatus = 0;
		pp->cycleheat[iii].heatoffstart = Get100MsTick();
		pp->st->FL_TEMPSTATUS[iii] = 0;
    }

	for(iii = pp->real_temp_ch_num;iii<TEMP_E502_CH_NUM;iii++)
	{
		pp->mh->DATEMP_FUNC[iii] = 0;
	}

    /*初始化约束升温功能*/
    InitConstraint(pp,ConstraintFunc);
}

/*******************************************************************************************
 *Brief			:检查PID是否有自学习过自学习过的温度各段PID参数会略有不同
 *Param			:
 *Return Value	:返回0表示未自学习 1 表示自学习过	2表示无PID控制
 *Aauthor		:ZT
 *Date			:20190627
********************************************************************************************/
UI16 ChkPidIsRF(PTEMPCTRLCOMM pp)
{
	UI16 i=0,j=0,num = 0;
	
	UI16 kp[TEMP_E502_CH_NUM] = {0};
	UI16 ti[TEMP_E502_CH_NUM] = {0};
	UI16 td[TEMP_E502_CH_NUM] = {0};
	UI16 ch[TEMP_E502_CH_NUM] = {0};
	
    for (j = 0; j < pp->real_temp_ch_num; j++)
    {

        if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[j]))
        {//表示使用该段使用PID控制
			kp[num] = pp->mh->DAPID_KP[j];
			ti[num] = pp->mh->DAPID_TI[j];
			td[num] = pp->mh->DAPID_TD[j];
			ch[num] = j;
			num++;
        }
	}

	//循环比较各段PID参数，只要其中有任意两段PID三个参数都相同的话，证明未进行过自学习
	for (j = 0; j < num; j++)
    {
     	for (i = j + 1;i < num;i++)
     	{
			if ((kp[j] == kp[i]) && (ti[j] == ti[i]) && (td[j] == td[i]))
			{
				return 0;
			}
		}
	}

	if(num < 2)
	{
		return 2;
	}
	else
	{
		double min_slope;

		min_slope = kp[0] * ti[0];
		pp->g_newtemp.min_chnl = ch[0];
		
		for (j = 1; j < num; j++)
	    {
		    if ((kp[j] * ti[j]) > min_slope)
	        {
	            min_slope = kp[j] * ti[j];
	            pp->g_newtemp.min_chnl = ch[j];
	        }
		}
		return 1;
	}
}


void TempRFStop(PTEMPCTRLCOMM pp)
{
    initNewTempPara(pp);
    if (pp->PRF_para_Comm != NULL)
    {
        FreeCacheWord(pp->PRF_para_Comm);
        pp->PRF_para_Comm = NULL;
    }
    CLR_PART_ERR(pp, ER0_STDY_COMM);
}


/*******************************************************************************************
 *Brief			:温度优化开启，满足条件就开启优化功能
 *Param			:password 强制优化密码
 *Return Value	:是否成功 TRUE成功	FALSE优化开启失败
 *Aauthor		:ZT
 *Date			:20190301
********************************************************************************************/
WORD TempRFStart(PTEMPCTRLCOMM pp,UI16 password)
{
    UI16 max = 0, min = 10000;
    WORD iii = 0;
    UI16 permission = TRUE;
		UI16 tempopen = FALSE;
		UI16 Err_State = 0;		//温度错误状态，用于区分不符合哪种条件

	if (ChkSysCfg(CO9_RSV3))
	{
		return 1;
	}

    if (pp->st->FL_HEATERSTATUS == FALSE)
    {
        for (iii = 0; iii < pp->real_temp_ch_num; iii++)
        {
			if(CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
            {
            	tempopen = TRUE;
				
                pp->g_newtemp.tempRFMode[iii] = 1;

                //实际温度小于60度，才能够开始自学习
                if (pp->st->DATEMP_REAL[iii] > VALIDCHKTEMPRF)
                {
                    permission = FALSE;
					Err_State = 1;
                }

                //设定温度需要在150摄氏度以上，才能够开始自学习
                if (pp->md->DATEMP_SETTING[iii] < TEMP_MINSETTING)
                {
                    permission = FALSE;
					Err_State = 2;
                }

                if (pp->st->DATEMP_REAL[iii]> max)
                {
                    max = pp->st->DATEMP_REAL[iii];
                }
                if (pp->st->DATEMP_REAL[iii] < min)
                {
                    min = pp->st->DATEMP_REAL[iii];
                }
            }
        }

		if(!tempopen)
		{
			return 0;
		}

		if(permission && ((FORCERFPASSWORD == password) || ((max < (pp->st->WTEMP_LM73 / 10 + 5)) && (ABS_DIFF(max, min) < 5))))
        {
        	if(password == FORCERFPASSWORD)
				pp->YouHuaFlag = 0x01;
			else
				pp->YouHuaFlag = 0x00;
			
        	pp->st->DATEMP_YOUHUAXUQIU = 1;	//优化条件满足后，提示面板开启电热
            pp->g_newtemp.tempRFFunc = 1;

			//初始化缓冲区模型相关参数
			for (iii = 0; iii < pp->real_temp_ch_num; iii++)
        	{
				if(CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
				{
					InitTempStucPara(pp, iii);
				}
			}
			
            SET_PART_ERR(pp, ER0_STDY_COMM);
			
			CLR_PART_ERR(pp, ER0_TEMP_REAL_HIGH);
			CLR_PART_ERR(pp, ER0_TEMP_SET_LOW);

			return 0;
        }
        else
        {
            TempRFStop(pp);
            //SET_PART_ERR(pp, ER0_STDYER_COMM);
			
			if(Err_State == 2)
				SET_PART_ERR(pp, ER0_TEMP_SET_LOW);
			else
				SET_PART_ERR(pp, ER0_TEMP_REAL_HIGH);

			return 1;
        }
    }
    else
    {
    	pp->st->DATEMP_YOUHUAOK = 0x0;
        SET_PART_ERR(pp, ER0_STDYER_COMM);
		return 1;
    }
}

void TempTempRFStartPro(WORD password,WORD id,WORD ch)
{
	int i = 0;

	for(i = 0;i < TEMP_E502_CH_NUM;i++)
	{
		if((id & (0x01 << i)) > 0)
		{
			if(i >=g_tempcomm_num)
			{//当传入id中部件号大于配置id数时，停止
				break;
			}
			
			if(TempRFStart(g_tempcomm[i],password) == 1)
			{
				for(;i>=0;i--)
				{
					TempRFStop(g_tempcomm[i]);
				}

				break;
			}
		}
	}		
}
/*-----------------------------------------------------------------------------------+
|		电热连续加热升温检测函数     ccc                                        |
+------------------------------------------------------------------------------------*/
void CheckTempheat(PTEMPCTRLCOMM pp,int channel, int temp)
{
	if (pp->tempctrldatastu.g_tempchk[channel].tempreset != 1)    //只有首次加温做检查 08-5-30		//YF
		return;
	
    if (pp->tempctrldatastu.heatonsts[channel] != pp->tempctrldatastu.heatcheck[channel].lastheatonsts)
    {
        pp->tempctrldatastu.heatcheck[channel].lastheatonsts = pp->tempctrldatastu.heatonsts[channel];
        if (pp->tempctrldatastu.heatonsts[channel] == 3)
        {
            pp->tempctrldatastu.heatcheck[channel].lasttime = GetSecTick();
            pp->tempctrldatastu.heatcheck[channel].lasttemp = temp;
        }
    }
    else if ((pp->tempctrldatastu.heatonsts[channel] == 3) && (pp->mh->FL_TEMPWORDUPCHECK == 1)
        &&(pp->mh->DA_THERMOCOUPLE_PRT == 0))//电热连续加温检查选择使用，且为0时，使用老逻辑
    {
        if (delt_time(GetSecTick(), pp->tempctrldatastu.heatcheck[channel].lasttime) > TEMPCHECKTIME)
        {
            if ((temp - pp->tempctrldatastu.heatcheck[channel].lasttemp) < 120)
            {
				SET_PART_ERR(pp, ER0_HTON_COMM);
                //WarmOff_Comm(pp);
                GetSys()->md->CUSTOM_PARA[0] = 7;
				WarmOff();
				SetheatS(pp,channel,TEMP_ST_ERR);
            }
            pp->tempctrldatastu.heatcheck[channel].lasttemp = temp;
            pp->tempctrldatastu.heatcheck[channel].lasttime = GetSecTick();
        }
    }
}





/*单次计算温度加温缓存区大小并发送给面板保存*/
void inttempchknew(PTEMPCTRLCOMM pp,UI16 curchnl)        /////rrrr
{
    int deltt;

    if (pp->g_newtemp.tempdata[curchnl].dek < 0)
    {
        deltt = (int)pp->st->DATEMP_REAL[curchnl]- ADtoTemp(pp->PRF_para_Comm[curchnl].start_temp + T2P(TEMPRF_UP));
        if ((pp->g_newtemp.tempRFFunc == 1) || (pp->tempctrldatastu.g_tempchk[curchnl].tempreset == 1))
        {
            if (deltt <= 5 )
            {
                deltt = 5;
            }

			pp->mh->DATEMP_RAMP[curchnl] = deltt;
			PartDataTx((PPART)pp, MH_ID, 1, GET_IND(PTEMPCOMM_MACHSET, DATEMP_RAMP) + curchnl, FALSE); 
        }
    }
}


/*******************************************************************************************
  * @函数名称	NewConstraintFunction()
  * @函数说明   在全功率加热阶段，限制温度功率输出函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  * @作者       马亚萍
  * @日期       2019.01.25
 ****************************************************************************************/

void NewConstraintFunction(PTEMPCTRLCOMM pp,WORD curchnl)
{
    double efficent = 0;
 
    pp->g_newtemp.temppwr[curchnl].e = pp->tempctrldatastu.tempdata[curchnl].set - pp->tempctrldatastu.tempdata[curchnl].test;
    pp->g_newtemp.temppwr[curchnl].slope = pp->tempctrldatastu.tempdata[curchnl].test - pp->g_newtemp.tempdata[curchnl].last0;

    if (pp->g_newtemp.temppwr[curchnl].slope > 0)
    {
        pp->g_newtemp.temppwr[curchnl].time = pp->g_newtemp.temppwr[curchnl].e / pp->g_newtemp.temppwr[curchnl].slope;
    }

    if ((curchnl != pp->g_newtemp.min_chnl) && (pp->g_newtemp.temppwr[pp->g_newtemp.min_chnl].slope > 0))
    {
        if (pp->tempctrldatastu.tempdata[curchnl].test < pp->tempctrldatastu.tempdata[curchnl].set - T2P(15))
        {

            if ((!pp->g_newtemp.tempRFFunc) && (pp->g_newtemp.temppwr[curchnl].slope > 0))
            {

                efficent  = ((double)(pp->g_newtemp.temppwr[curchnl].time)) / ((double)(pp->g_newtemp.temppwr[pp->g_newtemp.min_chnl].time));


                if (efficent > 8.0)
                {
                    efficent = 8.0;
                }

                if (pp->tempctrldatastu.tempdata[curchnl].test < pp->tempctrldatastu.tempdata[curchnl].ramp)
                {
                    pp->tempctrldatastu.temppwr[curchnl].pk = efficent * pp->g_newtemp.temppwr[curchnl].pk0 * pp->tempctrldatastu.temppwr[curchnl].pk;
                    pp->g_newtemp.temppwr[curchnl].pk0 = efficent;
                }
                else
                {
                    pp->tempctrldatastu.temppwr[curchnl].pk = efficent * pp->tempctrldatastu.temppwr[curchnl].pk;
                }

            }
            else
            {
                pp->tempctrldatastu.temppwr[curchnl].pk = pp->tempctrldatastu.temppwr[curchnl].pk;
            }
        }
        else
        {
            pp->tempctrldatastu.temppwr[curchnl].pk = pp->tempctrldatastu.temppwr[curchnl].pk;
        }

    }
    else
    {

        if (curchnl == pp->g_newtemp.min_chnl)
        {
            pp->tempctrldatastu.temppwr[curchnl].pk = pp->tempctrldatastu.temppwr[curchnl].pk;
        }
        else if ((pp->tempctrldatastu.tempdata[pp->g_newtemp.min_chnl].test > pp->tempctrldatastu.tempdata[pp->g_newtemp.min_chnl].ramp) 
                 && (pp->g_newtemp.tempdata[pp->g_newtemp.min_chnl].dek_filt < 5))
        {
            pp->tempctrldatastu.temppwr[curchnl].pk = pp->tempctrldatastu.temppwr[curchnl].pk;
        }
        else
        {
            if (pp->tempctrldatastu.tempdata[curchnl].test > (pp->tempctrldatastu.tempdata[curchnl].set - T2P(15)))
            {
                pp->tempctrldatastu.temppwr[curchnl].pk = pp->tempctrldatastu.temppwr[curchnl].pk;
            }
            else if (pp->tempctrldatastu.tempdata[curchnl].test > pp->tempctrldatastu.tempdata[curchnl].ramp)
            {
                pp->tempctrldatastu.temppwr[curchnl].pk = pp->tempctrldatastu.temppwr[curchnl].pk;
            }
            else
            {
                pp->tempctrldatastu.temppwr[curchnl].pk = 0.1 * pp->tempctrldatastu.temppwr[curchnl].pk;

            }
        }
    }

    pp->g_newtemp.tempdata[curchnl].last0 = pp->tempctrldatastu.tempdata[curchnl].test;
}


void initcurculpid(PTEMPCTRLCOMM pp,UI16 curchnl)
{
    int kp = 0, kd = 0;
	float cur_temp;
	int set_temp;
	WORD flag_into_pid;
	
	cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp;
	set_temp = pp->md->DATEMP_SETTING[curchnl];
    kp = pp->tempctrldatastu.pidpara[curchnl].p;
    kd = pp->tempctrldatastu.pidpara[curchnl].kd;

    switch (pp->g_newtemp.fnextheatmodenew[curchnl])
    {
        case 0:
            //判断是否可以修改
            if ((pp->st->DATEMP_REAL[curchnl] < VALIDCHKTEMPRF)
                    && (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[curchnl])|| (TEMPRAMPMODE == 2)))
            {
                pp->tempctrldatastu.g_tempchk[curchnl].tempreset =  1;
            }
            else
            {
                pp->tempctrldatastu.g_tempchk[curchnl].tempreset =  0;
            }
            pp->g_newtemp.fnextheatmodenew[curchnl] = 1;
            break;
        case 1:
            //1、全速加热   
            if ((pp->tempctrldatastu.tempdata[curchnl].ramp_mode == 0 && pp->tempctrldatastu.tempdata[curchnl].test <= pp->tempctrldatastu.tempdata[curchnl].ramp - RAMP_BIAS * 40) 
				|| (pp->tempctrldatastu.tempdata[curchnl].ramp_mode > 0 && pp->tempctrldatastu.tempdata[curchnl].flag_reachRamp == 0))
            {
                pp->tempctrldatastu.temppwr[curchnl].pk = 10000;
                pp->tempctrldatastu.heatonsts[curchnl]  = 3;

                pp->tempctrldatastu.tempdata[curchnl].lasttest = pp->tempctrldatastu.tempdata[curchnl].test;
                pp->g_newtemp.tempdata[curchnl].dek_start = pp->g_newtemp.tempdata[curchnl].dek_filt;
            }
            //2、PID控制加热  大于RAMP区，小于警戒温度
            else
            {
                pp->g_newtemp.fnextheatmodenew[curchnl] = 2;
                pp->tempctrldatastu.temppwr[curchnl].pk = 0;
            }
            break;
        case 2:
            pp->tempctrldatastu.temppwr[curchnl].pk = 0;
            pp->tempctrldatastu.heatonsts[curchnl] = 0;

			flag_into_pid = FALSE; //判断是否进入PID

			//老程序模式，dek_filt < 10进入PID；新程序控制时，需在5度内才dek_filt < 10进入
			if (pp->tempctrldatastu.tempdata[curchnl].ramp_mode == 0) 
			{
				if (pp->g_newtemp.tempdata[curchnl].dek_filt < 10)
					flag_into_pid = TRUE;
			}
			else //新程序控制
			{
				if (cur_temp < set_temp - 5.0f) //当前温度<(设定温度-5)时，dek<0时进入PID
				{
					if (pp->g_newtemp.tempdata[curchnl].dek < TEMP_UP_SPEED_LOWER_LIMIT)
					{
						flag_into_pid = TRUE;
						if ((pp->tempctrldatastu.tempdata[curchnl].ramp_mode == 2) && (pp->tempctrldatastu.tempdata[curchnl].flag_adaptive))
						{	//拐点已出现，需要自适应
							pp->tempctrldatastu.tempdata[curchnl].flag_adaptive = 0;	
							Adaptive(pp, curchnl, cur_temp, set_temp); //自适应
							pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0; //不进行前馈积分
						}
					}
				}
				else //当前温度>(设定温度-5)时，dek_filt<10时进入PID
				{
					if (pp->g_newtemp.tempdata[curchnl].dek_filt < 10)
						flag_into_pid = TRUE; //拐点还未出现，需在后面继续判断拐点
				}		
			}

            if (flag_into_pid) //进入PID
            {	
				//if (TEMPRAMPMODE > 1)
                //{
                //    inttempchknewRt(pp,curchnl);//如果希望缓冲区与原模式相同，每次加热都学习缓冲区
                //}

                //分段积分模式
                if (CHK_TEMP_PD_CTRL(pp->mh->DATEMP_FUNC[curchnl]))
                {
                    /*PD控制降低过冲*/
                    pp->g_newtemp.fnextheatmodenew[curchnl] = 5;
                }
                else
                {
                    /*正常控制 直接进入PID*/
                    pp->g_newtemp.fnextheatmodenew[curchnl] = 4;
					pp->tempctrldatastu.tempdata[curchnl].ki_temp = (int)(cur_temp);
					pp->tempctrldatastu.tempdata[curchnl].ki_time =  GetSecTick();
                }

                pp->g_newtemp.tempdata[curchnl].time_state = GetSecTick();	//记录离开本状态的时间，也就是进入下一个状态的时间
            }
            break;
        case 3:
        case 4:
			//自适应，如果是dek_filt<10进入的PID，说明温度还会继续升高，需要继续判断拐点
			if ((pp->tempctrldatastu.tempdata[curchnl].ramp_mode == 2)
				&& (pp->tempctrldatastu.tempdata[curchnl].flag_adaptive)
				&& (pp->g_newtemp.tempdata[curchnl].dek < TEMP_UP_SPEED_LOWER_LIMIT))
			{
				pp->tempctrldatastu.tempdata[curchnl].flag_adaptive = 0;
				Adaptive(pp, curchnl, cur_temp, set_temp); //自适应
			}
			
            if ((pp->g_newtemp.tempRFFunc) || (pp->tempctrldatastu.ramp_effective) || (pp->tempctrldatastu.pid_effective))
			{
				//do nothing.
			}
			else
			{
	            PIDControl(pp,curchnl); //PID控制
				GetKi0(pp,curchnl); //前馈积分量学习
			}
            break;
        case 5:
            //case 5 为PD控制
            kp = 0.8 * kp;	//减小kp
            kd = 1.2 * kd;	//增加kd
            pp->tempctrldatastu.temppwr[curchnl].pk = (kp * (int)pp->tempctrldatastu.temppwr[curchnl].ek + 50) / 100 +
                                                (kd * (int)(pp->tempctrldatastu.temppwr[curchnl].ek - pp->tempctrldatastu.temppwr[curchnl].ek0) + 50) / 100;

            //离开PD控制的条件
            //条件1，误差为负，说明PD控制已超出设定值
            if (pp->tempctrldatastu.temppwr[curchnl].ek < 0)
            {
                pp->g_newtemp.fnextheatmodenew[curchnl] = 4;
            }

            //条件2，在进入PD三分钟以后，如果ek不为负不会离开PD控制
            if (delt_time(GetSecTick(), pp->g_newtemp.tempdata[curchnl].time_state) > 240)
            {
                if (pp->g_newtemp.tempdata[curchnl].dek_filt < 3)
                {
                    pp->g_newtemp.fnextheatmodenew[curchnl] = 4;
                }                
            }
            //条件3，20分钟后强制离开状态5，进入PID控制，防止离不开这个状态出现稳态误差
            if (delt_time(GetSecTick(), pp->g_newtemp.tempdata[curchnl].time_state) > 1200)
            {
                pp->g_newtemp.fnextheatmodenew[curchnl] = 4;
            }

			if (pp->tempctrldatastu.temppwr[curchnl].pk == 0)
            {
                pp->tempctrldatastu.heatonsts[curchnl] = 0;
            }
            else
            {
                pp->tempctrldatastu.heatonsts[curchnl] = 1;
            }

            break;
        case 6:
            //case 6 为PD控制
            kp = 0.8 * kp;	//减小kp
            kd = 1.2 * kd;	//增加kd
            if (pp->tempctrldatastu.tempdata[curchnl].set - pp->g_newtemp.tempdata[curchnl].temp_last_state > 60)
            {
                pp->tempctrldatastu.temppwr[curchnl].ek = (2 * pp->tempctrldatastu.tempdata[curchnl].set - pp->g_newtemp.tempdata[curchnl].temp_last_state - pp->tempctrldatastu.tempdata[curchnl].test - 60);
            }
            else
            {
                pp->tempctrldatastu.temppwr[curchnl].ek = (2 * pp->tempctrldatastu.tempdata[curchnl].set - pp->g_newtemp.tempdata[curchnl].temp_last_state - pp->tempctrldatastu.tempdata[curchnl].test);
            }
            pp->tempctrldatastu.temppwr[curchnl].pk = (kp * (int)pp->tempctrldatastu.temppwr[curchnl].ek + 50) / 100 +
                                                (kd * (int)(pp->tempctrldatastu.temppwr[curchnl].ek - pp->tempctrldatastu.temppwr[curchnl].ek0) + 50) / 100;

            //离开PD控制的条件
            //条件1，误差为负，说明PD控制已超出设定值
            if (pp->tempctrldatastu.temppwr[curchnl].ek < 0)
            {
                pp->g_newtemp.fnextheatmodenew[curchnl] = 4;
            }
            //条件2，在进入PD五分钟以后，如果ek不为负不会离开PD控制
            if (delt_time(GetSecTick(), pp->g_newtemp.tempdata[curchnl].time_state) > 300)
            {
                if (pp->g_newtemp.tempdata[curchnl].dek_filt < 1)
                {
                    pp->g_newtemp.fnextheatmodenew[curchnl] = 4;
                }
            }
            //条件3，20分钟后强制离开状态5，进入PID控制，防止离不开这个状态出现稳态误差
            if (delt_time(GetSecTick(), pp->g_newtemp.tempdata[curchnl].time_state) > 1200)
            {
                pp->g_newtemp.fnextheatmodenew[curchnl] = 4;
            }

			if (pp->tempctrldatastu.temppwr[curchnl].pk == 0)
            {
                pp->tempctrldatastu.heatonsts[curchnl] = 0;
            }
            else
            {
                pp->tempctrldatastu.heatonsts[curchnl] = 1;
            }
            break;
        default:
            break;

    }

	//如果在正常加热过程中，温度低于缓冲区，使用全功率加热。PID继续累积
	TempDropProtect(pp,curchnl);
	
    if (pp->tempctrldatastu.tempdata[curchnl].test >= (pp->tempctrldatastu.tempdata[curchnl].set + pp->tempctrldatastu.errmax))
    {
        pp->tempctrldatastu.temppwr[curchnl].pk  = 0;
        pp->tempctrldatastu.heatonsts[curchnl]   = TEMP_ST_OVERTOP;
		SetheatS(pp,curchnl, TEMP_ST_OVERTOP);
    }
	else
	{
		CLR_TMP_ST(pp,curchnl, TEMP_ST_OVERTOP);
	}
	
    //>>>>>-------------以下代码为控制器输出限幅----------
    if (pp->tempctrldatastu.temppwr[curchnl].pk > 10000)
    {
        pp->tempctrldatastu.temppwr[curchnl].pk = 10000;
    }
    if (pp->tempctrldatastu.temppwr[curchnl].pk < 0)
    {
        pp->tempctrldatastu.temppwr[curchnl].pk = 0;
    }
    //---------------2018.09.25LJT-------------------<<<<<

}


WORD GetMaxKDT(PTEMPCTRLCOMM pp,WORD curchnl, WORD num)
{
    UI8 iii = 0;
    WORD ret = FALSE;

    if (pp->PRF_para_Comm == NULL)
    {
        return ret;
    }

    for (iii = 0; iii < num; iii++)
    {
        if (pp->PRF_para_Comm[curchnl].KDT_current_max < pp->PRF_para_Comm[curchnl].KDT_current[iii])
        {
            pp->PRF_para_Comm[curchnl].KDT_current_max = pp->PRF_para_Comm[curchnl].KDT_current[iii];
            ret = TRUE;
        }
    }

    return ret;
}


void CalB(PTEMPCTRLCOMM pp,WORD curchnl, WORD x, WORD y)
{
    if (pp->PRF_para_Comm != NULL)
    {
        pp->PRF_para_Comm[curchnl].b_current = y - pp->PRF_para_Comm[curchnl].KDT_current_max * x;
    }
}


void CalPID(PTEMPCTRLCOMM pp,WORD curchnl)
{
    fp32 Kp, Ti, Td, Tc;
    fp32 kdt;//K/T
    fp32 tao;//τ滞后时间
    fp32 b;//	y = kx + b;
	
    if (pp->PRF_para_Comm != NULL)
    {
        kdt = pp->PRF_para_Comm[curchnl].KDT_current_max;
        b = pp->PRF_para_Comm[curchnl].b_current;
        tao = (pp->PRF_para_Comm[curchnl].start_temp - b) / kdt;

        if (tao < TEMP_MINTAO) //滞后时间最小4s
        {
            tao = TEMP_MINTAO;
        }
        if (kdt > TEMP_MAXK) //斜率最高90
        {
            kdt = TEMP_MAXK;
        }

		Tc = pp->tempcs[curchnl].chnlctltm_rel;
		
        //ZN公式推导pid
        Kp = 12000 / (kdt * tao);
        Ti = tao * 2;
        Td = tao / 2;

        pp->tempctrldatastu.pidpara[curchnl].p = Kp * 100;
        pp->tempctrldatastu.pidpara[curchnl].ki = (Kp * Tc / Ti) * 100;
        pp->tempctrldatastu.pidpara[curchnl].kd = (Kp * Td / Tc) * 100;

#ifdef HYDR_TEMP
		pp->mh->DAPID_KP[curchnl] = Kp * 10;
		pp->mh->DAPID_TI[curchnl] = Ti * 10;
		pp->mh->DAPID_TD[curchnl] = Td * 10;
#else
		pp->mh->DAPID_KP[curchnl] = Kp * 10 * 10;
		pp->mh->DAPID_TI[curchnl] = Ti * 10 / 0.6f;
		pp->mh->DAPID_TD[curchnl] = Td * 10 / 0.6f;
#endif
    }
}

/*******************************************************************************************
 *Brief			:1、温度读完一轮后上传警报
 				 2、当温度警报发生变化时，清楚当前温度警报重新上传警报，用于记录不同的温度警报
 *Param			:ch:当前扫描的温度段数
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191121
********************************************************************************************/
void TempErrPro(PTEMPCTRLCOMM pp,UI16 ch)
{
	UI16 err_quadrature = 0;	//当前警报积数
	
	if (pp->err_cyclic_start == 0)
	{
		pp->err_ch = ch;
		pp->err_cyclic_start = 1;
	}
	else if (pp->err_cyclic_start > 0)
	{
		if (pp->err_ch == ch)
		{	
			//段读取两个循环来确定温度警报，并上传警报信息。
			if(((pp->err_manual == 1)&&(pp->err_cyclic_start == 2)) || ((pp->err_manual == 0)&&(pp->err_cyclic_start >= 1)))
			{
				for (int i = 0; i < pp->real_temp_ch_num; i++)
				{
					err_quadrature = err_quadrature + (pp->st->FL_TEMPSTATUS[i]>>11);
				}

				if (CHK_PART_ERR(pp,ER1_TEMP_ERRPRO))
				{
					if (err_quadrature != pp->err_quadrature_last)
					{
						pp->err_quadrature_last = err_quadrature;
						CLR_PART_ERR(pp,ER1_TEMP_ERRPRO);	
						return;
					}
					else
					{
						PartDataTx((PPART)pp, ST_ID, TEMP_E502_CH_NUM, GET_IND(PTEMPCTRLCOMM_STATE, FL_TEMPSTATUS), FALSE);			
						SET_PART_ERR(pp,ER1_TEMP_ERRPRO);
					}
				}
				else
				{
					pp->err_quadrature_last = err_quadrature;
					PartDataTx((PPART)pp, ST_ID, TEMP_E502_CH_NUM, GET_IND(PTEMPCTRLCOMM_STATE, FL_TEMPSTATUS), FALSE); 		
					SET_PART_ERR(pp,ER1_TEMP_ERRPRO);
				}
				
				pp->err_cyclic_start = 0;
				pp->err_manual = 0;
			}	
			else
			{
				pp->err_cyclic_start = 2;
			}
		}
	}	
}

/*-----------------------------------------------------------------------------------+
|		电热开关状态函数                                                     |
+------------------------------------------------------------------------------------*/
void SetheatS(PTEMPCTRLCOMM pp,int ch, TEMP_HEAT_STATE flag)
{    
	UI16 MAX_CH = 0;
	
	if(ChkTempMachTypeE50X())		//用温度采集扩展板时，最大通道按照扩展板的数目来
		MAX_CH = TEMP_E502_CH_NUM;
	else
		MAX_CH = MAX_TEMPCHNL;		//当用主板上的温度时，走主板的最大通道号
	
	if(ch < MAX_CH)
	{
		if(CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[ch]) || ((CHK_TEMP_PCT_CTRL(pp->mh->DATEMP_FUNC[ch]))&&(flag!=TEMP_ST_ERR)))
		{
			CLR_TMP_ST_8BIT(pp,ch);
			SET_TMP_ST(pp,ch,flag);

			if(flag == TEMP_ST_ERR)
			{
				switch (pp->st->DATEMP_REAL[ch]) 
				{
					case 777:
					case 7777:
						SET_TMP_ST(pp,ch,TEMP_ST_777);
						break;
					case 788:
					case 7888:
						SET_TMP_ST(pp,ch,TEMP_ST_788);
						break;
					case 999:
					case 9999:
						SET_TMP_ST(pp,ch,TEMP_ST_999);
						break;
					default:
						break;
				}			
			}
		}	
	}	
}

/*-----------------------------------------------------------------------------------+
|		电热开关函数          0-OFF 1-ON   ccc                                  |
+------------------------------------------------------------------------------------*/
void On_Offheat(PTEMPCTRLCOMM pp,int channel, int flag)
{
    if ((channel >= 0) && (channel < TEMP_E502_CH_NUM))
    {
		if (flag)
        {
            SetPOBit(pp->po->O_TEMOP[channel], PC_ON);
        }
        else
        {
            SetPOBit(pp->po->O_TEMOP[channel], PC_OFFD);
        }
    }
}

/*-----------------------------------------------------------------------------------+
|		温度通道功率控制                                                     |
+------------------------------------------------------------------------------------*/
void   starttempctrl(PTEMPCTRLCOMM pp,UI16 curchnl)
{
    if (!CHK_TEMP_CLS(pp->mh->DATEMP_FUNC[curchnl]))
    {
        if (pp->tempctrldatastu.temppwr[curchnl].pk <= 70)
        {
            /*零功率关闭电热*/
			if(!CHK_TMP_ST(pp,curchnl,TEMP_ST_OVERTOP))
			{
				SetheatS(pp,curchnl, TEMP_ST_ZERO);
            	pp->tempctrldatastu.heatstat[curchnl]     = 0;
			}
			
            On_Offheat(pp,curchnl, 0); 
            pp->st->OUTPUT_PER[curchnl] = 0;
        }
        else if (pp->tempctrldatastu.temppwr[curchnl].pk > 9930)
        {
            /*全功率开电热*/			
            On_Offheat(pp,curchnl, 1);
            SetheatS(pp,curchnl, (TEMP_HEAT_STATE)pp->tempctrldatastu.heatonsts[curchnl]);
            pp->tempctrldatastu.heatstat[curchnl]     = 1;

            pp->st->OUTPUT_PER[curchnl] = 1000;
        }
        else
        {
            /*PID控制加热*/
            pp->tempctrldatastu.On_Time[curchnl]  = (pp->tempcs[curchnl].chnlctltm_rel * pp->tempctrldatastu.temppwr[curchnl].pk) / 10;

            On_Offheat(pp,curchnl, 1);
            SetheatS(pp,curchnl, (TEMP_HEAT_STATE)pp->tempctrldatastu.heatonsts[curchnl]);
            pp->tempctrldatastu.heatstat[curchnl] = 1;

            pp->st->OUTPUT_PER[curchnl] = pp->tempctrldatastu.temppwr[curchnl].pk / 10;
        }
    }
    else
    {
        pp->st->OUTPUT_PER[curchnl] = 0;
    }
}

void YouHuaBreakChk(PTEMPCTRLCOMM pp)
{
	if(ChkSysCfg(XF4_SAVC2))
		return;
	
	if(pp->st->DATEMP_YOUHUAOK == 1 || pp->st->DATEMP_YOUHUAOK == 2)
	{
		pp->LastYouHuaFlag = 1;			//优化过程中把一个标记置1表示进入过优化
	}
	else if(pp->st->DATEMP_YOUHUAOK == 3 || pp->st->DATEMP_YOUHUAOK == 4)
	{
		if(!CHK_PART_ERR(pp,ER5_YOUHUAFINISH) && (pp->LastYouHuaFlag == 1))
		{
			SET_PART_ERR(pp,ER5_YOUHUAFINISH);
			pp->Tm_YouHuaFinish = Get100MsTick();
		}
	}

	if(pp->LastYouHuaFlag == 1)
	{
		if(pp->st->DATEMP_YOUHUAOK == 0)		//优化过程中被中断了就提示优化中断
		{
			pp->LastYouHuaFlag = 0;
			SET_PART_ERR(pp,ER5_YOUHUABREAK);
			StandAlarm(TRUE);
		}
	}

	if(CHK_PART_ERR(pp,ER5_YOUHUAFINISH))		//优化完成提示5秒后自动清除并把标记置2
	{
		if((UI16)(Get100MsTick()-pp->Tm_YouHuaFinish) > 0)
		{
			CLR_PART_ERR(pp,ER5_YOUHUAFINISH);
			pp->LastYouHuaFlag = 2;				
		}
	}
}

void TempControlCore(PTEMPCTRLCOMM pp)
{
	UI16 curchnl = pp->curchnl;

    pp->g_newtemp.tempdata[curchnl].dek = pp->tempctrldatastu.tempdata[curchnl].test - pp->tempctrldatastu.tempdata[curchnl].lasttest;
    //新加的温度上升速度惯性滤波
    pp->g_newtemp.tempdata[curchnl].dek_filt = 0.8 * pp->g_newtemp.tempdata[curchnl].dek + 0.2 * pp->g_newtemp.tempdata[curchnl].dek_filt;

    //下面为原来的温度上升速度滤波
    pp->tempctrldatastu.temppwr[curchnl].ek = (pp->tempctrldatastu.tempdata[curchnl].set - pp->tempctrldatastu.tempdata[curchnl].test);

	if(ChkTempHighErr(pp, curchnl))
        return;

    if (pp->st->FL_HEATERSTATUS)
    {		
        if (pp->st->DATEMP_REAL[curchnl] >= MAXTEMP)
        {
        	GetSys()->md->CUSTOM_PARA[0] = 9;
			ClosCurheat(pp);
			SetheatS(pp,pp->curchnl,TEMP_ST_ERR);
        }
        else
        {
            initcurculpid(pp,curchnl);//PID控制函数

            /*在自学习时不使用同步升温*/
            if (!pp->g_newtemp.tempRFFunc && pp->g_newtemp.tempConstraintFunc)
            {
                NewConstraintFunction(pp,curchnl);	//同步升温函数
            }

            SpecialPidRF(pp,curchnl);	//强制自学习函数
            starttempctrl(pp,curchnl);	//电热输出函数	

			CheckTempheat(pp,curchnl, pp->tempctrldatastu.tempdata[curchnl].test);	//连续加热升温检测函数
        }
    }
	else
	{
		SetheatS(pp,curchnl, TEMP_ST_ZERO);
	}

    pp->tempctrldatastu.temppwr[curchnl].ek0 = pp->tempctrldatastu.temppwr[curchnl].ek;

    pp->tempctrldatastu.tempdata[curchnl].lasttest = pp->tempctrldatastu.tempdata[curchnl].test;
}
 
BOOL TempErrStByCh(PTEMPCTRLCOMM pp)
{
	UI16 ch = 0;
	
	if(pp == NULL)
	{
	//如果指针空，则查看下一个部件地址
		return FALSE;
	}

	ch = pp->curchnl;

	if(CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[ch]))
    {
        if (pp->md->DATEMP_SETTING[ch] > pp->st->DATEMP_REAL[ch])
        {
            return (pp->md->DATEMP_SETTING[ch] - pp->st->DATEMP_REAL[ch] > pp->md->DATEMP_MIN[ch]);
        }
        else
        {
            return (pp->st->DATEMP_REAL[ch] - pp->md->DATEMP_SETTING[ch] > pp->md->DATEMP_MAX[ch]);
        }
    }
		
		return FALSE;
}


BOOL TempPreWarmErrChk ()
{
	UI16 i = 0,err = 0;
	PTEMPCTRLCOMM pp;
	
	for(i=0;i<g_tempcomm_num;i++)
	{
		pp = g_tempcomm[i];
		
		if(pp == NULL)
		{
		//如果指针空，则查看下一个部件地址
			continue;
		}
		

	    if (pp->md->TM_HEATERWARM > 0 &&  pp->md->TM_HEATERWARM < 31 && pp->CoolCheck.Checked < 2)
	    {
	        SET_PART_ERR(pp, ER0_WUPT_COMM);
			err = 1;
	    }
	}

	if(err == 1)
	{
	    return TRUE;
	}

    return FALSE;
}


BOOL TempIsHoldMoldErrChk ()
{
	UI16 i = 0;
	PTEMPCTRLCOMM pp;

	for(i=0;i<g_tempcomm_num;i++)
	{
		pp = g_tempcomm[i];
		
		if(pp == NULL)
		{
		//如果指针空，则查看下一个部件地址
			continue;
		}
		
		if (pp->md->FL_TEMPHOLD == 1 || pp->Holdwarm_flag == 1)
	    {
	        SET_PART_ERR(pp, ER0_TMPHLD_COMM);
	        return TRUE;
	    }
	}   

    return FALSE;
}

//心跳包异常，则每段都报警779
void CanTempNodeErr(WORD i)
{
    UI16 j=0;
	PTEMPCTRLCOMM pp;

	if(i <= g_tempcomm_num)
	{
		pp = g_tempcomm[i-1];
		
		if(pp == NULL)
		{
		//如果指针空，则查看下一个部件地址
			return;
		}
		for (j=0;j < pp->real_temp_ch_num; j++)
		{
			if(!CHK_TEMP_CLS(pp->mh->DATEMP_FUNC[j]))
			{
				pp->st->DATEMP_REAL[j] = 779;
				SetheatS(pp,j,TEMP_ST_ERR);
			}
		}	

		if(i == 1)
		{
			m_oil.st->DATEMP_OIL = 779;
		}
        else if(i == 9)
		{
			m_oil.st->DATEMP_OIL2 = 779;
		}
		else if (i == g_tempcomm_num)
		{
			if(pp->fl_motor_temp)
			{
				m_motor.st->DATEMP_REALMOTOR = 779;
			}
		}
	}   
}

void InitThermocouple(PTEMPCTRLCOMM pp)//热电偶呆滞保护相关参数的初始化
{
    int k,i;
    
    pp->step_10s = 0;
    pp->tm_start_10s = 0;

    for(k=0;k<TEMPCHKTM;k++)
    {
        for(i=0;i<TEMP_E502_CH_NUM;i++)
        {
            pp->temp_themocp_chk[k][i] = 0;
        }
    }
    
    for(i=0;i<TEMP_E502_CH_NUM;i++)
    {
         if(!CHK_PART_ERR(pp, ER0_HTON_COMM))
         {
            CLR_TMP_ST(pp,i,TEMP_ST_NUP);   
         }
         pp->temp_ch_chk[i] = 1;
    }

}


/*******************************************************************************************
 *Brief			:对温度控制的内存数据进行初始化
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190412
********************************************************************************************/
void InitTempRamData(PTEMPCTRLCOMM pp)
{
	UI16 i;
    initNewTempPara(pp);
    for (i = 0; i < TEMP_E502_CH_NUM; i++)
    {			
        pp->tempctrldatastu.tempdata[i].last     = 0;
        pp->tempctrldatastu.tempdata[i].test     = 0;
		
        pp->tempctrldatastu.heatcheck[i].lastheatonsts = 0;
        pp->tempctrldatastu.heatonsts[i] =  0;
        pp->tempctrldatastu.heatstat[i]  =  0;

        pp->tempctrldatastu.temppwr[i].ek  = 0;
        pp->tempctrldatastu.temppwr[i].ek0 = 0;
        pp->tempctrldatastu.temppwr[i].pk  = 0;
        pp->tempctrldatastu.temppwr[i].ik  = 0;
    }
    pp->tempstep = 0;

	tempcoolinit[pp->temp_id] = 1;
}

/*******************************************************************************************
 *Brief			:计算温度的扫描周期及控制周期
 *Param			:温度部件指针
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191120
********************************************************************************************/
void InitCyclicData(PTEMPCTRLCOMM pp)
{
	UI16 i;
    //根据温度功能选项，统计温度使用段数。
    pp->temprealchnl = 0;
	
    for (i = 0; i < pp->real_temp_ch_num; i++)
    {
        if (!CHK_TEMP_CLS(pp->mh->DATEMP_FUNC[i]))
        {
            pp->temprealchnl++;
        }

		if(!(GetSys()->mh->CD_TYPE == 1 || GetSys()->mh->CD_TYPE == 4))
		{//如果是F519的IO扩展卡，可以运行6秒以下的温度输出。而如果是F518的继电扩展卡输出周期不能太短，防止继电器寿命果过短
			if(pp->mh->TM_RESPONSELESSCYCLE[i] < 6)
			{
				pp->mh->TM_RESPONSELESSCYCLE[i] = 6;
			}
		}
    }

    for (i = 0; i < pp->real_temp_ch_num; i++)
    {
        pp->tempcs[i].chnlscantm = 0;
        if (!CHK_TEMP_CLS(pp->mh->DATEMP_FUNC[i]))
        {
            //根据设置的最小的扫描周期，来计算每一段温度平均扫描时长。所有段的扫描时长总和等于最小扫描周期-20180820 zt
            pp->tempcs[i].chnlscantm = (WORD)(pp->mh->DATEMP_SCTM_MIN * 1000 / pp->temprealchnl);
            //根据最小扫描周期及，设定的控制周期，计算出控制周期于扫描周期的倍数-20180820 zt
            pp->tempcs[i].chnlctlcn = (pp->mh->TM_RESPONSELESSCYCLE[i] * 2 + pp->mh->DATEMP_SCTM_MIN) / (pp->mh->DATEMP_SCTM_MIN * 2);

			if (pp->tempcs[i].chnlctlcn < 1) //倍数最小为1，即控制周期等于扫描周期-20180820 zt
            {
                pp->tempcs[i].chnlctlcn = 1;
            }
			
			pp->tempcs[i].chnlctltm_rel = pp->mh->DATEMP_SCTM_MIN*pp->tempcs[i].chnlctlcn;
        }
    }

	Cal_Spd_Buf_Num(pp); //根据扫描周期确定用于升温速率计算的buffer个数
}


/*******************************************************************************************
 *Brief			:初始化温度PID
 *Param			:温度部件指针
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191120
********************************************************************************************/
void InitPidParaPro(PTEMPCTRLCOMM pp)
{
    int     i;
    float   TC;
    float   Td = 2.25 / 5;
    float   Ti = 13.25 / 2 / 2;
    float   Kp = 0.735 * 2;    

    for (i = 0; i < pp->real_temp_ch_num; i++)
    {
        //面板pid显示一位小数点
#ifdef HYDR_TEMP
		Kp = pp->mh->DAPID_KP[i] / 10.0f;
		Ti = pp->mh->DAPID_TI[i] / 10.0f;
		Td = pp->mh->DAPID_TD[i] / 10.0f;
#else
		Kp = pp->mh->DAPID_KP[i] / (10.0f * 10.0f);
		Ti = pp->mh->DAPID_TI[i] * 0.6f / 10.0f;
		Td = pp->mh->DAPID_TD[i] * 0.6f / 10.0f;
#endif
		TC = pp->tempcs[i].chnlctltm_rel;
		
        //100倍的数据放大，提高精度
        pp->tempctrldatastu.pidpara[i].p = Kp * 100;
        pp->tempctrldatastu.pidpara[i].ki = (Kp * TC / Ti) * 100;
        pp->tempctrldatastu.pidpara[i].kd = (Kp * Td / TC) * 100;
    }
}


/*-----------------------------------------------------------------------------------+
	温度测试初始化函数
	1、检测所有的温度RAMP确定需要控制的温度通道数及列表
	2、确定各段温度控制使用的PID值
	3、初始化温度采样时间、控制时间、第一次采样通道号
+------------------------------------------------------------------------------------*/
void   InitTempDataStu(PTEMPCTRLCOMM pp)
{
    int  i;

	pp->tempctrldatastu.errmax = T2P(5);		

    for (i = 0; i < pp->real_temp_ch_num; i++)
    {
    	pp->tempctrldatastu.g_tempchk[i].tempreset =  0;
		
        pp->tempctrldatastu.tempdata[i].set  = pp->md->DATEMP_SETTING[i];		      				// 初始化温度设定值
        pp->tempctrldatastu.tempdata[i].ramp = pp->mh->DATEMP_RAMP[i];		      				// 初始化RAMP
		
		SetTempChgNewRampSet(pp, i); //缓冲区模式选择

        if (pp->md->FL_TEMPHOLD == 1 || pp->Holdwarm_flag == 1)
        {
            pp->tempctrldatastu.tempdata[i].set = pp->md->DATEMP_HOLD ;  	// 保温模式 06-10-6
        }
		
		pp->tempctrldatastu.tempdata[i].ramp = T2P(pp->tempctrldatastu.tempdata[i].set - pp->tempctrldatastu.tempdata[i].ramp);
		pp->tempctrldatastu.tempdata[i].set	= T2P(pp->tempctrldatastu.tempdata[i].set);

        if (CHK_TEMP_NOCTRL(pp->mh->DATEMP_FUNC[i]) || (pp->md->DATEMP_SETTING[i] > MAX_TEMPSET))
        {
            pp->tempctrldatastu.tempdata[i].ramp = 0;
            pp->tempctrldatastu.tempdata[i].set  = 0;
			pp->tempctrldatastu.heatstat[i] = 0;
			
            On_Offheat(pp, i, 0);
            SetheatS(pp, i, TEMP_ST_ZERO);
        }
    }
}

/*******************************************************************************************
 *Brief			:温度数据初始化
 *Param			:温度部件指针
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191120
********************************************************************************************/
void InitTemp(PTEMPCTRLCOMM pp)
{
	UI16 i = 0;

	pp->tempstep  = 0;
    Temp_cnt_init();
	pp->cn_temp_cool = 0;
	pp->cn_oil_temp = 0;
	pp->cn_motor_temp = 0;
	pp->fl_cool_temp = FALSE;
	pp->fl_motor_temp = FALSE;
	pp->fl_oil_temp = FALSE;

	if (pp->temp_id == 1)
	{
		m_motor.fl_motor_temp = FALSE;
	}	
	
	for (i = 0; i < TEMP_E502_CH_NUM; i++)
    {
		if ((ChkSysCfg(CO9_XIND)) && (i == g_outtempuv_cn))
		{
			pp->mh->DATEMP_FUNC[i] = TEMP_FUNC_ONLYSHOW;
		}
	
    	if (i >= pp->real_temp_ch_num)
    	{
			pp->mh->DATEMP_FUNC[i] = TEMP_FUNC_CLOSE;
		}
    	if(CHK_TEMP_NOCTRL(pp->mh->DATEMP_FUNC[i]))
    	{	
			pp->st->OUTPUT_PER[i] = 0;
			SetPOBit(pp->po->O_TEMOP[i], PC_OFF);
		}
    	if(CHK_TEMP_CLS(pp->mh->DATEMP_FUNC[i]))
    	{
			pp->cycleheat[i].heatstatus  = 0;
			pp->st->FL_TEMPSTATUS[i] = 0;
			pp->st->DATEMP_REAL[i] = 0;
		}    

		if((pp->CoolCheck.Checked == 3) && CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[i]))
		{
			pp->CoolCheck.Checked = 0;
		}

		if (pp->mh->DATEMP_FUNC[i] == TEMP_FUNC_MOTOR)
		{
			pp->cn_motor_temp = i;
			pp->fl_motor_temp = TRUE;
			m_motor.fl_motor_temp = TRUE;
		}

		if (pp->mh->DATEMP_FUNC[i] == TEMP_FUNC_CHGCOOL)
		{
			pp->cn_temp_cool = i;
			pp->fl_cool_temp = TRUE;
		}

		if (pp->mh->DATEMP_FUNC[i] == TEMP_FUNC_OIL)
		{
			pp->cn_oil_temp = i;
			pp->fl_oil_temp = TRUE;
		}
    }

	if((pp->temp_id == 1) )
	{
	//第一块E502的第一段为油温
		if(pp->cn_oil_temp > 0)
		{
			pp->mh->DATEMP_FUNC[0] = TEMP_FUNC_CLOSE;
		}
		else
		{
			pp->mh->DATEMP_FUNC[0] = TEMP_FUNC_OIL;
		}
		
		pp->fl_oil_temp = TRUE;
		
		if(m_oil.mh->DATEMP_OILMAX == 0)
		{
			pp->st->DATEMP_REAL[pp->cn_oil_temp] = 0;
			m_oil.st->DATEMP_OIL = 0;
		}
	}
	else
	{
		pp->mh->DATEMP_FUNC[0] = TEMP_FUNC_CLOSE;
		pp->mh->DATEMP_RAMP[0] = 0;
		pp->st->DATEMP_REAL[0] = 0;
	}
	
	//电机温度使用最后一段温度，1块E502就是第一块的最后一段，2块E502则是第二块的最后一段
	if (pp->cn_motor_temp > 0)
	{
		m_motor.mh->MOTOR_TEMPCH = pp->cn_motor_temp;
	}						

	if (pp->cn_temp_cool > 0) //根据设定配置落料口温度的设置。
	{	
		pp->mh->TEMPCOOLCN = pp->cn_temp_cool;
   	}
	
	//求出温度模块的最后一段温度
	for (i = pp->real_temp_ch_num; i > 0; i--)
	{
		if(!CHK_TEMP_NOCTRL(pp->mh->DATEMP_FUNC[i - 1]))
		{	
			pp->final_temp = i;
			break;
		}	 
	}	

    InitThermocouple(pp);
}

void InitTempTkTj()
{
	static UI16 last_TEMP_TKTJ = 0;
	PTEMPCTRLCOMM pp;

	if (ChkSysCfg(CO9_TKTJ) != last_TEMP_TKTJ)
	{
		last_TEMP_TKTJ = ChkSysCfg(CO9_TKTJ);

		for(UI16 i =0;i<g_tempcomm_num;i++)
		{
			pp = g_tempcomm[i];
			if(pp != NULL)
			{
				pp->K_J_Varuety=1;//热电偶改变标记20190708HH
				if(ChkSysCfg(CO9_TKTJ))
				{
					pp->Used_KJ=1;//使用J型热电偶20190708HH
					T_tab = TJ_tab;
				}
				else
				{
					pp->Used_KJ=2;
					T_tab = Tk_tab;
				}
				InitTemp(pp);
				InitCyclicData(pp);
				InitTempDataStu(pp);
				InitPidParaPro(pp);
			}		  
		}	      
	}
}
/*******************************************************************************************
 *Brief			:将mh中的PID、缓冲区数据搬运到zd中，并写入下位机FLASH
 *Param			:温度部件指针
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191112
********************************************************************************************/
void TempPIDSave(PTEMPCTRLCOMM pp)
{
	MemcpyPro(&(pp->zd->DATEMP_RAMP[0]),&(pp->mh->DATEMP_RAMP[0]),CAL_LEN(pp->mh->DATEMP_RAMP)*4*sizeof(UI16));
	PartCfgSave((PPART)pp, ZD_ID);
}

/*******************************************************************************************
 *Brief			:用flash内保存的pid数据更新温度控制所用的pid及缓冲区。
 *Param			:id:温度部件号	1开始	按位操作 0x01为1部件 0x02为2部件 0xFF为所有部件
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191112
********************************************************************************************/
void TempPIDUpdate(UI16 id)
{
	PTEMPCTRLCOMM pp = NULL;
	
	for(int i = 0;i < g_tempcomm_num;i++)
	{
		if((id & (0x01 << i)) > 0)
		{			
			pp = g_tempcomm[i];

			if(pp != NULL)
			{				
				MemcpyPro(&(pp->mh->DATEMP_RAMP[0]),&(pp->zd->DATEMP_RAMP[0]),CAL_LEN(pp->mh->DATEMP_RAMP)*4*sizeof(UI16));
				PartDataTx((PPART)pp, MH_ID, CAL_LEN(pp->mh->DATEMP_RAMP) * 4, GET_IND(PTEMPCOMM_MACHSET, DATEMP_RAMP), FALSE);
			}
		}
	}
}


/*******************************************************************************************
 *Brief			:E502温度模块温度归零
 *Param			:temp_id:E502编号 从1开始，对第一块E502操作，id则为1
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191113
********************************************************************************************/
void TempZeroSet(UI16 temp_id)
{
	PTEMPCTRLCOMM pp;
	UI16 i = 0;
	if(temp_id > g_tempcomm_num)
	{
		return;
	}

	pp = g_tempcomm[temp_id - 1];

	if(pp == NULL)
	{
		return;
	}

	if (!ChkTempMachTypeE50X() && (pp->temp_id == 1)&&(GetHwModel() == MODEL_I3101))
	{
		/*温度AD超过最大值，开路或温度超标，不处理*/
		if (pp->st->WTEMP_ADVALUE >= TEMP_AD_MAX_VALUE - 1)
			return;

		if (pp->st->WTEMP_ADVALUE >=  0x320)    //温度归零时，加范围限制，AD码小于800时才能归零，防止误操作
			return;                                 

        /* CO9_HIGH (温度控制+20)打上时，此处归零校准功能失效，强制使用出厂调试区数据 */
		if (pp->st->WTEMP_ADJALL)		  			/*对所有路进行校准	设置值为所有路同时设置  */
		{
			for (i = 0; i < TEMP_E502_CH_NUM; i++)
			{
				m_temp.zd->WTEMP_CHZERO[i] = pp->st->WTEMP_ADVALUE;
			}
		}
		else
		{
			if(pp->st->WTEMP_CHANNEL < TEMP_E502_CH_NUM)
				m_temp.zd->WTEMP_CHZERO[pp->st->WTEMP_CHANNEL] = pp->st->WTEMP_ADVALUE;
		}

		PartCfgSave((PPART)(&m_temp),ZD_ID);

		/*同时保存到出厂数据*/
		CfgDbPartSave((PPART)(&m_temp), ZD_ID, 0, 0, FACTORY_PART_DB);
	}
	else
	{
		g_cane502txdata[temp_id].cmd=E502_TEMP_ZERO_CMD;
		g_cane502txdata[temp_id].wtemp_channel_adjall=(((pp->st->WTEMP_CHANNEL&0xff)<<8)|(pp->st->WTEMP_ADJALL&0xff));
		TempE502DataPdoWrite(temp_id,g_cane502txdata[temp_id].cmd,g_cane502txdata[temp_id].wtemp_channel_adjall,0);
		return ;
	}	
}

/*******************************************************************************************
 *Brief			:E502温度模块温度调整
 *Param			:temp_id:E502编号 从1开始，对第一块E502操作，id则为1
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191113
********************************************************************************************/
void TempCalbriate(UI16 temp_id)
{
	PTEMPCTRLCOMM pp;
	int wtest,i;
	
	if(temp_id > g_tempcomm_num)
	{
		return;
	}

	pp = g_tempcomm[temp_id-1];

	if(pp == NULL)
	{
		return;
	}

	if (!ChkTempMachTypeE50X() && pp->temp_id == 1&&(GetHwModel() == MODEL_I3101))
	{
		/*温度AD超过最大值，开路或温度超标，不处理*/
		if(pp->st->WTEMP_ADVALUE >= TEMP_AD_MAX_VALUE - 1)
			return;

		wtest = (pp->st->WTEMP_ADVALUE - m_temp.zd->WTEMP_CHZERO[pp->st->WTEMP_CHANNEL]) * 1000 / RELUV400;   /*UV值*/

		/*温度系数不能为0*/
		if(wtest <= 2)
			return;
		
		m_temp.st->WTEMP_ADJ = (WORD)wtest;		

        /* CO9_HIGH (温度控制+20)打上时，此处归零校准功能失效，强制使用出厂调试区数据 */
		if (pp->st->WTEMP_ADJALL)		  	/*对所有路进行校准	设置值为所有路同时设置  */
		{
			for (i = 0;i < TEMP_E502_CH_NUM;i++)
				m_temp.zd->WTEMP_CHB[i] = m_temp.st->WTEMP_ADJ;
		}
		else
		{
			if (pp->st->WTEMP_CHANNEL < TEMP_E502_CH_NUM)
				m_temp.zd->WTEMP_CHB[pp->st->WTEMP_CHANNEL] = m_temp.st->WTEMP_ADJ;
		}

		PartCfgSave((PPART)(&m_temp), ZD_ID);

		/*同时保存到出厂数据*/
		CfgDbPartSave((PPART)(&m_temp), ZD_ID, 0, 0, FACTORY_PART_DB);
	}
	else
	{
		g_cane502txdata[temp_id].cmd=E502_TEMP_CAB_CMD;
		g_cane502txdata[temp_id].wtemp_channel_adjall=(((pp->st->WTEMP_CHANNEL&0xff)<<8)|(pp->st->WTEMP_ADJALL&0xff));
		TempE502DataPdoWrite(temp_id,g_cane502txdata[temp_id].cmd,g_cane502txdata[temp_id].wtemp_channel_adjall,0);
	}
	return ;  
}


/*******************************************************************************************
 *Brief			:E502温度模块室温校准
 *Param			:temp_id:E502编号 从1开始，对第一块E502操作，id则为1
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20191113
********************************************************************************************/
void AMBTempAdj(UI16 temp_id)
{
	PTEMPCTRLCOMM pp;
	PTEMPCTRL pt = &m_temp;
	
	if(temp_id > g_tempcomm_num)
	{
		return;
	}

	pp = g_tempcomm[temp_id-1];

	if(pp == NULL)
	{
		return;
	}

	if (!ChkTempMachTypeE50X() && pp->temp_id == 1&&(GetHwModel() == MODEL_I3101))
	{
		if (pt->st->WTEMP_LM73 != 8000)		//如为室温出错，则不进行校准
		{
			if (pp->st->WTEMP_ADJAMB == 500)		//清除室温的校正值   20190426
			{
				pt->zd->WTEMP_OFFSET = 0;
			}
			else
			{
				if (pp->st->WTEMP_ADJAMB > g_ambient.curambient)     //ctrl_tbl[WTEMP_LM73])
				{
					pt->zd->WTEMP_OFFSET =(WORD)(pp->st->WTEMP_ADJAMB - g_ambient.curambient);    //ctrl_tbl[WTEMP_LM73]);
				}
				else
				{
					pt->zd->WTEMP_OFFSET =(WORD)((g_ambient.curambient - pp->st->WTEMP_ADJAMB) | 0x8000);
				}
			}
			
			PartCfgSave((PPART)pt, ZD_ID);		
			/*同时保存到出厂数据*/
			PartCfgSaveFa((PPART)pt, ZD_ID);
		}
	}
	else
	{
		g_cane502txdata[temp_id].cmd=E502_TEMP_AMB_ADJ_CMD;
		g_cane502txdata[temp_id].wtemp_adjamb=pp->st->WTEMP_ADJAMB;
		TempE502DataPdoWrite(temp_id,g_cane502txdata[temp_id].cmd,0,g_cane502txdata[temp_id].wtemp_adjamb);
		return ; 
	}	
}


void WarmOn_Comm(PTEMPCTRLCOMM pp)
{
    if (CheckPI(pp->pi->I_HEATTOUCH) == PI_RESET)
    {
        SET_PART_ERR(pp, ER0_HEATTOUCH_COMM);
        StandAlarm(TRUE);
    }
    else
    {
        InitNewTempByWarmOn(pp,pp->md->CONFUNC);
    	WarmOnNewRampSet(pp); //缓冲区模型参数初始化&&加热模式选择
    	
        CLR_PART_ERR(pp, ER0_HEATTOUCH_COMM);

        SetPOOnOff(pp->po->O_HEATER, PC_ON);	    
		
        pp->st->FL_HEATERSTATUS = TRUE;
		pp->st->DATEMP_YOUHUAXUQIU = 0;
		pp->LastYouHuaFlag = 0;
    }	
}

void WarmOff_Comm(PTEMPCTRLCOMM pp)
{
    int i;

    for (i = 0; i < TEMP_E502_CH_NUM; i++)
    {
        SetPOBit(pp->po->O_TEMOP[i], PC_OFF);
    }

    SetPOOnOff(pp->po->O_HEATER, PC_OFF);
    for (i = 0; i < TEMP_E502_CH_NUM; i++)
    {
    	pp->tempctrldatastu.temppwr[i].pk =0;
		pp->tempctrldatastu.heatcheck[i].lastheatonsts = 0;
		pp->tempctrldatastu.heatonsts[i] = 0;
		pp->tempctrldatastu.heatstat[i] = 0;
		CLR_TMP_ST_8BIT(pp,i);
		pp->st->OUTPUT_PER[i] = 0;
    }

    pp->st->FL_HEATERSTATUS = FALSE;


	pp->st->DATEMP_YOUHUAXUQIU = 0;
	pp->st->DATEMP_YOUHUAOK = 0;
    TempRFStop(pp);
}

void  EnterDebugE502()
{
	UI16 i = 0;
	PTEMPCTRLCOMM pp;

	for(i=0;i<g_tempcomm_num;i++)
	{
		pp = g_tempcomm[i];
		
		if(pp == NULL)
		{
		//如果指针空，则查看下一个部件地址
			continue;
		}
		g_cane502txdata[i+1].cmd=E502_ENTER_DEBUG;	
		g_cane502txdata[i+1].wtemp_channel_adjall=(((pp->st->WTEMP_CHANNEL&0xff)<<8)|(pp->st->WTEMP_ADJALL&0xff));
		TempE502DataPdoWrite(i+1,g_cane502txdata[i+1].cmd,g_cane502txdata[i+1].wtemp_channel_adjall,0);
	}
}

void ExitDebugE502()
{
	UI16 i = 0;
	PTEMPCTRLCOMM pp;

	for(i=0;i<g_tempcomm_num;i++)
	{
		pp = g_tempcomm[i];
		
		if(pp == NULL)
		{
		//如果指针空，则查看下一个部件地址
			continue;
		}
		g_cane502txdata[i+1].cmd=E502_EXIT_DEBUG;
		TempE502DataPdoWrite(i+1,g_cane502txdata[i+1].cmd,0,0);
	}
}

void  Can_e502_test(PTEMPCTRLCOMM pp)
{
	if(ChkOperMode(OM_AUTO))
	{
		return;
	}
	
	switch (pp->can_e502_writestep)
	{
		case 0:                                               //通讯检测流程  
			g_cane502txdata[pp->temp_id].cmd=E502_TEMP_TEST;
			TempE502DataPdoWrite(pp->temp_id,g_cane502txdata[pp->temp_id].cmd,0,0);
			pp->can_e502_writetm = GetMsTick();
			pp->st->WRITE_CNT_E502=1;
			pp->can_e502_writestep ++;
			pp->can_e502_test_flag = 1;
			break;	  
		case 1:
			if ((WORD)(GetMsTick() - pp->can_e502_writetm)>=100)
			{
				g_cane502txdata[pp->temp_id].cmd=E502_TEMP_TEST;
				TempE502DataPdoWrite(pp->temp_id,g_cane502txdata[pp->temp_id].cmd,0,0);
				pp->can_e502_writetm = GetMsTick();
				pp->st->WRITE_CNT_E502++;
				pp->can_e502_test_flag = 1;
			}
			break;
	}
}




/*===================================================
 函数名：Read_surtemp()
 功  能:读室温,2秒钟读一次，温度差值不应该大于2度，否则视为无效
 入口参数：
 出口参数：
=====================================================*/
void Read_Curtemp()
{
	UI16 i = 0;
	PTEMPCTRLCOMM pp;

	for(i=0;i<g_tempcomm_num;i++)
	{
		pp = g_tempcomm[i];

		if(pp == NULL)
		{
		//如果指针空，则查看下一个部件地址
			continue;
		}

		//3100主机未选择使用E502时 板温的室温给部件1
		if (!ChkTempMachTypeE50X() && pp->temp_id == 1&&(GetHwModel() == MODEL_I3101))
		{
			pp->st->WTEMP_LM73 = m_temp.st->WTEMP_LM73;
		}
		else
		{
			if (ChkTempMachTypeE50X())
			{
				if(pp->can_e502_testmode == 1)
				{
					Can_e502_test(pp);
				}
			}
			
			if((delt_time(A_100msclock,pp->ReadTime_E502_Ver) > 100) && (!ChkRunFlag(INIF_DEBUG)))
			{
				pp->ReadTime_E502_Ver = A_100msclock;
				g_cane502txdata[pp->temp_id].cmd=E502_TEMP_VER;
				TempE502DataPdoWrite(pp->temp_id,g_cane502txdata[pp->temp_id].cmd,0,0);
			}
			
			pp->st->WTEMP_LM73 = g_cantempe502comm[pp->temp_id].wtemp_lm73;
		}	
	}
}

int GetCurTempCanRead(PTEMPCTRLCOMM pp)
{
	UI16 id = 0;
	int flag = 0;

	id = pp->temp_id;
	
	if(id >= TEMP_MAX_NUM)
	{
		return flag;
	}  	
    if (pp->step == 0) //发送请求
    {
    	if (ChkRunFlag(INIF_DEBUG))
    		g_cane502txdata[id].cmd = E502_TEMP_TEST;
        else
			g_cane502txdata[id].cmd = E502_TEMP_VALUE;
		
        TempE502DataPdoWrite(id, g_cane502txdata[id].cmd, ((pp->curchnl & 0xff) << 8), 0);
        pp->clock_send = A_1msclock;
        pp->step = 1;
    }
    else if (pp->step == 1)
    {
        if ((ChkRunFlag(INIF_DEBUG)&&(delt_time(A_1msclock, pp->clock_send) > 100)) || (g_cantempe502comm[id].TempFlag == 2) && (pp->curchnl == g_cantempe502comm[id].CurChnl)) //读取到数据
        {
            pp->st->WTEMP_RELAT = g_cantempe502comm[id].wtemp_real;
			pp->st->WTEMP_ADVALUE = g_cantempe502comm[id].wtemp_advalue;
			
            pp->tempctrldatastu.tempdata[pp->curchnl].test = g_cantempe502comm[id].TestTemp;

			if (CHK_TEMP_PCT_CTRL(pp->mh->DATEMP_FUNC[pp->curchnl]))
			{
				if (ChkSysCfg(XF2_RELCL))
                {            
					pp->st->DATEMP_REAL[pp->curchnl] = g_cantempe502comm[id].RealTemp;
                    ChkTempHighErr(pp, pp->curchnl);
                }
				else
					pp->st->DATEMP_REAL[pp->curchnl] = 0;
			}
			else
			{
				pp->st->DATEMP_REAL[pp->curchnl] = g_cantempe502comm[id].RealTemp;
			}

			flag = g_cantempe502comm[id].TempFlag;
            g_cantempe502comm[id].TempFlag = 0;
            pp->step = 0;
            pp->cn = 0;
        }
        else //否则每个150ms发送请求
        {
            if (delt_time(A_1msclock, pp->clock_send) > 150)
            {
                pp->clock_send = A_1msclock;
                g_cane502txdata[id].cmd = E502_TEMP_VALUE;
                TempE502DataPdoWrite(id, g_cane502txdata[id].cmd, ((pp->curchnl & 0xff) << 8), 0);
                pp->cn++;
            }
            if (pp->cn > TEMPREDERRCN) //超过3次未读到，则当前段报警788， 并关闭当前的温度输出
            {
            	if(pp->st->DATEMP_REAL[pp->curchnl] != 779)//临时修改
            	{
					pp->st->DATEMP_REAL[pp->curchnl] = 788;	
				}

				SetPOBit(pp->po->O_TEMOP[pp->curchnl], PC_OFFD);

				//跳过当前段，开始下一段
                pp->step = 0;
                pp->cn = 0;
				flag = 2;
            }
        }
    }

    return	flag;
}

//用于7474的温度测试读取
void GetCurTmpCan(PTEMPCTRLCOMM pp)
{
    pp->st->WTEMP_RELAT = g_cantempe502comm[pp->temp_id].wtemp_real;
    pp->st->WTEMP_ADVALUE = g_cantempe502comm[pp->temp_id].wtemp_advalue;
}

void ChangeAdjTempCh(PTEMPCTRLCOMM pp)
{	
	g_cane502txdata[pp->temp_id].cmd = E502_TEMP_DATA;
	g_cane502txdata[pp->temp_id].wtemp_channel_adjall = (((pp->st->WTEMP_CHANNEL & 0xff) << 8) | (pp->st->WTEMP_ADJALL & 0xff));
	TempE502DataPdoWrite(pp->temp_id, g_cane502txdata[pp->temp_id].cmd, g_cane502txdata[pp->temp_id].wtemp_channel_adjall, 0);
}


void   Get_TempTest(PTEMPCTRLCOMM pp)
{
	if(pp == NULL)
	{
		return;
	}
	
	GetCurTmpCan(pp);    
}



/*-----------------------------------------------------------------------------------+
|      防冷启动检查函数          ccc                                                   |
+------------------------------------------------------------------------------------*/
void CoolWarmCheck(PTEMPCTRLCOMM pp)
{
    int   i, CoolNum = 0;
	UI16  opennum = 0;

    if ((pp->CoolCheck.Checked < 2) && (pp->md->TM_HEATERWARM > 0) && (pp->md->TM_HEATERWARM < 31))
    {
        if (pp->CoolCheck.Checked == 0)
        {
            CoolNum = 0;
            for (i = 0 ; i < pp->real_temp_ch_num; i++)
            {
                /*如果功能选择PID控制或者手动控制的时候才进行预温计时的温度判断
				这里i=0，是从料管温度第1段开始，但是ptmp->mh->DATEMP_FUNC[0] 这个0是油温开始，因此要加1，否则不对。
				*/                

				if(CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[i]))
				{
                    if (pp->st->DATEMP_REAL[i] + 5 >= pp->md->DATEMP_SETTING[i])
                    {
						SET_TMP_ST(pp,i,TEMP_ST_HOLD);
                    }
                    else
                    {
                        CoolNum = 1;
                    }

					opennum++;
				}
            }
            if ((CoolNum == 0)&&(opennum > 0))
            {
                if (pp->st->FL_HEATERSTATUS)
                {
                    pp->st->FL_HEATERSTATUS = 2;

                }
                pp->CoolCheck.Checked = 1;
                pp->CoolCheck.StartTime = GetSecTick();
                pp->st->WSTATERSV6  = 0;
            }

			if(opennum == 0)
			{
				pp->CoolCheck.Checked = 3;
			}
        }
        else if (pp->CoolCheck.Checked == 1)
        {
            pp->st->WSTATERSV6 = (WORD)(GetSecTick() - pp->CoolCheck.StartTime);
            if (pp->st->WSTATERSV6 >= (pp->md->TM_HEATERWARM * 60))
            {
                if (pp->st->FL_HEATERSTATUS)
                {
                    pp->st->FL_HEATERSTATUS = TRUE;

                }
                pp->CoolCheck.Checked = 2;
                pp->st->WSTATERSV6  = 0;
                CLR_PART_ERR(pp, ER0_WUPT_COMM);
            }
        }
    }
	else
	{
		pp->st->WSTATERSV6  = 0;
	}
}

void looptempctrl(PTEMPCTRLCOMM pp)
{
    /*加入功率控制算法*/
    int	i;
    for (i = 0; i < TEMP_E502_CH_NUM; i++)
    {
    	//j为当前模块温度段数
    			
		if(!CHK_TEMP_CLS(pp->mh->DATEMP_FUNC[i]))
        {
            if ((pp->tempctrldatastu.temppwr[i].pk > 70) && (pp->tempctrldatastu.temppwr[i].pk < 9930))
            {
                /*PID控制加热*/
                if (pp->tempctrldatastu.heatstat[i])
                {
                    if (delt_time(GetMsTick(), pp->tempcs[i].chnlctlcn_sttm) >= pp->tempctrldatastu.On_Time[i])
                    {
                        On_Offheat(pp,i, 0);
                        SetheatS(pp,i, TEMP_ST_ZERO);
                        pp->tempctrldatastu.heatstat[i] = 0;
                    }
                }
            }
        }
    }
}




/*-----------------------------------------------------------------------------------+
|      周期加热控制                                                              |
+-----------------------------------------------------------------------------------*/
void    cycleheat(PTEMPCTRLCOMM pp)
{
    int i = 0;
    for (i = 0; i < TEMP_E502_CH_NUM; i++)
    {
    	//i为当前模块温度段数

		if(CHK_TEMP_PCT_CTRL(pp->mh->DATEMP_FUNC[i]))
        {
            if ((pp->cycleheat[i].heatstatus == 0) && (DIFF_100MS(pp->cycleheat[i].heatoffstart) >= ((1000 - pp->md->DACENT_HEATERON[i])*pp->mh->TM_RESPONSELESSCYCLE[i] / 100)))
            {
                SetPOBit(pp->po->O_TEMOP[i], PC_ON);
                pp->cycleheat[i].heatonstart = Get100MsTick();
                pp->cycleheat[i].heatstatus  = 1;
				SetheatS(pp,i,TEMP_ST_CTRL);
            }
            else if ((pp->cycleheat[i].heatstatus == 1) && (DIFF_100MS(pp->cycleheat[i].heatonstart) >= pp->md->DACENT_HEATERON[i]*pp->mh->TM_RESPONSELESSCYCLE[i] / 100))
            {
                SetPOBit(pp->po->O_TEMOP[i], PC_OFFD);
                pp->cycleheat[i].heatoffstart = Get100MsTick();
                pp->cycleheat[i].heatstatus  = 0;
                SetheatS(pp,i,TEMP_ST_ZERO);
            }

            pp->st->OUTPUT_PER[i] = pp->md->DACENT_HEATERON[i];
        }
    }
}


/*-----------------------------------------------------------------------------------+
|      料管冷却处理                                                                  |
+-----------------------------------------------------------------------------------*/
void tempcooler(PTEMPCTRLCOMM pp)
{
    int   i;
    BOOL  ftcon = FALSE, ftcoff = FALSE;

    for (i = 0 ; i < TEMP_E502_CH_NUM; i++)
    {
    	//i为当前模块温度段数
    	
		/*当温度功能选择手动或者PID控制时候才进行料管冷却
		这里i=0，是从料管温度第1段开始，但是ptmp->mh->DATEMP_FUNC[0] 这个0是油温开始，因此要加1，否则不对。
		*/
        if (pp->md->FL_TEMPCOOL && CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[i]))
        {
            if ((pp->md->DATEMP_SETTING[i] + pp->mh->DATEMP_TEMPCOOLERON) <= pp->st->DATEMP_REAL[i])
            {
                SetPOBit(pp->po->O_COOL[i], PC_ON);
				SET_TMP_ST(pp,i,TEMP_ST_COOLER);
                ftcon = TRUE;
            }
            else if (pp->md->DATEMP_SETTING[i] >= (pp->st->DATEMP_REAL[i] + pp->mh->DATEMP_TEMPCOOLEROFF))
            {
				SetPOBit(pp->po->O_COOL[i], PC_OFFD);
				CLR_TMP_ST(pp,i,TEMP_ST_COOLER);
			}

            ftcoff = ftcoff | TestPOIsOn(pp->po->O_COOL[i]);
        }
        else     //2014-12-01 LY 修改冷却器功能选择不使用时候不关输出点,功能选择不使用马上关输出点
        {
            SetPOBit(pp->po->O_COOL[i], PC_OFFD);
			CLR_TMP_ST(pp,i,TEMP_ST_COOLER);
            ftcoff = ftcoff | TestPOIsOn(pp->po->O_COOL[i]);
        }
    }
    if (ftcon)
    {
        SetPOBit(pp->po->O_COOLER, PC_ON);
    }
    if (ftcoff == 0)
    {
        SetPOBit(pp->po->O_COOLER, PC_OFFD);
    }
}


/*-----------------------------------------------------------------------------------+
|      落料口温度超限报警                                                           |
+-----------------------------------------------------------------------------------*/

void TempCoolChk (PTEMPCTRLCOMM pp)
{
    if (pp->fl_cool_temp && (pp->mh->TEMPCOOLMAX != 0))
    {
		if(pp->st->DATEMP_CHARGEPOS >= pp->mh->TEMPCOOLMAX)
		{
       		SET_PART_ERR(pp, ER0_COORER_COMM);
        }
        else
        {
            CLR_PART_ERR(pp, ER0_COORER_COMM);
		}
	}
    else
    {
        CLR_PART_ERR(pp, ER0_COORER_COMM);
    }
}

/*-----------------------------------------------------------------------------------+
|      落料口冷却处理                                                                  |
+-----------------------------------------------------------------------------------*/
void  DischargePortCooler(PTEMPCTRLCOMM pp)
{
    if (pp->fl_cool_temp)
    {
        if (pp->st->DATEMP_CHARGEPOS >= pp->mh->DATEMP_TEMPCOOLERONMOUTH)
        {
            if (!TestPOIsOn(pp->po->O_DROP_COL))
            {
                SetPOBit(pp->po->O_DROP_COL, PC_ON);
            }
        }
        else if (pp->st->DATEMP_CHARGEPOS <= pp->mh->DATEMP_TEMPCOOLEROFFMOUTH)
        {
            if (TestPOIsOn(pp->po->O_DROP_COL))
            {
                SetPOBit(pp->po->O_DROP_COL, PC_OFFD);
            }
        }
    }
    else
    {
        if (TestPOIsOn(pp->po->O_DROP_COL))
        {
            SetPOBit(pp->po->O_DROP_COL, PC_OFFD);
        }
    }	
}

void GetTempOnTM_60s(PTEMPCTRLCOMM pp)
{
	WORD i = 0;
	WORD state = 0;

	if(!GetHeatingStatus()) 
	{
		pp->temppower.temp_power_starttm_ms = GetMsTick();
		pp->temppower.temp_power_div_starttm_ms = GetMsTick();
		return;
	}
	
	if (DIFF_MS(pp->temppower.temp_power_starttm_ms) < 60000)
	{
		if (DIFF_MS(pp->temppower.temp_power_div_starttm_ms) >= 1)
		{
			pp->temppower.temp_power_div_starttm_ms = GetMsTick();
			
			for (i = 0;i < TEMP_E502_CH_NUM; i++)
			{
				state = TestPOIsOn(pp->po->O_TEMOP[i]);
				if (state)
				{
					pp->temppower.temp_power_ontime_ms[i]++;
				}
			}	
		}				
	}
	else
	{
		pp->temppower.temp_power_starttm_ms = GetMsTick();

		for (i = 0;i < TEMP_E502_CH_NUM; i++)
		{
			pp->st->GETTEMPONTM_60s[i] = pp->temppower.temp_power_ontime_ms[i];
			pp->temppower.temp_power_ontime_ms[i] = 0;
		}
	}
}

void TempOutClearAll(PTEMPCTRLCOMM pp)  //清除所有温度输出点
{
    UI16 i;

    if(pp != NULL)
    {
        for (i = 0; i < TEMP_E502_CH_NUM; i++)
        {
            SetPOBit(pp->po->O_TEMOP[i], PC_OFF);
        }
    }
}

/*******************************************************************************************
  * @函数名称	Get_TempControl
  * @函数说明   温度控制函数，采用时间：40+1*400=440ms
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 ****************************************************************************************/
void   Get_TempControl(PTEMPCTRLCOMM pp)
{
	PMOTOR pmt = &m_motor;
    int flag;
    int i;

	if(pp == NULL)
	{
		return;
	}

    switch (pp->tempstep)
    {
        case 0:	//初始化时调用一次-20180820 zt
            pp->curchnl = 0xFF;//设置0xFF表示第一次扫描
            pp->tempstep ++;
            break;
        case 1:
            if (pp->temprealchnl > 0)
            {            	
				if(ChkRunFlag(INIF_DEBUG))							//测试界面时,通道始终不变 --WD1011
				{
					pp->curchnl = pp->st->WTEMP_CHANNEL%10;
				}
				else
				{
					if(pp->curchnl == 0xFF)
	            	{//第一次扫描保证在第一段开始
						pp->curchnl = 0;
						//查询开启的温度段，依次循环扫描-20180820 zt
		                for (i = 0; i < pp->real_temp_ch_num; i++)
		                {
		                    if (pp->tempcs[(pp->curchnl + i) % pp->real_temp_ch_num].chnlscantm > 0)
		                    {
		                        pp->curchnl = (pp->curchnl + i) % pp->real_temp_ch_num;
		                        break;
		                    }
		                }
					}
					else
					{
						//查询开启的温度段，依次循环扫描-20180820 zt
		                for (i = 1; i < pp->real_temp_ch_num; i++)
		                {
		                    if (pp->tempcs[(pp->curchnl + i) % pp->real_temp_ch_num].chnlscantm > 0)
		                    {
		                        pp->curchnl = (pp->curchnl + i) % pp->real_temp_ch_num;
		                        break;
		                    }
		                }
					}  
				}
            	      
				if(!ChkTempMachTypeE50X() && pp->temp_id == 1&&(GetHwModel() == MODEL_I3101))		//用了e502后，主板上的温度就不扫描了
					m_change_channels(pp->curchnl);					/*通道转换*/
				
                pp->chnlstart[pp->curchnl] = GetMsTick();
				
                if (pp->curchnl == 0 && pp->fl_checkagain == 0)
                {
                    pp->fl_checkagain = 1;
                }

                pp->last_temp[pp->curchnl] = pp->st->DATEMP_REAL[pp->curchnl];


				if (ChkRunFlag(INIF_DEBUG))
				{
					pp->tempcs[pp->curchnl].chnlscantmWait = 140;
				}
				else
				{
					if (!ChkTempMachTypeE50X() && pp->temp_id == 1&&(GetHwModel() == MODEL_I3101))
					{					
						if (pp->tempcs[pp->curchnl].chnlscantm > 290) //每段读取时间至少保留120ms的采样时间-20180809zt
		                {
		                    pp->tempcs[pp->curchnl].chnlscantmWait = pp->tempcs[pp->curchnl].chnlscantm - 210;
		                }
		                else
		                {
		                    pp->tempcs[pp->curchnl].chnlscantmWait = 80;
		                }
					}
	                else
	                {
						if (pp->tempcs[pp->curchnl].chnlscantm > 120) //每段读取时间至少保留120ms的采样时间-20180809zt
		                {
		                    pp->tempcs[pp->curchnl].chnlscantmWait = pp->tempcs[pp->curchnl].chnlscantm - 120;
		                }
		                else
		                {
		                    pp->tempcs[pp->curchnl].chnlscantmWait = 0;
		                }
					}
				}
                pp->tempstep ++;
            }
            break;
        case 2://等待，只预留120ms用于读取温度其他时间在此处消耗，在最后的120ms内完成读取并在120ms后运行温度控制，使得采集与控制之间的时间差尽量减小-20180809zt
            if ((WORD)(GetMsTick() - pp->chnlstart[pp->curchnl]) >= pp->tempcs[pp->curchnl].chnlscantmWait)
            {
            	pp->tempstep ++;
            }
            break;
        case 3:	
			if(!ChkTempMachTypeE50X() && (pp->temp_id == 1))
				m_start_temp_sampling(pp->curchnl);							 /*启动转换*/
            pp->lastflag = 0;
			pp->starttime = (UI16)Get100UsTick();
            pp->tempstep ++;
            break;
        case 4:   		

            if (pp->lastflag < 2)
            {	            	
				if(ChkTempMachTypeE50X())
					flag = GetCurTempCanRead(pp);
				else
					flag = getcurtemp();							/*采集温度，并转换*/

                pp->lastflag = flag;
            }

            if (pp->lastflag)   									/*注意此处if内的逻辑关系*/
            {
                if (pp->lastflag >= 2) 								/*采集成功，次数达到*/
                {  	
                    if(pp->lastflag == 2)
                    {
						pp->lastflag = 3;

						if (pp->st->DATEMP_REAL[pp->curchnl] >= MAXTEMP)
	        			{
							SetheatS(pp,pp->curchnl,TEMP_ST_ERR);
	                	}
	                	//油温温度赋值
	                	if(pp->fl_oil_temp && (pp->curchnl == pp->cn_oil_temp))
	                	{
							RealOilSet(pp);
						}
						
						//如果扫描温度段为落料口温度时，温度显示为落料口温度，原显示为0	-20180820 zt
						if (pp->fl_cool_temp && (pp->curchnl== pp->cn_temp_cool))
	                    {                    	
	                        pp->st->DATEMP_CHARGEPOS	=	pp->st->DATEMP_REAL[pp->cn_temp_cool];
	                    }

						//电机温度使用最后一段温度，1块E502就是第一块的最后一段，2块E502则是第二块的最后一段
						if(pp->fl_motor_temp)
						{
							if(pp->cn_motor_temp == pp->curchnl)
							{
								pmt->st->DATEMP_REALMOTOR	= pp->st->DATEMP_REAL[pp->cn_motor_temp];
							}						
						}
					}                

                    if (pp->first_flag[pp->curchnl] == 1)
                    {
                        pp->first_flag[pp->curchnl] = 0;
                        pp->last_temp[pp->curchnl] = pp->st->DATEMP_REAL[pp->curchnl];
                    }

                    if (ABS_DIFF(pp->st->DATEMP_REAL[pp->curchnl], pp->last_temp[pp->curchnl]) > 20 && pp->fl_checkagain == 1)
                    {
                        pp->tempstep = 3;
                        pp->st->DATEMP_REAL[pp->curchnl] = pp->last_temp[pp->curchnl];
                        pp->fl_checkagain = 0;
                    }
                    else
                    {
                        //稳定控制周期基数为chnlscantm，且非PID控制不需要进入 	-20180820 zt
                        if ((UI16)DIFF_MS(pp->chnlstart[pp->curchnl]) >= (UI16)pp->tempcs[pp->curchnl].chnlscantm)
                        {
                        	if(CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[pp->curchnl]))
                        	{
								pp->tempctrldatastu.tempdata[pp->curchnl].cur_temp = ADtoTemp_float(pp->tempctrldatastu.tempdata[pp->curchnl].test);
								if (pp->st->FL_HEATERSTATUS)
								{
									GetCurTempUpSpeed(pp, pp->curchnl); //每测得一个温度数据，就计算升温速率
									STOStart(pp); //检测是否进行特殊优化
									CalTempRamp(pp, pp->curchnl); //实时根据升温速率计算缓冲区
								}
								//为满足设定控制周期，控制周期为扫描周期的chnlctlcn倍	-20180820 zt
	                            pp->tempcs[pp->curchnl].chnlctlcn_st++;
	                            if (pp->tempcs[pp->curchnl].chnlctlcn <= pp->tempcs[pp->curchnl].chnlctlcn_st)
	                            {
	                                TempControlCore(pp);

	                                pp->tempcs[pp->curchnl].chnlctlcn_st = 0;//一轮控制结束重新计数	-20180820 zt
	                                pp->tempcs[pp->curchnl].chnlctlcn_sttm = GetMsTick();//控制周期开始时间，用于比例加温计算时间		-20180820 zt
	                            }
							}

								if(TempErrStByCh(pp))
								{								
									SET_TMP_ST(pp,pp->curchnl,TEMP_ST_DEV);
								}
								else
								{
									CLR_TMP_ST(pp,pp->curchnl,TEMP_ST_DEV);
								}
								pp->tempstep  = 1;//单段温度扫描结束，扫描下一段		-20180820 zt
	                        }
	                    }
	                }
					else
					{
						if(!ChkTempMachTypeE50X())
	          			{
		    				m_start_temp_sampling(pp->curchnl);
	          			}
					}
	            }
			else if ((!ChkTempMachTypeE50X()) && delt_time((UI16)Get100UsTick(), pp->starttime)>60)
			{     	        	

				/*从777   改成788，与反接区分*/
				pp->st->DATEMP_REAL[pp->curchnl]=788;					/*温度数据0.5秒不翻转循检下一通道*/

				//油温温度赋值
            	if(pp->fl_oil_temp && (pp->curchnl == pp->cn_oil_temp))
            	{
					RealOilSet(pp);
				}
				
				//如果扫描温度段为落料口温度时，温度显示为落料口温度，原显示为0	-20180820 zt
				if (pp->fl_cool_temp && (pp->curchnl== pp->cn_temp_cool))
                {                    	
                    pp->st->DATEMP_CHARGEPOS	=	pp->st->DATEMP_REAL[pp->cn_temp_cool];
                }

				//电机温度使用最后一段温度，1块E502就是第一块的最后一段，2块E502则是第二块的最后一段
				if(pp->fl_motor_temp)
				{
					if(pp->cn_motor_temp == pp->curchnl)
					{
						pmt->st->DATEMP_REALMOTOR	= pp->st->DATEMP_REAL[pp->cn_motor_temp];
					}						
				}

				ClosCurheat(pp);

		    	g_temp.count = 0;			                				/*初始化温度计数器*/
			  	pp->tempstep  = 0;
				g_temp.current_sum = 0;
				g_temp.invalid_sum = 0;
				g_temp.cninvalid = 0;
				g_temp.cnvalid = 0;

				g_temp.fail_count = 0;

			}
            break;
		default:
			pp->tempstep  = 0;
			break;
    }

    if (pp->st->FL_HEATERSTATUS) 										/*电热状态*/
    {
        looptempctrl(pp);	
        cycleheat(pp);		//比例加热函数
		SaveAdpative(pp);
		SaveIk0(pp);
		TurnToRamp(pp);
    }
	
	//判断是否完成缓冲区优化
	pp->st->RAMP_OPTIMIZE = Ramp_Optimize_OK(g_tempcomm[0]);
	
    tempcooler(pp);
	TempCoolChk(pp);
	DischargePortCooler(pp);//落料口冷却
	CoolWarmCheck(pp);
	OilCoolerChk();
	GetTempOnTM_60s(pp);	//温度输出功率统计

  	if (ChkSysCfg(CO1_OIL) && MotorIsOn() && OilTmpErr())
	{
		if (GetOperMode() ==0)
			MotorDown();
	}
    
    if(pp->mh->FL_TEMPWORDUPCHECK == 1)
      ChkThermocoupleDie(pp);

    if(!pp->st->FL_HEATERSTATUS)
		TempOutClearAll(pp);    //电热关掉时清除所有温度输出点，避免异常情况下只关闭了电热没关闭输出点
}

void ChkThermocoupleDie(PTEMPCTRLCOMM ptmp)//检查热电偶呆滞，即某一段反馈温度停在某个值不动，此时应该报警切电热
{
    if(ptmp->ChkThermocoupleDie_step_1 == 0)
    {
        if(ptmp->st->FL_HEATERSTATUS == 1)
            ptmp->ChkThermocoupleDie_step_1 = 1;
    }
    else if(ptmp->ChkThermocoupleDie_step_1 == 1)
    {
        if(ptmp->st->FL_HEATERSTATUS == 0)//电热从开变为关，初始化一遍
        {
            ptmp->ChkThermocoupleDie_step_1 = 0;
            InitThermocouple(ptmp);//把表清0，再次开启电热，需要等待填表
        }
    }
    else
        ptmp->ChkThermocoupleDie_step_1 = 0;

    /* 判断某一段温度是否需要进行检查 */
    JdgChNeedChk(ptmp);

    /* 间隔10s，存储某一段的当前温度值，共记录310s */
    StoreTemp300s(ptmp);

    /* 对比某一段当前温度与300s前的温度，温升小于3度，就报警，切电热 */
    CompareTempNotUp(ptmp);
    
}

void CompareTempNotUp(PTEMPCTRLCOMM ptmp)//对比last表中最后一行温度与第一行的温度，升温小于3度报警并切电热
{
    int i;

    if(ptmp->st->FL_HEATERSTATUS == 0)
    {
        if(ptmp->CompareTempNotUp_step_0 != 0)
            ptmp->CompareTempNotUp_step_0 = 0;
        if(ptmp->CompareTempNotUp_tm_10s != 0)
            ptmp->CompareTempNotUp_tm_10s = 0;
        return;
    }

    if(ptmp->CompareTempNotUp_step_0 == 0)
    {
        ptmp->CompareTempNotUp_tm_10s = (UI16)GetSecTick();
        ptmp->CompareTempNotUp_step_0 = 1;
        for(i = 0;i < TEMP_E502_CH_NUM;i++)
        {
            ptmp->temp_err_cnt[i] = 0;
        }
    }
    else if(ptmp->CompareTempNotUp_step_0 == 1)
    {
        if((UI16)GetSecTick() - ptmp->CompareTempNotUp_tm_10s > 320)//开启电热后，等待temp_themocp_chk[31][16]表填满
        {
            ptmp->CompareTempNotUp_tm_10s = (UI16)GetSecTick();
            ptmp->CompareTempNotUp_step_0 = 2;
        }
    }
    else if(ptmp->CompareTempNotUp_step_0 == 2)
    {
        if((UI16)GetSecTick() - ptmp->CompareTempNotUp_tm_10s > 10)//每隔10s，对比一次,即每次填表间隔期间，对比1次
            ptmp->CompareTempNotUp_step_0 = 3;
    }
    else if(ptmp->CompareTempNotUp_step_0 == 3)
    {
        for(i=0;i<TEMP_E502_CH_NUM;i++)//i从1开始，油温不比较
        {
            if(ptmp->temp_ch_chk[i] == 0 && //需要检测时才判断是否报警，0 = 检测，1 = 不检测
                (ptmp->temp_themocp_chk[29][i] - ptmp->temp_themocp_chk[0][i] <= 3))/* 表的第30行与第1行数据相差3度以内，就报警 */
            {   
                ptmp->temp_err_cnt[i]++;
                
                if(ptmp->temp_err_cnt[i] >= 3)
                {
                    SET_TMP_ST(ptmp,i,TEMP_ST_NUP);
                    SET_TMP_ST(ptmp,i,TEMP_ST_ERR);
                    SET_PART_ERR(ptmp, ER0_HTON_COMM);
                    ClosCurheat(ptmp);
                    WarmOff();
                }
                break;
            }
            else
            {
                ptmp->temp_err_cnt[i] = 0;
            }
        }
        
        ptmp->CompareTempNotUp_step_0 = 2;
        ptmp->CompareTempNotUp_tm_10s = (  UI16)GetSecTick();
    }
    else
       ptmp->CompareTempNotUp_step_0 = 0; 
}

/* 存储任一时刻，往前310s内，间隔10s的31组16段的温度数据存入temp_themocp_chk[][]表中 */
void StoreTemp300s(PTEMPCTRLCOMM ptmp)
{
    int i,j;

    if(ptmp->st->FL_HEATERSTATUS == 0)
    {
        if(ptmp->StoreTemp300s_k != 0)
            ptmp->StoreTemp300s_k = 0;
        if(ptmp->step_10s != 0)
            ptmp->step_10s = 0;
        return;
    }
    
    switch(ptmp->step_10s)
    {
        case 0:
            ptmp->tm_start_10s = (UI16)Get100MsTick();//计时起点
            ptmp->step_10s = 1;
            break;
            
        case 1:
            if((UI16)Get100MsTick() - ptmp->tm_start_10s > 100)//间隔10s存一行数据
            {
                for(i=0;i<TEMP_E502_CH_NUM;i++)//油温暂时也存进表中，备用
                {
                    ptmp->temp_themocp_chk[ptmp->StoreTemp300s_k][i] = ptmp->st->DATEMP_REAL[i];
                }
                ptmp->step_10s = 2;
            }
            break;
            
        case 2:
            if(ptmp->StoreTemp300s_k < TEMPCHKTM - 1)
            {
                ptmp->StoreTemp300s_k++;
                ptmp->step_10s = 0;
            }
            else//大于310s，把表往前滚一位
            {
                for(j=0;j<TEMPCHKTM-1;j++)
                {
                    for(i=0;i<TEMP_E502_CH_NUM;i++)//表滚完，第30行与第31行数据相同，第31行数据不用
                    {
                        ptmp->temp_themocp_chk[j][i] = ptmp->temp_themocp_chk[j+1][i];
                    }    
                }
                ptmp->step_10s = 0;
            }
            break;
            
        default:
            ptmp->step_10s = 0;
            break;
    }
}

/* 1、缓冲区       =   0，不需要检测 */
/* 2、反馈温度      >=  设定温度 - 电热连续加温检查度数，不需要检测 */
/* 3、电热连续加温检查度数 = 0时，使用老逻辑判断，不需要检测 */
/* 4、第10段以上，且反馈温度为0的，不需要检测，主机温度资源目前最多也只有9段 */
void JdgChNeedChk(PTEMPCTRLCOMM ptmp)
{
    int i;
    
    if(ptmp->st->FL_HEATERSTATUS == 0)
        return;
    
    for(i=0;i<TEMP_E502_CH_NUM;i++)//i从1开始，油温不判断
    {
        if((ptmp->mh->DATEMP_RAMP[i] == 0) || (ptmp->mh->DA_THERMOCOUPLE_PRT >= ptmp->md->DATEMP_SETTING[i]) ||
        ((ptmp->mh->DA_THERMOCOUPLE_PRT < ptmp->md->DATEMP_SETTING[i]) && (ptmp->st->DATEMP_REAL[i] >= ptmp->md->DATEMP_SETTING[i] - ptmp->mh->DA_THERMOCOUPLE_PRT)) || 
        (ptmp->mh->DA_THERMOCOUPLE_PRT == 0) || !CHK_TEMP_AI_CTRL(ptmp->mh->DATEMP_FUNC[i]) || (ptmp->tempctrldatastu.heatonsts[i] != 3))
            ptmp->temp_ch_chk[i] = 1;
        else
            ptmp->temp_ch_chk[i] = 0;
    }
}


UI32 TempCtrlCommCmd(PTEMPCTRLCOMM pp, UI32 cmd, UI32 para)
{
    int	i;
    BOOL ret = TRUE;

    switch (GET_CMD(cmd))
    {
        case CMD_DATA_RX:
            if (pp->mh != NULL && (GET_CMD_PARA(cmd) & 0xff) != ST_ID && (GET_CMD_PARA(cmd) & 0xff) != PO_ID)
            {          
            	//对温度段数数据做一个保护，防止数据溢出
				pp->real_temp_ch_num = (pp->mh->DATEMP_CH_NUM < TEMP_E502_CH_NUM) ? pp->mh->DATEMP_CH_NUM:TEMP_E502_CH_NUM;
				
				if (ChkSysCfg(CO9_XIND))
				{
					pp->real_temp_ch_num = (pp->real_temp_ch_num < (g_outtempuv_cn+1)) ? (g_outtempuv_cn+1):pp->real_temp_ch_num;
				}


				for (i = 0; i < TEMP_E502_CH_NUM; i++)
                {                	
                    pp->tempctrldatastu.tempdata[i].ramp = pp->mh->DATEMP_RAMP[i];
                }

				if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
				{
					int offset;
					offset = (para >> 16 & 0xffff);
					
					if((GET_IND(PTEMPCOMM_MACHSET, DATEMP_FUNC[0]) <= offset) && (GET_IND(PTEMPCOMM_MACHSET, DATEMP_RAMP[0]) > offset))
					{          
						CLR_TMP_ST_8BIT(pp,offset - GET_IND(PTEMPCOMM_MACHSET, DATEMP_FUNC[0]));
					}

					//温度上限保护，使用第一个温度部件的数据
					if(g_tempcomm[0] != NULL)
					{
						tempuplimit[pp->temp_id] = g_tempcomm[0]->mh->TEMP_PROTECT;
					}
				}

				if(pp->temp_id < TEMP_MAX_NUM)
				{	
					if(tempcoolinit[pp->temp_id])
					{
		            	InitTemp(pp);
						InitCyclicData(pp);
						InitTempDataStu(pp);
						InitPidParaPro(pp);
					}
            	}
			}
			
            UI16 offset;
            offset = ((para >> 16) & 0xFF);

			/*调试通道切换时，需要发送通道切换信号给E502*/
            if (((GET_CMD_PARA(cmd) & 0xff) == ST_ID) && (ChkRunFlag(INIF_DEBUG))
				&& (GET_IND(PTEMPCTRLCOMM_STATE, WTEMP_CHANNEL) == offset))
            {
            	ChangeAdjTempCh(pp);
            }
            break;
		case CMD_MANUAL_INIT:
			InitManualTempRF(pp);
			
			//清除状态
			for(i = 0;i<TEMP_E502_CH_NUM;i++)
			{
				CLR_TMP_ST_ERRBIT(pp,i);
			}
			pp->err_cyclic_start = 0;
			pp->err_manual = 1;
			break;
        default:
            ret = FALSE;
            break;
    }

    return ret;
}


BOOL InitTempCtrlCommPart(PTEMPCTRLCOMM pp, UI8 sn)
{
    static const SUB_INIT_INFO sub_init[] = TEMPCTRLCOMM_INIT_INFO;
	
    if (!INIT_PART(pp, TEMPCTRLCOMM_ID, sn, sub_init))
    {
        return FALSE;
    }    

	if(sn >= TEMP_MAX_NUM)
	{
		return FALSE;
	}

	pp->temp_id = sn<TEMP_MAX_NUM?sn:0;	//E502温度模块的部件序号

	InitTempRamData(pp);

	g_tempcomm[g_tempcomm_num] = pp;
	g_tempcomm_num++;
	
    pp->K_J_Varuety=0;	//初始化20190708HH
	pp->Used_KJ=0;	//初始化20190708HH
	pp->err_manual = 1;


	for (int i = 0;i < TEMP_E502_CH_NUM;i++)
	{
	    pp->first_flag[i] = 1;
	}
	UpdateTempPara(pp); 	
    return TRUE;
}

void GetCurTempUpSpeed(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	float cur_temp = 0; 
	float cur_temp_last50 = 0;
	float cur_temp_last25 = 0;
	int index = 0;
	int index_last50 = 0;
	int index_last25 = 0;

	if (pp == NULL)
	{
		return;
	}
	
	cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp; //料筒curchnl当前温度
	index = pp->tempctrldatastu.tempdata[curchnl].index; //温度数据存放位置
	pp->tempctrldatastu.tempdata[curchnl].temp_buff[index] = cur_temp; //存放温度数据
	
	//当数据缓冲区都有数据时再进行升温速率计算
	if (pp->tempctrldatastu.tempdata[curchnl].temp_buff[Spd_buf_num] > 0)
	{
		index_last50 = (index+1)%(Spd_buf_num+1); 
		cur_temp_last50 = pp->tempctrldatastu.tempdata[curchnl].temp_buff[index_last50]; //50个扫描周期前的温度数据
		index_last25 = (index+1+Spd_buf_num/2)%(Spd_buf_num+1); 
		cur_temp_last25 = pp->tempctrldatastu.tempdata[curchnl].temp_buff[index_last25]; //25个扫描周期前的温度数据
		pp->tempctrldatastu.tempdata[curchnl].temp_up_speed = (float)(cur_temp*3 - cur_temp_last25*4 + cur_temp_last50)/((float)Spd_buf_num); //升温速率，单位为度/扫描周期	
		pp->tempctrldatastu.tempdata[curchnl].temp_up_speed = pp->tempctrldatastu.tempdata[curchnl].temp_up_speed/((float)pp->mh->DATEMP_SCTM_MIN)*2.0f;	//换算成	度/2秒
	}
	
	//计算下一个温度数据的储存位置
	pp->tempctrldatastu.tempdata[curchnl].index = (index+1)%(Spd_buf_num+1);
}


void CalTempRamp(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	float cur_temp = 0, inertia = 0;
	int set_temp = 0;
	float temp_compensate = 0;//扫描周期可能过长，需要补偿温度

	if (pp == NULL)
	{
		return;
	}
	
	if ((pp->g_newtemp.tempRFFunc) || (pp->tempctrldatastu.ramp_effective) || (pp->tempctrldatastu.tempdata[pp->curchnl].ramp_mode < 1)) 
    {
	   return;
    }

	inertia = 0;
	temp_compensate = 0;
	//如果为0，表示正在全功率加热，需要实时计算缓冲区
	//当温度到达缓冲区时，将其置为1
	//缓冲区计算
	if (pp->tempctrldatastu.tempdata[curchnl].flag_reachRamp == 0)
	{
		cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp; //料筒当前温度
		set_temp = pp->md->DATEMP_SETTING[curchnl]; //当前料筒设定温度
		
		if (pp->tempctrldatastu.tempdata[curchnl].temp_up_speed > 0) 
		{
			inertia = pp->tempctrldatastu.tempdata[curchnl].deltTDvDv * pp->tempctrldatastu.tempdata[curchnl].temp_up_speed * pp->tempctrldatastu.tempdata[curchnl].temp_up_speed + pp->tempctrldatastu.tempdata[curchnl].b0;   
		}

        if(inertia < 5) //缓冲区小于5时等于5，避免计算出的缓冲区过小
            inertia = 5;
        
		//补偿的温度值为扫描造成的延迟的一半 + 二次确认所需的一个扫描周期 = 升温速度*扫描周期*0.5 + 升温速度*扫描周期*1 
		temp_compensate = pp->tempctrldatastu.tempdata[curchnl].temp_up_speed * (0.5 + 1);
		if (temp_compensate < 0)
		{
			temp_compensate = 0;
		}
		
		if (cur_temp > (set_temp - (pp->mh->DATEMP_RAMP[curchnl] + 10) - temp_compensate))
		{
			if ((cur_temp + inertia + temp_compensate) >= set_temp)
			{
				pp->tempctrldatastu.tempdata[curchnl].reachRamp_count++;
				if (pp->tempctrldatastu.tempdata[curchnl].reachRamp_count >= 2)
				{
					pp->tempctrldatastu.tempdata[curchnl].reachRamp_count = 0;
					pp->tempctrldatastu.tempdata[curchnl].flag_reachRamp = 1;
					pp->tempctrldatastu.tempdata[curchnl].temp_reach_ramp = inertia;
					pp->tempctrldatastu.tempdata[curchnl].speed_into_ramp = pp->tempctrldatastu.tempdata[curchnl].temp_up_speed;
					pp->tempctrldatastu.tempdata[curchnl].temp_into_ramp = cur_temp;
					//把计算出的缓冲区上传给面板
					#ifdef RAMP_MODEL_TEST
					inertia = 40; //测试用
					#endif
					//pp->mh->DATEMP_RAMP[curchnl] = inertia;
					//PartDataTx((PPART)pp, MH_ID, 1, GET_IND(PTEMPCOMM_MACHSET, DATEMP_RAMP) + curchnl, FALSE); 
				}
			}
			else if (pp->tempctrldatastu.tempdata[curchnl].reachRamp_count <= 1) //保证连续进2次，才关闭电热
			{
				pp->tempctrldatastu.tempdata[curchnl].reachRamp_count = 0;
			}
		}
	}
}


void Adaptive(PTEMPCTRLCOMM pp, UI16 curchnl, float cur_temp, UI16 set_temp)
{
	float deltaT = 0;
	float Ak[2] = {1, 1};
	float myError = 0;
	float beta = 0;
	float speed_square = 0;

	if (pp == NULL)
	{
		return;
	}

	speed_square = pp->tempctrldatastu.tempdata[curchnl].speed_into_ramp * pp->tempctrldatastu.tempdata[curchnl].speed_into_ramp;
	deltaT = cur_temp - pp->tempctrldatastu.tempdata[curchnl].temp_into_ramp; //实际惯性升温量

	if (pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt <= 0)
	{
		pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt = 0;
	}
	
	if ( pp->tempctrldatastu.tempdata[curchnl].speed_into_ramp > 0)
	{
		if (pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt == 0)
		{	
			pp->tempctrldatastu.tempdata[curchnl].deltTDvDv = deltaT / speed_square;
			pp->tempctrldatastu.tempdata[curchnl].b0 = 0;
			pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt = 2;
		}
		else if (pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt >= 2)
		{
			Ak[0] = speed_square;
			Ak[1] = 1;
			myError = cur_temp - (float)set_temp; //实际 - 预测

			//根据误差选择合适的遗忘因子beta
			if (abs(myError) < 1)
			{
				beta = 0.99;
			}
			else if (abs(myError) < 2)
			{
				beta = 0.7;
			}
			else
			{
				beta = 0.5;
			}

			//带遗忘因子的最小二乘法
			LeastSquareMethod(pp, curchnl, Ak, 2, myError, beta);
			
			pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt++;

			#ifdef RAMP_MODEL_TEST
				mprintf("(curchnl %d)RAMP adaptiving complete\n",curchnl);
			#endif
		}
		
		if ((pp->g_newtemp.tempRFFunc) || (pp->tempctrldatastu.ramp_effective))
		{
			pp->tempctrldatastu.tempdata[curchnl].adpative_save_flag = 0;
		}
		else
		{
			pp->tempctrldatastu.tempdata[curchnl].adpative_save_flag = 1;
		}
	}
}

//递推的最小二乘法
void LeastSquareMethod(PTEMPCTRLCOMM pp, UI16 curchnl, float Ak[], UI16 lenOfAk, float b, float beta) 
{
	float Q1[2] = {0,0};
	float Q2 = 0;
	float Q[2] = {0,0};
	float tmp[2] = {0,0};
	float tmp3[2][2] = {{0,0},{0,0}};
	float error = 0;
	float P0[2][2] = {{0,0},{0,0}};
	int i = 0, j = 0, k = 0;
	WORD init_p = 0;

	if (pp == NULL)
	{
		return;
	}

	//矩阵运算Q1=P*Ak' (3*3)*(3*1) 这里将结果写成了1*3
	for (i = 0; i < lenOfAk; i++)
	{
		for (j = 0; j < lenOfAk; j++)
		{
			Q1[i] = Q1[i] + pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j] * Ak[j];
		}
	}
	
	//矩阵运算tmp=Ak*P (1*3)*(3*3) 
	for (i = 0; i < lenOfAk; i++)
	{
		for (j = 0; j < lenOfAk; j++)
		{
			tmp[i] = tmp[i] + Ak[j] * pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j];
		}
	}
	
	//矩阵运算Q2=tmp*Ak' (1*3)*(3*1)
	for (i = 0; i < lenOfAk; i++)
	{
		Q2 = Q2 + tmp[i] * Ak[i];
	}

	//带遗忘因子(beta)的递推的最小二乘法
	Q2 = Q2 + beta;
	
	//矩阵运算Q=Q1/Q2 (1*3)/(1*1)
	for (i = 0; i < lenOfAk; i++)
	{
		Q[i] = Q1[i] / Q2;
	}
	
	error = b;
	
	//矩阵运算x=x+Q*error，更新x
	pp->tempctrldatastu.tempdata[curchnl].deltTDvDv += Q[0] *  error;
	pp->tempctrldatastu.tempdata[curchnl].b0 += Q[1] *  error;

    
	//b0的物理含义为料筒散热和热传递的和，限制其绝对值不超过5度
	if (pp->tempctrldatastu.tempdata[curchnl].b0 > 5.0f)
	{
		pp->tempctrldatastu.tempdata[curchnl].b0 = 5.0f; 
	}
	else if (pp->tempctrldatastu.tempdata[curchnl].b0 < -5.0f)
	{
		pp->tempctrldatastu.tempdata[curchnl].b0 = -5.0f;
	}

	//矩阵运算tmp3=I-Q*Ak
	for (i = 0; i < lenOfAk; i++)
	{
		for (j = 0; j < lenOfAk; j++)
		{
			if (i == j)
			{
				tmp3[i][j] = 1 - Q[i] * Ak[j];
			}
			else
			{
				tmp3[i][j] = 0 - Q[i] * Ak[j];
			}
		}
	}
	
	//矩阵运算P=tmp3*P0 (3*3)*(3*3)，P的迭代
	for (i = 0; i < lenOfAk; i++)
	{
		for (j = 0; j < lenOfAk; j++)
		{
			P0[i][j] = pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j];
			pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j] = 0;
		}
	}
	for (i = 0; i < lenOfAk; i++)
	{
		for (j = 0; j < lenOfAk; j++)
		{
			for (k = 0; k < lenOfAk; k++)
			{
				pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j] += tmp3[i][k] * P0[k][j] / beta;
			}
		}
	}
	
	//以下代码用来检测迭代的P的范围是否合理，如果不合理，就重置P
	//如果P的值超过限制,其绝对值最大可以为32768/1000 = 32
	for (i = 0; i < lenOfAk; i++)
	{
		for (j = 0; j < lenOfAk; j++)
		{
			if (abs(pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j]) > 32) 
			{
			   init_p = 1;	//表明需要初始化P
			}
		}
	}
	
	//如果P的关键值为0
	if ((abs(pp->tempctrldatastu.tempdata[curchnl].P_diss[0][0]) < 0.001f) || (abs(pp->tempctrldatastu.tempdata[curchnl].P_diss[1][1] < 0.001f)))
	{
		init_p = 1;	//表明需要初始化P
	}

	//如果满足以上任一条件，就初始化P
	if (init_p)
	{
		InitP(pp,curchnl);
	}
}

//强制自学习函数
void SpecialPidRF(PTEMPCTRLCOMM pp, UI16 curchnl)
{
    UI16	iii = 0;
    UI8	dopidRF = 0;
    UI16	len = 0;
    fp32	diff_temp = 0, diff_tm = 0;
    fp32	kdt = 0;
    UI8		iiimax = 0;
	float cur_temp = 0;

	if (pp == NULL)
	{
		return;
	}

	if (ChkSysCfg(CO9_RSV3))
	{
		return;
	}

    if (pp->tempctrldatastu.pid_effective == 0 && pp->tempctrldatastu.ramp_effective == 0)
    {
        return;
    }
	
	cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp;

    //申请内存空间，用于温度自学习。学习完毕后清空内存
    if (pp->PRF_para_Comm == NULL)
    {
        pp->PRF_para_Comm = (PTEMPRF_STRU_COMM)GetCacheWord(&len);

        if (sizeof(TEMPRF_STRU_COMM)*TEMP_E502_CH_NUM > len)
        {
            for (iii = 0; iii < TEMP_E502_CH_NUM; iii++)
            {
                pp->g_newtemp.tempRFMode[iii] = 0;
            }
            return;
        }
        else
        {
            memset(pp->PRF_para_Comm, 0, sizeof(TEMPRF_STRU_COMM)* TEMP_E502_CH_NUM);
        }
    }

	//主动优化面板提示
	if (pp->g_newtemp.tempRFFunc) 
	{
		SET_PART_ERR(pp, ER0_STDY_COMM); 
	}

    if (pp->PRF_para_Comm == NULL)
    {
        return;
    }

    switch (pp->g_newtemp.tempRFMode[curchnl])
    {
        case 0:
			pp->tempctrldatastu.temppwr[curchnl].ek = (pp->tempctrldatastu.tempdata[curchnl].set - pp->tempctrldatastu.tempdata[curchnl].test);
			PIDControl(pp,curchnl); //PID控制
			GetKi0(pp,curchnl); //前馈积分量学习	
            break;
        case 1:
			//若在进行缓冲区自学习，需将必要的参数初始化
			if (pp->tempctrldatastu.ramp_effective)
			{
				InitTempStucPara(pp, curchnl); //初始化参数
			}
			
			//PID自学习模块参数初始化
            if ((pp->tempctrldatastu.tempdata[curchnl].test < T2P(VALIDCHKTEMPRF)) && (pp->tempctrldatastu.tempdata[curchnl].test <= pp->tempctrldatastu.tempdata[curchnl].ramp - RAMP_BIAS * 40))
            {
                pp->PRF_para_Comm[curchnl].KDT_current_max = 0;
                pp->PRF_para_Comm[curchnl].start_temp = pp->tempctrldatastu.tempdata[curchnl].test;
                pp->PRF_para_Comm[curchnl].b_current = 0;
                for (iii = 0; iii < 4; iii++)
                {
                    pp->PRF_para_Comm[curchnl].temptestbuf[iii][0] = 0;
                    pp->PRF_para_Comm[curchnl].temptestbuf[iii][1] = 0;
                }
                pp->g_newtemp.tempRFMode[curchnl] = 2;//温度低于50℃，开启自整定
                pp->tempctrldatastu.temppwr[curchnl].pk = MAX_TEMPOUTBW;
            	pp->tempctrldatastu.heatonsts[curchnl]  = 3;
            }
            else
            {
                pp->g_newtemp.tempRFMode[curchnl] = 0;//温度超过50℃，关闭自整定
                pp->tempctrldatastu.pid_effective = 0;
				pp->tempctrldatastu.ramp_effective = 0;
            }
            break;
        case 2://自整定开始
            //记录4个区间，在四个区间中选取最大斜率数据。
            //起始点记录
            pp->tempctrldatastu.temppwr[curchnl].pk = MAX_TEMPOUTBW;
            pp->tempctrldatastu.heatonsts[curchnl]  = 3;
			if (pp->tempctrldatastu.pid_effective) //需要算PID，才PID自学习
			{
	            if (pp->tempctrldatastu.tempdata[curchnl].test < (pp->PRF_para_Comm[curchnl].start_temp + T2P(10)))
	            {
	                pp->PRF_para_Comm[curchnl].temptestbuf[0][0] = pp->tempctrldatastu.tempdata[curchnl].test;
	                pp->PRF_para_Comm[curchnl].temptestbuf[0][1] = Get100MsTick();
	            }
	            else if (pp->tempctrldatastu.tempdata[curchnl].test < (pp->PRF_para_Comm[curchnl].start_temp + T2P(20)))
	            {
	                pp->PRF_para_Comm[curchnl].temptestbuf[1][0] = pp->tempctrldatastu.tempdata[curchnl].test;
	                pp->PRF_para_Comm[curchnl].temptestbuf[1][1] = Get100MsTick();
	            }
	            else if (pp->tempctrldatastu.tempdata[curchnl].test < (pp->PRF_para_Comm[curchnl].start_temp + T2P(35)))
	            {
	                pp->PRF_para_Comm[curchnl].temptestbuf[2][0] = pp->tempctrldatastu.tempdata[curchnl].test;
	                pp->PRF_para_Comm[curchnl].temptestbuf[2][1] = Get100MsTick();
	            }
	            else if (pp->tempctrldatastu.tempdata[curchnl].test < (pp->PRF_para_Comm[curchnl].start_temp + T2P(50)))
	            {
	                pp->PRF_para_Comm[curchnl].temptestbuf[3][0] = pp->tempctrldatastu.tempdata[curchnl].test;
	                pp->PRF_para_Comm[curchnl].temptestbuf[3][1] = Get100MsTick();
	            }

	            //终点值记录
	            if (pp->tempctrldatastu.tempdata[curchnl].test > (pp->PRF_para_Comm[curchnl].start_temp + T2P(70)))
	            {
	                iiimax = 4;
	            }
	            else if (pp->tempctrldatastu.tempdata[curchnl].test > (pp->PRF_para_Comm[curchnl].start_temp + T2P(55)))
	            {
	                iiimax = 3;
	            }
	            else if (pp->tempctrldatastu.tempdata[curchnl].test > (pp->PRF_para_Comm[curchnl].start_temp + T2P(40)))
	            {
	                iiimax = 2;
	            }
	            else if (pp->tempctrldatastu.tempdata[curchnl].test > (pp->PRF_para_Comm[curchnl].start_temp + T2P(30)))
	            {
	                iiimax = 1;
	            }
	            else
	            {
	                iiimax = 0;
	            }

	            for (iii = 0; iii < iiimax; iii++)
	            {
	                //全功率加热斜率k计算
	                diff_temp = (fp32)(pp->tempctrldatastu.tempdata[curchnl].test - pp->PRF_para_Comm[curchnl].temptestbuf[iii][0]);
	                diff_tm = (fp32)(Get100MsTick() - pp->PRF_para_Comm[curchnl].temptestbuf[iii][1]) / 10;
	                kdt = diff_temp / diff_tm;
	                if (kdt > pp->PRF_para_Comm[curchnl].KDT_current[iii])
	                {
	                    pp->PRF_para_Comm[curchnl].KDT_current[iii] = kdt;
	                }
	            }

	            if (iiimax > 0)
	            {
	                if (GetMaxKDT(pp, curchnl, iiimax))
	                {
	                    //全功率加热切线偏移b计算
	                    CalB(pp, curchnl, (UI16)(GetSecTick() - (UI16)pp->g_newtemp.temp_starttime_S), pp->tempctrldatastu.tempdata[curchnl].test);
	                    //计算pid
	                    CalPID(pp,curchnl);
	                }
	            }
			}
			
			//理论上在这里进:如果当前温度>初始温度+70，表明PID自学习完成
			if (pp->tempctrldatastu.tempdata[curchnl].test >= (pp->PRF_para_Comm[curchnl].start_temp + T2P(TEMPRF_UP)))
            {
				pp->tempctrldatastu.temppwr[curchnl].pk = MAX_TEMPOUTBW;
        		pp->tempctrldatastu.heatonsts[curchnl]  = 3;

				//如果还要进行缓冲区自学习，则转步骤8
				if (pp->tempctrldatastu.ramp_effective)
				{
					pp->g_newtemp.tempRFMode[curchnl] = 8;
					pp->tempctrldatastu.tempdata[curchnl].speed_panel = pp->tempctrldatastu.tempdata[curchnl].temp_up_speed;
				}
				else //不进行缓冲区自学习，则直接转步骤0
				{
					pp->tempctrldatastu.tempdata[curchnl].ki_temp = (int)(cur_temp);
					pp->tempctrldatastu.tempdata[curchnl].ki_time =  GetSecTick();
					pp->g_newtemp.tempRFMode[curchnl] = 0;
				}
			}
			else if (pp->tempctrldatastu.tempdata[curchnl].test + T2P(50) >= pp->tempctrldatastu.tempdata[curchnl].set)
			{	//PID有效性检测
				//如果在这里进，说明全功率升温不足70度就关闭
				//这时候的特殊优化(无论PID、Ramp)都无意义,直接退出特殊优化
				if (pp->tempctrldatastu.pid_effective)
				{
					RestorePID(pp); //如果pid学习了一半，这时候pid的值都变化了，需要还原
				}
				pp->tempctrldatastu.pid_effective = 0;
				pp->tempctrldatastu.ramp_effective = 0;
				
				#ifdef RAMP_MODEL_TEST
					mprintf("PID & RAMP self-learning error for full power heating less than 70 degrees\n");
				#endif
				
				return;
			}
            break;
		case 7:
			pp->tempctrldatastu.temppwr[curchnl].pk = 0;
			pp->tempctrldatastu.heatonsts[curchnl]  = 0;
			if (pp->g_newtemp.tempdata[curchnl].dek < TEMP_UP_SPEED_LOWER_LIMIT)
			{
				if (pp->tempctrldatastu.ramp_effective)
				{
					Adaptive(pp, curchnl, pp->tempctrldatastu.tempdata[curchnl].cur_temp, pp->md->DATEMP_SETTING[curchnl]);
					init_ramp_estimate(pp,curchnl); //估计面板上的缓冲区数据
				}
				pp->tempctrldatastu.tempdata[curchnl].ki_temp = (int)(cur_temp);
				pp->tempctrldatastu.tempdata[curchnl].ki_time =  GetSecTick();

				pp->g_newtemp.tempRFMode[curchnl] = 0;
			}

			break;
		case 8:
			pp->tempctrldatastu.temppwr[curchnl].pk = MAX_TEMPOUTBW;
			pp->tempctrldatastu.heatonsts[curchnl]  = 3;
			if (pp->tempctrldatastu.tempdata[curchnl].test + T2P(50) >= pp->tempctrldatastu.tempdata[curchnl].set)
		   	{ 
			   pp->tempctrldatastu.temppwr[curchnl].pk = 0;
			   pp->tempctrldatastu.heatonsts[curchnl]  = 0;
			   pp->tempctrldatastu.tempdata[curchnl].speed_into_ramp = pp->tempctrldatastu.tempdata[curchnl].temp_up_speed;
			   pp->tempctrldatastu.tempdata[curchnl].temp_into_ramp = pp->tempctrldatastu.tempdata[curchnl].cur_temp;
			   pp->g_newtemp.tempRFMode[curchnl] = 7;
		    }
			break;
        case 9:
			break;
        default:
            break;
    }
	
	//若全功率升温没有超过100度 或 在进入缓冲区(50度)前没有全功率加热，则缓冲区自学习失败
	if (STOEffChk(pp))
	{
		#ifdef RAMP_MODEL_TEST
			mprintf("RAMP self-learning error for full power heating less than 70 degrees (or no full power heating before entering RAMP)\n");
		#endif
	}
		
    dopidRF = FALSE;
    for (iii = 0; iii < pp->real_temp_ch_num; iii++)
    {
        if (pp->g_newtemp.tempRFMode[iii] != 0)
        {
            dopidRF = TRUE;
        }
    }

	if (pp->tempctrldatastu.pid_effective == 0 && pp->tempctrldatastu.ramp_effective == 0)
	{
		dopidRF = FALSE; 
	}
		
    if (!dopidRF)
    {
    	for (iii = 0; iii < pp->real_temp_ch_num; iii++)
        {
			if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))//如果PID，排除油温等
			{
				pp->tempctrldatastu.tempdata[iii].temp_warmon = pp->tempctrldatastu.tempdata[iii].cur_temp;
				pp->tempctrldatastu.tempdata[iii].flag_adaptive = 0;
				InitP(pp,iii);
			}
		}

		if (pp->tempctrldatastu.pid_effective)
		{
			PartDataTx((PPART)pp, MH_ID, CAL_LEN(pp->mh->DAPID_KP) * 3, GET_IND(PTEMPCOMM_MACHSET, DAPID_KP), FALSE);
        	pp->g_newtemp.tempdata[curchnl].RFend_flag = 1;
			TempPIDSave(pp);
			#ifdef RAMP_MODEL_TEST
				mprintf("PID self-learning complete\n");
			#endif
		}
		
        if (pp->tempctrldatastu.ramp_effective)
        {
        	SaveTempPara(pp);
			#ifdef RAMP_MODEL_TEST
				mprintf("RAMP self-learning complete\n");
			#endif
        }
		
        FreeCacheWord(pp->PRF_para_Comm);
        pp->PRF_para_Comm = NULL;
		InitTempDataStu(pp);
		
		if (pp->g_newtemp.tempRFFunc)
		{
			pp->g_newtemp.tempRFFunc = 0;
	        CLR_PART_ERR(pp, ER0_STDY_COMM);
			if(pp->st->DATEMP_YOUHUAOK == 0x01)	//普通优化完成后，赋值为3，用于面板上点亮为绿色
				pp->st->DATEMP_YOUHUAOK = 0x03;
			else if(pp->st->DATEMP_YOUHUAOK == 0x02)	//强制优化完成后，赋值为4，用于面板上点亮为橙色
				pp->st->DATEMP_YOUHUAOK = 0x04;
		}
		
		pp->tempctrldatastu.pid_effective = 0;
		pp->tempctrldatastu.ramp_effective = 0;
    }

}

//初始化温度控制算法所需的参数
void InitTempStucPara(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	UI8 i = 0;

	if (pp == NULL)
	{
		return;
	}
	
	pp->tempctrldatastu.tempdata[curchnl].index = 0;  //温度数据存放位置，用于实时升温速度计算

	for (i = 0; i < Spd_buf_num + 1; i++)
	{
		pp->tempctrldatastu.tempdata[curchnl].temp_buff[i] = 0;
	}

	InitP(pp,curchnl);

	pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt = 0;
	pp->tempctrldatastu.tempdata[curchnl].deltTDvDv = 0;
	pp->tempctrldatastu.tempdata[curchnl].b0 = 0;
	pp->tempctrldatastu.tempdata[curchnl].ki0 = 0;
	pp->tempctrldatastu.tempdata[curchnl].flag_ki = 0;
	pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
}

//初始化递推最小二乘法的矩阵P
void InitP(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	int i = 0, j = 0;

	if (pp == NULL)
	{
		return;
	}

	for (i = 0; i < 2; i++)
	{	
		for (j = 0; j < 2; j++)
		{
			if (i == j)
			{
				pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j] = 1;
			}
			else
			{
				pp->tempctrldatastu.tempdata[curchnl].P_diss[i][j] = 0;
			}
		}
	}
}

//任意两段料筒温度差<15,返回TRUE
WORD TempWarmOnConsist(PTEMPCTRLCOMM pp)
{
    UI8 iii = 0;
	UI8 jjj = 0;
    WORD ret = TRUE;

	if (pp == NULL)
	{
		return 0;
	}
	
	//任意两段料筒温度差<15,返回TRUE
	for (iii = 1; iii < pp->real_temp_ch_num - 1; iii++)
	{
		if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))//如果PID，排除油温等
		{
			for (jjj = iii + 1; jjj < pp->real_temp_ch_num; jjj++)
			{
				if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[jjj]))//如果PID，排除油温等
				{
					if (abs(pp->tempctrldatastu.tempdata[iii].cur_temp - pp->tempctrldatastu.tempdata[jjj].cur_temp) > 15.0f)
					{
						ret = FALSE;
					}
				}
			}
		}
	}
    return ret;
}

//将程序中的变量存入FLASH(自适应使用)
void SaveAdpative(PTEMPCTRLCOMM pp)
{
	UI8 iii = 0;
	WORD flag_save = 0;

	if (pp == NULL)
	{
		return;
	}
	
	for (iii = 0; iii < TEMP_E502_CH_NUM; iii++)
	{
		if((pp->tempctrldatastu.tempdata[iii].adpative_save_flag == 1)&& ChkOperMode(OM_MANUAL))
		{
			flag_save = 1;
			pp->tempctrldatastu.tempdata[iii].adpative_save_flag = 0;
			pp->zd->LIN_FIT_K[iii] = (UI16)(pp->tempctrldatastu.tempdata[iii].deltTDvDv * (float)1e2);
			pp->zd->LIN_FIT_B[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].b0 * (float)1e2);
			pp->zd->P11[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].P_diss[0][0] * (float)1e3);
			pp->zd->P12[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].P_diss[0][1] * (float)1e3);
			pp->zd->P22[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].P_diss[1][1] * (float)1e3);
			pp->zd->STD_WARMON_CNT[iii] = pp->tempctrldatastu.tempdata[iii].std_warmon_cnt;
		}
	}
	
	if (flag_save)
	{
		PartCfgSave((PPART)pp, ZD_ID);	
	}
}

//将单位前馈积分存入FLASH
void SaveIk0(PTEMPCTRLCOMM pp)
{
	UI8 iii = 0;
	WORD flag_save = 0;

	if (pp == NULL)
	{
		return;
	}
		
	for (iii = 1; iii < TEMP_E502_CH_NUM; iii++)
	{
		if((pp->tempctrldatastu.tempdata[iii].ki0_save_flag == 1)&& ChkOperMode(OM_MANUAL))
		{
			flag_save = 1;
			pp->tempctrldatastu.tempdata[iii].ki0_save_flag = 0;
			pp->zd->IK0[iii] = pp->tempctrldatastu.tempdata[iii].ki0;	
		}
	}
	
	if (flag_save)
	{
		PartCfgSave((PPART)pp, ZD_ID);	
	}
}

//将程序中的变量存入FLASH
void SaveTempPara(PTEMPCTRLCOMM pp)
{
	UI8 iii = 0 ;

	if (pp == NULL)
	{
		return;
	}
	
	for (iii = 0; iii < TEMP_E502_CH_NUM; iii++)
	{ 		 
		pp->zd->LIN_FIT_K[iii] = (UI16)(pp->tempctrldatastu.tempdata[iii].deltTDvDv * (float)1e2);
		pp->zd->LIN_FIT_B[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].b0 * (float)1e2);
		pp->zd->IK0[iii] = pp->tempctrldatastu.tempdata[iii].ki0;	
		pp->zd->P11[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].P_diss[0][0] * (float)1e3);
		pp->zd->P12[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].P_diss[0][1] * (float)1e3);
		pp->zd->P22[iii] = 32768 + (pp->tempctrldatastu.tempdata[iii].P_diss[1][1] * (float)1e3);
		pp->zd->STD_WARMON_CNT[iii] = pp->tempctrldatastu.tempdata[iii].std_warmon_cnt;
	} 
	//memcpy(&(pp->zd->DATEMP_RAMP[0]),&(pp->mh->DATEMP_RAMP[0]),CAL_LEN(pp->mh->DATEMP_RAMP)*4*sizeof(UI16));
	PartCfgSave((PPART)pp, ZD_ID);				
}

//从DIC中读取缓冲区模型所需的参数
void UpdateTempPara(PTEMPCTRLCOMM pp)
{
	UI8 iii = 0 ;	

	if (pp == NULL)
	{
		return;
	}
	
	for (iii = 1; iii < TEMP_E502_CH_NUM; iii++)
	{
		pp->tempctrldatastu.tempdata[iii].std_warmon_cnt = pp->zd->STD_WARMON_CNT[iii];
		if (pp->tempctrldatastu.tempdata[iii].std_warmon_cnt <= 0)
		{
			pp->tempctrldatastu.tempdata[iii].std_warmon_cnt = 0;
			pp->tempctrldatastu.tempdata[iii].P_diss[0][0] = 1;
			pp->tempctrldatastu.tempdata[iii].P_diss[0][1] = 0;
			pp->tempctrldatastu.tempdata[iii].P_diss[1][0] = 0;
			pp->tempctrldatastu.tempdata[iii].P_diss[1][1] = 1;
			pp->tempctrldatastu.tempdata[iii].deltTDvDv = 0;
			pp->tempctrldatastu.tempdata[iii].b0 = 0;
			pp->tempctrldatastu.tempdata[iii].ki0 = 0;
		}
		else
		{ 
		  	pp->tempctrldatastu.tempdata[iii].deltTDvDv = (float)pp->zd->LIN_FIT_K[iii] * (float)1e-2;
		  	pp->tempctrldatastu.tempdata[iii].b0 = (float)(pp->zd->LIN_FIT_B[iii]-32768) * (float)1e-2;					
		  	pp->tempctrldatastu.tempdata[iii].ki0 = (UI16)pp->zd->IK0[iii];
		  	pp->tempctrldatastu.tempdata[iii].P_diss[0][0] = (float)(pp->zd->P11[iii]-32768) * (float)1e-3;
		  	pp->tempctrldatastu.tempdata[iii].P_diss[0][1] = (float)(pp->zd->P12[iii]-32768) * (float)1e-3;
		  	pp->tempctrldatastu.tempdata[iii].P_diss[1][0] = (float)(pp->zd->P12[iii]-32768) * (float)1e-3;
		  	pp->tempctrldatastu.tempdata[iii].P_diss[1][1] = (float)(pp->zd->P22[iii]-32768) * (float)1e-3;
		} 	
	}
}

WORD FindFirstCurchnl(PTEMPCTRLCOMM pp)
{
	//一般情况下，射嘴为段数最小的那个
	UI8 firstCurchnl = 0;

	if (pp == NULL)
	{
		return 0;
	}
	
	for (firstCurchnl = 1; firstCurchnl < pp->real_temp_ch_num; firstCurchnl++)
	{
		if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[firstCurchnl]))//如果PID，排除油温等
    	{
			break;
    	}
    }
	return firstCurchnl;
}

//缓冲区模型参数初始化&&加热模式选择
void WarmOnNewRampSet(PTEMPCTRLCOMM pp)
{
	float cur_temp = 0;
	UI16 set_temp = 0;
	WORD i = 0, curchnl = 0;

	if (pp == NULL)
	{
		return;
	}

	//根据code码判断是否关闭缓冲区模型，用面板上的缓冲区
	if (ChkSysCfg(CO9_RSV3))
	{
		pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0; 
		pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
		#ifdef RAMP_MODEL_TEST
			mprintf("original mode: according to the RAMP on the panel\n");
		#endif
		return;
	}

	pp->tempctrldatastu.temp_warmon_consist = TempWarmOnConsist(pp); //检测料筒温度是否一致，若是，才可能使用自适应功能
	pp->tempctrldatastu.first_curchnl = FindFirstCurchnl(pp);        //找到第一段料筒

	pp->tempctrldatastu.pid_effective = 0;                           //PID自学习有效性参数初始化
	pp->tempctrldatastu.ramp_effective = 0;                          //缓冲区自学习有效性参数初始化
	pp->tempctrldatastu.tao_confirm = 1;                             //初始化，要确保tao准确
	
	for (curchnl = 1; curchnl < pp->real_temp_ch_num; curchnl++)
	{
    	if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[curchnl]))  
    	{
			cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp;          //当前温度，精度:0.01度
			set_temp = pp->md->DATEMP_SETTING[curchnl];                         //设定温度
			pp->tempctrldatastu.tempdata[curchnl].temp_warmon = (UI16)cur_temp; //电热开启时料筒温度
			pp->tempctrldatastu.tempdata[curchnl].flag_reachRamp = 0;           //示性函数:是否到达缓冲区，若到(=1)，则关闭全功率加热
			pp->tempctrldatastu.tempdata[curchnl].reachRamp_count = 0;          //连续两次到达缓冲区，才会关电热
			pp->tempctrldatastu.tempdata[curchnl].adpative_save_flag = 0;       //自适应后的参数储存标志
			pp->tempctrldatastu.tempdata[curchnl].ki0_save_flag = 0;            //维稳自学习后的参数储存标志
			pp->tempctrldatastu.tempdata[curchnl].index = 0;
			pp->tempctrldatastu.tempdata[curchnl].flag_adaptive = 1;
			pp->tempctrldatastu.tempdata[curchnl].flag_ki = 0; 
            pp->tempctrldatastu.tempdata[curchnl].overtemp_inhibition = 1;      //超温抑制功能

			//升温速率buffer清空
			for (i = 0; i < Spd_buf_num + 1; i++) 
			{
				pp->tempctrldatastu.tempdata[curchnl].temp_buff[i] = 0; 
			}

			//判断是否能使用缓冲区模型
			if (ABS_DIFF((int)cur_temp,set_temp) >= (100 + pp->mh->DATEMP_RAMP[curchnl])         //1.设定温度 > 当前温度 + 100 + Ramp(面板上)，确保充分加热
				&& (pp->tempctrldatastu.tempdata[curchnl].deltTDvDv > 0)                       //2.缓冲区模型关键参数必须正确
				&& (pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt >= 2))                //3.已经优化过
			{
				pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 1; //新缓冲区加热
				pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 1; //前馈积分

				//判断是否能自适应
				if (pp->tempctrldatastu.temp_warmon_consist) //任意两段料筒温度相差不超过15度，才自适应，确保自适应数据纯净
				{
					pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 2; //新缓冲区加热，并对结果自适应
				}
			}
			else 
			{
				pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0; //否则，走老模式
				pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0; //不给前馈
			}

			//第一段走老模式
			if (curchnl == pp->tempctrldatastu.first_curchnl) 
			{
				pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0; 
				pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
			}

            if (pp->g_newtemp.tempConstraintFunc == TRUE) //同步升温开启时，走老模式
            {
				pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0; 
				pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
			}
		}
	}
}

//电热开启时，若改变设定温度，需进行温度控制模式判断
void SetTempChgNewRampSet(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	UI16 temp_warmon = 0;
	float cur_temp = 0;
	UI16 set_temp = 0;

	if (pp == NULL)
	{
		return;
	}

	//以下代码，需在电热开启时进行
	if (pp->st->FL_HEATERSTATUS == 0) 
	{
		return;
	}

	//关闭新缓冲区模式，用面板上的参数
	if (ChkSysCfg(CO9_RSV3))
	{
		pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0; 
		pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
		return;
	}

	//未优化，就进行RAMP=50的普通加热
	if ((pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt <= 0) && (!ChkSysCfg(CO9_RSV3)))
	{
		pp->mh->DATEMP_RAMP[curchnl] = 50;
		pp->tempctrldatastu.tempdata[curchnl].ramp = pp->mh->DATEMP_RAMP[curchnl];	
	}
	
	cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp; //当前温度
	set_temp = pp->md->DATEMP_SETTING[curchnl]; //设定温度
	
	if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[curchnl]))
	{
	    pp->tempctrldatastu.tempdata[curchnl].overtemp_inhibition = 1;      //超温抑制功能
	    
		temp_warmon = pp->tempctrldatastu.tempdata[curchnl].temp_warmon;
		if ((pp->g_newtemp.fnextheatmodenew[curchnl] <= 1)                      //还在全功率加热
			&& (set_temp - temp_warmon) > (100 + pp->mh->DATEMP_RAMP[curchnl])   //保证充分加热
			&& pp->tempctrldatastu.tempdata[curchnl].deltTDvDv > 0              //缓冲区模型关键参数必须正确
			&& pp->tempctrldatastu.tempdata[curchnl].std_warmon_cnt >= 2       //缓冲区自学习完毕
			&& (set_temp > cur_temp + pp->mh->DATEMP_RAMP[curchnl]))
		{
			//新缓冲区模型加热
			pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 1;
			pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 1;
			if (pp->tempctrldatastu.temp_warmon_consist)
			{
				//新缓冲区模型加热+自适应
				pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 2;
			}
		}
		else
		{
			//不用新缓冲区模型加热
			pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0;
			pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
		}

		//第一段料筒不用新缓冲区模型加热
		if (curchnl == pp->tempctrldatastu.first_curchnl)
		{
			pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0; 
			pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
		}
        
        //同步升温开启时，走老模式
        if (pp->g_newtemp.tempConstraintFunc == TRUE) 
        {
			pp->tempctrldatastu.tempdata[curchnl].ramp_mode = 0; 
			pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;
		}
	}
}

//PID控制
void PIDControl(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	int	DeltI = 0;				/*浮点数转为整数，放大10000倍*/
	float cur_temp = 0;
	float room_temp = 0;
    int kp = 0, ki = 0, kd = 0;

	if (pp == NULL)
	{
		return;
	}
	
	cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp; //料筒当前温度
	room_temp = (float)(pp->st->WTEMP_LM73 / 10.0); //室温

    kp = pp->tempctrldatastu.pidpara[curchnl].p;
    ki = pp->tempctrldatastu.pidpara[curchnl].ki * 10;
    kd = pp->tempctrldatastu.pidpara[curchnl].kd;

	//分段积分
	if (abs(pp->tempctrldatastu.temppwr[curchnl].ek) < (MAX_TEMPOUTBW * 20 / kp))
	{
		ki = ki;
	}
	else if (abs(pp->tempctrldatastu.temppwr[curchnl].ek) < (MAX_TEMPOUTBW * 40 / kp))
	{
		ki = ki / 2;
	}
	else if (abs(pp->tempctrldatastu.temppwr[curchnl].ek) < (MAX_TEMPOUTBW * 60 / kp))
	{
		ki = ki / 4;
	}
	else if (abs(pp->tempctrldatastu.temppwr[curchnl].ek) < (MAX_TEMPOUTBW * 80 / kp))
	{
		ki = ki / 8;
	}
	else if (abs(pp->tempctrldatastu.temppwr[curchnl].ek) < (MAX_TEMPOUTBW * 100 / kp))
	{
		ki = ki / 13;
	}

	if (ki < 1)
	{
		ki = 1;
	}

	if (abs(pp->tempctrldatastu.temppwr[curchnl].ek) > (MAX_TEMPOUTBW * 100 / kp))
	{
		ki = 0;
	}

	DeltI = ki * pp->tempctrldatastu.temppwr[curchnl].ek / 10;

    if (pp->tempctrldatastu.temppwr[curchnl].ek < 0)  //设定温度 < 实际温度
	{
		pp->tempctrldatastu.tempdata[curchnl].overtemp_inhibition = 0;
	}

	if ((pp->tempctrldatastu.tempdata[curchnl].overtemp_inhibition == 1) && (ChkSysCfg(CO9_FUZZY)))
	{
		DeltI = DeltI * 0.2f;
		//kd = kd * 1.5f;
	}
    
	pp->tempctrldatastu.temppwr[curchnl].ik = pp->tempctrldatastu.temppwr[curchnl].ik + DeltI;
	if ((!pp->g_newtemp.tempRFFunc) && (!pp->tempctrldatastu.ramp_effective))
	{
		//新温度控制下，首次进入PID，给予前馈积分
		if (pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid == 1) 
		{
			pp->tempctrldatastu.tempdata[curchnl].flag_first_into_pid = 0;

			//前馈积分量 = 单位前馈积分量 * (当前温度 - 室温)
			if (pp->tempctrldatastu.tempdata[curchnl].ki0 > 0)
			{
				pp->tempctrldatastu.temppwr[curchnl].ik = (int)(pp->tempctrldatastu.tempdata[curchnl].ki0 * (cur_temp - room_temp));
			}
			
			if (pp->tempctrldatastu.temppwr[curchnl].ik < 0)
			{
				pp->tempctrldatastu.temppwr[curchnl].ik = 0;
			}
		}
	}

	//通过PID参数计算控制量
	pp->tempctrldatastu.temppwr[curchnl].pk = (kp * (int)pp->tempctrldatastu.temppwr[curchnl].ek + 50) / 100 
											+ (pp->tempctrldatastu.temppwr[curchnl].ik + 50) / 100
										    + (kd * (int)(pp->tempctrldatastu.temppwr[curchnl].ek - pp->tempctrldatastu.temppwr[curchnl].ek0) + 50) / 100;

	//积分饱和清积分
	if ((pp->tempctrldatastu.temppwr[curchnl].pk > MAX_TEMPOUTBW) && (DeltI > 0) )
	{
		pp->tempctrldatastu.temppwr[curchnl].ik = pp->tempctrldatastu.temppwr[curchnl].ik - DeltI;
		pp->tempctrldatastu.temppwr[curchnl].pk = MAX_TEMPOUTBW; //这样饱和的时候才能全功率加热
	}
	if (pp->tempctrldatastu.temppwr[curchnl].ik < 0 )
	{
		pp->tempctrldatastu.temppwr[curchnl].ik = 0;
	}

	if (pp->tempctrldatastu.temppwr[curchnl].pk == 0)
	{
		pp->tempctrldatastu.heatonsts[curchnl] = 0;
	}
	else
	{
		pp->tempctrldatastu.heatonsts[curchnl] = 1;
	}
	if (pp->tempctrldatastu.tempdata[curchnl].test >= (pp->tempctrldatastu.tempdata[curchnl].set + pp->tempctrldatastu.errmax))
	{
		pp->tempctrldatastu.temppwr[curchnl].pk  = 0;
		pp->tempctrldatastu.heatonsts[curchnl]	 = TEMP_ST_OVERTOP;
		SetheatS(pp,curchnl, TEMP_ST_OVERTOP);
	}
	else
	{
		CLR_TMP_ST(pp,curchnl, TEMP_ST_OVERTOP);
	}
	
	//>>>>>-------------以下代码为控制器输出限幅----------
	if (pp->tempctrldatastu.temppwr[curchnl].pk > 10000)
	{
		pp->tempctrldatastu.temppwr[curchnl].pk = 10000;
	}
	if (pp->tempctrldatastu.temppwr[curchnl].pk < 0)
	{
		pp->tempctrldatastu.temppwr[curchnl].pk = 0;
	}
}

//单位前馈积分量学习
void GetKi0(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	float cur_temp = 0;
	float room_temp = 0;

#ifdef  RAMP_MODEL_TEST
	int iii = 0;
	UI8 flag_start = 1;
	UI8 flag_complete = 1;
#endif

	if (pp == NULL)
	{
		return;
	}
	
	cur_temp = pp->tempctrldatastu.tempdata[curchnl].cur_temp; //料筒当前温度
	room_temp = (float)(pp->st->WTEMP_LM73 / 10.0); //室温

	//若前馈积分量未学习，则学习单位前馈积分量 = 稳定时的积分量 / (当前温度 - 室温)
	if ((pp->tempctrldatastu.tempdata[curchnl].ki0 == 0) && (pp->tempctrldatastu.tempdata[curchnl].cur_temp > 100))
	{
		//判断温度稳定的依据:KITIME时间内温度波动不超过3度
		if (pp->tempctrldatastu.tempdata[curchnl].flag_ki == 0)
		{
			if (abs(cur_temp - (float)(pp->tempctrldatastu.tempdata[curchnl].ki_temp)) > 3.0f)
			{
				pp->tempctrldatastu.tempdata[curchnl].ki_time = GetSecTick();
				pp->tempctrldatastu.tempdata[curchnl].ki_temp = (int)(cur_temp);
			}
			
			if ((WORD)(GetSecTick() - pp->tempctrldatastu.tempdata[curchnl].ki_time) > KITIME)
			{
				pp->tempctrldatastu.tempdata[curchnl].flag_ki = 1; //非0表示稳态，之后用来计数
				pp->tempctrldatastu.tempdata[curchnl].ki_temp = 0; //之后用来累加ki
				pp->tempctrldatastu.tempdata[curchnl].ki_time = GetSecTick();

				//判断所有料筒是否都开始学习完前馈积分
				#ifdef RAMP_MODEL_TEST
					for (iii = 0; iii < pp->real_temp_ch_num; iii++)
				    {
				        if ((CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii])) && (pp->tempctrldatastu.tempdata[iii].flag_ki == 0))
				        {
				            flag_start = 0; //存在一段没开始学习前馈积分
				        }
				    }
					if (flag_start)
					{
						mprintf("feedforward-integral self-learning start\n",curchnl);
					}
				#endif
			}
		}
		else //温度稳定，学习前馈积分量
		{
			if ((WORD)(GetSecTick() -pp->tempctrldatastu.tempdata[curchnl].ki_time) < KITIME)
			{
				pp->tempctrldatastu.tempdata[curchnl].flag_ki++; //用来计数
				pp->tempctrldatastu.tempdata[curchnl].ki_temp += (pp->tempctrldatastu.temppwr[curchnl].ik + 50) / 100; //用来累加ki
			}
			else
			{
				//单位前馈积分量 = 稳定时的积分量 / (当前温度 - 室温)
				pp->tempctrldatastu.tempdata[curchnl].ki0 = (UI16)((float)pp->tempctrldatastu.tempdata[curchnl].ki_temp / (float)(pp->tempctrldatastu.tempdata[curchnl].flag_ki - 1) / (cur_temp - room_temp) * 100.0f);	
				if (pp->tempctrldatastu.tempdata[curchnl].ki0 <= 0)
				{
					pp->tempctrldatastu.tempdata[curchnl].ki0 = 1;
				}
				pp->tempctrldatastu.tempdata[curchnl].ki0_save_flag = 1;

				//判断所有料筒是否都已经学习完前馈积分
				#ifdef RAMP_MODEL_TEST
					for (iii = 0; iii < pp->real_temp_ch_num; iii++)
				    {
				        if ((CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii])) && (pp->tempctrldatastu.tempdata[iii].ki0 == 0))
				        {
				            flag_complete = 0; //存在一段没学完前馈积分
				        }
				    }
					if (flag_complete)
					{
						mprintf("feedforward-integral self-learning complete\n",curchnl);
					}
				#endif
			}
		}
	}
}

//新算法在全功率加热过程中，若检测到进入缓冲区，立刻关闭电热
void TurnToRamp(PTEMPCTRLCOMM pp)
{ 	
	UI8 iii = 1;

	if (pp == NULL)
	{
		return;
	}
	
	for (iii = 1; iii < pp->real_temp_ch_num; iii++)
	{
		if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
		{   //新算法在全功率加热过程中，若检测到进入缓冲区，需及时关闭电热
			if ((pp->tempctrldatastu.tempdata[iii].ramp_mode >= 1) && (pp->g_newtemp.fnextheatmodenew[iii] == 1) && (pp->tempctrldatastu.tempdata[iii].flag_reachRamp == 1))
			{
				pp->tempctrldatastu.temppwr[iii].pk = 0;
				pp->g_newtemp.fnextheatmodenew[iii] = 2;
			}
		}
	}
}

//判断温度PID是否需要自学习
WORD STO_PID_Need(PTEMPCTRLCOMM pp)
{
	UI8 iii = 0;
	UI16 max = 0, min = 10000;

	if (pp == NULL)
	{
		return 0;
	}

    //关闭新缓冲区模式，用面板上的参数，无需进一步判断是否能进行缓冲区自学习
	if (ChkSysCfg(CO9_RSV3))
	{
		return 0;
	}

	if (pp->g_newtemp.tempRFFunc) //若满足强制优化条件，直接返回1，学习PID
	{
		pp->tempctrldatastu.pid_effective = 1;
		
		for (iii = 1; iii < pp->real_temp_ch_num; iii++) //自学习前备份PID参数
		{	
			pp->tempctrldatastu.tempdata[iii].pid_kp = pp->mh->DAPID_KP[iii];
			pp->tempctrldatastu.tempdata[iii].pid_ti = pp->mh->DAPID_TI[iii];
			pp->tempctrldatastu.tempdata[iii].pid_td = pp->mh->DAPID_TD[iii];
		}
		
		return (pp->tempctrldatastu.pid_effective);
	}
    
	//要确保PID自学习的滞后时间tao准确: 开启电热后，只判断一次是否需要PID自学习
	if (pp->tempctrldatastu.tao_confirm == 0)
	{
		return 0;
	}
	else
	{
		pp->tempctrldatastu.tao_confirm = 0;
	}
	
	//PID已自学习 && 没有开启"每次加热都要进行PID自学习"的CODE码
	if ((ChkPidIsRF(pp) != 0) && !ChkSysCfg(CO9_TIP)) 
	{
		return 0;
	}

	//统计料筒当前最高和最低温度
	for (iii = 1; iii < pp->real_temp_ch_num; iii++)
	{
		if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
		{	
			if (pp->st->DATEMP_REAL[iii]> max)
				max = pp->st->DATEMP_REAL[iii];
			if (pp->st->DATEMP_REAL[iii] < min)
				min = pp->st->DATEMP_REAL[iii];
		}
	}

	//最高温度<室温+10 && 最大温差<10，则可进行PID自学习
	if ((max < (pp->st->WTEMP_LM73 / 10 + 10)) && (ABS_DIFF(max, min) < 10))
	{
		pp->tempctrldatastu.pid_effective = 1;

		for (iii = 1; iii < pp->real_temp_ch_num; iii++) //自学习前备份PID参数
		{	
			pp->tempctrldatastu.tempdata[iii].pid_kp = pp->mh->DAPID_KP[iii];
			pp->tempctrldatastu.tempdata[iii].pid_ti = pp->mh->DAPID_TI[iii];
			pp->tempctrldatastu.tempdata[iii].pid_td = pp->mh->DAPID_TD[iii];
		}
	}
	
	return (pp->tempctrldatastu.pid_effective);
}

//判断温度缓冲区是否需要自学习
WORD STO_RAMP_Need(PTEMPCTRLCOMM pp)
{
	UI8 iii = 0;

	if (pp == NULL)
	{
		return 0;
	}

    //关闭新缓冲区模式，用面板上的参数，无需进一步判断是否能进行缓冲区自学习
	if (ChkSysCfg(CO9_RSV3))
	{
		return 0;
	}
    
	//若存在一段料筒没有自学习过，则表明缓冲区需要自学习
	for (iii = 1; iii < pp->real_temp_ch_num; iii++)
	{
		if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
		{   
			if (pp->tempctrldatastu.tempdata[iii].std_warmon_cnt <= 0) 
			{
				pp->tempctrldatastu.ramp_effective = 1;
			}
		}
	}

	//所有料筒温度均小于60，且设定温度 > 当前温度 + 50，才能进行缓冲区自学习
	for (iii = 1; iii < pp->real_temp_ch_num; iii++)
	{
		if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
		{   
			if ((pp->tempctrldatastu.tempdata[iii].cur_temp > VALIDCHKTEMPRF) || (pp->tempctrldatastu.tempdata[iii].set < pp->tempctrldatastu.tempdata[iii].test + T2P(50)))
			{
				pp->tempctrldatastu.ramp_effective = 0;
			}
		}
	}

	STOEffChk(pp); //缓冲区自学习有效性检查
	
	return (pp->tempctrldatastu.ramp_effective);
}


//电热开启时，一直检测是否进行PID自学习和缓冲区自学习
void STOStart(PTEMPCTRLCOMM pp)
{
	UI8 iii = 0;
	UI8 flag_sto_start = 0;

	if (pp == NULL)
	{
		return;
	}

	//如果没在进行PID自学习，就检测是否需要PID自学习，若需要，则pid_effective = 1
	if (pp->tempctrldatastu.pid_effective == 0)
	{ 
		if (STO_PID_Need(pp))  
		{
			pp->g_newtemp.temp_starttime_S = GetSecTick(); 
			flag_sto_start = 1;
			#ifdef RAMP_MODEL_TEST
				mprintf("PID self-learning start\n");
			#endif
		}
	}

	//如果没在进行缓冲区自学习，就检测是否需要RAMP自学习，若需要，则ramp_effective=1
	if (pp->tempctrldatastu.ramp_effective == 0)
	{
		 if (STO_RAMP_Need(pp))
		 {
			flag_sto_start = 1;
			#ifdef RAMP_MODEL_TEST
				mprintf("RAMP self-learning start\n");
			#endif
		 }
	}
 
	//如果(PID和缓冲区)任意一个需要自学习，开启强制自学习
	if (flag_sto_start)
	{
		pp->YouHuaFlag = 0x01;
		for (iii = 1; iii < pp->real_temp_ch_num; iii++)
    	{
			if(CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
	    	{
	    		pp->g_newtemp.tempRFMode[iii] = 1;
	    	}
		}
	}
}

//缓冲区自学习有效性检查
WORD STOEffChk(PTEMPCTRLCOMM pp) 
{
	UI8 iii = 0;

	if (pp == NULL)
	{
		return 0;
	}

	//缓冲区自学习已失效，则不必再检查其有效性
	if (pp->tempctrldatastu.ramp_effective == 0)
	{
		return 0;
	}

	for (iii = 1; iii < pp->real_temp_ch_num; iii++)
    {
		if ((CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii])) && (pp->g_newtemp.tempRFMode[iii] != 1)) 
    	{
    		//若全功率升温没有超过70度 或 在进入缓冲区(50度)前没有全功率加热，则缓冲区自学习结果失效
    		if ((pp->tempctrldatastu.tempdata[iii].test <= (pp->PRF_para_Comm[iii].start_temp + T2P(70))) || (pp->tempctrldatastu.tempdata[iii].test + T2P(50) < pp->tempctrldatastu.tempdata[iii].set))
    		{
				if (pp->tempctrldatastu.temppwr[iii].pk < MAX_TEMPOUTBW)
				{
					pp->tempctrldatastu.ramp_effective = 0; //缓冲区自学习结果失效
					return 1;
				}
			}
		}
	}
	return 0;
}

//PID自学习失败后，将PID参数还原
void RestorePID(PTEMPCTRLCOMM pp) 
{
	UI8 iii = 0;

	//还原DIC
	for (iii = 1; iii < pp->real_temp_ch_num; iii++) //自学习前备份PID参数
	{	
		pp->mh->DAPID_KP[iii] = pp->tempctrldatastu.tempdata[iii].pid_kp;
		pp->mh->DAPID_TI[iii] = pp->tempctrldatastu.tempdata[iii].pid_ti;
		pp->mh->DAPID_TD[iii] = pp->tempctrldatastu.tempdata[iii].pid_td;
	}

	//还原内存中的PID参数
	InitPidParaPro(pp); 
}

//查表；输入微伏值，输出float型的温度
float ADtoTemp_float(WORD ADVALUE)
{
	WORD   i,imin,imax,lp;
	DWORD  tp1,tp2;
	float xtemp = 0;

	/*条件判读从> 改成>=,解决ADVALUE == T_tab[MAX_TEMPTBL]时，
	下面的while语句死循环的问题*/
	if(ADVALUE >= T_tab[MAX_TEMPTBL])
	{
	  	xtemp = 999;
	}
	else
	{
		imax = MAX_TEMPTBL;     //96
    	imin = 0;
    	i = imax / 2;   		//48
    	lp = 1;
    	while (lp)
    	{	  
  			if (T_tab[i] > ADVALUE)
 			{
   	  			imax = i;
  	  			i = imin + (imax-imin) / 2;
  			}
  			else if (T_tab[i+1] <= ADVALUE)
  			{
  	  			imin = i;
  	  			i = imin + (imax-imin)/2;
  			}	
  			else
  	  			lp = 0;
		}	
		tp1 = (WORD)(ADVALUE - T_tab[i]);
		tp2 = (WORD)(T_tab[i+1] - T_tab[i]);
	 	//xtemp = (WORD)(((i * 50 + tp1 * 50 / tp2) + 5) / 10);   		//5度范围内插值补偿后得到温度 
		xtemp = i * 5 + (float)tp1 / tp2 * 5;
	}
	
    return(xtemp);
}  

//判断是否完成缓冲区优化
WORD Ramp_Optimize_OK(PTEMPCTRLCOMM pp)
{
	UI8 iii = 1;
	WORD FPH = TRUE;
	UI8 count = 0;

	if (pp == NULL)
	{
		return 0;
	}

	//如果存在一个料筒没有进行缓冲区自学习 或 存在一个料筒没有进行维稳自学习，返回0
	for (iii = 1; iii < pp->real_temp_ch_num; iii++)
	{
		if (CHK_TEMP_AI_CTRL(pp->mh->DATEMP_FUNC[iii]))
		{  
			count++;
			if (pp->tempctrldatastu.tempdata[iii].std_warmon_cnt <= 0)
			{
				FPH = FALSE;
			}
		}
	}

	//如果没有料筒开启，返回0
	if (count == 0)
	{
		FPH = FALSE;
	}
	
	return FPH;
}

//根据扫描周期确定用于升温速率计算的buffer个数
void Cal_Spd_Buf_Num(PTEMPCTRLCOMM pp)
{
    UI16 i, curchnl;
    
	if (pp == NULL)
	{
		return;
	}

	//根据扫描周期确定buffer个数
	Spd_buf_num = (UI8)(50.0f / ((float)pp->mh->DATEMP_SCTM_MIN / 2.0f) + 0.5f); 

	if (Spd_buf_num%2 == 1)
	{
		Spd_buf_num = Spd_buf_num - 1; //Spd_buf_num需为偶数
	}

	//超限保护
	if (Spd_buf_num > 50)
	{
		Spd_buf_num = 50;
	}
	else if (Spd_buf_num < 10)
	{
		Spd_buf_num = 10;
	}

	#ifdef RAMP_MODEL_TEST
		Spd_buf_num = 3; //测试用
	#endif

	for (curchnl = 0; curchnl < pp->real_temp_ch_num; curchnl++)
	{
		pp->tempctrldatastu.tempdata[curchnl].index = 0;
		//升温速率buffer清空
		for (i = 0; i < Spd_buf_num + 1; i++) 
		{
			pp->tempctrldatastu.tempdata[curchnl].temp_buff[i] = 0;
		}
	}
}

//如果在正常加热过程中，温度低于缓冲区，使用全功率加热，PID继续累积
void TempDropProtect(PTEMPCTRLCOMM pp, UI16 curchnl)
{
	UI16 set_temp = 0;

	if (pp == NULL)
	{
		return;
	}

	set_temp = pp->md->DATEMP_SETTING[curchnl]; //获取设定温度

	if (pp->tempctrldatastu.tempdata[curchnl].ramp_mode >= 1)
	{ //若使用新缓冲区模型加热
		if ((pp->g_newtemp.fnextheatmodenew[curchnl] >= 2) && (pp->tempctrldatastu.tempdata[curchnl].test <= T2P(set_temp - pp->tempctrldatastu.tempdata[curchnl].temp_reach_ramp)))
		{
			//最后一段增加掉温全功率
			//if ((pp->final_temp - 1) == curchnl)
			{
				//1、全速加热   小于RAMP区
		        pp->tempctrldatastu.temppwr[curchnl].pk = 10000;
		    	pp->tempctrldatastu.heatonsts[curchnl] = 3;
			}		
		}
	}
	else //若不使用新缓冲区模型加热
	{
		if((pp->g_newtemp.fnextheatmodenew[curchnl] >= 2) && (pp->tempctrldatastu.tempdata[curchnl].test <= pp->tempctrldatastu.tempdata[curchnl].ramp - RAMP_BIAS * 40))
		{
			//只对最后一段温度做处理
			//if ((pp->final_temp - 1) == curchnl)
			{
				//1、全速加热   小于RAMP区
		        pp->tempctrldatastu.temppwr[curchnl].pk = 10000;
		        pp->tempctrldatastu.heatonsts[curchnl]  = 3;
			}		
		}
	}
}

//估计原程序的缓冲区，供变温使用
void init_ramp_estimate(PTEMPCTRLCOMM pp, UI16 curchnl)   
{
	int init_ramp = 5;

	if (pp == NULL)
	{
		return;
	}
	
	if ((pp->tempctrldatastu.tempdata[curchnl].deltTDvDv > 0) && (pp->tempctrldatastu.tempdata[curchnl].speed_panel > 0))
	{
		//估计: 从室温T0升温至T0+70度时关闭电热，料筒还能上升的温度
		init_ramp = (int)(pp->tempctrldatastu.tempdata[curchnl].deltTDvDv * pp->tempctrldatastu.tempdata[curchnl].speed_panel * pp->tempctrldatastu.tempdata[curchnl].speed_panel);

		//最低值保护
		if (init_ramp <= 5)
		{
			init_ramp = 5;
		}
		else if (init_ramp >= 50)
		{
			init_ramp = 50;
		}
	
		init_ramp = init_ramp + 10; //再加5度

		#ifdef RAMP_MODEL_TEST
			init_ramp = 35; //测试用
		#endif
		
		pp->mh->DATEMP_RAMP[curchnl] = init_ramp; //将学习的缓冲区赋值给DIC
		PartDataTx((PPART)pp, MH_ID, 1, GET_IND(PTEMPCOMM_MACHSET, DATEMP_RAMP) + curchnl, FALSE); //将学习的缓冲区上传给面板
	}
}


