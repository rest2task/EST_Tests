#include "canhead.h"
//#include "IL3.h"
#include "memtbl.h"
#include "printf.h"
#include "systempro.h"
#include "string.h"
#include "can_e702.h"
#include "servo.h"
#include "machapi.h"
#include "TempCtrlMw.h"
#include "ServFive.h"


/*
------------------------------------------------------------------------------
|       Global Variant  
------------------------------------------------------------------------------
*/
WORD        pick_rx_buf[6] = {0};                             // can pick rx buffer
extern UI32 can_rx_count;
extern BOOL can_node_E501_check[TEMPMW_MAX_NUM];
extern DB_TEMPMOLD g_cantempe501[TEMPMW_MAX_NUM];


//----------------------------------------------------------------------------
static void CanPDOReplyWaitingProcess(void);
static void CanSDOReplyWaitingProcess(void);
static void CanPDOReceiveProcess(void);
static void CanSDOReceiveProcess(void);        //专为CAN程序下载用SDO
static void CanNodeGuardRxProc(void);
void SetServoSpeed(UI16 portno, UI16 speed);
BOOL SendMonAlarmSt(UI16 imm);

/*
------------------------------------------------------------------------------
|       Static Functions
------------------------------------------------------------------------------
*/

//----------------------------------------------------------------------------
static  void
CanPDOReplyWaitingProcess(void)
{          
    WORD ext_kind;
    WORD ext_board_id;
    ext_kind = ( (LOBYTE(pick_rx_buf[2]) & 0x78) >> 3);
    ext_board_id = (LOBYTE(pick_rx_buf[2]) & 0x07);
                   
    switch ( ext_kind )
    {
        case CAN_LSPDIO_KIND    :
			if(ext_board_id < ARRAY_NUM(g_canliocomm))
			{
				g_canliocomm[ext_board_id].reply_count = 0;
				g_canliocomm[ext_board_id].flag_wait = false;    
				g_canliocomm[ext_board_id].node_state = BUS_FREE;
			}
			break;
                
        case CAN_DA_KIND        :
			if(ext_board_id < ARRAY_NUM(g_candacomm))
			{
                g_candacomm[ext_board_id].reply_count = 0;
                g_candacomm[ext_board_id].flag_wait = false;
                g_candacomm[ext_board_id].node_state = BUS_FREE;
			}
            break;     
		
		case CAN_SERVO_KIND 	   :
			if((ext_board_id < ARRAY_NUM(g_canservocomm)) && (LOBYTE(pick_rx_buf[0] & 0x07f) == CAN_ID_ME))
			{
//				UI16 ack_tm;
				
				//CanServoInovanceReceive,汇川伺服的接收部分 2008-11-4 13:21
				g_canservocomm[ext_board_id].pdo_rx_data[0] = pick_rx_buf[3];	 //命令值
				g_canservocomm[ext_board_id].pdo_rx_data[1] = pick_rx_buf[4];	 //实际转速
				g_canservocomm[ext_board_id].pdo_rx_data[2] = pick_rx_buf[5];	 //故障状态 
				g_canservocomm[ext_board_id].node_state = BUS_FREE; 

				/*board_id编号从1开始，伺服编号从0开始*/
				if(ext_board_id > 0 && (ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(XF6_CANSRV)))
					SetServoSpeed(ext_board_id - 1, pick_rx_buf[4]);
			}
			break;
			
       case CAN_PANEL_KIND          :
			if(ext_board_id < ARRAY_NUM(g_canpanelcomm))
                //简易的面板接收部分 目前不需要回复过来得数据 2008-12-26 13:35
                g_canpanelcomm[ext_board_id].node_state = BUS_FREE;   
                break;
                
        default :
                break;
    }
}     

