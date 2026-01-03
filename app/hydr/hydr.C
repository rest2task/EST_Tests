/*===========================================================================+
|  Class    : Pump process                                                   |
|  Task     : Pump process                                                   |
|----------------------------------------------------------------------------|
|  Compile  : CC'C2000 -                                                     |
|  Link     : CC'C2000 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Xiaqinghua                                                     |
|  Version  : V1.00                                                          |
|  Creation : 03/14/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include    "common.h"    
#include    "movectrl.h"
#include    "hydr.h"
#include    "servo.h"   
#include    "canhead.h"                                  
#include    "ZeroDebug.h"
#include    "hydr_dic.h"
#include    "configdf.h"
#include    "hydr_dic.h"
#include    "machine.h"
#include    "machapi.h"
#include    "da.h"
#include	"hwcfg.h"
#include	"com.h"
#include 	"cfgdb.h"
#include	"admeter.h"
#include	"alarm.h"
#include    "motor.h"
#include    "act.h"
#include    "can_e702.h"
#include	"dabios.h"

#include	"HydrPos.h"
#include	"pump.h"
#include 	"memtbl.h"

#include "PressSelfLearn.h"
#include "EleCtrlApp.h"
#include    "ServFive.h"
#include "hwmid.h"



/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
HYDRIC_STRU      g_hydric[MAXDAPORT];   //此结构体中存放的是DA输出相关的线性表设定流量，以及斜率计算出的数据
//其中g_hydric[portno].press,g_hydric[portno].flow用于SF闭环计算
MULTIHYDR_STRU   g_MultHydr;
PUMPLMT_STRU     g_pumplmt;
PUMP_STRU        g_pump;

HYDR             g_hydrout;
extern SERVO     g_hydrservo;
extern MACHINE 	 m_machine;
extern ADMETER   m_ad;
extern INNER_PARA inner_pa;
SRAMP_STRU_DA	g_sramp_da;


WORD    tda1,tda1j=0;
WORD    tda2,tda2j=0; 
WORD	g_actopen = 0;

void MotionControlRampOut(void);	

//压力曲线默认表分为25分段
//流量曲线默认表分为10分段
WORD        hydrautblpres[]=
{
	0,286,593,880,1169,1458,1803,2203,2555,2794,3033,3272,3514,3804,4095,0,0,0, 0,0,0,0,0,0,0 //140公斤特性曲线     
};

WORD        hydrautblflow[]=
{
	716,1157,1529,1856,2181,2507,2827,3139,3450,3767,4095      //流量特征曲线      
};

WORD       hydrautblflowZero[]=
{
	0,409,819,1228,1638,2047,2457,2866,3276,3685,4095     //标准无底流直线
};

WORD  hydrautblpres2[]=
{
	0,714,1428,2142,2857,3571,4285,5000,5714,6428,7142,7857,8571,9285,10000
};

WORD  hydrautblflow2[]=
{
	1748,2825,3733,4532,5326,6122,6903,7665,8424,9199,10000		  
};


WORD  hydrautblflowZero2[]=
{
	0,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000 		  
};


UI16        daprestbl[]=
{
	0,10,20,30,40,50,60,70,80,90,100,110,120,130,140
};

UI16        daflowtbl[]=
{
	0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
};
UI32        dalinetbl[]=
{
	0,7143,14286,21429,28571,35714,42857,50000,57143,64286,71429,78571,85714,92857,100000
};	

UI32  daflowlinetbl[]=
{
	0,10000,20000,30000,40000,50000,60000,70000,80000,90000,100000 		  
};

WORD        stndpump[]=
{
		100,         0,
		200,         0,
		300,	       0,
		400,	       0x0001,
		500,	       0x0003,
		600,	       0x0007,
		700,         0x0007,
		800,         0x0007,
		900,         0x000F,
		999,         0x000F 			                
};  
    

CUS_PORT_LIST cus_port_menu[OILPORTTB_NUM] = {0};
/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
WORD ActPreOpenProc(PACT pact)
{
	UI16 IsOpen = 0;
	UI16 i = 0;
	
	UID EjectActOpenList[] =
	{
		(EJECT_ID<<8) | OS_EJTF,
		(EJECT_ID<<8) | OS_EJTB,
		(EJECT_ID<<8) | OS_VEJTF,
		(EJECT_ID<<8) | OS_VEJTB,
	};

	g_actopen = 0;
	
	if (!ChkSysCfg(CO5_INJCHADA))
	{
		return 0;
	}
	
	if (GetSys()->pinj->md->Inj_Open == 1)
	{
		if ((pact->id&0xFF0F) == ((INJECT_ID<<8) | OS_INJE) && pact->spc_data == 0)
		{
			IsOpen = 1;
		}
	}

	if (GetSys()->pinj->md->Hod_Open == 1)
	{
		if ((pact->id&0xFF0F) == ((INJECT_ID<<8) | OS_INJE) && pact->spc_data == 1)
		{
			IsOpen = 1;
		}
	}
	
	if (GetSys()->pinj->md->Chg_Open == 1)
	{
		if ((pact->id&0xFF0F) == ((INJECT_ID<<8) | OS_CHRG))
		{			
			IsOpen = 1;
		}
	}

	if (GetSys()->pejt->mh->EJT_OPEN == 1)
	{
		for (i = 0; i< ARRAY_NUM(EjectActOpenList); i++)
		{
			if ((pact->id&0xFF0F) == EjectActOpenList[i])
			{			
				IsOpen = 1;
				break;
			}
		}
	}	

	if (IsOpen)
	{
		g_actopen = 1;
	}
	return IsOpen;
}


void        CanDaOut(WORD portno,WORD value,WORD Direction)         //目前DA扩展板只支持一块 2组压力流量输出
{
	static WORD E702_Press=0,E702_Flow=0 , E700B_D =0;
	static WORD E702_Press2=0,E702_Flow2=0 , E700B_D2 =0 ;
    PDA pda = (PDA)GetPartByID(MK_PART(DA_ID, 1));
	
	if((portno>3) || GetSys()->can_e700b_testmode !=0) //如果测试通讯时，这里压力流量不输出，否则有可能把测试通讯数据覆盖
		return;

    if(pda != NULL)
    {
        pda->st->DA_E700B_VAL[portno] = value;
        
        if(Direction == 2)
            pda->st->DA_E700B_DIR[portno] = 1;
        else
            pda->st->DA_E700B_DIR[portno] = 0;
    }
    
	if(portno==0)
	{
		E702_Press = value;
		if(Direction == 2)
			E700B_D |= 0x80;
		else
			E700B_D &= 0x7F;
			
	}
	else if(portno==1)
	{
		E702_Flow = value;
		if(Direction == 2)
			E700B_D |= 0x40;
		else
			E700B_D &= 0xBF;
	}
	else if(portno==2)
	{
		E702_Press2 = value;
		if(Direction == 2)
			E700B_D2 |= 0x80;
		else
			E700B_D2 &= 0x7F;
	}
	else if(portno==3)
	{
		E702_Flow2 = value;
		if(Direction == 2)
			E700B_D2 |= 0x40;
		else
			E700B_D2 &= 0xBF;
	}

	if (ChkSysCfg(XF6_EXTDA))
	{ 
		if((UI16)(Get100UsTick() - g_hydrout.last_da_can_100us) > 24 || DAIsAdjust())
		{
			g_hydrout.last_da_can_100us = Get100UsTick();
			//if(portno<2)
			E702_Port1_DataWrite(E702_Press,E702_Flow,E700B_D);	//范围0--4095
			//else
			E702_Port2_DataWrite(E702_Press2,E702_Flow2,E700B_D2);	//范围0--4095
		}
	}
}


//Direction,方向，0代表0-10v对应0-4095，1代表0-10v对应2048-4095，2代表-10v-0,对应0-2048
void        setdaout(int portno,WORD value,WORD Direction)
{
	
	if(GetHwModel() == MODEL_I3201 || GetHwModel() == MODEL_I3202 || GetHwModel() == MODEL_I3101)
	{
		if(portno < HW_DAPORTMAX)
			SetDA(portno,value,Direction); 
		else
			CanDaOut(portno - HW_DAPORTMAX,value,Direction);  
	}
	else
	{
		if (portno<	HW_DAPORTMAX)	
			SetDA(portno,value,Direction); 
	}

}

UI16  gethydelay(UI16 hydelay)
{
	PHYDR pp = &g_hydrout;

	if(hydelay == 0xffff)
	{
		if(pp->mh->HY_DELAY != 0xffff)
			return (pp->mh->HY_DELAY);
		else
			return (DEF_HYDELAY);
	}
	else
		return hydelay;
}

void        getcurrentpump()
{
	WORD*     pdataexchg;
	WORD*     pdataexchg1;
	WORD*     pdataexchg2;
	WORD*     pdataexchg3;
	WORD      i;
	PHYDR     pp = &g_hydrout;

	if (pp->mh->FL_PWRMATCHTBL==1)
	{
		pdataexchg = &g_pumplmt.clszsp;                     //动作配置 对于伺服机为位操作和pdataexchg2原理一样，普通机为流量限制表示开启0到g_pumplmt.clszsp之间的泵浦配置
		pdataexchg1 = (WORD*)&(pp->mh->TBL_PUMPLMT1);       //泵浦配置中的动作的流量限制
		pdataexchg2 = (WORD*)&(pp->mh->TBL_PWRMATCH1);      //泵浦配置中的开泵配置  位操作0x01表示开1号泵  0x03表示开1号和2号泵
		pdataexchg3 = (WORD*)&(pp->mh->DAFLOW_PWRMATCH1);	  //泵配置的流量
		for (i=0; i<20; i++)
		{
			*(pdataexchg+i) = *(pdataexchg1+i);	
		} 
		for (i=0; i<10; i++)
		{
			g_pump.currpump[2*i]   = *(pdataexchg3+i);
			g_pump.currpump[2*i+1] = *(pdataexchg2+i);
			if (*(pdataexchg3+i)>=FLWLMT)
			{
				g_pump.maxpump  = i;
				i=10;
			}	
		}   
	} 
	else
	{	    	
		for (i=0; i<10; i++)
		{
			g_pump.currpump[2*i]   = stndpump[2*i];
			g_pump.currpump[2*i+1] = stndpump[2*i+1];
			if (stndpump[2*i]>=FLWLMT)
			{
				g_pump.maxpump  = i;
				i=10;
			}	
		}   	    
		pdataexchg = &g_pumplmt.clszsp;   
		for (i=0; i<20; i++)
		{
			*(pdataexchg+i)=1;	
		}
	}
}

void        pumpout(WORD flow)
{
	PHYDR     pp = &g_hydrout;
	int   i=0;
	WORD  pumpsect,pumpdata,pumpstat,flowlmt;
	WORD* ppumpcur;

	if (g_pump.flowlmt <= FLWLMT)
		flowlmt = g_pump.flowlmt;
	else 
		flowlmt = FLWLMT;
	do
	{
		if (g_pump.currpump[2*i]>=FLWLMT)
			pumpsect = FLWLMT;
		else
			pumpsect = g_pump.currpump[2*i];
			i++;    
	}  
	while 
		((flow>pumpsect) && (flowlmt>pumpsect) && (i<=g_pump.maxpump));
	pumpdata = g_pump.currpump[2*i-1];
	ppumpcur = (WORD*)&(pp->po->O_PWRM_P1);
	for (i=0; i<8;i++)
	{  
		pumpstat = (pumpdata>>i)&1+4;
		SetPOBit(*(ppumpcur+i),pumpstat);
	}
}

void        getdefaulthydrtbl()          //LY 目前只支持2个端口的曲线（2组压力曲线，2组流量曲线）是否需要扩展  
{
	WORD     portno,i;
	PHYDR    pp = &g_hydrout;
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	for(portno=0; portno<ARRAY_NUM(g_hydric); portno++)
	{		
		if (ChkSysCfg(CO8_YL4))
		{
			memcpy(g_hydric[portno].prestbl, hydrautblpres2, sizeof(g_hydric[portno].prestbl));
			memcpy(g_hydric[portno].flowtbl, hydrautblflowZero2, sizeof(g_hydric[portno].flowtbl));
		}
		else
		{
			memcpy(g_hydric[portno].prestbl, hydrautblpres, sizeof(g_hydric[portno].prestbl));
			memcpy(g_hydric[portno].flowtbl, hydrautblflowZero, sizeof(g_hydric[portno].flowtbl));
		}
	
		memcpy(g_hydric[portno].pressettbl, dalinetbl, sizeof(g_hydric[portno].pressettbl));
		memcpy(g_hydric[portno].flowsettbl, daflowlinetbl, sizeof(g_hydric[portno].flowsettbl));

		if(pp->zd != NULL)
		{
			for(i=0;i<15;i++)
			{
				daprestbl[i] = (i*g_hydric[portno].limit)/14;   //根据压力上限初始化压力设定数组
			}
			MemcpyPro((void*)(&(pp->zd->DA_CUR[portno].DA_PRSOUT0)), g_hydric[portno].prestbl, DA_PRS_NUM*sizeof(UI16));
			MemcpyPro((void*)(&(pp->zd->DA_CUR[portno].DA_FLOWOUT0)), g_hydric[portno].flowtbl, DA_FLOW_NUM*sizeof(UI16));
			MemcpyPro((void*)(&(pp->zd->DA_SET[portno].DASET_PRSOUT0)), daprestbl, 15*sizeof(UI16));
			MemcpyPro((void*)(&(pp->zd->DA_SET[portno].DASET_FLOWOUT0)), daflowtbl, 11*sizeof(UI16));	
		}
	}
}

void        getdefaulthydrtblLine(WORD portno,WORD Line)          //得到DA曲线，0 标准含底流   1   标准直线   2    自定义 
{
	WORD     i;
	PHYDR    pp = &g_hydrout;
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	for(portno=0; portno<ARRAY_NUM(g_hydric); portno++)
	{		
		if (ChkSysCfg(CO8_YL4))
		{
			memcpy(g_hydric[portno].prestbl, hydrautblpres2, sizeof(g_hydric[portno].prestbl));
			if(Line == 1)
			{
				memcpy(g_hydric[portno].flowtbl, hydrautblflow2, sizeof(g_hydric[portno].flowtbl));
			}
			else
			{
				memcpy(g_hydric[portno].flowtbl, hydrautblflowZero2, sizeof(g_hydric[portno].flowtbl));
			}
		}
		else
		{
			memcpy(g_hydric[portno].prestbl, hydrautblpres, sizeof(g_hydric[portno].prestbl));
			if(Line == 1)
			{
				memcpy(g_hydric[portno].flowtbl, hydrautblflow, sizeof(g_hydric[portno].flowtbl));
			}
			else
			{
				memcpy(g_hydric[portno].flowtbl, hydrautblflowZero, sizeof(g_hydric[portno].flowtbl));
			}
		}
		
		memcpy(g_hydric[portno].pressettbl, dalinetbl, sizeof(g_hydric[portno].pressettbl));
		memcpy(g_hydric[portno].flowsettbl, daflowlinetbl, sizeof(g_hydric[portno].flowsettbl));

		if(pp->zd != NULL)
		{
			for(i=0;i<15;i++)
				daprestbl[i] = (i*g_hydric[portno].limit)/14;   //根据压力上限初始化压力设定数组
			MemcpyPro((void*)(&(pp->zd->DA_CUR[portno].DA_PRSOUT0)), g_hydric[portno].prestbl, DA_PRS_NUM*sizeof(UI16));
			MemcpyPro((void*)(&(pp->zd->DA_CUR[portno].DA_FLOWOUT0)), g_hydric[portno].flowtbl, DA_FLOW_NUM*sizeof(UI16));

			MemcpyPro((void*)(&(pp->zd->DA_SET[portno].DASET_PRSOUT0)), daprestbl, 15*sizeof(UI16));
			MemcpyPro((void*)(&(pp->zd->DA_SET[portno].DASET_FLOWOUT0)), daflowtbl, 11*sizeof(UI16));			
		}
	}
}


void        getdefaultpreschghydrtbl()          //压力设定按照原来系统压力上限改变前的比例换算成新的压力上限比例 
{
	WORD     portno,i;
	PHYDR    pp = &g_hydrout;

	for(portno = 0; portno< ARRAY_NUM(g_hydric); portno++)
	{		
		if((pp->zd != NULL)&&(pp->zd->DA_SET[portno].DASET_PRSOUT14!=g_hydric[portno].limit))
		{
			for(i=0;i<15;i++)
			{
				daprestbl[i] = ((*( ((UI16 *)&(pp->zd->DA_SET[portno].DASET_PRSOUT0))+i))*g_hydric[portno].limit/pp->zd->DA_SET[portno].DASET_PRSOUT14);   //根据压力上限初始化压力设定数组
			}
			MemcpyPro((void*)(&(pp->zd->DA_SET[portno].DASET_PRSOUT0)), daprestbl, 15*sizeof(UI16));
		}
	}
}
void setselfdefinehydrtbl()
{
	int portno,i;
	PHYDR     pp = &g_hydrout;

	for (portno=0; portno< ARRAY_NUM(g_hydric); portno++)
	{
		memcpy(g_hydric[portno].prestbl, (void*)(&(pp->zd->DA_CUR[portno].DA_PRSOUT0)), sizeof(g_hydric[portno].prestbl));
		memcpy(g_hydric[portno].flowtbl, (void*)(&(pp->zd->DA_CUR[portno].DA_FLOWOUT0)), sizeof(g_hydric[portno].flowtbl));
		for(i = 0;i<15;i++)
		{
			g_hydric[portno].pressettbl[i]=(((UI32)(*( ((UI16 *)&(pp->zd->DA_SET[portno].DASET_PRSOUT0))+i)))*100000 + g_hydric[portno].limit/ 2)/g_hydric[portno].limit;
		}
		for(i = 0;i<11;i++)
		{		
			g_hydric[portno].flowsettbl[i] =( (UI32)(*(((UI16 *)&(pp->zd->DA_SET[portno].DASET_FLOWOUT0))+i)))*1000;	   
		}	
	
	}
}

void        inithydlimtport()//DA最大值获取,以CODE中的系统压力上限为准,背压以自动控制部件中的背压上限为准
{
	int portno;
	WORD* da_hydraumax0;
	PHYDR     pp = &g_hydrout;
	PINJECT  pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1)); 
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	PAUTOCTRL pat = (PAUTOCTRL)GetPartByID(MK_PART(AUTOCTRL_ID, 1)); 
	da_hydraumax0	= (WORD*)&pp->st->DA_HYDRAUMAX0;

	for (portno=0; portno<MAXDAPORT ; portno++)
	{
		g_hydric[portno].limit   = GetSysConfig()->DAPRES_SYSLMT;
		if (g_hydric[portno].limit<DA_PRESSYSLIMIT)
			g_hydric[portno].limit = DA_PRESSYSLIMIT;  

		/*背压配置上，只有code码打上才复制*/
		if(((pinj->hy->HYDR_PORT_CHRGBACK>>portno)&1) && ChkSysCfg(CO2_CHGBK))
		{
			/*新QT面板，面板保存的背压上限在AUTOCTRL部件中的模具参数中*/
			g_hydric[portno].limit = pat->md->DAPRES_BACKLIMIT;
			/*如保存值<14，面板可能为HMI面板，使用原机器部件中背压上限值*/
			if(g_hydric[portno].limit < 14)
				g_hydric[portno].limit = pm->mh->DAPRES_BACKLIMIT;

			if(g_hydric[portno].limit<14)
				g_hydric[portno].limit = 70;
		}
		*(da_hydraumax0+portno) = g_hydric[portno].limit;	
	}  	
}

