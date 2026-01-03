#include "acttype.h"
#include "nozzle.h"
#include "pump.h"
#include "machine.h"
#include "servo.h"
#include "pump.h"
#include "part.h"
#include "typelst_dic.h"
#include "act.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "PressSelfLearn.h"
#include "keycmd.h"


int			SIZE_COUNT;       //记录当前代中正在执行第几个个体
int			SIZE_G;			  //记录当前执行到第几代优化
int			cycle_num;		  //记录循环的周期时间，通过中途赋0，分别表示上升阶段和稳定阶段
float       RandomNum[SIZE];  //存储随机数组
float		max_Overshoot;    //记录每个周期的超调值


PRESS_SELFL  selfl;

PPRESS_SELFL p_Press = &selfl;
PPRESS_STRU  p_PressStr = (PPRESS_STRU)(&g_press[0]);

UI16		FL_PWRMATCHTBL_SAVE;


/***************************************************************************************
  * @函数名称	SelfLearningPIDpara_G_p(int i)
  * @函数说明   用于当前代中每一个个体P 值的交叉变异更新
  * @输入参数   i-当前代中的个体序号
  * @返回参数   无
  * @作者       Mayaping
  * @日期       2019.09.25
 ****************************************************************************************/
void SelfLearningPIDpara_G_p(int i)
{	
	int r1, r2, r3, r4; //变异中用到的变量
	float h_p[SIZE];
	float v_p[SIZE], tempr, F;
	
	srand((unsigned int) GetSecTick());  //随机种子用来产生不同的随机数

	F = (Fmax - Fmin)*(G-SIZE_G)/(double)G + Fmin;

    r1 = r2 = r3 = r4 = 1;
	while(r1==r2 || r1==r3||r2==r3||r1==i||r2==i||r3==i||r4==i||r1==r4||r2==r4||r3==r4)
	{
		r1 = ceil(SIZE * (rand()%100/(double)101));
		r2 = ceil(SIZE * (rand()%100/(double)101));
		r3 = ceil(SIZE * (rand()%100/(double)101));
		r4 = ceil(SIZE * (rand()%100/(double)101));
	}

	h_p[i] = p_Press->m_p[r1]+ F*(p_Press->m_p[r2] - p_Press->m_p[r3]);
	

	//检查是否越界
	/*if (h_p[i] < MinP)
	{
		h_p[i] = MinP;
	}
	else if (h_p[i] > MaxP)
	{
		h_p[i] = MaxP;
	}*/
	while ((h_p[i] < MinP) || (h_p[i] > MaxP) )
	{

		r1 = r2 = r3 = r4 = 1;
		while(r1==r2 || r1==r3||r2==r3||r1==i||r2==i||r3==i||r4==i||r1==r4||r2==r4||r3==r4)
		{
			r1 = ceil(SIZE * (rand()%100/(double)101));
			r2 = ceil(SIZE * (rand()%100/(double)101));
			r3 = ceil(SIZE * (rand()%100/(double)101));
			r4 = ceil(SIZE * (rand()%100/(double)101));
		}

		h_p[i] = p_Press->m_p[r1]+ F*(p_Press->m_p[r2] - p_Press->m_p[r3]);
	}

	//交叉

	tempr = rand()%100/(double)101;
	if(tempr < (float)cr)
	{
		v_p[i] = h_p[i];
	}
	else
	{
		v_p[i] = p_Press->m_p[i];
	}

	//存储个体未变化时的值
    p_Press->T_BsJ1 = p_Press->c[i];
	p_Press->c[i] = 0;

    p_Press->p = v_p[i];

}


/***************************************************************************************
  * @函数名称	SelfLearningPIDpara_G_i(int i)
  * @函数说明   用于当前代中每一个个体i 值的交叉变异更新
  * @输入参数   i-当前代中的个体序号
  * @返回参数   无
  * @作者       Mayaping
  * @日期       2019.09.25
 ****************************************************************************************/

