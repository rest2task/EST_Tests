#ifndef CANHEAD_H
#define CANHEAD_H

#include "common.h"           //dsp上的common.h
#include "typedef.h"
#include "hwcfg.h"
#include "SRAM.h"


#define SWI_HIB_LOB(data)                   \
{                                           \
    (data) = ((data) << 8) | ((data) >> 8); \
}


#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef true
#define true    (1 != 0)
#endif

#ifndef false
#define false   (1 == 0)
#endif

#define			CAN_RX_MSG_NO		0x01
#define			CAN_TX_MSG_NO		0x11
extern WORD		canrx_msg;
void RX0_callback(unsigned int msg_no);
void RX1_callback(unsigned int msg_no);
void TX0_callback(unsigned int msg_no);
void TX1_callback(unsigned int msg_no);
inline WORD CanTxMsgNum(int can_num);
void CAN_rec_msg_config(int can_num, unsigned char msg_num, unsigned int can_id);

/*
------------------------------------------------------------------------------
|   Definition
------------------------------------------------------------------------------
*/
#define BOARD_KIND_MAX          15
#define BOARD_NUM_MAX           (7+1)


/*A620 A920*/
#define BPS_1000K	1000000
#define BPS_800K	800000    
#define BPS_500K	500000
#define BPS_250K	250000
#define BPS_125K	125000
#define BPS_100K	100000
#define BPS_50K		50000
#define BPS_20K		20000
#define BPS_10K		10000

#define CANOFFSET	0x4000
#define RGE(ADDR)	(*((volatile unsigned long*)ADDR))

//----------------------------------------------------------------------------
#define MAXCANTEMPRXBUFFER      30
#define MAX_PLANEL_SDO_RX_BUFFER 500    //2010-01-05
#define SDO_RXPROC_LENGTH		200     //2010-01-05
#define	SDO_TXPROC_LENGTH		10      //2010-01-05

//----------------------------------------------------------------------------
#define CPUS_MAX_NUM        4	
#define LIO_MAX_NUM         3	
#define DA_MAX_NUM          2	
#define TEMP_MAX_NUM        3
#define SERVO_MAX_NUM       8	
#define SIMP_MAX_NUM        4	
#define PANEL_MAX_NUM       2           //2008-10-23 10:46
#define E301_MAX_NUM		1
#define TEMPMW_MAX_NUM    6

//----------------------------------------------------------------------------
#define ENTER_PER_OPST      1
#define ENTER_OPST          2
#define REST_NODE           4   

//----------------------------------------------------------------------------
#define CAN_ID_ME           ((CAN_CPU_KIND << 3) | (0x1))    /* ((CAN_CPU_KIND << 4) | (0x1))  My id is 0x09 */

//----------------------------------------------------------------------------
#define CAN_NMT_RAD_CODE        0x000
#define CAN_NMT_SYNC_CODE       0x100
#define CAN_TIME_STAMP_CODE     0x280   //#define CAN_TIME_STAMP_CODE     0x200
#define CAN_EMERGENCY_CODE      0x100
#define CAN_PDO_TX_CODE         0x300
#define CAN_PDO_RE_CODE         0x380   //#define CAN_PDO_RE_CODE         0x400
#define CAN_SDO_TX_CODE         0x400   //#define CAN_SDO_TX_CODE         0x500
#define CAN_SDO_RE_CODE         0x480   //#define CAN_SDO_RE_CODE         0x600
#define CAN_PDO2_TX_CODE		0x500	//模式5的，驱动器应答包。
#define CAN_NMT_CTRL_CODE       0x700

#define CAN_SERVO_PDO_TX_CODE   0x200
#define CAN_SERVO_PDO_RX_CODE   0x180
#define CAN_SERVO_SDO_TX_CODE   0x600
#define CAN_SERVO_SDO_RX_CODE   0x580

/*---------------------------------------------------------------------------*/
#define	E502_TEMP_TEST		9           /*主机发送E502can通讯测试命令*/
#define	E502_TEMP_VER		0x0A        /*主机发送读E502软件版本命令*/

/*---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
typedef struct tag_CANINT CANINT;
struct tag_CANINT
  {
    WORD*       p_send_txbuf;           // send buffer pointer
    WORD*       p_fill_rxbuf;           // fill buffer pointer
    WORD        wcount;	                // send data length
    BOOL        sending;                // this flag is which sign have data to sending 
    BOOL        receiving;              // this flag is which sign have data to receiving 
    WORD*       p_receive_rxbuf;        // current receive buffer pointer
  };	
/*----------------------------------------------------------------------------*/
typedef struct tag_CANTXBUF CANTXBUF;
struct tag_CANTXBUF
  {
    WORD*       p_txbuf_start;      /* Pointer to the transmit buffer start address */
    WORD*       p_fill_txbuf;       /* Pointer to the transmit buffer current fill address */
    WORD*       p_txbuf_end;        /* Pointer to the transmit buffer end address */   
  };	