void limttoline()//判断是否是系统压力上限或者背压上限改变需要改变对应的曲线，并主动上传更新后的数据
{
	PINJECT  pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1)); 
	PMACHINE pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1)); 
	PHYDR     pp = &g_hydrout;
	UI16 i,a;

	if (GetSys()->last_da_mode != ChkSysCfg(CO8_YL4))
	{
		GetSys()->last_da_mode = ChkSysCfg(CO8_YL4);
		SetHydrCurReset(TRUE);
	}
	
	for(i=0;i<MAXDAPORT;i++)
	{
		if((pinj->hy->HYDR_PORT_CHRGBACK>>i)&1)
		{
		}
		else
		{			
			if((pm->mh->DAPRES_SYSLMT!=g_hydric[i].limit))
			{
				g_hydric[i].limit = pm->mh->DAPRES_SYSLMT;
				if (g_hydric[i].limit<DA_PRESSYSLIMIT)
					g_hydric[i].limit = DA_PRESSYSLIMIT; 

				for(a=0;a<15;a++)
				{
					*(&(pp->zd->DA_SET[i].DASET_PRSOUT0)+a) = (a*g_hydric[i].limit)/14;	
				}
				inner_pa.pa2 = 0;   //系统压力上限改变时，清除首次保存的数据
				SaveInnerPara();
				PartDataTx((PPART)pm, MH_ID, 2, GET_IND(PMACHINE_MACHSET, DA_LINE), FALSE);
			}
		}
	}

	setselfdefinehydrtbl();

	PartCfgSave((PPART)pp, ZD_ID);
	PartDataTx((PPART)pp, ZD_ID, CAL_LEN(*(pp->zd)), 0, FALSE);	
}

