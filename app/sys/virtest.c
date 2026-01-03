#include "hwcfg.h"

#ifdef VIRTEST
#include "vtest.h"
#include "machine.h"
#include "string.h"

#define VTEST_METER_NUM		4
#define VTEST_TEMP_NUM		15

#define CLMP_MAX_RAMP		8000		
#define INJ_MAX_RAMP		4000			
#define EJT_MAX_RAMP		4000			
#define DEF_UNIT_TM			500					/*100us, 50ms*/

#define MAX_TEMP_AD			(0x1000*100)
#define TEMP_UV_RATE_BASE	3					/*每10ms 基准变化数*/
#define TEMP_UV_RATE_COEF	16					/*每10ms 温度差变化数*/
#define TEMP_DEF_ZERO		340*100				/*缺省的归零值*/
#define TEMP_DEF_CHB		170					/*缺省的温度系数*/

#define A_CLSM	MK_ACT(CLAMP_ID, 1, OS_CLSM)
#define A_OPNM	MK_ACT(CLAMP_ID, 1, OS_OPNM)
#define A_INJ	MK_ACT(INJECT_ID, 1, OS_INJE)
#define A_CHRG	MK_ACT(INJECT_ID, 1, OS_CHRG)
#define A_SUCK	MK_ACT(INJECT_ID, 1, OS_SUCK)
#define A_EJTF	MK_ACT(EJECT_ID, 1, OS_EJTF)
#define A_EJTB	MK_ACT(EJECT_ID, 1, OS_EJTB)

typedef struct tyVTEST_M
{
	I32		chg_unit;
	UI16 	ad_val;
	UI16	ad_val_sav;
	UI16 	ad_ch;
	UI16	ramp;
	UI32 	last_100us;
}VTEST_M, *PVTEST_M;

typedef struct tyVTEST_TEMP
{
	BOOL	bon;
	I32	last_temp_ad;			/*ad值放大100倍*/
}VTEST_TEMP, *PVTEST_TEMP;

typedef struct tyVTEST
{
	VTEST_M 	meter[VTEST_METER_NUM];
	VTEST_TEMP	temp[VTEST_TEMP_NUM];
	BOOL		btemp_warmon;
	UI32		surtemp_uv;
	UI8			temp_cur_ch;
	UI32		last_ms_ticks;
}VTEST, *PVTEST;

VTEST vtst;

BOOL VTestGetAdValue(UI16 portno, UI16 *val)
{
	I32  chg;
	VTEST_M* pm;
	I32 diff_100us;
	
	if(portno > VTEST_METER_NUM || portno == 0)
		return FALSE;


	portno--;

	if(vtst.meter[portno].ad_ch == 0)
		return FALSE;
	
	pm = vtst.meter + portno;
	diff_100us = (I32)(Get100UsTick() - pm->last_100us);
	
//	if(diff_100us > DEF_UNIT_TM)
//		chg = pm->chg_unit;
//	else
		chg = (pm->chg_unit*diff_100us)/(DEF_UNIT_TM);

	if(chg != 0)
	{
	if(chg < 0 && pm->ad_val_sav < -chg)
		pm->ad_val = 0;
	else if(chg > 0 && pm->ad_val_sav + chg > MAX_ADVAULE)
		pm->ad_val = MAX_ADVAULE;
	else
		pm->ad_val = pm->ad_val_sav + chg;
	}
	
	if(diff_100us > DEF_UNIT_TM)
	{
		pm->ad_val_sav = pm->ad_val;
		pm->last_100us = Get100UsTick();
	}

	*val = pm->ad_val;
	return TRUE;
}

void VTestActFlowSet(PACT pact, UI16 flow)
{
	int meter;
	int flow_tmp;
	
	if(pact == NULL)
		return;

	flow_tmp = flow;
	meter = VTEST_METER_NUM;	
	switch(pact->gid)
	{
		case A_CLSM:
			meter = 1;
			flow_tmp = -flow_tmp;
			break;
		case A_OPNM:
			meter = 1;
			break;
		case A_INJ:
			meter = 0;
			flow_tmp = -flow_tmp;
			break;
		case A_CHRG:
			meter = 0;
			break;
		case A_SUCK:
			meter = 0;
			break;
		case A_EJTF:
			meter = 2;
			break;
		case A_EJTB:
			meter = 2;
			flow_tmp = -flow_tmp;
			break;
		default:
			break;
	}

	if(meter < ARRAY_NUM(vtst.meter))
	{
		vtst.meter[meter].chg_unit = ((I32)vtst.meter[meter].ramp*flow_tmp)/1000;
		vtst.meter[meter].ad_val_sav = vtst.meter[meter].ad_val;
		vtst.meter[meter].last_100us = Get100UsTick();
	}
}