static  void
CanPDO2ReceiveProcess(void)
{          
    WORD ext_kind;
    WORD ext_board_id;
	PSERVO	ps = (PSERVO)GetPartByID(MK_PART(SERVO_ID, 1));
	PPUMP_STRU_5 pp = &g_pump_5;

    ext_board_id = (LOBYTE(pick_rx_buf[3]) & 0x07); 
	ext_kind   = ((LOBYTE(pick_rx_buf[0])&0x78)>>3);
    if(!ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(XF6_CANSRV)&&(pp->cantestmode == 0))	
    	SWI_HIB_LOB(pick_rx_buf[2]);
	
    switch ( ext_kind )
    {
    	case CAN_CPU_KIND://运行监视状态，读取数据字典5021的运行状态值
			if(ext_board_id < ARRAY_NUM(g_canservocomm))
			{
				/*模式5的应答包格式
				字段  ID10~7  ID6~0                     Word0_Byte0                  Word0_Byte1           Word1          Word2       Word3
				含义  1010    CPU板ID                     伺服板ID                    伺服警报           压力反馈       速度反馈     电流反馈
				*/
				switch(((pick_rx_buf[2]& 0xE000)>>13))
				{
					case RUNSTATE:    //运行状态监控回复
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[0] =((pick_rx_buf[2]& 0xE000)>>13);               //扩展功能命令和扩展功能响应，高三位表示扩展功能码
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[1] = ((pick_rx_buf[2]& 0x1F00)>>8);                    //监控状态的索引号
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[2] = (((pick_rx_buf[2]<<8)|(pick_rx_buf[3]>>8)));  //回复的监控状态值
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[3] = (pick_rx_buf[3]&0x00FF);                     //状态及驱动器ID
                		g_canservocomm[ext_board_id].pdo_rx_mode5_data[4] = pick_rx_buf[4];                              //压力值
                		g_canservocomm[ext_board_id].pdo_rx_mode5_data[5] = pick_rx_buf[5];                              //转速	
                		g_canservocomm[ext_board_id].node_state = BUS_FREE;        
						break;
					case PARAREAD:   //数据读取回复
						break;
					case PARAWRITE:  //数据写回复
						break;
					case PARAPISET:  //PI参数设定回复
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[0] =((pick_rx_buf[2]& 0xE000)>>13);               //扩展功能命令和扩展功能响应，高三位表示扩展功能码			
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[1] = ((pick_rx_buf[2]& 0x0FFC)>>2);               //P值						
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[2] = (((pick_rx_buf[2]&0x0003)<<8)|(pick_rx_buf[3]>>8));//I值
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[3] = (pick_rx_buf[3]&0x00FF);                     //状态及驱动器ID						
                		g_canservocomm[ext_board_id].pdo_rx_mode5_data[4] = pick_rx_buf[4];                              //压力值
                		g_canservocomm[ext_board_id].pdo_rx_mode5_data[5] = pick_rx_buf[5];                              //转速	
                		g_canservocomm[ext_board_id].node_state = BUS_FREE;        						
						break;		
					case CHGCOMPARA://从泵异常通知回复，主机不走这个协议
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[0] =((pick_rx_buf[2]& 0xE000)>>13);               //扩展功能命令和扩展功能响应，高三位表示扩展功能码
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[1] = ((pick_rx_buf[2]& 0x1F00)>>8);               //主动上发通知类型  0:未准备  1:运行   2:保留  3:准备就绪
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[2] = (((pick_rx_buf[2]<<8)|(pick_rx_buf[3]>>8)));  //主动上发数据值
						g_canservocomm[ext_board_id].pdo_rx_mode5_data[3] = (pick_rx_buf[3]&0x00FF);                     //状态及驱动器ID
                		g_canservocomm[ext_board_id].pdo_rx_mode5_data[4] = pick_rx_buf[4];                              //压力值
                		g_canservocomm[ext_board_id].pdo_rx_mode5_data[5] = pick_rx_buf[5];                              //转速	
                		g_canservocomm[ext_board_id].node_state = BUS_FREE;        
						//pp->st->SERVO_ST[ext_board_id-1] = (pp->st->SERVO_ST[ext_board_id-1]|(((g_canservocomm[ext_board_id].pdo_rx_mode5_data[1]>>(ext_board_id-1))&1)<<7));
						ps->st->SERVO_ST[ext_board_id-1] = ((ps->st->SERVO_ST[ext_board_id-1]&0xFFF8)|g_canservocomm[ext_board_id].pdo_rx_mode5_data[1]);
						ps->st->SERVO_ERR[ext_board_id-1] = g_canservocomm[ext_board_id].pdo_rx_mode5_data[2];
						break;		
					default:
						break;							
				}
			}
            break;			
    	default :
            break;
    }
	pp->servocomst100us[ext_board_id] = Get100UsTick();
}  

