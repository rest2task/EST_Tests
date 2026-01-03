/*===========================================================================+
|  System   : AM335X                                                         |
|  Task     : DM9000.c                                                   |
|----------------------------------------------------------------------------|
|  Compile  : IAR -                                                          |
|  Link     : IAR -                                                          |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : chenhao                                                        |
|  Version  : V1.00                                                          |
|  Creation : 2018/6/15 10:02:38                                           |
|  Revision :                                                                |
+===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "dm9000.h"
#include "lwip_comm.h"
#include "lwip/pbuf.h"
#include "fsl_debug_console.h"
#include "fsl_semc.h"
#include "gpt_timer.h"
#include "common.h"
#include "udp_demo.h"
#include "TempCtrlComm.h"
#include "Typedef.h"
#include "udpctrl.h"

struct dm9000_config dm9000cfg;				//DM9000配置结构体


//#define delay_ms delay
volatile unsigned int g_clk_10us = 0;
unsigned char dm9000_mac_addr[6]={0x0, 0x16, 0xd4, 0x9f, 0xed, 0x55};
void EXTI0_19_IRQ(void);
void FeedDog(void);


//extern volatile unsigned  int A_1msclock ;
extern volatile DWORD A_1msclock;
#include "printf.h"

#include "tempctrl.h"
int flag_dm9k_ch[10]={0}; 
//========================================================
/*static void ten_udelay  (unsigned int u_time)
{
    unsigned int temp_clk_10us = 0;
    temp_clk_10us = g_clk_10us;
    while(1)
    {
        if ((g_clk_10us - temp_clk_10us ) >= u_time)
        break;
    }
}

static void loop_delay  (int cnt)
{
    volatile int loop = 0;
    loop = cnt;
    while(loop)
    {
        loop--;
    }
}*/

int Dm9000PinMuxSetup(void)
{
    int status = FALSE;

    status = TRUE;
    return status;
}

int GPMC_dm9000_Init(void )
{
    //unsigned int conf;
    //unsigned int baseAddr;
    //unsigned int cs;
    volatile unsigned int timeOut;
    //cs = DM9000_CHIP_SELECT_6;
    //conf = 0;
    timeOut =0xFFF;
    //baseAddr = (0x50000000);

    return(1);
}

/* GPIO SW Configuration */
#define GPIO_0_19                (0x09b0)
#define  GPIO_SW_PAD_OFFSET                     (GPIO_0_19)
#define  GPIO_INST_BASE_SW                      (SOC_GPIO_0_REGS)
#define  GPIO_SW_INTR_LINE                      (GPIO_INT_LINE_1)
#define  GPIO_SW_PIN_NUM                        (19u)
#define  GPIO_SW_SYS_INT_NUM                    (SYS_INT_GPIOINT0A)
#define REG32(x) 	(*((volatile unsigned int *)(x)))
void dm9000_int_init(void)
{  
  //  GPIO0ModuleClkConfig();

}
 

