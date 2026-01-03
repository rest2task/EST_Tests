/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    app_driver_func.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:            2013.11.18

Description:  Driver inteface parameters and functions declaration

Others:

History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef _APP_DRIVER_FUNC_H_
#define _APP_DRIVER_FUNC_H_

#define APP_CODE //APP_CODE must be defined when the file is used in APP

#include "types.h"

//NAND FLASH Parameters
#define NAND_PAGE_SIZE		(2 * 1024)
#define NAND_BLOCK_SIZE		(128 * 1024)


//#define A920_620_DRV //contain driver in "ec_master\driver\app\A620_A920_drive" when defined

/**  DEVice ID difinition  **/
#define DEV_TIMER			0x0
#define DEV_UART			0x1
#define DEV_ECAT			0x2
#define DEV_WDT				0x3
#define DEV_I2C				0x4
#define DEV_NAND			0x5
#define DEV_GPIO			0x6
#define DEV_PWM				0x7
#define DEV_AD				0x8
#define DEV_TEMP			0x9
#define DEV_CAN				0xA
#define DEV_DA				0xB

/**  OPeration Mode difinition(lower 16-bit of OP Mode)  **/
#define DEV_OP_READ			0 
#define DEV_OP_WRITE		1
#define DEV_OP_RW			2			//used in EtherCAT
#define DEV_OP_CONFIG		3			//used in EtherCAT/GPIO/TIMER etc.
#define DEV_OP_ERASE		4			//used in NAND Flash

#define DEV_OP_BCAST		(0x1 << 16) //Broadcast access, used in EtherCAT now
#define DEV_OP_INT			(0x1 << 17) //Operation with Interrupt, used in I2C now

/**  return value  **/
#define DEV_OK				0
#define DEV_ID_ERR			-1
#define DEV_OP_ERR			-2
/**  data in SRAM for APP and driver exchanging  **/
#define DRIER_ENTRY			(*(volatile unsigned int *)0x0C001FFC)


#define GPIO_GRP_NUM		8			//GPIO Group(GPA--GPQ), 0 for GPA, 1, for GPB...

#define FORCE_RESET_PIN  0                      //PIN number in GPIO group for Force Reset
#define FORCE_UPDATE_PIN 1                      //PIN number in GPIO group for Force Update
#define LED_RUN_PIN      2                      //PIN number in GPIO group for Run LED
#define LED_UART_PIN     3                      //PIN number in GPIO group for UART LED
#define LED_CANA_PIN     4                      //PIN number in GPIO group for CAN A LED
#define LED_CANB_PIN     5                      //PIN number in GPIO group for CAN B LED
#define SENSER_PS5C_PIN  6                      //PIN number in GPIO group for Senser PS5C
#define SENSER_PS6C_PIN  7                      //PIN number in GPIO group for Senser PS6C
#define KEY_RESET_PIN    8                      //PIN number in GPIO group for Reset Key
#define KEY_MANUAL_PIN   9                      //PIN number in GPIO group for Manual Key
#define TEST_TIME_PIN    10                     //PIN number in GPIO group for Time Test IO


/**************  EtherCAT driver and APP interface  **************/

//the max number of slave of one task, the First invalid one must be 0xFFFF, so the real max value is  MAX_SLAVES_IN_TASK - 1
#define MAX_SLAVES_IN_TASK 20
typedef struct app_ecat_interface
{
    uint8_t		*data_buff;				//data buffer pointer for EtherCAT and APP data exchanging
    uint16_t	station_addr;
    uint16_t	sent_1ms;					//Data Sent time
    uint16_t	ecatprocess;				//EtherCAT Dataprocessing flag
    uint8_t		rxpdo_num;					//Number of RXPDO
    uint8_t		txpdo_num;					//Number of TXPDO
} app_ecat_interface_t;

//Error codes of EtherCAT access
#define EC_ERR_NONE			0x0			//NO ERROR
#define EC_ERR_CMD			0x1			//wrong OP code(not 0, 1, or 2)
#define EC_ERR_LEN			0x2			//data length invalid
#define EC_ERR_WKC			0x3			//wrong WorKing Counter
#define EC_ERR_NO_SLV		0x4			//station address invalid
#define EC_ERR_COE			0x5			//COE communication Error
#define EC_ERR_TSK_BUSY		0x6			//Requested task is busy
#define EC_ERR_TIMEOUT		0x7			//ECAT Datagram Timeout error
#define EC_ERR_PROTOCOL		0x8			//Protocol is not supported
#define EC_ERR_SLV_MEM		0x9			//No more Slave buffer in APP interface
#define EC_ERR_EEPROM		0xA			//EEPROM Access Error
#define EC_DGRAM_ERROR		0xB			//State of returned Datagram is Error

#define EC_ERR_OTHER		0xF			//other errors