//----------------------------------------------------------------------------  
static  void
CanSDOReplyWaitingProcess(void)
{
    WORD ext_kind;
    //WORD ext_board_id;
    ext_kind = ( (LOBYTE(pick_rx_buf[2]) & 0x78) >> 3);
    //ext_board_id = (LOBYTE(pick_rx_buf[2]) & 0x07);
    
    switch ( ext_kind )
    {
        default :
               break;
    }
}   

//----------------------------------------------------------------------------  
static  void
CanPDOReceiveProcess(void)
{
	PIO pio = (PIO)GetPartByID(MK_PART(IO_ID, 1));
	// WORD i;
    // WORD temp_adress;
    BOOL flag_valid = false;
    WORD ext_kind;
    WORD ext_board_id;
    WORD dst_node;
    WORD last_tx_buf[6] = {0};  
	UI16 TempHoldErr_st = 0;
    
    dst_node = ( LOBYTE(pick_rx_buf[2]) & 0x7F );
    ext_kind = ( (dst_node & 0x78) >> 3);
    ext_board_id = ( dst_node & 0x07 );
        
    switch ( ext_kind )
    {
        case CAN_LSPDIO_KIND    :
 			if(ext_board_id < ARRAY_NUM(g_canliocomm))
 			{
                g_canliocomm[ext_board_id].input_state = pick_rx_buf[3];

				if(pio != NULL && ext_board_id < LIO_MAX_NUM)
				{
					pio->IO_LossPres[ext_board_id] = ((pick_rx_buf[4] >> 8) & 0xff);	//IO板24v电压信号，0代表异常，电压太低，1代表正常
					pio->IO_Overcurrent[ext_board_id] = (pick_rx_buf[4] & 0xff);	//IO板过流标记，0代表过流 ，1代表正常

					///新增IO扩展板版本号
					if (ext_board_id > 0 && ext_board_id < 4)
					{
						GetSys()->st->IO_E40xB_VER[ext_board_id-1] = (pick_rx_buf[5] & 0xff);
					}
				}
 			}
			flag_valid = true;
            break;
			
        case CAN_DA_KIND:    
 			if(ext_board_id < ARRAY_NUM(g_candacomm))
 			{
                g_candacomm[ext_board_id].rx_buf[0] = pick_rx_buf[3];    //扩展AD第一路
                g_candacomm[ext_board_id].rx_buf[1] = pick_rx_buf[4];    //扩展AD第二路     
 			}            
                flag_valid = true;
                break;   
				
        case CAN_PANEL_KIND        : 
                //目前测试通讯，不需要数据2008-12-26 13:59 
                //db_printf("C %d\r\n", can_rx_count);
                if(LOBYTE(pick_rx_buf[0] & 0x07f) == CAN_ID_ME)
                {
					can_rx_count++;
	                flag_valid = true;
                }
				break;
				
		case CAN_TEMPE502_KIND:
			if(g_tempcomm_num >= ext_board_id)
			{
				PTEMPCTRLCOMM ptempcom = g_tempcomm[ext_board_id - 1];
				
				if(((pick_rx_buf[3]>>8)&0xff)==0x01)
				{
					g_cantempe502comm[ext_board_id].TempFlag = (((pick_rx_buf[3]&0x00c0)>>6)&0x03);
				  	g_cantempe502comm[ext_board_id].CurChnl = (pick_rx_buf[3]&0x3f);
					g_cantempe502comm[ext_board_id].TestTemp = pick_rx_buf[4];
					g_cantempe502comm[ext_board_id].RealTemp = pick_rx_buf[5];
				}
				else if(((pick_rx_buf[3]>>8)&0xff)==0x02)
				{
					g_cantempe502comm[ext_board_id].wtemp_advalue = pick_rx_buf[4];
					g_cantempe502comm[ext_board_id].wtemp_real = pick_rx_buf[5];
				}
				else if(((pick_rx_buf[3]>>8)&0xff)==0x03)
				{
					g_cantempe502comm[ext_board_id].wtemp_lm73 = pick_rx_buf[4];
					if(pick_rx_buf[5] == 0x66)//收到E502回复信号，清零标记20190708HH
					{
						 ptempcom->K_J_Varuety = 0;
					}
				}
				else if(((pick_rx_buf[3]>>8)&0xff)==E502_TEMP_TEST)		
				{
					if(ptempcom != NULL)
					{
						g_cantempe502comm[ext_board_id].node_guarding_count = 0;
						if(ptempcom->can_e502_test_flag == 1)
						{
							ptempcom->st->READ_CNT_E502++;
							ptempcom->can_e502_test_flag = 0;
						}
					}
				}
				else if(((pick_rx_buf[3]>>8)&0xff)==E502_TEMP_VER)	
				{
					if(ptempcom != NULL)
					{					
						g_cantempe502comm[ext_board_id].wtemp_ver = pick_rx_buf[4];
						ptempcom->st->DATEMP_E502_VER = g_cantempe502comm[ext_board_id].wtemp_ver; 
						PartDataTx((PPART)ptempcom, ST_ID, 2, GET_IND(PTEMPCTRLCOMM_STATE, DATEMP_E502_VER), FALSE);
					}
				}
			}
			break;

		case CAN_HSPDTEMP_KIND:
			if(ext_board_id <= g_tempmw_num)
			{
				PTEMPCTRLMW pmw = g_temp_mw[ext_board_id - 1];
				g_cantempe501[ext_board_id].rx_buf[0] = pick_rx_buf[3];   //命令字
	            g_cantempe501[ext_board_id].rx_buf[1] = pick_rx_buf[4];   //第几段温度值 
	            g_cantempe501[ext_board_id].rx_buf[2] = pick_rx_buf[5];   //当前这一段温度的温度值

				if(pmw != NULL)
				{
					switch(g_cantempe501[ext_board_id].rx_buf[0])
					{
						case TEMPMOLD_REAL:
							*(pmw->st->DATEMP_REAL_MW + g_cantempe501[ext_board_id].rx_buf[1]) = g_cantempe501[ext_board_id].rx_buf[2];
							break;

						case TEMPMOLD_STATUS:
							*(pmw->st->FL_STATUS_MW + g_cantempe501[ext_board_id].rx_buf[1]) = g_cantempe501[ext_board_id].rx_buf[2];
							break;

						case TEMPMOLD_MW_BL:
							*(pmw->st->OUTPUT_PER_MW + g_cantempe501[ext_board_id].rx_buf[1]) = g_cantempe501[ext_board_id].rx_buf[2];
							break;
							
						case TEMPMOLD_MW_SW:
							pmw->st->WTEMP_LM73_MW = g_cantempe501[ext_board_id].rx_buf[2];
							break;
						case TEMPMOLD_MW_DR:
							if(pmw->st->FL_HEATERSTATUS_MW == 1 && ((WORD)(Get10MsTick() - pmw->mw_heat_starttm) > (MW_HEATONOFF *10)))
							{
							    //&& (g_cantempe501[ext_board_id].rx_buf[2] == 0))		//电热开启过一段时间后再检测发送上来的时时电热状态，变为0时，清除面板上的加热状态
								//pmw->st->FL_HEATERSTATUS_MW = g_cantempe501[ext_board_id].rx_buf[2];

								if (g_cantempe501[ext_board_id].rx_buf[2] == 0) //扩展板不能切主机状态
								{
									pmw->mw_heat_statue = 1;
									pmw->mw_heat_starttm = Get10MsTick();
								}
							}
                            break;

						case TEMPMOLD_VER:
							pmw->st->DATEMP_VER_MW = g_cantempe501[ext_board_id].rx_buf[2];
							break;
							
						case TEMPMOLD_ERR:	//模温扩展板发送上来的警报状态，高16位err1和低16位的err0，再重新组合和上传给面板
							TempHoldErr_st = pmw->st->FL_ERROR0 & ER0_TMPHLD_MW;  //模温取消保温模式警报由主机控制，需要屏蔽
							if(pmw->md->FL_USE_MW == 1)	 			//模温板选择使用时，才上传该块板子的警报
							{
								pmw->st->FL_ERROR0 = g_cantempe501[ext_board_id].rx_buf[1];
								//pmw->st->FL_ERROR1 = g_cantempe501[ext_board_id].rx_buf[2];
								//SET_PART_ERR(pmw,(((UI32)g_cantempe501[ext_board_id].rx_buf[2] << 16) + (UI32)g_cantempe501[ext_board_id].rx_buf[1]));
								SendMonAlarmSt(1);
							}
							else
							{
								pmw->st->FL_ERROR0 = 0;
								//pmw->st->FL_ERROR1 = 0;
							}

							pmw->st->FL_ERROR0 |= TempHoldErr_st;
							break;
							
						case TEMPMOLD_WARM:
							break;
							
						case TEMPMOLD_READ:
							g_cantempe501[ext_board_id].node_guarding_e501 = 0;
							pmw->st->READ_CNT_E501++;
							break;
							
						case TEMPMOLD_YHFH:
							pmw->st->MW_YOUHUAXUQIU = g_cantempe501[ext_board_id].rx_buf[2];
							break;
						case TEMPMOLD_YHOK:
							pmw->st->MW_YOUHUAOK = g_cantempe501[ext_board_id].rx_buf[2];
							break;	

						case TEMPMOLD_MW_P:
							*(pmw->md->DAPID_KP_MW + g_cantempe501[ext_board_id].rx_buf[1]) = g_cantempe501[ext_board_id].rx_buf[2];
							PartDataTx((PPART)pmw, MD_ID, 1, GET_IND(PTEMPMW_MOLDSET, DAPID_KP_MW) + g_cantempe501[ext_board_id].rx_buf[1], FALSE);
							break;
						case TEMPMOLD_MW_I:
							*(pmw->md->DAPID_TI_MW + g_cantempe501[ext_board_id].rx_buf[1]) = g_cantempe501[ext_board_id].rx_buf[2];
							PartDataTx((PPART)pmw, MD_ID, 1, GET_IND(PTEMPMW_MOLDSET, DAPID_TI_MW) + g_cantempe501[ext_board_id].rx_buf[1], FALSE);
							break;
						case TEMPMOLD_MW_D:
							*(pmw->md->DAPID_TD_MW + g_cantempe501[ext_board_id].rx_buf[1]) = g_cantempe501[ext_board_id].rx_buf[2];
							PartDataTx((PPART)pmw, MD_ID, 1, GET_IND(PTEMPMW_MOLDSET, DAPID_TD_MW) + g_cantempe501[ext_board_id].rx_buf[1], FALSE);
							break;
						case TEMPMOLD_MW_RAMP:
							*(pmw->md->DATEMP_RAMP_MW + g_cantempe501[ext_board_id].rx_buf[1]) = g_cantempe501[ext_board_id].rx_buf[2];
							PartDataTx((PPART)pmw, MD_ID, 1, GET_IND(PTEMPMW_MOLDSET, DATEMP_RAMP_MW) + g_cantempe501[ext_board_id].rx_buf[1], FALSE);
							break;
							
							default:
								break;
					}
				}
				//flag_valid = true;		//暂时先去除数据的回复包，防止数据过多，占通信
			}
			break;
			
        default :
                flag_valid = false;
                break;
    }
	
    if ( flag_valid == true)
    {    
       	last_tx_buf[0] = ( (CAN_PDO_RE_CODE | dst_node));
        last_tx_buf[1] = 1;                                             //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
        last_tx_buf[2] = ( HIBYTE(pick_rx_buf[2]) << 8) | ( CAN_ID_ME );
            
        CanTxBufWrite(last_tx_buf,dst_node);
        
        flag_valid = false;
    }
} 
/*------------------------------------  
 * 函数名称：CanSDOReceiveProcess
 * 功    能：CAN更新之接收处理
 * 版 本 号: 2010-01-05  CH
-------------------------------------*/
static  void
CanSDOReceiveProcess(void)
{
    WORD i;
    WORD ext_kind;
    WORD ext_board_id;
    WORD dst_node;
    
    dst_node = ( LOBYTE(pick_rx_buf[2]) & 0x7F );
    ext_kind = ( (dst_node & 0x78) >> 3);
    ext_board_id = ( dst_node & 0x07 );
   
    switch ( ext_kind )
    {        
        case CAN_PANEL_KIND        :
				if(ext_board_id >= ARRAY_NUM(g_canpanelcomm))
					break;
				
                for ( i=3;i<6;i++ )     //从pick_rx_buf[3:5]中取数据,放到sdo_rx_buf
                {
                    g_canpanelcomm[ext_board_id].sdo_rx_buf[g_canpanelcomm[ext_board_id].sdo_rx_task] = pick_rx_buf[i];    
                    g_canpanelcomm[ext_board_id].sdo_rx_task++;
                    if (g_canpanelcomm[ext_board_id].sdo_rx_task == MAX_PLANEL_SDO_RX_BUFFER)
                    {
                        g_canpanelcomm[ext_board_id].sdo_rx_task = 0;
                    }
                }

				
				getdatafromwcan();
                break;
        default :
                break;
    }
}    
   