void t_mdelay  (unsigned int u_time)
{
    unsigned int temp_clk_1ms = 0;
    temp_clk_1ms = A_1msclock;
    while(1)
    {
        if ((A_1msclock - temp_clk_1ms ) >= u_time)
        break;
    }
}
//初始化DM9000
//返回值:
//0,初始化成功
//1，DM9000A ID读取错误
u8 DM9000_Init(void)
{
	u32 temp;
	  /*1.片选信息设置*/
  //  Dm9000PinMuxSetup();
   // GPMC_dm9000_Init();
    /*2.中断初始化*/
   //  dm9000_int_init();   //关闭dm9000的中断，这个中断 源提供给ext_ad使用  陈昊 2018.12.12

	dm9000cfg.mode=DM9000_AUTO;
 	dm9000cfg.queue_packet_len=0;
	//DM9000的SRAM的发送和接收指针自动返回到开始地址，并且开启接收中断
	dm9000cfg.imr_all = IMR_PAR|IMR_PRI;
	//初始化MAC地址
	dm9000cfg.mac_addr[0]=dm9000_mac_addr[0];
	dm9000cfg.mac_addr[1]=dm9000_mac_addr[1];
	dm9000cfg.mac_addr[2]=dm9000_mac_addr[2];
	dm9000cfg.mac_addr[3]=dm9000_mac_addr[3];
	dm9000cfg.mac_addr[4]=dm9000_mac_addr[4];
	dm9000cfg.mac_addr[5]=dm9000_mac_addr[5];
	//初始化组播地址
	dm9000cfg.multicase_addr[0]=0Xff;
	dm9000cfg.multicase_addr[1]=0Xff;
	dm9000cfg.multicase_addr[2]=0Xff;
	dm9000cfg.multicase_addr[3]=0Xff;
	dm9000cfg.multicase_addr[4]=0Xff;
	dm9000cfg.multicase_addr[5]=0Xff;
	dm9000cfg.multicase_addr[6]=0Xff;
	dm9000cfg.multicase_addr[7]=0Xff;

	DM9000_Reset();							//复位DM9000
    delay_ms(1000);
	temp=DM9000_Get_DeiviceID();			//获取DM9000ID
	PRINTF("DM9000 ID:%#x \r\n",temp);
	if(temp!=DM9000_ID) return 1; 			//读取ID错误
	DM9000_Set_PHYMode(dm9000cfg.mode);		//设置PHY工作模式

	DM9000_WriteReg(DM9000_NCR,0X00);
	DM9000_WriteReg(DM9000_TCR,0X00);		//发送控制寄存器清零
	DM9000_WriteReg(DM9000_BPTR,0X3F);
	DM9000_WriteReg(DM9000_FCTR,0X38);
	DM9000_WriteReg(DM9000_FCR,0X00);
	DM9000_WriteReg(DM9000_SMCR,0X00);		//特殊模式
	DM9000_WriteReg(DM9000_NSR,NSR_WAKEST|NSR_TX2END|NSR_TX1END);//清除发送状态
	DM9000_WriteReg(DM9000_ISR,0X0F);		//清除中断状态
	DM9000_WriteReg(DM9000_TCR2,0X80);		//切换LED到mode1
	//设置MAC地址和组播地址
	DM9000_Set_MACAddress(dm9000cfg.mac_addr);		//设置MAC地址
	DM9000_Set_Multicast(dm9000cfg.multicase_addr);	//设置组播地址
	DM9000_WriteReg(DM9000_RCR,RCR_DIS_LONG|RCR_DIS_CRC|RCR_RXEN);
	DM9000_WriteReg(DM9000_IMR,IMR_PAR);
	FeedDog();
	temp=DM9000_Get_SpeedAndDuplex();		//获取DM9000的连接速度和双工状态
	if(temp!=0XFF)							//连接成功，通过串口显示连接速度和双工状态
	{
		PRINTF("DM9000 Speed:%dMbps,Duplex:%s duplex mode\r\n",(temp&0x02)?10:100,(temp&0x01)?"Full":"Half");
	}
	else
	{
		PRINTF("DM9000 Establish Link Failed!: \r\n");
	}
	
	DM9000_WriteReg(DM9000_IMR,dm9000cfg.imr_all);	//设置中断
	return 0;
}

u8 nor_data[2]; 
u8 nor_read[2];
//读取DM9000指定寄存器的值
//reg:寄存器地址
//返回值：DM9000指定寄存器的值
u16 DM9000_ReadReg(u16 reg)
{
    u16 DM9000_data;
    nor_data[0] = reg;
    nor_data[1] = reg>>8;
    SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0000000, &nor_data[0], 2);
	//DM9000_REG=reg;
    SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0020000, &nor_read[0], 2);
    DM9000_data = nor_read[1]<<8 | nor_read[0];
	return DM9000_data;
       
/*    
	DM9000_REG=reg;
	return DM9000_DATA;
    */
}

//向DM9000指定寄存器中写入指定值
//reg:要写入的寄存器
//data:要写入的值

void DM9000_WriteReg(u16 reg,u16 data)
{
    nor_data[0] = reg;
    nor_data[1] = reg>>8;
    SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0000000, &nor_data[0], 2);
    nor_data[0] =data;
    nor_data[1] = data>>8;
    SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0020000, &nor_data[0], 2);
//	DM9000_REG=reg;
//	DM9000_DATA=data;
}

