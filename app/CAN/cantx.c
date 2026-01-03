
#include "canhead.h"
//#include "il3.h"
#include "can_e702.h"
#include "machapi.h"
#include "servo.h"
#include "can_e502.h"
#include "TempCtrlMw.h"
#include "systempro.h"
#include "ServFive.h"


/*
------------------------------------------------------------------------------
|       Global Variant  
------------------------------------------------------------------------------
*/
extern		UI32	can_tx_count;
BOOL 	can_tx_disable = 0;
extern BOOL can_node_E501_check[TEMPMW_MAX_NUM];
extern DB_TEMPMOLD g_cantempe501[TEMPMW_MAX_NUM];

//----------------------------------------------------------------------------
//static void     CanPDOTransmitBufferWrite(WORD *, WORD , WORD , WORD , WORD);
void    		CanSDOTransmitBufferWrite(WORD *, WORD , WORD , WORD , WORD);
static void     CanLowIOTransmit (void);
static void     CanTransmitStart(void);
static void     CanNodeGuardTxProc(void);
static void     TempMoldTransmit(void); 

/*
------------------------------------------------------------------------------
|       Static Functions
------------------------------------------------------------------------------
*/ 
//----------------------------------------------------------------------------

void CanTxBufWrite(WORD *last_tx_buf, WORD dst_node)
{
	PTEMPCTRLMW pmw = (PTEMPCTRLMW)GetPartByID(MK_PART(TEMP_MW, 1));
    WORD ext_kind;
	WORD i;

	ext_kind = ((dst_node&0x78) >> 3);

	if((ChkSysCfg(CO43_CAN2EXT)&& (ext_kind==CAN_LSPDIO_KIND||ext_kind==CAN_DA_KIND || ext_kind == CAN_TEMPE502_KIND))
		|| ((pmw != NULL) && (pmw->mh->TEMP_CANLINK == 1) && (ext_kind==CAN_HSPDTEMP_KIND)))
	{
        Can2TxBufWrite(last_tx_buf);
	}
	else
	{
	    if ( (g_cantxbuf.p_txbuf_end - g_cantxbuf.p_fill_txbuf) < 6)    //leave enough space to write
	    {
	        g_cantxbuf.p_fill_txbuf = g_cantxbuf.p_txbuf_start;
	    }
	
	    for ( i = 0; i < 6; i++)                                        //fill data in transmit buffer
	    {
	        *g_cantxbuf.p_fill_txbuf = last_tx_buf[i];
	        g_cantxbuf.p_fill_txbuf++;
	    }
	}
}

//----------------------------------------------------------------------------
void CanSDOTransmitBufferWrite(WORD *p_tx_data, WORD func_code, WORD length, WORD dst_node, WORD send_count )
{
    WORD i;
//    WORD j;    
    WORD last_tx_buf[6] = {0};
    // WORD ext_board_kind;
    //WORD ext_board_id;
  
    //ext_board_kind = ( dst_node & 0x78 ) >> 3 ;
    //ext_board_id   = ( dst_node & 0x07 ); 
    
    while(1)
    { 
        if (length > 3)
        {
            last_tx_buf[0] = (func_code | dst_node);  //2009-10-29 13:02 last_tx_buf[0] = ( (func_code | dst_node) << 2 );               //PUT INTO MSGID
            last_tx_buf[1] = 3 + 1;                                         //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
            last_tx_buf[2] = (send_count << 8) | ( CAN_ID_ME ); 
            last_tx_buf[3] = ( *p_tx_data++ ); 
            last_tx_buf[4] = ( *p_tx_data++ ); 
            last_tx_buf[5] = ( *p_tx_data++ );   
            
            CanTxBufWrite(last_tx_buf,dst_node);

            length = length - 3;
        }
        else
        {
            if (length == 0)
            {
                return;
            }
    //        last_tx_buf[0] = ( (func_code | dst_node) << 2 );   //PUT INTO MSGID
            last_tx_buf[0] = ((func_code | dst_node));
            last_tx_buf[1] = 3 + 1;    	//length = 3                                     //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
            last_tx_buf[2] = ( send_count << 8) | ( CAN_ID_ME );
            for (i = 0; i < 3; i++)
            {
                last_tx_buf[3 + i] = ( *p_tx_data++ ); 
            }
            
            CanTxBufWrite(last_tx_buf,dst_node);

            length = 0;
            return;
        }
    }             
}

