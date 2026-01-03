#include "il3.h"
#include "pindefine.h"
#include "canhead.h"
#include "useriap.h"
#include "memtbl.h"

void CanPLCTest(void)
{
#ifdef PLC_ENABLE

   //wTest=pick_rx_buf[2];
   WORD wType,wAddress;
   wType=pick_rx_buf[3];
   wAddress=pick_rx_buf[4];
   g_CanPlcTestComm.wLength=3;
   
   switch(wType)
   {
   case CODE_X:
	   g_CanPlcTestComm.flag_send		=	TRUE;
	   g_CanPlcTestComm.tx_data[0]		=	(WORD)dwInputXB;
	   g_CanPlcTestComm.tx_data[1]		=	(WORD)(dwInputXB>>16);
	   break;
   case CODE_Y:
	   g_CanPlcTestComm.flag_send		=	TRUE;
	   g_CanPlcTestComm.tx_data[0]		=	(WORD)dwOutputY;
	   g_CanPlcTestComm.tx_data[1]		=	(WORD)(dwOutputY>>16);
	   break;
   case CODE_M:
	   g_CanPlcTestComm.flag_send		=	TRUE;
	   g_CanPlcTestComm.tx_data[0]		=	(WORD)(dwMRegister[wAddress]);
	   g_CanPlcTestComm.tx_data[1]		=	(WORD)(dwMRegister[wAddress]>>16);
	   break;
   case CODE_S:
	   g_CanPlcTestComm.flag_send		=	TRUE;
	   g_CanPlcTestComm.tx_data[0]		=	(WORD)(dwSRegister[wAddress]);
	   g_CanPlcTestComm.tx_data[1]		=	(WORD)(dwSRegister[wAddress]>>16);
	   break;
   case CODE_T:
	   g_CanPlcTestComm.flag_send		=	TRUE;
	   if(Timer[wAddress].bTimeRun)
	   {
	   		if(wAddress<200)
		   		g_CanPlcTestComm.tx_data[0]		=	(WORD)(A_1msclock-Timer[wAddress].wBeginTime);
	  		else if(wAddress>199&&wAddress<400)
		  		g_CanPlcTestComm.tx_data[0]		=	(WORD)(A_10msclock-Timer[wAddress].wBeginTime);
	   		else
		   		g_CanPlcTestComm.tx_data[0]		=	(WORD)(A_100msclock-Timer[wAddress].wBeginTime);
	   }
	   else
	   		g_CanPlcTestComm.tx_data[0]	=	0;
	   g_CanPlcTestComm.tx_data[1]=0;
	   break;
   case CODE_C:
	   g_CanPlcTestComm.flag_send		=	TRUE;
	   if(Counter[wAddress].bCountRun)
	   		g_CanPlcTestComm.tx_data[0]		=	(WORD)(Counter[wAddress].wCurCount);
	   else
	   		g_CanPlcTestComm.tx_data[0]	=	0;
	   g_CanPlcTestComm.tx_data[1]=0;
	   break;
   case CODE_D:
	   g_CanPlcTestComm.flag_send		=	TRUE;
	   g_CanPlcTestComm.tx_data[0]		=	m_plc_d[wAddress];
	   g_CanPlcTestComm.tx_data[1]=0;
	   break;
   default:
	   break;
   }
   g_CanPlcTestComm.tx_data[2]		=	g_CanPlcTestComm.tx_data[0]+g_CanPlcTestComm.tx_data[1];
#endif
}

void CanPlcSendFile(WORD* pick_rx_buf)
{
#ifdef PLC_ENABLE
	
	//wTest=pick_rx_buf[2];
	DWORD wType,i;//wyh;
	WORD *pIL,wYH;
	if((pick_rx_buf[3]^pick_rx_buf[4])!=pick_rx_buf[5])
		return;
	wType=(DWORD)((((DWORD)pick_rx_buf[4])<<16)|(pick_rx_buf[3]));
	if(wType==0)
	{
		pIL=(WORD*)ILLENADDRESS;
		g_CanPlcTestComm.flag_send		=	TRUE;
		g_CanPlcTestComm.wLength		=	3;
		g_CanPlcTestComm.tx_data[0]		=	0;
		g_CanPlcTestComm.tx_data[1]		=	*(pIL++);
		g_CanPlcTestComm.tx_data[2]		=	*(pIL++);
	}
	else
	{
		pIL=(WORD*)((DWORD)(ILADDRESS+(wType-1)*2));
		wType+=1;
		g_CanPlcTestComm.flag_send		=	TRUE;
		g_CanPlcTestComm.wLength		=	300;
		g_CanPlcTestComm.tx_data[0]		=	(WORD)wType;
		g_CanPlcTestComm.tx_data[1]		=	(WORD)(wType>>16);
		for(i=2;i<299;i++)
			g_CanPlcTestComm.tx_data[i]		=	*(pIL++);
		wYH=g_CanPlcTestComm.tx_data[0];
		for(i=1;i<299;i++)
			wYH^=g_CanPlcTestComm.tx_data[i];
		g_CanPlcTestComm.tx_data[299]	=	wYH;
			
		//wyh=g_CanPlcTestComm.tx_data[0];
		//for(i=1;i<299;i++)
		//	wyh=wyh^g_CanPlcTestComm.tx_data[i];
		//g_CanPlcTestComm.tx_data[299]=wyh;
	}
	
	//ctrl_tbl[1377]=pick_rx_buf[4];
	//ctrl_tbl[1378]=pick_rx_buf[3];
	//ctrl_tbl[1379]+=1;
#endif	
}

