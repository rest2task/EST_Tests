#include "WMessage_Touch.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "stdio.h"
#include "texttbl.h"
#include "privdef.h"
#include "labelwgt.h"

DEFINE_MESSAGE_TOUCH_MAP

CONNECT_MESSAGE_TOUCH_EVENT

#define CALIBRATE_FILE      "/opt/tslib/etc/pointercal"

static UI16 Num = 5;
static UI32 time_1s = 0;

static void CtlConnect()
{
}
static int Message_TouchOnInit(Message_Touch_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Message_TouchOnShow(Message_Touch_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{   
    char buff[255];
    time_1s = GetTick();

    snprintf(buff,sizeof(buff)-1,"%d %s",Num,GetTextTran(TEXT_MSG_CALIBRATE));

    LabelSetText(ppg->lblinfo,buff);

    return 0;
}
static int Message_TouchOnHide(Message_Touch_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Message_TouchOnUpdate(Message_Touch_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if (Num !=0)
    {
        if(GetTick() - time_1s >= 1000)
        {
            char buff[255];
            time_1s = GetTick();
            Num--;
            snprintf(buff,sizeof(buff)-1,"%d %s",Num,GetTextTran(TEXT_MSG_CALIBRATE));
            LabelSetText(ppg->lblinfo,buff);
        }
    }

    if(Num <= 0)
    {
        char scmd[128];
        Num = 0;
        snprintf(scmd,sizeof(scmd)-1,CMD_DEL" %s",CALIBRATE_FILE);
        osystem(scmd);
        PanelMainRestart();
    }
    return 0;
}
static int Message_TouchOnChange(Message_Touch_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