/*伺服5模式新增 2015-01-04*/
void CanSDOTransmitBufferWriteMode5(WORD *p_tx_data, WORD func_code, WORD length, WORD dst_node)
{
    WORD i;
//    WORD j;    
    WORD last_tx_buf[6] = {0};
    // WORD ext_board_kind;
    //WORD ext_board_id;
  
    //ext_board_kind = ( dst_node & 0x78 ) >> 3 ;
    //ext_board_id   = ( dst_node & 0x07 ); 
    
    while(1)
    { 
        if (length > 3)
        {
            last_tx_buf[0] = (func_code | dst_node);  //2009-10-29 13:02 last_tx_buf[0] = ( (func_code | dst_node) << 2 );               //PUT INTO MSGID
            last_tx_buf[1] = 3 + 1;                                         //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
            last_tx_buf[2] = ( *p_tx_data++ ); 
            last_tx_buf[3] = ( *p_tx_data++ ); 
            last_tx_buf[4] = ( *p_tx_data++ ); 
            last_tx_buf[5] = ( *p_tx_data++ );   
            
            CanTxBufWrite(last_tx_buf,dst_node);

            length = length - 3;
        }
        else
        {
            if (length == 0)
            {
                return;
            }
            last_tx_buf[0] = ((func_code | dst_node));
            last_tx_buf[1] = 3 + 1;    	//length = 3                                     //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
            last_tx_buf[2] = ( *p_tx_data++ );
            for (i = 0; i < 3; i++)
            {
                last_tx_buf[3 + i] = ( *p_tx_data++ ); 
            }
            
            CanTxBufWrite(last_tx_buf,dst_node);

            length = 0;
            return;
        }
    }             
}

void CanPDOTransmitBufferWriteE502(WORD *p_tx_data, WORD func_code, WORD dst_node,WORD send_count )
{
//    WORD i;
//    WORD j;    
    WORD last_tx_buf[6] = {0};

    
    last_tx_buf[0] = ((func_code | dst_node));
    last_tx_buf[1] = 3 + 1;    	//length = 3                                     //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
		
    last_tx_buf[2] = (send_count << 8) | ( CAN_ID_ME );
    last_tx_buf[3] = ( *p_tx_data++ ); 
    last_tx_buf[4] = ( *p_tx_data++ ); 
    last_tx_buf[5] = ( *p_tx_data++ );   
	CanTxBufWrite(last_tx_buf,dst_node);

    return;
        
                 
}

void    TempMoldPDOTransmitBufferWrite(WORD *p_tx_data, WORD func_code, WORD dst_node, WORD send_count)
{
//  WORD i;
//    WORD j;    
    WORD last_tx_buf[6] = {0};
//    WORD ext_board_kind;
//    WORD ext_board_id;
//    
//    ext_board_kind = ( dst_node & 0x78 ) >> 3 ;
//    ext_board_id   = ( dst_node & 0x07 ); 

    last_tx_buf[0] = (func_code | dst_node);               //PUT INTO MSGID
    last_tx_buf[1] = 4;
    last_tx_buf[2] = (send_count << 8) | ( CAN_ID_ME ); 
    last_tx_buf[3] = ( *p_tx_data++ ); 
    last_tx_buf[4] = ( *p_tx_data++ ); 
    last_tx_buf[5] = ( *p_tx_data++ );   
    CanTxBufWrite(last_tx_buf,dst_node);
}