/*----------------------------------------------------------------------------*/
typedef struct tag_CANRXBUF CANRXBUF;
struct tag_CANRXBUF
  {
    WORD*       p_rxbuf_start;      /* Pointer to the receive buffer start address */
    WORD*       p_pick_rxbuf;       /* Pointer to the receive buffer current pick address */
    WORD*       p_rxbuf_end;        /* Pointer to the receive buffer end address */   
  };	

/*----------------------------------------------------------------------------*/
typedef struct tagCanIOComm CanIOComm;
struct tagCanIOComm {
    WORD    node_state;
    BOOL    flag_send;
    WORD*   p_origin_data;
    WORD    origin_data_length;
    
    BOOL    flag_wait;
    
    WORD    send_count;
    
    WORD    reply_time;
    WORD    reply_count;
    BOOL    comm_err;
    
    WORD    input_state;
    
    WORD    node_guarding_time;
    WORD    node_guarding_count;

	WORD   pdo_tx_data[4];
};
/*----------------------------------------------------------------------------*/
typedef struct tagCanDAComm CanDAComm;
struct tagCanDAComm {
    WORD    node_state;
    BOOL    flag_send;
    WORD*   p_origin_data;
    WORD    origin_data_length;
    
    BOOL    flag_wait;
    
    WORD    send_count;
    
    WORD    reply_time;
    WORD    reply_count;
    BOOL    comm_err;
  //  WORD*   p_tx_data;
    WORD    tx_buf[5];
  //  WORD    tx_data_length;
    WORD    rx_buf[4];
  //  WORD    rx_data_length;
    WORD    node_guarding_time;
    WORD    node_guarding_count;
};
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
typedef struct tagCanTempE502Comm  CanTempE502Comm;
struct tagCanTempE502Comm {
   	 WORD    pdo_tx_data[4];
	 WORD    pdo_rx_data[4];
	 WORD*   p_origin_data;
	 BOOL    flag_send;	
	 BOOL	 comm_err;	   
	 WORD	 send_count;
	 WORD	 node_guarding_count;

	 
	 WORD 	 TestTemp;  //采集当前温度的毫伏值
	 WORD 	 RealTemp;  //采集当前温度
	 UI8    TempFlag;  //判断当前温度是否100次采集完
	 UI8	CurChnl;	//当前通道
	 WORD	 wtemp_lm73;    				  //读取室温  
	 WORD    wtemp_real;            		  //校正后的实时温度(7474界面的温度显示)
	 WORD    wtemp_advalue;  				  //温度AD值
	 WORD	wtemp_ver;						//版本号
};

typedef struct tagCanServoComm CanServoComm;
struct tagCanServoComm {
    WORD    node_state;
    BOOL    flag_send;
    WORD*   p_origin_data;
    WORD    origin_data_length;
    
    WORD    send_count;
    
    WORD    pdo_tx_data[4];
    WORD    pdo_rx_data[4];
    WORD    pdo_rx_mode5_data[6];
    
    BOOL    comm_err;
};

/*----------------------------------------------------------------------------*/
typedef struct tagCanPanelComm CanPanelComm;
struct tagCanPanelComm {
    WORD    node_state;
    BOOL    flag_send;
    WORD*   p_origin_data;
    WORD    origin_data_length;
    
    WORD    send_count;
    
    WORD    pdo_tx_data[4];
    WORD    pdo_rx_data[4];
    
    BOOL    comm_err;     
    BOOL    sdo_re_flag_send;        //2010-01-05  whaul
    WORD*   p_sdo_data;
    WORD    p_sdo_data_length; 
    WORD	sdo_txpro_buf[SDO_TXPROC_LENGTH];
   
    WORD    sdo_rx_buf[MAX_PLANEL_SDO_RX_BUFFER];  
    WORD    sdo_rx_task;
    WORD    sdo_rx_tasktail; 
    WORD	wrxcount;
    BOOL	fpackage;
    WORD	bheadget;
    WORD	sdo_rxpro_buf[SDO_RXPROC_LENGTH];
    WORD	sdo_rxpro_no; 
    BOOL	comm_can;       	//can更新有效,用于最后一次回复判断  2010-01-05      
};

/*----------------------------------------------------------------------------*/
typedef struct tagDB_TEMPMOLD DB_TEMPMOLD;   //模温扩展板
struct tagDB_TEMPMOLD
{
    WORD    wMarkUsed;			//包头，EB90
    WORD	nTempId;			//标记1，暂时无用		20200116
	WORD    TempMold_node_state;	//模温扩展板开始启动发送数据标记
	WORD    node_state_mw;				//当前模温扩展板存在标记
	BOOL    flag_send_mw;
	BOOL    flag_wait_mw;
    WORD    send_count_mw;
    WORD    reply_time_mw;
    WORD    reply_count_mw;
	WORD*   p_origin_data_mw;
	WORD    tx_buf[4];
    WORD    rx_buf[4];
	WORD    node_guarding_e501;
	WORD	node_gard_time;
} ;


