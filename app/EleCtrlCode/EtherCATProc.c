/*===========================================================================+
|  Author   : Yanfeng 	                                                     |
|  Version  : V1.00                                                          |
|  Creation : 09/30/2014                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	"typedef.h"
#include	"EtherCATProc.h"
#include	"app_driver_func.h"
#include	"app_data.h"
#include	"admeter.h"
#include	"servo.h"
#include	"adjust.h"
#include	"definelist.h"
 #include 	"systempro.h"
#include 	"memtbl.h"
#include	"EleCtrlApp.h"
#include    "servo.h"
#include    "da.h"
#include "AutoRunWithOutServo.h"
#include "app_driver_func.h"

#ifdef HC_IS620_SLAVE
extern    int ecat_is620_board[NUM_IS620];    // ????????????????is620
extern    int ecat_is620_board_total;
#endif

#ifdef HC_IS580_SLAVE
extern	int ecat_is580_board[NUM_IS580];    // ????????????????is620
extern	int ecat_is580_board_total;
#endif

#ifdef HC_ES750_SLAVE
extern	int ecat_es750_board[NUM_ES750];    // ????????????????is620
extern	int ecat_es750_board_total;
#endif



void EcatReq(u8 *data, int pkg_cnt);
void PumpStru(void);
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
RPDO:PDO通讯
索引		子索引		映射地址				内容
0x1600	0x01			0x60FF 0020			速度指令
0x1600	0x02			0x60E0 0010			正向转矩
0x1600	0x03			0x60E1 0010			反向转矩

TPDO:PDO通讯
索引		子索引		映射地址				内容
0x1A00	0x01			0x6064 0020			实际位置
0x1A00	0x02			0x606C 0020			实际速度
0x1A00	0x03			0x6077 0010			实际转矩

编码器位置归零
1.	6060+00 = 6
2.	6098+00 = 35
3.	607C+00 = 0(或，某一具体数据)
4.	6040+00 = 6 --> 7 --> 15 --> 31
5.	结果：位置编码器值[6064+00] = 607C+00
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PDOSEND	PdoSendData[ELESERVONUM];
PDORECV	PdoRecvData[ELESERVONUM];

WORD 	EcatSdoSendData[6*2][32];	//slaves 4 to 8 
WORD 	EcatSdoRecvData[6*2][32];	//slaves 4 to 8 

uint32	ecat_op_flag;
extern   MACHINE m_machine;

WORD hexToDecimal(WORD hex)
{
    WORD decimal = 0;
	WORD base = 1;

    while (hex > 0)
    {
        WORD remainder = hex % 10;
        decimal += remainder * base;
        hex /= 10;
        base *= 16;
    }

    return decimal;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
|供其他文件调用的函数
|GetPulsePosByServoNo()::获取伺服驱动反馈的编码器值
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int GetRealPulsePosByServoNo(WORD ServoNo)
{
    return (PdoRecvData[ServoNo].RealEncode);
}

int	GetRealSpeedByServoNo(WORD	ServoNo)
{
    return (PdoRecvData[ServoNo].RealSpeed);
}

int GetRPMByServoNo(WORD	ServoNo)
{
	PSERVO	p_servo = &g_hydrservo;
	return p_servo->st->SERVO_SPEED[ServoNo];
}


void SetServoCtrl(WORD ServoNo,WORD CtrlWord)
{
	PdoSendData[ServoNo].CtrlWord = CtrlWord;
}

//	检测SDO发送数据后是否已经处理完毕
BOOL EcatSdoChk(WORD	ServoNo)
{
	UI16 state;
    if (ServoNo < GetRealSlaveNum())
    {
    	state = (EcatSdoSendData[ServoNo][1] >> 4)&0x0F;
    	if (state > 0 && (g_pumpn[ServoNo]->sdo_alarm_abort == 0))
		{
			g_pumpn[ServoNo]->st->SERVO_COMM_STL = state;
			g_pumpn[ServoNo]->st->SERVO_COMM_STH = 0;
			g_pumpn[ServoNo]->sdo_alarm_starttm = Get100MsTick();
		}

        return ((EcatSdoSendData[ServoNo][1] >> 8) & 0x01);
    }
    else
    {
        return (FALSE);
    }
}
BOOL EcatSdoRecvChk(WORD	ServoNo)
{
	UI16 state;
    if (ServoNo < GetRealSlaveNum())
    {
    	state = (EcatSdoRecvData[ServoNo][1] >> 4)&0x0F;
    	if (state > 0 && (g_pumpn[ServoNo]->sdo_alarm_abort == 0))
		{
			g_pumpn[ServoNo]->st->SERVO_COMM_STL = state;
			g_pumpn[ServoNo]->st->SERVO_COMM_STH = 0;
			g_pumpn[ServoNo]->sdo_alarm_starttm = Get100MsTick();
		}

        return ((EcatSdoRecvData[ServoNo][1] >> 8) & 0x01);
    }
    else
    {
        return (FALSE);
    }
}
WORD GetSdoControlWord(WORD ServoNo)
{
    return (EcatSdoSendData[ServoNo][1]);
}
void	InitEcatData()
{
    int i;
    for (i = 0; i < 4; i++)
    {
        PdoSendData[i].SpeedCmd  = 0;
        PdoSendData[i].ForTorque = 0;
        PdoSendData[i].RevTorque = 0;

        PdoRecvData[i].RealEncode = 0;
        PdoRecvData[i].RealSpeed  = 0;
        PdoRecvData[i].RealTorque = 0;
    }

}
void	SetEcatSdoSend(WORD	ServoNo, WORD Index, WORD SubIndex, DWORD SdoData, WORD DataLenth)
{
    EcatSdoSendData[ServoNo][0] = 12 + DataLenth / 8;

    EcatSdoSendData[ServoNo][1] = 0x0003;	//	控制字	非广播，写，数据需处理
    EcatSdoSendData[ServoNo][2] = 0x1301;	//	传输类型	SDO,COE,Mailbox
    EcatSdoSendData[ServoNo][3] = ServoNo + 1;		//	从站地址

    EcatSdoSendData[ServoNo][4] = Index;			//	索引值
    EcatSdoSendData[ServoNo][5] = SubIndex << 8;		//	子索引值
    if (DataLenth == 32)
    {
        EcatSdoSendData[ServoNo][6] = SdoData & 0xffff;		//
        EcatSdoSendData[ServoNo][7] = SdoData >> 16;		//
    }
    else
    {
        EcatSdoSendData[ServoNo][6] = SdoData;
    }
    ec_app_inter[ServoNo]->data_buff = (uint8 *)EcatSdoSendData[ServoNo];
    EcatReq((u8*)EcatSdoSendData[ServoNo], 1);
}


void	SetEcatSdoRecv(WORD	ServoNo, WORD Index, WORD SubIndex, WORD DataLenth)
{
    EcatSdoRecvData[ServoNo][0] = 12 + DataLenth / 8;

    EcatSdoRecvData[ServoNo][1] = 0x0001;	//	控制字	非广播，读，数据需处理
    EcatSdoRecvData[ServoNo][2] = 0x1301;	//	传输类型	SDO,COE,Mailbox
    EcatSdoRecvData[ServoNo][3] = ServoNo + 1;		//	从站地址
    EcatSdoRecvData[ServoNo][4] = Index;			//	索引值
    EcatSdoRecvData[ServoNo][5] = SubIndex << 8;		//	子索引值
    EcatSdoRecvData[ServoNo][6] = 0;

    ec_app_inter[ServoNo]->data_buff = (uint8 *)EcatSdoRecvData[ServoNo];
    EcatReq((u8*)EcatSdoRecvData[ServoNo], 1);
}

DWORD	GetSdoRecv(WORD	ServoNo, WORD NUM)
{
    return (EcatSdoRecvData[ServoNo][NUM]);
}


UI16 GetSERVO_CURRENT2(WORD SlaveNo)
{
    PSERVO	p_servo = &g_hydrservo;

    return p_servo->st->SERVO_CURRENT[SlaveNo];

}

#include "CalibrationAggControl.h"

extern CalibrationSlopeData CSlopeData;
extern UI16 m_speed;

void	SetEcatPdoSend()
{
	PDA pd = (PDA)GetPartByID(MK_PART(DA_ID, 1));
	PSERVO  pp = &g_hydrservo;
	PMACHINE pm = &m_machine;
	int a = 0,b = 0,c = 0,d = 0;
	
    int i,j,speed;
	UI16 ServoType = 0,Servo750Num = 0;


	for(i = 0; i < GetRealSlaveNum(); i++)
	{
		ServoType = pm->st->SERVO_TYPE[i];
		j = i - Servo750Num;
		switch(ServoType)
		{
			case TYPE_IS620:
				if (pp->mh->FL_SERVO_CURVE > 0)
				{
					a = g_pumpn[i]->McPara.Mcnow[1];
					b = g_press[i].PressSet[1].wData;
					c = m_speed;
					d = g_pumpn[i]->McCtrl.RealPres;
				}
				speed = PdoSendData[i].SpeedCmd * (g_pumpn[i]->EcoderPPR/ 60.0f);
				app_rxpdo_buf[j][0]=speed&0xff;
				app_rxpdo_buf[j][1]=(speed>>8)&0xff;
				app_rxpdo_buf[j][2]=(speed>>16)&0xff;
				app_rxpdo_buf[j][3]=(speed>>24)&0xff;
				
				app_rxpdo_buf[j][4]=PdoSendData[i].ForTorque&0xff;
				app_rxpdo_buf[j][5]=(PdoSendData[i].ForTorque>>8)&0xff;
				
				app_rxpdo_buf[j][6]=PdoSendData[i].RevTorque&0xff;
				app_rxpdo_buf[j][7]=(PdoSendData[i].RevTorque>>8)&0xff;
				
				app_rxpdo_buf[j][8] = PdoSendData[i].CtrlWord&0xff;
				app_rxpdo_buf[j][9] = (PdoSendData[i].CtrlWord>>8)&0xff;
				
				app_rxpdo_buf[j][10]=a&0xff;
				app_rxpdo_buf[j][11]=(a>>8)&0xff;
				app_rxpdo_buf[j][12]=(a>>16)&0xff;
				app_rxpdo_buf[j][13]=(a>>24)&0xff;
				
				app_rxpdo_buf[j][14]=b&0xff;
				app_rxpdo_buf[j][15]=(b>>8)&0xff;
				app_rxpdo_buf[j][16]=(b>>16)&0xff;
				app_rxpdo_buf[j][17]=(b>>24)&0xff;
					
				app_rxpdo_buf[j][18]=c&0xff;
				app_rxpdo_buf[j][19]=(c>>8)&0xff;
				app_rxpdo_buf[j][20]=(c>>16)&0xff;
				app_rxpdo_buf[j][21]=(c>>24)&0xff;
				
				app_rxpdo_buf[j][22]=d&0xff;
				app_rxpdo_buf[j][23]=(d>>8)&0xff;
				app_rxpdo_buf[j][24]=(d>>16)&0xff;
				app_rxpdo_buf[j][25]=(d>>24)&0xff;
				break;
			case TYPE_IS580:
				app_rxpdo_buf[j][0]= PdoSendData[i].SpeedCmd&0xff;
				app_rxpdo_buf[j][1]=(PdoSendData[i].SpeedCmd>>8)&0xff;
				app_rxpdo_buf[j][2]=(PdoSendData[i].SpeedCmd>>16)&0xff;
				app_rxpdo_buf[j][3]=(PdoSendData[i].SpeedCmd>>24)&0xff;
				app_rxpdo_buf[j][4]= PdoSendData[i].ForTorque&0xff;
				app_rxpdo_buf[j][5]=(PdoSendData[i].ForTorque>>8)&0xff;
				app_rxpdo_buf[j][6]= PdoSendData[i].CtrlWord&0xff;
				app_rxpdo_buf[j][7]=(PdoSendData[i].CtrlWord>>8)&0xff;
				break;
			case TYPE_ES750:
				app_rxpdo_buf[j][0]= PdoSendData[i].SpeedCmd&0xff;
				app_rxpdo_buf[j][1]=(PdoSendData[i].SpeedCmd>>8)&0xff;
				app_rxpdo_buf[j][2]=(PdoSendData[i].SpeedCmd>>16)&0xff;
				app_rxpdo_buf[j][3]=(PdoSendData[i].SpeedCmd>>24)&0xff;
				app_rxpdo_buf[j][4]= PdoSendData[i].ForTorque&0xff;
				app_rxpdo_buf[j][5]=(PdoSendData[i].ForTorque>>8)&0xff;
				app_rxpdo_buf[j][6]= PdoSendData[i].CtrlWord&0xff;
				app_rxpdo_buf[j][7]=(PdoSendData[i].CtrlWord>>8)&0xff;
				Servo750Num++;
				break;
			case TYPE_ES750_2:
				app_rxpdo_buf[j][8]= PdoSendData[i].SpeedCmd&0xff;
				app_rxpdo_buf[j][9]=(PdoSendData[i].SpeedCmd>>8)&0xff;
				app_rxpdo_buf[j][10]=(PdoSendData[i].SpeedCmd>>16)&0xff;
				app_rxpdo_buf[j][11]=(PdoSendData[i].SpeedCmd>>24)&0xff;
				app_rxpdo_buf[j][12]= PdoSendData[i].ForTorque&0xff;
				app_rxpdo_buf[j][13]=(PdoSendData[i].ForTorque>>8)&0xff;
				app_rxpdo_buf[j][14]= PdoSendData[i].CtrlWord&0xff;
				app_rxpdo_buf[j][15]=(PdoSendData[i].CtrlWord>>8)&0xff; 
				break;
			default:
				break;
		}
	}
}

void	EcatPdoRecvDataExplain()
{
    /*
    1.	位置编码器值:6064 + 00:INT32  编码器单位  -231  --- 231    TPDO
    //2.	实际转速    :606C + 00:INT32  单位指令/s  -231  --- 231    TPDO
    2.	实际速度	:200B + 0x38:INT32 0.1rpm/s	(原实际转速换算太耗时)
    3.	实际扭矩    :6077 + 00:INT16  0.1%       -5000 – 5000 TPDO
    */
	UI16 ServoType = 0,Servo750Num = 0;
	UI16 i = 0,j = 0;
	
	PMACHINE pm = &m_machine;
	PSERVO    pp = &g_hydrservo;
	
	for(i = 0; i < GetRealSlaveNum(); i++)
	{
		ServoType = pm->st->SERVO_TYPE[i];
		j = i - Servo750Num;
		switch(ServoType)
		{
			case TYPE_IS620:
				PdoRecvData[i].RealEncode	= (app_txpdo_buf[j][3]<<24) | (app_txpdo_buf[j][2]<<16) | (app_txpdo_buf[j][1]<<8) | app_txpdo_buf[j][0];
				PdoRecvData[i].RealSpeed	= (app_txpdo_buf[j][7]<<24) | (app_txpdo_buf[j][6]<<16) | (app_txpdo_buf[j][5]<<8) | app_txpdo_buf[j][4];
				PdoRecvData[i].RealTorque	= (app_txpdo_buf[j][9]<<8)	| app_txpdo_buf[j][8];
				PdoRecvData[i].AlarmPara	= (app_txpdo_buf[j][11]<<8) | app_txpdo_buf[j][10]; 	
				PdoRecvData[i].ServoTemp	= (app_txpdo_buf[j][13]<<8) | app_txpdo_buf[j][12];
				PdoRecvData[i].MotorTemp	= (app_txpdo_buf[j][15]<<8) | app_txpdo_buf[j][14];
				PdoRecvData[i].RealPI		= (app_txpdo_buf[j][17]<<8) | app_txpdo_buf[j][16];
				PdoRecvData[i].ServoFlag	= (app_txpdo_buf[j][19]<<8) | app_txpdo_buf[j][18];
				pp->st->SERVO_SPEED[i]= GetSpdRPMBySpdPulse(i,PdoRecvData[i].RealSpeed);
				SetServoState(i,PdoRecvData[i].RealEncode,GetSpdRPMBySpdPulse(i,PdoRecvData[i].RealSpeed),PdoRecvData[i].RealTorque,PdoRecvData[i].AlarmPara,PdoRecvData[i].ServoTemp,
					PdoRecvData[i].RealPI,PdoRecvData[i].RealPV,PdoRecvData[i].ServoFlag,PdoRecvData[i].MotorTemp,PdoRecvData[i].ServoState);	
				break;
			case TYPE_IS580:
				PdoRecvData[i].RealSpeed	= (app_txpdo_buf[j][3]<<24) | (app_txpdo_buf[j][2]<<16) | (app_txpdo_buf[j][1]<<8) | app_txpdo_buf[j][0];
				PdoRecvData[i].RealTorque	= (app_txpdo_buf[j][5]<<8)	| app_txpdo_buf[j][4];
				PdoRecvData[i].AlarmPara	= (app_txpdo_buf[j][9]<<24) | (app_txpdo_buf[j][8]<<16) | (app_txpdo_buf[j][7]<<8) | app_txpdo_buf[j][6];
				PdoRecvData[i].ServoTemp	= (app_txpdo_buf[j][11]<<8)  | app_txpdo_buf[j][10];
				PdoRecvData[i].MotorTemp	= (app_txpdo_buf[j][13]<<8) | app_txpdo_buf[j][12];
				PdoRecvData[i].RealPI		= (app_txpdo_buf[j][15]<<8) | app_txpdo_buf[j][14];
				PdoRecvData[i].ServoFlag	= (app_txpdo_buf[j][17]<<8) | app_txpdo_buf[j][16];
				pp->st->SERVO_SPEED[i]=PdoRecvData[i].RealSpeed;
				SetServoState(i,PdoRecvData[i].RealEncode,PdoRecvData[i].RealSpeed,PdoRecvData[i].RealTorque,hexToDecimal(PdoRecvData[i].AlarmPara),PdoRecvData[i].ServoTemp,
					PdoRecvData[i].RealPI,PdoRecvData[i].RealPV,PdoRecvData[i].ServoFlag,PdoRecvData[i].MotorTemp,PdoRecvData[i].ServoState);	
				break;
			case TYPE_ES750:
				PdoRecvData[i].RealSpeed	= (app_txpdo_buf[j][3]<<24) | (app_txpdo_buf[j][2]<<16) | (app_txpdo_buf[j][1]<<8) | app_txpdo_buf[j][0];
				PdoRecvData[i].RealTorque	= (app_txpdo_buf[j][5]<<8)	| app_txpdo_buf[j][4];
				PdoRecvData[i].AlarmPara	= (app_txpdo_buf[j][9]<<24) | (app_txpdo_buf[j][8]<<16) | (app_txpdo_buf[j][7]<<8) | app_txpdo_buf[j][6];
				PdoRecvData[i].ServoTemp	= (app_txpdo_buf[j][11]<<8)  | app_txpdo_buf[j][10];
				PdoRecvData[i].MotorTemp	= (app_txpdo_buf[j][13]<<8) | app_txpdo_buf[j][12];
				PdoRecvData[i].RealPI		= (app_txpdo_buf[j][15]<<8) | app_txpdo_buf[j][14];
				PdoRecvData[i].ServoFlag	= (app_txpdo_buf[j][17]<<8) | app_txpdo_buf[j][16];
				pp->st->SERVO_SPEED[i]=PdoRecvData[i].RealSpeed;
				SetServoState(i,PdoRecvData[i].RealEncode,PdoRecvData[i].RealSpeed,PdoRecvData[i].RealTorque,hexToDecimal(PdoRecvData[i].AlarmPara),PdoRecvData[i].ServoTemp,
					PdoRecvData[i].RealPI,PdoRecvData[i].RealPV,PdoRecvData[i].ServoFlag,PdoRecvData[i].MotorTemp,PdoRecvData[i].ServoState);	
				Servo750Num++;
				break;
			case TYPE_ES750_2:
				PdoRecvData[i].RealSpeed	= (app_txpdo_buf[j][21]<<24) | (app_txpdo_buf[j][20]<<16) | (app_txpdo_buf[j][19]<<8) | app_txpdo_buf[j][18];
				PdoRecvData[i].RealTorque	= (app_txpdo_buf[j][23]<<8)  | app_txpdo_buf[j][22];
				PdoRecvData[i].AlarmPara	= (app_txpdo_buf[j][27]<<24) | (app_txpdo_buf[j][26]<<16) | (app_txpdo_buf[j][25]<<8) | app_txpdo_buf[j][24];
				PdoRecvData[i].ServoTemp	= (app_txpdo_buf[j][29]<<8)  | app_txpdo_buf[j][28];
				PdoRecvData[i].MotorTemp	= (app_txpdo_buf[j][31]<<8)  | app_txpdo_buf[j][30];
				PdoRecvData[i].RealPI		= (app_txpdo_buf[j][33]<<8)  | app_txpdo_buf[j][32];
				PdoRecvData[i].ServoFlag	= (app_txpdo_buf[j][35]<<8)  | app_txpdo_buf[j][34];
				pp->st->SERVO_SPEED[i]=PdoRecvData[i].RealSpeed;
				SetServoState(i,PdoRecvData[i].RealEncode,PdoRecvData[i].RealSpeed,PdoRecvData[i].RealTorque,hexToDecimal(PdoRecvData[i].AlarmPara),PdoRecvData[i].ServoTemp,
					PdoRecvData[i].RealPI,PdoRecvData[i].RealPV,PdoRecvData[i].ServoFlag,PdoRecvData[i].MotorTemp,PdoRecvData[i].ServoState);	
				break;
			default:
				break;
		}
	}
}