/* 背压压力上限改变或者通道号改变，将准备改变的通道号的线性复制到改变前通道，然后新改变的通道的线性根据背压压力上限重新生成新线性，并主动上传到上位机*/
/*lastportno和chgportno从1号到4号*/
void chgbackpresset(UI16 lastportno,UI16 chgportno,UI16 chgpres)
{
	PHYDR pp = &g_hydrout;
	UI16 i = 0,tmpportno = 0,ly = 0;
	WORD*  da_hydraumax0	= (WORD*)&pp->st->DA_HYDRAUMAX0;
	PINJECT  pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1)); 
	if(lastportno>4||chgportno>4||(lastportno==0&&chgportno==0))
		return;
	for(ly=0;ly<4;ly++)
	{
		if(((ly+1)!=lastportno)&&((ly+1)!=chgportno)) //选出非改变前和改变后的线性通道号
			tmpportno = ly;
	}
	if(lastportno>0)
	{

		g_hydric[lastportno-1].limit = pp->zd->DA_SET[tmpportno].DASET_PRSOUT14;
		*(da_hydraumax0+lastportno-1) = g_hydric[lastportno-1].limit;
	}
	if(chgportno>0)
	{
		g_hydric[chgportno-1].limit = chgpres;
		*(da_hydraumax0+chgportno-1) = g_hydric[chgportno-1].limit;	
	}
	for(i=0;i<15;i++)
	{
		if(chgportno>0)
		{
			if(lastportno>0)
				*(&(pp->zd->DA_SET[lastportno-1].DASET_PRSOUT0)+i) = *(&(pp->zd->DA_SET[tmpportno].DASET_PRSOUT0)+i);
			*(&(pp->zd->DA_SET[chgportno-1].DASET_PRSOUT0)+i) = i*g_hydric[chgportno-1].limit/14;
		}
		else
		{
			if(lastportno>0)
				*(&(pp->zd->DA_SET[lastportno-1].DASET_PRSOUT0)+i) = *(&(pp->zd->DA_SET[tmpportno].DASET_PRSOUT0)+i);
		}
	}
	setselfdefinehydrtbl();
	pinj->hy->HYDR_PORT_CHRGBACK = ((chgportno>0)?(0x01<<(chgportno-1)):0);
	PartDataTx((PPART)pp, ZD_ID, CAL_LEN(*(pp->zd)), 0, FALSE);	
}

