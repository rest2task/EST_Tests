#include "WWake_Screen.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "keyprot.h"
#include "keyprot2.h"

DEFINE_WAKE_SCREEN_MAP

CONNECT_WAKE_SCREEN_EVENT

static void CtlConnect()
{
}
static int Wake_ScreenOnInit(Wake_Screen_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Wake_ScreenOnShow(Wake_Screen_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Wake_ScreenOnHide(Wake_Screen_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Wake_ScreenOnUpdate(Wake_Screen_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Wake_ScreenOnChange(Wake_Screen_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    /*
     * 值为1时表示在按键测试页面显示唤醒页面
     * 值为0时表示按键测试页面已不处于唤醒页面状态
     * */

    if(pwgt == ppg->btscreen)//截屏
    {
        SetLCD(TRUE);//20190417
        Send_LCDOnOff(TRUE);//20190822.cyx
        VarAdrSetInt(PT15_SCREEN_PROTECT,0);
        VarAdrSetInt(PT15_WAKE_SCREEN_SHOW,0);

        PanelCloseDialogByName(wake_screen_pg_name);
    }

    return 0;
}