//读取DM9000的PHY的指定寄存器
//reg:要读的PHY寄存器
//返回值:读取到的PHY寄存器值
u16 DM9000_PHY_ReadReg(u16 reg)
{
	u16 temp;
	DM9000_WriteReg(DM9000_EPAR,DM9000_PHY|reg);
	DM9000_WriteReg(DM9000_EPCR,0X0C);				//选中PHY，发送读命令
	delay_ms(10);
	DM9000_WriteReg(DM9000_EPCR,0X00);				//清除读命令
	temp=(DM9000_ReadReg(DM9000_EPDRH)<<8)|(DM9000_ReadReg(DM9000_EPDRL));
	return temp;
}

//向DM9000的PHY寄存器写入指定值
//reg:PHY寄存器
//data:要写入的值
void DM9000_PHY_WriteReg(u16 reg,u16 data)
{
	DM9000_WriteReg(DM9000_EPAR,DM9000_PHY|reg);
	DM9000_WriteReg(DM9000_EPDRL,(data&0xff));		//写入低字节
	DM9000_WriteReg(DM9000_EPDRH,((data>>8)&0xff));	//写入高字节
	DM9000_WriteReg(DM9000_EPCR,0X0A);				//选中PHY,发送写命令
	delay_ms(50);
	DM9000_WriteReg(DM9000_EPCR,0X00);				//清除写命令
}

//获取DM9000的芯片ID
//返回值：DM9000的芯片ID值
u32 DM9000_Get_DeiviceID(void)
{
	u32 value=0;
	value =DM9000_ReadReg(DM9000_VIDL);
	value|=DM9000_ReadReg(DM9000_VIDH) << 8;
	value|=DM9000_ReadReg(DM9000_PIDL) << 16;
	value|=DM9000_ReadReg(DM9000_PIDH) << 24;
	return value;
}

//获取DM9000的连接速度和双工模式
//返回值：	0,100M半双工
//			1,100M全双工
//			2,10M半双工
//			3,10M全双工
//			0XFF,连接失败！
u8 DM9000_Get_SpeedAndDuplex(void)
{
	u32 temp;
	u32 i=0;
	if(dm9000cfg.mode==DM9000_AUTO)					//如果开启了自动协商模式一定要等待协商完成
	{
		while(!(DM9000_PHY_ReadReg(0X01)&0X0020))	//等待自动协商完成
		{
			//delay_ms(100);
			delay_ms(10);
			FeedDog();
			i++;
			if(i>100)
				return 0XFF;					//自动协商失败
		}
	}
	else											//自定义模式,一定要等待连接成功
	{
		while(!(DM9000_ReadReg(DM9000_NSR)&0X40))	//等待连接成功
		{
			//delay_ms(100);
			delay_ms(10);
			FeedDog();
			i++;
			if(i>100)
				return 0XFF;					//连接失败
		}
	}
    PRINTF(" \n\r  peedAndDuplex cnt = %d\n\r",i);
	temp =((DM9000_ReadReg(DM9000_NSR)>>6)&0X02);	//获取DM9000的连接速度
	temp|=((DM9000_ReadReg(DM9000_NCR)>>3)&0X01);	//获取DM9000的双工状态
	return temp;
}

//设置DM900的PHY工作模式
//mode:PHY模式
void DM9000_Set_PHYMode(u8 mode)
{
	u16 BMCR_Value,ANAR_Value;
	switch(mode)
	{
		case DM9000_10MHD:		//10M半双工
			BMCR_Value=0X0000;
			ANAR_Value=0X21;
			break;
		case DM9000_10MFD:		//10M全双工
			BMCR_Value=0X0100;
			ANAR_Value=0X41;
			break;
		case DM9000_100MHD:		//100M半双工
			BMCR_Value=0X2000;
			ANAR_Value=0X81;
			break;
		case DM9000_100MFD:		//100M全双工
			BMCR_Value=0X2100;
			ANAR_Value=0X101;
			break;
		case DM9000_AUTO:		//自动协商模式
			BMCR_Value=0X1000;
			ANAR_Value=0X01E1;
			break;
	}
	DM9000_PHY_WriteReg(DM9000_PHY_BMCR,BMCR_Value);
	DM9000_PHY_WriteReg(DM9000_PHY_ANAR,ANAR_Value);
 	DM9000_WriteReg(DM9000_GPR,0X00);	//使能PHY
}