//----------------------------------------------------------------------------        
static void    
CanLowIOTransmit (void)
{
    WORD i;
    WORD func_code;
    WORD dst_node;
    WORD length;
    WORD send_count;
    WORD *p_tx_data;
       
    for ( i=0; i<LIO_MAX_NUM; i++ )
    {
       if ( (g_canliocomm[i].flag_send == true) )//&& (g_canliocomm[i].flag_wait == false)
                //&& (node_presence[CAN_LSPDIO_KIND][i] == true) )            //transmit condition
       {
            p_tx_data = g_canliocomm[i].p_origin_data;
            func_code = CAN_PDO_TX_CODE;                                    //get character value 
            length    = g_canliocomm[i].origin_data_length;
            dst_node  = ( CAN_LSPDIO_KIND << 3 ) | ( i );                    //object board node
            send_count= g_canliocomm[i].send_count++;
            
            CanSDOTransmitBufferWrite(p_tx_data, func_code, length, dst_node, send_count); //write PDO data to txbuff
            
            g_canliocomm[i].flag_send = false;
            //g_canliocomm[i].flag_wait = true;  //!!
            g_canliocomm[i].reply_time = A_100msclock;
            g_canliocomm[i].reply_count = 0;       
            g_canliocomm[i].node_state = BUS_BUSY;   
        }
        
        if ( (node_presence[CAN_LSPDIO_KIND][i] == true) && (g_canliocomm[i].flag_wait == true)
                && ( (WORD)(A_100msclock - g_canliocomm[i].reply_time) > 1) )
        {
            if ( g_canliocomm[i].reply_count < 3)
            {
                g_canliocomm[i].reply_count++;
                p_tx_data = g_canliocomm[i].p_origin_data;
                func_code = CAN_PDO_TX_CODE;                                    //get character value 
                length    = g_canliocomm[i].origin_data_length;
                dst_node  = ( CAN_LSPDIO_KIND << 3 ) | ( i );                    //object board node
                send_count= g_canliocomm[i].send_count;
                
                CanSDOTransmitBufferWrite(p_tx_data, func_code, length, dst_node, send_count); //write PDO data to txbuff
                
                g_canliocomm[i].reply_time = A_100msclock;
            }
            else
            {
                g_canliocomm[i].reply_count = 0;
                g_canliocomm[i].comm_err = true;
                g_canliocomm[i].flag_wait = false;
                g_canliocomm[i].node_state = BUS_FREE;   
            }
        }             
    }
}

static void  
CanServoTransmit(void)    
{
    WORD i;
    WORD func_code;
    WORD dst_node;
    WORD length;
    WORD send_count;
    WORD *p_tx_data;
	PPUMP_STRU_5 pp = &g_pump_5;
	
    for ( i=0; i<SERVO_MAX_NUM; i++ )
    {
       if ( (g_canservocomm[i].flag_send == true) )            //transmit condition
       {
            p_tx_data = g_canservocomm[i].p_origin_data;                                    //get character value 
            length    = g_canservocomm[i].origin_data_length;
            dst_node  = ( CAN_SERVO_KIND << 3 ) | ( i );                    //object board node
            send_count= 0;
			/*伺服5模式新增 2015-01-04*/
            if(!ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(XF6_CANSRV)&&pp->cantestmode == 0)
            {
				func_code = CAN_PDO2_TX_CODE;  
				CanSDOTransmitBufferWriteMode5(p_tx_data, func_code, length, dst_node); 
            }
			else
			{
				func_code = CAN_PDO_TX_CODE;
            	CanSDOTransmitBufferWrite(p_tx_data, func_code, length, dst_node, send_count);    
			}
            g_canservocomm[i].flag_send = false;
            g_canservocomm[i].node_state = BUS_BUSY;   
        }       
    }
}

