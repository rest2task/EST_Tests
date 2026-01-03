#include "WServoDebug1.h"
#include "typedef.h"
#include "pagefram.h"
#include "protcmd.h"
#include "editwgt.h"
#include "stdio.h"
#include "keymod.h"
#include "key.h"
#include "tabpagewgt.h"
#include "imagewgt.h"
#include "privdef.h"
#include "oprintf.h"
#include "database.h"
#include "kjump.h"
#include "editwgt.h"
#include "information.h"

DEFINE_SERVODEBUG1_MAP

CONNECT_SERVODEBUG1_EVENT

//驱动器参数交互
#define SERVO_PAR_NO       16
static PRO_SDO_RX_LOG servo_read[SERVO_PAR_NO];
//#define REFRESH_SERVODATA 0x0112 //重新读取驱动器参数
static UI16 ServoAdd[16] =   //驱动器地址指针
{
        0x0100, //MCU
        0x0101, //FPGA
        0x0104, //驱动器额定电压
        0x0105, //驱动器额定功率
        0x0107, //驱动器额定电流
        0x0009, //电机额定电压
        0x0010, //电机额定功率
        0x0011, //电机额定电流
        0x0012, //电机额定扭矩
        0x0013, //电机峰值扭矩
        0x0014, //电机额定转速
        0x0015, //电机最大转速
        0x0000, //编码器类型
        0x0202, //旋转方向
        0x0803, //高速增益
        0x0800  //低速增益
};

static UI8 ServoLen[16] ={0};//驱动器数据长度指针

static UI16 ServoFunc[16] ={0};//驱动器参数功能码指针

static UI8 ServoId[6] ={1,2,3,4,5,6};//驱动器的编号指针

static UI8 old_page;

/**
* @brief     :画面控件数据显示
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191113
*/
static void GetServoCfg(PRO_SDO_RX_LOG* logs)
{
    char cbuf[20];

    //驱动
    sprintf(cbuf,"%.1f",(logs)->servo_val/ 10.);//驱动MCU软件版本号
    EditSetText(servodebug1_pg.stMCU,cbuf);
    sprintf(cbuf,"%.1f",(logs+1)->servo_val/ 10.);//驱动FPGA软件版本号
    EditSetText(servodebug1_pg.stFPGA,cbuf);
    sprintf(cbuf,"%d",(logs+2)->servo_val);//驱动额定电压
    EditSetText(servodebug1_pg.stRatedVoltage,cbuf);
    sprintf(cbuf,"%.2f",(logs+3)->servo_val/ 100.);//驱动额定功率
    EditSetText(servodebug1_pg.stRatedPower,cbuf);
    sprintf(cbuf,"%.2f",(logs+4)->servo_val/ 100.);//驱动额定电流
    EditSetText(servodebug1_pg.stRatedCurrent,cbuf);

    //电机
    if((logs+5)->servo_val == 0)
    {
        sprintf(cbuf,"%d",220);//电机额定电压
    }
    else if((logs+5)->servo_val == 1)
    {
        sprintf(cbuf,"%d",380);//电机额定电压
    }
    else
    {
       sprintf(cbuf,"%d",(logs+5)->servo_val);//电机额定电压
    }
    EditSetText(servodebug1_pg.stMotorRatedVoltage,cbuf);
    sprintf(cbuf,"%.2f",(logs+6)->servo_val / 100.);//电机额定功率
    EditSetText(servodebug1_pg.stMotorPower,cbuf);
    sprintf(cbuf,"%.2f",(logs+7)->servo_val / 100.);//电机额定电流
    EditSetText(servodebug1_pg.stMotorRatedCurrent,cbuf);
    sprintf(cbuf,"%.2f",(logs+8)->servo_val / 100.);//电机额定扭矩
    EditSetText(servodebug1_pg.stRatedTorque,cbuf);
    sprintf(cbuf,"%.2f",(logs+9)->servo_val / 100.);//电机峰值扭矩
    EditSetText(servodebug1_pg.stMaxTorque,cbuf);
    sprintf(cbuf,"%d",(logs+10)->servo_val);//电机额定转速
    EditSetText(servodebug1_pg.stRatedRPM,cbuf);
    sprintf(cbuf,"%d",(logs+11)->servo_val);//电机最大转速
    EditSetText(servodebug1_pg.stMaxRPM,cbuf);
    sprintf(cbuf,"%d",(logs+12)->servo_val);//电机编码器类型
    EditSetText(servodebug1_pg.stEncodeType,cbuf);

    //参数调整
    VarAdrSetInt(p_PP_TMP_TMPVAL14,(logs+13)->servo_val); //旋转方向增益     临时变量14
    VarAdrSetInt(p_PP_TMP_TMPVAL15,(logs+14)->servo_val); //高速增益        临时变量15
    VarAdrSetInt(p_PP_TMP_TMPVAL16,(logs+15)->servo_val); //低速增益        临时变量16
}