//设置DM9000的MAC地址
//macaddr:指向MAC地址
void DM9000_Set_MACAddress(u8 *macaddr)
{
	u8 i;
	for(i=0;i<6;i++)
	{
		DM9000_WriteReg(DM9000_PAR+i,macaddr[i]);
	}
}
//设置DM9000的组播地址
//multicastaddr:指向多播地址
void DM9000_Set_Multicast(u8 *multicastaddr)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		DM9000_WriteReg(DM9000_MAR+i,multicastaddr[i]);
	}
}
//复位DM9000
void DM9000_Reset(void)
{
	//复位DM9000,复位步骤参考<DM9000 Application Notes V1.22>手册29页
	delay_ms(100);								//一定要有这个延时，让DM9000准备就绪！
 	DM9000_WriteReg(DM9000_GPCR,0x01);			//第一步:设置GPCR寄存器(0X1E)的bit0为1
	DM9000_WriteReg(DM9000_GPR,0);				//第二步:设置GPR寄存器(0X1F)的bit1为0，DM9000内部的PHY上电
 	DM9000_WriteReg(DM9000_NCR,(0x02|NCR_RST));	//第三步:软件复位DM9000
	do
	{
		delay_ms(25);
	}while(DM9000_ReadReg(DM9000_NCR)&1);		//等待DM9000软复位完成
	DM9000_WriteReg(DM9000_NCR,0);
	DM9000_WriteReg(DM9000_NCR,(0x02|NCR_RST));	//DM9000第二次软复位
	do
	{
		delay_ms(25);
	}while (DM9000_ReadReg(DM9000_NCR)&1);
}

#include "fsl_debug_console.h"

extern u32 lwtcp_time_save;
extern u32 lwudp_time_save;

u32 lwudp_send_time_save;
status_t SEMC_ConfigureIPCommand(SEMC_Type *base, uint8_t size_bytes);
//通过DM9000发送数据包
//p:pbuf结构体指针



u32 DM9000_request_finished()
{
	if((DM9000_ReadReg(DM9000_TCR) & 0x01) == 0)		//代表DM9000数据发送已经完毕
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void DM9000_SendPacket(struct pbuf *p)
{
	struct pbuf *q;
	u16 pbuf_index = 0;
	u8 word[2], word_index = 0;
    
    lwudp_send_time_save = g_clk_10us;
    
	DM9000_WriteReg(DM9000_IMR,IMR_PAR);		//关闭网卡中断
    nor_data[0] = DM9000_MWCMD;
    
    SEMC_ConfigureIPCommand(SEMC, 1);
    SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0000000, &nor_data[0], 1);    
    SEMC_ConfigureIPCommand(SEMC, 2);
    //SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0000000, &nor_data[0], 2);
	//DM9000_REG=DM9000_MWCMD;					//发送此命令后就可以将要发送的数据搬到DM9000 TX SRAM中
	q=p;
	//向DM9000的TX SRAM中写入数据，一次写入两个字节数据
	//当要发送的数据长度为奇数的时候，我们需要将最后一个字节单独写入DM9000的TX SRAM中
    
    //PRINTF("---tx fill-start- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwudp_time_save),p->tot_len ); 
 	while(q)
	{
		if (pbuf_index < q->len)
		{
			word[word_index++] = ((u8_t*)q->payload)[pbuf_index++];
			if (word_index == 2)
			{
				//DM9000_DATA=((u16)word[1]<<8)|word[0];
                SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0020000, &word[0], 2);
				word_index = 0;
			}
		}else
		{
			q=q->next;
			pbuf_index = 0;
		}
	}
	//还有一个字节未写入TX SRAM
	if(word_index==1)
    {
        //DM9000_DATA=word[0];    
        
        SEMC_ConfigureIPCommand(SEMC, 1);
        SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0020000, &word[0], 1);        
        SEMC_ConfigureIPCommand(SEMC, 2);
       // SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0020000, &word[0], 2);

    }
//chenhao 2018.07.06
//PRINTF("---tx fill-done- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwtcp_time_save),p->tot_len ); 
//PRINTF("---tx fill-done- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwudp_time_save),p->tot_len ); 
#define TX_UNLOOP
//#undef TX_UNLOOP

#ifdef TX_UNLOOP
        if((DM9000_ReadReg(DM9000_ISR)&0X02))
        {
            while((DM9000_ReadReg(DM9000_ISR)&0X02)==0);			//等待发送完成   
        }
        DM9000_WriteReg(DM9000_ISR,0X02);						//清除发送完成中断
