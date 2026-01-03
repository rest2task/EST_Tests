/*===========================================================================+
|  Class    : EtherCat process                                               |
|  Task     : EtherCat process                                               |
|----------------------------------------------------------------------------|
|  Compile  : CC'C2000 -                                                     |
|  Link     : CC'C2000 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : zhoutuo                                                   	 |
|  Version  : V0.01                                                          |
|  Creation : 12/27/2016                                                     |
|  Revision : 06-21-2009                                                     |
+===========================================================================*/

#include 	"actcom.h"
#include    "canhead.h"
#include    "part.h"
#include    "hwcfg.h"
#include	"ServoEcat.h"
#include	"EtherCATProc.h"
#include 	"memtbl.h"
#include	"definelist.h"
#include	"EleCtrlApp.h"
#include "AutoRunWithOutServo.h"
#include "App_data.h"


SERVOECAT m_servoecat;
UI16	step_ATTONITY = ATTONITY;
UI16	step_SDO_FULL = SDO_FULL;

extern PPUMP	g_pumpn[MAX_PUMP];
extern UI16 g_pump_num;
extern   MACHINE m_machine;

extern u32 g_sdo_abort_code;
extern u16 g_sdo_abort_servono;
extern u32 rxpdo_send_cnt, rxpdo_rec_cnt,txpdo_send_cnt,txpdo_rec_cnt;

UI8 ServoType[MAX_SERVO_NUM];

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


void ServoDataRxEnd(PPROSERVOECAT pp);
BOOL ServoComPro(void);
BOOL ServoDataENErr(PPROSERVOECAT pp);
UI16* GetCacheWord(UI16* psize);
void FreeCacheWord(void* pcache);


void ServoTypeProc()
{
	UI8 i;
	PMACHINE pm = &m_machine;
	UI8 offset = 0;
	UI8 type;
	
    for (i = 0; i < GetRealSlaveNum(); i++)
    {
    	type = ServoType[i];
		switch(type)
		{
			case TYPE_IS580:
				pm->st->SERVO_TYPE[offset] = TYPE_IS580;
				break;
			case TYPE_IS620:
				pm->st->SERVO_TYPE[offset] = TYPE_IS620;
				break;
			case TYPE_ES750:
				pm->st->SERVO_TYPE[offset] = TYPE_ES750;
				offset++;
				pm->st->SERVO_TYPE[offset] = TYPE_ES750_2;
				break;
		}
		offset++;
	}
}

/*580驱动走ethercat扩展卡自学习*/
BOOL ServoSdoTurning(PPRO_SDO_RX_PKT_HD ppkt, UI16 pkt_id)
{
	UI8 i;
	
	if(ppkt == NULL)
		return FALSE;
	if(!ChkOperMode(OM_MANUAL) || !MotorIsOn2())
		return FALSE;
	
	for (i = 0; i < GetRealSlaveNum(); i++)
	{
		SdoDtDataUpdata(i, 0x2000, 1, 0x2101, 16);//自学习
	}
	return TRUE;
}


BOOL ChkServoErrPro()
{
    int i;
    UI16 err;
    WORD	ret = FALSE;

    for (i = 0; i < ECAT_SLV_NUM; i++)
    {
        err = GetServoErr(i);
        if (err != 0 && ((err&0x0FFF) != 0x0e08 || ChkSysCfg(XF1_HTMT)))
        {
            ret = ret | TRUE;
        }
    }
	
    return !ret;
}


void ChkServoErr(PPUMP p_pump, WORD err)
{
	if (p_pump->servo_err_clear_flag)
	{
		if (DIFF_10MS(p_pump->servo_err_clear_tm)>10)
		{
			p_pump->servo_err_clear_flag = 0;
		}

		return;
	}

	if (ChkSysCfg(XF6_EFFC) && ChkActRun(&m_motor, ACT_SN_BIT(OS_MOTON)))
	{
		return;
	}

	if (p_pump->servo_err_last != err)
	{
		p_pump->st->SERVO_ERR = err;
		
		if((err != 0) && ((err&0x0FFF) != 0x0E08))
		{
			SET_PART_ERR(p_pump,ER0_SERVO_ERR);				
		}
		else
		{
			CLR_PART_ERR(p_pump,ER0_SERVO_ERR);
		}

//		//当出现E08警报时，如果警报恢复，那么5ms后驱动器恢复使能
//		if (((g_pumpn[i]->servo_err_last&0x0FFF) == 0x0e08) && (err == 0))
//		{
//			g_tempcomm[0]->st->DATEMP_REAL[2]++;
//			MotorE08ReActDly(5);
//		}
	}

	p_pump->servo_err_last = err;
}


BOOL ChkEncoderPPR()
{
	UI16 i = 0;
	for (i = 0; i < GetRealSlaveNum(); i++)
    {
		if(i < g_pump_num)
		{
			g_pumpn[i]->EcoderPPR = 0x01<<g_pumpn[i]->mh->ENCODERESOLUTION;
		}
    }
		
		return TRUE;
}

void EtherCatTest(BOOL outre)
{
    PSERVO	p_servo = &g_hydrservo;

	p_servo->mh->READCNT = txpdo_send_cnt-txpdo_rec_cnt;
    p_servo->mh->WRITECNT = rxpdo_send_cnt-rxpdo_rec_cnt;

}


/*******************************************************************
|SDO数据交互
|
|
*******************************************************************/
WORD	SDOBUSYFLAG = 0;

SDOTXBUF        g_sdotxbuf;
SDORXBUF        g_sdorxbuf;
PROSERVOECAT	g_pro_servo;



BOOL ChkRunSdo(WORD line)
{
    if (SDOBUSYFLAG & ((~0x01) << line) > 0)
    {
        return FALSE;
    }
    else
    {
        SDOBUSYFLAG = SDOBUSYFLAG | (0x01 << line);
        return TRUE;
    }
}


//void FreeSdo

void InitStaticSdoData()
{
    int i;
	PSERVOECAT	pp = &m_servoecat;
	 
    for (i = 0; i < GetRealSlaveNum(); i++)
    {
    	if(i < g_pump_num)
    	{
			SdoDataDownload(i, 0x2000, (UI16*)(&(g_pumpn[i]->st->SERVO_TEMP)), 0x0b27, 0, 0);

			if(pp->motortemp_type[i] == 1)
			{
				SdoDataDownload(i, 0x2000, (UI16*)(&(g_pumpn[i]->st->MOTOR_TEMP)), 0x0b87, 0, 0);
			}
		}        
    }
}


/*******************************************************************************************
 *Brief			:初始读写驱动器数据
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190122
********************************************************************************************/
void InitSdoDataRWBef()
{
    UI8 i;
    PSERVOECAT	pp = &m_servoecat;
    if (pp == NULL)
    {
        return;
    }

    for (i = 0; i < GetRealSlaveNum(); i++)
    {
        SdoDtDataDownload(i, 0x2000, &(pp->servo_max_rpm[i]), 0x0015, 0, 0);
        SdoDtDataUpdata(i, 0x2000, 1430, 0x0241, 16);

//		if(i < g_pump_num)
//		{
//			SdoDtData32Download(i, 0x2000,(UI16 *)(&(g_pumpn[i]->EcoderPPR)),((UI16 *)(&(g_pumpn[i]->EcoderPPR)))+1, 0x0031, 0, 0);
//		}
    }
}

