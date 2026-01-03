#ifndef _HYDRPOS_H_
#define _HYDRPOS_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "HydrPosApi.h"

/*---------------------------------------------------------------------------+
|           NewPosControl                                                    |
+---------------------------------------------------------------------------*/

typedef struct tyHydrStruData
{
	UI16	Max_turn;			//驱动器的最大转速
	UI16	Cylinder_R;			//油缸直径	单位mm
	UI16	Cylinder_r;			//活塞杆直径		单位mm
	UI16	Oilpump_Q; 			//油泵排量	单位cc/转

	UI16	Min_flow;			//最小流量	0.1%
	UI16	Min_press;			//最小压力	0.1%
	UI16	Min_turn;			//最小转速	1rpm
	UI16	kp_pos;				//位置Kp
	UI16	Cylinder_num;		//油泵个数

	UI16	k_MotortoAD;		//转速对应实际移动速度的公约数
}HydrStruData, *PHydrStruData;


void SetPosCtrlPara(WORD ServoNo, WORD RunDirec, WORD AdNo, int *PSramp, int *Pspd, int *PPramp, int *Ppres, int *Ppos, UI16 seg, UI16 dietime, UI16 hydrdlytime, UI16 mode,int sm, int diffpos, int movenum);
void InitSetHydrStruCtrlData(WORD portno, UI16 max_turn, UI16 r, UI16 R, UI16 Cylinder_num,UI16 q,UI16 q_k, UI16 min_press, UI16 min_flow, UI16 k);
WORD SmoothStop(WORD HydrPosId, WORD RunDirec, WORD AdNo, int PSramp, ACT_OP_FUNC_NEW VlvFunc, PACT pact);
int NewPosControl(WORD PortNo, int SetPress, int SetSpd, int LastPress, int LastSpd);


#ifdef __cplusplus
}
#endif

#endif

