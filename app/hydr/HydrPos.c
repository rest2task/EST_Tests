#include 	"com.h"
#include    "common.h"
#include    "movectrl.h"
#include    "hydr.h"
#include	"HydrPos.h"
#include	"Machapi.h"
#include	"Admeter.h"
#include 	"clamp.h"
#include 	"act.h"


//HydrStruCtrl g_HydrStruCtrl[MAXDAPORT];

HydrStruData g_hydrstrudata[MAXDAPORT];


void InitSetHydrStruCtrlData(WORD portno, UI16 max_turn, UI16 r, UI16 R, UI16 Cylinder_num,UI16 q,UI16 q_k, UI16 min_press, UI16 min_flow, UI16 k)
{
	if(portno >= MAXDAPORT)
		return;
	
	if(portno > 0)
		portno--;
	else
		return;
		
    PHydrStruData p_hydrstrudata;
    p_hydrstrudata = &(g_hydrstrudata[portno]);

	if(p_hydrstrudata == NULL)
		return;
	
	if (portno >= g_pump_num)
		return;

	if (g_pumpn[portno]->MotorMaxRPM > 1500)
	{
		if (max_turn > g_pumpn[portno]->MotorMaxRPM)
		{
			max_turn = g_pumpn[portno]->MotorMaxRPM;
		}
	}
	
	p_hydrstrudata->k_MotortoAD = q_k;
    p_hydrstrudata->Max_turn = max_turn;
    p_hydrstrudata->Cylinder_r = r / 10;
    p_hydrstrudata->Cylinder_R = R / 10;

	p_hydrstrudata->Cylinder_num = Cylinder_num;
    p_hydrstrudata->kp_pos= k;
	if(p_hydrstrudata->kp_pos == 0)
		p_hydrstrudata->kp_pos = 100;
    p_hydrstrudata->Min_flow = min_flow;	
	p_hydrstrudata->Min_press = min_press;

    p_hydrstrudata->Min_turn = max_turn * min_flow / ACC_999;

    p_hydrstrudata->Oilpump_Q = q;
	if(p_hydrstrudata->Oilpump_Q == 0 || p_hydrstrudata->Oilpump_Q > 500)
		p_hydrstrudata->Oilpump_Q = 60;
}



//rpm->mm
float GetKMotorToAD(WORD portno)
{
    float q = 0;//流量	mm3/min
    float Cylinder_opn_s = 0;//油缸面积差 mm2

    PHydrStruData p_hydrdata;

    p_hydrdata = &g_hydrstrudata[portno-1];

    Cylinder_opn_s = PI * (((p_hydrdata->Cylinder_R / 2) * (p_hydrdata->Cylinder_R / 2)) - ((p_hydrdata->Cylinder_r / 2) * (p_hydrdata->Cylinder_r / 2)))*p_hydrdata->Cylinder_num;
    q = p_hydrdata->Oilpump_Q * 1000.0f;

    return ((q / Cylinder_opn_s) / 60.0f);
}

WORD SmoothStop(WORD HydrPosId, WORD RunDirec, WORD AdNo, int PSramp, ACT_OP_FUNC_NEW VlvFunc, PACT pact)
{
	PSTOP_CTRL_PARA Databuff= NULL;

	if(HydrPosId == 0)
		return 0;

	if(HydrPosId <= GetHydrPosNum())
	{		
		if((Databuff = (PSTOP_CTRL_PARA)ApiNewPkt(HydrPosId, 0, 0, sizeof(STOP_CTRL_PARA))) != NULL)
		{
			Databuff->HydrPosId = HydrPosId;
			Databuff->RunDirec = RunDirec;
			Databuff->AdNo = AdNo;
			Databuff->PSramp = PSramp;
			Databuff->VlvFunc = VlvFunc;
			Databuff->pact = pact;
            Databuff->pp = pact->part;
			
			if(SmoothStopTran(Databuff) == 0)
			{
				ApiFreeBuf(Databuff);
				return 0;
			}
			
            ApiFreeBuf(Databuff);
			return 1;
		}
	}
    return 0;
}


void SetPosCtrlPara(WORD ServoNo, WORD RunDirec, WORD AdNo, int *PSramp, int *Pspd, int *PPramp, int *Ppres, int *Ppos, UI16 seg, UI16 dietime, UI16 hydrdlytime, UI16 mode,int sm, int diffpos, int movenum)
{
	PPOS_CTRL_PROC_PARA Databuff= NULL;
	PHydrStruData p_hydrdata;
	
	if(ServoNo == 0 || ServoNo > g_pump_num)
		return;

	if((p_hydrdata = &(g_hydrstrudata[ServoNo-1])) == NULL)
		return;

	if(ServoNo <= GetHydrPosNum())
	{		
		if((Databuff = (PPOS_CTRL_PROC_PARA)ApiNewPkt(ServoNo, 0, 0, sizeof(POS_CTRL_PROC_PARA))) != NULL)
		{
			Databuff->HydrPosId = ServoNo;
			Databuff->RunDirec = RunDirec;
			Databuff->AdNo = AdNo;
			Databuff->PSramp = PSramp;
			Databuff->Pspd = Pspd;
			Databuff->PPramp = PPramp;
			Databuff->Ppres = Ppres;
			Databuff->Ppos = Ppos;
			Databuff->seg = seg;
			Databuff->dietime = dietime;
			Databuff->hydrdlytime = hydrdlytime;
			Databuff->mode = mode;
			
			if(sm == 0)
				Databuff->mode = Databuff->mode|DECMODE_END_LOOP;

			if(ChkSysCfg(CO43_PfRamp))
				Databuff->mode = Databuff->mode|DECMODE_OLD_RAMP;
				
			
			Databuff->sm = p_hydrdata->Min_turn;
			Databuff->MaxRpm = p_hydrdata->Max_turn;
			Databuff->kpPos = p_hydrdata->kp_pos;

			Databuff->k_MotortoAD = GetKMotorToAD(ServoNo)*10;
			Databuff->diffpos = diffpos;
			Databuff->movenum = movenum;			
			Databuff->endpress = p_hydrdata->Min_press;
			
			SetPosCtrlParaTran(Databuff);
			
            ApiFreeBuf(Databuff);
			return;
		}
		
	}
}



int NewPosControl(WORD PortNo, int SetPress, int SetSpd, int LastPress, int LastSpd)
{
	PPOS_CTRL_PROC_RT_PARA Databuff = NULL;
	int spdbuff = 0;
	
	if((Databuff = (PPOS_CTRL_PROC_RT_PARA)ApiNewPkt(PortNo, 0, 0, sizeof(POS_CTRL_PROC_RT_PARA))) != NULL)
	{
		Databuff->SetPress = SetPress;
		Databuff->SetSpd = SetSpd;
		Databuff->SetPressLast = LastPress;
		Databuff->SetSpdLast = LastSpd;
		
		spdbuff = NewPosControlTran(Databuff);
		
		ApiFreeBuf(Databuff);
		
		return spdbuff;
	}

	return 0;
}