//----------------------------------------------------------------------------   
//--------------------2008-12-26 13:28--------------------------------------------   
static void  
CanPanelTransmit(void)    
{
    WORD i;
    WORD func_code;
    WORD dst_node;
    WORD length;
    WORD send_count;
    WORD *p_tx_data;
       
    for ( i=0; i < PANEL_MAX_NUM; i++ )
    {
       if((g_canpanelcomm[i].flag_send == true) )            //transmit condition
       {
            p_tx_data = g_canpanelcomm[i].p_origin_data;
            func_code = CAN_PDO_TX_CODE;                                    //get character value 
            length    = g_canpanelcomm[i].origin_data_length;
            dst_node  = ( CAN_PANEL_KIND << 3 ) | ( i );                    //object board node
            send_count= 0;
            
            CanSDOTransmitBufferWrite(p_tx_data, func_code, length, dst_node, send_count); //write PDO data to txbuff
            
            g_canpanelcomm[i].flag_send = false;
            g_canpanelcomm[i].node_state = BUS_BUSY;   
        }   
   //-------- 2010-01-05   增加面板SDO发送，用于面板更新程序 -------         
       if ( (g_canpanelcomm[i].sdo_re_flag_send == true) )            //transmit condition
       {  
            p_tx_data = g_canpanelcomm[i].p_sdo_data;
            func_code = CAN_SDO_RE_CODE;                                    //get character value 
            length    = g_canpanelcomm[i].p_sdo_data_length;
            dst_node  = ( CAN_PANEL_KIND << 3 ) | ( i );                    //object board node
            send_count= 0;
            CanSDOTransmitBufferWrite(p_tx_data, func_code, length, dst_node, send_count); //write PDO data to txbuff
            
            g_canpanelcomm[i].sdo_re_flag_send = false;
 //           g_canpanelcomm[i].node_state = BUS_BUSY;
        }   
    }
} 


void CanExtTempMoldWrite(WORD id,WORD data0,WORD  data1,WORD  data2)
{
	g_cantempe501[id].tx_buf[0]=data0;
    g_cantempe501[id].tx_buf[1]=data1;
    g_cantempe501[id].tx_buf[2]=data2;
    g_cantempe501[id].p_origin_data_mw = g_cantempe501[id].tx_buf;
    g_cantempe501[id].flag_send_mw = true;
	TempMoldTransmit();
}

