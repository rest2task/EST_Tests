/******************************************************************************
  文 件 名   : keycmd.h
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月22日
  最近修改   :
  功能描述   : 键盘命令处理
  函数列表   :
  修改历史   :
  1.日    期   : 2013年5月22日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/
#ifndef _KEYCMD_H_
#define _KEYCMD_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "protcmd.h"
#include "canhead.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     _NULL_KEY           0x00F0

/*---------------------------------------------------------------------------+
|           Application function key                                         |
+---------------------------------------------------------------------------*/

#define KEY_MANUAL      0x0034  //for ARM system 05-5-12
#define KEY_ADJUST		0x0001
#define KEY_SEMIAUTO	0x0002
#define KEY_TIMEAUTO	0x0003
#define KEY_SENSORAUTO	0x0004
#define KEY_RUN			0x0005
#define KEY_ALARM_CLEAR	0x0006	//警报清除按钮
#define KEY_WARM		0x0020
#define KEY_WARMOFF		0x0021
#define KEY_MWON		0x0024
#define KEY_MWOFF		0x0025
#define KEY_SVALM_CLEAR 0x0026  //电储料驱动警报清除 20230529dylong




#define KEY_BY1		    0xBFD8 //0x4048      //备用键一

/* Machine 02 sect manual key define                                        */

//#define     KEY_ADJFWD         100  //A310
//#define     KEY_ADJBWD         101  //A310


/* Machine 01 sect manual key define                                        */

/* Machine other sect manual key define                                        */

//#define     KEY_AIRBLOWA      39
//#define     KEY_AIRBLOWB      40
//#define     KEY_SAFEDCLS      0x43
//#define     KEY_SAFEDOPN      0x42

 
#define     KEYON             0x8000
#define		RSP_KEYON         0x0100
#define     PNLWAITTIME       650		//400	//改成650ms
#define		ACT_KEY_START	  0x0101

typedef struct tyKEY_ST
 {
	UI32	last_keypress_ms;
	WORD	last_rx_ms;
	WORD    lastkey;             
    WORD	currentkey;             
    WORD	repeat;			//0: key off, 1:第一个key on, > 1相同的连续按键
    BOOL	bnew;
	BOOL	keyon;
 } KEY_ST, *PKEY_ST;

//typedef BOOL(*KEY_CMD_FUNC)(UI16 key, UI16 repeat);

typedef enum enKEY_CMD_TYPE
{
	KEYON_OFF,						/*键按下时启动，键放开时结束*/
	KEYON_SWICTH,					/*第一次按键动作启动，再次按键动作结束*/
	KEYON_ONE						/*按键启动，动作一直运行直到其自行结束*/
}KEY_CMD_TYPE;

/*函数返回TRUE，表示按键命令已经处理;
如FALSE，表示按键命令没有处理，函数返回后继续处理*/
typedef BOOL (*KEY_CMD_FUNC)(UI16 keycode, BOOL bkeyon, UI16 repeat_cnt, UI32 start_press_ms);

/*定义需要特殊处理的按键，缺省按键处理时KEY_ON_OFF类型*/

//KEY_CMD_FUNC	cmd_func;		/*对按键需要特殊处理时，设置命令处理函数，如不需要特殊处理，置NULL*/
typedef struct tyACT_KEY_MAP
{
	UI8		part_type;
	UI8		act_sn;
	UI8		adj_act_sn;
	UI8		type;							/*KEY_CMD_TYPE*/
	KEY_CMD_FUNC cmd_func;					/*如果函数指针为空或函数返回值为FALSE，按type指定的缺省按键方式处理*/
}ACT_KEY_MAP, *PACT_KEY_MAP;

typedef struct tag_STATKEY
  {
    BOOL       runkey; 
	BOOL		IOoutclr[LIO_MAX_NUM];	//IO清除错误标记指令
	WORD        IOflag[LIO_MAX_NUM];		//IO错误标记记录，不用给扩展板一直发送0
  } STATKEYSTRU;

typedef struct tyKEY_EM
{
    BOOL       last_resetkey;             
    BOOL       last_manualkey;

	BOOL	 tmp_manualkey;
	BOOL	 tmp_Resetkey;
	UI16	reset_key_chg_tm;
	UI16	manual_key_chg_tm;
}KEY_EM, *PKEY_EM;
extern      STATKEYSTRU       g_statkey;
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
void KeyScanProc(void);
void InitKey(void);
void GetEmergKey(void);
void ComKeyRecv(PPROT_KEYPAD_CMD_PKT ppkt);
void ClearRunKey(void);
BOOL GetRunKey(void);

#ifdef __cplusplus
}
#endif

#endif


