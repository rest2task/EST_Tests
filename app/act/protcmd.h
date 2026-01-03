#ifndef _PROTCMD_H_
#define _PROTCMD_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "prot.h"
#include "ioproc.h"
#include "servo_dic.h"
#include "da_dic.h"
#include "admeter_dic.h"



#pragma  pack(1)

#define CMD_OPT_BITS	0xf000
#define CMD_BITS		0xfff
#define CMD_ACK_BIT	0x8000

enum enPROT_CMD
{
	ACK_CMD	= 0x000,
	SUB_REQ_CMD,//子部件请求
	SUB_RSP_CMD,//子部件回复
	DAT_REQ_CMD,
	DAT_RSP_CMD,
	DAT_SAV_CMD,
	SUB_ACTV_CMD = 0x010,
	DAT_ACTV_CMD = 0x011,

	BOOTUP_MSG = 0x020,
	BOOTUP_MSG_UDP = 0x0220,		//网络通讯时的boot启动命令
	KEYPAD_CMD = 0x021,
	TSK_ST_MSG	= 0x022,

	//多部件集合数据
	DATASET_REQ_CMD = 0x024,
	DATASET_RSP_CMD = 0x025,

	/*实时数据记录*/
	DATREC_REQ_CMD = 0x026,
	DATREC_RSP_CMD = 0x027,
	DATREC_OP_CMD = 0x028,
	
	/*表通信命令*/
	TBL_REQ_CMD = 0x031,
	TBL_RSP_CMD = 0x032,
	MULTI_TBL_REQ_CMD = 0x033,
	MULTI_TBL_RSP_CMD = 0x034,

	/*分组多包通信*/
	PKT_REQ_CMD = 0x040,
	PKT_RSP_CMD = 0x041,

	//上位机发送的控制命令
	SYS_CTRL_CMD = 0x50,

	UPGRADE_RESET_CMD = 0x51,			//升级重启

    //下位机发送给上位机的控制命令
    SYS_CTRL_CPU_TO_PANEL_CMD = 0x60,
    
	/*智能注射，智能卡协议*/
	/*智能注射   控制器向智能卡发送射出实时数据*/
	iINJ_INJ_REAL_CMD = 0x84,
	/*智能注射   控制器向智能卡发送温度实时数据*/
	iINJ_TMP_REAL_CMD = 0x85,

	/*智能注射  控制器向智能卡发送动作状态*/
	iINJ_ACT_ST_CMD = 0x86,
	/*智能注射  控制器向智能卡成品数据*/
	iINJ_PRODUCT_INFO_CMD = 0x87,

	/*智能注射  控制器查询智能卡状态*/
	iINJ_QUERY_ST_CMD = 0x88, 	
	/*智能注射  智能卡回复状态*/
	iINJ_REPLY_ST_CMD = 0x89,	
	/*2G物联网模块通信协议*/
	TNET_2GMOD_CMD_BEGIN = 0x100,
	TNET_2GMOD_CMD_END= 0x1FF,	

		/*面板读写驱动器数据通讯命令*/
	PKT_SERVO_REQ_CMD = 0x101,	
	PKT_SERVO_RSP_CMD = 0x102,

	//警报时数据上传
	ALARM_DATA_TX_CMD = 0x110,

	COMM_RX_TEST_UDP = 0x170,	//udp网络通讯测试命令，面板给主机发送170
	COMM_TX_TEST_UDP = 0x171,	//udp网络通讯测试命令，主机给面板发送171
	
	DATREC_QXRC_CMD = 0x0202,		//后台曲线数据下发命令
	DATREC_QXTX_CMD = 0x0203,		//后台曲线数据上传命令，发送给面板
	DATREC_QXOP_CMD = 0x0204,		//后台曲线数据停止命令
	/*580+ethercat扩展卡自学习*/
	MOTOR_TURNING_CMD = 0x0250,
};

enum enSYS_CTRL_CMD
{
	CFG_RESET_CMD	= 0x0001,			//用户配置数据复位