void SelfLearningPIDpara_G_i(int i)
{	
	int r1, r2, r3, r4; //变异中用到的变量
    //float F;
	float h_i[SIZE];
	float v_i[SIZE], tempr, F;

	F = (Fmax - Fmin)*(G-SIZE_G)/(double)G + Fmin;
	
	srand((unsigned int) 5*GetSecTick());  //随机种子用来产生不同的随机数

    r1 = r2 = r3 = r4 = 1;
	while(r1==r2 || r1==r3||r2==r3||r1==i||r2==i||r3==i||r4==i||r1==r4||r2==r4||r3==r4)
	{
		r1 = ceil(SIZE * (rand()%100/(double)101));
		r2 = ceil(SIZE * (rand()%100/(double)101));
		r3 = ceil(SIZE * (rand()%100/(double)101));
		r4 = ceil(SIZE * (rand()%100/(double)101));
	}

	h_i[i] = p_Press->m_i[r1]+ F*(p_Press->m_i[r2] - p_Press->m_i[r3]);

	//检查是否越界	
	/*if (h_i[i] < MinI)
	{
		h_i[i] = MinI;
	}
	else if (h_i[i] > MaxI)
	{
		h_i[i] = MaxI;
	}*/

	while((h_i[i] < MinI) || (h_i[i] > MaxI))
	{
		r1 = r2 = r3 = r4 = 1;
		while(r1==r2 || r1==r3||r2==r3||r1==i||r2==i||r3==i||r4==i||r1==r4||r2==r4||r3==r4)
		{
			r1 = ceil(SIZE * (rand()%100/(double)101));
			r2 = ceil(SIZE * (rand()%100/(double)101));
			r3 = ceil(SIZE * (rand()%100/(double)101));
			r4 = ceil(SIZE * (rand()%100/(double)101));
		}
		
		h_i[i] = p_Press->m_i[r1]+ F*(p_Press->m_i[r2] - p_Press->m_i[r3]);
		
	}
	
	//交叉

	tempr = rand()%100/(double)101;
	if(tempr < (float)cr)
	{
		v_i[i] = h_i[i];
	}
	else
	{
		v_i[i] = p_Press->m_i[i];
	}

	p_Press->ki = v_i[i];

}



/***************************************************************************************
  * @函数名称	InitialFirstGePID_p ()
  * @函数说明   利用随机函数给初始代中的每一个个体赋P值
  * @输入参数   无
  * @返回参数   无
  * @作者       Mayaping
  * @日期       2019.09.25
 ****************************************************************************************/
void InitialFirstGePID_p ()
{
	int i,j;
	srand((unsigned int) GetSecTick()); //随机种子用来产生不同的随机数
	
    for(i=1; i<SIZE; i++)
    {
       RandomNum[i] = rand()%100/(double)101;
    }
	for(j=1; j<SIZE; j++)
	{
	    p_Press->m_p[j] = MinP + (MaxP - MinP)*RandomNum[j];
	}		
}


/***************************************************************************************
  * @函数名称	InitialFirstGePID_i ()
  * @函数说明   利用随机函数给初始代中的每一个个体赋i值
  * @输入参数   无
  * @返回参数   无
  * @作者       Mayaping
  * @日期       2019.09.25
 ****************************************************************************************/
void InitialFirstGePID_i ()
{
	int i,j;
	srand((unsigned int) 3*GetSecTick()); //随机种子用来产生不同的随机数
	
    for(i=1; i<SIZE; i++)
    {
       RandomNum[i] = rand()%100/(double)101;
    }
	for(j=1; j<SIZE; j++)
	{
		p_Press->m_i[j] = MinI + (MaxI - MinI)*RandomNum[j];
	}		
}


//打开起压
void SetPid_EveryG(PHYDR pp, PACT pact)
{
	ActSetHydr(pact, MC_CTR_MODE_SPEED|MC_CTR_MODE_PRES, pp->mh->PRESSELF_PRES, pp->mh->PRESSELF_FLOW, 0, 0, 0, 0,
	0,HYDR_PORT(pp->mh->PRESSELF_PORT),pp->mh->PRESSELF_PUMP,pp->mh->PRESSELF_PUMP,
    p_Press->p,p_Press->ki, 0,1,0); 
}

//关闭压力
void OneAction_End(PHYDR pp, PACT pact)
{	
	HYDR_CLS(0, 0);
}


//关闭压力
void OneAction_Off(PHYDR pp, PACT pact)
{	
	HYDR_CLS(0, 0);
}


/***************************************************************************************
  * @函数名称	OptiPara(int j)
  * @函数说明   用超调量和误差值计算每一个起压过程中的指标函数值
  * @输入参数   j-当前代中的运行的当前个体的编号
  * @返回参数   无
  * @作者       Mayaping
  * @日期       2019.09.25
 ****************************************************************************************/
void OptiPara(int j)
{
	if(p_PressStr->presspwr.ek < 0)
		p_Press->B = 1*abs(p_PressStr->presspwr.ek);	//1,2为权重值
	else
		p_Press->B = 2*abs(p_PressStr->presspwr.ek);
	
	if (cycle_num == (State_Time - 1))    //稳态运行到最后一个周期时加上超调的影响
	{
		p_Press->c[j] = p_Press->c[j] + p_Press->B + Overshoot_weight*p_Press->chaotiao[j];
	}
	else
	{
		p_Press->c[j] = p_Press->c[j] + p_Press->B;
	}
}


