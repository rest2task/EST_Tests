#include "common.h"
#include "canhead.h"
#include "can_e702.h"
#include "hwcfg.h"
#include "TempCtrlMw.h"
#include "keycmd.h"
#include "hwmid.h"


UI32 	can_rx_count=0;
UI32	can_tx_count=0;
UI32	can_tx_in_count = 0;

CANTXBUF        g_cantxbuf;
CANRXBUF        g_canrxbuf;

CANINT          g_canint;

CanIOComm       g_canliocomm[LIO_MAX_NUM];
CanDAComm       g_candacomm[DA_MAX_NUM];
CanServoComm    g_canservocomm[SERVO_MAX_NUM];

CanPanelComm    g_canpanelcomm[PANEL_MAX_NUM];
CanTempE502Comm g_cantempe502comm[TEMP_MAX_NUM];
DB_TEMPMOLD     g_cantempe501[TEMPMW_MAX_NUM];
extern BOOL     can_node_E501_check[TEMPMW_MAX_NUM];
BOOL node_presence[BOARD_KIND_MAX][BOARD_NUM_MAX] = {0};
BOOL node_oncheck[BOARD_KIND_MAX][BOARD_NUM_MAX];
WORD        tst_io1 = 0;
BOOL		cantx_flag = 0;				//发送机制改进用     2011-1-10   whaul
WORD		cantx_time = 0; 


//----------------------------------------------------------------------------
//static BYTE		Enter_SWRst(BYTE);
//static BYTE		Quit_SWRst(BYTE);
static void     CanProcessInit(void);
static void     CanNodePresenceRevert(WORD, WORD);
static void     CanLioStructInit(void);
static void     CanDAStructInit(void);
static void     CanServoStructInit(void);
static void     CanNodeGuardProcess(void);


BOOL CanExtIOWrite(UI8 id, UI16* extio_val)
{
	if(id < LIO_MAX_NUM)
	{
	    g_canliocomm[id].pdo_tx_data[0] = *extio_val;
	    //g_canliocomm[id].p_origin_data = extio_val;
	    g_canliocomm[id].origin_data_length = 2;
	    g_canliocomm[id].flag_send = TRUE;

		if(g_statkey.IOoutclr[id] == 1)
		{
			g_statkey.IOoutclr[id] = 0;
			g_canliocomm[id].pdo_tx_data[1] = 0x01;
			g_statkey.IOflag[id] = 0x01;
		}
		else
		{
			if(g_statkey.IOflag[id] == 0x01)
			{
				g_statkey.IOflag[id] = 0;
				g_canliocomm[id].pdo_tx_data[1] = 0x0;
			}
		}
		return TRUE; 
	}
	else
		return FALSE;
}

