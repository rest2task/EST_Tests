/******************************************************************************
  文 件 名   : motor.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 马达部件
  函数列表   :
              InitMotorPart
              MoterOverLoadChk
              MotorChkLubErr
              MotorIsOn
              MotorOff
              MotorOn
              MotorOnComplete
              MotorProc
              MotorStart
              MotorTemperErr
              SetMotorOnSafeChk
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "actcom.h"
#include "lub.h"
#include "motor.h"
#include "hydr.h"
#include "EtherCATProc.h"
#include "EleCtrlApp.h"
#include "App_driver_func.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern UI16 g_OilPortSelect;
extern CUS_PORT_LIST cus_port_menu[OILPORTTB_NUM];

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
int ecat_master_init(void);
void ServoRecoverActNoEn(void);


/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
MOTOR m_motor;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
const	WORD	MotorStart_SdoData[4][4] =
{
	{0x6060, 0, 9, 8},		//设置驱动器控制模式=9
	{0x6040, 0, 6, 16}, 	//开启使能
	{0x6040, 0, 7, 16},
	{0x6040, 0, 15, 16},
};
const	WORD	MotorOff_SdoData[1][4] =
{{0x6040, 0, 7, 16}};		//关闭使能

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

void InitMotorControlMode()
{
	if (ChkSysCfg(CO1_ECAT))
		m_motor.motor_control_mode = MOTOR_CTRL_ECAT;
	else if (ChkSysCfg(XF6_CANSRV)&&(!ChkSysCfg(CO0_SERVOP)))
		m_motor.motor_control_mode = MOTOR_CTRL_5MODE;
	else
		m_motor.motor_control_mode = MOTOR_CTRL_COMMON;

	//m_motor.motor_control_mode = MOTOR_CTRL_ECAT;//强制写死只能使用Ecat
}


void MotorServoSateChk()
{
	UI16 ServoNo;
	if (!ChkSysCfg(C08_MCHAR) && MotorIsOn2())
	{
		for (ServoNo = 1; ServoNo <= PUMP_ECAT_NUM; ServoNo++)
        {
            if (((!ChkSysCfg(XF1_HTMT))&&(!(ChkServoState(SERVOST_ENABLED)&(0x01<<(ServoNo-1))))) || (ChkSysCfg(XF1_HTMT)&&(ChkServoState(SERVOST_FAULT))&&((g_pumpn[ServoNo-1]->st->SERVO_ERR&0x0FFF) == 0x0e08)))
            {
				break;
            }
        }

		if ((ServoNo - 1) == ECAT_SLV_NUM)
		{
			m_motor.servonenable_starttm = Get10MsTick();
		}
		else if (DIFF_10MS(m_motor.servonenable_starttm) > 2)
		{
			MotorE08ReActDly(0);
			m_motor.servonenable_starttm = Get10MsTick();
		}
	}
	else
	{
		m_motor.servonenable_starttm = Get10MsTick();
	}
}

/*******************************************************************************************
 *Brief			:马达闲置关停功能。当主控电机长时间处于底流速度以下时关闭马达
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20200921
********************************************************************************************/
void MotorFreeTimeChk()
{
	PMOTOR pp = &m_motor;

	//功能关闭
	if (pp->mh->TM_FREETIME_MOTOROFF == 0 || pp->mh->FL_FREETIME_MOTOROFF == 0 || !MotorIsOn() || !ActTskIsEmpty())
	{
		pp->motor_freetime_start = GetSecTick();
		pp->st->TM_COUNTDOWNMOTOROFF = pp->mh->TM_FREETIME_MOTOROFF;
		return;
	}

	pp->st->TM_COUNTDOWNMOTOROFF = pp->mh->TM_FREETIME_MOTOROFF - (((UI16)GetSecTick() - (UI16)pp->motor_freetime_start)/60);

	if (((UI16)GetSecTick() - (UI16)pp->motor_freetime_start)/60 >= pp->mh->TM_FREETIME_MOTOROFF)
	{		
		pp->st->TM_COUNTDOWNMOTOROFF = 0;
		SET_PART_ERR(pp,ER1_MOTORFREETIME);
		MotorDown();
	}
}


void MotorStateSet(WORD state)
{
	PMOTOR pp = &m_motor;
	
	pp->st->FL_MOTORSTATUS = state;
	PartDataTx((PPART)pp, ST_ID, 1, GET_IND(PMOTOR_STATE, FL_MOTORSTATUS), TRUE);
}


