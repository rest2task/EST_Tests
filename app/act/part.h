#ifndef _PART_H_
#define _PART_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "printf.h"
#include "types.h"
#define MAX_PART_SN		15
#define MAX_ACT_SN		15

typedef enum enPART_RX_TYPE
{
	INIT_CFG_RX,					//
	MMI_SUB_RX,
	MMI_DATA_RX
}PART_RX_TYPE;

typedef enum enPART_SUB_ID
{
	//COM_ID,
	PO_ID = 0,			//输出点 0
	PI_ID,				//输入点 1
	MH_ID,				//机器参数 2
	MD_ID,				//模具参数 3
	PA_ID,				//动作参数 4
	ZD_ID = PA_ID,		//machine zero debug data 4
	VR_ID = PA_ID,		//版本信息 4
	HY_ID,				//液压伺服 5
	ST_ID,				//状态 6
	PART_SUB_END
}PART_SUB_ID;

/*Part.part_st_bits 状态位定义*/
#define PART_DISABLE	0x01

#define PART_SUB_NUM PART_SUB_END
typedef struct tyACT* PACT;
//各种类型部件结构体的共有部分，在结构的开始位置
typedef struct tyPART
{
	UID		id;							//部件ID
	UI16	act_run_flag;				//动作运行状态，某位置1，表示该位对应的动作在运行。
	UI16		act_num;					//该部件的动作数，指示pact_tbl的数据。
	UI16		part_st_bits;				/*初始化状态，部件状态位，目前只使用了最低位，PART_DISABLE*/
	UI16	sub_rx_bits;				/*标记子类数据是否已收到*/
	
	UI16	sub_len[PART_SUB_NUM];		//各子类数据的长度，以WORD位单位。
	UI16*	psub[PART_SUB_NUM];			//指向各子类数据
	PACT	pact_tbl;					//指向部件的动作结构
}PART, *PPART;

#define MAX_PART_NUM		70
//部件列表，每创建一个部件，就登记在该表中。
typedef struct tyPART_TBL
{
	UI16		num;					//部件数目
	PPART		part[MAX_PART_NUM];		//存放登记的部件指针。
}PART_TBL, *PPART_TBL;

//部件索引项，与部件列表结合，，用于部件ID转换到部件结构体
typedef struct tyPART_IND
{
	UI8				type;				//部件类别
	UI8				ind[MAX_PART_SN];	//该类别对应序号的部件在部件列表中的位置。
}PART_IND, *PPART_IND;

#pragma pack(1)
typedef struct tySUB_INIT_INFO
{
	UI8		sub_id;
	UI16	sub_len;
}SUB_INIT_INFO, *PSUB_INIT_INFO;

typedef enum enPART_CATEGORY
{
	PART_CAT_SYS,		// 0
	PART_CAT_ACT,		// 1
	PART_CAT_SUB,		// 2	
	PART_CAT_BCTRL,		// 3
	PART_CAT_CTRL,		// 4
	PART_CAT_SUB_SRAM,	// 5 数据存放外部内存
}PART_CATEGORY;

typedef enum enPART_CMD
{
	CMD_RESET,
	CMD_DATA_TX,
	CMD_DATA_RX,
	CMD_MANUAL_INIT,						/*手动模式初始化*/
	CMD_SETUP,								/*部件设置，初始化部件的关联部件指针*/
	CMD_METERCH,							/*获取部件电子尺通道号*/
	CMD_CLR_SUB_DATA	,					/*清除部件及子部件指定子类的数据*/
	CMD_AUTO_ALARM_SET,						/*自动警报设置，模座使用*/
	CMD_QC_PARA,                                /*QC监控处理*/
	PART_CMD_END = 0xff
}PART_CMD;

