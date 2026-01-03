#ifndef _TEMPCTRL_H_
#define _TEMPCTRL_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "hwcfg.h"
#include "part.h"
#include "tempctrl_dic.h"

#define     MAXPULSENEG           100            // 二次测温最大误差 LSB  (原为脉冲)
#define     MAXTEMPCOUNT          400	

typedef struct tag_TEMPAMBIENT	 				//室温结构体
{
	BOOL	f_getbase;							//得到室温基准值（首次读到的值）
	BOOL	f_transform;						//室温转微伏值查表请求标志
	WORD	curambient;							//当前室温
	WORD	lastambient;						//上次室温
	WORD	starttime;							//启动读室温启始时间
	WORD	readtime;							//读间隔时间
	WORD	cnvalid;						   	//读到有效值次数
	WORD	cninvalid;							//读到无效值次数
	WORD	readstep;							//读步骤
	WORD	readLM73Dlytm;
	WORD 	agetime;  							//老练时间计算用,暂放此结构体中
	WORD	ReadTime_E505_Ver;					//室温扩展板版本读取间隔时间
	WORD	ReadTime_E502_Ver;					//室温扩展板版本读取间隔时间
	WORD	failcount;							//错误计数
	WORD	base_readstep;						//基准值读步骤
}TEMPAMBIENT_STRU; 

typedef struct tyTEMPCTRL
{
	PART				part;
	
	PTEMP_ZDSET			zd;
	PTEMPCTRL_STATE		st;

	UI16			fl_first;		//室温读取标记，用于刚开机时，延迟一段时间后再读室温使用
}TEMPCTRL, *PTEMPCTRL;

  
typedef struct tag_TEMPSTU
{
    WORD            count;                      //温度采样次数
    WORD            cnvalid;                    //温度采样有效次数
    WORD            cninvalid;                  //温度采样无效次数        
    DWORD           current_sum;                //当前有效温度采集之和
    DWORD           invalid_sum;                //当前无效温度采集之和 
    WORD			fail_count;					//错误采样次数
} TEMP_STRU;


#define TEMPCTRL_INIT_INFO	{{ZD_ID, CAL_LEN(TEMP_ZDSET)}, {ST_ID, CAL_LEN(TEMPCTRL_STATE)}}

extern TEMPAMBIENT_STRU  g_ambient;
extern TEMPCTRL		m_temp;


/*接口函数*/

BOOL InitTempCtrlPart(void);
void Read_surtemp(void);
UI32 TempCtrlCmd(UI32 cmd, UI32 para);
void AMBTempAdjMain(void);
void Temp_cnt_init(void);



#ifdef __cplusplus
}
#endif

#endif