/*
------------------------------------------------------------------------------
|       Initialize CAN RXBUF Function
------------------------------------------------------------------------------
*/
void InitCANRxBuf (void)
{
    g_canrxbuf.p_rxbuf_start    = canrbl;
    g_canrxbuf.p_rxbuf_end      = canrbl + CANRXBUFFSIZE;
    g_canrxbuf.p_pick_rxbuf     = g_canrxbuf.p_rxbuf_start;
    g_canint.p_receive_rxbuf    = g_canrxbuf.p_rxbuf_start;  
}    
/*
------------------------------------------------------------------------------
|       Can Receive Interrupt Function
------------------------------------------------------------------------------
*/
void CanReceiveInterrupt(void)
{
    if ( (g_canrxbuf.p_rxbuf_end - g_canint.p_receive_rxbuf) < 6)    //leave enough space to transmit
    {
        g_canint.p_receive_rxbuf = g_canrxbuf.p_rxbuf_start;
    } 
    
    CanRxFrameMailbox0();
}
/*
------------------------------------------------------------------------------
|       Can Node Guard Rx Function
------------------------------------------------------------------------------
*/
void CanNodeGuardRxProc(void)   
{ 
    WORD ext_kind;
    WORD ext_board_id;
//	WORD type;
	UI16 i,comd_r,num_r,data_r;
	
   if ( HIBYTE(pick_rx_buf[2]) == 0xAA)
    {
        ext_kind = ( (LOBYTE(pick_rx_buf[2]) & 0x78) >> 3);
        ext_board_id = (LOBYTE(pick_rx_buf[2]) & 0x7);

		switch (ext_kind)
            {
				case CAN_TEMPE502_KIND:	//主机发送一个心跳包，等接受到后，则赋值0
					g_cantempe502comm[ext_board_id].node_guarding_count = 0;
					break;

				case CAN_HSPDTEMP_KIND:
					g_cantempe501[ext_board_id].node_guarding_e501 = 0;
					break;
					
                default :    
					break;  
            }               
    } 


	if(HIBYTE(pick_rx_buf[2]) == 0xBB)		//模温设定数据回复包检测函数
    {
        ext_kind = ( (LOBYTE(pick_rx_buf[2]) & 0x78) >> 3);
        ext_board_id = (LOBYTE(pick_rx_buf[2]) & 0x7);

		switch (ext_kind)
        {
			case CAN_HSPDTEMP_KIND:
				comd_r = pick_rx_buf[3];   //命令字
            	num_r = pick_rx_buf[4];   //第几段温度值 
            	data_r = pick_rx_buf[5];   //当前这一段温度的温度值

				if(comd_r != 0)
				{
					PTEMPCTRLMW pmw = g_temp_mw[ext_board_id - 1];
					if(pmw != NULL)
					{
						for(i=0;i<MWNUM;i++)		//循环检测缓存中的各个值，当命令字，参数值都相等时，对该变量不再检测和重发
						{
							if((pmw->mw_buf[i].ext_command == comd_r) && (pmw->mw_buf[i].ext_num_set == num_r)
								&& (pmw->mw_buf[i].ext_data_set == data_r))
								pmw->mw_buf[i].ext_begin_flag = 0;
						}
					}
				}
				break;
				
            default :    
				break;  
        }               
    } 
}
/*
------------------------------------------------------------------------------
|       CAN RECEIVE Function
------------------------------------------------------------------------------
*/
void CanReceiveProcess(void)
{
    WORD i;     
    //tst---
    WORD ext_kind;
    WORD ext_board_id;
    WORD dst_node;  	
    // ---tst      
    while (1)
    {
        if ( (g_canint.p_receive_rxbuf != g_canrxbuf.p_pick_rxbuf)
            && ( (g_canrxbuf.p_rxbuf_end - g_canrxbuf.p_pick_rxbuf) < 6) )    //leave enough space to transmit
        {
            g_canrxbuf.p_pick_rxbuf = g_canrxbuf.p_rxbuf_start;
        } 
        
        if ( (g_canint.p_receive_rxbuf >= g_canrxbuf.p_pick_rxbuf)
            && ( (g_canint.p_receive_rxbuf - g_canrxbuf.p_pick_rxbuf) < 6) )
        {
            break;
        }
        
        for ( i=0; i<6; i++)
        {
            pick_rx_buf[i] = *g_canrxbuf.p_pick_rxbuf++;
        }
        
	    //tst--
        dst_node = ( LOBYTE(pick_rx_buf[2]) & 0x7F );
        ext_kind = ( (dst_node & 0x78) >> 3);
        ext_board_id = ( dst_node & 0x07 );

        if ((ext_kind != CAN_DA_KIND) && ((pick_rx_buf[0] & 0x0780) != CAN_PDO2_TX_CODE) && (pick_rx_buf[0] != 0x331))	//此处屏蔽伺服的数据，防止出现伺服发送PI时，数据会与其他扩展板的板号凑上   20200630     yxh
        {
            node_presence[ext_kind][ext_board_id] = true;		
        }
		
		if(((pick_rx_buf[0] & 0x0780) != CAN_PDO2_TX_CODE) && (pick_rx_buf[0] != 0x331))	//此处屏蔽伺服的数据，防止出现伺服发送PI时，数据会与其他扩展板的板号凑上   20200630     yxh
		{
			node_oncheck[ext_kind][ext_board_id] = true;      
		}
		
        switch ( pick_rx_buf[0] & 0x0780 )
        {
            case CAN_NMT_CTRL_CODE :
            		if (ext_kind == CAN_DA_KIND)
            		{
            			if ( HIBYTE(pick_rx_buf[2]) == 0x55)								//DA扩展板发送给主机的握手信号特征码：0x55
            				E702_CanNodePresenceCheck(pick_rx_buf);	//====接收到E702的握手信号的处理

            			if ( HIBYTE(pick_rx_buf[2]) == 0xAA)								//DA扩展板发送给主机的心跳信号特征码：0xAA
            			{
            				E702_HeartReceive(pick_rx_buf);					//======接收到E702的心跳信号的处理
            				if ((ext_kind == CAN_DA_KIND) && (node_presence[ext_kind][ext_board_id] == false))
            				{
            					ApplyNodePresenceFromE702(pick_rx_buf);//======主机向E702请求握手信号
            				}
            			}
            		}
            		else
            		{
                        CanNodePresenceCheck();
                        CanNodeGuardRxProc(); 
                    } 
                    break;
            case CAN_PDO_RE_CODE :
                    CanPDOReplyWaitingProcess();
                    E702_DataReadReceive(pick_rx_buf);	//======接收到请求返回的E702地址数据，并赋值给用于上传的地址
                    break;
            case CAN_PDO_TX_CODE :
                    CanPDOReceiveProcess();
                    break;
            case CAN_SDO_RE_CODE :
                    CanSDOReplyWaitingProcess();
                    break;
            case CAN_SDO_TX_CODE :
                    CanSDOReceiveProcess();			//2010-01-05
                    break;
			case CAN_PDO2_TX_CODE:
					CanPDO2ReceiveProcess();
					break;
            default :
                    break;
        }
	}       
}

