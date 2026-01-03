#include	"common.h"
#include	"AutoRunWithOutServo.h"
#include	"EtherCATProc.h"

#include 	"machapi.h"
#include   "ioproc.h"


extern PDORECV	PdoRecvData[ELESERVONUM];
extern PDOSEND	PdoSendData[ELESERVONUM];
extern PPUMP	g_pumpn[12];

WORD AD_BUFFER_AUTO[4] = {0};
int Encodestart[4] = {0};




void	EcatPdoRecvDataExplain_Auto()
{
    /*
    1.	位置编码器值:6064 + 00:INT32  编码器单位  -231  --- 231    TPDO
    2.	实际转速    :606C + 00:INT32  单位指令/s  -231  --- 231    TPDO
    3.	实际扭矩    :6077 + 00:INT16  0.1%       -5000 – 5000 TPDO
    */
    int i ;
	PPUMP p_pump = g_pumpn[0];
	if (p_pump == NULL)
	{
		return;
	}
	
    for (i = 0; i < 1; i++)
    {
//        PdoRecvData[i].RealEncode 		= PdoRecvData[i].RealEncode + PdoSendData[i].SpeedCmd/2000;
//        PdoRecvData[i].RealSpeed 		= PdoSendData[i].SpeedCmd;
//        PdoRecvData[i].RealTorque 		= 200;
//        PdoRecvData[i].AlarmPara		= 0;
//        PdoRecvData[i].ServoBusVoltage 	= 5500;
//        PdoRecvData[i].RealPI			= 10;
//        PdoRecvData[i].RealPV			= 10;
//        PdoRecvData[i].ServoFlag		= 0;
//        PdoRecvData[i].RealPowr			= PdoRecvData[i].RealPI * PdoRecvData[i].RealPV;

       // PdoRecvData[i].ServoEnable		= 1;

       // PdoRecvData[i].ServoState 		= 1;

	   if(!(TestPOIsOn(GetSys()->pclmp->po->O_OPN_SOL) || TestPOIsOn(GetSys()->pclmp->po->O_CLS_SOL)))
	 {
		  Encodestart[1] = PdoRecvData[i].RealEncode;
	 }

	    if(!(TestPOIsOn(GetSys()->pejt->po->O_EJT_FWD) || TestPOIsOn(GetSys()->pejt->po->O_EJT_BWD)))
	 {
		  Encodestart[2] = PdoRecvData[i].RealEncode;
	 }

		 if(!(TestPOIsOn(GetSys()->pinj->po->O_INJ_SOL) || TestPOIsOn(GetSys()->pinj->po->O_SUC_SOL)))
	 {
		  Encodestart[0] = PdoRecvData[i].RealEncode;
	 }

	   if(TestPOIsOn(GetSys()->pinj->po->O_SUC_SOL))
	  {
		   AD_BUFFER_AUTO[0] = AD_BUFFER_AUTO[0] + ((UI32)(PdoRecvData[i].RealEncode - Encodestart[0])*5/p_pump->EcoderPPR);
	  }

	  if(TestPOIsOn(GetSys()->pinj->po->O_INJ_SOL))
	  {
		   AD_BUFFER_AUTO[0] = AD_BUFFER_AUTO[0] - ((UI32)(PdoRecvData[i].RealEncode - Encodestart[0])*5/p_pump->EcoderPPR);
	  }

	   if(TestPOIsOn(GetSys()->pclmp->po->O_OPN_SOL))
	   {
			AD_BUFFER_AUTO[1] = AD_BUFFER_AUTO[1] + ((UI32)(PdoRecvData[i].RealEncode - Encodestart[1])*5/p_pump->EcoderPPR);
	   }	  

	   if(TestPOIsOn(GetSys()->pclmp->po->O_CLS_SOL))
	   {
			AD_BUFFER_AUTO[1] = AD_BUFFER_AUTO[1] - ((UI32)(PdoRecvData[i].RealEncode - Encodestart[1])*5/p_pump->EcoderPPR);
	   }

	   if(TestPOIsOn(GetSys()->pejt->po->O_EJT_FWD))
	   {
			AD_BUFFER_AUTO[2] = AD_BUFFER_AUTO[2] + ((UI32)(PdoRecvData[i].RealEncode - Encodestart[2])*5/p_pump->EcoderPPR);
	   }

	   if(TestPOIsOn(GetSys()->pejt->po->O_EJT_BWD))
	   {
			AD_BUFFER_AUTO[2] = AD_BUFFER_AUTO[2] - ((UI32)(PdoRecvData[i].RealEncode - Encodestart[2])*5/p_pump->EcoderPPR);
	   }

	   
    }
}

void EncodeRz_Auto()
{
	
}