//----------------------------------------------------------------------------
// node_state's code 
#define BUS_FREE                0
#define BUS_BUSY                1
//#define BUS_SEND                2
//#define BUS_SERVO_READ_SEND     3
//#define BUS_SERVO_WRITE_SEND    4



//----------------------------------------------------------------------------

enum {
    CAN_CPU_KIND = 1,
    CAN_ERULER_KIND,
    CAN_HSPDIO_KIND,
    CAN_SERVO_KIND,
    CAN_DA_KIND,
	CAN_POEWR_KIND, 						//作为能耗表的地址
    CAN_LSPDIO_KIND,
    CAN_RELAY_KIND,
    CAN_TEMP_KIND,
    CAN_PANEL_KIND,
    //CAN_IS500_KIND,//IS500
	CAN_E301_KIND,			//E301 extend board  
	//CAN_PLC,
    CAN_TEMPE502_KIND = 0xC,			//作为E502温度采集板的地址 	20191226
    CAN_HSPDTEMP_KIND = 0xD, 			//做为E501模温扩展板的地址	20191226
    CAN_TEMPE505_KIND = 0xE,			//作为E505室温板的地址   		20191226
    //CAN_PC = 0xE,
    CAN_SIMPLE_KIND = 0xF
};

#define COM_RX_UPDATE		0x1190			// 程序更新

/*
------------------------------------------------------------------------------
|       Global Variant  
------------------------------------------------------------------------------
*/
extern  CANTXBUF     g_cantxbuf;
extern  CANRXBUF     g_canrxbuf;
extern  CANINT       g_canint;
extern  CanIOComm    g_canliocomm[LIO_MAX_NUM];
extern  CanDAComm    g_candacomm[DA_MAX_NUM];
extern  CanPanelComm g_canpanelcomm[PANEL_MAX_NUM];     //2008-12-26 13:19
extern 	CanTempE502Comm g_cantempe502comm[TEMP_MAX_NUM];
extern	DB_TEMPMOLD     g_cantempe501[TEMPMW_MAX_NUM];
extern  CanServoComm g_canservocomm[SERVO_MAX_NUM];

extern  BOOL        node_presence[BOARD_KIND_MAX][BOARD_NUM_MAX];
extern  BOOL        node_oncheck[BOARD_KIND_MAX][BOARD_NUM_MAX];

extern  WORD        cantbl[CANTXBUFFSIZE + 10];

extern  WORD        canrbl[CANRXBUFFSIZE + 10];                 
extern  WORD        pick_rx_buf[6];           

extern  WORD        tst_io1; //for tst     
extern  WORD        da_tst_array[2];//tst   

extern 	BOOL		cantx_flag;			//2011-1-11  Whaul
extern 	WORD		cantx_time;         // for can txd protect
/*
------------------------------------------------------------------------------
|   Function Prototype
------------------------------------------------------------------------------
*/
void    CanInit(void);
void    CanNodePresenceCheck(void);
void    __irq  CanIsr(void);
void    CanProcess(void);

void    InitCANTxBuf (void);
void    CanTransmitInterrupt(void);
void    CanTransmitProcess(void);

void    InitCANRxBuf (void);
void    CanReceiveInterrupt(void);
void    CanReceiveProcess(void);

void    CanTransmitCheck(void);

void    CanTstProcess(void);//for tst 

void initwcantask(void);        //2010-01-05  whaul
void wcan_initbuff(void);        //2010-01-05  whaul
void getdatafromwcan(void);       // 2010-01-05  whaul

BOOL CanExtIOWrite(UI8 id, UI16* extio_val);
BOOL CanExtIORead(UI8 id, UI16* extio_val);
BOOL CanExtADRead(UI8 id, UI16* extio_val);
void CanSDOTransmitBufferWrite(WORD *p_tx_data, WORD func_code, WORD length, WORD dst_node, WORD send_count );
void CanTxBufWrite(WORD *last_tx_buf, WORD dst_node);
void CanExtTempMoldWrite(WORD id,WORD data0,WORD  data1,WORD  data2);

/*from hardeware driver*/
void CanDeviceInit(unsigned char Ch, unsigned long Baud);
void CanRxFrameMailbox0(void);
void CanTxFrameMailbox4(void);
void CanCMRSet(void);

#include "can2.h"
BOOL Can2TxComplete(void);
BOOL Can1TxComplete(void);
BOOL Can2TxFrameMailbox(PCAN_FRAM pfram);
BOOL Can2RxFrameMailbox(PCAN_FRAM pfram);
void  Can2Loop(void);
void Can2DeviceInit(unsigned long Baud);
void CanDisableTx(BOOL bdisable);

#endif
