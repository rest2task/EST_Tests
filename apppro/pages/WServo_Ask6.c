#include "WServo_Ask6.h"
#include "typedef.h"
#include "panel.h"
#include "servo.h"

DEFINE_SERVO_ASK6_MAP

CONNECT_SERVO_ASK6_EVENT

#define SERVO_PARRESETOK       0x331300d2  //驱动器参数重置对话框确认
#define SERVO_SELFLEARN        0x331300d3  //伺服自学习确认

//驱动器参数重置确认条件
extern BOOL bDriveParResetConfirm;
//界面切换确认条件
extern BOOL bSwitchPageConfirm;

static void ServoResetStatus()
{
	VarAdrSetInt(SERVO_PARRESETOK,0);
	VarAdrSetInt(SERVO_SELFLEARN,0);
	bSwitchPageConfirm = FALSE;
	bDriveParResetConfirm=FALSE;

}

static int okOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int cancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(servo_ask6_pg.cancel, WGT_CLICK_MSG, cancelOnClick);
    WgtMsgConnect(servo_ask6_pg.ok, WGT_CLICK_MSG, okOnClick);
}
static int Servo_Ask6OnInit(Servo_Ask6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_Ask6OnShow(Servo_Ask6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_Ask6OnHide(Servo_Ask6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_Ask6OnUpdate(Servo_Ask6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_Ask6OnChange(Servo_Ask6_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == PageGetWgtByName(servo_ask6_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(servo_ask6_pg_name);//20191223.cyx
    }
    return 0;
}
static int okOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	VarAdrSetInt(SERVO_PARRESETOK,1);//驱动参数重置确认框标记
	PanelCloseDialogByName(servo_ask6_pg_name);


    return 0;
}
static int cancelOnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	ServoResetStatus();
	PanelCloseDialogByName(servo_ask6_pg_name);

    return 0;
}
