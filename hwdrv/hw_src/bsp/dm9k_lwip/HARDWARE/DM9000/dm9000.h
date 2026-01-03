/*===========================================================================+
|  System   : AM335X                                                         |
|  Task     : DM9000.h                                                 |
|----------------------------------------------------------------------------|
|  Compile  : IAR -                                                          |
|  Link     : IAR -                                                          |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : chenhao                                                        |
|  Version  : V1.00                                                          |
|  Creation : 2018/6/15 10:02:47                                             |
|  Revision :                                                                |
+===========================================================================*/
#ifndef _DM9000AEP_H
#define _DM9000AEP_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "hw_control_AM335x.h"
//#include "gpio_v2.h"
#include "types.h"
#include "Typedef.h"
#include "lwip/pbuf.h"

//#define DM9000_REG (*((volatile unsigned short  *) 0x02000000))
//#define DM9000_DATA (*((volatile unsigned short  *) 0x02000002))

#define DM9000_REG (*((volatile unsigned short  *)  0xb0000000))
#define DM9000_DATA (*((volatile unsigned short  *) 0xb0020000))


#define DM9000_CHIP_SELECT_6                (6)
#define DM9000_BASE                         (0x02000000)  /*hxs add */    //这个自己指定，只要在0x0 到0x1FFF_FFFF以内，不用和其他cs设备冲突。
#define DM9000_SIZE	                        GPMC_CS_SIZE_16MB

/*
  //目前在使用的dm9000  未优化
#define STNOR_GPMC_CONFIG1 0x00001000  //16位模式，非引脚复用 NOR型  异步asynchronous
#define STNOR_GPMC_CONFIG2 0x00080800  //片选信号CSRDOffTime CSWROffTime  20ns*8=160ns
#define STNOR_GPMC_CONFIG3 0x00020201  //不使用数据/地址信号切换nADV,所以配置比较随意
#define STNOR_GPMC_CONFIG4 0x08000800  //读使能#OE、写使能信号#WE持续     20ns*8=160ns
#define STNOR_GPMC_CONFIG5 0x01060D0D  //读写周期 RDCYCLETIME WRCYCLETIME 20*13=260ns, 读有效访问RDACCESSTIME 20*6=120ns  multiple access没在使用，设定较随意
#define STNOR_GPMC_CONFIG6 0x00000F80  // 连续访问周期间隔   15*20=300ns
#define STNOR_GPMC_CONFIG7 0x00000F42  // DM9000物理地址0x0200 0000, 16MB window， CSVALID
*/
    
    /*  //第一次优化 ok
#define STNOR_GPMC_CONFIG1 0x00001000  //16位模式，非引脚复用 NOR型  异步asynchronous
#define STNOR_GPMC_CONFIG2 0x00040400  //片选信号CSRDOffTime CSWROffTime  20ns*8=160ns
#define STNOR_GPMC_CONFIG3 0x00010101  //不使用数据/地址信号切换nADV,所以配置比较随意
#define STNOR_GPMC_CONFIG4 0x04000400  //读使能#OE、写使能信号#WE持续     20ns*8=160ns
#define STNOR_GPMC_CONFIG5 0x01040909  //读写周期 RDCYCLETIME WRCYCLETIME 20*13=260ns, 读有效访问RDACCESSTIME 20*6=120ns  multiple access没在使用，设定较随意
#define STNOR_GPMC_CONFIG6 0x00000200  // 连续访问周期间隔   15*20=300ns
#define STNOR_GPMC_CONFIG7 0x00000F42  // DM9000物理地址0x0200 0000, 16MB window， CSVALID
    */
   
   //第二次优化 ok  注释也重新修正