void CanPlcReceiveFile(WORD* pick_rx_buf)
{
#ifdef PLC_ENABLE
	WORD i,j,wYH;
	DWORD dNum1;
	
	//db_printf("CRX: %04x %04x %04x %04x % 04x %04x\r\n", pick_rx_buf[0], pick_rx_buf[1], pick_rx_buf[2], pick_rx_buf[3], pick_rx_buf[4], pick_rx_buf[5] );


	if(bReceiveBegin)
	{
		if(pick_rx_buf[3]==0xEB90)
		{
			bReceiveBegin=FALSE;
			if(g_CanPlcTestComm.wrxPos != 0)
			{
				db_printf("Error begin, %d, %d\r\n", i, g_CanPlcTestComm.wrxPos);
				g_CanPlcTestComm.wrxPos = 0;
			}
		}
		else
			return;
	}

	wPlcUpdate=1;
	
	for(i=3;i<6;i++)
	{
		g_CanPlcTestComm.rx_data[g_CanPlcTestComm.wrxPos]=pick_rx_buf[i];
		g_CanPlcTestComm.wrxPos++;
		if(g_CanPlcTestComm.wrxPos==MAX_PLCRECEIVE)
		{
			wYH=g_CanPlcTestComm.rx_data[0];
			for(j=1;j<259;j++)
				wYH^=g_CanPlcTestComm.rx_data[j];
			
			if(wYH!=g_CanPlcTestComm.rx_data[259] || g_CanPlcTestComm.rx_data[0]!=0xEB90)
				db_printf("error XOR check\r\n");
				
			if((wYH==g_CanPlcTestComm.rx_data[259])&&(g_CanPlcTestComm.rx_data[0]==0xEB90))
			{
				dNum1=g_CanPlcTestComm.rx_data[2];
				dNum1<<=16;
				dNum1+=g_CanPlcTestComm.rx_data[1];
				db_printf("blk rx: %d\r\n", dNum1);
				if(dNum1==0xffffffff)
				{
					dwPlcFileLength= (((DWORD)g_CanPlcTestComm.rx_data[4]) << 16) + g_CanPlcTestComm.rx_data[3];
					if(dwPlcFileLength > 0)
					{
						PlcRecFileAdd	= m_plc_d;
						wPlcGetBuffer=0;
					}
					else
					{
						bReceiveBegin=TRUE;
						g_CanPlcTestComm.wrxPos=0;
						wPlcUpdate = 0;
					}
				}
				else if(dwPlcFileLength > 0)
				{
					for(j=0;j<PLCBUFFERLEN;j++)
					{
						*(PlcRecFileAdd+j+wPlcGetBuffer*PLCBUFFERLEN)=g_CanPlcTestComm.rx_data[j+3];
					}

					wPlcGetBuffer++;
					dNum1+=PLCBUFFERLEN;
					if((dNum1>=dwPlcFileLength)||wPlcGetBuffer==32)
					{
						
						if(Program_Erase(PlcIAPToFlashAddress) != 0)
							db_printf("Flash erse error %08x", PlcIAPToFlashAddress);
						//ÉÕÐ´

						delay_ms(1000);
						Program_IAP((uint32)(&m_plc_d[0]), PlcIAPToFlashAddress,8*256*2);
						db_printf("program plc , %d, %d\r\n\r\n", dNum1, dwPlcFileLength);
						wPlcGetBuffer=0;
						PlcIAPToFlashAddress+=32*256*2;
					}
					if(dNum1>=dwPlcFileLength)
					{
						HWRESET_SET;			/*Ç¿ÖÆ¸´Î»*/
						while(1);
					}
				}
				
				g_CanPlcTestComm.tx_data[0]=0x01;
			}
			else
				g_CanPlcTestComm.tx_data[0]=0x02;	
			

		
			bReceiveBegin=TRUE;
			g_CanPlcTestComm.wrxPos=0;
			if(dwPlcFileLength > 0)
			{
				g_CanPlcTestComm.wLength=3;
				g_CanPlcTestComm.flag_send=TRUE;
			}
			break;
		}
	}


	if(dwPlcFileLength == 0)
		wPlcUpdate = 0;
#endif	
}

void CanPLCTestTransmit(void)    
{
#ifdef PLC_ENABLE
	
	WORD func_code;
	WORD dst_node;
	WORD length;
	WORD send_count;
	//WORD board_kind;
	WORD *p_data;

	/*g_CanPlcTestComm.tx_data[0]=0x01;
	g_CanPlcTestComm.tx_data[1]=0x02;
	g_CanPlcTestComm.tx_data[2]=0x03;*/

	if(g_CanPlcTestComm.flag_send==TRUE)
	{
		p_data				=	g_CanPlcTestComm.tx_data;
		func_code			=	CAN_PDO_TX_CODE;
		length				=	g_CanPlcTestComm.wLength;
		send_count			=	0xAA;
		//board_kind			=	0xD;
		dst_node			=	0x71;
		CanSDOTransmitBufferWrite(p_data, func_code, length, dst_node, send_count);
		g_CanPlcTestComm.flag_send=FALSE;
	}
#endif	
}


