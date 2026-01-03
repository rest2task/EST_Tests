#include "moldtemp.h"
#include "file.h"
#include "string.h"
#include "oprintf.h" 
#include "canmod.h"
#include "database.h"


static CPROC_INFO MoldTempProcs[] = 
{
	{"pageIn",	MoldTempPageIn}
};

DB_TEMPMOLD     g_dbtempmDefault;
DB_TEMPMOLD2    g_dbtempmDefault2;

DB_TEMPMOLD     g_dbtempm;
DB_TEMPMOLD2    g_dbtempm2;

DB_TEMPMOLD   g_dbtempmDefault = {
    0xEB90,                     //已使用标志 EB90 wMarkUsed

    1,                          //nTempId;
    {0,0,0,0,0,0,0,0},          //wData[8];
    {25,25,25,25,25,25,25,25},  //UI16     wSetData[8];
    {0,0,0,0,0,0,0,0},          //UI16     wFuncData[8];

    0,            			  //wHeat;
    0,                        //wStart;
    0,                        //	wStart1;
    100,                      //	wHold;
    0,                        //wIntermediate;
    {25,0,0,0,0},             //wPid[5];
    {0,0,0,0,0}               //wReservd[5];
};

DB_TEMPMOLD2   g_dbtempmDefault2 = {
    0xEB90,                     //已使用标志 EB90 wMarkUsed

    1,                          //nTempId;
    {0,0,0,0,0,0,0,0},          //wData[8];
    {25,25,25,25,25,25,25,25},  //UI16     wSetData[8];
    {0,0,0,0,0,0,0,0},          //UI16     wFuncData[8];

    0,            			  //wHeat;
    0,                        //wStart;
    0,                        //	wStart1;
    100,                      //	wHold;
    0,                        //wIntermediate;
    {25,0,0,0,0},             //wPid[5];
    {0,0,0,0,0}               //wReservd[5];
};

extern TASKCAN  g_TaskCan;
void MoldTempRecvData(UI8 cCanType, UI8 cType,UI8 kind,UI8 cCanTypex,UI8 nServoId);

void MoldTempInital(PCMOD pmod)
{
	//1.Register functions
	RegisterModProcs(pmod, MoldTempProcs, ARRAY_NUM(MoldTempProcs));
    //CanDataRxCbSet(MoldTempRecvData);
}

BOOL MoldTempPageIn(PCPAR para)
{

	return TRUE;
}

