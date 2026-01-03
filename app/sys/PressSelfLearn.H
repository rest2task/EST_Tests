#ifndef _PRESSSELFLEARN_H
#define _PRESSSELFLEARN_H

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//差分进化优化PID参数时用到的宏定义
#define 	SIZE		31		//差分进化的个体数(一代中)
#define		G			20		//进化的代数

#define		MinP		150		// 积分P的最小值
#define		MaxP		1000	// 积分P的最大值
#define		MinI		8		//积分I的最小值
#define		MaxI		50		//积分I的最大值
#define		Fmax		0.9		//变异缩放因子最大值
#define		Fmin		0.2		//变异缩放因子最小值
#define		cr			0.7		//差分进化

#define		Overshoot_weight	50		//超调权重
#define		SetPress_value		50		//自学习过程中的设定压力

#define		Adjust_slope		1.5		//用此斜率作为判断条件，进行消抖
#define		Optipara_max		60000	//当参数不合适时，剔除改组参数用到的人为设定指标值

#define		State_Time	800 	//超过设定值后持续的周期数
#define		Up_Time		600 	//判断上升的时间，只要时间到也跳走
#define		XiaoDou_Time 100 	//时间判断，用于在稳态部分消除抖动


typedef struct tag_PRESS_SELFL
{
  // 以下参数为自学习优化PID参数
	float m_p[SIZE];      		//每个个体的p值
	float m_i[SIZE];			//每个个体的I值
	UI64  	BsJ;				//运行到当前代所有个体的最优指标函数值
	float chaotiao[SIZE];		//一代中每个 个体的超调量
	UI64  c[SIZE];				//个体当前运行的指标函数值
	float Bests_p;				//运行的最优P值
	float Bests_i;				//运行的最优i值
	float B;					//计算指标函数时，用到的中间变量

	UI64 	T_BsJ1;				//记录上一代中当前个体的指标函数值，用于判断比较
	float p;					//每次实际运行的p值
	float ki;					//每次实际运行的I值
}PRESS_SELFL,*PPRESS_SELFL;

enum Chafen_step
{
    INIT_firstGePID = INIT1_STEP,   	//初始化起始代中的个体
	SetFistGeBSJ = STAGE_4_STEP,		//起始代中每个个体执行前赋值
	UpFirstBsj = STAGE_5_STEP,			//起始代中每一个个体的上升阶段
	StateFirstBsj = STAGE_6_STEP,		//起始代中每一个个体的稳态阶段
	FirstGeEnd = STAGE_7_STEP,			// 判断起始代中的所有个体是否执行完毕
	Opti_GePID = STAGE_8_STEP,			//主循环代从第1代开始每一个个体进行交叉编译更新
    Opti_GePID_UP = STAGE_9_STEP,		//主循环代从第1代开始每一个个体的上升阶段
    Opti_GePID_STATE = STAGE_10_STEP,	//主循环代从第1代开始每一个个体的稳态阶段
    Opti_GePID_End = STAGE_11_STEP,		//判断当前代中的个体是否全部执行完成
    NExt_G = STAGE_12_STEP,				//进入 下一代
    Opti_PID_End = STAGE_13_STEP
};

ACT_RET ChaFen_Opti (PACT pact,UI32 para);
void OneAction_Off(PHYDR pp, PACT pact);


	//ChaFen_Opti (PACT pact, PRESS_SELFL* p_Press, PRESS_STRU *p_PressStr, MC_PARA PMcPara, MC_CTRL PMcCtrl);



#ifdef __cplusplus
}
#endif

#endif