void VTestFlowReset()
{
	int i;

	for(i = 0; i < ARRAY_NUM(vtst.meter); i++)
	{
		vtst.meter[i].chg_unit = 0;
	}

	vtst.meter[0].ad_ch = 1;
	vtst.meter[0].ramp = ((UI32)INJ_MAX_RAMP*MAX_ADVAULE)/0x10000;
	vtst.meter[1].ad_ch = 2;
	vtst.meter[1].ramp = ((UI32)CLMP_MAX_RAMP*MAX_ADVAULE)/0x10000;
	vtst.meter[2].ad_ch = 3;
	vtst.meter[2].ramp = ((UI32)EJT_MAX_RAMP*MAX_ADVAULE)/0x10000;
}

void VTESTWarmOn()
{
	vtst.btemp_warmon = TRUE;
}

void VTESTWarmOff()
{
	vtst.btemp_warmon = FALSE;
}

void VTESTSurTempUV(UI16 uv)
{
	vtst.surtemp_uv = uv;
}

void VTESTSetCurCh(UI8 ch)
{
	vtst.temp_cur_ch = ch;
}

void VTESTChHeatOnOff(UI8 ch, BOOL bon)
{
	if(ch < ARRAY_NUM(vtst.temp))
		vtst.temp[ch].bon = bon;
}

UI16 VTESTGetTempUV()
{
	if(vtst.temp_cur_ch < ARRAY_NUM(vtst.temp))
		return (UI16)(vtst.temp[vtst.temp_cur_ch].last_temp_ad/100 + 10);
	else
		return 0xffff;
}

void VTESTTempLoop()
{
	int i;
	
	if(GetMsTick() - vtst.last_ms_ticks > 100)
	{
		vtst.last_ms_ticks = GetMsTick();
		for(i = 0; i < ARRAY_NUM(vtst.temp); i++)
		{
			if(vtst.temp[i].bon && vtst.btemp_warmon)
				vtst.temp[i].last_temp_ad = vtst.temp[i].last_temp_ad + TEMP_UV_RATE_BASE + ((MAX_TEMP_AD - vtst.temp[i].last_temp_ad)*TEMP_UV_RATE_COEF)/MAX_TEMP_AD;
			else 
				vtst.temp[i].last_temp_ad = vtst.temp[i].last_temp_ad - TEMP_UV_RATE_BASE - (vtst.temp[i].last_temp_ad*TEMP_UV_RATE_COEF)/MAX_TEMP_AD;

			if(vtst.temp[i].last_temp_ad > MAX_TEMP_AD)
				vtst.temp[i].last_temp_ad = MAX_TEMP_AD - 1;
			else if(vtst.temp[i].last_temp_ad < TEMP_DEF_ZERO)
				vtst.temp[i].last_temp_ad = TEMP_DEF_ZERO;
		}
	}
}
void VTestTempReset()
{
	memset(&vtst, 0, sizeof(vtst));
	vtst.last_ms_ticks = GetMsTick();
	vtst.temp_cur_ch = 0;
}

UI8  vtest_sfdr_st;
UI16 vtest_sfdr_100ms;
UI32 VTESTPISet(UI32 pi_st)
{
	extern MACHINE 	 m_machine;
	
	PMACHINE pm = &m_machine;
	
	if(pm->pclmp !=NULL)
	{
		pi_st |= ((UI32)1 <<(pm->pclmp->pi->I_CLS_END -1));
		pi_st |= ((UI32)1 <<(pm->psfdr->pi->I_RSD_CLS - 1));
		pi_st |= ((UI32)1 <<(pm->pejt->pi->I_EJT_BWD- 1));
		pi_st |= ((UI32)1 <<(pm->pinj->pi->I_PUR_GUA- 1));
		pi_st |= ((UI32)1 <<(pm->pinj->pnzl->pi->I_NOZ_FWD) - 1);
		pi_st |= ((UI32)1 <<(pm->plub->pi->I_LUB_CHK) - 1);
		
		if(vtest_sfdr_st == 2)
		{
			/*打开安全门*/
			pi_st &= (~((UI32)1 <<(pm->psfdr->pi->I_SDR_CLS - 1)));
			pi_st &= (~((UI32)1 <<(pm->psfdr->pi->I_SDR_CL2 - 1)));
			if(Get100MsTick() - vtest_sfdr_100ms >= 20)
				vtest_sfdr_st = 0;
		}
		else
		{
			pi_st |= ((UI32)1 <<(pm->psfdr->pi->I_SDR_CLS - 1));
			pi_st |= ((UI32)1 <<(pm->psfdr->pi->I_SDR_CL2 - 1));
			if(vtest_sfdr_st == 1 && Get100MsTick() - vtest_sfdr_100ms >= 10)
			{
				vtest_sfdr_st = 2;
				vtest_sfdr_100ms = Get100MsTick();
			}
		}

		return pi_st;
	}
	else
	{
		return pi_st;
	}
}

void VTESTAutoStart()
{
	vtest_sfdr_st = 1;
	vtest_sfdr_100ms = Get100MsTick();
}

#endif