BOOL CanExtIORead(UI8 id, UI16* extio_val)
{
	if(id < LIO_MAX_NUM)
	{
		if(extio_val != NULL)
			*extio_val = g_canliocomm[id].input_state;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CanExtADRead(UI8 id, UI16* extio_val)
{
	if(id < DA_MAX_NUM)
	{
		if(extio_val != NULL)
			*extio_val = g_candacomm[id].rx_buf[0];
		return TRUE;
	}
	else
		return FALSE;
}

static void
CanLioStructInit(void)
{             
	UI16 i;
    for ( i=0; i<LIO_MAX_NUM; i++)
    {
        g_canliocomm[i].node_state = BUS_FREE;
        g_canliocomm[i].flag_send = false;
        g_canliocomm[i].p_origin_data = (void *)0;
        g_canliocomm[i].origin_data_length = 0;       
        g_canliocomm[i].p_origin_data = g_canliocomm[i].pdo_tx_data; 
        g_canliocomm[i].origin_data_length = 4;       
        g_canliocomm[i].flag_wait = false;
        g_canliocomm[i].reply_count = 0;
        g_canliocomm[i].comm_err = false;
        g_canliocomm[i].input_state = 0;
        g_canliocomm[i].node_guarding_count = 0;

		g_canliocomm[i].pdo_tx_data[0] = 0;
		g_canliocomm[i].pdo_tx_data[1] = 0;
		g_canliocomm[i].pdo_tx_data[2] = 0;
		g_canliocomm[i].pdo_tx_data[3] = 0;
    }
}
static void
CanDAStructInit(void)
{ 
	UI16 i;
    for ( i=0; i<DA_MAX_NUM; i++)
    {
        g_candacomm[i].node_state = BUS_FREE;
        g_candacomm[i].flag_send = false;
        g_candacomm[i].p_origin_data = g_candacomm[i].tx_buf;//(void *)0;//da_tst_array;//
        g_candacomm[i].origin_data_length = 2;  //2 word : first is press, second is flux
        g_candacomm[i].flag_wait = false;
        g_candacomm[i].reply_count = 0;
        g_candacomm[i].comm_err = false;
        
        g_candacomm[i].node_guarding_count = 0;
    }
}

static void
CanServoStructInit(void)
{
    UI16 i;
    for ( i=0; i<SERVO_MAX_NUM; i++)
    {
        g_canservocomm[i].node_state    = BUS_FREE;
        g_canservocomm[i].flag_send = false;
        g_canservocomm[i].p_origin_data =  g_canservocomm[i].pdo_tx_data;
        g_canservocomm[i].origin_data_length = 3;
        
        g_canservocomm[i].send_count = 0;
        
        g_canservocomm[i].pdo_tx_data[0] = 0x16;
        g_canservocomm[i].pdo_tx_data[1] = 0;           //放频率
        g_canservocomm[i].pdo_tx_data[2] = 0;           //放转矩
        g_canservocomm[i].pdo_rx_data[0] = 0;
        g_canservocomm[i].pdo_rx_data[1] = 0;           //放实际速度
        g_canservocomm[i].pdo_rx_data[2] = 0;           //放故障状态
        g_canservocomm[i].pdo_rx_data[3] = 0;           //放故障状态
                
        g_canservocomm[i].comm_err = false;
    }
}

static void
CanPanelStructInit(void)
{
    UI16 i;
    for ( i=0; i<PANEL_MAX_NUM; i++)
    {
        g_canpanelcomm[i].node_state    = BUS_FREE;
        g_canpanelcomm[i].flag_send = false;
        g_canpanelcomm[i].p_origin_data =  g_canpanelcomm[i].pdo_tx_data;
        g_canpanelcomm[i].origin_data_length = 3;
        
        g_canpanelcomm[i].send_count = 0;
        
        g_canpanelcomm[i].pdo_tx_data[0] = 0;       //命令行
        g_canpanelcomm[i].pdo_tx_data[1] = 0;           
        g_canpanelcomm[i].pdo_tx_data[2] = 0;         

        g_canpanelcomm[i].pdo_rx_data[0] = 0;       //命令行
        g_canpanelcomm[i].pdo_rx_data[1] = 0;          
        g_canpanelcomm[i].pdo_rx_data[2] = 0;          
                
        g_canpanelcomm[i].comm_err = false;
        
        g_canpanelcomm[i].sdo_re_flag_send = false;        //2010-01-05  whaul
        g_canpanelcomm[i].p_sdo_data =  0;
        g_canpanelcomm[i].p_sdo_data_length = 8;     
        
        g_canpanelcomm[i].sdo_rx_task =  0;
        g_canpanelcomm[i].sdo_rx_tasktail = 0;        //2010-01-05  whaul
    }
}
static void CanTempE502StructInit(void)
{
    UI16 i;
    for ( i=0; i<TEMP_MAX_NUM; i++)
    {
        g_cantempe502comm[i].flag_send = false;
        g_cantempe502comm[i].p_origin_data =  g_cantempe502comm[i].pdo_tx_data;
        
        
        g_cantempe502comm[i].pdo_tx_data[0] = 0;
        g_cantempe502comm[i].pdo_tx_data[1] = 0;           //???
        g_cantempe502comm[i].pdo_tx_data[2] = 0;           //???
		g_cantempe502comm[i].pdo_tx_data[3] = 0;  

        g_cantempe502comm[i].pdo_rx_data[0] = 0;
        g_cantempe502comm[i].pdo_rx_data[1] = 0;           //?????
        g_cantempe502comm[i].pdo_rx_data[2] = 0;           //?????
        g_cantempe502comm[i].pdo_rx_data[3] = 0;           //?????

		g_cantempe502comm[i].node_guarding_count=0;
		g_cantempe502comm[i].comm_err=false;
                    }
}

static void
CanTempMoldStructInit(void)
{
    UI16 i;
    for( i=0; i<TEMPMW_MAX_NUM; i++)
    {
        g_cantempe501[i].wMarkUsed = 0xEB90;
        g_cantempe501[i].nTempId = 1;	
		g_cantempe501[i].TempMold_node_state = 0;
		g_cantempe501[i].node_state_mw = 0;
		g_cantempe501[i].flag_send_mw = 0;
		g_cantempe501[i].flag_wait_mw = 0;
		g_cantempe501[i].send_count_mw = 0;
		g_cantempe501[i].reply_time_mw = 0;
		g_cantempe501[i].reply_count_mw = 0;
		g_cantempe501[i].p_origin_data_mw =  g_cantempe501[i].tx_buf;
		
		g_cantempe501[i].tx_buf[0] = 0;
        g_cantempe501[i].tx_buf[1] = 0;          
        g_cantempe501[i].tx_buf[2] = 0;           
		g_cantempe501[i].tx_buf[3] = 0; 
		
		g_cantempe501[i].rx_buf[0] = 0;
        g_cantempe501[i].rx_buf[1] = 0;          
        g_cantempe501[i].rx_buf[2] = 0;          
		g_cantempe501[i].rx_buf[3] = 0; 
		g_cantempe501[i].node_guarding_e501 = 0;
		g_cantempe501[i].node_gard_time = 0;
    }
}

static void CanProcessInit(void)
{
    UI16 i;
    UI16 j;
    for (i = 0 ; i < BOARD_KIND_MAX ; i++ )
    {
        for (j = 0 ; j < BOARD_NUM_MAX ; j++ )
        {
            node_presence[i][j] = false;
        }
    }
    CanLioStructInit();
    CanDAStructInit();
	CanServoStructInit();
    CanPanelStructInit();
    E702_CanStructInit();   
	CanTempE502StructInit();
	CanTempMoldStructInit();
}
//----------------------------------------------------------------------------
#include "machapi.h"
void CanTempNodeErr(WORD i);

static void CanNodeGuardProcess(void)
{
	WORD i;
    static WORD steptime = 0;    
    if ( (WORD)(A_10msclock - steptime) > 10)
    {
        for ( i=1;i<=g_tempcomm_num;i++)
        {
			if(g_cantempe502comm[i].node_guarding_count>20 && ChkTempMachTypeE50X())
			{
				g_cantempe502comm[i].comm_err=true;
				if(GetHeatingStatus())
				{
					GetSys()->md->CUSTOM_PARA[0] = 2;
					WarmOff();	
					SET_PART_ERR(g_tempcomm[i-1], ER0_E502_ERR);
				}

				if(g_tempcomm_num > 0)
				{
					CanTempNodeErr(i);
				}	
			}
        }

		for(i=1;i<TEMPMW_MAX_NUM;i++)
		{
			if(g_cantempe501[i].node_guarding_e501 > 5)
			{
				can_node_E501_check[i] = FALSE;
				mw_piece = mw_piece & (~(0x0001<<(i-1)));
			}
		}
        steptime = A_10msclock;
    }
}

//----------------------------------------------------------------------------
static void
CanNodePresenceRevert(WORD kind,WORD id)
{
//    WORD i;
    WORD dst_node;
    WORD last_tx_buf[6] = {0};
    
    dst_node = ( kind << 3 ) | ( id );
      
 //   last_tx_buf[0] = ( (CAN_NMT_CTRL_CODE | dst_node) << 2 );       //PUT INTO MSGID
    last_tx_buf[0] = ( (CAN_NMT_CTRL_CODE | dst_node));       //PUT INTO MSGID
    last_tx_buf[1] = 4;                                             //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
    last_tx_buf[2] = ( ENTER_OPST << 8) | ( CAN_ID_ME );
    last_tx_buf[3] = (3<<8) | 0x90;	//A6A9的硬件版本号是2 ；IV3200的硬件版本号是3，90用来给扩展板做标记判断
    last_tx_buf[4] = 0x03;			//A6A9的软件版本号是2 ；IV3200的软件版本号是3，90用来给扩展板做标记判断
    last_tx_buf[5] = 0;
	
    CanTxBufWrite(last_tx_buf,dst_node);    
}
/*
------------------------------------------------------------------------------
|       Can Node Presence Check Function
------------------------------------------------------------------------------
*/
void CanNodePresenceCheck(void)
{
    WORD ext_kind;
    WORD ext_board_id;
	
    if ( HIBYTE(pick_rx_buf[2]) == 0x55)
    {
        ext_kind = ( (LOBYTE(pick_rx_buf[2]) & 0x78) >> 3);
        ext_board_id = (LOBYTE(pick_rx_buf[2]) & 0x7);		   

		if(ext_kind >= BOARD_KIND_MAX || ext_board_id >= BOARD_NUM_MAX)
			return;

#if 1
		switch (ext_kind)
        {
			case CAN_TEMPE502_KIND:	//主机发送一个心跳包，等接受到后，则赋值0
//				if (ext_board_id > 0 && g_tempcomm[ext_board_id - 1] != NULL)
//				{
//					type  =LOBYTE(pick_rx_buf[3]);	//E502、E506扩展板会发送板子标记过来用于区分硬件
//				}				
				break;
			case CAN_LSPDIO_KIND:
//				test_io_ver = pick_rx_buf[3];	//硬件版本识别
				break;
            default :    
				break;  
        }
#endif

        if (node_presence[ext_kind][ext_board_id] == false)
        {
            node_presence[ext_kind][ext_board_id] = true;
        }
        CanNodePresenceRevert( ext_kind, ext_board_id );
        pick_rx_buf[2] = 0;    
    } 

   	if((HIBYTE(pick_rx_buf[2])&0xff)== 0x02)   //E501温度扩展板采用主机主动发送握手命令方式，接收到回复包后认为通讯建立  20200116
	{
    	ext_kind = ((LOBYTE(pick_rx_buf[2]) & 0x78) >> 3);
        ext_board_id = (LOBYTE(pick_rx_buf[2]) & 0x7);
		
		if(ext_kind >= BOARD_KIND_MAX || ext_board_id >= BOARD_NUM_MAX)
			return;
		
		if(ext_kind == CAN_HSPDTEMP_KIND)
		{
	    	can_node_E501_check[ext_board_id] = true;
			mw_piece |=  (1 << (ext_board_id - 1));		//按位计算，得出每一块模温板的在离线状态
			g_cantempe501[ext_board_id].TempMold_node_state = 1;
			g_temp_mw[ext_board_id-1]->mw_put_step = 0 ;
			g_cantempe501[ext_board_id].node_guarding_e501 = 0;	//接收到握手信号后，将数据清零，防止又进入时，该次数大于错误次数而引起误警报

			if (node_presence[ext_kind][ext_board_id] == false)
	        {
	            node_presence[ext_kind][ext_board_id] = true;
	        }
		}
		
		pick_rx_buf[2] = 0;
	}
}

void CanInit(void)
{
	CanProcessInit();
	InitCANTxBuf ();
	InitCANRxBuf ();
	m_can_init_device(0,BPS_500K);
	E702_CanStructInitsystem();
	Can2Init();

}

void CanProcess(void)
{	
    CanTransmitProcess();
    CanReceiveProcess();
    CanNodeGuardProcess();
	//m7新增：使能邮箱接收。
	m_can_receive_block();
}

/*----------------------------------------------------------------------------*/
/*
    CAN TRANSMIT PROCESS CHECK
	Record:whaul修改于2011-1-11  
	重发机制更改: 原为保护性重发,N毫秒 发送指针始终没动,丢掉发送缓冲区数据,允许装载发送.
	 			  现为定时2ms到后,发送标志仍没清掉(发送不成功),重新启动发送,不进行重新装载
*/

void
CanTransmitCheck(void) 
{
	if ((WORD)((WORD)A_1msclock - cantx_time) > 2)		
 	{
 		if (cantx_flag != 0)
 		{
			cantx_time = (WORD)A_1msclock;
 		}
 	}	
}	

/*----------------------------------------------------------------------------*/
/*
    CAN tstprocess
*/
void
CanTstProcess(void)                                 //for tst 
{
    WORD i;
    
    for (i=0; i<LIO_MAX_NUM; i++)
    {
        if (node_presence[CAN_LSPDIO_KIND][i] == true)
        {
            g_canliocomm[i].flag_send = true;
            tst_io1++;
        }
    }
    for (i=0; i<DA_MAX_NUM; i++)
    {
        if (node_presence[CAN_DA_KIND][i] == true)
        {
            g_candacomm[i].flag_send = true;
        }
    }

	g_canservocomm[1].flag_send = true;
}