//部件类型信息
typedef struct tyPART_TYPE_INFO
{
	UI8	type;								//类别
	UI8	cat;								//分类
	UI16 part_size;							//部件结构体大小
	void* init_func;						//部件初始化函数
	void* cmd_func;							//部件命令结构函数
	UI16 stor_sub_bits;						//存储在下位机的子类信息，不需要从上位机接收的数据
	UI16 state_sub_bits;					//状态子类信息，不需要从上位机接收的数据
}PART_TYPE_INFO, *PPART_TYPE_INFO;

typedef struct tyPART_ALARM_GRADE
{
   UI8 type;                        //部件id
   UI16 alarm_grade_offset ;      //警报等级偏移量
   UI16 alarm_grade[4];            //记录警报等级
}PART_ALARM_GRADE,*PPART_ALARM_GRADE;


//部件数据修改缓存
#define CHANGE_CACHE_HD_NUM	20
#define CHANGE_CACHE_LEN	100

typedef struct tyPART_DATA_CHANGE_CACHE_HD
{
	UI16*	psub;	//数据的目标首指针	
	UI16	offset;	//数据的目标偏移位置
	UI16*	pdat;	//数据当前存储地址
	PPART	pp;		//该数据的部件指针
	UI16	len;	//数据长度
	UI32	cmd;	//cmd
}PART_DATA_CHANGE_CACHE_HD, *PPART_DATA_CHANGE_CACHE_HD;


typedef struct tyPART_DATA_CHANGE_CACHE
{
	UI16	data_num;	//缓冲数据包 包数
	PART_DATA_CHANGE_CACHE_HD	change_cache_hd[CHANGE_CACHE_HD_NUM];	//各个数据包包头的指针
	UI16	change_cache[CHANGE_CACHE_LEN];	//实际数据存储区
}PART_DATA_CHANGE_CACHE, *PPART_DATA_CHANGE_CACHE;

#pragma pack()

//#define	PART_SUB_NUM		((OFFSETOF(PART, act_num) - OFFSETOF(PART, po_num))/sizeof(UI16))

typedef BOOL(*SYS_CMD_FUNC)(UI32 cmd, UI32 para);
typedef BOOL(*PART_CMD_FUNC)(PPART pp, UI32 cmd, UI32 para);
typedef BOOL(*SYS_INIT_FUNC)(void);
typedef BOOL(*PART_INIT_FUNC)(PPART pp, UI8 sn);

void *Memcpy1(UI16 *dest, UI16 *src,size_t count) ;
void SetPartErr(PPART pp, UI32 err);


#define TSK_ALL_MSK			(0xffff)
#define ERR_ALL_MSK			((UI32)(-1))

//设定警报时，需要注意在AlarmClass.c中设定警报等级。如果未设定，默认3等级(提示、警报(自动))，上传警报。
#define	SET_PART_ERR(p, err)	SetPartErr((PPART)p, (UI32)err)
#define	CLR_PART_ERR(p, err)	ClrPartErr((PPART)(p), err)
#define	CHK_PART_ERR(p, err)	ChkPartErr((PPART)(p), err)

#define GET_PART_TYPE(id)		((((UID)(id)) >> 8) &0xff)
#define GET_PART_SN(id)			((((UID)(id)) >> 4) &0x0f)
#define GET_ACT_SN(id)			(((UID)(id)) & 0x0f)
#define GET_PART_ACT_SN(id)		(((UID)(id)) &0xff)
#define SUB_BIT(sub_id)			((UI16)(0x01) << (sub_id))

#define SET_PART_ST(pp, bit)	((pp) != NULL ? ((PPART)(pp))->part_st_bits |= (bit) : 0)
#define CLR_PART_ST(pp, bit)	((pp) != NULL ? ((PPART)(pp))->part_st_bits &= (~(bit)) : 0)
#define CHK_PART_ST(pp, bit)	((pp) != NULL ? ((PPART)(pp))->part_st_bits & (bit) : 0)