#endif  
        
	//向DM9000写入发送长度
	DM9000_WriteReg(DM9000_TXPLL,p->tot_len&0XFF);
	DM9000_WriteReg(DM9000_TXPLH,(p->tot_len>>8)&0XFF);		//设置要发送数据的数据长度
	DM9000_WriteReg(DM9000_TCR,0X01);						//启动发送

#ifndef TX_UNLOOP    
	while((DM9000_ReadReg(DM9000_ISR)&0X02)==0);			//等待发送完成   
	DM9000_WriteReg(DM9000_ISR,0X02);						//清除发送完成中断
#endif  
    
//PRINTF("---tx done-- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwtcp_time_save),p->tot_len ); 
//PRINTF("---tx done-- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwudp_time_save),p->tot_len ); 
 	DM9000_WriteReg(DM9000_IMR,dm9000cfg.imr_all);			//DM9000网卡接收中断使能
}

//extern u32 lwudp_rev_time_save;
//DM9000接收数据包
//接收到的数据包存放在DM9000的RX FIFO中，地址为0X0C00~0X3FFF
//接收到的数据包的前四个字节并不是真实的数据，而是有特定含义的
//byte1:表明是否接收到数据，为0x00或者0X01，如果两个都不是的话一定要软件复位DM9000
//		0x01，接收到数据
//		0x00，未接收到数据
//byte2:第二个字节表示一些状态信息，和DM9000的RSR(0X06)寄存器一致的
//byte3:本帧数据长度的低字节
//byte4:本帧数据长度的高字节
//返回值：pbuf格式的接收到的数据包
struct pbuf *DM9000_Receive_Packet(void)
{
	struct pbuf* p;
	struct pbuf* q;
    u32 rxbyte;
	u16 rx_status, rx_length;
    u16* data;
	u16 dummy;
	int len;

	p=NULL;
__error_retry:
	DM9000_ReadReg(DM9000_MRCMDX);					//假读
//	rxbyte=(u8)DM9000_DATA;						//进行第二次读取
//  SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0020000, &nor_read[0], 1);
    SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0020000, &nor_read[0], 2);    //chenhao  2020.12.08
    rxbyte = (u8)nor_read[0];
	if(rxbyte)										//接收到数据
	{
        //lwudp_rev_time_save = g_clk_10us;

		if(rxbyte>1)								//rxbyte大于1，接收到的数据错误,挂了
		{
            PRINTF("dm9000 rx: rx error, stop device\r\n");
			DM9000_WriteReg(DM9000_RCR,0x00);
			DM9000_WriteReg(DM9000_ISR,0x80);
			return (struct pbuf*)p;
		}
        nor_data[0] = DM9000_MRCMD;
 //       SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0000000, &nor_data[0], 1);
        SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0000000, &nor_data[0], 2);
		//DM9000_REG=DM9000_MRCMD;
//		rx_status=DM9000_DATA;
//        rx_length=DM9000_DATA; 
        
        SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0020000, &nor_read[0], 2);
        rx_status = nor_read[1]<<8 | nor_read[0];
        SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0020000, &nor_read[0], 2);
        rx_length = nor_read[1]<<8 | nor_read[0];        
        

		//if(rx_length>512)printf("rxlen:%d\r\n",rx_length);
        p=pbuf_alloc(PBUF_RAW,rx_length,PBUF_POOL);	//pbufs内存池分配pbuf
		if(p!=NULL)									//内存申请成功
        {
            for(q=p;q!=NULL;q=q->next)
            {
                data=(u16*)q->payload;
                len=q->len;
//PRINTF("---rx fill start-- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwudp_rev_time_save),rx_length ); 
                while(len>0)
                {
                    SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0020000, (u8 *)data, 2);
					//*data=DM9000_DATA;
                    data++;
                    len-= 2;
                }
//PRINTF("---rx fill done-- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwudp_rev_time_save),rx_length ); 
            }
        }else										//内存申请失败
		{
			PRINTF("pbuf内存申请失败:%d\r\n",rx_length);
            data=&dummy;
			len=rx_length;
			while(len)
			{ 
                 SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0020000, (u8 *)data, 2);
				//*data=DM9000_DATA;
				len-=2;
			}
        }
		//根据rx_status判断接收数据是否出现如下错误：FIFO溢出、CRC错误
		//对齐错误、物理层错误，如果有任何一个出现的话丢弃该数据帧，
		//当rx_length小于64或者大于最大数据长度的时候也丢弃该数据帧
		if((rx_status&0XBF00) || (rx_length < 0X40) || (rx_length > DM9000_PKT_MAX))
		{
			PRINTF("rx_status:%#x\r\n",rx_status);
			if (rx_status & 0x100)PRINTF("rx fifo error\r\n");
            if (rx_status & 0x200)PRINTF("rx crc error\r\n");
            if (rx_status & 0x8000)PRINTF("rx length error\r\n");
            if (rx_length>DM9000_PKT_MAX)
			{
				PRINTF("rx length too big\r\n");
				DM9000_WriteReg(DM9000_NCR, NCR_RST); 	//复位DM9000
				delay_ms(5);
			}
			if(p!=NULL)pbuf_free((struct pbuf*)p);		//释放内存
			p=NULL;
			goto __error_retry;
		}
