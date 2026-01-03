#include "WServo_Config.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "keymod.h"
#include "key.h"
#include "tabpagewgt.h"
#include "buttonwgt.h"
#include "servoconfig.h"
#include "stdio.h"
#include "oprintf.h"
#include "protcmd.h"

DEFINE_SERVO_CONFIG_MAP

CONNECT_SERVO_CONFIG_EVENT

#define SERVO_CONFIG_NUM 15 //伺服配置显示数量
#define HPARA_MAX_ROW    20 //H参数显示表格最大行数

#define TMPSTR_NUM_START        TMPSTR(0)   //参考编码起始关联值
#define TMPSTR_NAME_START       TMPSTR(20)  //参考名称起始关联值
#define TMPSTR_CURVALUE_START   TMPSTR(40)  //参考值起始关联值
#define TMPSTR_DEFVALUE_START   TMPSTR(60)  //出厂值起始关联值
#define TMPSTR_UNIT_START       TMPSTR(80)  //单位起始关联值
#define TMPSTR_PAGENUM_SHOW     TMPSTR(100) //页数显示

#define TMP_HPARA_SHOW          TMPVAL(0)   //H参数可见
#define TMP_ROW1_SHOW           TMPVAL(1)   //表格第一行可见
#define TMP_COLUMN_LENGTH       TMPVAL(100) //表格竖线长度
#define TMP_SERVO_ID            TMPVAL(200) //驱动编号

#define HEAD_HIGH 30 //表头高
#define ROW_HIGH  22 //行高

static UI16 cur_page = 1;//H参数当前子页
static UI16 maxpage = 1;//H参数最大页数
static int cur_tabpage = -1;//当前tab页

static UI8 read_servo_id = 1;
static UI8 req_servo_read_num = 0;
static PRO_SDO_RX_LOG servo_read[HPARA_MAX_ROW];//读取伺服数据
static UI16 servodata[2] = {0};//伺服回调附带数据

static void ButtonSetStatus(PWGT pwgt, BOOL status)
{
    if(status)
    {
        OBJSetValueByAttrName((POBJ)pwgt,"textColor",GUI_WHITE,0,NULL,ATTR_UINT);
        OBJSetValueByAttrName((POBJ)pwgt,"backColor",0xe1b400,0,NULL,ATTR_UINT);
    }
    else
    {
        OBJSetValueByAttrName((POBJ)pwgt,"textColor",GUI_BLACK,0,NULL,ATTR_UINT);
        OBJSetValueByAttrName((POBJ)pwgt,"backColor",0xFFFFFFFF,0,NULL,ATTR_UINT);
    }
}

//设置当前按钮状态
static void SetCurButtonState()
{
    int i;
    PWGT pwgt;
    for(i = 0; i < SERVO_CONFIG_NUM; i++)
    {
        pwgt = PageFramCurWgtByIndex(i+1);
        if(TabPageGetCurSel(servo_config_pg.tabpage1) == i)
        {
            ButtonSetStatus(pwgt, TRUE);
        }
        else
        {
            ButtonSetStatus(pwgt, FALSE);
        }
    }
}

