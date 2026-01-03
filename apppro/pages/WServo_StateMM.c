#include "WServo_StateMM.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "stdio.h"
#include "servo.h"
#include "canmod.h"
#include "texttbl.h"
#include "protcmd.h"
#include "common.h"
#include "buttonwgt.h"
#include "kjump.h"
#include "dataCfgtb.h"
#include "prot.h"
#include "privdef.h"
#include "ctrlcmd.h"

DEFINE_SERVO_STATEMM_MAP

CONNECT_SERVO_STATEMM_EVENT

#define TMP_MGMT_SERVO_MACHTYPE	    TMPSTR(0)   //机型 临时字符串变量0
#define TMP_MGMT_SRV_SPEED_0        TMPSTR(60) //麦米驱动转速0  TMPSTR(60)--TMPSTR(66)
#define TMP_SERVO_COMMSTATE      TMPSTR(40)  //麦米伺服通讯状态 临时字符串变量40
//#define ADDR_SERVO_CANST1			0x0a160058    //麦米伺服CAN通信状态1
extern PHASH_TBL pmap_tblx1;//can state

static void ServoStateMMUpdate()
{
    UI8 i,j;
    char cbuf[10];
    static UI32 time_300ms =0;
    const char *ptransx;

    // comm state Online or Offline
    if((GetTick()-time_300ms)>100)//300--100
    {
        time_300ms = GetTick();

        for(j=1;j<8;j++)
        {
            if(pmap_tblx1 !=NULL)
            {
                ptransx = GetMapTran(pmap_tblx1,(VarAdrToInt(d_servo1_STATE_MGMT_SRV_ST_1  + j -1)>0 ?1:0));
                VarAdrSetStr((TMP_SERVO_COMMSTATE+j-1),ptransx);
            }
        }
    }

    //servo speed rpm
    for(i=0;i<7;i++)
    {
        if( VarAdrToInt(d_servo1_STATE_MGMT_SRV_SPEED_DIRECT_1 + i)> 0 )//转速方向 0-正 1-负
        {
            snprintf(cbuf,sizeof(cbuf)-1,"-%d",VarAdrToInt(d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_1+i) );
        }
        else
        {
            snprintf(cbuf,sizeof(cbuf)-1,"%d",VarAdrToInt(d_servo1_STATE_MGMT_SRV_SPEED_PERCENT_1+i) );
        }

        VarAdrSetStr(TMP_MGMT_SRV_SPEED_0+i,cbuf);
    }
}

static void CtlConnect()
{
}
static int Servo_StateMMOnInit(Servo_StateMM_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_StateMMOnShow(Servo_StateMM_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    g_version = ReadVersion();
    VarAdrSetStr(TMP_MGMT_SERVO_MACHTYPE,g_version.system1);
    UI16 pdata[0];
    pdata[0] = 1;
    ProtSysCtrl(COMM_SUBCMD_MGMT_STATE_REQUEST,1,pdata);
    return 0;
}
static int Servo_StateMMOnHide(Servo_StateMM_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 pdata[0];
    pdata[0] = 0;
    ProtSysCtrl(COMM_SUBCMD_MGMT_STATE_REQUEST,1,pdata);
    return 0;
}
static int Servo_StateMMOnUpdate(Servo_StateMM_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ServoStateMMUpdate();
    return 0;
}
static int Servo_StateMMOnChange(Servo_StateMM_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 value;
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    return 0;
}