#define     COM_TX_REPLY        0x1100
void senddatatowcan(void)
{
  g_canpanelcomm[1].sdo_txpro_buf[0]   = 0xEB90;
  g_canpanelcomm[1].sdo_txpro_buf[5]   = (WORD)(COM_TX_REPLY ^ g_canpanelcomm[1].sdo_txpro_buf[2] ^ 6 ^ g_canpanelcomm[1].sdo_txpro_buf[4]); 
  g_canpanelcomm[1].p_sdo_data = g_canpanelcomm[1].sdo_txpro_buf;
  g_canpanelcomm[1].p_sdo_data_length = (WORD)(g_canpanelcomm[1].sdo_txpro_buf[3] + 5);
  g_canpanelcomm[1].sdo_re_flag_send = TRUE;
}

void wcandataexplain()
{
  WORD   fdata;
  
  fdata = (WORD)((g_canpanelcomm[1].sdo_rx_buf[1] & 0x0FF ) | 0x1100);
  db_printf("Get SDO pkt, %d, %x, %d\r\n", g_canpanelcomm[1].sdo_rx_buf[3], g_canpanelcomm[1].sdo_rx_buf[1], g_canpanelcomm[1].sdo_rx_buf[2]);  
  switch (fdata)
	{     
		case COM_RX_UPDATE:
			/*只接收到起始包才会更新，以免更新完成后，接收到面板重发的包，再次进入更新状态*/
			if((g_canpanelcomm[1].sdo_rx_buf[2]& 0x7fff) == 1)
			{
				/*置更新位，硬件复位重启后，bootloader判读有更新位，进行更新状态，实际的主机程序更新是由bootloader和面板通信完成的*/
				UpgradeCmdRx(0x0001);
			}
			else
			{
				/*修改bootloader的bug，最近一版bootloader更新完成后，无法成功发送最后一包应该给面板，导致面板显示更新一直没有完成，
				主机重启后，在接收到面板的重发更新包后，发送回复，使面板能正常结束更新状态*/
				if((g_canpanelcomm[1].sdo_rx_buf[2]& 0x8000) || g_canpanelcomm[1].sdo_rx_buf[2] > 0x100)
				{
					db_printf("Send reply to MMI, %d\r\n", g_canpanelcomm[1].sdo_rx_buf[2]& 0x7fff);
					g_canpanelcomm[1].sdo_txpro_buf[1]   = COM_TX_REPLY;		
					g_canpanelcomm[1].sdo_txpro_buf[2]   = g_canpanelcomm[1].sdo_rx_buf[2];		//包号回复
					g_canpanelcomm[1].sdo_txpro_buf[3]   = 1 + 5;								//长度
					g_canpanelcomm[1].sdo_txpro_buf[4]   = g_canpanelcomm[1].sdo_rx_buf[1];		//接收的命令
				}
				senddatatowcan();
			}
			break;		
	} 	

	g_canpanelcomm[1].wrxcount   = 0;
	g_canpanelcomm[1].fpackage   = FALSE;
}