//加载H参数数据
void LoadHParaData(int data_type, int page)
{
    int i;
    char pageshow[10];
    char buff[255];
    char inbuff[255],outbuff[255];
    int fontsize = 12;//字体大小
    int insize,maxsize,value_type;
    UI16 data_id;
    UI16 row_show_num = 0;//显示行数

    maxpage = (GetHParaNum(data_type)%HPARA_MAX_ROW)==0?GetHParaNum(data_type)/HPARA_MAX_ROW:GetHParaNum(data_type)/HPARA_MAX_ROW+1;
    if(page > maxpage)
    {
        return;
    }
    cur_page = page;
    snprintf(pageshow, sizeof(pageshow), "%d/%d", page, maxpage);
    VarAdrSetStr(TMPSTR_PAGENUM_SHOW, pageshow);

    for(i = 0; i < HPARA_MAX_ROW; i++)
    {
        data_id = i + 1 + (page - 1)*HPARA_MAX_ROW;
        if(data_id > GetHParaNum(data_type))
        {
            VarAdrSetInt(TMP_ROW1_SHOW+i, 0);//该行不可见
            VarAdrSetStr(TMPSTR_NUM_START+i,"");
            VarAdrSetStr(TMPSTR_NAME_START+i,"");
            VarAdrSetStr(TMPSTR_CURVALUE_START+i,"");
            VarAdrSetStr(TMPSTR_DEFVALUE_START+i,"");
            VarAdrSetStr(TMPSTR_UNIT_START+i,"");
        }
        else
        {
            row_show_num++;
            VarAdrSetInt(TMP_ROW1_SHOW+i, 1);//该行可见
            VarAdrSetStr(TMPSTR_NUM_START+i,GetHPara(data_type, "num", data_id, buff));

            OBJGetValueByAttrName((POBJ)servo_config_pg.subpage1_label19,"fontSize",&fontsize,0,NULL,&value_type);
            snprintf(inbuff, sizeof(inbuff), "%s", GetHPara(data_type, "name", data_id, buff));
            insize = strlen(inbuff) + 1;//非0即可
            maxsize = RU32(servo_config_pg.subpage1_label19->x_size);//控件宽度
            elidedText(fontsize, inbuff, insize, outbuff, maxsize);
            VarAdrSetStr(TMPSTR_NAME_START+i, outbuff);

            OBJGetValueByAttrName((POBJ)servo_config_pg.subpage1_edit1,"fontSize",&fontsize,0,NULL,&value_type);
            snprintf(inbuff, sizeof(inbuff), "%s", GetHPara(data_type, "curvalue", data_id, buff));
            insize = strlen(inbuff) + 1;//非0即可
            maxsize = RU32(servo_config_pg.subpage1_edit1->x_size) - 4;//编辑框宽度减去4
            elidedText(fontsize, inbuff, insize, outbuff, maxsize);
            //oprintf("%d: %d %s %d %s %d\n",i,fontsize,inbuff,insize,outbuff,maxsize);
            VarAdrSetStr(TMPSTR_CURVALUE_START+i, outbuff);

            VarAdrSetStr(TMPSTR_DEFVALUE_START+i,GetHPara(data_type, "defvalue", data_id, buff));
            VarAdrSetStr(TMPSTR_UNIT_START+i,GetHPara(data_type, "unit", data_id, buff));
        }
    }

    VarAdrSetInt(TMP_COLUMN_LENGTH , HEAD_HIGH + ROW_HIGH * row_show_num);
}

//接收伺服数据
static void Read_Callback(BOOL result, PRO_SDO_RX_LOG* logs, int cnt, void* param)
{
    int i;
    UI16 req_first_dataid;
    UI16 data[2];

    memcpy(data, param, sizeof(data));
    OBJMemFree(param);
    param = NULL;

    oprintf("SERVO_CONFIG result:%d,cnt:%d data = %d,%d\n\n",result,cnt,data[0],data[1]);
    if (result)
    {
        if(data[0]==servodata[0] && data[1]==servodata[1])
        {
            req_first_dataid = 1 + (servodata[1] - 1)*HPARA_MAX_ROW;
            for(i = 0; i < req_servo_read_num; i++)
            {
                SetHPara_CurValue(servodata[0], req_first_dataid+i, (logs+i)->servo_val);
            }
            LoadHParaData(servodata[0], servodata[1]);
        }
    }
}

//请求伺服数据
static void ReqServoIdData(int data_type, int page)
{
    int i, j;
    UI16 data_id;
    char buff[255];
    char addr[SERVOCONFIG_NUM_LEN];
    UI16 *param = NULL;

    if(page > maxpage)
    {
        return;
    }

    req_servo_read_num = GetHParaNum(data_type) - (page - 1)*HPARA_MAX_ROW;
    if(req_servo_read_num > HPARA_MAX_ROW)
    {
        req_servo_read_num = HPARA_MAX_ROW;
    }

    servodata[0] = data_type;
    servodata[1] = page;

    param = OBJMemMalloc(sizeof(servodata));
    memcpy(param, servodata, sizeof(servodata));

    for(i = 0; i < req_servo_read_num; i++)
    {
        data_id = i + 1 + (page - 1)*HPARA_MAX_ROW;
        servo_read[i].servo_sn = read_servo_id;
        servo_read[i].servo_len = (UI8)StrToWord(GetHPara(data_type, "width", data_id, buff), 0);
        servo_read[i].servo_func = 0x2000;
        snprintf(addr,sizeof(addr),"%s",GetHPara(data_type, "num", data_id, buff));
        for(j = 0; j < strlen(addr)-1; j++)
        {
            addr[j] = addr[j+1];
        }
        addr[j] = '\0';
        sscanf(addr,"%X",&(servo_read[i].servo_addr));
        //oprintf("%d : servo_sn = %d,servo_len = %d,addr = %x\n",i,servo_read[i].servo_sn,servo_read[i].servo_len,servo_read[i].servo_addr);
    }

    oprintf("ReqServoIdData: data = %d,%d\n",servodata[0],servodata[1]);
    ServoCtlRead(servo_read, req_servo_read_num, Read_Callback, param);
}

