#ifndef _SERVOECAT_H_
#define _SERVOECAT_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "acttype.h"
#include "Servo_dic.h"
#include "ServoEcat_dic.h"



#define TYPE_IS580 1
#define TYPE_IS620 2
#define TYPE_ES750 3
#define TYPE_ES750_2 4

//#define SERVODATARD	0

#define PACKETLOSSMAXMS	500//1000	驱动ES810 的标准改成了500，超过500无效    //最大EtherCat通讯丢包时间	单位:ms
#define PACKETLOSSMINMS	500		//最小EtherCat通讯丢包时间 单位:ms
#define MAXTORQUE	3000	//驱动器最大扭矩设定	300%



//伺服相应状态
#define SERVO_STATE_INIT	0x00
#define SERVO_STATE_WEAK	0x40
#define SERVO_STATE_NOERR	0x50
#define SERVO_STATE_RDY	0x31
#define SERVO_STATE_WAIT	0x33
#define SERVO_STATE_RUN	0x37
#define SERVO_STATE_ERR	0x18
#define SERVO_STATE_ERRSTOP	0x1F
#define SERVO_STATE_FSTOP	0x17

typedef enum enSERVO_CTRL_MODE
{
	SERVO_CTRL_MODE_SPEED = 0x00,	//速度控制
	SERVO_CTRL_MODE_HYDR = 0x07		//液压控制
}SERVO_CTRL_MODE;


typedef struct tySERVOECAT
{
	PART				part;
	PSERVOECAT_STATE	st;

	ACT					act[SERVOECAT_ACT_SN_END - 1];
	
	UI16			servo_max_rpm[MAX_SERVO_NUM];	//驱动器内实际电机最大转速
	UI16			motortemp_type[MAX_SERVO_NUM];	//电机温度类型	0:不使用 1:kty	2:ptc

	UI16			cycle_tm_out_cn;	//主循环周期超时统计(全自动时)
	UI16			cycle_tm_out_check_start100ms;	//超时检查起始时间(100ms)

	UI16			ecat_init_step;			//Ecat初始化步骤
	UI16			ecat_init_step_tm;	

	UI16			ecat_sdorx_starttm;
	UI16			ecat_sdotx_starttm;
}SERVOECAT, *PSERVOECAT;

/*===========================================================================+
|          SDO通讯相关定义                                  |
+===========================================================================*/

#define	SDOTXDTBUFFSIZE	0x20
#define SDORXDTBUFFSIZE	0x20

#define	SDOTXBUFFSIZE	0x20
#define SDORXBUFFSIZE	0x20


/*读写步的宏定义*/
#define ATTONITY		0x00
#define	SDO_ARRANGE		0x01
#define	SDO_SENDING		0x02
#define	SDO_WAITASN		0x03
#define	SDO_END			0x04
#define SDO_FULL		0x05
#define	SDO_ERR			0x06

/*--------------*/



typedef struct	EN_SDORX_DATAFORM
{
	UI16				function;			//功能识别。0x20为功能码，0x60为COE的SDO。
	UI16*				lcladresshead_rx_p;	//内部数据首地址关联地址
	UI16				extadresshead_rx;	//外部数据首地址
	UI16				adresslen_rx;		//数据长度	
	UI16				servono;			//驱动器编号
	UI16				step;				//该数据在SDO读写中的步
	UI16				value;				//值

	//低8位符号:0:加。1:减。2:乘。3:除	
	//高8位位特殊处理
	UI16				sign;				
}SDORX_DATAFORM, *PSDORX_DATAFORM;
typedef struct	EN_SDOTX_DATAFORM
{
	UI16				function;			//功能识别。0x20为功能码，0x60为COE的SDO。
	UI16				lcldata;			//内部数据
	UI16				extadresshead_tx;	//外部数据首地址
	UI16				adresslen_tx;		//数据长度	
	UI16				servono;			//驱动器编号
	UI16				step;				//该数据在SDO读写中的步
}SDOTX_DATAFORM, *PSDOTX_DATAFORM;