	ENTER_ADJ_CMD = 0x0002,			//进入调机状态
	EXIT_ADJ_CMD = 0x0003,			//退出调机状态

	DA_ADJ_CMD = 0x0004,				//DA最大最小值调整

	TEMP_ZERO_CMD = 0x0005, 			//温度归零调机
	TEMP_ADJ_CMD = 0x0006, 				//温度校准调机

	FORCE_PO_CMD = 0x0007,				//输出点强制输出
	EXIT_PO_CMD = 0x0008,				//退出输出点强制输出


	ENTER_DA_CMD = 0x0009,				//进入DA曲线调试
	EXIT_DA_CMD = 0x000A,				//退出DA曲线调试

	PRESS_MAX_ADJ_CMD = 0x0000B,		//DA压力最大值调整
	FLOW_MAX_ADJ_CMD = 0x0000C,			//DA流量最大值调整

	PRESS_CUR_CMD = 0x0000D,			//DA压力曲线调整
	FLOW_CUR_CMD = 0x0000E,				//DA流量曲线调整
	DA_PAIR_CUR_CMD = 0x0000F,			//DA曲线压力流量同时调整
	DA_CUR_SAVE_CMD = 0x000010,			//保存DA曲线

	METER_LENGTH_ADJ_CMD = 0x0011,	//电子尺长度调整
	METER_ZERO_ADJ_CMD = 0x0012,			//电子尺归零
	MACHINE_CFG_MODI_CMD = 0x0013,			//系统机器参数修改

	DA_CFG_RESET_CMD = 0x0014,			//DA曲线及最大值 最小值重置.
	CAN_TEST_CMD = 0x0015	,			//CAN测试命令

	AMB_TEMP_ADJ_CMD = 0x0016,			//室温校准

	CFG_USER_TO_FACTORY_CMD = 0x0017,    //用户配置数据复制到出厂配置数据

	ENTER_DALEARN_CMD = 0x0018,    //进入自整定画面
	EXIT_DALEARN_CMD = 0x0019,    //退出自整定画面
	START_DALEARN_CMD = 0x001A,    //自整定开始
	END_DALEARN_CMD = 0x001B,    //自整定结束
	DA_MAX_ADJOVER_CMD = 0x001C,		   //DA最大值调整结束

	PRESS_DAMIN_ADJ_CMD = 0x00001D,		//DA压力最小值调整
	FLOW_DAMIN_ADJ_CMD = 0x0001E,		//DA流量最小值调整
	MOLD_ADJ_CMD = 0x0001F,		       //松模调模采样初始化	
	CHGBACK_ADJ_CMD = 0x0020,           //背压线性重新生成
	CFG_ERASE_CMD = 0x0021,				//清楚主机配置内容，重启后主机配置被重写成程序内置的缺省配置。
	ZEROLINE_ADJ_CMD = 0x0022,           //直线线性的生成

	/*面板驱动器更新时，主机不能进行CAN通信*/
	CAN_FORBID_CMD = 0x0023,			//面板更新驱动器软件，禁止主机CAN通信
	CAN_ENBALE_CMD = 0x0024,				//更新完成，允许主机CAN通信
	
	PAYMENT_TIMEOUT_CMD = 0x25,                 //分期付款时间到命令	
	OPEN_RELVLV_CMD = 0x0026,			//每隔一定模数后开启一个泄压阀

	/*pdat[0]强制优化密码	pdat[1]优化的温度模块(bit) pdat[2]优化的温度段数(bit)*/
	COMM_SUBCMD_TEMPOPTIMIZE_CMD = 0x30,                     //温度优化
	CAN_TEST_E502 = 0x33, 				//E502 can通讯测试命令	20190410
	ERR_CNT_CLEAR = 0x34,				//警报次数清空		20190410
	CAN_TEST_EXIT = 0x35, 				//E505,E502can通讯测试画面退出

