#ifndef _CTRLCMD_H_
#define _CTRLCMD_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif
#include "typedef.h"

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
	PAYMENT_TIMEOUT_CMD,                 //分期付款时间到命令
    OPENRELVLV_CMD = 0x0026,             //每隔一定模数后开启一个泄压阀 20190610 hz

    TEMPOPTIMIZE_CMD = 0x0030,           //温度优化命令
    EXPADLENGTHADJ_CMD = 0x0031,       //扩展电子尺长度调整  20190522 hz

    CAN_TEST_E505_CMD = 0x0032,        //E505 can通讯测试命令	20190522 hz
    CAN_TEST_E502_CMD = 0x0033,        //E502 can通讯测试命令	20190522 hz
    ERR_CNT_CLEAR_CMD = 0x0034,        //警报次数清空		20190522 hz
    CAN_TEST_EXIT_CMD = 0x0035,         //E505,E502can通讯测试画面退出   20190522 hz

    COMM_SUBCMD_SERVOPUMP_WRITE = 0x0040, //ECAT画面泵浦保存数据
    COMM_SUBCMD_SLOPE_CALIBRATION = 0x0041, //斜率校准
    COMM_SUBCMD_TEMP_PID_RESTORE = 0x0042,  //PID恢复为上一次自学习结果

    //20200407HJM 模温功能
    CAN_TEST_E501_CMD = 0x0043, //E501 can通讯测试命令
    MT_OPTIMIZE_CMD = 0x0044,   //模温优化命令
    CAN_TEST_E700B_CMD = 0x0045, //E700B 通讯测试命令
    COMM_SUBCMD_NEEDLUB = 0x0046, //强制润滑
    COMM_SUBCMD_CAN_TEST_HYDRVLV = 0x0047, //液压阀扩展板通讯测试命令
    COMM_SUBCMD_MGMT_STATE_REQUEST = 0x0048, //麦格米特伺服数据请求命令//20241205 chj 金鹰增加麦格米特模式
    FORCE_VLV_CMD = 0x0049,				//顺序阀强制输出命令

    COMM_RX_SELFSTUDY = 0x0055,  //卓越自学习功能

    SYS_MAIN_CTRL_PANELT_CMD = 0x60,	//下位机控制上位机命令 20230518 chj

    //驱动器参数交互 20191108 hz
    COMM_RX_SERVO_REQ = 0x101,   //读驱动器参数
    COMM_TX_SERVO_REQ = 0x101,   //读驱动器参数
    COMM_RX_SERVO_RSP = 0x102,   //写驱动器参数
    COMM_TX_SERVO_RSP = 0x102,   //写驱动器参数
    //警报时上传数据
    COMM_RX_ALARM_DATA = 0x0110,
    COMM_TX_DATA_RSP_CMD = 0x111,//检测页面向下位机请求指定过程量的值
    COMM_RX_DATA_RSP_CMD = 0x111//接收下位机回复检测页面的指定过程量的值
};

//下位机控制上位机的小命令 20230518 chj
enum enMAIN_CTRL_PANEL_CMD{
    CFG_CHANGE_NEED_RESTART_CMD = 0x1,  //主机通知面板弹出:重启主机
    PO_TIMEOUT_EXIT_CMD = 0x0002,       //IO强制输出超时，通知面板清除IO强制输出表 20250305 chj
};

enum enTYPE_ID{
	TYPE_PLC=0x1000,					//PLC程序。
	TYPE_METER_DEFAULT = 0x1001,		//出厂配置数据。（电子尺）
	TYPE_METER_USER = 0x1002,			//用户配置数据。（电子尺）
	TYPE_TABLE_PART = 0x1003,			//表配置数据。 （部件）
	TYPE_TABLE_IN = 0x1004,				//用户表配置数据   （输入点）
	TYPE_TABLE_OUT = 0x1005,			//用户表配置数据   （输出点）
	TYPE_CFG1 = 0x2000,					//主机向智能卡或者面板发送参数配置文件
	TYPE_CFG2 = 0x2001,					//智能卡或者面板向主机发送参数配置文件
};

#ifdef __cplusplus
}
#endif

#endif