/*
------------------------------------------------------------------------------
|       CAN Transmit Start
------------------------------------------------------------------------------
*/
static void
CanTransmitStart(void)
{
    if ( (g_cantxbuf.p_fill_txbuf != g_canint.p_send_txbuf)
        && ( (g_cantxbuf.p_txbuf_end - g_canint.p_send_txbuf) < 6) )    //leave enough space to transmit, _fill_txbuf != _send_txbuf must should be write
    {
        g_canint.p_send_txbuf = g_cantxbuf.p_txbuf_start;
    } 
    
    if ( (g_cantxbuf.p_fill_txbuf < g_canint.p_send_txbuf)
        || ( (g_cantxbuf.p_fill_txbuf - g_canint.p_send_txbuf) >= 6) )
    {
		if(can_tx_disable != TRUE)
		{
	        CanTxFrameMailbox4();
	        g_canint.sending = true;
		}
		else
		{
			/*不发送*/
			g_canint.p_send_txbuf += 6;
		}
    }

}
/*
------------------------------------------------------------------------------
|       Initialize CAN TXBUF Function
------------------------------------------------------------------------------
*/
void InitCANTxBuf (void)
{
    g_cantxbuf.p_txbuf_start    = cantbl;
    g_cantxbuf.p_txbuf_end      = cantbl + CANTXBUFFSIZE;
    g_cantxbuf.p_fill_txbuf     = g_cantxbuf.p_txbuf_start;
    g_canint.p_send_txbuf       = g_cantxbuf.p_txbuf_start;  
    g_canint.sending            = false; 
        
}  
/*
------------------------------------------------------------------------------
|       Can Transmit Interrupt Function
------------------------------------------------------------------------------
*/
void CanTransmitInterrupt(void)
{
	cantx_flag = 0;				// whaul 2011/01/10

    if ( (g_cantxbuf.p_fill_txbuf != g_canint.p_send_txbuf)
        && ( (g_cantxbuf.p_txbuf_end - g_canint.p_send_txbuf) < 6) )    //leave enough space to transmit
    {
        g_canint.p_send_txbuf = g_cantxbuf.p_txbuf_start;
    } 
    if ( (g_cantxbuf.p_fill_txbuf < g_canint.p_send_txbuf)
        || ( (g_cantxbuf.p_fill_txbuf - g_canint.p_send_txbuf) >= 6) )
    {
        CanTxFrameMailbox4();
    }
    else
    {
        g_canint.sending = false;
    }
}
/*
------------------------------------------------------------------------------
|       Can Node Guarding TX Process Function
------------------------------------------------------------------------------
*/   
static void
CanNodeGuardTxProc (void)
{
    WORD i;
    WORD j;
    static WORD steptime = 0;
    WORD func_code;
    WORD dst_node;
    WORD length;
    WORD send_count;
    WORD board_kind;
    WORD *p_data;
    static WORD okflag = 0;
	
    if ( (WORD)(A_10msclock - steptime) > 10) 
    {
        steptime = A_10msclock;
        for ( j=0; j<BOARD_KIND_MAX; j++)
        {
            for ( i=0; i<BOARD_NUM_MAX;i++)
            {
                if (node_presence[j][i] == true)
                {
                    p_data      = 0;    //不起作用，但是要写，可以是任意的
                    func_code   = CAN_NMT_CTRL_CODE;                                   
                    length      = 1;    //不起作用，但是要写，
                    send_count  = 0xAA; //node guarding code
                    switch (j)
                    {
						case 0xc:
							if(i > g_tempcomm_num || !ChkTempMachTypeE50X())		//code代码未打时，不发心跳，防止出现can线插上，502的run和talk却交替起来的现象
							{
								break;
							}
							board_kind = CAN_TEMPE502_KIND;
							g_cantempe502comm[i].node_guarding_count++;
							dst_node    = ( board_kind << 3 ) | ( i );  
                   	 		CanSDOTransmitBufferWrite(p_data, func_code, length, dst_node, send_count); 
							break;
							
                        default :    
						break;  
                    }
                }
            }
        }
    }   


	totalnum = 0;
	for(i = 0;i < g_tempmw_num;i++)	//该函数主要为了找出握手成功的模温板数目
	{
		if(((mw_piece >> i) & 0x01) == 0x01)
			totalnum++;
	}

	if(okflag == 0)
	{
		for(i = 0;i < g_tempmw_num;i++)	//该函数主要为了刚开机时，在有任意一块扩展板连接成功后，再等待3s钟后再开始发送心跳，保证其他几块扩展板的设定参数可以正常发送完毕，按照一块扩展板0.5s计算
		{
			if((can_node_E501_check[i+1] == true)&& (g_cantempe501[i+1].TempMold_node_state == 0)
				&& ((UI16)(Get10MsTick() - g_cantempe501[i+1].node_gard_time) > 300))
			{
				okflag = 1;
			}
		}
	}

	if(totalnum > 0 && (okflag == 1))	//实际在使用的模温块数不为0，同时一块板子所有设定参数发送完毕并且延时3s钟后
	{
		mw_hearttm = MW_HEART / totalnum;	//按照最大7块模温板计算，每隔1000/7=142ms给下一块模温板发送心跳

		if(sendnum >= g_tempmw_num)
			sendnum = 0;
		
		if(delt_time(A_1msclock,mw_scantm) > mw_hearttm)	//每过固定时间，轮训到下一块模温扩展板去发送心跳
		{
			mw_scantm = (WORD)A_1msclock;
			if((can_node_E501_check[sendnum+1] == true)&& (g_cantempe501[sendnum+1].TempMold_node_state == 0) && (node_presence[CAN_HSPDTEMP_KIND][sendnum+1] == true)
					&& (g_temp_mw[sendnum]->md->FL_USE_MW == 1))
			{
				p_data      = 0;    //不起作用，但是要写，可以是任意的
	            func_code   = CAN_NMT_CTRL_CODE;                                   
	            length      = 1;    //不起作用，但是要写，
	            send_count  = 0xAA; 
				board_kind = CAN_HSPDTEMP_KIND;
				//g_cantempe501[sendnum+1].node_guarding_e501++;
				dst_node    = ( board_kind << 3 ) | ( sendnum + 1);  
	            CanSDOTransmitBufferWrite(p_data, func_code, length, dst_node, send_count); 
			} 
			
			if(can_node_E501_check[sendnum+1] == true)
				g_cantempe501[sendnum+1].node_guarding_e501++;
			sendnum++;
			for(i = sendnum; i < g_tempmw_num;i++)		//寻找出下一块连接上的模温板
			{
				if(((mw_piece >> i) & 0x01) == 0x01)
				{
					sendnum = i;
					break;
				}
				else
				{
					sendnum = 0;	   //一轮都查询不到时，直接从第一块开始轮训
				}
			}

			if(sendnum == 0)	//当sendnum值为0的时候，表示后续的模温板都没有连接上，此时需要从头开始扫描，找到已经连上的那一块模温板，防止多跑一次，导致发心跳的时间超出1s
			{
				for(i = 0; i < g_tempmw_num;i++)
				{
					if(((mw_piece >> i) & 0x01) == 0x01)
					{
						sendnum = i;
						break;
					}
				}
			}
		}
	}
}