void	EcatProc()
{
	SetEcatPdoSend();
}
//gonggw, 2015.04.24, separate EcatOPProc to EcatReceive and EcatSend

void EcatReceive()
{
    if (cur_master->main_device.open == 0)
    {
        return;
    }
    ecat_receive();
    ecat_state_check();
}
void EcatSend()
{
    if (cur_master->main_device.open == 0)
    {
        return;
    }

    if (ecat_sr_flag == 1)
    {
        return;
    }
    EcatProcess();
    ecat_queue();
    ecat_send();
}

void	EcatOPProc()
{
    EcatProcess();
    ecat_receive();
    EcatPdoRecvDataExplain();
    EcatProc();		//	将控制指令通过EtherCAT发送
    ecat_state_check();
    ecat_queue();
    ecat_send();
}
DWORD GetEcatOpFlag()
{
    return (ecat_op_flag);
}
void SetEcatOpFlag(DWORD flag_ecat_op)
{
    ecat_op_flag = flag_ecat_op;
}
BOOL EcatCtrlOper()
{
    if (GetEcatOpFlag() < 2)
    {
        if (GetEcatOpFlag() == 0)
        {
            EcatReceive();
            if (ecat_rec_flag)
            {
                SetEcatOpFlag(1);
                ecat_rec_flag = 0;
            }
        }
        else if (GetEcatOpFlag() == 1)
        {
            EcatPdoRecvDataExplain();
#if BOGUS
			EcatPdoRecvDataExplain_Auto();
#endif
			PumpStru();

			if (!ChkMachineErr(ER0_MEM))
            {
                SdoProcess();
            }

            EcatProc(); //	将控制指令通过EtherCAT发送
            EcatSend();
            SetEcatOpFlag(2); 

			return TRUE;
        }
    }
    else
    {
    }

	return FALSE;
}