void MoldTempRecvData(UI8 cCanType, UI8 cType,UI8 kind,UI8 cCanTypex,UI8 nServoId)
{
	UI8 cCanType2,cCanType1;
	UI8 nTempMoldId;
	UI16 i;
	BOOL bTime=FALSE;
	BOOL bTime2=FALSE;
	static BOOL bTempHand = TRUE;
	static BOOL bTempHand2 = TRUE;
	static UI16 wCount=0;
	static UI16 wCount2=0;
	static UI32 time_250ms = 0;

	if((GetTick() - time_250ms) > 250)
	{
		time_250ms = GetTick();
		bTime=TRUE;
		bTime2=TRUE;
	}


	//if  (!read(fdcan,g_TaskCan.cRxBuf,12))
	//{
	if((bTime)&&(wCount>100)){ 
		wCount=0;
		VarAdrSetInt(MDTEMP_STATEX1,0);
	}
	if((bTime2)&&(wCount2>100)){
		wCount2=0;
		VarAdrSetInt(MDTEMP_STATEX2,0);
	}
	//wCount++;
	//wCount2++;
	//return;
	//}

	//fast mold temp
	if(kind==CAN_HSPDTEMP_KIND)
	{
		cCanType2=g_TaskCan.cRxBuf[3];
		cCanType1=g_TaskCan.cRxBuf[1];   
		nTempMoldId=g_TaskCan.cRxBuf[3]&0x07;
		if(nTempMoldId==1)
		{
			wCount=0;
			if (cCanType2 == ((CAN_HSPDTEMP_KIND << 3) | (1 & 0x07))) 
			{  //hand
				if((g_TaskCan.cRxBuf[4]==0x55) && (cCanType1 == ((CAN_NMT_CTRL_CODE << 4) | CAN_PANEL_KIND ))) 
				{
					VarAdrSetInt(MDTEMP_STATEX1,1);
					bTempHand=FALSE;
					bTime=FALSE;
					CreateTaskCan(CAN_TX_TEMPRESPONSE,1,0,0x02);
					for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TSET,1,i,g_dbtempm.wSetData[i]);
					for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TFUNC,1,i,g_dbtempm.wFuncData[i]);

					CreateTaskCan(CAN_TX_THEAT,1,0,g_dbtempm.wHeat);
					CreateTaskCan(CAN_TX_TSTART,1,0,g_dbtempm.wStart);
					CreateTaskCan(CAN_TX_TSTART1,1,0,g_dbtempm.wStart1);
					CreateTaskCan(CAN_TX_THOLD,1,0,g_dbtempm.wHold);
					CreateTaskCan(CAN_TX_TINTERMIDIA,1,0,g_dbtempm.wIntermediate);
					return;
				}
				else if((g_TaskCan.cRxBuf[4] == 0xAA) && (cCanType1 == ((CAN_NMT_CTRL_CODE << 4) | CAN_PANEL_KIND )))
				{
					VarAdrSetInt(MDTEMP_STATEX1,1);
					bTime=FALSE;
					if(bTempHand)
					{
						CreateTaskCan(CAN_TX_TEMPRESPONSE,1,0,0x02);
						for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TSET,1,i,g_dbtempm.wSetData[i]);
						for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TFUNC,1,i,g_dbtempm.wFuncData[i]);

						CreateTaskCan(CAN_TX_THEAT,1,0,g_dbtempm.wHeat);
						CreateTaskCan(CAN_TX_TSTART,1,0,g_dbtempm.wStart);
						CreateTaskCan(CAN_TX_TSTART1,1,0,g_dbtempm.wStart1);
						CreateTaskCan(CAN_TX_THOLD,1,0,g_dbtempm.wHold);
						CreateTaskCan(CAN_TX_TINTERMIDIA,1,0,g_dbtempm.wIntermediate);
						bTempHand=FALSE;
					}	
                    if(VarAdrToUI16(TEMP_FL_HEATERSTATUS)) CreateTaskCan(CAN_TX_THOTKEY,1,0,1);
					else CreateTaskCan(CAN_TX_THOTKEY,1,0,0);
					CreateTaskCan(CAN_TX_HEARTBEAT,1,0,0xAA);
				}
				else if ((g_TaskCan.cRxBuf[6]==0x11) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND ))) 
				{   //实时温度
					VarAdrSetInt(MDTEMP_STATEX1,1);
					bTime=FALSE;
					i=g_TaskCan.cRxBuf[8];
					g_dbtempm.wData[i]=(g_TaskCan.cRxBuf[9]<<8) | g_TaskCan.cRxBuf[10];
				}
				else if ((g_TaskCan.cRxBuf[6]==0x12) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND))) 
				{
					VarAdrSetInt(MDTEMP_STATEX1,1);
					bTime=FALSE;
					i=g_TaskCan.cRxBuf[8];
					g_dbtempm.wReservd[i]=(g_TaskCan.cRxBuf[9]<<8) | g_TaskCan.cRxBuf[10];
				}
				else if ((g_TaskCan.cRxBuf[6]==0x13) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND))) 
				{
					VarAdrSetInt(MDTEMP_STATEX1,1);
					bTime=FALSE;
					VarAdrSetInt(MDTEMP_STATE1,g_TaskCan.cRxBuf[8]);
				}
				else if ((g_TaskCan.cRxBuf[6]==0x14) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND))) 
				{
					VarAdrSetInt(MDTEMP_STATEX1,1);
					bTime=FALSE;
					VarAdrSetInt(MDTEMP_VERSION1,(g_TaskCan.cRxBuf[7]<<8) | g_TaskCan.cRxBuf[8]);
				}
			}
		}
		else if(nTempMoldId==2)
		{
			wCount2=0;
			if (cCanType2 == ((CAN_HSPDTEMP_KIND << 3) | (2 & 0x07))) 
			{  //hand
				if((g_TaskCan.cRxBuf[4]==0x55) && (cCanType1 == ((CAN_NMT_CTRL_CODE << 4) | CAN_PANEL_KIND ))) 
				{
					VarAdrSetInt(MDTEMP_STATEX2,1);
					bTime2=FALSE;
					bTempHand2=FALSE;
					CreateTaskCan(CAN_TX_TEMPRESPONSE,2,0,0x02);
					for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TSET,2,i,g_dbtempm2.wSetData[i]);
					for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TFUNC,2,i,g_dbtempm2.wFuncData[i]);

					CreateTaskCan(CAN_TX_THEAT,2,0,g_dbtempm2.wHeat);
					CreateTaskCan(CAN_TX_TSTART,2,0,g_dbtempm2.wStart);
					CreateTaskCan(CAN_TX_TSTART1,2,0,g_dbtempm2.wStart1);
					CreateTaskCan(CAN_TX_THOLD,2,0,g_dbtempm2.wHold);
					CreateTaskCan(CAN_TX_TINTERMIDIA,2,0,g_dbtempm2.wIntermediate);
					return;
				}
				else if((g_TaskCan.cRxBuf[4] == 0xAA) && (cCanType1 == ((CAN_NMT_CTRL_CODE << 4) | CAN_PANEL_KIND )))
				{
					VarAdrSetInt(MDTEMP_STATEX2,1);
					bTime2=FALSE;
					if(bTempHand2)
					{
						CreateTaskCan(CAN_TX_TEMPRESPONSE,2,0,0x02);
						for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TSET,2,i,g_dbtempm2.wSetData[i]);
						for(i=0;i<8;i++) CreateTaskCan(CAN_TX_TFUNC,2,i,g_dbtempm2.wFuncData[i]);

						CreateTaskCan(CAN_TX_THEAT,2,0,g_dbtempm2.wHeat);
						CreateTaskCan(CAN_TX_TSTART,2,0,g_dbtempm2.wStart);
						CreateTaskCan(CAN_TX_TSTART1,2,0,g_dbtempm2.wStart1);
						CreateTaskCan(CAN_TX_THOLD,2,0,g_dbtempm2.wHold);
						CreateTaskCan(CAN_TX_TINTERMIDIA,2,0,g_dbtempm2.wIntermediate);
						bTempHand2=FALSE;
					}	
                    if(VarAdrToUI16(TEMP_FL_HEATERSTATUS)) CreateTaskCan(CAN_TX_THOTKEY,2,0,1);
					else CreateTaskCan(CAN_TX_THOTKEY,2,0,0);
					CreateTaskCan(CAN_TX_HEARTBEAT,2,0,0xAA);
				}
				else if ((g_TaskCan.cRxBuf[6]==0x11) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND ))) 
				{   //实时温度
					VarAdrSetInt(MDTEMP_STATEX2,1);
					bTime2=FALSE;
					i=g_TaskCan.cRxBuf[8];
					g_dbtempm2.wData[i]=(g_TaskCan.cRxBuf[9]<<8) | g_TaskCan.cRxBuf[10];
				}
				else if ((g_TaskCan.cRxBuf[6]==0x12) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND))) 
				{
					VarAdrSetInt(MDTEMP_STATEX2,1);
					bTime2=FALSE;
					i=g_TaskCan.cRxBuf[8];
					g_dbtempm2.wReservd[i]=(g_TaskCan.cRxBuf[9]<<8) | g_TaskCan.cRxBuf[10];
				}
				else if ((g_TaskCan.cRxBuf[6]==0x13) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND))) 
				{
					VarAdrSetInt(MDTEMP_STATEX2,1);
					bTime2=FALSE;
					VarAdrSetInt(MDTEMP_STATE1,g_TaskCan.cRxBuf[8]);
				}
				else if ((g_TaskCan.cRxBuf[6]==0x14) && (cCanType1 == ((CAN_PDO_TX_CODE << 4) | CAN_PANEL_KIND))) 
				{
					VarAdrSetInt(MDTEMP_STATEX2,1);
					bTime2=FALSE;
					VarAdrSetInt(MDTEMP_VERSION1,(g_TaskCan.cRxBuf[7]<<8) | g_TaskCan.cRxBuf[8]);	
				}
			}
		}
	}
	else{
		if(bTime){ 
			VarAdrSetInt(MDTEMP_STATEX1,0);
		}
		if(bTime2){
			VarAdrSetInt(MDTEMP_STATEX2,0);
		}

	}
	//tempmold end
}


