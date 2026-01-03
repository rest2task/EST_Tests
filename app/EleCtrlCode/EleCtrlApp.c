/*===========================================================================+
|  Author   : Yanfeng 	                                                     |
|  Version  : V1.00                                                          |
|  Creation : 01/25/2016                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include "common.h"
#include "EleCtrlApp.h"
#include "EtherCATProc.h"
#include "admeter.h"
#include "hydr.h"
#include "servo.h"
#include "machapi.h"
#include "machine.h"
#include "definelist.h"
#include "app_driver_func.h"
#include "app_data.h"
 #include "systempro.h"

#include "AutoRunWithOutServo.h"


extern 	HYDR			g_hydrout;
extern UI16	g_pump_num;


UI16 GetRealEcatSlaveNum()
{
	return ec_app_data_p->real_slave_num;
}


/************************************************************************************
|从站连接状态:bit0 bit1...bit31
************************************************************************************/
WORD GetSlaveLink()
{
    //从站连接状态:bit0 bit1...bit31
    return (ec_app_data_p->slave_link_bitmap[0]);
}

/************************************************************************************
|从站丢失个数
************************************************************************************/
/*
WORD GetSlaveMissNum()
{//从站丢失个数
	return(ec_app_data_p->slave_miss);
}
*/
/************************************************************************************
|pdo连接超时个数
************************************************************************************/
WORD GetPdoOverTimeNum()
{
    //pdo连接超时个数
    return (ec_app_data_p->num_overtime_pdo);
}

/************************************************************************************
|网络状态
************************************************************************************/
WORD GetEcatNetState()
{
    //网络状态
    //ECAT_NET_INIT = 0,		/*无网络 			*/
    //ECAT_NET_OK, 			/*网络配置正确	*/
    //ECAT_NET_EMAC_ERR, 	/*EMAC初始化失败	*/
    //ECAT_NET_SLAVE_MISS, 	/*至少有一个从站失败(不在OP状态)*/
    //ECAT_NET_SLAVE_ERR,	/*所有从站失败 	*/
    //ECAT_NET_MASTER_ERR	/*Master失败 			*/
    return (ec_app_data_p->ecat_net_state);
}

/************************************************************************************
|实际从站个数
************************************************************************************/
WORD GetRealSlaveNum()
{
	if(ChkRunFlag(INIF_USEECAT))
	{
		return g_pump_num;
	}
	else
	{
		return 0;
	}
}

/************************************************************************************
|从站状态
************************************************************************************/
WORD GetSlaveState(WORD SlaveNo)
{
    //从站状态
    if (SlaveNo < ECAT_SLV_NUM)
    {
        return (ec_app_data_p->slave_state[SlaveNo]);
    }
    else
    {
        return (0);
    }
}

/************************************************************************************
|EtherCAT从站配置确认
************************************************************************************/
BOOL	SetAlarm_SlaveNum()	/*设置警报:从站个数不匹配*/
{

	return TRUE;
//	
//    PMOTOR pp = &m_motor;
//    BOOL ret = TRUE;
//    if (GetRealSlaveNum() != g_pump_num)//MAXSERVONUM)
//    {
//        SET_PART_ERR(pp, ER0_SALVNUM);
//        ret = FALSE;
//    }
//    return (ret);
}
BOOL	SetAlarm_NetState()	/*设置警报:网络初始化错误*/
{
    PMOTOR pp = &m_motor;
    BOOL ret = TRUE;
    if (GetEcatNetState() != 1)
    {
        SET_PART_ERR(pp, ER0_NETERR);
        ret = FALSE;
    }
    return (ret);
}
BOOL	SetAlarm_SlaveState()	/*设置警报:从站状态错误*/
{
    int i;
    PMOTOR pp = &m_motor;
    BOOL ret = TRUE;
	
    for (i = 0; i < ECAT_SLV_NUM; i++)
    {
        if (GetSlaveState(i) != 0x08)
        {
            SET_PART_ERR(pp, ER0_SALVERR);
            ret = FALSE;
        }
    }
	
    return (ret);
}
BOOL	SetAlarm_SlaveLineOff()	/*从站断线警报*/
{
    PMOTOR pp = &m_motor;
    BOOL ret = TRUE;

    if (GetSlaveLink() != 0)
    {
    	if (MotorIsOn2() || !ChkSysCfg(XF6_EFFC))
    	{
        	SET_PART_ERR(pp, ER0_SLVLOFF);
			ret = FALSE;
    	}       
    }
    else
    {
        CLR_PART_ERR(pp, ER0_SLVLOFF);
    }
    return (ret);
}
BOOL	ECATConfigCheck()
{
    BOOL ret = TRUE;

    if (!SetAlarm_NetState())			//网络初始化错误警报
    {
        ret = FALSE;
    }
    else if (!SetAlarm_SlaveState())	//从站状态错误警报
    {
        ret = FALSE;
    }
    else if (!SetAlarm_SlaveNum())		//从站个数不匹配警报
    {
        ret = FALSE;
    }
    else if (!SetAlarm_SlaveLineOff())	//从站断线
    {
        ret = FALSE;
    }

    return (ret);
}

BOOL ECATStateCheck()
{
    BOOL ret = TRUE;

    if (!SetAlarm_NetState())
    {
        ;
    }				//网络初始化错误警报
    else if (!SetAlarm_SlaveState())	{;}		//从站状态错误警报
    else if (!SetAlarm_SlaveLineOff())	{;}		//从站断开警报
    else if (!SetAlarm_SlaveNum()) {;}			//从站个数不匹配警报
    else
    {
        ret = FALSE;
    }

    return ret;
}


