#include "command.h"
#include "oprintf.h" 
#include "protcmd.h"
#include "ctrlcmd.h"

/************************************************************************/
/* 松模初始化话命令                                                     */
/************************************************************************/
void ReleaseMdInit()
{
	ProtSysCtrl(MOLD_ADJ_CMD,0,NULL);
}
/************************************************************************/
/* 发送code码给下位机，然后发送保存命令                                 */
/************************************************************************/
void CodeSendSave()
{
	int len;
	UI16 part_sub_id;
	UI16 part_id;

	part_id = PART1_ID(MACHINE_ID);
	part_sub_id = part_id | MACHSET;
    len = GetSubLenByID(part_sub_id);
	ProtSendPartSub(part_id,MACHSET,len,GetSubByID(part_sub_id, NULL));

	part_id = PART1_ID(METER_ID);
	part_sub_id = part_id | MACHSET;
	len = GetSubLenByID(part_sub_id);
	ProtSendPartSub(part_id,MACHSET,len,GetSubByID(part_sub_id, NULL));
	ProtSysCtrl(MACHINE_CFG_MODI_CMD,0,NULL);//save cmd
}

void CodeSendSaveBack()//20230518 chj 此函数调用后，告诉主机需要提示重启,内容同CodeSendSave一样
{
    int len;
    UI16 part_sub_id;
    UI16 part_id;
    UI16 FLAG = 0x95;

    part_id = PART1_ID(MACHINE_ID);
    part_sub_id = part_id | MACHSET;
    len = GetSubLenByID(part_sub_id);
    ProtSendPartSub(part_id,MACHSET,len,GetSubByID(part_sub_id, NULL));

    part_id = PART1_ID(METER_ID);
    part_sub_id = part_id | MACHSET;
    len = GetSubLenByID(part_sub_id);
    ProtSendPartSub(part_id,MACHSET,len,GetSubByID(part_sub_id, NULL));
    ProtSysCtrl(MACHINE_CFG_MODI_CMD,1,&FLAG);//save cmd
}

/************************************************************************/
/* 发送AD归零位置给下位机，然后发归零命令                               */
/************************************************************************/
BOOL AdMeterZero(UI32 zero_addr,UI32 abs_addr)
{
	PVAR pdata;

	VarAdrSetInt(zero_addr,VarAdrToUI16(abs_addr));
	VarSendByAdr(zero_addr);
	if(pdata = VarAdrToVar(zero_addr))
		VarSaveInt(pdata);
	ProtSysCtrl(METER_ZERO_ADJ_CMD,0,NULL);//zero cmd

	return TRUE;
}
/************************************************************************/
/* 主机调试实际温度归零命令                                             */
/************************************************************************/
void RealTempZero()
{
	ProtSysCtrl(TEMP_ZERO_CMD,0,NULL);
}
/************************************************************************/
/* 主机调试温度校正命令                                                 */
/************************************************************************/
void TempCorrection()
{
	ProtSysCtrl(TEMP_ADJ_CMD,0,NULL);
}
/************************************************************************/
/* 主机调试室温校正命令                                                 */
/************************************************************************/
void RoomTempSend()
{
	ProtSysCtrl(AMB_TEMP_ADJ_CMD,0,NULL);
}