	/*pdat[0]子类id 例如pump1号部件的mh	0x0F12 */
	SYS_DATA_SAVE_CMD = 0x40,			//主机参数保存
	SLOPE_CALIBRATION = 0x41,			//斜率校准 pdat[0] = 0x01开始	0xFF结束	pdat[1] = 动作id 例如开关模自学习 0x2010 开合模1号部件
	TEMP_PID_UPDATE_CMD = 0x42,			//温度控制的pid参数还原为最近一次学习的参数。 pdat[0] = 温度部件序号 1开始	0xFF表示所有部件
	CAN_TEST_E501 = 0x43,				//E501温度扩展板通讯测试命令
	COMM_MW_TEMPOPTIMIZE_CMD = 0x44	,	//E501温度扩展板优化命令
	CAN_TEST_E700B = 0x45,				//E700B DA扩展板通讯测试命令

	NEED_LUB_CMD = 0x46,				//时间润滑时间到
};

enum enSYS_CTRL_CPU_TO_PANEL_CMD
{
	CFG_CHG_NEED_RESET_CMD = 0x0001,    //需要重启才生效的CODE码发生改变，通知面板提示重启主机
	PO_TIMEOUT_EXIT_CMD = 0x0002,       //IO强制输出超时，通知面板清除IO强制输出表
};

typedef struct tyPROT_ACK_PKT
{
	UI16		recv_cmd;
	UI16		recv_id;
}PROT_ACK_PKT, *PPROT_ACK_PKT;

typedef struct tyPROT_SUB_REQ_PKT
{
	UI16		sub_id_bits;
}PROT_SUB_REQ_PKT, *PPROT_SUB_REQ_PKT;

typedef struct tyPROT_SUB_RSP_PKT
{
	UI16		sub_num;			//申请的子类的个数
	UI16		sub_ind[1];			//每个子类的数据长度，如203B代表，最高位2代表MH的子类，3B代表该MH子类中共有数据3B个
}PROT_SUB_RSP_PKT, *PPROT_SUB_RSP_PKT;

typedef struct tyPROT_DAT_REQ_PKT
{
	UI16		sub_ind;
	UI16		offset;
}PROT_DAT_REQ_PKT, *PPROT_DAT_REQ_PKT;

typedef struct tyPROT_DAT_RSP_PKT
{
	UI16		sub_ind;
	UI16		offset;
	UI16		sub_data[1];
}PROT_DAT_RSP_PKT, *PPROT_DAT_RSP_PKT;

typedef struct tyPROT_BOOTUP_MSG_PKT
{
	UI16	boot_type;
	UI16	error_bits;
	UI16	hw_ver;
	UI16	sw_ver;
}PROT_BOOTUP_MSG_PKT, *PPROT_BOOTUP_MSG_PKT;

//面板发送给下位机键盘命令
typedef struct tyPROT_KEYPAD_CMD_PKT
{
	UI16	keycode;
	UI16	keystatus;
}PROT_KEYPAD_CMD_PKT, *PPROT_KEYPAD_CMD_PKT;

/*下位机发送给面板按键，目前只有手动键*/
typedef struct tyPROT_KEYPAD_RSP_PKT
{
	UI16	keycode;
}PROT_KEYPAD_RSP_PKT, *PPROT_KEYPAD_RSP_PKT;

typedef struct tyPROT_TSK_ST_ITEM
{
	UI16	act_id;
	UI16	tsk_st;
	UI16	step;
	UI16	act_tm_L;					/*动作的设定时间 100ms   高16位*/
	UI16	start_ms_L;					/*动作运行时间 100ms        高16位*/
	UI16	step_ms_L;					/*动作每步的时间, 10ms    高16位*/
	UI16	mov_ms_L;					/*动作运动的时间(面板暂未使用), 100ms  高16位*/

	UI16	act_tm_H;					/*动作的设定时间 100ms   低16位*/
	UI16	start_ms_H;					/*动作运行时间 100ms		低16位*/
	UI16	step_ms_H;					/*动作每步的时间,10ms			低16位*/
	UI16	mov_ms_H;					/*动作运动的时间(面板暂未使用), 100ms			低16位*/
}PROT_TSK_ST_ITEM, *PPROT_TSK_ST_ITEM;