void HydrDaCurSave()
{
	const PART_CFG_IND* pcfg;
	PHYDR pp = &g_hydrout;
	PMACHINE  pm     =  &m_machine;
	UI16 i;
	inithydlimtport();
	setselfdefinehydrtbl();
	PartCfgSave( &g_hydrout, ZD_ID);//保存内存数据到用户区
	PartDataTx((PPART)pp, ZD_ID, CAL_LEN(*(pp->zd)), 0, FALSE);//主动发送DA线性到面板
	
	if((pcfg = CfgDbPartRead((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0, FACTORY_PART_DB)) == NULL) //判断保存的用户数区数据是否需要写到出厂区
	{
		CfgDbPartSave((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0,FACTORY_PART_DB);
	}
	else
	{
		if(pcfg->offset==0)
		{
			for(i=0;i<4;i++)
			{
				if((*(((UI16*)pcfg->psave)+144+14+i*36))!=pp->zd->DA_SET[i].DASET_PRSOUT14)
				{
					inner_pa.pa2 = pm->mh->DA_LINE;    //用于改变系统压力上限后，做为首次保存进出厂数据时，记录当前的线性类型
					SaveInnerPara();
					CfgDbPartSave((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0,FACTORY_PART_DB);
					break;
				}
			}
		}
	}
}

BOOL ChkDACurMode()
{
	int i = 0;
	PHYDR pp = &g_hydrout;
	UI16 cur = 0,set = 0;
	
	for (i = 10; i >= 0;i--)
	{
		if((*( ((UI16 *)&(pp->zd->DA_SET[0].DASET_FLOWOUT0))+i)) != 0)
		{
			break;
		}
	}
	cur = (*( ((UI16 *)&(pp->zd->DA_CUR[0].DA_FLOWOUT0))+i));
	set = (*( ((UI16 *)&(pp->zd->DA_SET[0].DASET_FLOWOUT0))+i));

	if (((cur/set > 75) && !ChkSysCfg(CO8_YL4)) || ((cur/set <= 75) && ChkSysCfg(CO8_YL4)))
	{	
		return FALSE;
	}

	return TRUE;
}

/*
1.用户区有数据读取用户区的数据判断存放的数据的压力上限是否与系统压力上限相同，不同根据压力上限初始化
用户区和出厂区以及DIC和计算数组中的压力设定数据.
2.用户区无数据,但出厂区有数据.读取出厂区的数据判断存放的数据的压力上限与系统压力上限是否相同,不同根据
根据系统压力上限初始化用户区和出厂区以及DIC和计算数组的压力设定数据.
3.用户和出厂区都无保存数据，根据压力上限初始化用户区和出厂区以及DIC和计算数组的压力设定数据.
*/
void HydrLoadDACur()
{
	const PART_CFG_IND* pcfg;
	PHYDR pp = &g_hydrout;
	PMACHINE  pm     =  &m_machine;

	/*先查找用户配置区是否有DA曲线，与版本0x01的DA曲线不兼容*/
	if((pcfg = PartCfgRead(pp, ZD_ID)) == NULL || pcfg->ver == 0x01)
	{
		/*读出厂的是否有DA曲线*/
		if((pcfg = CfgDbPartRead((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0, FACTORY_PART_DB)) == NULL || pcfg->ver == 0x01)
		{
			/*没有， 用程序内根据各个通道压力上限计算出的均分曲线*/
			getdefaulthydrtbl();
			CfgDbPartSave((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0,FACTORY_PART_DB);
			inner_pa.pa2 = 0;       //内置曲线为标准含底流
			SaveInnerPara();
		}
		else// 如果出厂区有数据要判断存放的数据的压力上限是否与系统压力上限相同
		{
			PartDataRx((PPART)pp, ZD_ID, pcfg->len, pcfg->offset, pcfg->psave, INIT_CFG_RX);
			getdefaultpreschghydrtbl();		
		}
		/*保存到用户配置区*/
		PartCfgSave((PPART)pp, ZD_ID);
	}
	else// 如果用户区有数据要判断存放的数据的压力上限是否与系统压力上限相同
	{
		PartDataRx((PPART)pp, ZD_ID, pcfg->len, pcfg->offset, pcfg->psave, INIT_CFG_RX);
		getdefaultpreschghydrtbl();
	}	

#ifdef DA_V
	if (!ChkDACurMode())
	{
		
		/*没有， 用程序内根据各个通道压力上限计算出的均分曲线*/
		getdefaulthydrtbl();
		CfgDbPartSave((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0,FACTORY_PART_DB);
		inner_pa.pa2 = 0;		//内置曲线为标准含底流
		SaveInnerPara();
		/*保存到用户配置区*/
		PartCfgSave((PPART)pp, ZD_ID);		
	}
#endif
	pm->mh->DA_LINE  = inner_pa.pa1;       //ADDA线性类型保存，用于上传给面板
	PartDataTx((PPART)pm, MH_ID, 2, GET_IND(PMACHINE_MACHSET, DA_LINE), FALSE);

	setselfdefinehydrtbl();
}


WORD        getpresdavalue(UI32 pres,WORD preslmt, WORD portno)
{	
	UI32 relpres;
	WORD  darel;
	int   i=0;
	if (portno>= HW_DAPORTMAX) portno = 0;       //only 2 line
		relpres=((UI32)(pres*100) + preslmt/ 2)/ preslmt;	//4舍5入  
	if (relpres>100*ACCURACY)
		relpres=100*ACCURACY;  	
	while((relpres>=g_hydric[portno].pressettbl[i]) && (i<DA_LINEMAX-1))
	{
		i++;
	}  

	if ((relpres-g_hydric[portno].pressettbl[i-1])<1)
		darel = g_hydric[portno].prestbl[i-1];
	else
		darel = (((unsigned int)(g_hydric[portno].prestbl[i]-g_hydric[portno].prestbl[i-1])*(relpres-g_hydric[portno].pressettbl[i-1]))
	/(g_hydric[portno].pressettbl[i]-g_hydric[portno].pressettbl[i-1]))+g_hydric[portno].prestbl[i-1]; 

	if(i==0)
		darel = g_hydric[portno].prestbl[0];

	return(darel);          
}

WORD        getflowdavalue(UI32 flow,WORD portno)
{	
	WORD  darel;
	int   i=0; 
	if (portno>=HW_DAPORTMAX) 
		portno = 0;       //only 2 line	
	if (flow >= FLWLMT*FLOWACCURACY)
		darel=g_hydric[portno].flowtbl[DA_FLOW_LINEMAX-1];
	else
	{	  
		while((flow>=g_hydric[portno].flowsettbl[i]) && (i<DA_FLOW_LINEMAX-1))
		{
			i++;
		}  
		if ((flow-(g_hydric[portno].flowsettbl[i-1]))<1)
			darel = g_hydric[portno].flowtbl[i-1];
		else
			darel = (WORD)(((UI32)(g_hydric[portno].flowtbl[i]-g_hydric[portno].flowtbl[i-1])*(flow-g_hydric[portno].flowsettbl[i-1]))
				/(g_hydric[portno].flowsettbl[i]-g_hydric[portno].flowsettbl[i-1])) +g_hydric[portno].flowtbl[i-1];

		if(i==0)
			darel = g_hydric[portno].flowtbl[0];          
	}    
	return(darel);          
}

void        hydraupresout(WORD portno,UI32 pres,WORD Direct)
{
	WORD   presdavalue;
	presdavalue = getpresdavalue(pres,g_hydric[portno-1].limit,portno-1);
	setdaout(2*portno-2,presdavalue,Direct);
}	

void        hydraupresoutTY(WORD portno,UI32 pres,UI16 flow_set,WORD Direct)
{
	WORD   presdavalue;
	PHYDR pp = &g_hydrout;
	presdavalue = getpresdavalue(pres,g_hydric[portno-1].limit,portno-1);
	/*通用新加2016-01-11*/
	if((portno == 1 &&  ChkSysCfg(CO1_ECAT))
	|| (portno == 2 &&  (ChkSysCfg(CO1_ECAT)==0)))
	{
		if(flow_set>500&&flow_set<=1000)
		{
			presdavalue = presdavalue - (pp->mh->HIFLOW_ADJPARA*(flow_set-500))/10;
			if(presdavalue>4095)
				presdavalue = 0;
		}
		else if(flow_set>=1&&flow_set<=500)
		{
			presdavalue = presdavalue + (pp->mh->LOWFLOW_ADJPARA*(500-flow_set))/10;
			if(presdavalue>4095)
				presdavalue = 4095;
		}
	}
	setdaout(2*portno-2,presdavalue,1);
}


/*flow 放大了FLOWACCURACY倍，flow_set没有被放大 */
void        hydrauflowout(WORD portno,UI32 flow, UI32 flow_set,WORD Direct)
{
	//PTEMPCTRL ptmp = (PTEMPCTRL)GetPartByID(MK_PART(TEMPCTRL_ID, 1));
	WORD   flowdavalue;
	PHYDR  pp = &g_hydrout;

	if(portno == 0 || portno > pp->max_hydr_port)
		return;

	if (portno==1) 
	{
		if (flow>0) 
			SetPOBit(pp->po->O_PQVLV, PC_ON);
		else  
			SetPOBit(pp->po->O_PQVLV,PC_OFF);

		if(!DAIsAdjust())   //此处占时屏蔽，使在DA校正时，一旦泵浦输出过，就不在关闭输出
			pumpout(flow_set);	
	}
	flowdavalue = getflowdavalue(flow,portno-1);
	setdaout(2*portno-1,flowdavalue,Direct); 
}

void        inithydrauport()	//压力上限保护
{
	int i;	
	inithydlimtport();
	HydrLoadDACur();  
	getcurrentpump();
	for (i=0;i<MAXDAPORT;i++)
	{
		hydraupresout(i+1,0,1);
		hydrauflowout(i+1,0,0,1);  
	} 
}

void    	SetFlowData(WORD portno,UI32 flow,WORD nomlmt, WORD srvlmt, UI32 flow_set,WORD Direct)
{
	//PTEMPCTRLCOMM ptc = (PTEMPCTRLCOMM)GetPartByID(MK_PART(TEMPCTRLCOMM_ID, 1));
	PHYDR     pp = &g_hydrout;
	
	if(portno == 0 || portno > pp->max_hydr_port)
		return;
	
	g_pump.flowlmt = nomlmt;
	hydrauflowout(portno,flow, flow_set,Direct);     //硬件输出
}

void     SetPressData(WORD portno,UI32 pres,WORD Direct)
{
	PMACHINE    pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	PHYDR     pp = &g_hydrout;
	
	if(portno == 0 || portno > pp->max_hydr_port)
		return;

	hydraupresout(portno,pres,Direct); 
  
	if(ChkSysCfg(CO5_INJCHADA))  
	{
		if(g_actopen == 1)
		{				
		  	if(CUS_ID == CS_TONGYONG || ChkSysCfg(XF3_MSTOR))
				hydraupresoutTY(portno,pres,p_WorkStru[portno].SpeedSet,Direct);
			else
				hydraupresout(portno,pres,Direct); 
		}
		else if(g_actopen == 0)
		{
		}
	}     
}

//动作完成需要关闭液压时 只需要清除设定的优先级列表即可 具体的后续相关液压控制权在 movedispatchproc()和MoveHydrRampOut()(普通机);和ServoPIDCtrlProc()(伺服);中
void   CloseHydrOut(WORD portno,WORD Priority,WORD pramp,WORD framp)
{
	int i;
	int j;
	int z;
	int count = 0;
	WORD  lastsrvlmt;
//	PHYDR  pp = &g_hydrout;	
	PMC_CTRL PMcCtrl;

	for(i = 0; i < MAXDAPORT; i++)
	{
		if(i < g_pump_num)
		{			
			if( (1 << i) & portno)
			{				
				g_Dispatch[i + 1].Fuction[((Priority >>( i * 2)) & 0x03) + 1] = 0;

				for(j=1;j<5;j++)
				{		
					if(g_Dispatch[i+1].Fuction[j]==0)		
					{
						count++;
					}
					else if(g_Dispatch[i + 1].Fuction[j]==2 && j > ((Priority >>( i * 2)) & 0x03) + 1)
					{
						g_Dispatch[i + 1].Fuction[j] = 1;			 	
					}
				}

				if((PMcCtrl = &(g_pumpn[i]->McCtrl)) == NULL)
				{
					lastsrvlmt = 1;
				}
				else
				{
					lastsrvlmt = PMcCtrl->SlavePump;
				}

				if(count ==4)	
				{
					if(g_hydrout.clopnrel == 1)
						SetMoveUnitDataList(MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES,0,0,pramp,framp,0,0,0,portno,lastsrvlmt,1,GetkpDefault(), GetkiDefault(),ZEROHYDRD1,1,0xffff,0);//设定“0压控制”表
					else	
						SetMoveUnitDataList(MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES,0,0,pramp,framp,0,0,0,portno,lastsrvlmt,1,GetkpDefault(), GetkiDefault(),ZEROHYDRD,1,0xffff,0);//设定“0压控制”表


					if (ChkSysCfg(C08_NEWHYD))
					{
						lastsrvlmt= lastsrvlmt & ~(0x01<<(portno-1));
						
						for (z = 0; z < MAXDAPORT; z++)
						{
							if (((lastsrvlmt&GetSys()->st->PUMP_MAIN)>>z)&0x01 > 0)
							{
								SetMoveUnitDataList(MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES,0,0,pramp,framp,0,0,0,z+1,0,1,GetkpDefault(), GetkiDefault(),(g_hydrout.clopnrel == 1)?ZEROHYDRD1:ZEROHYDRD,1,0xffff,0);//设定“0压控制”表
							}
						}
					}
				}
			}
		}		
	}
}

void   CloseDaOut_Pres(WORD portno,WORD Priority,WORD pramp,WORD dire)	//模拟量输出关闭
{
	int i;

	for(i = 0; i < MAXDAPORT; i++)
	{		
		if( (1 << i) & portno)
		{
			SetMoveUnitData_Da_pres(MC_CTR_MODE_DA,0,pramp,pramp,0,portno,1,ZEROHYDRD,1,0xffff,dire,0);//设定“0压控制”表
		}		
	}
}

void   CloseDaOut_Flow(WORD portno,WORD Priority,WORD framp,WORD dire)	//模拟量输出关闭
{
	int i;

	for(i = 0; i < MAXDAPORT; i++)
	{		
		if( (1 << i) & portno)
		{
			SetMoveUnitData_Da_flow(MC_CTR_MODE_DA,0,framp,framp,0,portno,1,ZEROHYDRD,1,0xffff,dire,0);//设定“0压控制”表
		}		
	}
}


void InitHydrData()//液压参数初始化
{ 
	PMACHINE    pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	int portno,i;
	g_pump.flowlmt = FLWLMT;  
	getcurrentpump();            //获得PUMP配置数据 
	inithydrauport();            //初始化液压通道数据
	InitPresCtrl();
	for (portno=0; portno<MAXDAPORT; portno++)
	{            
		g_hydric[portno].Press  = 0;
		g_hydric[portno].Flow = 0;
		for(i=0;i<5;i++)
			g_Dispatch[portno].Fuction[i] =0;
		InitServoPIDParaCold(portno);   //冷启动PID参数初始化	
		CloseDaOut_Pres((1<<portno),0,0,1);	//按手动按键时，关闭模拟量的P输出
		CloseDaOut_Flow((1<<portno),0,0,1);	//按手动按键时，关闭模拟量的F输出
	}  
	
	if (ChkSysCfg(CO0_SERVOP))
	{
		CloseHydrOut(0x0F,0x0F, 0, 0);
	}
}    


void DA_OUTPUMP_OUT(void)
{
	PHYDR     ph = &g_hydrout;
	int k;
	WORD*     pwrm;	

	pwrm  =	  (WORD*)&(ph->po->O_PWRM_P1);	

	for (k=0;k<8;k++)    
	{
		if((ph->mh->DA_TBL_PUMPOUT >> k) & 1)   
			SetPOBit(*(pwrm+k),PC_ON);                
		else
			SetPOBit(*(pwrm+k),0); 
	}
}


void DA_OUTPUMP_Clear(void)
{
	PHYDR     ph = &g_hydrout;
	int k;
	WORD*     pwrm;	

	pwrm  =	  (WORD*)&(ph->po->O_PWRM_P1);	

	for (k=0;k<8;k++)    
	{
		if(TestPOIsOn(*(pwrm+k)))
			SetPOBit(*(pwrm+k),0); 
	}
}


//          强制液压数据设定函数
void   SetFlowHydrOutData(WORD portno,WORD pres, WORD flowad)
{
	WORD* da_outyput0;
	PHYDR     pp = &g_hydrout;
	WORD   presdavalue;
	PINJECT  pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1)); 
	PDA pd = (PDA)GetPartByID(MK_PART(DA_ID, 1));

	if(portno == 0 || portno > pp->max_hydr_port)
		return;
	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;
	if(GetHwModel() == MODEL_I3201 || GetHwModel() == MODEL_I3202 || GetHwModel() == MODEL_I3101)	//IV3200
	{
		if(portno < 3 || !pd->F2DAMAX)
		{
			setdaout(2*portno-1,flowad,1);
			SetPressData(portno,pres*ACCURACY,1);
		}
		else if(portno == 3 && ChkSysCfg(XF6_EXTDA))
		{
			presdavalue = getpresdavalue(pres*ACCURACY,g_hydric[portno-1].limit,portno-1);
			presdavalue = (WORD)(((UI32)presdavalue * (pd->st->TEMPMAX[2*portno-2] - pd->mh->WDA_ZERO[2*portno-2])) / 4096) + pd->mh->WDA_ZERO[2*portno-2]; 
			E702_Port1_AbsDataWrite(presdavalue,flowad);	//范围0--4095
			E702_CanTransmit();
		}
		else if(portno == 4 && ChkSysCfg(XF6_EXTDA))
		{
			presdavalue = getpresdavalue(pres*ACCURACY,g_hydric[portno-1].limit,portno-1);
			presdavalue = (WORD)(((UI32)presdavalue * (pd->st->TEMPMAX[2*portno-2] - pd->mh->WDA_ZERO[2*portno-2])) / 4096) + pd->mh->WDA_ZERO[2*portno-2]; 
			E702_Port2_AbsDataWrite(presdavalue,flowad);	//范围0--4095
			E702_CanTransmit();
		}

	}
	else		//IV5200
	{
		if(portno < 5 || !pd->F2DAMAX)
		{
			setdaout(2*portno-1,flowad,1);
			SetPressData(portno,pres*ACCURACY,1);
		}
	}
	*(da_outyput0+2*portno-2) = pres;	
	if(pp->md->DACHK_CHG == 1)   //流量调整时输出储料动作
	{
		ForkAct(pinj->act + OS_DACHRG - 1, FIRST_FORK, NULL, 0, 0);
	}
	DA_OUTPUMP_OUT();
}

void   SetPressHydrOutData(WORD portno,WORD presad, WORD flow)
{
	WORD* da_outyput0;
	PHYDR pp = &g_hydrout;
	WORD   flowdavalue;
	PINJECT  pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));
	PDA pd = (PDA)GetPartByID(MK_PART(DA_ID, 1));
 
	if(portno == 0 || portno > pp->max_hydr_port)
	return;
	
	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;
	if(GetHwModel() == MODEL_I3201 || GetHwModel() == MODEL_I3202 || GetHwModel() == MODEL_I3101)	//IV3200
	{
		if(portno < 3 || !pd->F2DAMAX)
		{
			setdaout(2*portno-2,presad,1);
			SetFlowData(portno,flow*ACCURACY,1, 1, flow,1);
		}
		else if((portno == 3)&& ChkSysCfg(XF6_EXTDA))
		{
			flowdavalue = getflowdavalue(flow*ACCURACY,portno-1);
			flowdavalue = (WORD)(((UI32)flowdavalue * (pd->st->TEMPMAX[2*portno-1] - pd->mh->WDA_ZERO[2*portno-1])) / 4096) + pd->mh->WDA_ZERO[2*portno-1]; 
			E702_Port1_AbsDataWrite(presad,flowdavalue);	//范围0--4095
			E702_CanTransmit();
		}
		else if((portno == 4)&& ChkSysCfg(XF6_EXTDA))
		{
			flowdavalue = getflowdavalue(flow*ACCURACY,portno-1);
			flowdavalue = (WORD)(((UI32)flowdavalue * (pd->st->TEMPMAX[2*portno-1] - pd->mh->WDA_ZERO[2*portno-1])) / 4096) + pd->mh->WDA_ZERO[2*portno-1]; 
			E702_Port2_AbsDataWrite(presad,flowdavalue);	//范围0--4095
			E702_CanTransmit();
		}

	}
	else	//IV5200
	{
		if(portno < 5 || !pd->F2DAMAX)
		{
			setdaout(2*portno-2,presad,1);
			SetFlowData(portno,flow*ACCURACY,1, 1, flow,1);
		}
	}	
	*(da_outyput0+2*portno-1) = flow;  

	
	if(ChkSysCfg(CO2_CHGBK) && ((pinj->hy->HYDR_PORT_CHRGBACK>>(portno - 1))&1))   //流量调整时输出储料动作
	{
//		if(!ChkSysCfg(CO1_ECAT))
//		{
//			SetPressData(HYDR_PORT(pinj->hy->HYDR_PORT_CHARGE),pinj->pa->DAPRES_CHG1*ACCURACY,1);
//			SetFlowData(HYDR_PORT(pinj->hy->HYDR_PORT_CHARGE),pinj->pa->DAFLOW_CHG1*ACCURACY,1, 1, pinj->pa->DAFLOW_CHG1,1);
//		}
		
		ForkAct(pinj->act + OS_DACHRG - 1, FIRST_FORK, NULL, 0, 0);
	}

	if(ChkSysCfg(CO1_ECAT)&&ChkSysCfg(CO5_INJCHADA))
	{
		InitServoPIDPara(1);
		g_hydric[1].Press = GetSysConfig()->DAPRES_SYSLMT*10;
		if(ChkSysCfg(CO1_ECAT)&&ChkSysCfg(CO5_INJCHADA)&&((pinj->md->Inj_Open > 0)||(pinj->md->Hod_Open > 0))&&(portno==1))
		{
			PACT pact = pinj->act + OS_INJE - 1;

			ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, presad,flow,0,pinj->mh->DAFLOW_INJRAMP,0,	
							pinj->mh->DAFLOW_INJRAMP, 0,HYDR_PORT(pinj->hy->HYDR_PORT_INJCT),pinj->hy->HYDR_PUMPS_INJCT,pinj->hy->HYDR_PUMPS_INJCT,
							pinj->hy->SERVO_KP_INJCT,pinj->hy->SERVO_KI_INJCT, INJDFLAG, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pinj->hy->HYDR_PORT_INJCT));
		}
	}		
	else if((ChkSysCfg(CO1_ECAT)==0)&&ChkSysCfg(CO5_INJCHADA)) //朗格专用  校正第一通道主系统压力时，开环第2通道压力输出最大值;
	{                                                                                       //校正第2通道时第一通道输出系统最大压力和设定的校正流量;	
		if(portno==2)
		{
			SetPressData(1,GetSysConfig()->DAPRES_SYSLMT*ACCURACY,1);
			SetFlowData(1,flow*ACCURACY,1, 1, flow,1);
		}
		else if(portno==1)
		{
			HWDASet(2, 4095);
		}
	}
	DA_OUTPUMP_OUT();
}