//tabpage 可见不可见设置
static void ServoConfigTabPageShow()
{
    int i;
    VarAdrSetInt(TMP_HPARA_SHOW, 0);
    for(i = 0; i < SERVO_CONFIG_NUM; i++)
    {
        if(GetHParaNum(i+1) > 0)
        {
            VarAdrSetInt(TMP_HPARA_SHOW, VarAdrToInt(TMP_HPARA_SHOW)|(1<<i));
            TabPageSetSubVisble(servo_config_pg.tabpage1, i, TRUE);
        }
        else
        {
            VarAdrSetInt(TMP_HPARA_SHOW, VarAdrToInt(TMP_HPARA_SHOW)&(~(1<<i)));
            TabPageSetSubVisble(servo_config_pg.tabpage1, i, FALSE);
        }
    }

    for(i = 0; i < SERVO_CONFIG_NUM; i++)
    {
        if(VarAdrToInt(TMP_HPARA_SHOW) & (1<<i))
        {
            TabPageSelPage(servo_config_pg.tabpage1, i);
            break;
        }
    }

    if(VarAdrToInt(TMP_HPARA_SHOW))
    {
        if(VarAdrToInt(SERVO_CURRENT_NUM) > 6)
        {
//20220805 dyl
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
            WGTSetVisible(servo_config_pg.stTips_F2, FALSE);
            WGTSetVisible(servo_config_pg.stTips_F6, FALSE);
#else//G10/G15:KeyBoard
            WGTSetVisible(servo_config_pg.stTips_F2, TRUE);
            WGTSetVisible(servo_config_pg.stTips_F6, FALSE);
#endif
            FkeyTab(servo_config_pg.tabpage1, GUI_KEY_FUN2);
        }
        else
        {
//20220805 dyl
#ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
            WGTSetVisible(servo_config_pg.stTips_F2, FALSE);
            WGTSetVisible(servo_config_pg.stTips_F6, FALSE);
#else//G10/G15:KeyBoard
            WGTSetVisible(servo_config_pg.stTips_F2, FALSE);
            WGTSetVisible(servo_config_pg.stTips_F6, TRUE);
#endif
            FkeyTab(servo_config_pg.tabpage1, GUI_KEY_FUN6);
        }
        TabPageSetSubVisble(servo_config_pg.tabpage1, SERVO_CONFIG_NUM, FALSE);
    }
    else
    {
        WGTSetVisible(servo_config_pg.stTips_F2, FALSE);
        WGTSetVisible(servo_config_pg.stTips_F6, FALSE);
        TabPageSetSubVisble(servo_config_pg.tabpage1, SERVO_CONFIG_NUM, TRUE);
        TabPageSelPage(servo_config_pg.tabpage1, SERVO_CONFIG_NUM);
    }
}

static void CtlConnect()
{
}
static int Servo_ConfigOnInit(Servo_Config_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    LoadServoConfig();
    return 0;
}
static int Servo_ConfigOnShow(Servo_Config_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    LoadServoConfig();
    ServoConfigTabPageShow();

    VarAdrSetInt(TMP_SERVO_ID, read_servo_id);

    cur_tabpage = -1;

    return 0;
}
static int Servo_ConfigOnHide(Servo_Config_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}

static int Servo_ConfigOnUpdate(Servo_Config_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    static UI32 time_500ms = 0;

    SetCurButtonState();

    if(cur_tabpage != TabPageGetCurSel(ppg->tabpage1))
    {
        time_500ms = GetTick();
        cur_tabpage = TabPageGetCurSel(ppg->tabpage1);
        LoadHParaData(cur_tabpage+1, 1);
    }

    if(GetTick() - time_500ms >= 500)
    {
        if(servodata[0] != (cur_tabpage+1) || servodata[1] != cur_page)
        {
            ReqServoIdData(cur_tabpage+1, cur_page);
        }
    }

    return 0;
}
static int Servo_ConfigOnChange(Servo_Config_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 offset;
    //20210914 dyl touch 根据索引来定位那个tab子页，兼容触摸和按键
    offset = WGTGetGOffset(pwgt);
    if(offset > 0 && offset <= SERVO_CONFIG_NUM)
    {
        TabPageSelPage(ppg->tabpage1, offset-1);
    }

    if(pwgt==ppg->btlast)
    {
        if(cur_page > 1)
        {
            cur_page--;
            LoadHParaData(TabPageGetCurSel(ppg->tabpage1)+1, cur_page);
        }
    }
    else if(pwgt==ppg->btnext)
    {
        if(cur_page < maxpage)
        {
            cur_page++;
            LoadHParaData(TabPageGetCurSel(ppg->tabpage1)+1, cur_page);
        }
    }
    else if(pwgt==ppg->edservoid)
    {
        read_servo_id = VarAdrToInt(TMP_SERVO_ID);
        ReqServoIdData(TabPageGetCurSel(ppg->tabpage1)+1, cur_page);
    }
    else if(pwgt==ppg->btread)
    {
        ReqServoIdData(TabPageGetCurSel(ppg->tabpage1)+1, cur_page);
    }

    return 0;
}