typedef struct tyPROT_TSK_ST_MSG_PKT
{
	UI16	num;
	UI16	item_len;
}PROT_TSK_ST_MSG_PKT, *PPROT_TSK_ST_MSG_PKT;

typedef struct tyPROT_MON_MSG_PKT
{
	UI16	type;
	UI16	len;
	UI16	mon_data[1];
}PROT_MON_MSG_PKT, *PPROT_MON_MSG_PKT;

/*TBL_REQ 和TBL_RSP用 相同包头 格式*/
typedef struct tyPROT_TBL_PKT
{
	UI16	type;				//表的类型,配置表，IO输入表，IO输出表
	UI16	sn;					//表的编号，1，2...
	UI16	start_row;			//表数据的起始位置
	UI16	row_num;			//表数据的结束位置
}PROT_TBL_PKT, *PPROT_TBL_PKT;


typedef struct tyPROT_TBL_HD
{
	UI16	type;
	UI8		ver;
	UI8		sn;
	UI16	row_num;
	UI16	row_len;
	UI16	rev[3];
}PROT_TBL_HD, *PPROT_TBL_HD;

typedef struct tyPROT_TBL_REQ
{
	UI16	type;
	UI16	sn;
}PROT_TBL_REQ, *PPROT_TBL_REQ;

typedef struct tyPROT_MTBL_REQ_PKT
{
	UI16			req_num;
	UI16			rev;
	PROT_TBL_REQ	req[1];
}PROT_MTBL_REQ_PKT, *PPROT_MTBL_REQ_PKT;

typedef struct tyPROT_MTBL_RSP_PKT
{
	UI16			tbl_num;
	UI16			rev;
	PROT_TBL_HD		tbl[1];
}PROT_MTBL_RSP_PKT, *PPROT_MTBL_RSP_PKT;

typedef struct tyDATSET_IND
{
	UI16	part_id;		//该参数在哪一个部件          例:0x0710
	UI16	sub_ind;		//该参数在哪一个子类		例:0x06
	UI16	offset;			//该参数在子类中的偏移    例:0x03，即7160003，代表开关模电子尺
}DATSET_IND, *PDATSET_IND;

typedef struct tyTRIGSET_IND
{
	UI16	part_id;
	UI16	sub_ind;
	UI16	offset;
	UI16	threshold;
	UI8		trig_mod;
	UI8		and_or;
}TRIGSET_IND, *PTRIGSET_IND;

typedef struct tyPROT_DATSET_REQ_PKT
{
	UI16	set_id;
	UI16	period_ms;
	UI16	ind_num;
}PROT_DATSET_REQ_PKT, *PPROT_DATSET_REQ_PKT;

typedef struct tyPROT_DATSET_RSP_PKT
{
	UI16	set_id;
}PROT_DATSET_RSP_PKT, *PPROT_DATSET_RSP_PKT;

typedef struct tyPROT_DATREC_REQ_PKT
{
	UI16	rec_id;			//记录的id，由面板自己定义，主要用于一张画面中，多个曲线图时，可以通过该变量来区分放在哪一张图当中
	UI16	trigger_id;		//记录触发的ID，该值在0x0010-0xFEFF范围时，代表是动作ID,而当为0，表示面板发送开始与停止   1   表示全自动循环记录一次，循环结束关闭，还有2,3,等等
	UI16	rec_period;		//记录数据的采用周期时间
	UI16	rec_total_ms; 	//记录总时间
	UI16	ind_num;		//每个采样周期需要记录的数据个数，最多6个
}PROT_DATREC_REQ_PKT, *PPROT_DATREC_REQ_PKT;

