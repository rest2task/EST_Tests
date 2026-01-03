#ifndef _TEMPCTRLMW_H_
#define _TEMPCTRLMW_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "TempCtrlMw_dic.h"
#include "Canhead.h"

#define  MWNUM		0x04		//每块模温扩展板面板发送下来的设定参数的缓存区，所允许存放的最大数目
#define  MW_HEART	1000		//模温扩展板的心跳总时间
#define  MW_SENDTM	3			//模温板设定参数，刚开机时给扩展板发送的各个参数间的时间间隔
#define  MW_HEATONOFF	12		//模温电热开关状态发送的时间间隔
#define  MW_MANULE	3			//模温手动按键命令发送的时间间隔
#define  MW_MAYHTM	8			//模温优化命令发送的时间间隔


//主机向501发送的命令字
#define TEMP_DATA	0x01  	//主机向501温度扩展板发送温度设定值
#define TEMP_FUN	0x02	//主机向501温度扩展板发送温度功能选择项
#define TEMP_SET_P	0x03	//主机向501发送温度P值
#define TEMP_SET_I	0x04	//主机向501发送温度I值
#define TEMP_SET_D	0x05	//主机向501发送温度D值
#define TEMP_RAMP	0x06	//主机向501发送温度缓冲区值
#define TEMP_TIME	0x07	//主机向501发送温度每段的输出周期时间
#define TEMP_DIVH	0x08	//主机向501发送温度每段的上偏差值
#define TEMP_DIVL	0x09	//主机向501发送温度每段的下偏差值
#define TEMP_JKMD	0x0A	//主机向501发送热电偶类型，J型或者K型
#define TEMP_BL		0x0B	//主机向501发送手动加热百分比数
#define TEMP_KEEP	0x0C	//主机向501发送保温功能是否启用
#define TEMP_WARM	0x0D	//主机向501发送保温温度
#define TEMP_CYCL	0x0E	//主机向501发送最小扫描周期时间
#define TEMP_DETE	0x0F	//主机向501发送温度上升连续监测功能是否使用
#define TEMP_NUM	0x10	//主机向501发送温度段数
#define TEMP_YWT	0x11	//主机向501发送预温计时
#define TEMP_USE	0x12	//主机向501发送温度扩展板功能是否启动
#define TEMP_YH		0x13	//主机向501发送温度优化命令
#define TEMP_YHQZ	0x14	//主机向501发送温度强制优化命令
#define TEMP_TOBU	0x15	//主机向501发送主机的同步升温功能


#define TEMP_SWITCH	0x25	//主机向501发送模温电热开关
#define TEMP_MAUL	0x26	//主机向501发送主机的手动键命令
#define TEMP_TEST	0x27	//主机向501发送主机的can通讯测试命令



//501向主机发送的命令字
#define TEMPMOLD_REAL     0x41    // E501发送实时温度   data0: 第几段  （数字0~7，对应1段~8段）    data1：当前段的温度值 
#define TEMPMOLD_STATUS   0x42    // E501发送加热状态   1个byte，8个bit，每位bit表示一段加热状态。0：不加热，1加热  
#define TEMPMOLD_MW_BL	  0x43	  //E501发送加热输出百分比
#define TEMPMOLD_MW_SW	  0x44	  //E501发送模温扩展板的室温温度
#define TEMPMOLD_MW_DR	  0x45	  //E501发送模温扩展板的电热状态
#define TEMPMOLD_VER      0x46    // E501发送版本号
#define TEMPMOLD_ERR      0x47    // E501发送 错误代码  0:无错误 1:错误类型1     2:错误类型2
#define TEMPMOLD_WARM     0x48    // E501发送加温状态给主机，当由于警报关闭电热时，告诉主机去清除主机上的加热按键状态
#define TEMPMOLD_READ     0x49    // E501发送 can测试次数给主机，主要用于通讯测试
#define TEMPMOLD_YHFH     0x4A    // E501发送 模温扩展板温度优化开始标记
#define TEMPMOLD_YHOK     0x4B    // E501发送 模温扩展板优化完成标记

#define TEMPMOLD_MW_P	  0x60	  //E501发送计算后的P值给主机   data0: 第几段  （数字0~7，对应1段~8段）    data1：当前段的P值 
#define TEMPMOLD_MW_I	  0x61	  //E501发送计算后的I值给主机   data0: 第几段  （数字0~7，对应1段~8段）    data1：当前段的I值 
#define TEMPMOLD_MW_D	  0x62	  //E501发送计算后的D值给主机   data0: 第几段  （数字0~7，对应1段~8段）    data1：当前段的D值 
#define TEMPMOLD_MW_RAMP  0x63	  //E501发送计算后的缓冲区值给主机   data0: 第几段  （数字0~7，对应1段~8段）    data1：当前段的缓冲区值



