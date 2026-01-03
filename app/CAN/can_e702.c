#include "common.h"
#include "canhead.h"
#include "can_e702.h"
//#include "LPC2400CAN.h"
#include "systempro.h"
//#include "dbstat.h"
//#include "dbctrl.h"
//#include "hydrauic.h"   
#include "da.h"
#include "Machine.h"
void	E702_CanStructInit(void);//====E702相关的数据初始化
void	E702_CanNodePresenceCheck(WORD	*);//====接收到E702的握手信号的处理
void	E702_CanNodePresenceRevert(WORD ,WORD );//====回复握手信号给E702的处理
void	E702_CanSDOTransmitBufferWrite(WORD *, WORD , WORD , WORD  );//======将发送数据填入发送缓冲区
void	E702_CanTransmit (void);//======设置发送到E702的数据
void	E702_DataWrite(WORD	,WORD	,WORD	,WORD	);//======应用程序中调用的发送到E702的数据设置
void	E702_Port1_DataWrite(WORD	,WORD,WORD	);
void	E702_Port2_DataWrite(WORD	,WORD,WORD	);
void	E702_DataSafe(WORD	,WORD	);
void	E702_DataRead(WORD	,WORD	);
void	E702_DataReadReceive(WORD	*);
void	E702_HeartReceive(WORD	*);//======接收到E702的心跳信号的处理
void	E702_HeartSend(WORD ,WORD );//======发送心跳信号到E702的处理
void	ApplyNodePresenceFromE702(WORD	*);//======主机向E702请求握手信号
WORD 	ChangeDaByMode(WORD value);


CanE702Comm    g_cane702comm[DA_MAX_NUM];
extern DA m_da;
extern HYDR g_hydrout;

//====E702相关的数据初始化================
void	E702_CanStructInitsystem(void)
{
	int i;
	for(i=0;i<DA_MAX_NUM;i++)
		g_cane702comm[i].handshake=0;
}

void	E702_CanStructInit(void)
{ 
	int i;
    for ( i=0; i<DA_MAX_NUM; i++)
    {
        g_cane702comm[i].node_state = BUS_FREE;
        g_cane702comm[i].flag_send = false;
        g_cane702comm[i].p_origin_data = g_candacomm[i].tx_buf;
        g_cane702comm[i].origin_data_length = 4;
        g_cane702comm[i].flag_wait = false;
        g_cane702comm[i].reply_count = 0;
        g_cane702comm[i].comm_err = false;
        
        g_cane702comm[i].node_guarding_count = 0;
        g_cane702comm[i].heart_starttime = Get100MsTick();
    }
}

//====接收到E702的握手信号的处理================
void	E702_CanNodePresenceCheck(WORD	*rx_buf)
{
  WORD ext_kind;
  WORD ext_board_id;
	ext_kind = ( (LOBYTE(*(rx_buf+2)) & 0x78) >> 3);	//发送数据的扩展板类型
  ext_board_id = (LOBYTE(*(rx_buf+2)) & 0x7);				//发送数据的扩展板编号
  if (ext_kind == CAN_DA_KIND)											//CAN_DA_KIND:DA扩展板
  {
  	if ( HIBYTE(*(rx_buf+2)) == 0x55)								//DA扩展板发送给主机的握手信号特征码：0x55
  	{
  		if (g_cane702comm[ext_board_id].handshake == 0)
  			g_cane702comm[ext_board_id].handshake = 1;
    	if (node_presence[ext_kind][ext_board_id] == false)
      {
      	node_presence[ext_kind][ext_board_id] = true;
      }
      E702_CanNodePresenceRevert( ext_kind, ext_board_id );	//====回复握手信号给E702的处理
      pick_rx_buf[2] = 0;
  	}
  }
}

//====回复握手信号给E702的处理================
void	E702_CanNodePresenceRevert(WORD kind,WORD id)
{
//    WORD i;
    WORD dst_node;
    WORD last_tx_buf[6] = {0};
    
    dst_node = ( kind << 3 ) | ( id );
    
    last_tx_buf[0] = ( (CAN_NMT_CTRL_CODE | dst_node));       //PUT INTO MSGID
    last_tx_buf[1] = 4;                                             //PUT INTO MSGCTRL ,it is the data length, notice bytes not word
    last_tx_buf[2] = ( 0x02 << 8) | ( CAN_ID_ME );
    last_tx_buf[3] = (3<<8) | 0x90;	//A6A9的硬件版本号是2 ；IV3200的硬件版本号是3
    last_tx_buf[4] = 0x03;			//A6A9的软件版本号是2 ；IV3200的软件版本号是3
    last_tx_buf[5] = 0;
    
    CanTxBufWrite(last_tx_buf,dst_node); 
}