/*portno = 0，清楚所有通道的DA输出，
portno != 0 时，除portno指定通道外，所有其他通道DA输出被清除*/
void ClearAllHydrOut(UI16 portno)   //屏蔽对4路扩展板的清零
{
	WORD* da_outyput0;
	PHYDR pp = &g_hydrout;
	int i;

	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;
	for(i = 0; i < pp->max_hydr_port; i ++)
	{
		if((portno == 0 || portno- 1 != i))
		{ 
			*(da_outyput0 + i*2) = 0;
			*(da_outyput0 + i*2 + 1) = 0;

			/*由于扩展板第3 路和第4 路是同时输出的，此处第4 路清零时，会将第3 路
			用0x16的方式,将上次的压力，流量值重新输出一遍，而在对第3 路进行压力，流量
			的最大值调整时，马上通过0x17 输出不一样的值，从而导致电流表晃动*/

			hydraupresout(i + 1,0,1);      
			hydrauflowout(i + 1,0, 0,1); 
		}  
	}
	hydraupresout(4,0,1);      
	hydrauflowout(4,0,0,1); 
}

/*portno = 0，清楚所有通道的DA输出，
portno != 0 时，除portno指定通道外，所有其他通道DA输出被清除*/
void ClearAllHydrOut2(UI16 portno) 
{
	WORD* da_outyput0;
	PHYDR pp = &g_hydrout;
	int i;

	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;

	if(portno == 1 || portno == 2)
	{
		for(i = 0; i < pp->max_hydr_port; i ++)
		{
			if((portno == 0 || portno- 1 != i))
			{ 
				*(da_outyput0 + i*2) = 0;
				*(da_outyput0 + i*2 + 1) = 0;

				/*由于扩展板第3 路和第4 路是同时输出的，此处第4 路清零时，会将第3 路
				用0x16的方式,将上次的压力，流量值重新输出一遍，而在对第3 路进行压力，流量
				的最大值调整时，马上通过0x17 输出不一样的值，从而导致电流表晃动*/

				hydraupresout(i + 1,0,1);      
				hydrauflowout(i + 1,0, 0,1); 
			}  
		}
	}
	else
	{
		for(i = 0; i < 2; i ++)           //屏蔽第三路和第4路的清零
		{
			if((portno == 0 || portno- 1 != i))
			{ 
				*(da_outyput0 + i*2) = 0;
				*(da_outyput0 + i*2 + 1) = 0;

				/*由于扩展板第3 路和第4 路是同时输出的，此处第4 路清零时，会将第3 路
				用0x16的方式,将上次的压力，流量值重新输出一遍，而在对第3 路进行压力，流量
				的最大值调整时，马上通过0x17 输出不一样的值，从而导致电流表晃动*/

				hydraupresout(i + 1,0,1);      
				hydrauflowout(i + 1,0, 0,1); 
			}  
		}
	}
}


void   SetHydrOutData(WORD portno,WORD pres, WORD flow)
{
	PMACHINE    pm = (PMACHINE)GetPartByID(MK_PART(MACHINE_ID, 1));
	WORD* da_outyput0;
	PHYDR     pp = &g_hydrout;
	PINJECT  pinj = (PINJECT)GetPartByID(MK_PART(INJECT_ID, 1));

	if(portno == 0 || portno > pp->max_hydr_port)
		return;

	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;
	if(ChkSysCfg(CO0_SERVOP))
		InitServoPIDPara(1);

	if(ChkSysCfg(CO1_ECAT)&&ChkSysCfg(CO5_INJCHADA)&&((pinj->md->Inj_Open > 0)||(pinj->md->Hod_Open > 0))&&(portno==1))
	{
		PACT pact = pinj->act + OS_INJE - 1;

		ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pres,flow,0,pinj->mh->DAFLOW_INJRAMP,0,	
						pinj->mh->DAFLOW_INJRAMP, 0,HYDR_PORT(pinj->hy->HYDR_PORT_INJCT),pinj->hy->HYDR_PUMPS_INJCT,pinj->hy->HYDR_PUMPS_INJCT,
						pinj->hy->SERVO_KP_INJCT,pinj->hy->SERVO_KI_INJCT, INJDFLAG, !(pact->ftaskstat & INTF_CEC), HYDR_PRI(pinj->hy->HYDR_PORT_INJCT));
	}

	SetPressData(portno,pres*ACCURACY,1);
	SetFlowData(portno,flow*ACCURACY,1, 1, flow,1);

	if (flow == 100)
		flow = 99;
	
	*(da_outyput0+2*portno-2) = pres;
	*(da_outyput0+2*portno-1) = flow;
	DA_OUTPUMP_OUT();
}

//          液压数据显示设定函数
void   SetHydrDisplay(WORD portno,WORD pres, WORD flow)
{
	WORD* da_outyput0;
	PHYDR pp = &g_hydrout;

	if(portno == 0 || portno > pp->max_hydr_port)
		return;

	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;

	if (flow == 999)
		flow = 1000;			

	*(da_outyput0+2*portno-2) = pres;	
	*(da_outyput0+2*portno-1) = flow;

	if(pres > 0)
		db_printf("ACT hyout, press:%d, flow:%d\r\n", pres, flow);
}