BOOL InitSdoRxBuf()
{
    PSDORX_DATAFORM prow = 0;
    prow = AllocMemTbl(MEMTBL_SDO_TBL, sizeof(SDORX_DATAFORM), SDORXBUFFSIZE + SDORXDTBUFFSIZE + 2);
    if (prow != NULL)
    {
        g_sdorxbuf.p_rxbuf_start    = prow;
        g_sdorxbuf.p_rxbuf_end      = (prow + SDORXBUFFSIZE);
        g_sdorxbuf.p_sdopick_rxbuf  = g_sdorxbuf.p_rxbuf_start;
        g_sdorxbuf.p_lclpick_rxbuf  = g_sdorxbuf.p_rxbuf_start;

        g_sdorxbuf.p_rxbuf_start_dt		= (prow + (SDORXBUFFSIZE + 1));
        g_sdorxbuf.p_rxbuf_end_dt		= (prow + (SDORXBUFFSIZE + SDORXDTBUFFSIZE + 1));
        g_sdorxbuf.p_sdopick_rxbuf_dt	= g_sdorxbuf.p_rxbuf_start_dt;
        g_sdorxbuf.p_lclpick_rxbuf_dt	= g_sdorxbuf.p_rxbuf_start_dt;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

BOOL InitSdoTxBuf()
{
    PSDOTX_DATAFORM prow = 0;
    prow = AllocMemTbl(MEMTBL_SDO_TBL, sizeof(SDOTX_DATAFORM), SDOTXBUFFSIZE + SDOTXDTBUFFSIZE + 2);

    if (prow != NULL)
    {
        g_sdotxbuf.p_txbuf_start	= prow;
        g_sdotxbuf.p_txbuf_end		= (prow + SDOTXBUFFSIZE);
        g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
        g_sdotxbuf.p_lclfill_txbuf	= g_sdotxbuf.p_txbuf_start;

        g_sdotxbuf.p_txbuf_start_dt		= (prow + (SDOTXBUFFSIZE + 1));
        g_sdotxbuf.p_txbuf_end_dt		= (prow + (SDOTXBUFFSIZE + SDOTXDTBUFFSIZE + 1));
        g_sdotxbuf.p_sdofill_txbuf_dt	= g_sdotxbuf.p_txbuf_start_dt;
        g_sdotxbuf.p_lclfill_txbuf_dt	= g_sdotxbuf.p_txbuf_start_dt;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}


UI16* SdoDataUpdata(UI8 ServoNo, UI16 function, UI16 lcldata, UI16 extadresshead)
{
    UI16* step = 0;

    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }

    if ((g_sdotxbuf.p_lclfill_txbuf == g_sdotxbuf.p_sdofill_txbuf) && ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step != ATTONITY	)
    {
        db_printf("Sdotxrbl full");
        return &(step_SDO_FULL);
    }

    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->function = function;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->lcldata = lcldata;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->extadresshead_tx = extadresshead;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->adresslen_tx = 16;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->servono = ServoNo;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step = SDO_ARRANGE;

    step = &(((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step);

    if (g_sdotxbuf.p_lclfill_txbuf == g_sdotxbuf.p_txbuf_end)
    {
        g_sdotxbuf.p_lclfill_txbuf = g_sdotxbuf.p_txbuf_start;
    }
    else
    {
        g_sdotxbuf.p_lclfill_txbuf++;
    }

    if (g_sdotxbuf.p_lclfill_txbuf == g_sdotxbuf.p_sdofill_txbuf && ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step != ATTONITY)
    {
//        ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step = SDO_FULL;
        db_printf("Sdotxrbl full");
    }

    return step;
}

UI16* SdoData32Updata(UI8 ServoNo, UI16 function, UI32 lcldata, UI16* lcladressheadh, UI16 extadresshead)
{
    UI16* step = 0;
    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }

    if ((((g_sdotxbuf.p_lclfill_txbuf + 1) == g_sdotxbuf.p_sdofill_txbuf) || (g_sdotxbuf.p_lclfill_txbuf == g_sdotxbuf.p_sdofill_txbuf))
            && (((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step != ATTONITY || ((PSDOTX_DATAFORM)(g_sdotxbuf.p_lclfill_txbuf + 1))->step != ATTONITY))
    {
        db_printf("Sdotxrbl full");
        return &(step_SDO_FULL);
    }

    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->function = function;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->lcldata = lcldata & 0xFFFF;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->extadresshead_tx = extadresshead;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->adresslen_tx = 32;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->servono = ServoNo;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step = SDO_ARRANGE;

    if (g_sdotxbuf.p_lclfill_txbuf == g_sdotxbuf.p_txbuf_end)
    {
        g_sdotxbuf.p_lclfill_txbuf = g_sdotxbuf.p_txbuf_start;
    }
    else
    {
        g_sdotxbuf.p_lclfill_txbuf++;
    }

    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->function = function;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->lcldata = (lcldata >> 16) & 0xFFFF;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->extadresshead_tx = extadresshead;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->adresslen_tx = 32;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->servono = ServoNo;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step = SDO_ARRANGE;

    step = &(((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step);

    if (g_sdotxbuf.p_lclfill_txbuf == g_sdotxbuf.p_txbuf_end)
    {
        g_sdotxbuf.p_lclfill_txbuf = g_sdotxbuf.p_txbuf_start;
    }
    else
    {
        g_sdotxbuf.p_lclfill_txbuf++;
    }

    if ((((g_sdotxbuf.p_lclfill_txbuf + 1) == g_sdotxbuf.p_sdofill_txbuf) || (g_sdotxbuf.p_lclfill_txbuf == g_sdotxbuf.p_sdofill_txbuf))
            && (((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step != ATTONITY || ((PSDOTX_DATAFORM)(g_sdotxbuf.p_lclfill_txbuf + 1))->step != ATTONITY))
    {
//        ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf)->step = SDO_FULL;
        db_printf("Sdotxrbl full");
    }
    return step;
}


UI16* SdoDataDownload(UI8 ServoNo, UI16 function, UI16* lcladresshead, UI16 extadresshead, UI16 value, UI8 sign)
{
    UI16* step = 0;
    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }

    if (g_sdorxbuf.p_lclpick_rxbuf == g_sdorxbuf.p_sdopick_rxbuf && ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step != ATTONITY)
    {
        db_printf("Sdorxrbl full");
        return &(step_SDO_FULL);
    }

    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->function = function;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->lcladresshead_rx_p = lcladresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->extadresshead_rx = extadresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->adresslen_rx = 16;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->servono = ServoNo;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step = SDO_ARRANGE;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->value = value;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->sign = sign;

    step = &(((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step);

    if (g_sdorxbuf.p_lclpick_rxbuf == g_sdorxbuf.p_rxbuf_end)
    {
        g_sdorxbuf.p_lclpick_rxbuf = g_sdorxbuf.p_rxbuf_start;
    }
    else
    {
        g_sdorxbuf.p_lclpick_rxbuf++;
    }

    if (g_sdorxbuf.p_lclpick_rxbuf == g_sdorxbuf.p_sdopick_rxbuf && ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step != ATTONITY)
    {
//        ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step = SDO_FULL;
        db_printf("Sdorxrbl full");
    }
    return step;
}


UI16* SdoData32Download(UI8 ServoNo, UI16 function, UI16* lcladressheadl, UI16* lcladressheadh, UI16 extadresshead, UI16 value, UI8 sign)
{
    UI16* step = 0;
    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }

    if (((g_sdorxbuf.p_lclpick_rxbuf == g_sdorxbuf.p_sdopick_rxbuf) || ((g_sdorxbuf.p_lclpick_rxbuf + 1) == g_sdorxbuf.p_sdopick_rxbuf))
            && (((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step != ATTONITY || ((PSDORX_DATAFORM)(g_sdorxbuf.p_lclpick_rxbuf + 1))->step != ATTONITY))
    {
        db_printf("Sdorxrbl full");
        return &(step_SDO_FULL);
    }

    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->function = function;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->lcladresshead_rx_p = lcladressheadl;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->extadresshead_rx = extadresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->adresslen_rx = 32;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->servono = ServoNo;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step = SDO_ARRANGE;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->value = value;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->sign = sign;

    if (g_sdorxbuf.p_lclpick_rxbuf == g_sdorxbuf.p_rxbuf_end)
    {
        g_sdorxbuf.p_lclpick_rxbuf = g_sdorxbuf.p_rxbuf_start;
    }
    else
    {
        g_sdorxbuf.p_lclpick_rxbuf++;
    }

    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->function = function;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->lcladresshead_rx_p = lcladressheadh;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->extadresshead_rx = extadresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->adresslen_rx = 32;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->servono = ServoNo;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step = SDO_ARRANGE;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->value = value;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->sign = sign;

    step = &(((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step);

    if (g_sdorxbuf.p_lclpick_rxbuf == g_sdorxbuf.p_rxbuf_end)
    {
        g_sdorxbuf.p_lclpick_rxbuf = g_sdorxbuf.p_rxbuf_start;
    }
    else
    {
        g_sdorxbuf.p_lclpick_rxbuf++;
    }

    if (((g_sdorxbuf.p_lclpick_rxbuf == g_sdorxbuf.p_sdopick_rxbuf) || ((g_sdorxbuf.p_lclpick_rxbuf + 1) == g_sdorxbuf.p_sdopick_rxbuf))
            && (((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step != ATTONITY || ((PSDORX_DATAFORM)(g_sdorxbuf.p_lclpick_rxbuf + 1))->step != ATTONITY))
    {
//        ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf)->step = SDO_FULL;
        db_printf("Sdorxrbl full");
    }
    return step;
}



COM_SDO_RET SdoRxData()
{
    COM_SDO_RET ret = RX_SDO_FREE;
//	static WORD tm_reading;
    static DWORD cn_reading = 0;
    //static UI8* flag = 0;
    WORD ServoNo;
    WORD servo_subindex;
    WORD servo_index;
    WORD Index;
    WORD servo_lenth;

    if (g_sdorxbuf.p_sdopick_rxbuf == NULL)
    {
        return ret;
    }

    switch (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step)
    {
        case SDO_ARRANGE:
            cn_reading = 0;
            if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->servono < GetRealSlaveNum())
            {

                ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_SENDING;
            }
            else
            {
                ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_END;
                break;
            }
        case SDO_SENDING:
            ServoNo = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->servono;
            Index = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->extadresshead_rx;

            if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->function == 0x6000)
            {
                servo_index = 0x6000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
            }
			else if(((PSDOTX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->function == 0x6800)
			{
                servo_index = 0x6800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
			}
			else if(((PSDOTX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->function == 0x2800)
			{
                servo_index = 0x2800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
			}
            else
            {
                servo_index = 0x2000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
            }

            servo_lenth = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->adresslen_rx;

            SetEcatSdoRecv(ServoNo, servo_index, servo_subindex, servo_lenth);

            ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_WAITASN;

            ret = RX_SDO_BUSY;
            break;
        case SDO_WAITASN:
            ServoNo = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->servono;
            if (EcatSdoRecvChk(ServoNo))
            {
                UI32 backvalue;
                UI16 value = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->value;
                UI16 sign = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->sign;
                if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->adresslen_rx == 32)
                {

                    backvalue = ((UI32)GetSdoRecv(ServoNo, 6) & 0xFFFF) + (((UI32)GetSdoRecv(ServoNo, 7) & 0xFFFF) << 16);

                    if (value > 0)
                    {
                        switch (sign)
                        {
                            case 0:
                                backvalue = backvalue + value;
                                break;
                            case 1:
                                backvalue = backvalue - value;
                                break;
                            case 2:
                                backvalue = backvalue * value;
                                break;
                            case 3:
                                backvalue = backvalue / value;
                                break;
                        }
                    }

                    *(((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->lcladresshead_rx_p) = (UI16)(backvalue & 0xFFFF);
                    ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = ATTONITY;


                    if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_rxbuf_end)
                    {
                        g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_start;
                    }
                    else
                    {
                        g_sdorxbuf.p_sdopick_rxbuf++;
                    }

                    *(((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->lcladresshead_rx_p) = (UI16)((backvalue >> 16) & 0xFFFF);
                }
                else
                {
                    backvalue = GetSdoRecv(ServoNo, 6);

                    if (value > 0)
                    {
                        switch (sign)
                        {
                            case 0:
                                backvalue = backvalue + value;
                                break;
                            case 1:
                                backvalue = backvalue - value;
                                break;
                            case 2:
                                backvalue = backvalue * value;
                                break;
                            case 3:
                                backvalue = backvalue / value;
                                break;
                        }
                    }
                    *(((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->lcladresshead_rx_p) = (UI16)backvalue;
                }

                ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_END;
            }
            else// if (abs(GetMsTick() - tm_reading)> 50)
            {
                cn_reading++;

                if (cn_reading > 1000) //原来是5000，警报时间太久
                {
                    ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_ERR;
                }
                else if ((cn_reading % 100) == 99)
                {
                    if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->adresslen_rx == 32)
                    {
                        ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_SENDING;
                        if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_rxbuf_start)
                        {
                            g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_end;
                        }
                        else
                        {
                            g_sdorxbuf.p_sdopick_rxbuf--;
                        }
                        ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_SENDING;
                    }
                    else
                    {
                        ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_SENDING;
                    }
                }
            }
            ret = RX_SDO_STAND;
            break;
        case SDO_END:
            ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_ARRANGE;

            if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_lclpick_rxbuf)
            {
                g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_start;
            }
            else
            {
                if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_rxbuf_end)
                {
                    g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_start;
                }
                else
                {
                    g_sdorxbuf.p_sdopick_rxbuf++;
                }
            }
            ret = RX_SDO_END;
            break;
        case SDO_ERR:
            ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf)->step = SDO_ARRANGE;
            if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_lclpick_rxbuf)
            {
                g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_start;
            }
            else
            {
                if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_rxbuf_end)
                {
                    g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_start;
                }
                else
                {
                    g_sdorxbuf.p_sdopick_rxbuf++;
                }
            }
            ret = RX_SDO_ERR;
            break;
        default:
            if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_lclpick_rxbuf)
            {
                g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_start;
            }
            else
            {
                if (g_sdorxbuf.p_sdopick_rxbuf == g_sdorxbuf.p_rxbuf_end)
                {
                    g_sdorxbuf.p_sdopick_rxbuf = g_sdorxbuf.p_rxbuf_start;
                }
                else
                {
                    g_sdorxbuf.p_sdopick_rxbuf++;
                }
            }
            ret = RX_SDO_END;
            break;
    }
    return ret;
}

COM_SDO_RET SdoTxData()
{

    COM_SDO_RET ret = TX_SDO_FREE;
    WORD ServoNo;
    WORD servo_subindex;
    WORD servo_index;
    WORD Index;
    WORD servo_lenth;
    UI32 servo_data;

    static DWORD cn_sending;

    switch (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step)
    {
        case SDO_ARRANGE:
            cn_sending = 0;
            if (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->servono < GetRealSlaveNum())
            {
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_SENDING;
            }
            else
            {
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_END;
                break;
            }
        case SDO_SENDING:

            ServoNo = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->servono;
            Index = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->extadresshead_tx;

            if (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->function == 0x6000)
            {
                servo_index = 0x6000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
            }
			else if(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->function == 0x6800)
			{
                servo_index = 0x6800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
			}
			else if(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->function == 0x2800)
			{
                servo_index = 0x2800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
			}
            else
            {
                servo_index = 0x2000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
            }

            servo_lenth = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->adresslen_tx;

            if (servo_lenth == 32)
            {
                servo_data = (UI32)(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->lcldata);
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = ATTONITY;

                if (g_sdotxbuf.p_sdofill_txbuf == g_sdotxbuf.p_txbuf_end)
                {
                    g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
                }
                else
                {
                    g_sdotxbuf.p_sdofill_txbuf++;
                }

                servo_data = (servo_data & 0xFFFF) | ((UI32)(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->lcldata) << 16);
            }
            else
            {
                servo_data = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->lcldata;
            }

            SetEcatSdoSend(ServoNo, servo_index, servo_subindex, servo_data, servo_lenth);

            ret = TX_SDO_BUSY;
            break;
        case SDO_WAITASN:
            ServoNo = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->servono;
            if (EcatSdoChk(ServoNo))
            {
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_END;
            }
            else
            {
                cn_sending++;
                if (cn_sending > 1000) //原来是5000，警报时间太久
                {
                    ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_ERR;
                }
                else if (cn_sending % 100 == 99)
                {
                    if (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->adresslen_tx == 32)
                    {
                        ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_SENDING;

                        if (g_sdotxbuf.p_sdofill_txbuf == g_sdotxbuf.p_txbuf_start)
                        {
                            g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_end;
                        }
                        else
                        {
                            g_sdotxbuf.p_sdofill_txbuf--;
                        }
                    }

                    ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_SENDING;
                }
            }
            ret = TX_SDO_STAND;
            break;
        case SDO_END:
            ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_ARRANGE;

            if (g_sdotxbuf.p_sdofill_txbuf + 1 == g_sdotxbuf.p_lclfill_txbuf)
            {
                g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
            }
            else
            {
                if (g_sdotxbuf.p_sdofill_txbuf == g_sdotxbuf.p_txbuf_end)
                {
                    g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
                }
                else
                {
                    g_sdotxbuf.p_sdofill_txbuf++;
                }
            }

            ret = TX_SDO_END;
            break;
        case SDO_ERR:
            ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf)->step = SDO_ARRANGE;

            if (g_sdotxbuf.p_sdofill_txbuf + 1 == g_sdotxbuf.p_lclfill_txbuf)
            {
                g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
            }
            else
            {
                if (g_sdotxbuf.p_sdofill_txbuf == g_sdotxbuf.p_txbuf_end)
                {
                    g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
                }
                else
                {
                    g_sdotxbuf.p_sdofill_txbuf++;
                }
            }

            ret = TX_SDO_ERR;
            break;
        default:
            if (g_sdotxbuf.p_sdofill_txbuf == g_sdotxbuf.p_lclfill_txbuf)
            {
                g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
            }
            else
            {
                if (g_sdotxbuf.p_sdofill_txbuf == g_sdotxbuf.p_txbuf_end)
                {
                    g_sdotxbuf.p_sdofill_txbuf = g_sdotxbuf.p_txbuf_start;
                }
                else
                {
                    g_sdotxbuf.p_sdofill_txbuf++;
                }
            }

            ret = TX_SDO_END;
            break;
    }
    return ret;
}


UI16* SdoDtDataUpdata(UI8 ServoNo, UI16 function, UI16 lcldata, UI16 extadresshead, UI8 DataLenth)
{
    UI16* step = 0;
    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }
    if ((g_sdotxbuf.p_lclfill_txbuf_dt == g_sdotxbuf.p_sdofill_txbuf_dt) && (((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step != ATTONITY))
    {
        db_printf("Sdotxrbl full");
        return &(step_SDO_FULL);
    }

    if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_lclfill_txbuf_dt)
    {
        g_sdotxbuf.p_sdofill_txbuf_dt	= g_sdotxbuf.p_txbuf_start_dt;
        g_sdotxbuf.p_lclfill_txbuf_dt	= g_sdotxbuf.p_txbuf_start_dt;
    }

    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->function = function;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->lcldata = lcldata;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->extadresshead_tx = extadresshead;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->adresslen_tx = DataLenth;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->servono = ServoNo;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step = SDO_ARRANGE;

    step = &(((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step);

    if (g_sdotxbuf.p_lclfill_txbuf_dt == g_sdotxbuf.p_txbuf_end_dt)
    {
        g_sdotxbuf.p_lclfill_txbuf_dt = g_sdotxbuf.p_txbuf_start_dt;
    }
    else
    {
        g_sdotxbuf.p_lclfill_txbuf_dt++;
    }

    if ((g_sdotxbuf.p_lclfill_txbuf_dt == g_sdotxbuf.p_sdofill_txbuf_dt) && (((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step != ATTONITY))
    {
//        ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step = SDO_FULL;
        db_printf("Sdotxrbl full");
    }

    return step;
}

UI16* SdoDtData32Updata(UI8 ServoNo, UI16 function, UI32 lcldata, UI16 extadresshead)
{
    UI16* step = 0;
    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }
    if ((((g_sdotxbuf.p_lclfill_txbuf_dt + 1) == g_sdotxbuf.p_sdofill_txbuf_dt) || (g_sdotxbuf.p_lclfill_txbuf_dt == g_sdotxbuf.p_sdofill_txbuf_dt))
            && (((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step != ATTONITY || ((PSDOTX_DATAFORM)(g_sdotxbuf.p_lclfill_txbuf_dt + 1))->step != ATTONITY))
    {
        db_printf("Sdotxrbl full");
        return &(step_SDO_FULL);
    }

    if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_lclfill_txbuf_dt)
    {
        g_sdotxbuf.p_sdofill_txbuf_dt	= g_sdotxbuf.p_txbuf_start_dt;
        g_sdotxbuf.p_lclfill_txbuf_dt	= g_sdotxbuf.p_txbuf_start_dt;
    }

    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->function = function;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->lcldata = lcldata & 0xFFFF;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->extadresshead_tx = extadresshead;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->adresslen_tx = 32;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->servono = ServoNo;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step = SDO_ARRANGE;

    if (g_sdotxbuf.p_lclfill_txbuf_dt == g_sdotxbuf.p_txbuf_end_dt)
    {
        g_sdotxbuf.p_lclfill_txbuf_dt = g_sdotxbuf.p_txbuf_start_dt;
    }
    else
    {
        g_sdotxbuf.p_lclfill_txbuf_dt++;
    }

    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->function = function;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->lcldata = (lcldata >> 16) & 0xFFFF;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->extadresshead_tx = extadresshead;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->adresslen_tx = 32;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->servono = ServoNo;
    ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step = SDO_ARRANGE;

    step = &(((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step);
    if (g_sdotxbuf.p_lclfill_txbuf_dt == g_sdotxbuf.p_txbuf_end_dt)
    {
        g_sdotxbuf.p_lclfill_txbuf_dt = g_sdotxbuf.p_txbuf_start_dt;
    }
    else
    {
        g_sdotxbuf.p_lclfill_txbuf_dt++;
    }

    if ((((g_sdotxbuf.p_lclfill_txbuf_dt + 1) == g_sdotxbuf.p_sdofill_txbuf_dt) || (g_sdotxbuf.p_lclfill_txbuf_dt == g_sdotxbuf.p_sdofill_txbuf_dt))
            && (((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step != ATTONITY || ((PSDOTX_DATAFORM)(g_sdotxbuf.p_lclfill_txbuf_dt + 1))->step != ATTONITY))
    {
//        ((PSDOTX_DATAFORM)g_sdotxbuf.p_lclfill_txbuf_dt)->step = SDO_FULL;
        db_printf("Sdotxrbl full");
    }

    return step;
}


UI16* SdoDtDataDownload(UI8 ServoNo, UI16 function, UI16* lcladresshead, UI16 extadresshead, UI16 value, UI8 sign)
{
    UI16* step;
    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }

    if ((g_sdorxbuf.p_lclpick_rxbuf_dt == g_sdorxbuf.p_sdopick_rxbuf_dt) && (((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step != ATTONITY))
    {
        db_printf("Sdorxrbl full");
        return &(step_SDO_FULL);
    }

    if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_lclpick_rxbuf_dt)
    {
        g_sdorxbuf.p_sdopick_rxbuf_dt	= g_sdorxbuf.p_rxbuf_start_dt;
        g_sdorxbuf.p_lclpick_rxbuf_dt	= g_sdorxbuf.p_rxbuf_start_dt;
    }

    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->function = function;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->lcladresshead_rx_p = lcladresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->extadresshead_rx = extadresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->adresslen_rx = 16;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->servono = ServoNo;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step = SDO_ARRANGE;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->value = value;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->sign = sign;

    step = &(((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step);

    if (g_sdorxbuf.p_lclpick_rxbuf_dt == g_sdorxbuf.p_rxbuf_end_dt)
    {
        g_sdorxbuf.p_lclpick_rxbuf_dt = g_sdorxbuf.p_rxbuf_start_dt;
    }
    else
    {
        g_sdorxbuf.p_lclpick_rxbuf_dt++;
    }

    if ((g_sdorxbuf.p_lclpick_rxbuf_dt == g_sdorxbuf.p_sdopick_rxbuf_dt) && (((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step != ATTONITY))
    {
//        ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step = SDO_FULL;
        db_printf("Sdorxrbl full");
    }
    return step;
}


UI16* SdoDtData32Download(UI8 ServoNo, UI16 function, UI16* lcladressheadl, UI16* lcladressheadh, UI16 extadresshead, UI16 value, UI8 sign)
{
    UI16* step;
    if (ChkRunFlag(INIF_USEECAT) == 0)
    {
        return &(step_ATTONITY);
    }

    if (((g_sdorxbuf.p_lclpick_rxbuf_dt == g_sdorxbuf.p_sdopick_rxbuf_dt) || ((g_sdorxbuf.p_lclpick_rxbuf_dt + 1) == g_sdorxbuf.p_sdopick_rxbuf_dt))
            && ((((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step != ATTONITY) || (((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step != ATTONITY)))
    {
        db_printf("Sdorxrbl full");
        return &(step_SDO_FULL);
    }

    if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_lclpick_rxbuf_dt)
    {
        g_sdorxbuf.p_sdopick_rxbuf_dt	= g_sdorxbuf.p_rxbuf_start_dt;
        g_sdorxbuf.p_lclpick_rxbuf_dt	= g_sdorxbuf.p_rxbuf_start_dt;
    }

    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->function = function;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->lcladresshead_rx_p = lcladressheadl;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->extadresshead_rx = extadresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->adresslen_rx = 32;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->servono = ServoNo;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step = SDO_ARRANGE;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->value = value;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->sign = sign;


    if (g_sdorxbuf.p_lclpick_rxbuf_dt == g_sdorxbuf.p_rxbuf_end_dt)
    {
        g_sdorxbuf.p_lclpick_rxbuf_dt = g_sdorxbuf.p_rxbuf_start_dt;
    }
    else
    {
        g_sdorxbuf.p_lclpick_rxbuf_dt++;
    }

    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->function = function;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->lcladresshead_rx_p = lcladressheadh;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->extadresshead_rx = extadresshead;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->adresslen_rx = 32;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->servono = ServoNo;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step = SDO_ARRANGE;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->value = value;
    ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->sign = sign;

    step = &(((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step);
    if (g_sdorxbuf.p_lclpick_rxbuf_dt == g_sdorxbuf.p_rxbuf_end_dt)
    {
        g_sdorxbuf.p_lclpick_rxbuf_dt = g_sdorxbuf.p_rxbuf_start_dt;
    }
    else
    {
        g_sdorxbuf.p_lclpick_rxbuf_dt++;
    }

    if (((g_sdorxbuf.p_lclpick_rxbuf_dt == g_sdorxbuf.p_sdopick_rxbuf_dt) || ((g_sdorxbuf.p_lclpick_rxbuf_dt + 1) == g_sdorxbuf.p_sdopick_rxbuf_dt))
            && ((((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step != ATTONITY) || (((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step != ATTONITY)))
    {
//        ((PSDORX_DATAFORM)g_sdorxbuf.p_lclpick_rxbuf_dt)->step = SDO_FULL;
        db_printf("Sdorxrbl full");
    }
    return step;
}


COM_SDO_RET SdoDtRxData()
{
    COM_SDO_RET ret = RX_SDO_FREE;
    WORD ServoNo;
    WORD servo_subindex;
    WORD servo_index;
    WORD Index;
    WORD servo_lenth;
    static WORD cn_reading = 0;

    if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_lclpick_rxbuf_dt && ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step == ATTONITY)
    {
        return ret;
    }

    switch (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step)
    {
        case SDO_ARRANGE:
            cn_reading = 0;

            if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->servono < GetRealSlaveNum())
            {
                ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = SDO_SENDING;
            }
            else
            {
                ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = SDO_END;
                break;
            }
        case SDO_SENDING:

            ServoNo = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->servono;
            Index = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->extadresshead_rx;

            if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->function == 0x6000)
            {
                servo_index = 0x6000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
            }
			else if(((PSDOTX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->function == 0x6800)
			{
                servo_index = 0x6800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
			}
			else if(((PSDOTX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->function == 0x2800)
			{
                servo_index = 0x2800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
			}
            else
            {
                servo_index = 0x2000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
            }

            servo_lenth = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->adresslen_rx;

            SetEcatSdoRecv(ServoNo, servo_index, servo_subindex, servo_lenth);

            ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = SDO_WAITASN;
            ret = RX_SDO_BUSY;
            break;
        case SDO_WAITASN:
            ServoNo = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->servono;

            if (EcatSdoRecvChk(ServoNo))
            {
                UI32 backvalue;
                UI16 value = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->value;
                UI16 sign = ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->sign;
                if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->adresslen_rx == 32)
                {
                    backvalue = ((UI32)GetSdoRecv(ServoNo, 6) & 0xFFFF) + (((UI32)GetSdoRecv(ServoNo, 7) & 0xFFFF) << 16);

                    if (value > 0)
                    {
                        switch (sign)
                        {
                            case 0:
                                backvalue = backvalue + value;
                                break;
                            case 1:
                                backvalue = backvalue - value;
                                break;
                            case 2:
                                backvalue = backvalue * value;
                                break;
                            case 3:
                                backvalue = backvalue / value;
                                break;
                        }
                    }

                    *(((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->lcladresshead_rx_p) = (UI16)(backvalue & 0xFFFF);
                    ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = ATTONITY;

                    if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_rxbuf_end_dt)
                    {
                        g_sdorxbuf.p_sdopick_rxbuf_dt = g_sdorxbuf.p_rxbuf_start_dt;
                    }
                    else
                    {
                        g_sdorxbuf.p_sdopick_rxbuf_dt++;
                    }

                    *(((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->lcladresshead_rx_p) = (UI16)((backvalue >> 16) & 0xFFFF);
                }
                else
                {
                    backvalue = GetSdoRecv(ServoNo, 6);

                    if (value > 0)
                    {
                        switch (sign)
                        {
                            case 0:
                                backvalue = backvalue + value;
                                break;
                            case 1:
                                backvalue = backvalue - value;
                                break;
                            case 2:
                                backvalue = backvalue * value;
                                break;
                            case 3:
                                backvalue = backvalue / value;
                                break;
                        }
                    }
                    *(((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->lcladresshead_rx_p) = (UI16)backvalue;
                }

                ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = SDO_END;
            }
            else
            {
                cn_reading++;
                if (cn_reading > 1000) //原来是5000，警报时间太久
                {
                    ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = SDO_ERR;
                }
                else if (cn_reading % 100 == 99)
                {
                    if (((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->adresslen_rx == 32)
                    {
                        ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = SDO_SENDING;


                        if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_rxbuf_start_dt)
                        {
                            g_sdorxbuf.p_sdopick_rxbuf_dt = g_sdorxbuf.p_rxbuf_end_dt;
                        }
                        else
                        {
                            g_sdorxbuf.p_sdopick_rxbuf_dt--;
                        }
                    }

                    ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = SDO_SENDING;
                }
            }

            ret = RX_SDO_STAND;
            break;
        case SDO_END:
            ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = ATTONITY;

            if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_rxbuf_end_dt)
            {
                g_sdorxbuf.p_sdopick_rxbuf_dt = g_sdorxbuf.p_rxbuf_start_dt;
            }
            else
            {
                g_sdorxbuf.p_sdopick_rxbuf_dt++;
            }

            ret = RX_SDO_END;
            break;
        case SDO_ERR:
            if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_rxbuf_end_dt)
            {
                g_sdorxbuf.p_sdopick_rxbuf_dt = g_sdorxbuf.p_rxbuf_start_dt;
            }
            else
            {
                g_sdorxbuf.p_sdopick_rxbuf_dt++;
            }
            ret = RX_SDO_ERR;
            break;
        default:
            ((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step = ATTONITY;

            if (g_sdorxbuf.p_sdopick_rxbuf_dt == g_sdorxbuf.p_rxbuf_end_dt)
            {
                g_sdorxbuf.p_sdopick_rxbuf_dt = g_sdorxbuf.p_rxbuf_start_dt;
            }
            else
            {
                g_sdorxbuf.p_sdopick_rxbuf_dt++;
            }

            ret = RX_SDO_END;
            break;
    }
    return ret;
}

COM_SDO_RET SdoDtTxData()
{

    COM_SDO_RET ret = TX_SDO_FREE;
    WORD ServoNo;
    WORD servo_subindex;
    WORD servo_index;
    WORD Index;
    WORD servo_lenth;
    UI32 servo_data;
    static WORD cn_sending = 0;

    if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_lclfill_txbuf_dt && ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step == ATTONITY)
    {
        return ret;
    }

    switch (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step)
    {
        case SDO_ARRANGE:
            cn_sending = 0;
            if (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->servono < GetRealSlaveNum())
            {
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = SDO_SENDING;
            }
            else
            {
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = SDO_END;
                break;
            }
        case SDO_SENDING:

            ServoNo = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->servono;
            Index = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->extadresshead_tx;

            if (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->function == 0x6000)
            {
                servo_index = 0x6000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
            }
			else if(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->function == 0x6800)
			{
                servo_index = 0x6800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16;
			}
			else if(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->function == 0x2800)
			{
                servo_index = 0x2800 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
			}
            else
            {
                servo_index = 0x2000 | Index >> 8;
                servo_subindex = (((0xFF & Index) / 16) * 10) + (0xFF & Index) % 16 + 1;
            }

            servo_lenth = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->adresslen_tx;

            if (servo_lenth == 32)
            {
                servo_data = (UI32)(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->lcldata);
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = ATTONITY;

                if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_txbuf_end_dt)
                {
                    g_sdotxbuf.p_sdofill_txbuf_dt = g_sdotxbuf.p_txbuf_start_dt;
                }
                else
                {
                    g_sdotxbuf.p_sdofill_txbuf_dt++;
                }

                servo_data = (servo_data & 0xFFFF) | ((UI32)(((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->lcldata) << 16);
            }
            else
            {
                servo_data = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->lcldata;
            }

            SetEcatSdoSend(ServoNo, servo_index, servo_subindex, servo_data, servo_lenth);

            ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = SDO_WAITASN;
            ret = TX_SDO_BUSY;
            break;
        case SDO_WAITASN:
            ServoNo = ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->servono;
            if (EcatSdoChk(ServoNo))
            {
                ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = SDO_END;
            }
            else
            {
                cn_sending++;
                if (cn_sending > 1000) //原来是5000，警报时间太久

                {
                    ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = SDO_ERR;
                }
                else if (cn_sending % 100 == 99)
                {
                    if (((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->adresslen_tx == 32)
                    {
                        ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = SDO_SENDING;
                        if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_txbuf_start_dt)
                        {
                            g_sdotxbuf.p_sdofill_txbuf_dt = g_sdotxbuf.p_txbuf_end_dt;
                        }
                        else
                        {
                            g_sdotxbuf.p_sdofill_txbuf_dt--;
                        }
                    }
                    ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = SDO_SENDING;
                }
            }
            ret = TX_SDO_STAND;
            break;
        case SDO_END:
            ((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step = ATTONITY;

            if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_txbuf_end_dt)
            {
                g_sdotxbuf.p_sdofill_txbuf_dt = g_sdotxbuf.p_txbuf_start_dt;
            }
            else
            {
                g_sdotxbuf.p_sdofill_txbuf_dt++;
            }

            ret = TX_SDO_END;
            break;
        case SDO_ERR:
            if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_txbuf_end_dt)
            {
                g_sdotxbuf.p_sdofill_txbuf_dt = g_sdotxbuf.p_txbuf_start_dt;
            }
            else
            {
                g_sdotxbuf.p_sdofill_txbuf_dt++;
            }

            ret = TX_SDO_ERR;
            break;
        default:
            if (g_sdotxbuf.p_sdofill_txbuf_dt == g_sdotxbuf.p_txbuf_end_dt)
            {
                g_sdotxbuf.p_sdofill_txbuf_dt = g_sdotxbuf.p_txbuf_start_dt;
            }
            else
            {
                g_sdotxbuf.p_sdofill_txbuf_dt++;
            }

            ret = TX_SDO_END;
            break;
    }
    return ret;
}



COM_SDO_RET SdoReceiveProcess(PSERVOECAT	pp)
{
    static BOOL RxSdoAllot = FALSE;
    COM_SDO_RET ret = RX_SDO_FREE;

    if ((((PSDORX_DATAFORM)g_sdorxbuf.p_sdopick_rxbuf_dt)->step != ATTONITY) && RxSdoAllot)
    {
        ret = SdoDtRxData();
    }
    else if (RxSdoAllot == FALSE || (DIFF_100MS(pp->ecat_sdorx_starttm) > 10))
    {
        ret = SdoRxData();
        if (ret == RX_SDO_END || ret == RX_SDO_FREE || ret == RX_SDO_ERR)
        {
        	pp->ecat_sdorx_starttm = Get100MsTick();			
            RxSdoAllot = TRUE;
        }
        else
        {
            RxSdoAllot = FALSE;
        }
    }

    return ret;
}

COM_SDO_RET SdoSendProcess(PSERVOECAT	pp)
{
    static BOOL TxSdoAllot = FALSE;
    COM_SDO_RET ret = TX_SDO_FREE;

    if ((((PSDOTX_DATAFORM)g_sdotxbuf.p_sdofill_txbuf_dt)->step != ATTONITY) && TxSdoAllot)
    {
        ret = SdoDtTxData();
    }
    else if (TxSdoAllot == FALSE || (DIFF_100MS(pp->ecat_sdotx_starttm) > 10))
    {
        ret = SdoTxData();
        if (ret == TX_SDO_END || ret == TX_SDO_FREE)
        {
        	pp->ecat_sdotx_starttm = Get100MsTick();
            TxSdoAllot = TRUE;
        }
        else
        {
            TxSdoAllot = FALSE;
        }
    }
    return ret;
}

void SdoProcess()
{
    static BOOL SdoAllot = FALSE;
    COM_SDO_RET ret;
    PSERVOECAT	pp = &m_servoecat;
	UI16 i=0,flag_750 = 0;

    if (ChkRunFlag(INIF_SDO_NULL))
    {
        if (ChkRunFlag(INIF_ETHERCAT))
        {
        	switch (pp->ecat_init_step)
        	{
				case 0:
					pp->ecat_init_step_tm = Get10MsTick();
					pp->ecat_init_step++;					
					GetSys()->motor_enable_mode = ChkSysCfg(XF1_HTMT);
					break;
				case 1:
					if(DIFF_10MS(pp->ecat_init_step_tm)>10)
					{
						for(i = 0; i < ecat_es750_board_total; i++)
						{
							SdoDtDataUpdata(ecat_es750_board[i], 0x6000, 9, 0x6000, 16);//设置Ecat控制模式
							SdoDtDataUpdata(ecat_es750_board[i], 0x2000, 1, 0xA509, 16);//直接速度模式
							SdoDtDataUpdata(ecat_es750_board[i], 0x6800, 9, 0x6000, 16);//设置Ecat控制模式
							SdoDtDataUpdata(ecat_es750_board[i], 0x2800, 1, 0xA509, 16);//直接速度模式
						}
						for(i = 0; i < ecat_is580_board_total; i++)
						{
							SdoDtDataUpdata(ecat_is580_board[i], 0x6000, 9, 0x6000, 16);//设置Ecat控制模式
							SdoDtDataUpdata(ecat_is580_board[i], 0x2000, 1, 0xA509, 16);//直接速度模式
						}
						for(i = 0; i < ecat_is620_board_total; i++)
						{
							SdoDtDataUpdata(ecat_is620_board[i], 0x2000, 1430, 0x0241, 16);//输入驱动器密码
							SdoDtDataUpdata(ecat_is620_board[i], 0x6000, 9, 0x6000, 8);//设置Ecat控制模式
							
							if (GetSys()->motor_enable_mode > 0)//设置使能信号源:通讯使能/外部使能
								SdoDtDataUpdata(ecat_is620_board[i], 0x2000, 1, 0x0302, 16);
							else
								SdoDtDataUpdata(ecat_is620_board[i], 0x2000, 14, 0x0302, 16);
						}
						pp->ecat_init_step_tm = Get10MsTick();
						pp->ecat_init_step++;
					}
					break;
				case 2:
					if(DIFF_10MS(pp->ecat_init_step_tm)>20)
					{	
						flag_750 = 0;
						for(i = 0; i < ecat_es750_board_total; i++)
						{
							SdoDtDataDownload(ecat_es750_board[i], 0x2000, &(g_pumpn[ecat_es750_board[i]]->MotorRatedSpeed), 0xF105, 0, 0);
							SdoDtDataDownload(ecat_es750_board[i], 0x2000, &(g_pumpn[ecat_es750_board[i]]->MotorRatedFrequency), 0xF104, 0, 0);
							flag_750++;
							SdoDtDataDownload(ecat_es750_board[i], 0x2800, &(g_pumpn[ecat_es750_board[i]+flag_750]->MotorRatedSpeed), 0xF105, 0, 0);
							SdoDtDataDownload(ecat_es750_board[i], 0x2800, &(g_pumpn[ecat_es750_board[i]+flag_750]->MotorRatedFrequency), 0xF104, 0, 0);
						}
						for(i = 0; i < ecat_is580_board_total; i++)
						{
							SdoDtDataDownload(ecat_is580_board[i], 0x2000, &(g_pumpn[ecat_is580_board[i]]->MotorRatedSpeed), 0xF105, 0, 0);
							SdoDtDataDownload(ecat_is580_board[i], 0x2000, &(g_pumpn[ecat_is580_board[i]]->MotorRatedFrequency), 0xF104, 0, 0);
						}
						for(i = 0; i < ecat_is620_board_total; i++)
						{
							SdoDtData32Download(ecat_is620_board[i], 0x2000, (UI16 *)(&(g_pumpn[ecat_is620_board[i]+flag_750]->MotorMaxRPM)), ((UI16 *)(&(g_pumpn[ecat_is620_board[i]+flag_750]->MotorMaxRPM)))+1,0x0015, 0, 0);
						}
						pp->ecat_init_step_tm = Get10MsTick();
						pp->ecat_init_step++;
					}
					break;
				case 3:
					if(DIFF_10MS(pp->ecat_init_step_tm)>20)
					{
						flag_750 = 0;
						for(i = 0; i < ecat_es750_board_total; i++)
						{
							g_pumpn[ecat_es750_board[i]]->MotorPolePair = (g_pumpn[ecat_es750_board[i]]->MotorRatedFrequency*60)/g_pumpn[ecat_es750_board[i]]->MotorRatedSpeed;
							SdoDtDataDownload(ecat_es750_board[i], 0x2000, &(g_pumpn[ecat_es750_board[i]]->MotorMaxFrequency), 0xF010, 0, 0);
							flag_750++;
							g_pumpn[ecat_es750_board[i]]->MotorPolePair = (g_pumpn[ecat_es750_board[i]+flag_750]->MotorRatedFrequency*60)/g_pumpn[ecat_es750_board[i]+flag_750]->MotorRatedSpeed;
							SdoDtDataDownload(ecat_es750_board[i], 0x2800, &(g_pumpn[ecat_es750_board[i]+flag_750]->MotorMaxFrequency), 0xF010, 0, 0);
						}
						for(i = 0; i < ecat_is580_board_total; i++)
						{
							g_pumpn[ecat_is580_board[i]]->MotorPolePair = (g_pumpn[ecat_is580_board[i]]->MotorRatedFrequency*60)/g_pumpn[ecat_is580_board[i]+flag_750]->MotorRatedSpeed;
							SdoDtDataDownload(ecat_is580_board[i], 0x2000, &(g_pumpn[ecat_is580_board[i]]->MotorMaxFrequency), 0xF010, 0, 0);
						}
						for(i = 0; i < ecat_is620_board_total; i++)
						{
							SdoDtDataDownload(ecat_is620_board[i], 0x2000, &(pp->motortemp_type[ecat_is620_board[i]+flag_750]), 0x0242, 0, 0);
						}
						pp->ecat_init_step_tm = Get10MsTick();
						pp->ecat_init_step++;
					}
					break;				
				case 4:
					if(DIFF_10MS(pp->ecat_init_step_tm)>20)
					{
						flag_750 = 0;
						for(i = 0; i < ecat_es750_board_total; i++)
						{
							g_pumpn[ecat_es750_board[i]]->MotorMaxRPM = (g_pumpn[ecat_es750_board[i]]->MotorMaxFrequency*60)/g_pumpn[ecat_es750_board[i]]->MotorPolePair;
							SdoDtData32Download(ecat_es750_board[i], 0x2000,(UI16 *)(&(g_pumpn[ecat_es750_board[i]]->EcoderPPR)),((UI16 *)(&(g_pumpn[ecat_es750_board[i]]->EcoderPPR)))+1, 0xA106, 0, 0);
							flag_750++;
							g_pumpn[ecat_es750_board[i]+flag_750]->MotorMaxRPM = (g_pumpn[ecat_es750_board[i]+flag_750]->MotorMaxFrequency*60)/g_pumpn[ecat_es750_board[i]+flag_750]->MotorPolePair;
							SdoDtData32Download(ecat_es750_board[i], 0x2800,(UI16 *)(&(g_pumpn[ecat_es750_board[i]+flag_750]->EcoderPPR)),((UI16 *)(&(g_pumpn[ecat_es750_board[i]+flag_750]->EcoderPPR)))+1, 0xA106, 0, 0);
						}
						for(i = 0; i < ecat_is580_board_total; i++)
						{
							g_pumpn[ecat_is580_board[i]+flag_750]->MotorMaxRPM = (g_pumpn[ecat_is580_board[i]]->MotorMaxFrequency*60)/g_pumpn[ecat_is580_board[i]+flag_750]->MotorPolePair;
							SdoDtData32Download(ecat_is580_board[i], 0x2000,(UI16 *)(&(g_pumpn[ecat_is580_board[i]]->EcoderPPR)),((UI16 *)(&(g_pumpn[ecat_is580_board[i]+flag_750]->EcoderPPR)))+1, 0xA106, 0, 0);
						}
						for(i = 0; i < ecat_is620_board_total; i++)
						{
							SdoDtData32Download(ecat_is620_board[i], 0x2000,(UI16 *)(&(g_pumpn[ecat_is620_board[i]+flag_750]->EcoderPPR)),((UI16 *)(&(g_pumpn[ecat_is620_board[i]+flag_750]->EcoderPPR)))+1, 0x0031, 0, 0);
						}
						pp->ecat_init_step_tm = Get10MsTick();
						pp->ecat_init_step++;
					}
					break;
				case 5:
					if(DIFF_10MS(pp->ecat_init_step_tm)>20)
					{
						 pp->ecat_init_step++;
					}
					break;				
				case 6:
					Set_H0C35(PACKETLOSSMAXMS);
           			ClrRunFlag(INIF_ETHERCAT);
					break;

			}

			if (SdoAllot)
            {
                ret = SdoReceiveProcess(pp);
                if ((ret == RX_SDO_END) || (ret == RX_SDO_FREE))
                {
                    SdoAllot = !SdoAllot;
                }
                else
                {
                    if (ret == RX_SDO_ERR)
                    {
                        SET_PART_ERR(pp, ER0_SDOREADERR);
                    }
                }
            }
            else
            {
                ret = SdoSendProcess(pp);
                if ((ret == TX_SDO_END) || (ret == TX_SDO_FREE) || (ret == TX_SDO_STAND))
                {
                    SdoAllot = !SdoAllot;
                }
                else
                {
                    if (ret == TX_SDO_ERR)
                    {
                        SET_PART_ERR(pp, ER0_SDOSENDERR);
                    }
                }
            }
        }
        else
        {
            /*通讯驱动器参数操作*/
            ServoComPro();
            if (SdoAllot)
            {
                ret = SdoReceiveProcess(pp);
                if ((ret == RX_SDO_END) || (ret == RX_SDO_FREE))
                {
                    SdoAllot = !SdoAllot;
                }
                else
                {
                    if (ret == RX_SDO_ERR)
                    {
                        SET_PART_ERR(pp, ER0_SDOREADERR);
                    }
                }
            }
            else
            {
                ret = SdoSendProcess(pp);
                if ((ret == TX_SDO_END) || (ret == TX_SDO_FREE) || (ret == TX_SDO_STAND))
                {
                    SdoAllot = !SdoAllot;
                }
                else
                {
                    if (ret == TX_SDO_ERR)
                    {
                        SET_PART_ERR(pp, ER0_SDOSENDERR);
                    }
                }
            }
        }

		if (g_sdo_abort_code > 0)
		{
			if (g_sdo_abort_servono < PUMP_ECAT_NUM)
			{
				g_pumpn[g_sdo_abort_servono]->st->SERVO_COMM_STL = g_sdo_abort_code&0xFFFF;
				g_pumpn[g_sdo_abort_servono]->st->SERVO_COMM_STH = (g_sdo_abort_code>>16)&0xFFFF;

				g_pumpn[g_sdo_abort_servono]->sdo_alarm_starttm = Get100MsTick();
				g_pumpn[g_sdo_abort_servono]->sdo_alarm_abort = 1;
			}
		}

		for (i = 0; i < PUMP_ECAT_NUM;i++)
		{
			if (DIFF_100MS(g_pumpn[i]->sdo_alarm_starttm) > 50)
			{
				g_pumpn[i]->st->SERVO_COMM_STL = 0;
				g_pumpn[i]->st->SERVO_COMM_STH = 0;
			}
		}
    }
}

void InitSdoProcess()
{
	PSERVOECAT	pp = &m_servoecat;
    if (InitSdoRxBuf() && InitSdoTxBuf())
    {
        SetRunFlag(INIF_SDO_NULL);
    }
    else
    {
        ClrRunFlag(INIF_SDO_NULL);
    }
	pp->ecat_init_step = 0;
}


/*******************************************************************************************
 *Brief			:对所有驱动器设置H0C35数据
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190121
********************************************************************************************/
void Set_H0C35(UI16 data)
{
    UI8	i = 0;

    for (i = 0; i < ecat_is620_board_total; i++)
    {
        SdoDtDataUpdata(ecat_is620_board[i], 0x2000, data, 0x0C35, 16);

    }
}

/*******************************************************************************************
 *Brief			:设置控制模式 H1100设置7位液压模式，0为电动模式
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20190121
********************************************************************************************/
void Set_H1100(UI16 data)
{
    UI8	i = 0;
    for (i = 0; i < GetRealSlaveNum(); i++)
    {
        SdoDtData32Updata(i, 0x2000, data, 0x1100);
    }
}

void SetServoMode(UI16 bit)
{
    UI8	i = 0;
    for (i = 0; i < GetRealSlaveNum(); i++)
    {

		SdoDtData32Updata(i, 0x2000, 0x00, 0x1100);

//		if ((bit >> i) & 0x01)
//	    {
//	        SdoDtData32Updata(i, 0x2000, 0x07, 0x1100);
//	    }
//	    else
//	    {
//	        SdoDtData32Updata(i, 0x2000, 0x00, 0x1100);
//	    }
    }
}


void ClrServoErrByNo(WORD Servo)
{
    SdoDtDataUpdata(Servo, 0x2000, 1, 0x0D01, 16);
	PdoSendData[Servo].CtrlWord = (PdoSendData[Servo].CtrlWord)|(0x01<<7);
}

/*******************************************************************************************
 *Brief			:清除驱动器警报，只清除警报(对所有驱动器做清警报处理，无论是否存在警报)
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181115
********************************************************************************************/
void ServoRecoverActNoEn()
{
    int i;
    UI16 err;
    if (!ChkRunFlag(INIF_INITPART))
    {
        for (i = 0; i < GetRealSlaveNum(); i++)
        {
            /*清除驱动器警报，不分类，无论是否存在警报，都做清除处理*/
            err = GetServoErr(i);
            if (err != 0 && (err != 0x6e08 || ChkSysCfg(XF1_HTMT)))
            {
                ClrServoErrByNo(i);
				g_pumpn[i]->servo_err_clear_tm = Get10MsTick();
				g_pumpn[i]->servo_err_clear_flag = 1;
            }
        }
    }
}


/*******************************************************************************************
 *Brief			:载入数据到缓存内，如果读写条件成立则将缓存内数据移到调用数据中，供读写时调用
 *Param			:
 *Return Value	:TRUE:条件成立可以读写;FALSE:条件不成立，返回错误
 *Aauthor		:ZT
 *Date			:20181009
********************************************************************************************/
BOOL ServoValReqRx(PPRO_SDO_RX_PKT_HD ppkt, UI16 pkt_id)
{
    PPRO_SDO_RE_LOG ptbl_servo;
    UI16 row_len;
    UI16 row_num;
    UI16 cache_size;
    UI16 size;
    PPROSERVOECAT pp = &g_pro_servo;

    if (ppkt == NULL || pp == NULL)
    {
        return FALSE;
    }

	if (pp->pro_sdo_rx_pkt[1].type > 0 || pp->ptbl_Cache != NULL)
    {
        //调用数据被占用，说明有数据正在读写,以前模式为不理睬当前指令，
        //现在更改为丢弃当前动作，运行新的指令动作
//        
//        pp->pro_sdo_rx_pkt[0].pkt_state = 2;		
//        return FALSE;

//		FreeCacheWord(pp->ptbl_Cache);
		ServoDataENErr(pp);
    }
	
//	db_printf("ServoValReqRx pkt type=%04x;ver=%04x;row=%04x;\r\n",ppkt->type,ppkt->ver, ppkt->row);
	
    pp->pro_sdo_rx_pkt[0].row = ppkt->row;
    pp->pro_sdo_rx_pkt[0].row_len = ppkt->row_len;
    pp->pro_sdo_rx_pkt[0].max_row = ppkt->max_row;
    pp->pro_sdo_rx_pkt[0].type = ppkt->type;
    pp->pro_sdo_rx_pkt[0].ver = ppkt->ver;
    pp->pro_sdo_rx_pkt[0].pkt_state = 0;	//初始化为读取失败状态，读取成功后赋值1
    pp->pro_row_rt[0] = 0;					//初始化为0
    pp->pkt_id[0] = pkt_id;

	//数据包必须满足 1:ver等于0x01	2:行数小于最大行数	3:最大行数等于80
	if(!((ppkt->ver == 0x01) && (ppkt->row <= ppkt->max_row) && (ppkt->max_row == 80)))
	{
		return FALSE;
	}

    


    if (pp->pro_sdo_rx_pkt[0].type == 2)
    {
        if (MotorIsOn())
        {
            //开马达时不能写数据
            pp->pro_sdo_rx_pkt[0].pkt_state = 0;
            return FALSE;
        }
    }


    ptbl_servo = (PPRO_SDO_RE_LOG)(ppkt + 1);
    row_len = ppkt->row_len;
    row_num = ppkt->row;
    size = row_len * row_num;

    pp->ptbl_Cache = (PPRO_SDO_RE_LOG)GetCacheWord(&cache_size);
    if (pp->ptbl_Cache == NULL)
    {
        //无缓存可用时，返回FALSE
        pp->pro_sdo_rx_pkt[0].pkt_state = 3;
        return FALSE;
    }

    if (cache_size < size)
    {
        FreeCacheWord(pp->ptbl_Cache);
        return FALSE;
    }

    pp->pro_row_rt[1] = pp->pro_row_rt[0];
    pp->pkt_id[1] = pp->pkt_id[0];

    /*缓存数据区移到调用数据区*/
    memcpy(&(pp->pro_sdo_rx_pkt[1]), &(pp->pro_sdo_rx_pkt[0]), sizeof(PRO_SDO_RX_PKT_HD));
    /*将读取数据内容移到共用内存*/
    MemcpyPro(pp->ptbl_Cache, ptbl_servo, size);

    pp->ppro_pick_now = pp->ptbl_Cache;

    pp->pro_step = SDO_ARRANGE;
    pp->pro_readstartTm_100ms = Get100MsTick();

    return TRUE;
}

/*******************************************************************************************
 *Brief			:读数据，从内存中拿出数据，并一个个读取SDO
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181009
********************************************************************************************/
BOOL ServoValReqRE(PPROSERVOECAT pp)
{
    PPRO_SDO_RE_LOG ppro_pick_buf = NULL;
    UI16	servoNo = 0;
    UI16	servoAddr = 0;
    UI16	servoLen = 0;
    UI16	servoFunc = 0;

    switch (pp->pro_step)
    {
        case SDO_ARRANGE:
            pp->value_ret_l = 0;
            pp->value_ret_h = 0;
            if (pp->pro_row_rt[1] >= pp->pro_sdo_rx_pkt[1].row)
            {
                pp->pro_step = SDO_END;
            }
            else
            {
                pp->pro_row_rt[1]++;
                pp->pro_step = SDO_SENDING;
            }

            break;
        case SDO_SENDING:
            if (pp->ppro_pick_now->servo_sn > 0)
            {
                //上位机下发编号为1开始，下位机读取时编号0开始
                servoNo = pp->ppro_pick_now->servo_sn - 1;
            }
            else
            {
                servoNo = pp->ppro_pick_now->servo_sn;
            }
            servoAddr = pp->ppro_pick_now->servo_addr;
            servoFunc = pp->ppro_pick_now->servo_func;
            servoLen = pp->ppro_pick_now->servo_len;


            /*将需要读取的数据写进SDO轮询中，等待回复*/
            if (servoLen != 32)
            {
                pp->pro_rx_step = SdoDtDataDownload(servoNo, servoFunc, &(pp->value_ret_l), servoAddr, 0, 0);
            }
            else
            {
                pp->pro_rx_step = SdoDtData32Download(servoNo, servoFunc, &(pp->value_ret_l), &(pp->value_ret_h), servoAddr, 0, 0);
            }

			
			//db_printf("SDO DATA RX List, ServoNo = %d ServoAddr = %04x ServoFunc = %04x ServoLen = %d\r\n",servoNo+1,servoAddr,servoFunc,servoLen);

            pp->readstartTm_100ms = Get100MsTick();
            pp->pro_step = SDO_WAITASN;
            break;
        case SDO_WAITASN:
            if (*(pp->pro_rx_step) == ATTONITY)//Step至空，说明读取完成
            {
                ppro_pick_buf = (PPRO_SDO_RE_LOG)pp->ppro_pick_now;
                ppro_pick_buf->servo_val_h = pp->value_ret_h;
                ppro_pick_buf->servo_val_l = pp->value_ret_l;

				
//				db_printf("SDO DATA RX List Return, Ret_L = %d Ret_H = %04x\r\n",pp->value_ret_l,pp->value_ret_h);

                pp->ppro_pick_now = (PPRO_SDO_RE_LOG)((UI32)pp->ppro_pick_now + pp->pro_sdo_rx_pkt[1].row_len);
                pp->pro_step = SDO_ARRANGE;
            }
			else if (*(pp->pro_rx_step) == SDO_ERR)//Step至空，说明读取完成
			{
				pp->pro_step = SDO_ERR;
			}
            else if (DIFF_100MS(pp->readstartTm_100ms) > 5)//sdo去读数据超时保护
            {
                SET_PART_ERR(&m_servoecat, ER0_SDOREADERR);
                pp->pro_step = SDO_ERR;
            }
            break;
        case SDO_END:
            /*所有数据读取完成，标记完成状态*/
            pp->pro_sdo_rx_pkt[1].pkt_state = 1;
            ServoDataRxEnd(pp);
            break;
        case SDO_ERR:
			ServoDataRxEnd(pp);
            break;
        case ATTONITY:
            break;
        default:
            break;
    }

    return TRUE;
}

/*******************************************************************************************
 *Brief			:写数据，从内存中拿出数据，并一个个写入SDO轮询
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181009
********************************************************************************************/
BOOL ServoValRspRE(PPROSERVOECAT pp)
{
    UI16	servoNo = 0;
    UI16	servoAddr = 0;
    UI16	servoLen = 0;
    UI16	servoFunc = 0;
    UI16	value_ret_l, value_ret_h;

    switch (pp->pro_step)
    {
        case SDO_ARRANGE:
            pp->value_ret_l = 0;
            pp->value_ret_h = 0;
            if (pp->pro_row_rt[1] >= pp->pro_sdo_rx_pkt[1].row)
            {
                pp->pro_step = SDO_END;
            }
            else
            {
                pp->pro_row_rt[1]++;
                pp->pro_step = SDO_SENDING;
            }

            break;
        case SDO_SENDING:
            if (pp->ppro_pick_now->servo_sn > 0)
            {
                //上位机下发编号为1开始，下位机读取时编号0开始
                servoNo = pp->ppro_pick_now->servo_sn - 1;
            }
            else
            {
                servoNo = pp->ppro_pick_now->servo_sn;
            }

            servoAddr = pp->ppro_pick_now->servo_addr;
            servoFunc = pp->ppro_pick_now->servo_func;
            servoLen = pp->ppro_pick_now->servo_len;
            value_ret_l = pp->ppro_pick_now->servo_val_l;
            value_ret_h = pp->ppro_pick_now->servo_val_h;

//			db_printf("SDO DATA TX List, ServoNo = %d ServoAddr = %04x ServoFunc = %04x ServoLen = %d Val_L = %04x Val_H = %04x\r\n",servoNo+1,servoAddr,servoFunc,servoLen,value_ret_l,value_ret_h);

            /*将需要写的数据写进SDO轮询中，等待回复*/
            if (servoLen != 32)
            {
                pp->pro_rx_step = SdoDtDataUpdata(servoNo, servoFunc, value_ret_l, servoAddr, servoLen);
            }
            else
            {
                pp->pro_rx_step = SdoDtData32Updata(servoNo, servoFunc, ((value_ret_h << 16) & 0xFFFF) | (value_ret_l & 0xFFFF), servoAddr);
            }

            pp->readstartTm_100ms = Get100MsTick();
            pp->pro_step = SDO_WAITASN;
            break;
        case SDO_WAITASN:
            if (*(pp->pro_rx_step) == ATTONITY)//Step至空，说明读取完成
            {
                pp->ppro_pick_now = (PPRO_SDO_RE_LOG)((UI32)pp->ppro_pick_now + pp->pro_sdo_rx_pkt[1].row_len);
                pp->pro_step = SDO_ARRANGE;
            }
			else if (*(pp->pro_rx_step) == SDO_ERR)//Step至空，说明读取完成
			{
				pp->pro_step = SDO_ERR;
			}
            else if (DIFF_100MS(pp->readstartTm_100ms) > 5)//sdo去读数据超时保护
            {
                SET_PART_ERR(&m_servoecat, ER0_SDOREADERR);
                pp->pro_step = SDO_ERR;
            }
            break;
        case SDO_END:
            /*所有数据操作完成，标记完成状态*/
            pp->pro_sdo_rx_pkt[1].pkt_state = 1;
            ServoDataRxEnd(pp);
            break;
        case SDO_ERR:
            ServoDataRxEnd(pp);
            break;
        case ATTONITY:
            break;
        default:
            break;
    }

    return TRUE;
}

BOOL ServoComPro()
{
    PPROSERVOECAT pp = &g_pro_servo;

    if (pp->ppro_pick_now == NULL || pp->ptbl_Cache == NULL)//非空判断
    {
        return FALSE;
    }

    if (pp->pro_sdo_rx_pkt[1].type == 1)
    {
        //type = 1驱动器读数据	-zt 20180914
        ServoValReqRE(pp);
    }
    else if (pp->pro_sdo_rx_pkt[1].type == 2)
    {
        //type = 2驱动器写数据	-zt 20180914
        ServoValRspRE(pp);
    }
		
		return TRUE;
}

/*******************************************************************************************
 *Brief			:正常读写后，应答给面板，读数据成功回给面板表头及数据块，而写数据只回给面板表头，说明成功
 *Param			:back:表示需要应答
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181009
********************************************************************************************/
BOOL SendServoDataReq(PPROSERVOECAT pp, BOOL back)
{
    PPRO_SDO_RX_PKT_HD ppkt;
    UI16 rowlen;
    UI16 row;
    UI8* prow;

    if (pp == NULL)
    {
        return FALSE;
    }

    if (pp->pro_sdo_rx_pkt[1].type == 1)
    {
        //读数据需要将整片数据返回给面板
        rowlen =  pp->pro_sdo_rx_pkt[1].row_len;
        row =  pp->pro_row_rt[1];

        if ((ppkt = (PPRO_SDO_RX_PKT_HD)ProtNewPkt(PKT_SERVO_REQ_CMD, pp->pkt_id[1], back, rowlen * row + sizeof(PRO_SDO_RX_PKT_HD))) != NULL)
        {
            /*将包头数据拷贝到通讯缓冲区中*/
#ifdef SYS232
            MemcpyPro(ppkt, &(pp->pro_sdo_rx_pkt[1]), sizeof(PRO_SDO_RX_PKT_HD));
#else
			memcpy(ppkt, &(pp->pro_sdo_rx_pkt[1]), sizeof(PRO_SDO_RX_PKT_HD));
#endif
            prow = (UI8*)(ppkt + 1);
            if (pp->ptbl_Cache != NULL)
            {
#ifdef SYS232
                MemcpyPro(prow, pp->ptbl_Cache, rowlen * row);
#else
				memcpy(prow, pp->ptbl_Cache, rowlen * row);
#endif
            }
            else
            {
				memset(prow, 0, rowlen * row);
            }
            return MainProtTxPkt(ppkt);
        }
    }
    else if (pp->pro_sdo_rx_pkt[1].type == 2)
    {
        //因为只发送表头，所以row = 0
        pp->pro_sdo_rx_pkt[1].row = 0;
        //写数据成功后将表头回给面板，表示完成
        if ((ppkt = (PPRO_SDO_RX_PKT_HD)ProtNewPkt(PKT_SERVO_RSP_CMD, pp->pkt_id[1], back, sizeof(PRO_SDO_RX_PKT_HD))) != NULL)
        {
            /*将包头数据拷贝到通讯缓冲区中*/
#ifdef SYS232
            MemcpyPro(ppkt, &(pp->pro_sdo_rx_pkt[1]), sizeof(PRO_SDO_RX_PKT_HD));
#else
 			memcpy(ppkt, &(pp->pro_sdo_rx_pkt[1]), sizeof(PRO_SDO_RX_PKT_HD));
#endif
            return MainProtTxPkt(ppkt);
        }
    }


    return FALSE;
}


void ServoDataRxEnd(PPROSERVOECAT pp)
{
    if (pp == NULL)
    {
        return;
    }

    SendServoDataReq(pp, FALSE);

    memset(&(pp->pro_sdo_rx_pkt[0]), 0, sizeof(PRO_SDO_RX_PKT_HD) * 2);

    if (pp->ptbl_Cache != NULL)
    {
        FreeCacheWord(pp->ptbl_Cache);
    }
    pp->ppro_pick_now = NULL;
    pp->ptbl_Cache = NULL;
    pp->pro_step = ATTONITY;
}


/*******************************************************************************************
 *Brief			:读写失败时将表头回给面板，表示失败
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181009
********************************************************************************************/
BOOL ServoDataENErr(PPROSERVOECAT pp)
{
    PPRO_SDO_RX_PKT_HD ppkt;
	WORD ret = 0;

    //因为只发送表头，所以row = 0
    pp->pro_sdo_rx_pkt[0].row = 0;
    //读写数据失败后将表头回给面板
    if ((ppkt = (PPRO_SDO_RX_PKT_HD)ProtNewPkt(((pp->pro_sdo_rx_pkt[0].type == 1) ? PKT_SERVO_REQ_CMD : PKT_SERVO_RSP_CMD), pp->pkt_id[0], FALSE, sizeof(PRO_SDO_RX_PKT_HD))) != NULL)
    {
        /*将包头数据拷贝到通讯缓冲区中*/
        memcpy(ppkt, &(pp->pro_sdo_rx_pkt[0]), sizeof(PRO_SDO_RX_PKT_HD));
        memset(&(pp->pro_sdo_rx_pkt[0]), 0, sizeof(PRO_SDO_RX_PKT_HD));
        ret = MainProtTxPkt(ppkt);
    }

	if(pp->ptbl_Cache != NULL)
	{
		FreeCacheWord(pp->ptbl_Cache);
	}

	return ret;
}

BOOL ServoDataReqRx(PPRO_SDO_RX_PKT_HD ppkt, UI16 pkt_id)
{
    PPROSERVOECAT pp = &g_pro_servo;

    if (ppkt == NULL || pp == NULL)
    {
        return FALSE;
    }

    if (!ServoValReqRx((PPRO_SDO_RX_PKT_HD)ppkt, pkt_id))
    {
        ServoDataENErr(pp);
    }
    return TRUE;
}

BOOL ServoDataRspRx(PPRO_SDO_RX_PKT_HD ppkt, UI16 pkt_id)
{
    PPROSERVOECAT pp = &g_pro_servo;

    if (ppkt == NULL || pp == NULL)
    {
        return FALSE;
    }

    if (!ServoValReqRx((PPRO_SDO_RX_PKT_HD)ppkt, pkt_id))
    {
        ServoDataENErr(pp);
    }
    return TRUE;
}

/*******************************************************************************************
 *Brief			:主要防止驱动器出现问题，导致无法释放内存，在此做了两秒的保护，两秒必须释放占用的内存并停止读写
 *Param			:
 *Return Value	:
 *Aauthor		:ZT
 *Date			:20181009
********************************************************************************************/
void FreeServoDatCache()
{
    PPROSERVOECAT pp = &g_pro_servo;

    if (pp->ptbl_Cache == NULL)
    {
        return;
    }

    if (DIFF_100MS(pp->pro_readstartTm_100ms) > 20)
    {
        ServoDataRxEnd(pp);
    }
}

BOOL InitServoEcatPart()
{
    PSERVOECAT pp;

    pp = &m_servoecat;
    static const SUB_INIT_INFO sub_init[]  = SERVOECAT_INIT_INFO;

    if (!INIT_PART(pp, ECAT_ID, 1, sub_init))
    {
        return FALSE;
    }

		pp->cycle_tm_out_cn = 0;
    return TRUE;
}


UI32 ServoEcatCmd(UI32 cmd, UI32 para)
{
    switch (GET_CMD(cmd))
    {
        case CMD_MANUAL_INIT:
            break;
        case CMD_SETUP:
            break;
        case CMD_DATA_RX:
            break;
    }

    return TRUE;
}

