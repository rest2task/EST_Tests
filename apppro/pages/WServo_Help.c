#include "WServo_Help.h"
#include "typedef.h"
#include "pagefram.h"
#include "mod.h"

DEFINE_SERVO_HELP_MAP

CONNECT_SERVO_HELP_EVENT

#define TMP_SERVO_HELPSELECT	TMPVAL(1)   //伺服帮助选择 临时变量
#define TMP_SERVO_ALARMNUM  	TMPVAL(2)   //伺服警报编号 临时变量

UI16  wHelp =0;
static  UI16  wErr=1;

static void CtlConnect()
{
}
static int Servo_HelpOnInit(Servo_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_HelpOnShow(Servo_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	VarAdrSetInt(TMP_SERVO_HELPSELECT,wHelp);
	VarAdrSetInt(TMP_SERVO_ALARMNUM,wErr);
//20181019hz
	switch(VarAdrToUI16(TMP_SERVO_HELPSELECT))
	{
	case 0:
		WGTSetVisible(ppg->label79, TRUE);
		WGTSetVisible(ppg->label80, FALSE);
		WGTSetVisible(ppg->label81, FALSE);
		WGTSetVisible(ppg->label82, FALSE);
		break;
	case 1:
		WGTSetVisible(ppg->label79, FALSE);
		WGTSetVisible(ppg->label80, TRUE);
		WGTSetVisible(ppg->label81, FALSE);
		WGTSetVisible(ppg->label82, FALSE);
		break;
	case 2:
		WGTSetVisible(ppg->label79, FALSE);
		WGTSetVisible(ppg->label80, FALSE);
		WGTSetVisible(ppg->label81, TRUE);
		WGTSetVisible(ppg->label82, FALSE);
		break;
	case 3:
		WGTSetVisible(ppg->label79, FALSE);
		WGTSetVisible(ppg->label80, FALSE);
		WGTSetVisible(ppg->label81, FALSE);
		WGTSetVisible(ppg->label82, TRUE);
		break;
	default:
		break;
	}

    return 0;
}
static int Servo_HelpOnHide(Servo_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_HelpOnUpdate(Servo_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Servo_HelpOnChange(Servo_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr;
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	switch(addr)
	{
	case TMP_SERVO_HELPSELECT:
		wHelp = VarAdrToUI16(TMP_SERVO_HELPSELECT);
        //20181019hz
		switch(wHelp)
		{
		case 0:
			WGTSetVisible(ppg->label79, TRUE);
			WGTSetVisible(ppg->label80, FALSE);
			WGTSetVisible(ppg->label81, FALSE);
			WGTSetVisible(ppg->label82, FALSE);
			break;
		case 1:
			WGTSetVisible(ppg->label79, FALSE);
			WGTSetVisible(ppg->label80, TRUE);
			WGTSetVisible(ppg->label81, FALSE);
			WGTSetVisible(ppg->label82, FALSE);
			break;
		case 2:
			WGTSetVisible(ppg->label79, FALSE);
			WGTSetVisible(ppg->label80, FALSE);
			WGTSetVisible(ppg->label81, TRUE);
			WGTSetVisible(ppg->label82, FALSE);
			break;
		case 3:
			WGTSetVisible(ppg->label79, FALSE);
			WGTSetVisible(ppg->label80, FALSE);
			WGTSetVisible(ppg->label81, FALSE);
			WGTSetVisible(ppg->label82, TRUE);
			break;
		default:
			break;
		}
		break;
	case TMP_SERVO_ALARMNUM:
		wErr = VarAdrToUI16(TMP_SERVO_ALARMNUM);
		break;
	default:
		break;
	}

    return 0;
}