void getdatafromwcan(void)
{
	CanPanelComm* pp = g_canpanelcomm + 1;
	int ind = 0;
	WORD chksum;

	if(pp->sdo_rx_task >= ARRAY_NUM(pp->sdo_rx_buf))
		pp->sdo_rx_task =0;

	/*包最小长度是5*/
	while(ind < pp->sdo_rx_task && pp->sdo_rx_task > 4)
	{
		/*缓存的第一个字节是否是0xEB90*/
		if(pp->sdo_rx_buf[0] != 0xEB90)
		{
			/*不是，则在缓存中找包头*/
			if(pp->sdo_rx_buf[ind] != 0xEB90)
			{
				ind++;
			}
			else
			{
				/*找到包头，忽略包头前的数据，把剩余的缓存数据拷贝到缓存起始地址*/
				pp->sdo_rx_task -= ind;
				memcpy(pp->sdo_rx_buf, pp->sdo_rx_buf + ind, pp->sdo_rx_task*sizeof(pp->sdo_rx_buf[0]));
				ind = 0;
			}
		}
		else 
		{
			/*找到包头0xEB90, 包的长度在第4个字节，长度包括0xEB90头, 包的最后字为异或校验，校验不包括0xEB90包头*/
			if(pp->sdo_rx_task >= pp->sdo_rx_buf[3])
			{
				/*异或校验*/
				chksum = 0;
				for(ind = 1; ind < pp->sdo_rx_buf[3]; ind++)
					chksum ^= pp->sdo_rx_buf[ind];
	
				if(chksum != 0 || pp->sdo_rx_buf[3] < 4)
				{
					/*校验错或长度错，重新寻找包头*/
					pp->sdo_rx_buf[0] = 0x0000;
					ind = 1;
				}
				else
				{
					/*得到一个完整的包*/
					ind = pp->sdo_rx_buf[3];
					wcandataexplain();
	
					/*把剩余的缓存数据拷贝到起始地址, 再重新寻找下一个包头*/
					memcpy(pp->sdo_rx_buf, pp->sdo_rx_buf + ind, pp->sdo_rx_task - ind);
					pp->sdo_rx_task -= ind;
					ind = 0;
				}
			}
			else if(pp->sdo_rx_buf[3] >= ARRAY_NUM(pp->sdo_rx_buf))
			{
				/*SDO 包的长度大于缓存长度，重新寻找包头*/
				pp->sdo_rx_buf[0] = 0x0000;
				ind = 1;
			}
			else
			{
				break;
			}
		}
	}

	if(pp->sdo_rx_buf[0] != 0xEB90)
		pp->sdo_rx_task =0;
}