typedef struct tyPROT_DATREC_RSP_PKT
{
	UI16	rec_id;			//记录的id，由面板自己定义，主要用于一张画面中，多个曲线图时，可以通过该变量来区分放在哪一张图当中
	UI16	rec_ticks;		//
	UI16	rec_len;		//
}PROT_DATREC_RSP_PKT, *PPROT_DATREC_RSP_PKT;

typedef enum enDATREC_OP
{
	DATREC_OP_CLOSE = 1,
	DATREC_OP_TX ,
	DATREC_OP_STOP_TX ,
	DATREC_OP_CLR,
	DATREC_OP_STOP,
	DATREC_OP_START,
}DAT_REC_OP;

typedef struct tyPROT_DATREC_OP_PKT
{
	UI16	rec_id;		//记录的id，由面板自己定义，主要用于一张画面中，多个曲线图时，可以通过该变量来区分放在哪一张图当中
	UI16	op_type;
}PROT_DATREC_OP_PKT, *PPROT_DATREC_OP_PKT;

typedef struct tyPROT_FIRST_PACK_PKT
{
	UI16	type_id;
	UI16	total_pkt_num;
	UI32	total_data_len;
}PROT_FIRST_PACK_PKT, *PPROT_FIRST_PACK_PKT;

typedef struct tyPROT_PACK_PKT
{
	UI16	type_id;
}PROT_PACK_PKT, *PPROT_PACK_PKT;

typedef struct tyPROT_CTRL_CMD_PKT
{
	UI16	ctrl_cmd;
	UI16	data_len;
}PROT_CTRL_CMD_PKT, *PPROT_CTRL_CMD_PKT;

typedef struct tyPROT_CTRL_CPU_TO_PANEL_CMD_PKT
{
	UI16	ctrl_cmd;
    UI16	data_len;
}PROT_CTRL_CPU_TO_PANEL_CMD_PKT, *PPROT_CTRL_CPU_TO_PANEL_CMD_PKT;

/*智能注射*/
/********************************************************/
typedef struct tyIINJ_INJ_REAL_PKT
{
	UI16	press;
	UI16	pos;
}IINJ_INJ_REAL_PKT, *PIINJ_INJ_REAL_PKT;

typedef struct tyIINJ_TMP_REAL_PKT
{
	UI16	temp_num;
	UI16	temp[1];
}IINJ_TMP_REAL_PKT, *PIINJ_TMP_REAL_PKT;

typedef struct tyIINJ_ACT_ST_PKT
{
	UI16	act_id;
	UI16	act_st;
}IINJ_ACT_ST_PKT, *PIINJ_ACT_ST_PKT;


typedef struct tyIINJ_PD_INFO_PKT
{
	UI16	vp_switch_pos;
	UI16	cushion_pso;
	UI16	charge_start_pos;
	UI16	inject_tm;
	UI16	charge_tm;
	UI16	cycle_tm;
	UI16	inj_max_press;
	UI16	vp_swicth_press;
}IINJ_PD_INFO_PKT, *PIINJ_PD_INFO_PKT;

typedef struct tyIINJ_QUERY_ST_PKT
{
	UI16	op_cmd;
}IINJ_QUERY_ST_PKT, *PIINJ_QUERY_ST_PKT;

typedef struct tyIINJ_REPLY_ST_PKT
{
	UI16	state;
}IINJ_REPLY_ST_PKT, *PIINJ_REPLY_ST_PKT;