void   SetHydrDisplay_P(WORD portno,WORD pres)
{
	WORD* da_outyput0;
	PHYDR pp = &g_hydrout;

	if(portno == 0 || portno > pp->max_hydr_port)
		return;

	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;

	*(da_outyput0+2*portno-2) = pres;	
}

void   SetHydrDisplay_F(WORD portno,WORD flow)
{
	WORD* da_outyput0;
	PHYDR pp = &g_hydrout;

	if(portno == 0 || portno > pp->max_hydr_port)
		return;

	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;

	*(da_outyput0+2*portno-1) = flow;
}

void   AdjustHydrDisplay(WORD Adjbit)
{
	WORD* da_outyput0;
	PHYDR pp = &g_hydrout;
	int portno;

	da_outyput0 = (WORD*)&pp->st->DA_OUTPUT0;

	if(Adjbit)
	{
		for(portno=1; portno<4; portno++)
			*(da_outyput0+2*portno-1) = *(da_outyput0+2*portno-1)/10;
	}
	else
	{
		for(portno=1; portno<4; portno++)
			*(da_outyput0+2*portno-1) = *(da_outyput0+2*portno-1)*10;
	}
}

//此函数用于主控制将那一张运动参数列表传递给那一个运动部件输出
void  movedispatchproc()
{  
	WORD portno;
	PSERVO  pp = &g_hydrservo;	 
	for (portno=1; portno<4; portno++)
	{
		if(g_Dispatch[portno].Fuction[1]!= 0)           //区别端口第一优先级列表是否有参数设定，无参数设定直接跳到第二优先级列表判断
		{
			if(g_Dispatch[portno].Fuction[1] == 1)        //第一优先级列表有设定将此表的数据传递给MOVE_STRU计算
			{
				SetMoveUnitData(portno,1);                   //调用函数将g_Dispatch.List[0]的参数传递到MOVE_STRU中用于SF计算
				g_Dispatch[portno].Fuction[1] =2;
				g_Dispatch[portno].Fuction[0] =0;
				p_WorkStru[portno].MoveStatus = 0;
			}
		}
		else if((g_Dispatch[portno].Fuction[2] != 0))
		{
			if(g_Dispatch[portno].Fuction[2]== 1)         //第二优先级列表有设定将此表的数据传递给MOVE_STRU计算
			{
				SetMoveUnitData(portno,2);                   //调用函数将g_Dispatch.List[0]的参数传递到MOVE_STRU中用于SF计算
				g_Dispatch[portno].Fuction[2] =2;
				g_Dispatch[portno].Fuction[0] =0;            //"0压控制"参数不启用
				p_WorkStru[portno].MoveStatus = 0;
			}
		}
		else if((g_Dispatch[portno].Fuction[3]!= 0))
		{
			if(g_Dispatch[portno].Fuction[3]== 1)         //第三优先级列表有设定将此表的数据传递给MOVE_STRU计算
			{
				SetMoveUnitData(portno,3);                   //调用函数将g_Dispatch.List[0]的参数传递到MOVE_STRU中用于SF计算
				g_Dispatch[portno].Fuction[3] =2;
				g_Dispatch[portno].Fuction[0] =0;            //"0压控制"参数不启用
				p_WorkStru[portno].MoveStatus = 0;  	   
			}
		}
		else if((g_Dispatch[portno].Fuction[4] != 0))
		{
			if(g_Dispatch[portno].Fuction[4]== 1)        //第四优先级列表有设定将此表的数据传递给MOVE_STRU计算
			{
				SetMoveUnitData(portno,4);                  //调用函数将g_Dispatch.List[0]的参数传递到MOVE_STRU中用于SF计算
				g_Dispatch[portno].Fuction[4] =2;
				g_Dispatch[portno].Fuction[0] =0;           //"0压控制"参数不启用
				p_WorkStru[portno].MoveStatus = 0;
			} 
		}
		else                                           //"液压无动作控制"
		{
			if((g_Dispatch[portno].Fuction[0] == 1))      //判断是否设定了“0压控制”表
			{
				p_WorkStru[portno].MoveStatus = 1;          //标记"液压无动作"   
				SetMoveUnitData(portno,0); 	               //传递“0压控制”参数    	
				g_press[portno].CntZeroSpeed    = 0; 
				g_Dispatch[portno].Fuction[0] =2; 	 
			}
			else if(g_Dispatch[portno].Fuction[0] ==0)
				SetMoveUnitDataList(MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES,0,0,0,0,0,0,0, 1 << (portno - 1) ,1,1,pp->mh->KP_DEFAULT,pp->mh->KI_DEFAULT,ZEROHYDRD,1,0xffff,0);//设定“0压控制”表	
		}      
	}
}


void MoveHydrRampOut2()
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	int  lasttime;
	int   portno =1 ;
	int 	tmp;
	for(portno=1; portno<5; portno++)                //液压端口从“1”号端口开始，“0”号端口为备用端口号特殊用途标记等
	{		
		if (!CHK_MOTOR_MODE(MOTOR_CTRL_COMMON) || (((GetSys()->st->PUMP_MAIN>>(portno-1))&0x01)==0))
		{		
			if (p_WorkStru[portno].HydrDlyTime==0)                  //液压延迟结束，开始液压输出
			{  
				lasttime = (Get100UsTick()- p_WorkStru[portno].MoveStart100us);
				if (p_WorkStru[portno].SpeedRampOK==0)		   //有速度斜率，走斜率输出	
				{
					if (lasttime<p_WorkStru[portno].SpeedRamp) //流量斜率计算
					{
						tmp = p_WorkStru[portno].SpeedSet-p_WorkStru[portno].SpeedLast; //4舍5入
						tmp = tmp*lasttime + (p_WorkStru[portno].SpeedRamp >> 1);
						p_WorkStru[portno].SpeedCurrent = p_WorkStru[portno].SpeedLast+(tmp/p_WorkStru[portno].SpeedRamp);
					}
					else 
					{
						p_WorkStru[portno].SpeedCurrent = p_WorkStru[portno].SpeedSet;
						p_WorkStru[portno].SpeedRampOK  = 1;	
					}  
				}

				if (((CLAMP_OPN_PID_CH == CLAMP_CLS_PID_CH)&&(portno == CLAMP_OPN_PID_CH && ((pclmp->md->FL_CLSOPEN_MODE == 2)||(pclmp->md->FL_CLSOPEN_MODE == 4)) && (pclmp->clampfunc == 1)))
					||((CLAMP_OPN_PID_CH != CLAMP_CLS_PID_CH)&&((pclmp->md->FL_CLSOPEN_MODE == 2)||(pclmp->md->FL_CLSOPEN_MODE == 4)) && (pclmp->clampfunc == 1)))
				{
					
				}
				else
				{
					SetFlowData(p_WorkStru[portno].HydrPortNo,p_WorkStru[portno].SpeedCurrent*FLOWACCURACY,p_WorkStru[portno].NormalPumpLmt,p_WorkStru[portno].ServoPumpLmt,p_WorkStru[portno].SpeedSet/10,p_WorkStru[portno].Direction_F);
				}
				
				if (p_WorkStru[portno].PressRampOK==0)		   //有压力斜率，走斜率输出	
				{
					if (lasttime<p_WorkStru[portno].PressRamp) //压力斜率计算
					{
						tmp = p_WorkStru[portno].PressSet-p_WorkStru[portno].PressLast; //4舍5入
						tmp = tmp*lasttime +  (p_WorkStru[portno].PressRamp >> 1);
						p_WorkStru[portno].PressCurrent = p_WorkStru[portno].PressLast+(tmp/p_WorkStru[portno].PressRamp);
					}
					else 
					{
						p_WorkStru[portno].PressCurrent = p_WorkStru[portno].PressSet;
						p_WorkStru[portno].PressRampOK  = 1;
					}  
				}
				
				SetPressData(p_WorkStru[portno].HydrPortNo,p_WorkStru[portno].PressCurrent*ACCURACY,p_WorkStru[portno].Direction_P);
			}
			else						  //液压延迟
			{
				if ((DWORD)(Get100UsTick()- p_WorkStru[portno].MoveStart100us) >=((DWORD)p_WorkStru[portno].HydrDlyTime*10))
				{
					p_WorkStru[portno].HydrDlyTime  = 0;
					p_WorkStru[portno].MoveStart100us = Get100UsTick();	
				}	
			}
		}
	}  
}