//offset address in data buffer
#define EC_OFF_LENGTH		0			//offset of data length
#define EC_OFF_CONTROL		2			//offset of control word
#define EC_OFF_TYPE			4			//offset of Transfer Type
#define EC_OFF_SLV_ADDR		6			//offset of slave station address
#define EC_OFF_INDEX		8			//offset of reg address or Index of Object Directory for CANopen
#define EC_OFF_SUBINDEX		10			//offset of sub-Index in Object with Index
#define EC_OFF_DATA			12			//offset of active data
#define EC_OFF_BUF_HDR		EC_OFF_DATA	//Size of Buffer header 

#define EC_DATAGRAM_RETRIES 3			//retries times when datagram timeout 

#ifdef APP_CODE
#define SET_CON_DATA_ACTIVE(con)    (con | (0x1<<0))					//Set data active flag
#define SET_CON_BROADCAST(con)		(con | (0x1<<3))					//Set BroadCast flag
#define SET_CON_ACCESS(con, acc)    ((con & (~(0x7<<1))) | (acc<<1))	//control[2:1], Access type(R/W/RW, Broadcast)
#define GET_STA_ERROR(con)          ((con & (0xf << 4))>>4)			//control[19:16], EtherCAT access error code
#define GET_STA_FINISH(con)         ((con & (0x1 << 8)) >> 8)			//control[20], EtherCAT access finish flag

#define GET_CON_DATA_ACTIVE(con)    ((con & (0x1<<0))>>0)				//control[0], data active flag, high active
#define CLR_CON_DATA_ACTIVE(con)    (con & (~(0x1<<0)))				//clear data active flag
#define GET_CON_ACCESS(con)         ((con & (0x3<<1))>>1)				//control[2:1], Access type(R/W/RW)
#define GET_CON_BROADCAST(con)      ((con & (0x1<<3))>>3)				//control[3], broadcast access type, high active
#define SET_STA_ERROR(con,err_code) ((con & (~(0xf << 4))) | ((err_code & 0xf) << 4))             //control[7:4], EtherCAT access error code
#define SET_STA_FINISH(con)         (con | (0x1 << 8))                  //control[8], EtherCAT access finish flag

//Supported Mailbox protocol
#define EC_MBX_ERR			0x0			//Error
#define EC_MBX_AOE			0x1			//Vendor Specific(Beckhoff: AoE-ADS over EtherCAT)
#define EC_MBX_EOE			0x2			//Ethernet over EtherCAT
#define EC_MBX_COE			0x3			//CANopen over EtherCAT
#define EC_MBX_FOE			0x4			//File Access over EtherCAT
#define EC_MBX_SOE			0x5			//Servo Profile over EtherCAT
#define EC_MBX_VOE			0xF			//Vendor Specific(VoE)  

typedef struct {
    u16 DataLen;
    u16 ConWord;
    u16 TransferType;
    u16 EscAddress;
    u16 OdIndex;
    u16 OdSubIndex;
} EcatReq_t;
#endif
/*****************************************************************/
#ifdef APP_CODE
//max bytes in one EtherCAT datagram
#define ECAT_MAX_DATA_SIZE 1486

extern int (*AppDriverFunc)(u32 dev_id, u32 chip, u8 *data, u32 data_len, u8 *addr, u32 addr_len, u32 op_mode);

extern app_ecat_interface_t *ec_app_inter[];
//PDO data buffer
extern u8 *app_rxpdo_buf[];
extern u8 *app_txpdo_buf[];
extern u8 *rxpdo_state;
extern int txpdo_diff;			//pdo?車那?米?????那㊣??2?㏒?㊣那℅??a25[250us]

//Calculate PDO package loss rate
extern u32 rxpdo_send_cnt, rxpdo_rec_cnt;
extern u32 txpdo_send_cnt, txpdo_rec_cnt;
extern u32 rxpdo_queue, rxpdo_sent;
extern u32 txpdo_queue, txpdo_sent;


void app_data_process(void);

#define EcatProcess app_data_process
//Driver interface functions declaration


//EtherCAT Operation
//ECAT receive
void ecat_receive(void);
//ECAT Send
void ecat_send(void);
//add datagram to send queue
void ecat_queue(void);
//slave state check and dynamic sync
void ecat_state_check(void);
//ECAT receive and send
 u32 get_jiffies(void);
void ecat_net_op(void);
 void ecat_master_set(u32 val);

 
 int ecat_task_req(void);

extern u32	ecat_sr_flag; //EtherCAT datagrams send/received flag; 1 sent, 0 for received
extern u32	ecat_rec_flag;
extern u32 ecat_rec_wdt_flag;
//return PDO cycle period in tick
 u32  pdo_cycle_period(void);

#else
int app_driver_func(uint32_t dev_id, uint32_t chip, uint8_t *data, uint32_t data_len, uint8_t *addr, uint32_t addr_len, uint32_t op_mode);
#endif //end of #ifdef APP_CODE
#endif //end of #ifndef _APP_DRIVER_FUNC_H_