static void Read_Callback(BOOL result, PRO_SDO_RX_LOG* logs, int cnt, void* param)
{
    oprintf("result:%d,cnt:%d\n",result,cnt);
    if (!result)
    {
        oprintf("result 0");
    }
    else
    {
        EditSetReadOnly(servodebug1_pg.stRotatedDir,FALSE);
        EditSetBKColor(servodebug1_pg.stRotatedDir,0xffffff);
        EditSetReadOnly(servodebug1_pg.stHighSpdGain,FALSE);
        EditSetBKColor(servodebug1_pg.stHighSpdGain,0xffffff);
        EditSetReadOnly(servodebug1_pg.stLowSpdGain,FALSE);
        PageWgtSetFocus(servodebug1_pg.stRotatedDir);
        EditSetBKColor(servodebug1_pg.stLowSpdGain,0xffffff);
        WGTSetVisible(servodebug1_pg.label2,FALSE);
        GetServoCfg(logs);
    }
}
static void Write_Callback(BOOL result, PRO_SDO_RX_LOG* logs, int cnt, void* param)
{
    return;
}
/**
* @brief     :画面请求数据，根据画面设计，单次只请求一个驱动器数据
* @param     :驱动器id
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191113
*/
static void ReqServoIdData(UI16 servo_id)
{
    int i;

    EditSetReadOnly(servodebug1_pg.stRotatedDir,TRUE);
    EditSetBKColor(servodebug1_pg.stRotatedDir,0xdedede);
    EditSetReadOnly(servodebug1_pg.stHighSpdGain,TRUE);
    EditSetBKColor(servodebug1_pg.stHighSpdGain,0xdedede);
    EditSetReadOnly(servodebug1_pg.stLowSpdGain,TRUE);
    EditSetBKColor(servodebug1_pg.stLowSpdGain,0xdedede);
    WGTSetVisible(servodebug1_pg.label2,TRUE);
    for(i = 0; i < SERVO_PAR_NO; ++i)
    {
        servo_read[i].servo_sn = ServoId[servo_id];
        servo_read[i].servo_len = ServoLen[i];
        servo_read[i].servo_func = ServoFunc[i];
        servo_read[i].servo_addr = ServoAdd[i];
    }
    ServoCtlRead(servo_read, SERVO_PAR_NO, Read_Callback, NULL);
    return;
}

/**
* @brief     :数据改变，发送
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191113
*/
static void SendServoParData(UI16 servo_id, UI16 servo_par_id, UI32 data)
{
    PRO_SDO_RX_LOG *log;
    log = (PRO_SDO_RX_LOG*)OBJMemMalloc(sizeof(PRO_SDO_RX_LOG));
    log->servo_sn = ServoId[servo_id];
    log->servo_len = ServoLen[servo_id];
    log->servo_func = ServoFunc[servo_id];
    log->servo_addr = servo_par_id;
    log->servo_val = data;
    ServoCtlWrite(log, 1, Write_Callback, NULL);
    OBJMemFree(log);
}