void   MoveHydrRampOut()
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	int   portno =1;
	for(portno=1; portno<5; portno++)                //液压端口从“1”号端口开始，“0”号端口为备用端口号特殊用途标记等
	{	
		if (!CHK_MOTOR_MODE(MOTOR_CTRL_COMMON) || (((GetSys()->st->PUMP_MAIN>>(portno-1))&0x01)==0))
		{
			if (p_WorkStru[portno].HydrDlyTime==0)                  //液压延迟结束，开始液压输出
			{  
				if (p_WorkStru[portno].SpeedRampOK==0)		   //有速度斜率，走斜率输出	
				{
					if(p_WorkStru[portno].SpeedSet > p_WorkStru[portno].SpeedLast)
					{
						p_WorkStru[portno].SpeedCurrent100b = p_WorkStru[portno].SpeedCurrent100b + p_WorkStru[portno].SpeedRampbit;
						p_WorkStru[portno].SpeedCurrent = p_WorkStru[portno].SpeedCurrent100b/FLOWACCURACY;
						 
						if(p_WorkStru[portno].SpeedSet <= p_WorkStru[portno].SpeedCurrent)
						{
						    p_WorkStru[portno].SpeedCurrent = p_WorkStru[portno].SpeedSet;
							p_WorkStru[portno].SpeedCurrent100b = p_WorkStru[portno].SpeedCurrent*FLOWACCURACY;
						    p_WorkStru[portno].SpeedRampOK  = 1;
						}
					}
					else if(p_WorkStru[portno].SpeedSet < p_WorkStru[portno].SpeedLast)
					{
						p_WorkStru[portno].SpeedCurrent100b = p_WorkStru[portno].SpeedCurrent100b - p_WorkStru[portno].SpeedRampbit;
						p_WorkStru[portno].SpeedCurrent = p_WorkStru[portno].SpeedCurrent100b/FLOWACCURACY;

						if(p_WorkStru[portno].SpeedSet >= p_WorkStru[portno].SpeedCurrent)
						{
						    p_WorkStru[portno].SpeedCurrent = p_WorkStru[portno].SpeedSet;
							p_WorkStru[portno].SpeedCurrent100b = p_WorkStru[portno].SpeedCurrent*FLOWACCURACY;
						    p_WorkStru[portno].SpeedRampOK  = 1;
						}
					}
					else if(p_WorkStru[portno].SpeedSet == p_WorkStru[portno].SpeedLast)
					{
					    p_WorkStru[portno].SpeedRampOK  = 1;
					} 
				}

				if (((CLAMP_OPN_PID_CH == CLAMP_CLS_PID_CH)&&(portno == CLAMP_OPN_PID_CH && ((pclmp->md->FL_CLSOPEN_MODE == 2)||(pclmp->md->FL_CLSOPEN_MODE == 4)) && (pclmp->clampfunc == 1)))
					||((CLAMP_OPN_PID_CH != CLAMP_CLS_PID_CH)&&((pclmp->md->FL_CLSOPEN_MODE == 2)||(pclmp->md->FL_CLSOPEN_MODE == 4)) && (pclmp->clampfunc == 1)))
				{
					
				}
				else
					SetFlowData(p_WorkStru[portno].HydrPortNo,p_WorkStru[portno].SpeedCurrent100b,p_WorkStru[portno].NormalPumpLmt,p_WorkStru[portno].ServoPumpLmt, p_WorkStru[portno].SpeedSet/10,p_WorkStru[portno].Direction_F);
				
			    if (p_WorkStru[portno].PressRampOK==0)		   //有压力斜率，走斜率输出	
			    {
					if(p_WorkStru[portno].PressSet > p_WorkStru[portno].PressLast)
					{
						p_WorkStru[portno].PressCurrent100b = p_WorkStru[portno].PressCurrent100b + p_WorkStru[portno].PressRampbit;
						p_WorkStru[portno].PressCurrent = p_WorkStru[portno].PressCurrent100b/ACCURACY;
						if(p_WorkStru[portno].PressSet <= p_WorkStru[portno].PressCurrent)
						{
							p_WorkStru[portno].PressCurrent = p_WorkStru[portno].PressSet;
						    p_WorkStru[portno].PressCurrent100b = p_WorkStru[portno].PressCurrent*ACCURACY;
							p_WorkStru[portno].PressRampOK  = 1;
						}
					}
					else if(p_WorkStru[portno].PressSet < p_WorkStru[portno].PressLast)
					{
					    p_WorkStru[portno].PressCurrent100b = p_WorkStru[portno].PressCurrent100b - p_WorkStru[portno].PressRampbit;
						p_WorkStru[portno].PressCurrent = p_WorkStru[portno].PressCurrent100b/ACCURACY;
					    if(p_WorkStru[portno].PressSet >= p_WorkStru[portno].PressCurrent)
					    {
						    p_WorkStru[portno].PressCurrent = p_WorkStru[portno].PressSet;
						    p_WorkStru[portno].PressCurrent100b = p_WorkStru[portno].PressCurrent*ACCURACY;
						    p_WorkStru[portno].PressRampOK  = 1;
					    }
					}
					else if(p_WorkStru[portno].PressSet == p_WorkStru[portno].PressLast)
					{
					    p_WorkStru[portno].PressRampOK  = 1;
					} 
			    }

				SetPressData(p_WorkStru[portno].HydrPortNo,p_WorkStru[portno].PressCurrent100b,p_WorkStru[portno].Direction_P);
			}
			else						  //液压延迟
			{
				if ((DWORD)(Get100UsTick()- p_WorkStru[portno].MoveStart100us) >=(((DWORD)p_WorkStru[portno].HydrDlyTime)*10))
				{
					p_WorkStru[portno].HydrDlyTime  = 0;
					p_WorkStru[portno].MoveStart100us = Get100UsTick();	
				}	
			}
		}
	}  
}

void   LoopHydrauic()
{
	movedispatchproc();
	
	if(!DAIsAdjust())
	{
		if(ChkSysCfg(CO43_PfRamp))
			MoveHydrRampOut2();
		else
			MoveHydrRampOut();
	}	

	MotionControlRampOut();	
}

void   LoopHydrauic_sv()	//单独伺服斜率后台输出
{
	movedispatchproc();
	MotionControlRampOut();	
}


BOOL HydrCmd(UI32 cmd, UI32 para)
{
	UI32 ret = TRUE;

	switch(GET_CMD(cmd))
	{
		case CMD_DATA_RX:
		if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
		{
			if(((GET_CMD_PARA(cmd) >> 8) & 0xff) != INIT_CFG_RX)
			{
				getcurrentpump();
			}
		}		
		break;

		default:
		ret = FALSE;
		break;
	}

	return ret;
}


BOOL InitHydrPart()
{
	PHYDR pp = & g_hydrout;

	static const SUB_INIT_INFO sub_init[]  = HYDR_INIT_INFO;
	static const ACT_INIT_INFO act_init[] = 
	{
		{OS_PRESSELF, OS_PRESSELF, ACT_CTRB,ChaFen_Opti,OneAction_Off}
	};
	 
	if(!INIT_PART(pp, HYDR_ID, 1, sub_init))
		return FALSE;
	
	if(!INIT_ACT(pp, act_init))
		return FALSE;

	pp->max_hydr_port = 4;

	return TRUE;
}

/*主机重置后，根据系统压力上限以及背压压力上限比例缩放DIC中和用户区的线性数据*/
void SetBackReset()
{
	PHYDR pp = &g_hydrout;
	inithydlimtport();
	getdefaultpreschghydrtbl();
	setselfdefinehydrtbl();
	PartCfgSave((PPART)pp, ZD_ID);
	PartDataTx((PPART)pp, ZD_ID, CAL_LEN(*(pp->zd)), 0, FALSE);
}