/*make action ID*/
#define MK_ACT(part, part_sn, act_sn)	(((UI16)((part) << 8) & 0xff00) + (((part_sn)  << 4) & 0xf0) + ((act_sn) & 0x0f))
/*make part ID*/
#define MK_PART(part, part_sn)			(((UI16)((part) << 8) & 0xff00) + (((part_sn)  << 4) & 0xf0))

#define INIT_PART(pp, id, sn, sub)	(InitPart((PPART)(pp), (id), (sn), (sub), sizeof(sub)/sizeof(SUB_INIT_INFO))) 

#define MAKE_CMD(para, cmd)			((((UI32)(para)) << 16) | ((cmd) & 0xffff))
#define GET_CMD(cmd)				((cmd) & 0xffff)
#define GET_CMD_PARA(cmd)			(((cmd) >> 16) & 0xffff)
#define GET_CMD_SUB(cmd)			(((cmd) >> 16) & 0x00ff)
#define GET_CMD_RXTYPE(cmd)			(((cmd) >> 24) & 0x00ff)

#define INT_ABS(x)					((x) >= 0 ? (x) : -(x))
#define ABS_DIFF(x1, x2)			(AbsDiff(x1, x2))

#define CHK_ACT_IS_IDLE(pp, act_sn) (ChkActIsIDLE((pp)->act, (act_sn)))

/*接口函数定义*/
BOOL InitPart(PPART pp,  UI8 type, UI8 sn, const  SUB_INIT_INFO* psub_init, int sub_num);
BOOL PartErrSet(void* pp, UI32 err_msk, BOOL bset);
UI16 ChkActRun(void* pp, UI16 mask);
UI16 ChkPartActRun(void* pp);
UI16 GetActRunFlag(void* pp);
void SetActRunFlag(void*  pp, UI8 sn, BOOL brun);
PACT GetActByID(UI16 act_id);
PPART GetPartByID(UI16 part_id);
BOOL AddPartToTbl(PPART pp);
void PartDataRx(PPART pp, UI8 sub_id, UI16 len, UI16 offset, UI16* pdat, UI8 rx_type);
BOOL PartDataTx(PPART pp, UI8 sub_id, UI16 len, UI16 offset, BOOL brsp);
BOOL PartSubTx(PPART pp, UI16 sub_id_bits, BOOL brsp);
BOOL PartDataReqTx(PPART pp, UI16 sub_id, UI16 len, UI16 offset);
BOOL PartSubReqTx(PPART pp, UI16 sub_id_bits, BOOL brsp);
void ClearAllPartSubRxBit(void);
void ClearAllPartError(void);
PACT GetPartActBySN(PPART pp, UI8 sn);
PPART GetNextPartByInd(int* ind);
UI16 GetPartSubNeedRxBit(PPART pp);
UI16 GetPartErrLowByte(PPART pp);
const PART_TYPE_INFO* GetPartTypeInfo(UI8 part_type);
BOOL GetPartSubData(PPART pp, UI8 sub_id, UI16 ind, UI16* pval);
BOOL SetPartSubData(PPART pp, UI8 sub_id, UI16 ind, UI16 val);
void ClearAllPartSub(UI8 sub_id);
UI32 CallPartCmd(PPART pp, UI32 cmd, UI32 para);
void InitPartTypeInfo(void);
void ClrPartErr(PPART pp, UI32 err);
UI32 ChkPartErr(PPART pp, UI32 err);
UI16 GetSubDataByID(UI16 part_id, UI8 sub_id, UI16 ind);
int AbsDiff(int x1, int x2);
int AbsDiff_2(int x1, int x2);

int AbsDiffMin(int x1, int x2);
void PartNUllSubRx(PPART pp,  UI8 rx_type);

void InitPartDataChangeCache(void);
void MovePartDataChangeCache(void);
void MovePartDataChangeCacheWithActStart(UID actid);
UI16 ChkActIsIDLE(PACT pact, UI16 act_sn);


#ifdef __cplusplus
}
#endif

#endif