WORD GetServoIsEnable(WORD SlaveNo)
{
    if (SlaveNo < GetRealSlaveNum())
    {
    	return ((ChkServoState(SERVOST_ENABLED) >> SlaveNo) & 0x01);
    }
    else
    {
        return 0;
    }
}


WORD GetSlaveZeroFlag(WORD SlaveNo)
{
    if (SlaveNo < GetRealSlaveNum())
    {
        return (PdoRecvData[SlaveNo].ZeroFlag);
    }
    else
    {
        return (0);
    }
}

I16 GetRealTorque(WORD SlaveNo)
{
    if (SlaveNo < GetRealSlaveNum())
    {
        return PdoRecvData[SlaveNo].RealTorque;
    }
    else
    {
        return 0;
    }
}


BOOL IsBusVLower(WORD V)
{
    int i;
    for (i = 0; i < GetRealSlaveNum(); i++)
    {
        if (PdoRecvData[i].ServoBusVoltage > V)
        {
            return FALSE;
        }
    }

    return TRUE;
}


UI8 ChkServoState(SERVO_STATEFLAG st)
{
	int i;
	UI8 ret = 0x00;
	switch(st)
	{
		case SERVOST_MODEERR:
			for(i=0;i<ECAT_SLV_NUM;i++)
			{
				if((PdoRecvData[i].ServoFlag&(WORD)(0x0001<<9))==0)
				{
					ret = ret|(0x01<<i);
				}
			}
			break;
		case SERVOST_NOVOL:
			for(i=0;i<ECAT_SLV_NUM;i++)
			{
				if((PdoRecvData[i].ServoFlag&(WORD)(0x0001<<4))==0)
				{
					ret = ret|(0x01<<i);
				}
			}
			break;
		case SERVOST_START:
			for(i=0;i<ECAT_SLV_NUM;i++)
			{
				if((PdoRecvData[i].ServoFlag&(WORD)(0x50))!=(WORD)(0x50))
				{
					ret = ret|(0x01<<i);
				}
			}
			break;
		case SERVOST_RDY:
			for(i=0;i<ECAT_SLV_NUM;i++)
			{
				if((PdoRecvData[i].ServoFlag&(WORD)(0x31))!=(WORD)(0x31))
				{
					ret = ret|(0x01<<i);
				}
			}
			break;
		case SERVOST_QUICK:
			for(i=0;i<ECAT_SLV_NUM;i++)
			{
				if((PdoRecvData[i].ServoFlag&(WORD)(0x33))!=(WORD)(0x33))
				{
					ret = ret|(0x01<<i);
				}
			}
			break;
		case SERVOST_FAULT:
			for(i=0;i<ECAT_SLV_NUM;i++)
			{
				if((PdoRecvData[i].ServoFlag&(WORD)((0x01<<7)|(0x01<<3)))>0)
				{
					ret = ret|(0x01<<i);
				}
			}
			break;
		case SERVOST_ENABLED:
			for(i=0;i<ECAT_SLV_NUM;i++)
			{
				if(((PdoRecvData[i].ServoFlag & 0x04) >> 2)>0)
				{
					ret = ret|(0x01<<i);
				}
			}
			break;
	}

	return ret;
}



UI16 GetServoErr(WORD SlaveNo)
{
    if (SlaveNo < GetRealSlaveNum())
    {
        return (PdoRecvData[SlaveNo].AlarmPara);
    }
    else
    {
        return (0);
    }
}