typedef struct tag_TEMPMW_BUF
{	
    UI16     ext_board_last;        //存放当前为第几块模温扩展板
	UI16     ext_command;           //存放当前这一块模温扩展板接收到的当前参数的命令字
	UI16     ext_num_set;           //存放当前这一块模温扩展板接收的参数(如第几段的温度)
	UI16     ext_data_set;          //存放当前这一块模温扩展板接收的参数(如第几段的温度的温度值)
	UI16	 ext_begin_tm;			//参数下发给主机后计时开始时间
	UI16	 ext_begin_flag;		//参数下发给主机后标记接收到后赋值为TRUE，当接收到模温扩展板发送的反回值后赋值为FALSE
	UI16	 ext_begin_cnt;			//参数开始重发机制时的重发次数
}TEMPMW_BUF;

typedef struct tyTEMPCTRLMW
{
	PART				part;
	PTEMPCTRLMW_PO		po;
	PTEMPCTRLMW_PI		pi;
	PTEMPMW_MACHSET		mh;
	PTEMPMW_MOLDSET		md;
	PTEMPCTRLMW_STATE		st;
	TEMPMW_BUF			mw_buf[MWNUM];		//模温设定参数数据缓冲区
	UI16	mw_num;			//存在当前这一块模温的临时参数的编号
	
	UI16	mw_id;			//模温模块编号 从1开始
	UI16	mw_put_step;	//模温数据装载动作步
	UI16	mw_put_time;	//模温装载时，时间的初始化
	UI16	mw_segments;	//模温使用的温度段数，小于8则用8 ，保证刚开机时，所有8段的数据都先发送给模温扩展板
	UI16	mw_sendnum;		//模温板发送设定参数，当前的编号
	
	UI16	can_e501_testmode;	//e501进入can通讯测试模式
	UI16	can_e501_writestep; //e501写步骤
	UI16	can_e501_writetm;	//e501写时间标记

	UI16	mw_heat_statue;		//模温加热开启和关闭标记
	UI16	mw_heat_starttm;	//模温加热开启和关闭的起始时间

	UI16	mw_close_flag;		//模温选择不使用后的2s内，该变量为1，其他时候都为0 
	UI16	mw_close_tm;		//从模温选择不使用开始计时，持续2s时间，主要利用这段时间，将该块模温板的状态数据清零并发送给面板，再停止上传该块模温板的状态数据

	UI16	mw_yh_begin;		//模温优化开启，0 不开启，1 普通优化，2  强制优化
	UI16	mw_yh_tm;			//模温强制优化开启时间

	UI16	mw_maul_statue;		//模温手动按键命令
	UI16	mw_maul_starttm;	//模温手动按键命令起始时间
}TEMPCTRLMW, *PTEMPCTRLMW;

#define TEMPCTRLMW_INIT_INFO	{{PO_ID, CAL_LEN(TEMPCTRLMW_PO)},{PI_ID, CAL_LEN(TEMPCTRLMW_PI)}, {MH_ID, CAL_LEN(TEMPMW_MACHSET)}, \
							 {MD_ID, CAL_LEN(TEMPMW_MOLDSET)}, {ST_ID, CAL_LEN(TEMPCTRLMW_STATE)}}


extern PTEMPCTRLMW	g_temp_mw[TEMPMW_MAX_NUM];
extern UI16	g_tempmw_num;
extern UI16	mw_sendtommi_step;	//模温时时状态值发送给面板的动作步
extern UI16	mw_sendtommi_tm;	//模温时时状态值发送给面板的时间初始值
extern UI16	mw_sendtommi_num;   //模温时时状态值发送给面板的当前模温板号
extern UI16 sendnum;			//模温板当前心跳发送的版本编码
extern UI16 totalnum;			//模温板实际连接成功的总数目
extern UI16 mw_piece;			//每一块模温板在线状态，按位操作，第一位为1代表第一块模温板在线，以此类推
extern UI16 mw_hearttm;			//每一块模温板的心跳发送间隔时间
extern UI16 mw_scantm;			//模温的心跳扫描时间初始化

BOOL TempMwIsHoldMoldErrChk(void);
void  Temp_mold_buf(UI16 num,PTEMPCTRLMW pp);
void  TempMoldSetData(PTEMPCTRLMW pp,UI16 i);
BOOL  InitTempMwPart(PTEMPCTRLMW pp, UI8 sn);
UI32  TempMwCmd(PTEMPCTRLMW pp, UI32 cmd, UI32 para);
void  TempdataReset(PTEMPCTRLMW pp,UI16 n);
void  Mw_oncom(PTEMPCTRLMW pp,UI16 i);
void  Mw_offcom(PTEMPCTRLMW pp,UI16 i);
void  Mwon(void);
void  Mwoff(void);
BOOL  GetMwStatus(void);
void  TempMwcheck(void);
void  Can_Test(void);
void  Can_e501_test(PTEMPCTRLMW pp);
void  Temp_mwRFStartPro(WORD password,WORD id,WORD ch);
BOOL Temp_ErrInj_all(void);
BOOL Temp_ErrInj_one(PTEMPCTRLMW pp,UI16 i);


#ifdef __cplusplus
}
#endif

#endif