//======将发送数据填入发送缓冲区======
void	E702_CanSDOTransmitBufferWrite(WORD *p_tx_data, WORD func_code, WORD dst_node, WORD send_count)
{
//	WORD i;
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

//======设置发送到E702的数据======
void	E702_CanTransmit(void)
{
    WORD i;
    WORD func_code;
    WORD dst_node;
//    WORD length;
    WORD send_count;
    WORD *p_tx_data;
    
    for ( i=0; i<DA_MAX_NUM; i++ )
    {
//			if ( (g_cane702comm[i].node_state == BUS_FREE) && (node_presence[CAN_DA_KIND][i] == true) )
    	if (g_cane702comm[i].flag_send == true)
    	{
      	p_tx_data = g_cane702comm[i].p_origin_data;
      	func_code = CAN_PDO_TX_CODE;
//      	length    = g_cane702comm[i].origin_data_length;
      	dst_node  = ( CAN_DA_KIND << 3 ) | ( i );
      	send_count= g_cane702comm[i].send_count++;
      	
      	E702_CanSDOTransmitBufferWrite(p_tx_data, func_code,dst_node, send_count); //write PDO data to txbuff
      	                               
      	g_cane702comm[i].flag_send = false;
//      	g_cane702comm[i].flag_wait = true;         
      	g_cane702comm[i].reply_time = A_1msclock;
      	g_cane702comm[i].reply_count = 0;    
      	g_cane702comm[i].node_state = BUS_BUSY;    
    	}
    }
}

void	E702_CanTxDelayFlagSet(WORD flag)
{
	g_cane702comm[1].delayflag = flag;
}

void	E702_CanTransmitDelay(void)
{
	WORD WDA_DATA = 0;
    PDA pp = &m_da;

	if ((g_cane702comm[1].delayflag >= 1 && g_cane702comm[1].delayflag <= 8) && (WORD)(GetMsTick() - g_cane702comm[1].delaytime) > 40)
	{
		g_cane702comm[1].delaytime = GetMsTick();

		if(g_cane702comm[1].delayflag == 1)
			WDA_DATA = pp->mh->WDA_MAX[4];
		else if(g_cane702comm[1].delayflag == 2)
			WDA_DATA = pp->mh->WDA_ZERO[4];
		else if(g_cane702comm[1].delayflag == 3)
			WDA_DATA = pp->mh->WDA_MAX[5];
		else if(g_cane702comm[1].delayflag == 4)
			WDA_DATA = pp->mh->WDA_ZERO[5];
		else if(g_cane702comm[1].delayflag == 5)
			WDA_DATA = pp->mh->WDA_MAX[6];
		else if(g_cane702comm[1].delayflag == 6)
			WDA_DATA = pp->mh->WDA_ZERO[6];
		else if(g_cane702comm[1].delayflag == 7)
			WDA_DATA = pp->mh->WDA_MAX[7];
		else if(g_cane702comm[1].delayflag == 8)
			WDA_DATA = pp->mh->WDA_ZERO[7];

		  E702_DataSafe(g_cane702comm[1].delayflag,WDA_DATA);
	      E702_CanTransmit();
		g_cane702comm[1].delayflag++;
	}
}

void  Can_E700B_test()
{
	PMACHINE pp = GetSys();

	if(ChkOperMode(OM_AUTO) || (pp == NULL))
	{
		return;
	}
	
	switch (pp->can_e700b_writestep)
	{
		case 0:  
			E702_DataWrite(1,0x02,0,0);
			pp->can_e700b_writetm = GetMsTick();
			pp->st->WRITE_CNT_E700B=1;
			pp->can_e700b_writestep ++;
			break;	  
		case 1:
			if ((WORD)(GetMsTick() - pp->can_e700b_writetm)>=100)
			{
				E702_DataWrite(1,0x02,0,0);
				pp->can_e700b_writetm = GetMsTick();
				pp->st->WRITE_CNT_E700B++;
			}
			break;

			default:
				break;
	}
}

//======接收到E702的心跳信号的处理======
void	E702_HeartReceive(WORD	*rx_buf)
{
  WORD ext_kind;
  WORD ext_board_id;
	ext_kind = ( (LOBYTE(*(rx_buf+2)) & 0x78) >> 3);	//发送数据的扩展板类型
  ext_board_id = (LOBYTE(*(rx_buf+2)) & 0x7);				//发送数据的扩展板编号
  if (ext_kind == CAN_DA_KIND)											//CAN_DA_KIND:DA扩展板
  {
  	if ( HIBYTE(*(rx_buf+2)) == 0xAA)								//DA扩展板发送给主机的心跳信号特征码：0xAA
  	{
      E702_HeartSend( ext_kind, ext_board_id );
      g_cane702comm[ext_board_id].heart_starttime = Get100MsTick();
	  if((*(rx_buf+4)) != GetSys()->st->DA_E700B_VER)
	  {
			GetSys()->st->DA_E700B_VER = (*(rx_buf+4));
			PartDataTx((PPART)GetSys(), ST_ID, 1, GET_IND(PMACHINE_STATE, DA_E700B_VER), FALSE);
	  }

	}
  }
}

//======发送心跳信号到E702的处理======
void	E702_HeartSend(WORD kind,WORD id)
{
//    WORD i;
    WORD dst_node;
    WORD last_tx_buf[6] = {0};
    
    dst_node = ( kind << 3 ) | ( id );
    
    last_tx_buf[0] = ( (CAN_NMT_CTRL_CODE | dst_node));
    last_tx_buf[1] = 4;
    last_tx_buf[2] = ( 0xAA << 8) | ( CAN_ID_ME );
    last_tx_buf[3] = ( 3 << 8) | 0x90;	//A6A9的硬件版本号是2 ；IV3200的硬件版本号是3
    last_tx_buf[4] = 0x03;				//A6A9的软件版本号是2 ；IV3200的软件版本号是3	
    last_tx_buf[5] = 0;
    
    CanTxBufWrite(last_tx_buf,dst_node);  	
}

//======主机心跳断开的处理======
void	E702_HeartErr()
{
	int i;
	for ( i=0; i<DA_MAX_NUM; i++ )
	{
		if (node_presence[CAN_DA_KIND][i] == true)
		{
			if ((UI16)(Get100MsTick() - g_cane702comm[i].heart_starttime)>20)
			{
				node_presence[CAN_DA_KIND][i] = false;
			}
		}
	}
}

//======主机向E702请求握手信号======
void	ApplyNodePresenceFromE702(WORD	*rx_buf)
{
  WORD ext_kind;
  WORD ext_board_id;

//  WORD i;
  WORD dst_node;
  WORD last_tx_buf[6] = {0};

  ext_kind = ( (LOBYTE(*(rx_buf+2)) & 0x78) >> 3);	//发送数据的扩展板类型
  ext_board_id = (LOBYTE(*(rx_buf+2)) & 0x7);				//发送数据的扩展板编号
  if (ext_kind == CAN_DA_KIND)											//CAN_DA_KIND:DA扩展板
  {
//  	if (( HIBYTE(*(rx_buf+2)) != 0x55) && ( HIBYTE(*(rx_buf+2)) != 0xAA))
  	{
    	if (node_presence[ext_kind][ext_board_id] == false)
      {
				dst_node = ( ext_kind << 3 ) | ( ext_board_id ); 
		    
		    last_tx_buf[0] = ( (CAN_NMT_CTRL_CODE | dst_node));
		    last_tx_buf[1] = 4;
		    last_tx_buf[2] = ( 0x11 << 8) | ( CAN_ID_ME );
		    last_tx_buf[3] = ( 0x02 << 8) | 0x90;
		    last_tx_buf[4] = 0x02;
		    last_tx_buf[5] = 0;
		    
            CanTxBufWrite(last_tx_buf,dst_node);	
      }
  	}
  }
}

//======应用程序中调用的发送到E702的数据设置
void	E702_DataWrite(WORD	id,WORD	data0,WORD	data1,WORD	data2)
{
	g_cane702comm[id].tx_buf[0]=data0;
	g_cane702comm[id].tx_buf[1]=data1;
	g_cane702comm[id].tx_buf[2]=data2;
	g_cane702comm[id].p_origin_data = g_cane702comm[id].tx_buf;
	g_cane702comm[id].flag_send = true;
}

void	E702_Port1_DataWrite(WORD	pres,WORD	flow,WORD Direction)	//范围0--4095
{
	WORD Tpres,Tflow;

	if (!ChkRunFlag(INIF_DEBUG))
	{
		pres = ChangeDaByMode(pres);
		flow = ChangeDaByMode(flow);
	}
//		if (node_presence[CAN_DA_KIND][1] == true)
		if (g_cane702comm[1].handshake == 1)
		{
			Tpres = pres;
			Tflow = flow;
			E702_DataWrite(1,0x16|Direction,Tpres,Tflow);
		}
		else
		{
			Tpres = pres >>2;
			Tflow = flow >>2;
			E702_DataWrite(1,Tpres,Tflow,0);
		}
		E702_CanTransmit();
}

void	E702_Port2_DataWrite(WORD	pres,WORD	flow,WORD Direction)	//范围0--4095
{
	if (!ChkRunFlag(INIF_DEBUG))
	{
		pres = ChangeDaByMode(pres);
		flow = ChangeDaByMode(flow);
	}
	E702_DataWrite(1,0x26|Direction,pres,flow);
	E702_CanTransmit();
}
void	E702_Port1_AbsDataWrite(WORD	pres,WORD	flow)	//范围0--4095
{
	if (!ChkRunFlag(INIF_DEBUG))
	{
		pres = ChangeDaByMode(pres);
		flow = ChangeDaByMode(flow);
	}
	E702_DataWrite(1,0x17,pres,flow);
}

void	E702_Port2_AbsDataWrite(WORD	pres,WORD	flow)	//范围0--4095
{
	if (!ChkRunFlag(INIF_DEBUG))
	{
		pres = ChangeDaByMode(pres);
		flow = ChangeDaByMode(flow);
	}
	E702_DataWrite(1,0x27,pres,flow);
}

void	E702_DataSafe(WORD	addr,WORD	data)
{
	E702_DataWrite(1,(addr<<8)|0x06,data,0);
}
void	E702_DataRead(WORD	addr,WORD	data)
{
	E702_DataWrite(1,(addr<<8)|0x03,0,0);
}
//======接收到请求返回的E702地址数据，并赋值给用于上传的地址==========
void	E702_DataReadReceive(WORD	*rx_buf)
{
  PDA pp = &m_da;
  WORD ext_kind;
  WORD ext_board_id;
  ext_kind = ( (LOBYTE(*(rx_buf+2)) & 0x78) >> 3);	//发送数据的扩展板类型
  ext_board_id = (LOBYTE(*(rx_buf+2)) & 0x7);				//发送数据的扩展板编号
  if (ext_kind == CAN_DA_KIND)											//CAN_DA_KIND:DA扩展板
	{
		if ( LOBYTE(*(rx_buf+3)) == 0x03)
		{
			g_cane702comm[ext_board_id].E702_DataAddr = HIBYTE(*(rx_buf+3));				// 申请读数据返回的地址值
			g_cane702comm[ext_board_id].E702_DataReceive = *(rx_buf+4);			// 申请读数据返回的数据值
			if (ext_board_id == 1)
			{
				switch(g_cane702comm[ext_board_id].E702_DataAddr)
				{
					case 1://第一通道压力最大值
						pp->mh->WDA_MAX[4] = g_cane702comm[ext_board_id].E702_DataReceive;
						pp->st->TEMPMAX[4] = pp->mh->WDA_MAX[4];
						break;
					case 2://第一通道压力最小值
						pp->mh->WDA_ZERO[4] = g_cane702comm[ext_board_id].E702_DataReceive;
						pp->st->TEMPMIN[4] = pp->mh->WDA_ZERO[4];
						break;
					case 3://第一通道流量最大值
						pp->mh->WDA_MAX[5] = g_cane702comm[ext_board_id].E702_DataReceive;
						pp->st->TEMPMAX[5] = pp->mh->WDA_MAX[5];
						break;
					case 4://第一通道流量最小值
						pp->mh->WDA_ZERO[5] = g_cane702comm[ext_board_id].E702_DataReceive;
						pp->st->TEMPMIN[5] = pp->mh->WDA_ZERO[5];
						break;
				}		
			}	
		}
		else if( LOBYTE(*(rx_buf+3)) == 0x02)
		{
			g_candacomm[ext_board_id].node_guarding_count = 0;
			GetSys()->st->READ_CNT_E700B ++;
		}
	}
}

//======发送请求E702的地址数据，用于最大值和最小值的显示和调整
void	E702_DataRequest(void)
{
	static WORD	E702_1msClock=0;
	static WORD	count =1;
	if (ChkRunFlag(INIF_DEBUG))	//调试界面 或 DA校正画面
	{
		if ((UI16)(GetMsTick() - E702_1msClock)>50)
		{
			E702_1msClock = GetMsTick();
			E702_DataRead(count,0);
			count++;
			if (count>4) count=1;
		}
	}
}