typedef struct tag_SDOTXBUF SDOTXBUF;
struct tag_SDOTXBUF
  {
    PSDOTX_DATAFORM       p_txbuf_start;      /* Pointer to the transmit buffer start address */
    PSDOTX_DATAFORM       p_sdofill_txbuf;       /* Pointer to the transmit buffer current fill address */
    PSDOTX_DATAFORM       p_lclfill_txbuf;       /* Pointer to the transmit buffer current fill address */
	PSDOTX_DATAFORM       p_txbuf_end;        /* Pointer to the transmit buffer end address */   

	
    PSDOTX_DATAFORM       p_txbuf_start_dt;      /* Pointer to the transmit buffer start address */
    PSDOTX_DATAFORM       p_sdofill_txbuf_dt;       /* Pointer to the transmit buffer current fill address */
    PSDOTX_DATAFORM       p_lclfill_txbuf_dt;       /* Pointer to the transmit buffer current fill address */
	PSDOTX_DATAFORM       p_txbuf_end_dt;        /* Pointer to the transmit buffer end address */   
  };	
/*----------------------------------------------------------------------------*/
typedef struct tag_SDORXBUF SDORXBUF;
struct tag_SDORXBUF
  {
    PSDORX_DATAFORM       p_rxbuf_start;      /* Pointer to the receive buffer start address */
    PSDORX_DATAFORM       p_sdopick_rxbuf;       /* Pointer to the receive buffer current pick address */	
    PSDORX_DATAFORM       p_lclpick_rxbuf;       /* Pointer to the receive buffer current pick address */
    PSDORX_DATAFORM       p_rxbuf_end;        /* Pointer to the receive buffer end address */   

	
    PSDORX_DATAFORM       p_rxbuf_start_dt;      /* Pointer to the receive buffer start address */
    PSDORX_DATAFORM       p_sdopick_rxbuf_dt;       /* Pointer to the receive buffer current pick address */	
    PSDORX_DATAFORM       p_lclpick_rxbuf_dt;       /* Pointer to the receive buffer current pick address */
    PSDORX_DATAFORM       p_rxbuf_end_dt;        /* Pointer to the receive buffer end address */   
  };	


extern  SDOTXBUF     g_sodtxbuf;
extern  SDORXBUF     g_sdorxbuf;

/*----------------------------------------------------------------------------*/


typedef enum enCOM_SDO_RET
{
	RX_SDO_FREE = 0,		/*SDO读数据空闲*/
	RX_SDO_BUSY,			/*SDO读数据忙碌，需要等待*/
	RX_SDO_STAND,			/*SDO读数据等待，等上一个数据读完成*/
	RX_SDO_END,				/*SDO读数据完成，可以进行下一个*/
	RX_SDO_ERR,				/*SDO读数据错误，需要特殊处理*/
	
	TX_SDO_FREE = 0,		/*SDO读数据空闲*/
	TX_SDO_BUSY,			/*SDO读数据忙碌，需要等待*/
	TX_SDO_STAND,			/*SDO读数据等待，等上一个数据读完成*/
	TX_SDO_END,				/*SDO读数据完成，可以进行下一个*/
	TX_SDO_ERR,				/*SDO读数据错误，需要特殊处理*/
	INVALD_SDO_RET	= 0xffff
}COM_SDO_RET;



//驱动器数据读写头文件
typedef struct tyPRO_SDO_RX_PKT_HD
{
	UI16	type;			//类型	1:读数据	2:写数据
	UI8		ver;			//驱动器数据版本， 目前为0x01
	UI8		row;			//采集数据行数
	UI16	max_row;		//允许的最大行数
	UI16	row_len;		//每行的数据长度
	UI8		pkt_state;		//包状态	0:读取失败	1:读取完成	2:被占用	3:申请缓存失败
	UI8		def;			//保留
}PRO_SDO_RX_PKT_HD, *PPRO_SDO_RX_PKT_HD;