#define STNOR_GPMC_CONFIG1 0x00001000  //16位模式，非引脚复用 NOR型  异步asynchronous
#define STNOR_GPMC_CONFIG2 0x00020200  //片选信号CSRDOffTime CSWROffTime  20ns*2=40ns
#define STNOR_GPMC_CONFIG3 0x00010101  //不使用数据/地址信号切换nADV,所以配置比较随意
#define STNOR_GPMC_CONFIG4 0x02000200  //读使能#OE、写使能信号#WE持续     20ns*2=40ns
#define STNOR_GPMC_CONFIG5 0x00020406  //读写周期 RDCYCLETIME 20*6=120ns WRCYCLETIME 20*4=80ns； 读有效访问RDACCESSTIME 20*2=40ns，这个值要保证，否则link不成功    
#define STNOR_GPMC_CONFIG6 0x00000200  // 连续访问周期间隔   2*20=40ns  经测试，这个其实没什么用
#define STNOR_GPMC_CONFIG7 0x00000F42  // DM9000物理地址0x0200 0000, 16MB window， CSVALID 
 
  
    
    /*
   //第三次优化 ok  数值虽然变小，但实际时间和第二次优化一样。因此不采用这个参数
#define STNOR_GPMC_CONFIG1 0x00001000  //16位模式，非引脚复用 NOR型  异步asynchronous
#define STNOR_GPMC_CONFIG2 0x00010100  //片选信号CSRDOffTime CSWROffTime  20ns*8=160ns
#define STNOR_GPMC_CONFIG3 0x00010101  //不使用数据/地址信号切换nADV,所以配置比较随意
#define STNOR_GPMC_CONFIG4 0x01000100  //读使能#OE、写使能信号#WE持续     20ns*8=160ns
#define STNOR_GPMC_CONFIG5 0x00020204  //读写周期 RDCYCLETIME WRCYCLETIME 20*13=260ns, 读有效访问RDACCESSTIME 20*6=120ns  multiple access没在使用，设定较随意  
#define STNOR_GPMC_CONFIG6 0x00000000  // 连续访问周期间隔   15*20=300ns
#define STNOR_GPMC_CONFIG7 0x00000F42  // DM9000物理地址0x0200 0000, 16MB window， CSVALID 
    */
    
    
    
 // 以下配置是有问题的。 
/*
 //dm90000,提速  2018.07.13
#define STNOR_GPMC_CONFIG1 0x00001000  //16位模式，非引脚复用 NOR型  异步asynchronous
#define STNOR_GPMC_CONFIG2 0x00040400  //片选信号CSRDOffTime CSWROffTime  20ns*8=160ns
#define STNOR_GPMC_CONFIG3 0x00010101  //不使用数据/地址信号切换nADV,所以配置比较随意
#define STNOR_GPMC_CONFIG4 0x04000400  //读使能#OE、写使能信号#WE持续     20ns*8=160ns
#define STNOR_GPMC_CONFIG5 0x01040404 //读写周期 RDCYCLETIME WRCYCLETIME 20*13=260ns, 读有效访问RDACCESSTIME 20*6=120ns  multiple access没在使用，设定较随意
#define STNOR_GPMC_CONFIG6 0x00000200  // 连续访问周期间隔   15*20=300ns
#define STNOR_GPMC_CONFIG7 0x00000F42  // DM9000物理地址0x0200 0000, 16MB window， CSVALID
*/
    
    /*
 //dm90000,提速1倍
#define STNOR_GPMC_CONFIG1 0x00001000  //16位模式，非引脚复用 NOR型  异步asynchronous
#define STNOR_GPMC_CONFIG2 0x00040400  //片选信号CSRDOffTime CSWROffTime  20ns*8=160ns
#define STNOR_GPMC_CONFIG3 0x00020201  //不使用数据/地址信号切换nADV,所以配置比较随意
#define STNOR_GPMC_CONFIG4 0x04000400  //读使能#OE、写使能信号#WE持续     20ns*8=160ns
#define STNOR_GPMC_CONFIG5 0x01060606  //读写周期 RDCYCLETIME WRCYCLETIME 20*13=260ns, 读有效访问RDACCESSTIME 20*6=120ns  multiple access没在使用，设定较随意
#define STNOR_GPMC_CONFIG6 0x00000840  // 连续访问周期间隔   15*20=300ns
#define STNOR_GPMC_CONFIG7 0x00000F42  // DM9000物理地址0x0200 0000, 16MB window， CSVALID
*/

#define DM9000_ID			0X90000A46	//DM9000 ID


#define DM9000_PKT_MAX		1536		//DM9000最大接收包长度

#define DM9000_PHY			0X40		//DM9000 PHY寄存器访问标志
//DM9000寄存器
#define DM9000_NCR			0X00
#define DM9000_NSR			0X01
#define DM9000_TCR			0X02
#define DM9000_TSRI			0X03
#define DM9000_TSRII		0X04
#define DM9000_RCR			0X05
#define DM9000_RSR			0X06
#define DM9000_ROCR			0X07
#define DM9000_BPTR			0X08
#define DM9000_FCTR			0X09
#define DM9000_FCR			0X0A
#define DM9000_EPCR			0X0B
#define DM9000_EPAR			0X0C
#define DM9000_EPDRL		0X0D
#define DM9000_EPDRH		0X0E
#define DM9000_WCR			0X0F
#define DM9000_PAR			0X10		//物理地址0X10~0X15
#define DM9000_MAR			0X16		//多播地址0X16~0X1D
#define DM9000_GPCR			0X1E
#define DM9000_GPR			0X1F
#define DM9000_TRPAL		0X22
#define DM9000_TRPAH		0X23
#define DM9000_RWPAL		0X24
#define DM9000_RWPAH		0X25