static void  CanTempE502Transmit(void)    
{
    WORD i;
    WORD func_code;
    WORD dst_node;
    WORD send_count;
    WORD *p_tx_data;

    for ( i=0; i<TEMP_MAX_NUM; i++ )
    {
       if (g_cantempe502comm[i].flag_send == true )            //transmit condition
       {
            p_tx_data = g_cantempe502comm[i].p_origin_data;                                    //get character value 
			func_code = CAN_PDO_TX_CODE;
			dst_node  = ( CAN_TEMPE502_KIND << 3 ) | ( i );                    //object board node
			
			send_count=g_cantempe502comm[i].send_count++;
            CanPDOTransmitBufferWriteE502(p_tx_data, func_code, dst_node,send_count);    
			
            g_cantempe502comm[i].flag_send = false;
        }       
    }
}



/*******************************************************************************************
  * @函数名称	CanTempE502_KJ_Transmit(void)
  * @函数说明 发送给E502用于选择K型和J型热电偶
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  * @作者       HH
  * @日期       20190708

修改历史:
1、	修改日期:2019/10/09
	修改作者:周拓
	修改内容:将单块E502形式改为多块的
 ****************************************************************************************/
void CanTempE502_KJ_Transmit()
{	
	PTEMPCTRLCOMM pp;
	UI16 i = 0;
	for(i = 0; i < g_tempcomm_num; i++)
	{
		pp = g_tempcomm[i];
		
		if((pp != NULL) && (pp->K_J_Varuety > 0))
		{
			if (delt_time(A_10msclock, pp->clock_kj) > 20)
			{
				pp->clock_kj = A_10msclock;
				g_cane502txdata[pp->temp_id].cmd = E502_TEMP_DATA;
				g_cane502txdata[pp->temp_id].wtemp_channel_adjall = (((pp->st->WTEMP_CHANNEL & 0xff) << 8) | (pp->st->WTEMP_ADJALL & 0xff));
				TempE502DataPdoWrite(pp->temp_id, g_cane502txdata[pp->temp_id].cmd, g_cane502txdata[pp->temp_id].wtemp_channel_adjall, pp->Used_KJ);		
				pp->clock_kj_cn++;
			}

			if (pp->clock_kj_cn > 25)
			{
				pp->clock_kj_cn = 0;
				pp->K_J_Varuety = 0;
				SET_PART_ERR(pp,ER5_E502_J_INVALID);
			}
		}
	}
	

}