//驱动器数据读行数据
typedef struct tyPRO_SDO_RX_LOG
{
	UI8 	servo_sn;			//驱动器编号
	UI8		servo_len;			//驱动器数据长度
	UI16	servo_func;			//驱动器功能码
	UI16 	servo_addr;			//驱动器地址
	UI16	def[3];				//保留为了与下方反馈数据的地址对齐
}PRO_SDO_RX_LOG, *PPRO_SDO_RX_LOG;

//驱动器数据反馈行数据
typedef struct tyPRO_SDO_RE_LOG
{
	UI8 	servo_sn;			//驱动器编号
	UI8		servo_len;			//驱动器数据长度
	UI16	servo_func;			//驱动器功能码
	UI16 	servo_addr;			//驱动器地址
	UI16	servo_val_l;		//驱动器数据低位
	UI16	servo_val_h;		//驱动器数据高位
	UI16	def;				//保留
}PRO_SDO_RE_LOG, *PPRO_SDO_RE_LOG;



typedef struct tyPROSERVOECAT
{
	PPRO_SDO_RE_LOG ptbl_Cache;		//缓存，存放需要读取的数据列表
	PRO_SDO_RX_PKT_HD	pro_sdo_rx_pkt[2];	//读驱动器数据包头0:为缓存数据 1:为调用数据
	PPRO_SDO_RE_LOG ppro_pick_now;	//当前读取数据块的首地址
	UI16			pro_step;	
	UI16			pro_row_rt[2];
	UI16				pkt_id[2];
	UI16			pro_readstartTm_100ms;

	UI16			value_ret_h;
	UI16			value_ret_l;
	UI16*			pro_rx_step;
	UI32			readstartTm_100ms;	
}PROSERVOECAT, *PPROSERVOECAT;


extern WORD	SDOBUSYFLAG;
extern SERVOECAT m_servoecat;


#define SERVOECAT_INIT_INFO	{{ST_ID, CAL_LEN(SERVOECAT_STATE)}}


void InitSdoProcess(void);
void SdoProcess(void);
UI16* SdoDataUpdata(UI8 ServoNo,UI16 function, UI16 lcldata, UI16 extadresshead);
UI16* SdoDataDownload(UI8 ServoNo,UI16 function, UI16* lcladresshead, UI16 extadresshead, UI16 value, UI8 sign);
UI16* SdoData32Updata(UI8 ServoNo,UI16 function, UI32 lcldata,UI16* lcladressheadh, UI16 extadresshead);
UI16* SdoData32Download(UI8 ServoNo,UI16 function, UI16* lcladressheadl,UI16* lcladressheadh, UI16 extadresshead, UI16 value, UI8 sign);

UI16* SdoDtDataUpdata(UI8 ServoNo,UI16 function, UI16 lcldata, UI16 extadresshead,UI8 DataLenth);
UI16* SdoDtDataDownload(UI8 ServoNo,UI16 function, UI16* lcladresshead, UI16 extadresshead, UI16 value, UI8 sign);
UI16* SdoDtData32Updata(UI8 ServoNo,UI16 function, UI32 lcldata, UI16 extadresshead);
UI16* SdoDtData32Download(UI8 ServoNo,UI16 function, UI16* lcladressheadl,UI16* lcladressheadh, UI16 extadresshead, UI16 value, UI8 sign);
void Set_H0C35(UI16 data);
void Set_H1100(UI16 data);
void SetServoMode(UI16 bit);


void EtherCatTest(BOOL outre);
void InitSdoDataRWBef(void);

BOOL InitServoEcatPart(void);
UI32 ServoEcatCmd(UI32 cmd, UI32 para);
BOOL ChkEncoderPPR(void);
void FreeServoDatCache(void);
BOOL ChkServoErrPro(void);
BOOL ServoSdoTurning(PPRO_SDO_RX_PKT_HD ppkt, UI16 pkt_id);


							 
#ifdef __cplusplus
}
#endif

#endif




