#ifndef _ADMETER_H_
#define _ADMETER_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include	"admeter_dic.h"

#define ADMETER_FILTERING	8	//电子尺滤波的缓存数组
#define ADMETER_SPSD_NUM 3

#define ADDEF_SPD_BUF_NUM	30						//速度缓存区
#define ADDEF_ADD_BUF_NUM	ADDEF_SPD_BUF_NUM/2		// 10		加速度缓存区
#define ADDEF_POS_BUF_NUM	ADDEF_SPD_BUF_NUM+1		// 21		位置缓存区

#define AD_10VGND_OFF_NUM	800		// 认为10v，GND断开时的AD码变化量
#define ADDEF_SPD_THRESHOLD_MIN	4*MAX_LOOP_100US		//出错的阀值，每周期超20bit的变化

#define ADDEF_SETPOS_BUF_NUM	3
#define ADDEF_ALLALARM	1

typedef enum enADDEF_TYPE
{
	ADDEF_NOERR = 0,
	ADDEF_FWD_ACC = 1,          //电子尺变大方向 加速变大
	ADDEF_FWD_DEC = 2,  		//电子尺变大方向 减速变大
	ADDEF_BWD_ACC = 3,          //电子尺变小方向 加速变小
	ADDEF_BWD_DEC = 4,          //电子尺变小方向 减速变小
	ADDEF_OFFING = 5,			//异常终止

	ADDEF_10VOFF = 6,
	ADDEF_GNDOFF = 7,
}ADDEF_TYPE;

typedef enum enAD_CH_TYPE
{
	CH_METER,            //0-10输入AD
	CH_FINE_METER,  
	CH_PRESS_10V,            // 0-10V输入AD
	CH_PRESS_5V,            // 1-5V输入AD
	CH_OTHER
}AD_CH_TYPE;

typedef struct tySPEED_DEF
{
	UI8		adDef_ch;		//滤波通道号
	UI8		adDef_func;		//滤波功能:0未开启 1开启
	UI32	adDef_starttm;	//采集起始时间
	UI16	adDef_err_cn;	//连续错误次数
	UI16	threshold;		//阈值
	UI16	adDef_flag;		//异常终止状态  0  代表正常

	int		adDef_offpos;	//异常时的位置
	int		adDef_offspd;	//异常时的速度
	int		adDef_minspd_diff;
	UI16	adDef_off_add;	//异常导致的加速度变化方向	1:加速	0:减速
	
	int		adDef_pos_last[ADDEF_POS_BUF_NUM];//位置缓冲区
	UI16	adDef_pos_no;	

	int		adDef_rev_pos;	//修正后的位置
}SPEED_DEF, *PSPEED_DEF;

/*电子尺移动速度计算结构*/
typedef struct tySPEED_CAL
{
	UI16					ch;								/*电子尺通道号 */
	UI16					spd_accuracy;					/*采集速度精度设置	0:0.1mm/s	1:0.01mm/s*/
	UI16					cur_wind_ind;					/*当前计算窗口位置*/
	UI16					cur_speed_samp;					/*当前已采样的速度个数*/
	UI16					win_pos[SPEED_WIN_SIZE];		
	UI16					win_100us[SPEED_WIN_SIZE];
	UI16					last_chg_tm;					/*最近的电子尺变化时间间隔, 100us*/	
	UI32					speed_samp[SPEED_SAMPLE_NUM];	/*采样速度缓存*/
	UI32					speed;
}SPEED_CAL, * PSPEED_CAL;


typedef struct tyADMETER
{
	PART				part;

	PADMETER_MACHSET	mh;
	PADMETER_STATE		st;

	UI16					last_sample_100us;
	
	AD_CH_TYPE			ch_type[ADMETER_CH_NUM];
	UI16					ch_ad_value[ADMETER_CH_NUM];
	int					ch_meter[ADMETER_CH_NUM];
	int					ch_meter_c[ADMETER_CH_NUM];
	UI16					ch_relative[ADMETER_CH_NUM];

	AD_CH_TYPE			ch_type_ext[ADMETER_CH_NUM_EXT];
	UI16				ch_ad_value_ext[ADMETER_CH_NUM_EXT];   //扩展板得到的邋AD值                        
	int                 ch_meter_ext[ADMETER_CH_NUM_EXT];        //扩展板输出的相对位置            
	UI16				ch_relative_ext[ADMETER_CH_NUM_EXT];     //扩展板输出的相对位置绝对值

	UI16				ruler_clamp[ADMETER_FILTERING];
	UI16				Put_clamp;
		
	SPEED_DEF 			spd_def[ADMETER_CH_NUM];
	SPEED_DEF 			spd_ext_def[ADMETER_CH_NUM_EXT];

	SPEED_CAL			speed_cal[ADMETER_SPSD_NUM];
	UI32				admeter_speed[ADMETER_SPSD_NUM];
}ADMETER, *PADMETER;


#define 	ADMETER_INIT_INFO		{{MH_ID, CAL_LEN(ADMETER_MACHSET)}, {ST_ID, CAL_LEN(ADMETER_STATE)}}

BOOL 	InitAdMeterPart(void);
void 	SetAdChType(UI16 portno, AD_CH_TYPE type);
void 	AdMeterLoop(void);
void		AdMeterLoopdly(UI16 dly);
WORD 	GetAdValue(int portno);
int 		GetMeterLenByCh(UI8 ch);
int 		GetMeterPosByCh(UI8 ch);
int 		GetMeterMax(UI8 ch);
BOOL 	AdMeterCmd(UI32 cmd, UI32 para);
int 	GetMeterValue(UI16 ch);
void 	AdMeterLengthModify(UI16 ch, UI16 val, WORD type);
void 	AdMeterZeroAdj(WORD type);
UI32 	AdMeterSpeedLoop(PSPEED_CAL pcal);
BOOL 	AdMeterSpeedInit(UI8 ch, PSPEED_CAL pcal,UI8 accuracy);
void 	AdMeterCfgTx(void);
WORD GetPresSensorValueByPumpNo(UI16 ch);
AD_CH_TYPE GetAdChType(UI16 portno);

#ifdef __cplusplus
}
#endif
#endif