static void PageShow(ServoDebug1_PG* ppg)
{
    UI8 bit;
    int servo_num;
    bit = 0;
    servo_num = VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM);
    switch (servo_num) {
    case 1:
        bit = 1;
        break;
    case 2:
        bit = 3;
        break;
    case 3:
        bit = 7;
        break;
    case 4:
        bit = 15;
        break;
    case 5:
        bit = 31;
        break;
    default:
        bit = 63;
        break;
    }
    TabPageSetSubVisble(ppg->tabpage1, 0, getbit(bit, 0));
    TabPageSetSubVisble(ppg->tabpage1, 1, getbit(bit, 1));
    TabPageSetSubVisble(ppg->tabpage1, 2, getbit(bit, 2));
    TabPageSetSubVisble(ppg->tabpage1, 3, getbit(bit, 3));
    TabPageSetSubVisble(ppg->tabpage1, 4, getbit(bit, 4));
    TabPageSetSubVisble(ppg->tabpage1, 5, getbit(bit, 5));

    if(VarAdrToInt(p_PP_SERVO_SERVO_CURRENT_NUM) < 2)
    {
        WGTSetVisible(ppg->label1, FALSE);
    }
    else
    {
        WGTSetVisible(ppg->label1, TRUE);
    }
}

static void InitTempData() //初始化临时变量
{
    int i;
    for(i = 0; i < 13; i++)
    {
        VarAdrSetStr(p_PP_TMP_TMPSTR1 + i,"0");
    }

    VarAdrSetInt(p_PP_TMP_TMPVAL14, 0); //旋转方向增益     临时变量14
    VarAdrSetInt(p_PP_TMP_TMPVAL15, 0); //高速增益        临时变量15
    VarAdrSetInt(p_PP_TMP_TMPVAL16, 0); //低速增益        临时变量16
}

static void CtlConnect()
{
}
static int ServoDebug1OnInit(ServoDebug1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;
    for(i = 0; i < SERVO_PAR_NO; ++i)
    {
        ServoLen[i] = 16;
        ServoFunc[i] = 0x2000;
    }
    return 0;
}

static int ServoDebug1OnShow(ServoDebug1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    InitTempData();
    TabPageSelPage(ppg->tabpage1, 0);
    old_page = 0;
    ReqServoIdData(0);
    FkeyTab(ppg->tabpage1, GUI_KEY_FUN2);
    ImageSetIndex(ppg->image1, 1);
    PageShow(ppg);
    return 0;
}
static int ServoDebug1OnHide(ServoDebug1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ServoDebug1OnUpdate(ServoDebug1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI16 cur_page = TabPageGetCurSel(ppg->tabpage1);
    if(old_page != cur_page && !WGTGetVisible(servodebug1_pg.label2))
    {
        ReqServoIdData(cur_page);
        ImageSetIndex(ppg->image1, cur_page + 1);
        old_page = cur_page;
    }
    return 0;
}
static int ServoDebug1OnChange(ServoDebug1_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);

    if(pwgt == ppg->tabpage1)
        return 0;

    if(VarAdrToUI16(OPMODE_INDEX)!=MODE_MANUAL || VarAdrToInt(d_motor1_STATE_FL_MOTORSTATUS)!=0)
    {
        VarSendSaveSetIntByAdr(addr,msg_value);//原始值
        ShowMsg(VM_MSG_MANUAL_CUTMOTOR);
        return -1;
    }

    if(pwgt == ppg->stRotatedDir)
    {
        SendServoParData(TabPageGetCurSel(ppg->tabpage1), ServoAdd[13], VarAdrToInt(p_PP_TMP_TMPVAL14));
    }
    else if(pwgt == ppg->stHighSpdGain)
    {
        SendServoParData(TabPageGetCurSel(ppg->tabpage1), ServoAdd[14], VarAdrToInt(p_PP_TMP_TMPVAL15));
    }
    else if(pwgt == ppg->stLowSpdGain)
    {
        SendServoParData(TabPageGetCurSel(ppg->tabpage1), ServoAdd[15], VarAdrToInt(p_PP_TMP_TMPVAL16));
    }
    return 0;
}