/*警报数据上传结构体*/
typedef struct tyPORT_ALARM_DATA_PKT
{
    UI16	part_id;								//当前的部件id
    UI16	current_err0;							// 当前警报1
    UI16    current_err1;							// 当前警报2
    UI16    servo_err[MAX_SERVO_NUM];              // 伺服警报
    UI16	pi_input[PI_STATUS_NUM];				//输入点状态			
    UI16	po_output[PO_STATUS_NUM];				//输出点状态			
    UI16 	adposi_relative[ADMETER_CH_NUM];		//AD位置相对值
    UI16	ad_relative[ADMETER_CH_NUM];			//实际AD值
    
    UI16	ele_spdout_rpm[MAX_SERVO_NUM];			//电机输出转速
    UI16	ele_real_spd[MAX_SERVO_NUM];			//电机反馈转速
    UI16  	ele_real_torque[MAX_SERVO_NUM]; 		//电机反馈扭矩

    UI16    hydr_out_pres[MAX_SERVO_NUM];			//液压输出压力
    UI16	hydr_out_spd[MAX_SERVO_NUM];			//液压输出流量
    UI16	hydr_real_pres[MAX_SERVO_NUM];			//液压反馈压力

    UI16	da_value[MAX_DA_NUM];					//DA输出值
    UI16	cn_moldnum_l;							//开模数低16位
    UI16	cn_moldnum_h;							//开模数高16位
    UI16	num;									//当前的执行的任务数量
    UI16	item_len;								//每个任务状态的长度，以WORD 计数
    UI16    port_adtemp;                           //记录温度扫描的当期通道
    UI16    ad_tpvalue;                            //记录当期通道的温度ad值
    UI16	wtemp_lm73;								//记录当前温度模块的室温
    UI16    temp_real[16];                         //记录温度实际值
    UI16    reserve[10];                           //预留10个数据的位置
}PORT_ALARM_DATA_PKT,*PPORT_ALARM_DATA_PKT;

typedef struct tyPROT_TSK_ALARM_ST_ITEM
{
	UI16	act_id;						//动作id
	UI16	step;						//动作步
	UI32	start_ms;					//动作运行的时间
}PROT_TSK_ALARM_ST_ITEM, *PPROT_TSK_ALARM_ST_ITEM;


typedef struct tyPROT_DAT_TEST			//通讯测试包
{	
	UI16		recv_cmd;				//返回通讯测试包的命令
	UI16		recv_id;				//返回通讯测试包的动作ID
}PROT_DAT_TEST, *PPROT_DAT_TEST;

/**********************************************************/
#define CFG_DB_PACK_BEGIN_TYPE	0x1000
#define CFG_DB_PACK_END_TYPE  0x1100

/*面板参数文件ID号*/
#define MMI_PARA_FILE_TYPE		0x1100

#define MAKE_SUB_IND(sub_id, len)	(((sub_id) << 12) |((len) &0xfff))
#define	SUB_IND_ID(sub_ind)			(((sub_ind) >> 12) & 0x0f)
#define	SUB_IND_LEN(sub_ind)		((sub_ind) & 0x0fff)

#define PACK_FIN_SN		0xffff
#define PACK_PKT_SIZE	220

#pragma  pack()

//分组包接收状态
typedef struct tyPACK_PKT_RX_ST
{
	UI8		bstart;
	UI8		req_retry;
	UI16	type_id;
	UI16	next_pkt_sn;
	UI16	total_pkt;
	UI32	cur_len;
	UI32	total_len;
	UI16	last_rx_10ms;
	UI16	last_req_tx_10ms;
	UI8*	pcache;
	UI16	cache_size;
	UI16	cache_fill;
}PACK_PKT_RX_ST, *PPACK_PKT_RX_ST;

//分组包发送状态
typedef struct tyPACK_PKT_TX_ST
{
	UI8*	pcache;
	UI16	cache_size;
	int		len;
	UI8		bstart;
	UI16	type_id;
	UI16	pkt_num;
	UI16	last_pkt_len;
}PACK_PKT_TX_ST, *PPACK_PKT_TX_ST;

void* ProtNewPkt(UI16 cmd, UI16 id, BOOL bneed_ack, UI16 pkt_len);
BOOL ProtTxPkt(void* ppkt, UI8 prot_id);
void ProtPktRecv(PPROT_HD phd, UI8 prot_id);
void PackPktTx(void);

#ifdef __cplusplus
}
#endif

#endif