/*曲线重置:
1.判断出厂区无曲线，根据系统压力均分曲线保存到出厂以及用户区
2.出厂区有曲线，判断保存的曲线是压力上限是否与系统压力上限相同，不同按照保存的曲线比例放缩为系统压力上限的曲线
*/
void SetHydrCurReset(BOOL inlay)
{
	PHYDR pp = &g_hydrout;
	PMACHINE  pm     =  &m_machine;
	const PART_CFG_IND* pcfg;
	inithydlimtport();                                                           //根据系统压力上限初始化压力
	/*读出厂区是否有DA曲线*/
	if((inlay > 0) || (pcfg = CfgDbPartRead((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0, FACTORY_PART_DB)) == NULL)
	{
		/*没有，用程序内置缺省曲线*/
		getdefaulthydrtbl();
		CfgDbPartSave((PPART)pp, ZD_ID, CAL_LEN(HYDR_ZDSET), 0, FACTORY_PART_DB);
		inner_pa.pa2 = 0;     //内置曲线为标准含底流
		SaveInnerPara();
	}
	else
	{
		PartDataRx((PPART)pp, ZD_ID, pcfg->len, pcfg->offset, pcfg->psave, INIT_CFG_RX);//读取出厂区数据到DIC中
		getdefaultpreschghydrtbl();                                              //比例缩放曲线到DIC中
	}
	pm->mh->DA_LINE  = inner_pa.pa2;       //ADDA线性类型保存，用于上传给面板
	inner_pa.pa1 = inner_pa.pa2;          //数据重置完后，需清除pa1当中保存的上次的线性类型
	SaveInnerPara();
	setselfdefinehydrtbl();                                                      //根据DIC中数据初始化计算数组	
	
	PartDataTx((PPART)pm, MH_ID, 2, GET_IND(PMACHINE_MACHSET, DA_LINE), FALSE);
	PartCfgSave((PPART)pp, ZD_ID);                                               //将DIC中的数据保存到用户区           
	PartDataTx((PPART)pp, ZD_ID, CAL_LEN(*(pp->zd)), 0, FALSE);                  //主动发送数据到面板
}


void inithydatacomisok()       //通信成功后根据系统压力和自动控制部件中的背压上限初始化DIC和计算数组中的DA线性数据
{
	PHYDR pp = &g_hydrout;
	inithydlimtport();
	HydrLoadDACur();
	PartDataTx((PPART)pp, ZD_ID, CAL_LEN(*(pp->zd)), 0, FALSE);
}



/*******************************   模拟量输出控制部分  PID 输出端口 ***********************/
void	PosPidMethod_DA(UI16 method)
{
	switch(method)
	{
		case 0:  /*不使用PID*/
			g_sramp_da.pidpara_da.p_da	= 0;
			g_sramp_da.pidpara_da.ki_da	= 0;
			g_sramp_da.pidpara_da.kd_da	= 0;
			g_sramp_da.posdata_da.ik_da	= 0;			
			break;
		case 1:  /*P*/ 
			g_sramp_da.pidpara_da.ki_da	= 0;
			g_sramp_da.pidpara_da.kd_da	= 0;
			g_sramp_da.posdata_da.ik_da	= 0;				
			break;
		case 2:  /*PI*/
			g_sramp_da.pidpara_da.kd_da	= 0;
			g_sramp_da.posdata_da.ik_da	= 0;			
			break;
		case 3:  /*PD*/
			g_sramp_da.pidpara_da.ki_da	= 0;
			g_sramp_da.posdata_da.ik_da	= 0;				
			break;
		case 4:  /*PID*/
			g_sramp_da.posdata_da.ik_da	= 0;			
			break;	
		case 5:
			g_sramp_da.pidpara_da.kd_da	= 0;
			break;
		default: /*使用PID*/		
			break;
	}
}

void  PosParaInit_DA(UI16 pidnum,UI16 posp,UI16 posi,UI16 posd)     //位置PID参数初始化
{
	g_sramp_da.pidpara_da.p_da	= posp*10;
	g_sramp_da.pidpara_da.ki_da	= posi*5;
	g_sramp_da.pidpara_da.kd_da	= posd*10;
	g_sramp_da.posdata_da.pk_da	= 0;
	g_sramp_da.posdata_da.pk0_da	= 0;
	g_sramp_da.posdata_da.ek_da	= 0;
	g_sramp_da.posdata_da.ek0_da	= 0;
	g_sramp_da.posdata_da.ek00_da= 0;
	g_sramp_da.posdata_da.ik_da	= 0;	
	g_sramp_da.caltm_da 		= PIDCALTM;
	g_sramp_da.speedlast_da	= 0;
	PosPidMethod_DA(5);
}

void  Pospidpara_DA()
{
	UI32	Delti;
	
	g_sramp_da.posdata_da.ek00_da	=	g_sramp_da.posdata_da.ek0_da;
	g_sramp_da.posdata_da.ek0_da    =	g_sramp_da.posdata_da.ek_da;
	g_sramp_da.posdata_da.ek_da		=	g_sramp_da.posset_da - g_sramp_da.posreal_da;	
	if(g_sramp_da.posdata_da.ek_da<0 || g_sramp_da.posdata_da.ek_da>30000)                     
		g_sramp_da.posdata_da.ek_da = 0;
	Delti=(long int)g_sramp_da.pidpara_da.ki_da*g_sramp_da.posdata_da.ek_da;
	
	g_sramp_da.posdata_da.ik_da	=	(g_sramp_da.posdata_da.ik_da + Delti)/10;
	g_sramp_da.posdata_da.ik_da = (g_sramp_da.posdata_da.ik_da>0)?g_sramp_da.posdata_da.ik_da:0;
	
	g_sramp_da.posdata_da.pk_da=(long int)(g_sramp_da.pidpara_da.p_da*(g_sramp_da.posdata_da.ek_da)+ g_sramp_da.posdata_da.ik_da
                      + g_sramp_da.pidpara_da.kd_da*10*((g_sramp_da.posdata_da.ek_da-g_sramp_da.posdata_da.ek0_da)-(g_sramp_da.posdata_da.ek0_da-g_sramp_da.posdata_da.ek00_da)));
}


void PospidProc_DA(UI16 pres,UI16 flow,UI16 posset,UI16 posreal,UI16 port,UI16 pramp,UI16 Dramp,UI16 pressmin,UI16 flowmin) //位置控制程序
{
	PCLAMP pclmp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	long int flownow,flowli;
	//UI16 tempflow,temppres;
	
	g_sramp_da.posset_da  	= 	posset+20;
	g_sramp_da.posreal_da 	= 	posreal;
	//g_sramp_da.speedmax_da	= 	getflowdavalue(flow*FLOWACCURACY*10,port-1);
	g_sramp_da.speedmax_da	= 	flow*10000*10/FLWLMT;
	g_sramp_da.speedmin_da	= 	flowmin*10000*10/FLWLMT;
	if(pramp > 0)
		g_sramp_da.accelerationstep_da = (PIDCALTM * 1000)/pramp;
	else	
		g_sramp_da.accelerationstep_da = 50;
	if(Dramp > 0)
		g_sramp_da.decelerationstep_da = (PIDCALTM * 1000)/Dramp;
	else
		g_sramp_da.decelerationstep_da = 200;
	if((WORD)(Get100UsTick() -g_sramp_da.tmflag_da) >= g_sramp_da.caltm_da)
	{
		g_sramp_da.tmflag_da = Get100UsTick();		
		PosPidMethod_DA(5);                     //使用PID全部功能
		Pospidpara_DA();	
        g_sramp_da.speedreal_da = g_sramp_da.posdata_da.pk_da/1000;
		if(g_sramp_da.speedreal_da > (g_sramp_da.speedlast_da + g_sramp_da.accelerationstep_da))
		{
			g_sramp_da.speedreal_da = g_sramp_da.speedlast_da + g_sramp_da.accelerationstep_da;
			g_sramp_da.posdata_da.ik_da = g_sramp_da.posdata_da.ik_da - (UI32)(g_sramp_da.pidpara_da.ki_da*g_sramp_da.posdata_da.ek_da)/10;
		}
		if(g_sramp_da.speedreal_da < (g_sramp_da.speedlast_da - g_sramp_da.decelerationstep_da))
		{
			g_sramp_da.speedreal_da = g_sramp_da.speedlast_da - g_sramp_da.decelerationstep_da;	
			g_sramp_da.posdata_da.ik_da = g_sramp_da.posdata_da.ik_da - (UI32)(g_sramp_da.pidpara_da.ki_da*g_sramp_da.posdata_da.ek_da)/10;
		}
		if(g_sramp_da.speedreal_da > g_sramp_da.speedmax_da)
		{
			g_sramp_da.speedreal_da = g_sramp_da.speedmax_da;		
			g_sramp_da.posdata_da.ik_da = g_sramp_da.posdata_da.ik_da- (UI32)(g_sramp_da.pidpara_da.ki_da*g_sramp_da.posdata_da.ek_da)/10;
		}
		if(g_sramp_da.speedreal_da < g_sramp_da.speedmin_da)
		{
			g_sramp_da.speedreal_da = g_sramp_da.speedmin_da;		
			g_sramp_da.posdata_da.ik_da = g_sramp_da.posdata_da.ik_da - (UI32)(g_sramp_da.pidpara_da.ki_da*g_sramp_da.posdata_da.ek_da)/10;	
		} 
		g_sramp_da.speedlast_da = g_sramp_da.speedreal_da;
		
		flowli = (((long int)g_sramp_da.speedreal_da)*99/10000);
		if(flowli <= 1)
			flowli = 1;
		else if(flowli >= 99)
			flowli = 99;
		pclmp->pflowend = (UI16)flowli;

		//用于在开关模函数中斜率加减时，记录当前的时时模拟量值，防止突然手动放开结束后，可以衔接好模拟量，并以一定斜率下降
		if(ChkActRun(pclmp, ACT_SN_BIT(OS_CLSM)))
		{

			p_WorkStru[CLAMP_CLS_PID_CH].SpeedCurrent = pclmp->pflowend *10;	//SpeedCurrent由SpeedSet赋值而来，所以是被放大过10倍的，所以此处要乘以10
			p_WorkStru[CLAMP_CLS_PID_CH].SpeedCurrent100b = pclmp->pflowend* FLOWACCURACY*10;	//SpeedCurrent100b同理，也是要乘以10，并且此处默认写死在第二组流量通道

			flownow = getflowdavalue((UI32)flowli*FLOWACCURACY*10, CLAMP_CLS_PID_CH - 1);
		}
		else
		{
			p_WorkStru[CLAMP_OPN_PID_CH].SpeedCurrent = pclmp->pflowend *10;	//SpeedCurrent由SpeedSet赋值而来，所以是被放大过10倍的，所以此处要乘以10
			p_WorkStru[CLAMP_OPN_PID_CH].SpeedCurrent100b = pclmp->pflowend* FLOWACCURACY*10;	//SpeedCurrent100b同理，也是要乘以10，并且此处默认写死在第二组流量通道

			flownow = getflowdavalue((UI32)flowli*FLOWACCURACY*10, CLAMP_OPN_PID_CH - 1);
		}		

		if(ChkActRun(pclmp, ACT_SN_BIT(OS_CLSM)))
        {
    		pclmp->pclsflowendda = flownow;
			if(pclmp->md->FL_CLSOPEN_MODE == 4)
			{
    			setdaout(CLAMP_CLS_PID_CH*2-1,flownow,CLAMP_CLS_P_DIREC);           
			}
			else
			{
				setdaout(CLAMP_CLS_PID_CH*2-1,flownow,CLAMP_CLS_PID_DIREC); 
			}
        }
		else
		{
			pclmp->pflowendda = flownow;
			if(pclmp->md->FL_CLSOPEN_MODE == 4)
			{
				if(ChkSysCfg(XF2_OPNBK))
				{
					if((3800-flownow) > getflowdavalue(0,CLAMP_VLV_USE))
						setdaout(CLAMP_OPN_PID_CH*2-1,3800-flownow,CLAMP_OPN_P_DIREC);
					else
					{
						flownow = getflowdavalue(0,CLAMP_VLV_USE);
						setdaout(CLAMP_OPN_PID_CH*2-1,flownow,CLAMP_OPN_P_DIREC);
					}
				}
				else 
				{
					setdaout(CLAMP_OPN_PID_CH*2-1,flownow,CLAMP_OPN_P_DIREC);
				}
			}
			else
			{
				setdaout(CLAMP_OPN_PID_CH*2-1,flownow,CLAMP_OPN_PID_DIREC);
			}
		}
	}
}

UI16 PospidProc_DASPC(UI16 pres,UI16 flow,UI16 posset,UI16 posreal,UI16 port,UI16 pramp)//位置控制程序
{
    PCLAMP pclamp = (PCLAMP)GetPartByID(MK_PART(CLAMP_ID, 1));
	g_sramp_da.posset_da  	= 	posset;
	g_sramp_da.posreal_da 	= 	posreal;
    g_sramp_da.posdata_da.ek_da     =   g_sramp_da.posset_da - g_sramp_da.posreal_da;   
    if(g_sramp_da.posdata_da.ek_da<0 || g_sramp_da.posdata_da.ek_da>30000)                     
        g_sramp_da.posdata_da.ek_da = 0;    
    g_sramp_da.posdata_da.pk_da=(long int)(g_sramp_da.pidpara_da.p_da*(g_sramp_da.posdata_da.ek_da));
    g_sramp_da.speedreal_da = g_sramp_da.posdata_da.pk_da/1000;
    return getflowdavalue((g_sramp_da.speedreal_da)*10,CLAMP_VLV_USE);        
}

void AdposServoRampDownUp(UI16 startpres,UI16 startflow,UI16 endpres,UI16 endflow,UI16 startpos,UI16 endpos,UI16 realpos)
{
    int32 temppres,tempflow;
    
    temppres = ((realpos-startpos)*(endpres*10 - startpres*10)/(endpos - startpos) + startpres*10);
    tempflow = ((realpos-startpos)*(endflow*10 - startflow*10)/(endpos - startpos) + startflow*10);

	if(tempflow >= 990)
		tempflow = 999;
	
    //if(ChkSysCfg(CO1_ECAT))
    {
        g_pumpn[0]->McPara.Mcnow[0]   = (UI16)temppres;
        if(g_pumpn[0]->McPara.Mcnow[0] <= endpres)
            g_pumpn[0]->McPara.Mcnow[0] = endpres;
		
		g_pumpn[0]->McPara.McSet[0] = g_pumpn[0]->McPara.Mcnow[0];

		g_pumpn[0]->McPara.Mcnow[1] = ActSetSpdToRPM(0, tempflow);
        if(g_pumpn[0]->McPara.Mcnow[1] <= ActSetSpdToRPM(0, endflow))
            g_pumpn[0]->McPara.Mcnow[1] = ActSetSpdToRPM(0, endflow);     

		 g_pumpn[0]->McPara.McSet[1] = g_pumpn[0]->McPara.Mcnow[1];   
    }
}