/***************************************************************************************
  * @函数名称	ChaFen_Opti (PACT pact,UI32 para)
  * @函数说明   差分进化的执行步骤,分为初始代和主循环代两大部分
  * @输入参数   动作参数
  * @返回参数   无
  * @作者       Mayaping
  * @日期       2019.09.25
 ****************************************************************************************/
ACT_RET ChaFen_Opti (PACT pact,UI32 para)
{
  PHYDR pp = (PHYDR)(pact->part);
	
	ACT_RET ret = ACT_OK;

	if(pp == NULL || p_Press == NULL || p_PressStr == NULL)
	{
		return ACT_ERR_ABORT;
	}

    switch (pact->step)
    {	     
     	case INIT_firstGePID:  // 初始化起始代中的个体
     		if(!ChkOperMode(OM_MANUAL))
     		{
				return ACT_ERR_ABORT;
			}

		 	SIZE_COUNT = 1;    
			SIZE_G = 0;
			cycle_num = 0;
			InitialFirstGePID_p();
			InitialFirstGePID_i();

			SetOperMode(OM_PR_RF);
			
			ClearRunKey();
			SET_PART_ERR(GetSys(),ER0_SKEY);
			ACT_STEP_NEW(INIT_firstGePID + 1);
			break;	
		case INIT_firstGePID + 1:  //等待运行键
			if(GetRunKey())
			{
				CLR_PART_ERR(GetSys(), ER0_SKEY);
				ClearRunKey();
				ACT_STEP_NEW(SetFistGeBSJ);
			}			
			break;
		case SetFistGeBSJ:    //起始代中每个个体执行前赋值  		
		case SetFistGeBSJ + 1:
		    if(ACT_STEP_100MS>1)
			{
				p_Press->p = p_Press->m_p[SIZE_COUNT];
		        p_Press->ki = p_Press->m_i[SIZE_COUNT];	

				
				CHK_SET_MOV(SetPid_EveryG);
	            ACT_STEP_NEW(UpFirstBsj);

				if((p_Press->p == MinP) && (p_Press->ki == MinI))
				{
					ACT_STEP_NEW(FirstGeEnd);
				}

				max_Overshoot = 0;
		    }
 			break;			
		case UpFirstBsj:     //起始代中每一个个体的上升阶段

			cycle_num ++;

			if(p_PressStr->presspwr.ek < 0)
			{
				if(abs(p_PressStr->presspwr.ek) > max_Overshoot)
				{
					max_Overshoot = abs(p_PressStr->presspwr.ek); 
					p_Press->chaotiao[SIZE_COUNT] = max_Overshoot;
				}
			}
			
			if ((cycle_num > Up_Time))    //到达给定的上升周期后，进入稳态
			{
				ACT_STEP_NEW(StateFirstBsj);
				cycle_num = 0;
			}
			
			break;
        
		case StateFirstBsj:	       //起始代中每一个个体的稳态阶段
		case StateFirstBsj + 1:
			cycle_num ++;
			OptiPara(SIZE_COUNT);	//进入稳态后开始计算指标函数的值

			if (cycle_num > XiaoDou_Time)    //消除执行过程中参数太不合适造成的噪声
			{
				if (abs((p_PressStr->presspwr.ek + SetPress_value) - (p_PressStr->presspwr.ek0 + SetPress_value)) > Adjust_slope)
				{
					p_Press->c[SIZE_COUNT] = Optipara_max;
					CHK_SET_MOV(OneAction_End);
					ACT_STEP_NEW(FirstGeEnd);
				}
			}

		    if (cycle_num > State_Time)    //稳态周期到达后，判断是否进行最优替换
			{
			    
				if (SIZE_COUNT == 1)
				{
					p_Press->BsJ = p_Press->c[SIZE_COUNT];
				}

				if( p_Press->c[SIZE_COUNT] <= p_Press->BsJ)
				{
					p_Press->Bests_p = p_Press->p;
					p_Press->Bests_i = p_Press->ki;
					p_Press->BsJ = p_Press->c[SIZE_COUNT];
				}

				CHK_SET_MOV(OneAction_End);
				ACT_STEP_NEW(FirstGeEnd);
			}
			break;				
		case FirstGeEnd:            //判断起始代中的所有个体是否执行完毕
			SIZE_COUNT ++;

			 //如果起始代中的所有 个体均被执行完毕，那么进入主循环代
		    if(SIZE_COUNT > SIZE - 1)  
		    {
		    	SIZE_COUNT  = 1;
				SIZE_G = 0;
		    	ACT_STEP_NEW(Opti_GePID);
				cycle_num = 0;
		    }
			else   //否则，执行下一个未被执行的个体
			{
				ACT_STEP_NEW(SetFistGeBSJ);
				cycle_num = 0;
			}			
			break;	
			
        case Opti_GePID:           //主循环代从第1代开始每一个个体进行交叉编译更新
		case Opti_GePID + 1:
			if(ACT_STEP_100MS>1)
			{
				SelfLearningPIDpara_G_p(SIZE_COUNT);
				SelfLearningPIDpara_G_i(SIZE_COUNT);
			
				CHK_SET_MOV(SetPid_EveryG);
				ACT_STEP_NEW(Opti_GePID_UP);

				max_Overshoot = 0;
			}
			break;
			
		case Opti_GePID_UP:		//主循环代从第1代开始每一个个体的上升阶段

            cycle_num ++;

			if(p_PressStr->presspwr.ek < 0)
			{
				if(abs(p_PressStr->presspwr.ek) > max_Overshoot)
				{
					max_Overshoot = abs(p_PressStr->presspwr.ek); 
					p_Press->chaotiao[SIZE_COUNT] = max_Overshoot;
				}
			}

			if ((cycle_num > Up_Time))		//到达给定的上升周期后，进入稳态
			{
				ACT_STEP_NEW(Opti_GePID_STATE);
				cycle_num = 0;
			}
			
			break;
			
		case Opti_GePID_STATE:		//主循环代从第1代开始每一个个体的稳态阶段
		case Opti_GePID_STATE+ 1:

			cycle_num ++;
			//if(cycle_num > 300)
			{
				OptiPara(SIZE_COUNT);  //进入稳态后开始计算指标函数的值
			}		


			if (cycle_num > XiaoDou_Time)  //消除执行过程中参数太不合适造成的噪声
			{
				if (abs((p_PressStr->presspwr.ek+SetPress_value) - (p_PressStr->presspwr.ek0+SetPress_value))> Adjust_slope)
				{
					p_Press->c[SIZE_COUNT] = Optipara_max;
					CHK_SET_MOV(OneAction_End);
					ACT_STEP_NEW(Opti_GePID_End);
					
				}
			}
		

			if (cycle_num > State_Time)     //稳态周期到达后，判断是否进行代间替换和最优替换
			{
			    if (p_Press->c[SIZE_COUNT] < p_Press->T_BsJ1)
				{
					p_Press->T_BsJ1 = p_Press->c[SIZE_COUNT];
					p_Press->m_p[SIZE_COUNT] = p_Press->p;
					p_Press->m_i[SIZE_COUNT] = p_Press->ki;
				
				}
				
				if (p_Press->T_BsJ1 < p_Press->BsJ)
				{
					p_Press->BsJ = p_Press->T_BsJ1;
					p_Press->Bests_p = p_Press->m_p[SIZE_COUNT];
					p_Press->Bests_i = p_Press->m_i[SIZE_COUNT];

				}

				SET_OFF(OneAction_End);
				ACT_STEP_NEW(Opti_GePID_End);
			}			
			break;			
		case Opti_GePID_End:		//判断当前代中的个体是否全部执行完成
			SIZE_COUNT++;
			if(SIZE_COUNT > SIZE - 1)
		    {
		    	ACT_STEP_NEW(NExt_G);
				cycle_num = 0;
		    }
			else
			{
				ACT_STEP_NEW(Opti_GePID);
				cycle_num = 0;
			}			
			break;
			
        case NExt_G: 		//进入 下一代
			SIZE_G++;
			if (SIZE_G < G)
			{
				ACT_STEP_NEW(Opti_GePID);
				SIZE_COUNT  = 1;	//进入下一代对个体重新计数
				cycle_num = 0;
			}
			else  
			{
				ACT_STEP_NEW(Opti_PID_End);
			}
            
			break;
		
        case Opti_PID_End:
		case Opti_PID_End + 1:
            CHK_SET_MOV(OneAction_End);
			pp->mh->PRESSELF_P = p_Press->Bests_p;
			pp->mh->PRESSELF_I = p_Press->Bests_i;
			PartDataTx((PPART)pp, MH_ID, 2, GET_IND(PHYDR_MACHSET, PRESSELF_P), FALSE);
			
			SetOperMode(OM_MANUAL);
			ret = ACT_END;
            break;        
        default:
			CHK_SET_MOV(OneAction_End);
			SetOperMode(OM_MANUAL);
            ret = ACT_END;
            break;
    }


    if (ret == ACT_END || ret == ACT_ERR_ABORT)
    {
        return ret;
    }
    else
    {
        return ret;
    }
}