static void  CanTempE501_HAND_SET(UI16 num)   //E501握手命令发送
{
	WORD func_code;
	WORD dst_node;
	WORD length;
	WORD send_count;
	WORD board_kind;
	WORD *p_data;

	p_data      = 0;                     //不起作用，但是要写，可以是任意的
	func_code   = CAN_NMT_CTRL_CODE;                                   
	length      = 1;                     //不起作用，但是要写，
	send_count  = 0x55; 
	board_kind  = CAN_HSPDTEMP_KIND; 
	dst_node = ( board_kind << 3 ) | ( num + 1);	//此处需要加1，因为num进来的是从0开始的
	CanSDOTransmitBufferWrite(p_data, func_code, length, dst_node, send_count);
}


static void CanNodehandshake()
{
	static WORD steptime = 0;
    WORD i;

	if(g_tempmw_num > 0)
	{
		if((WORD)(A_10msclock - steptime) > 10)		//每隔100ms群发一遍握手命令，一块扩展板一帧握手信号数据，所以此处采用群发
		{
			steptime = A_10msclock;
			for(i=0;i<g_tempmw_num;i++)
			{
				PTEMPCTRLMW ptempmw = g_temp_mw[i];
				if(i<g_tempmw_num && can_node_E501_check[i+1] == FALSE && (ptempmw != NULL) && (ptempmw->md->FL_USE_MW == 1))
				{
					CanTempE501_HAND_SET(i); 
				}
			}
		}

		for(i=0;i<g_tempmw_num;i++)
		{
			if(ComIsOk() && g_cantempe501[i+1].TempMold_node_state == 1)	//当握手完成同时通讯正常后，开始交互所以的设定数据，每块扩展板间至少间隔50ms再发送数据，防止一次性发送数据过多
			{
				PTEMPCTRLMW ptempmw = g_temp_mw[i];
				TempMoldSetData(ptempmw,i);
				break;				//当扫描到一块501扩展板时，后续的就不去扫面，先把当前这块需要的所有数据发完，再去检测后面的扩展板   20200117
			}
		}
	}
}

static void  
TempMoldTransmit(void)    //模温扩展板数据写can缓冲
{
    WORD i;
    WORD func_code;
    WORD dst_node;
//    WORD length;
    WORD send_count;
    WORD *p_tx_data;
		
    for (i=0; i<g_tempmw_num; i++ )
    {
        if (g_cantempe501[i+1].flag_send_mw == true)
        {
	        p_tx_data = g_cantempe501[i+1].p_origin_data_mw;
	        func_code = CAN_PDO_TX_CODE;
	        dst_node  = ( CAN_HSPDTEMP_KIND << 3 ) | ( i + 1);	//加 1，给扩展板发送数据时，板号从1开始
	        send_count= g_cantempe501[i+1].send_count_mw++;
	        
	        TempMoldPDOTransmitBufferWrite(p_tx_data, func_code,dst_node, send_count); //write PDO data to txbuff
	                                       
	        g_cantempe501[i+1].flag_send_mw = false;
        }
    }
}

/*
------------------------------------------------------------------------------
|       CAN TRANSMIT Function
------------------------------------------------------------------------------
*/
void CanTransmitProcess(void)
{
	CanServoTransmit();
    CanLowIOTransmit();

	CanTempE502_KJ_Transmit();//发送热电偶类型20190708HH

	CanTempE502Transmit();
    E702_CanTransmit();//======设置发送到E702的数据
    CanPanelTransmit(); 
    CanTransmitCheck(); 
	CanNodehandshake();		//主机主动握手函数
	TempMoldTransmit();
    if ( g_canint.sending == false )
    {
        CanTransmitStart();
    }

	CanNodeGuardTxProc();
}
    
void CanDisableTx(BOOL bdisable)
{
	if(bdisable == TRUE)
	{
		can_tx_disable = TRUE;
	}
	else
	{
		can_tx_disable = FALSE;
	}
}