//PRINTF("---rx done-- =%d * 10 us--len=%d \n\r",(g_clk_10us-lwudp_rev_time_save),rx_length ); 
	}else
    {
        DM9000_WriteReg(DM9000_ISR,ISR_PTS);			//清除所有中断标志位
        dm9000cfg.imr_all=IMR_PAR|IMR_PRI;				//重新接收中断
        DM9000_WriteReg(DM9000_IMR, dm9000cfg.imr_all);
    }
    
	return (struct pbuf*)p;
}
//中断处理函数
void DM9000_ISRHandler(void)
{
	PUDP_CTRL pudp = udp_ctrl;
	static u16 k = 0;
	u16 i = 0;
	u16 int_status;
	u16 last_io;
	//UI16 RSR_statue = 0,TSRI_statue = 0,TSRII_statue = 0;
    
	//last_io = DM9000_REG;
    SEMC_IPCommandNorRead_Dm9k(SEMC,  0xb0000000, &nor_read[0], 2);
    last_io = nor_read[1]<<8 | nor_read[0];
    
	int_status=DM9000_ReadReg(DM9000_ISR);				//读中断状态数据
	DM9000_WriteReg(DM9000_ISR,int_status);				//清除中断标志位，DM9000的ISR寄存器的bit0~bit5写1清零

	//RSR_statue = DM9000_ReadReg(DM9000_RSR);
	//PRINTF("DM9000_RSR statue:%d \r\n",RSR_statue); 
	
	if(int_status & ISR_ROS)
		PRINTF("overflow \r\n");
    if(int_status & ISR_ROOS)
		PRINTF("overflow counter overflow \r\n");

	
	//TSRI_statue = DM9000_ReadReg(DM9000_TSRI);
	//TSRII_statue = DM9000_ReadReg(DM9000_TSRII);

	if(int_status & ISR_PRS)		//接收中断
	{
		i++;
 		//接收完成中断，用户自行添加所需代码
	}
	if(int_status & ISR_PTS)			//发送中断
	{
		if(pudp != NULL)
			pudp->Udp_tx_cnt = 0;
		//发送完成中断，用户自行添加所需代码
	}
	if(pudp->Udp_tx_cnt > 50)
	{
		pudp->Udp_tx_cnt = 0;
		k++;
		//DM9000_Init();
		//PRINTF("DM9000  Err :  %d, %x, %x, %x, %x\r\n",k,RSR_statue,TSRI_statue,TSRII_statue,int_status);
	}
	
    nor_data[0] = last_io;
    nor_data[1] = last_io >> 8; 
    SEMC_IPCommandNorWrite_Dm9k(SEMC, 0xb0000000, &nor_data[0], 2);
}
/**********************************************************************************************************
*	函 数 名: EXTI0_20_IRQHandler
*	功能说明: dm9000中断服务函数
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/

void EXTI0_19_IRQ(void)
{
 //   GPIOPinIntClear(GPIO_INST_BASE_SW, GPIO_SW_INTR_LINE, GPIO_SW_PIN_NUM);
    DM9000_ISRHandler();
}


UI16 DM9000_Get_LinkStatus(void)		//读取DM9000时候在离线的状态
{
	u32 tmp = 0;
	tmp = DM9000_ReadReg(DM9000_NSR) & 0x40;

	if(tmp == 0)
		return 0;
	else
		return 1;
}
