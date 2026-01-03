#ifndef _IOPROC_H_
#define _IOPROC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "hwcfg.h"
#include "part.h"
#include "io_dic.h"

#define PI_SAMPL_TM		10				/*输入点的采样周期 1ms, 10*100us*/
//#define PO_SAMPL_TM		10				/*输入点的采样周期 1ms, 10*100us*/
#define PO_SAMPL_TM		MAX_LOOP_100US		

#define VLV_MIN_RSP_TM	 100			/*电磁阀的最小响应时间，10ms, 100*100us*/
#define DEF_FILT_NUM	3				/*缺省输入点3次滤波*/
#define PI_FORCE_BIT   0x40			/*强制输入点使用，在高7位  */

#define PI_NEG_BIT		0x80			/*反逻辑，在最高位*/
#define PI_FILT_MSK		0x0f
#define PC_ON      		0x0001
#define PC_DLYON   	0x0005
#define PC_OFF     	0x0004  
#define PC_OFFD    	0x0000

#define MAX_PO_DELAY 50000				/*阀操作延迟最大5秒, 100us单位, 不能超过(0xffff - PO_SAMPL_TM)*/

typedef enum enPI_STATUS
{
	PI_NOUSED = -1,
	PI_RESET = 0,
	PI_SET = 1
}PI_STATUS;

typedef enum enPO_STATUS
{
	PO_NOUSED = -1,
	PO_RESET = 0,
	PO_SET = 1
}PO_STATUS;


typedef struct tyPICNT_ST
{
 	UI16		name;
	PI_STATUS	last_st;
	UI32        cycle_cnt;
	UI16		onecyc;
	UI16		cnt_per_cycle;
	UI32        start_ms;
	UI32		cycle_start_ms;
	UI32		cycle_tm;			//周期时间
	UI32		last_cycle_tm;
	UI16        bstart;
}PICNT_ST, *PPINT_ST;

//#define PO_NEW
typedef struct tyIO
{
	PART	part;

	PIO_STATE		st;

	UI16			po_100us_tick;
	
	UI8				pi_filt_cnt[MAX_PI_NUM];
	UI8				pi_cfg[MAX_PI_NUM];			/*低4位滤波次数(0-15),  高4为: 配置,最高位置1表示反逻辑*/
	UI16     		pi_status[PI_STATUS_NUM];
	UI16			pi_max;
	UI16			po_max;
	UI16			pi_inn_num;			/*CPU板内置输入点数*/
	UI16			po_inn_num;			/*CPU板内置输出点数*/
	UI16			pi_ext_num;			/*扩展输入点数*/
	UI16			po_ext_num;			/*扩展输出点数*/

	UI8				po_vlvdelay[MAX_PO_NUM];
	UI16			po_set100us[MAX_PO_NUM];
	UI8				po_chain[MAX_PO_NUM];			/*记录关联到同一逻辑点的物理点单向链接，用于实现一个逻辑点多个物理点输出*/

	UI16			pi_inn_status_msk;			//不完整UI16位位数
	UI8				pi_inn_status_num;			//代表状态的UI16位完整个数	(板内置)	
	UI8				pi_ext_status_num;			//代表状态的UI16位完整个数	(扩展板)	
	
	UI16			po_inn_out_msk;				//不完整UI16位位数
	UI8				po_inn_out_num;			
	UI8				po_ext_out_num;
	UI32			last_ext_po_100us;

	UI16			adpos_inplance_in;	//进到位信号
	UI16			adpos_inplance_out;	//退到位信号

	UI16			force_flag;
	UI16			Ionum;
	
	UI16			IO_LossPres[4];		//LIO_MAX_NUM
	UI16			IO_Overcurrent[4];

	BOOL			pi_core[MAX_PI_NUM]; //中子进/退终输入点标记
}IO, *PIO;

//输入逻辑点定义表
typedef struct tyPOLINK_LOG
{
	UID 	part_id;			//逻辑点所属的部件ID
	UID		ind;				//逻辑点在部件输入子类的偏移
}POLINKLOG, *PPOLINK_LOG;

#define IO_INIT_INFO		{{ST_ID, CAL_LEN(IO_STATE)}}

UI16 GetPOBit(WORD name);
BOOL TestPI(WORD  name);			//YF
BOOL CheckPIUsed(WORD  name);
PI_STATUS CheckPI(WORD name);
void SetPOBit(WORD  name,WORD pcstat);
void SetPOOnOff(WORD  name,WORD pcstat);
void LoopPOOnOff(void);
BOOL LoopPI(void);
BOOL TestPOIsOn(WORD  name);
UI32 GetPI32(int ind);
UI32 SetPO32(int ind, UI32 val);
UI32 GetPO32(int ind);
BOOL InitIOPart(void);
void ClearAllPOOut(void);
void PICntLoop(PPINT_ST pcnt);
void ResetPICnt(PPINT_ST pcnt, UI8 pi_name, UI16 cnt_per_cycle);
UI32 GetPICycleMs(PPINT_ST pcnt);
UI16 GetPICnt(PPINT_ST pcnt);
void SetPICfg(WORD name, UI8 cfg);
void SetPICore(WORD name, BOOL iscore);
void POForceOutPut(UI16 len, UI16* poutput);
void POForceEnd(void);
void SetModelIOMax(void);
void IOSync(void);
void POChainClear(void);
void POChainAdd(WORD pre_name, WORD cur_name);
void IOVErr(void);
void IOSetHWRegAdr(BOOL bnew_adr);
void CheckIoForce(void);
void ChkAdposiInplace(void);		//检查动作是否到位
BOOL ChkAdFwdInplace(WORD AdNo);
BOOL ChkAdBwdInplace(WORD AdNo);
UI16 GetPI16(int ind);
UI16 GetPO16(int ind);
WORD GetMaxPo(void);
void SetPIForceCfg(UI16 state);
void ClearPIForceCfg(void);

PO_STATUS CheckPO(WORD name);
void ChkSetPOBit(WORD  name,WORD postat);




#define SetPOBitDelay(name, pcstat, delay) 	SetPOBit((name), (pcstat) + ((delay) << 1))
#define ChkSetPOBitDelay(name, pcstat, delay)  ChkSetPOBit((name), (pcstat) + ((delay) << 1))

#ifdef __cplusplus
}
#endif

#endif