BOOL MotorTemperErr()
{
    PMOTOR pp = &m_motor;

	if(pp->fl_motor_temp == FALSE)
	{
		return FALSE;
	}

    if (pp->st->DATEMP_REALMOTOR > pp->mh->MOTORTEMP_ERRORMAX)
    {
        SET_PART_ERR(pp, ER0_MTTMP);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int MotorGetTemperCH()
{
    PMOTOR pp = &m_motor;

    return pp->mh->MOTOR_TEMPCH;
}

BOOL MotorIsOn()
{
    return (m_motor.st->FL_MOTORSTATUS != 0);
}

BOOL MotorIsOn2()
{
    return (m_motor.st->FL_MOTORSTATUS == 1);
}


void MotorOffOnce()
{
    PMOTOR pp = &m_motor;
    WORD starttime,timedelayed = 0;

    if(pp != NULL)
    {
        SetPOOnOff(pp->po->O_MTR_STP,PC_ON);
        
        starttime = Get100MsTick(); 
        while(timedelayed<5)
    	{
        	timedelayed = DIFF_100MS(starttime);	 
        	LoopPOOnOff();
      	}
        
        SetPOOnOff(pp->po->O_MTR_STP,PC_OFFD);
        LoopPOOnOff();
    }
}

ACT_RET MotorOffPDO(PACT pact, UI32 para)
{
    ACT_RET ret = ACT_OK;
	PMOTOR pp = (PMOTOR)(pact->part);
    static WORD ServoNo = 0;


    if (pp == NULL)
    {
        return ACT_ERR_ABORT;
    }


    switch (pact->step)
    {
        case INIT1_STEP:
            ACT_START();
			MotorStateSet(MOTOROFF_STARTINGPROT);
			if (ChkSysCfg(XF1_HTMT))
			{
				ACT_STEP_NEW(STAGE_2_STEP+3);
			}
			else
			{
            	ACT_STEP_NEW(STAGE_2_STEP);
			}
            ServoNo = 0;
            break;
        case STAGE_2_STEP:            	
        	for (ServoNo = 0; ServoNo < PUMP_ECAT_NUM; ServoNo++)
        	{
        		SetServoCtrl(ServoNo,7);
            }

            ACT_STEP_NEW(STAGE_2_STEP + 3);
            break;       
        case STAGE_2_STEP+3: 
			if (ACT_STEP_MS > 100 || !ChkSysCfg(XF6_EFFC))
			{
				if (ChkSysCfg(XF6_EFFC))
				{
					ClrRunFlag(INIF_USEECAT);
				}	
				
	            SetPOBit(pp->po->O_MTR_STR, PC_OFFD);
	            SetPOBit(pp->po->O_MTR_STP, PC_ON);
				SetPOBit(pp->po->O_SERVO_EN, PC_OFF);
	            ACT_STEP_NEW(STAGE_2_STEP + 4);
			}
            break;
        case STAGE_2_STEP+4:
            if (ACT_STEP_MS > 300)
            {
                SetPOBit(pp->po->O_MTR_STP, PC_OFF);                
                ACT_STEP_NEW(END_STEP);
            }
            break;
        case END_STEP:
			ClrRunFlag(INIF_MOTOR);
            MotorStateSet(MOTOR_OFF);
            ret = ACT_END;
            break;
		default:
			ClrRunFlag(INIF_MOTOR);
            MotorStateSet(MOTOR_OFF);
			ret = ACT_END;
			break;
    }

    return ret;
}


ACT_RET MotorOff(PACT pact, UI32 para)
{
    ACT_RET ret = ACT_OK;
	PMOTOR pp = (PMOTOR)(pact->part);
    static WORD ServoNo = 0;


    if (pp == NULL)
    {
        return ACT_ERR_ABORT;
    }


    switch (pact->step)
    {
        case INIT1_STEP:
            ACT_START();
			MotorStateSet(MOTOROFF_STARTINGPROT);
			if (ChkSysCfg(XF1_HTMT) || ChkSysCfg(XF6_EFFC))
			{
				ACT_STEP_NEW(STAGE_2_STEP+3);
			}
			else
			{
            	ACT_STEP_NEW(STAGE_2_STEP);
			}
            ServoNo = 0;
            break;
        case STAGE_2_STEP:
            if (ACT_STEP_MS > 1)
            {
                pp->motorsdoflag = SdoDtDataUpdata(ServoNo, 0x6000, 7, 0x4000, 16);
                if (*(pp->motorsdoflag) != SDO_FULL)
                {
                    ACT_STEP_NEW(STAGE_2_STEP + 1);
                }
            }
            break;
        case STAGE_2_STEP+1:
            if (*(pp->motorsdoflag) == ATTONITY)
            {
                ServoNo++;

                if (ServoNo < GetRealSlaveNum())
                {
                    ACT_STEP_NEW(STAGE_2_STEP);
                }
                else
                {
                    ACT_STEP_NEW(STAGE_2_STEP + 3);
                }
            }
            else if (ACT_STEP_MS > 800)
            {
                SET_PART_ERR(pp, ER0_ESDO2);
                ACT_STEP_NEW(END_STEP);
            }
            break;
        case STAGE_2_STEP+3: 
			if (ChkSysCfg(XF6_EFFC))
			{
				ClrRunFlag(INIF_USEECAT);
			}	
			
            SetPOBit(pp->po->O_MTR_STR, PC_OFFD);
            SetPOBit(pp->po->O_MTR_STP, PC_ON);
			SetPOBit(pp->po->O_SERVO_EN, PC_OFF);
            ACT_STEP_NEW(STAGE_2_STEP + 4);
            break;
        case STAGE_2_STEP+4:
            if (ACT_STEP_MS > 300)
            {
                SetPOBit(pp->po->O_MTR_STP, PC_OFF);                
                ACT_STEP_NEW(STAGE_2_STEP + 5);
            }
            break;
        case STAGE_2_STEP+5:
            MotorStateSet(MOTOR_OFF);
            ACT_STEP_NEW(END_STEP);
            break;
        case END_STEP:
			ClrRunFlag(INIF_MOTOR);
            MotorStateSet(MOTOR_OFF);
            ret = ACT_END;
            break;
		default:
			ClrRunFlag(INIF_MOTOR);
            MotorStateSet(MOTOR_OFF);
			ret = ACT_END;
			break;
    }

    return ret;
}

BOOL MotorChkLubErr()
{
    UI8 	num;
    PLUB	plub;

    if ((plub = (PLUB)GetMachineSubPartList(LUB_ID, &num)) != NULL)
    {
        while (num-- > 0)
        {
            if (LubLevelErr(plub + num))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
void MotorOnComplete(PMOTOR pp)
{
    pp->motor_on_ms = GetMsTick();
    ClrRunFlag(INIF_MOTOR);
}

extern PTBL p_hydr_pri_tbl;

UI16 GetMainPortBit()
{
	PTBL ptbl = p_hydr_pri_tbl;

	UI16 row_len;
	UI16 i;
	UI8* pnow;
	UI8* pend;
	UI16 mainportbit = 0;

	if (GetSys()->md->FL_HYDY_WAY_SET > 0)
	{
		if (GetSys()->md->FL_HYDY_WAY_SET != 2)
		{
			if (g_OilPortSelect > 0)
			{			
				for(i = 0;i < ARRAY_NUM(cus_port_menu); i++)
				{
					if (cus_port_menu[i].part_id == 0xFFFF)
					{
						break;
					}
					mainportbit = mainportbit|(cus_port_menu[i].port);				
				}
			}
		}
		else
		{
			if(GetSysMold()->FL_HYTBLSEL == 0 || (!ChkOperMode(OM_MANUAL) && GetJointActSel() != 0))
			{
				if(ptbl == NULL || ptbl->hd.row_len <= 0)
				return 0x01;

				row_len = ptbl->hd.row_len;
				pnow = ptbl->prow;
				pend = (UI8*)ptbl->prow + row_len*ptbl->hd.row;
				
				/*找到key指定的行*/
				while(pnow < pend)
				{
					mainportbit = mainportbit| ((PHY_PRI_ROW)pnow)->port_bits;
					pnow += row_len;
				}	
			}
		}
	}
	

	if (GetSys()->md->FL_HYDY_WAY_SET == 0)
	{
		if (GetSysMold()->FL_HYTBLSEL == 0 || (!ChkOperMode(OM_MANUAL) && GetJointActSel() != 0))
		{
			if(ptbl == NULL || ptbl->hd.row_len <= 0)
			return 0x01;

			row_len = ptbl->hd.row_len;
			pnow = ptbl->prow;
			pend = (UI8*)ptbl->prow + row_len*ptbl->hd.row;
			
			/*找到key指定的行*/
			while(pnow < pend)
			{
				mainportbit = mainportbit| ((PHY_PRI_ROW)pnow)->port_bits;
				pnow += row_len;
			}	
		}
	}

    mainportbit |= 0x01;
	GetSys()->st->PUMP_MAIN |= mainportbit;
	return GetSys()->st->PUMP_MAIN;
}



ACT_RET MotorEnableOnPDO(PACT pact, UI32 para)
{
    ACT_RET ret = ACT_OK;
    static WORD ServoNo = 0, SdoNum = 0;
    PMOTOR pp = (PMOTOR)(pact->part);

    if (pp == NULL)
    {
        return ACT_ERR_ABORT;
    }

    if (ChkActRun(pp, ACT_SN_BIT(OS_MOTOFF)))
    {
        return ACT_END;
    }
    switch (pact->step)
    {
        case INIT1_STEP:
            if (!ChkActRun(pp, ACT_SN_BIT(OS_MOTON)))
            {
            	MotorStateSet(MOTOR_STARTING);
                
                if (ChkSysCfg(XF1_HTMT))
			        ACT_STEP_NEW(STAGE_5_STEP);
                else
                    ACT_STEP_NEW(INIT2_STEP);
                
                ServoNo = 0;
            }
            break;
        case INIT2_STEP:
         	if ((ChkServoState(SERVOST_FAULT) == 0)&&(!ChkServoState(SERVOST_NOVOL)))
            {
                ServoNo = 0;
                SdoNum = 1;
				pp->enableonnum = 0;

				for (ServoNo = 0; ServoNo < ECAT_SLV_NUM; ServoNo++)
                {
                    if (!(ChkServoState(SERVOST_ENABLED)&(0x01<<ServoNo)))
                    {                		
						break;
                    }
                }

				//如果所有驱动器都已经使能了，则结束任务
				if (ServoNo == ECAT_SLV_NUM)
                	ACT_STEP_NEW(END_STEP);
				else
					ACT_STEP_NEW(STAGE_1_STEP);
            }
			
            if (ACT_STEP_MS > 15000)
            {
                SET_PART_ERR(pp, ER0_MTRS);
                return ACT_ERR_ABORT;
            }
            break;
		case INIT3_STEP:
			 if (ACT_STEP_MS > 20)
			 {
				ACT_STEP_NEW(STAGE_1_STEP);
			 }
			break;
        case STAGE_1_STEP:
            if (ACT_STEP_MS > 5)
            {
            	for (ServoNo = 0; ServoNo < ECAT_SLV_NUM; ServoNo++)
            	{
            		SetServoCtrl(ServoNo,MotorStart_SdoData[SdoNum][2]);
                }

				SdoNum++;
                if (SdoNum >= 4)
                {
                    ACT_STEP_NEW(STAGE_1_STEP + 1);
                }
                else
                {
                    ACT_STEP_NEW(STAGE_1_STEP);
                }

		    }
            break;
    case STAGE_1_STEP + 1:
            for (ServoNo = 0; ServoNo < ECAT_SLV_NUM; ServoNo++)
            {
                if (!(ChkServoState(SERVOST_ENABLED)&(0x01<<ServoNo)))
                {
                    break;
                }
            }

			if (ServoNo == ECAT_SLV_NUM)
            {
            	ACT_STEP_NEW(END_STEP);
			}
			else
			{
			    if (ACT_STEP_MS >= 2000)
                {            
					SdoNum = 1;
					pp->enableonnum++;

					if (pp->enableonnum > 4)
					{
						SET_PART_ERR(pp, ER0_MTRS);
                		return ACT_ERR_ABORT;
            		}
					else
					{
						ACT_STEP_NEW(INIT3_STEP);
					}
                }
			}			
            break;
        case STAGE_5_STEP:
            if (ChkServoErrPro())
            {                    		
                ACT_STEP_NEW(END_STEP);
            }
            else
            {
            	SetPOBit(pp->po->O_SERVO_EN, PC_OFF);	//关闭使能点
				SetPOBit(pp->po->O_MTR_STR, PC_OFF);	//关闭马达开
				SetPOBit(pp->po->O_MTR_STP, PC_ON);		//打开马达停
                ACT_STEP_NEW(STAGE_5_STEP + 1);
            }
            break;
        case STAGE_5_STEP + 1:
            if(ACT_STEP_MS > 200)
            {
            	SetPOBit(pp->po->O_MTR_STP, PC_OFF);	//关闭马达停
                ServoRecoverActNoEn();
                ACT_STEP_NEW(STAGE_5_STEP + 2);
            }
            break;
        case STAGE_5_STEP + 2:
            if(ACT_STEP_MS > 100)
            {
            	SetPOBit(pp->po->O_MTR_STR, PC_ON);				
                SetPOBit(pp->po->O_SERVO_EN, PC_ON);
                ACT_STEP_NEW(END_STEP);
            }
            break;
        case END_STEP:
			MotorStateSet(MOTOR_ON);
            ret = ACT_END;
            break;
    }
    return ret;
}

void MotorEnableOnAct()
{
    PMOTOR pp = &m_motor;

    if(!ChkRunFlag(INIF_USEECAT))
        return;

    if (pp->st->FL_MOTORSTATUS == MOTOR_ON || pp->st->FL_MOTORSTATUS == MOTOR_STARTING)
    {
    	ForkAct(m_motor.act + OS_MOTOENABLEON - 1, SYNC_FORK, NULL, 0x00F, 0);    
    }
}

void MotorE08ReActDly(WORD dly_ms)
{
    PMOTOR pp = &m_motor;

    if(!ChkRunFlag(INIF_USEECAT))
        return;

    if (pp->st->FL_MOTORSTATUS == MOTOR_ON || pp->st->FL_MOTORSTATUS == MOTOR_STARTING)
    {
    	if(!ChkServoSpdOver(50))
    	{
			ForkAct(m_motor.act + OS_MOTOENABLEON - 1, SYNC_FORK, NULL, 0x00F, dly_ms);
		}        
    }
}


ACT_RET MotorOn_EleCtrl_PDO(PMOTOR pp, PACT pact)
{
    ACT_RET ret = ACT_OK;
    static WORD ServoNo = 0;
    static WORD SdoNum = 0;

    if (pp == NULL)
    {
        return ACT_ERR_ABORT;
    }

    switch (pact->step)
    {
        case INIT1_STEP:
            if (pp->SafeFunc != NULL && !(*(pp->SafeFunc))((PPART)pp))
            {
                return ACT_ERR_ABORT;
            }
            else if (OilLvErr())
            {
                return ACT_ERR_ABORT;
            }
            else if (MotorChkLubErr())
            {
                return ACT_ERR_ABORT;
            }
			else if (HydrPrsSenZSetErrChk())
			{
				return ACT_ERR_ABORT;
			}
            else
            {
                SetRunFlag(INIF_MOTOR);
                if (pp->st->FL_MOTORSTATUS != MOTOR_STARTING)
                {
					MotorStateSet(MOTOR_STARTING);

                    ACT_START();
                    ACT_STEP_NEW(INIT1_STEP + 1);
                }
            }
            break;
		case INIT1_STEP + 1:
			SetPOBit(pp->po->O_MTR_STR, PC_ON);
			if (ChkSysCfg(XF6_EFFC)&&CHK_MOTOR_MODE(MOTOR_CTRL_ECAT))
			{				
				ACT_STEP_NEW(INIT1_STEP + 2);
			}
            else
            {
				ACT_STEP_NEW(INIT2_STEP);
			}
            break;
		case INIT1_STEP+2:
			if (ACT_STEP_MS >= 500)
			{
				if(!ecat_master_init()) 				//EtherCAT Init
				{
					SET_PART_ERR(pp, ER0_MTRS);
                    return ACT_ERR_ABORT;
				}
				
				while (get_jiffies() % pdo_cycle_period());
				ecat_master_set(1);	

				ACT_STEP_NEW(INIT1_STEP + 3);
			}
			break;
		case INIT1_STEP+3:
			if (ACT_STEP_MS >= 50)
			{
				if (ECATStateCheck())
				{
					SET_PART_ERR(pp, ER0_MTRS);
                    return ACT_ERR_ABORT;
				}
				else
				{
					SetRunFlag(INIF_USEECAT);
					SetRunFlag(INIF_ETHERCAT);
					m_servoecat.ecat_init_step = 0;
					ACT_STEP_NEW(INIT2_STEP);
				}
			}
			break;

        case INIT2_STEP:	//确认时钟配置是否完成
			if (!ChkRunFlag(INIF_ETHERCAT) || (!ChkRunFlag(INIF_USEECAT)))
        	{
	            ServoNo = 0;
	            pp->enableonnum = 0;
	            ACT_STEP_NEW(INIT3_STEP);
            }

			if (ACT_STEP_MS >= 2000)
			{
				SET_PART_ERR(pp, ER0_MTRS);
                return ACT_ERR_ABORT;
			}
            break;
        case INIT3_STEP:
            if (ACT_STEP_MS > 50)
            {
            	SetPOBit(pp->po->O_MTR_STR, PC_ON);
                SetPOBit(pp->po->O_SERVO_EN, PC_ON);
					
                if (ChkServoErrPro())
                {                    		
                    ACT_STEP_NEW(INIT4_STEP + 1);
                }
                else
                {
                	ServoRecoverActNoEn();
                    ACT_STEP_NEW(INIT4_STEP);
                }
            }
            break;
        case INIT4_STEP:
			if (ChkServoErrPro())
            {
                SetPOBit(pp->po->O_MTR_STR, PC_ON);
                SetPOBit(pp->po->O_SERVO_EN, PC_ON);		
                ACT_STEP_NEW(INIT4_STEP + 1);
            }
            else
            {
				if (ACT_STEP_MS > 2000)
				{
					SET_PART_ERR(pp, ER0_MTRS);
                    return ACT_ERR_ABORT;
                }
            }
            break;
        case INIT4_STEP+1:
            if (ACT_STEP_MS > 1000 || ChkSysCfg(CO4_MOTR))
            {
				if ((ChkServoState(SERVOST_FAULT) == 0)&&(!ChkServoState(SERVOST_NOVOL)))
                {
                    ServoNo = 0;
                    SdoNum = 0;
                    if (!ChkSysCfg(CO4_MOTR))
                    {
                        SetPOBit(pp->po->O_MTR_STR, PC_OFF);
                    }
					if (ChkSysCfg(XF1_HTMT))
					{
						ACT_STEP_NEW(END_STEP+1);
					}
					else
					{
						ACT_STEP_NEW(STAGE_1_STEP);
					}
                }
                if (ACT_STEP_MS > 15000)
                {
                    SET_PART_ERR(pp, ER0_MTRS);
                    return ACT_ERR_ABORT;
                }
            }
            break;
        case STAGE_1_STEP:  //马达启动，设置从站使能
            if (ACT_STEP_MS > 30)
            {
            	SdoNum = 1;
                ACT_STEP_NEW(STAGE_1_STEP + 1);
            }
            break;
        case STAGE_1_STEP+1:
            if (ACT_STEP_MS > 5)
            {
            	for (ServoNo = 0; ServoNo < PUMP_ECAT_NUM; ServoNo++)
            	{
            		SetServoCtrl(ServoNo,MotorStart_SdoData[SdoNum][2]);
                }
				
				SdoNum++;
				
				if (SdoNum < 4)
				{
					ACT_STEP_NEW(STAGE_1_STEP + 1);
				}
				else
				{
					ACT_STEP_NEW(END_STEP);
				}
            }
            break;
        case END_STEP:		//马达使能完毕，检测反馈使能信号
            for (ServoNo = 0; ServoNo < ECAT_SLV_NUM; ServoNo++)
            {
                if (!(ChkServoState(SERVOST_ENABLED)&(0x01<<ServoNo)))
                {
                    break;
                }
            }

			if (ServoNo == ECAT_SLV_NUM)
            {
            	ACT_STEP_NEW(END_STEP + 1);
			}
			else
			{
			    if (ACT_STEP_MS >= 2000)
                {            
					SdoNum = 1;
					pp->enableonnum++;

					if (pp->enableonnum > 4)
					{
						SET_PART_ERR(pp, ER0_MTRS);
                		return ACT_ERR_ABORT;
            		}
					else
					{
						ACT_STEP_NEW(STAGE_1_STEP);
					}
                }
			}
            break;
        case END_STEP+1:		//马达启动完毕 
			MotorStateSet(MOTOR_ON);
			ChkEncoderPPR();	//对编码器系数赋值
			InitHydrData();		//开马达后初始化液压数据
            MotorOnComplete(pp);
			GetMainPortBit();
            ret = ACT_END;
            break;
        case END_STEP+5://马达启动失败
            SET_PART_ERR(pp, ER0_MTRS);
			MotorStateSet(MOTOR_OFF);
            ret = ACT_ERR_ABORT;
            break;
    }
	
    return ret;
}


ACT_RET MotorOn(PMOTOR pp, PACT pact)
{
    ACT_RET ret = ACT_OK;

    if (pp == NULL)
    {
        return ACT_ERR_ABORT;
    }

    switch (pact->step)
    {
        case INIT1_STEP:
            if (pp->SafeFunc != NULL && !(*(pp->SafeFunc))((PPART)pp))
            {
                return ACT_ERR_ABORT;
            }
            else if (OilLvErr())
            {
                return ACT_ERR_ABORT;
            }
            else if (MotorChkLubErr())
            {
                return ACT_ERR_ABORT;
            }
            else if (HydrPrsSenZSetErrChk())
            {
                return ACT_ERR_ABORT;
            }
            else if (ChkSysCfg(CO4_SRAU) && (OilTmpErr() || WaterTmpErr()))
            {
                return ACT_ERR_ABORT;
            }
            else
            {
                SetRunFlag(INIF_MOTOR);
                if (pp->st->FL_MOTORSTATUS != MOTOR_STARTING)
                {
                    SetPOBit(pp->po->O_MTR_STR, PC_ON);
					MotorStateSet(MOTOR_STARTING);
                }

                ACT_START();
                ACT_STEP_NEW(STAGE_1_STEP);
            }
            break;
        case STAGE_1_STEP:
			if(STEP_10MS_CHK(pp->mh->TM_MOTORYSTART))
            {
                SetPOBit(pp->po->O_MTR_STR, PC_OFF);
                ACT_STEP_NEW(STAGE_1_STEP + 1);
            }
            break;
        case STAGE_1_STEP + 1:
			if(STEP_10MS_CHK(pp->mh->TM_MOTORDELTA))
            {
                SetPOBit(pp->po->O_MTR_Y_D, PC_ON);
                ACT_STEP_NEW(STAGE_1_STEP + 2);
            }
            break;
        case STAGE_1_STEP + 2:
            if (ACT_STEP_100MS >= 1)
            {
                SetPOBit(pp->po->O_MTR_STR, PC_ON);
                ACT_STEP_NEW(STAGE_1_STEP + 3);
            }
            break;
        case STAGE_1_STEP  + 3:
            if (ACT_STEP_100MS >= 20)
            {
                SetPOBit(pp->po->O_MTR_Y_D, PC_OFF);
                if (!ChkSysCfg(CO4_MOTR))
                {
                    SetPOBit(pp->po->O_MTR_STR, PC_OFF);
                }
                //SetPOBit(pp->po->O_MTR_STR,PC_OFF);
                ACT_STEP_NEW(STAGE_2_STEP);
            }
            break;
        case STAGE_2_STEP:
            if (pp->mh->FL_MOTORCNT >= 2 &&  pp->mh->FL_MOTORCNT <= 4 && CheckPIUsed(pp->pi->I_MTRCMPT))
            {
                ACT_STEP_NEW(STAGE_3_STEP);
            }
            else if (STEP_10MS_CHK(pp->mh->TM_HOLDMOTORON))
            {
                if (CheckPIUsed(pp->pi->I_MTRCMPT))
                {
                    if (TestPI(pp->pi->I_MTRCMPT))
                    {
						MotorStateSet(MOTOR_ON);
                    }
                    else
                    {
                        SET_PART_ERR(pp, ER0_MTRS);
						MotorDown();
                    }
                }
                else
                {
					MotorStateSet(MOTOR_ON);
                }

                ACT_STEP_NEW(END_STEP);
            }
            break;
        case STAGE_3_STEP:
            if (TestPI(pp->pi->I_MTRCMPT))
            {
                SetPOBit(pp->po->O_MTR_STR2, PC_ON);
                ACT_STEP_NEW(STAGE_3_STEP + 1);
            }
            else if(STEP_10MS_CHK(pp->mh->TM_WARNMOTORON))
            {
                SET_PART_ERR(pp, ER0_MTRS);
                MotorDown();
                ACT_STEP_NEW(END_STEP);
            }
            break;
        case STAGE_3_STEP + 1:
            if(STEP_10MS_CHK(pp->mh->TM_MOTORYSTART))
            {
                ACT_STEP_NEW(STAGE_3_STEP + 2);
            }
            break;
        case STAGE_3_STEP + 2:
            if (TestPI(pp->pi->I_MTRCMPT2))
            {
                SetPOBit(pp->po->O_MTR_STR2, PC_OFF);
                if (pp->mh->FL_MOTORCNT >= 3 && pp->mh->FL_MOTORCNT <= 4 && CheckPIUsed(pp->pi->I_MTRCMPT3))
                {
                    SetPOBit(pp->po->O_MTR_STR3, PC_ON);
                    ACT_STEP_NEW(STAGE_4_STEP);
                }
                else
                {
					MotorStateSet(MOTOR_ON);
                    ACT_STEP_NEW(END_STEP);
                }
            }
            else if(STEP_10MS_CHK(pp->mh->TM_WARNMOTORON))
            {
                SET_PART_ERR(pp,  ER0_MTRS);     // set error flag
                MotorDown();
                ACT_STEP_NEW(END_STEP);
            }
            break;
        case STAGE_4_STEP:
            if(STEP_10MS_CHK(pp->mh->TM_MOTORYSTART))
            {
                ACT_STEP_NEW(STAGE_4_STEP + 1);
            }
            break;
        case STAGE_4_STEP + 1:
            if (TestPI(pp->pi->I_MTRCMPT3))
            {
                SetPOBit(pp->po->O_MTR_STR3, PC_OFF);
                if (pp->mh->FL_MOTORCNT == 4 && CheckPIUsed(pp->pi->I_MTRCMPT4))
                {
                    SetPOBit(pp->po->O_MTR_STR4, PC_ON);
                    ACT_STEP_NEW(STAGE_5_STEP);
                }
                else
                {
					MotorStateSet(MOTOR_ON);
                    ACT_STEP_NEW(END_STEP);
                }
            }
            else if(STEP_10MS_CHK(pp->mh->TM_WARNMOTORON))
            {
                SET_PART_ERR(pp,  ER0_MTRS);     // set error flag
                MotorDown();
                ACT_STEP_NEW(END_STEP);
            }
            break;
        case STAGE_5_STEP:
            if(STEP_10MS_CHK(pp->mh->TM_MOTORYSTART))
            {
                ACT_STEP_NEW(STAGE_5_STEP + 1);
            }
            break;
        case STAGE_5_STEP + 1:
            if (TestPI(pp->pi->I_MTRCMPT4))
            {
                SetPOBit(pp->po->O_MTR_STR4, PC_OFF);
				MotorStateSet(MOTOR_ON);
                ACT_STEP_NEW(END_STEP);
            }
            else if(STEP_10MS_CHK(pp->mh->TM_WARNMOTORON))
            {
                SET_PART_ERR(pp,  ER0_MTRS);     // set error flag
                MotorDown();
                ACT_STEP_NEW(END_STEP);
            }
            break;
        case END_STEP:
            MotorOnComplete(pp);
            ret = ACT_END;
            break;
    }

    return ret;
}

ACT_RET MotorProc(PACT pact, UI32 para)
{
    ACT_RET ret;
    PMOTOR pp = (PMOTOR)(pact->part);

    if (pp != NULL)
    {
        if(CHK_MOTOR_MODE(MOTOR_CTRL_COMMON) && ChkSysCfg(XF1_CHRCP))
        {
            ret = MotorOn(pp, pact);    //普通机模式下采用星三角启动方式
        }
    	else
    	{
			ret = MotorOn_EleCtrl_PDO(pp, pact);
		}

        if (ret != ACT_ERR_ABORT && pact->step  > INIT1_STEP && pp->SafeFunc != NULL)
        {
            if (!(*(pp->SafeFunc))((PPART)pp))
            {
                MotorDown();
                ret = ACT_ERR_ABORT;
            }
        }

        return ret;
    }
    else
    {
        return ACT_ERR_ABORT;
    }
}

BOOL MotorDown()
{
    PMOTOR pp = &m_motor;

    //原来这里是或条件，也就是会总是满足。
    //现在改成与条件，就排除正在关马达和已经关掉马达，不需要再关一次
    if (pp->st->FL_MOTORSTATUS != MOTOR_OFF)
    {
        return ForkAct(m_motor.act + OS_MOTOFF - 1, FIRST_FORK, NULL, 0, 0);
    }
    else
    {
        return FALSE;
    }
}


BOOL MotorStart()
{
    return ForkAct(m_motor.act, FIRST_FORK, NULL, 0, 0);
}

void SetMotorOnSafeChk(SAFECHK_FUNC pfunc)
{
    m_motor.SafeFunc = pfunc;
}

void MoterOverLoadChk()
{
    PMOTOR pp = &m_motor;

	if (MotorIsOn())
	{
		if (CheckPI(pp->pi->I_MTROVL) == PI_SET)
	    {
	        SET_PART_ERR(pp, ER0_MOVL);
	    }
	}   
}


void MachineMotorCheck()
{
    PMOTOR pp = &m_motor;

    if (CheckPIUsed(pp->pi->I_MOTO_OK) || CheckPIUsed(pp->pi->I_MOTO_SRV))
    {
        if (MotorIsOn())
        {
            pp->Delayms2 = GetMsTick();
            if ((UI16)(GetMsTick() - pp->Delayms1) >= 2000)
            {
                if (((CheckPI(pp->pi->I_MOTO_OK) == 0) && !CHK_PART_ERR(pp, ER0_MOTORERR)) ||
                        ((CheckPI(pp->pi->I_MOTO_SRV) == 1) && !CHK_PART_ERR(pp, ER0_MOTORERR)))
                {
					SET_PART_ERR(pp, ER0_MOTORERR);
					StandAlarm(TRUE);
                }
            }
        }
        else
        {
            pp->Delayms1 = GetMsTick();
            if ((UI16)(GetMsTick() - pp->Delayms2) >= 2000)
            {
                if (((CheckPI(pp->pi->I_MOTO_OK) == 1) && !CHK_PART_ERR(pp, ER0_MOTORERR))
                        || ((CheckPI(pp->pi->I_MOTO_SRV) == 0) && !CHK_PART_ERR(pp, ER0_MOTORERR)))
                {
                    pp->MotorCnt++;
                    if (pp->MotorCnt > 500)
                    {
						SET_PART_ERR(pp, ER0_MOTORERR);
						StandAlarm(TRUE);
                    }
                }
                else
                {
                    pp->MotorCnt = 0;
                }
            }
        }
    }
}

BOOL MotorCmd(UI32 cmd, UI32 para)
{
    PMOTOR	pp = &m_motor;
	UI32 ret = TRUE;

	switch(GET_CMD(cmd))
	{
		case CMD_DATA_RX:
		if((GET_CMD_PARA(cmd) & 0xff) == MH_ID)
		{
			if (pp->mh->PUMP_REVERSE_LIMITTM > 2000)
			{
				pp->mh->PUMP_REVERSE_LIMITTM = 2000;
			}
		}		
		break;

		default:
		ret = FALSE;
		break;
	}

	return ret;
}


BOOL InitMotorPart()
{
    PMOTOR	pp = &m_motor;
    static const SUB_INIT_INFO sub_init[] = MOTOR_INIT_INFO;
    static const ACT_INIT_INFO act_init[] = 
	{
		{OS_MOTON, OS_MOTON, ACT_CTRB, MotorProc, MotorDown},
		{OS_MOTOFF, OS_MOTOFF, ACT_CTRB, MotorOffPDO, NULL,NULL,TRUE},
		{OS_MOTOENABLEON, OS_MOTOENABLEON, ACT_CTRB, MotorEnableOnPDO, MotorOffPDO, NULL, TRUE}
	};

    if (!INIT_PART(pp, MOTOR_ID, 1, sub_init))
    {
        return FALSE;
    }

    if (!INIT_ACT(pp, act_init))
    {
        return FALSE;
    }

    pp->po->O_MTR_STP = 41;
    pp->po->O_MTR_STR = 42;
    pp->po->O_MTR_Y_D = 43;
    return TRUE;
}

