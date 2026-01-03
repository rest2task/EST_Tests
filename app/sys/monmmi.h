#ifndef _MONMMI_H_
#define _MONMMI_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"

#define MON_CYCLE_ST_ID			0x01
#define MON_PRE_DEF_ID_END		0x100
#define MON_RESET_STOP_MS		500

#define TOTAL_IND_NUM		250
#define MON_ITEM_NUM		8
#define MON_MIN_PERIOD		50	//ms
#define REC_MIN_PERIOD		2	//ms

#define REC_SEND_NUM	2	//状态值触发发送次数，当前为2次

#define MIN_REC_TOTAL_MS	200
 typedef struct tyMON_IND
{
	UI16*	pdat;
	UI16	dat_len;			//有效数据长度， 以WORD为单位
	UI16	len;				//该索引要求的数据长度，以WORD为单位
}MON_IND, *PMON_IND;

typedef BOOL (*MON_TX_FUNC)(void);

typedef struct tyMMI_MON_ACT
{
	UI8		type;
	UI16	intval_ms;
	MON_TX_FUNC func;
	UI32	last_tx_ms;
}MMI_MON_ACT;

typedef struct tyMON_ITEM
{
	UI16	set_id;
	UI16	period_ms;
	UI16	ind_start;
	UI16	ind_num;
	UI16	data_len;				//以字节为单位
	UI32	last_tx_ms;
	UI16	trig_send;				//触发时发送 :0不开启，1开启 >1代表发送次数				
}MON_ITEM, *PMON_ITEM;

typedef struct MMI_MON_ST
{
	UI8		item_num;
	UI8		next_free_mon;
	UI16	ind_num;
	//UI32	mon_start_ms;
	MON_ITEM	mon[MON_ITEM_NUM];
	MON_IND		ind[TOTAL_IND_NUM];
}MMI_MON_ST, *PMMI_MON_ST;

#ifdef DM9000_ENABLE
	#define DAT_REC_NUM				6// 4					//支持的同时画线的最大画面数量，如开关模曲线画面为两张图，一张关模，一张开模
	#define DAT_SAMP_MAX_NUM		40//6					//一张曲线画面中，支持的最大画面的条数
#else
	#define DAT_REC_NUM				4
	#define DAT_SAMP_MAX_NUM		6
#endif
#define DAT_REC_BUF_SIZE		120					/*使发送数据时，通信包的长度不超过512字节, 需为1、2、3、4、5、6的公约数*/
#define DATREC_SEND_MIN_PERIOD	100
#define TRIG_NUM				2

#define TRIG_RISING				0					/*上升沿触发*/
#define TRIG_FALLING			1					/*下降沿触发*/

#define TRIG_AND				0					/*触发逻辑与*/
#define TRIG_OR					1					/*触发逻辑或*/

#define REC_SCOP_CONT				2				/*示波器连续记录*/	
#define REC_SCOP_SINGLE				3				/*示波器单次触发*/	

typedef enum enDAT_REC_STATE
{
	REC_IDLE = 0,					/*记录空闲*/
	REC_TRIGGER,					/*记录等待触发*/
	REC_START,						/*记录开始*/
	REC_STOP,						/*记录停止*/
	REC_SEND,						/*单次触发完成，数据发送状态*/
	REC_TRIGGER_END1,				/*准备停止1 状态*/
}DAT_REC_STATE;

typedef struct tyTRIG_CON
{
	UI16*	trig_pdat;				/*触发器数据指针*/
	UI16	threshold;				/*触发器阀值*/
	UI16	last_value;				/*触发器初始值*/
	UI8		trig_mode;				/*触发模式，低2位有效，0上升沿触发, 1下降沿触发*/
	UI8		and_or;					/*触发逻辑关系，低2位有效， 0逻辑与，1逻辑或*/
}TRIG_CON, *PTRIG_CON;

typedef struct tyDAT_REC
{
	UI8		prot_id;					//协议索引，为0代表是面板发下来的数据   为1  代表是pc机示波器监控软件
	UI16	Prot_cmd;					//保存面板下发的命令，用于区分是面板发送下来的，还是pc电脑后台发送下来的，用于上传时区分不同的命令上传
	UI16	rec_state;					//曲线当前所处的状态，0 空闲，1 等待触发，2  记录开始。。。。
	UI16	rec_id;						//记录的id，由面板自己定义，
	UI16	trigger_id;					/*触发id,0x0010到0xFEFF时为动作ID,还可以为0,1,2,3等值 */
	UI16	rec_period;					/*数据采样周期, , 100us单位*/

	UI16	rec_total_ms;				/*数据记录总时间, ms*/
	UI16	rec_ticks;					/*已采样周期数*/
	UI16	rec_start_ticks;			/*缓存第一个数据采用周期*/
	UI8		rec_dat_num;				/*一个采样周期内记录的数据个数，如开关模曲线，要采集电子尺位置，反馈压力，反馈速度，电子尺运动速度共4个点*/
	UI16	rec_len;					/*已存储在采样数据缓存的个数*/
	int		rec_sample_tm;				/*上次采样的毫秒时间，示波器记录为主机运行周期数*/
	UI32	rec_start_ms;				/*数据记录的开始时间，ms*/
	UI16*	rec_pdat[DAT_SAMP_MAX_NUM];	/*一个采样周期要采样的数据指针*/
	UI16	rec_buf[DAT_REC_BUF_SIZE];	/*采样数据缓存*/
	UI16*	prec_buf;					/*采样数据缓存指针*/
	UI16	buf_size;					/*缓存大小，word计算*/
	UI16	send_offset;				/*已发送数据个数， word计算*/
	UI16	trig_num;					/*触发条件个数*/
	TRIG_CON trig_con[TRIG_NUM];		/*触发条件, trigger_id = 2, 3时有效*/
}DAT_REC, *PDAT_REC;

typedef struct tyDAT_REC_ST
{
	DAT_REC		dat_rec[DAT_REC_NUM];		
}DAT_REC_ST, *PDAT_REC_ST;


typedef struct tySTATE_MAPPING
{
	UI32		dataid_old;
	UI32		dataid_new;
	UI16	len;

	
	PMON_IND pmon_ind[3];
	UI16	pmon_offset[3];
	UI16	set_id[3];
	UI16	addr_m[3];
	UI16*	addr_z[3];
}STATE_MAPPING,*PSTATE_MAPPING;

BOOL SendMonCycleRunSt(void);
void MMIMonLoop(void);
void MMIMonReset(void);
void DataSetCmdRx(PPROT_DATSET_REQ_PKT ppkt);
void DataRecCmdRx(PPROT_DATREC_REQ_PKT ppkt, UI8 prot_id, UI16 len,UI16 prot_cmd);
void DataRecOPRx(PPROT_DATREC_OP_PKT ppkt, UI8 prot_id);
void DataRec_Stop(PPROT_DATREC_OP_PKT ppkt, UI8 prot_id);

void MMIMonActRunNotify(UI16 act_id);
void MMIMonActEndNotify(UI16 act_id);
void MMIMonRecLoop(void);
void StateMappingInit(void);
void Data_Pc_Stop(void);


#ifdef __cplusplus
}
#endif

#endif