#define DM9000_VIDL			0X28
#define DM9000_VIDH			0X29
#define DM9000_PIDL			0X2A
#define DM9000_PIDH			0X2B

#define DM9000_CHIPR		0X2C
#define DM9000_TCR2			0X2D
#define DM9000_OCR			0X2E
#define DM9000_SMCR			0X2F
#define DM9000_ETXCSR		0X30
#define DM9000_TCSCR		0X31
#define DM9000_RCSCSR		0X32
#define DM9000_MRCMDX		0XF0
#define DM9000_MRCMDX1		0XF1
#define DM9000_MRCMD		0XF2
#define DM9000_MRRL			0XF4
#define DM9000_MRRH			0XF5
#define DM9000_MWCMDX		0XF6
#define DM9000_MWCMD		0XF8
#define DM9000_MWRL			0XFA
#define DM9000_MWRH			0XFB
#define DM9000_TXPLL		0XFC
#define DM9000_TXPLH		0XFD
#define DM9000_ISR			0XFE
#define DM9000_IMR			0XFF

#define NCR_RST             0X01
#define NSR_SPEED           0X80
#define NSR_LINKST         	0X40
#define NSR_WAKEST          0X20
#define NSR_TX2END          0X08
#define NSR_TX1END          0X04
#define NSR_RXOV            0X02

#define RCR_DIS_LONG        0X20
#define RCR_DIS_CRC         0X10
#define RCR_ALL             0X08
#define RCR_RXEN            0X01

#define IMR_PAR             0X80
#define IMR_ROOI            0X08
#define IMR_POI             0X04		//使能接收溢出中断
#define IMR_PTI             0X02		//使能发送中断
#define IMR_PRI             0X01		//使能接收中断

#define ISR_LNKCHGS         (1<<5)
#define ISR_ROOS            (1<<3)
#define ISR_ROS             (1<<2)
#define ISR_PTS             (1<<1)
#define ISR_PRS             (1<<0)
#define ISR_CLR_STATUS      (ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS)

//DM9000内部PHY寄存器
#define DM9000_PHY_BMCR		0X00
#define DM9000_PHY_BMSR		0X01
#define DM9000_PHY_PHYID1	0X02
#define DM9000_PHY_PHYID2	0X03
#define DM9000_PHY_ANAR		0X04
#define DM9000_PHY_ANLPAR	0X05
#define DM9000_PHY_ANER		0X06
#define DM9000_PHY_DSCR		0X10
#define DM9000_PHY_DSCSR	0X11
#define DM9000_PHY_10BTCSR	0X12
#define DM9000_PHY_PWDOR	0X13
#define DM9000_PHY_SCR		0X14

//DM9000工作模式定义
enum DM9000_PHY_mode
{
	DM9000_10MHD 	= 	0, 					//10M半双工
	DM9000_100MHD 	= 	1,					//100M半双工
	DM9000_10MFD 	= 	4, 					//10M全双工
	DM9000_100MFD 	= 	5,					//100M全双工
	DM9000_AUTO  	= 	8, 					//自动协商
};

//DM9000配置结构体
struct dm9000_config
{
	enum DM9000_PHY_mode mode;				//工作模式
	u8  imr_all;							//中断类型
	u16 queue_packet_len;					//每个数据包大小
	u8  mac_addr[6];						//MAC地址
	u8  multicase_addr[8];					//组播地址
};
extern struct dm9000_config dm9000cfg;		//dm9000配置结构体

u8   DM9000_Init(void);
u16  DM9000_ReadReg(u16 reg);
void DM9000_WriteReg(u16 reg,u16 data);
u16  DM9000_PHY_ReadReg(u16 reg);
void DM9000_PHY_WriteReg(u16 reg,u16 data);
u32  DM9000_Get_DeiviceID(void);
u8   DM9000_Get_SpeedAndDuplex(void);
void DM9000_Set_PHYMode(u8 mode);
void DM9000_Set_MACAddress(u8 *macaddr);
void DM9000_Set_Multicast(u8 *multicastaddr);
void DM9000_Reset(void);
void DM9000_SendPacket(struct pbuf *p);
struct pbuf *DM9000_Receive_Packet(void);
void DMA9000_ISRHandler(void);

int Dm9000PinMuxSetup(void);
int GPMC_dm9000_Init(void );
void DM9000_ISRHandler(void);
u32 DM9000_request_finished(void);

#endif



