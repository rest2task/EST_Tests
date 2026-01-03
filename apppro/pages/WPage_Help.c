#include "WPage_Help.h"
#include "typedef.h"
#include "pagefram.h"
#include "panel.h"
#include "mod.h"
#include "browserwgt.h"
#include "file.h"
#include "alarmrecord.h"
#include "oprintf.h"
#include "tabpagewgt.h"
#include <stdio.h>
#include "privdef.h"
#include "alarmrecord_G10.h"
#include "labelwgt.h"
#include "texttbl.h"
#include "database.h"
#include "time.h"

DEFINE_PAGE_HELP_MAP

CONNECT_PAGE_HELP_EVENT

#define     WARN_HELP_URL   HMI_ROOT_PATH"warnhelp/"
#define     SERVOWARN_HELP_URL      HMI_ROOT_PATH"servo_help/servo_ERR_cn_"
#define     SERVOWARN_HELP_URL_EN   HMI_ROOT_PATH"servo_help/servo_ERR_en_"
#define     FILE_SUFFIX     ".html"
#define     NO_HELP         HMI_ROOT_PATH"warnhelp/nohelp.html"
#define     NO_HELP_EN      HMI_ROOT_PATH"warnhelp/nohelp_en.html"

#define WARN_NUM_MAX    10 //最大警报数量
#define MAX_ALARM_CHAR_NUM  100 //最大警报字符长度
static char alarminfo[WARN_NUM_MAX][MAX_ALARM_CHAR_NUM];//显示警报全部内容

extern UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
static UI32 detail_alarm = 0;
static UI32 default_time = 0;
extern UI32 dwPanelErr;//20210914 dyl touch

//警报内容超过宽度显示省略号
static char* LabelAlarm_elidedText(char*input)
{
    PWGT pwgt = page_help_pg.TB_Tb1_subpage1_lblalarm;
    int fontsize,insize,maxsize,value_type;
    char outbuff[MAX_ALARM_CHAR_NUM];

    if(input == NULL)
    {
        return NULL;
    }

    OBJGetValueByAttrName((POBJ)pwgt,"fontSize",&fontsize,0,NULL,&value_type);//字体大小
    insize = 1;//非0即可
    maxsize = RU32(pwgt->x_size);//控件宽度
    elidedText(fontsize, input, insize, outbuff, maxsize);

    memcpy(input, outbuff, MAX_ALARM_CHAR_NUM);

    return input;
}

//显示警报表
static void ShowAlarmTable()
{
    UI32 dwWarn[WARN_NUM_MAX]={g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10};
    PWGT button_alarm[WARN_NUM_MAX]={
        page_help_pg.TB_Tb1_button1,page_help_pg.TB_Tb1_button2,page_help_pg.TB_Tb1_button3,
        page_help_pg.TB_Tb1_button4,page_help_pg.TB_Tb1_button5,page_help_pg.TB_Tb1_button6,
        page_help_pg.TB_Tb1_button7,page_help_pg.TB_Tb1_button8,page_help_pg.TB_Tb1_button9,
        page_help_pg.TB_Tb1_button10};
    PWGT subpage_alarm[WARN_NUM_MAX]={
        page_help_pg.TB_Tb1_subpage1,page_help_pg.TB_Tb1_subpage2,page_help_pg.TB_Tb1_subpage3,
        page_help_pg.TB_Tb1_subpage4,page_help_pg.TB_Tb1_subpage5,page_help_pg.TB_Tb1_subpage6,
        page_help_pg.TB_Tb1_subpage7,page_help_pg.TB_Tb1_subpage8,page_help_pg.TB_Tb1_subpage9,
        page_help_pg.TB_Tb1_subpage10};
    PWGT lblalarmnum[WARN_NUM_MAX]={
        page_help_pg.TB_Tb1_subpage1_lblalarmnum,page_help_pg.TB_Tb1_subpage2_lblalarmnum,page_help_pg.TB_Tb1_subpage3_lblalarmnum,
        page_help_pg.TB_Tb1_subpage4_lblalarmnum,page_help_pg.TB_Tb1_subpage5_lblalarmnum,page_help_pg.TB_Tb1_subpage6_lblalarmnum,
        page_help_pg.TB_Tb1_subpage7_lblalarmnum,page_help_pg.TB_Tb1_subpage8_lblalarmnum,page_help_pg.TB_Tb1_subpage9_lblalarmnum,
        page_help_pg.TB_Tb1_subpage10_lblalarmnum};
    PWGT lblalarm[WARN_NUM_MAX]={
        page_help_pg.TB_Tb1_subpage1_lblalarm,page_help_pg.TB_Tb1_subpage2_lblalarm,page_help_pg.TB_Tb1_subpage3_lblalarm,
        page_help_pg.TB_Tb1_subpage4_lblalarm,page_help_pg.TB_Tb1_subpage5_lblalarm,page_help_pg.TB_Tb1_subpage6_lblalarm,
        page_help_pg.TB_Tb1_subpage7_lblalarm,page_help_pg.TB_Tb1_subpage8_lblalarm,page_help_pg.TB_Tb1_subpage9_lblalarm,
        page_help_pg.TB_Tb1_subpage10_lblalarm};
    PWGT lblalarmtm[WARN_NUM_MAX]={
        page_help_pg.TB_Tb1_subpage1_lblalarmtm,page_help_pg.TB_Tb1_subpage2_lblalarmtm,page_help_pg.TB_Tb1_subpage3_lblalarmtm,
        page_help_pg.TB_Tb1_subpage4_lblalarmtm,page_help_pg.TB_Tb1_subpage5_lblalarmtm,page_help_pg.TB_Tb1_subpage6_lblalarmtm,
        page_help_pg.TB_Tb1_subpage7_lblalarmtm,page_help_pg.TB_Tb1_subpage8_lblalarmtm,page_help_pg.TB_Tb1_subpage9_lblalarmtm,
        page_help_pg.TB_Tb1_subpage10_lblalarmtm};

    char strid[MAX_ALARM_CHAR_NUM];
    int i;
    int warnpage = 0;//警报页数

    //20210914 dyl touch
    if(dwPanelErr == 0x34100101)//按键板通讯警报
    {
        snprintf(strid,sizeof(strid)-1,"#%05X",ALARM_NUMBER(dwPanelErr - 0x101));
        LabelSetText(lblalarmnum[warnpage], strid);

        snprintf(strid,sizeof(strid)-1,"%x",dwPanelErr);
        if(pmap_alarm!=NULL)
        {
            if(GetMapTran(pmap_alarm,dwPanelErr) != NULL)
            {
                snprintf(strid,sizeof(strid)-1,"%s",GetMapTran(pmap_alarm,dwPanelErr));
            }
        }
        memcpy(alarminfo[warnpage], strid, MAX_ALARM_CHAR_NUM);
        LabelSetText(lblalarm[warnpage], LabelAlarm_elidedText(strid));

        TimeToStr(strid,"yyyy-MM-dd hh:mm:ss",GetPanelErrTime());
        LabelSetText(lblalarmtm[warnpage], strid);

        warnpage++;

        WGTSetVisible(page_help_pg.TB,TRUE);
        WGTSetVisible(page_help_pg.lblinfo,FALSE);

        for(i = 0;i < warnpage;i++)
        {
            WGTSetVisible(subpage_alarm[i], TRUE);
            WGTSetVisible(button_alarm[i], TRUE);
        }
        for(i = warnpage;i < WARN_NUM_MAX;i++)
        {
            WGTSetVisible(subpage_alarm[i], FALSE);
            WGTSetVisible(button_alarm[i], FALSE);
        }

        return;
    }

    for(i = 0;i < WARN_NUM_MAX;i++)
    { 
        if(dwWarn[i] != 0)//有警报
        {
            UI16 wGroup = 0;
            //UI16 wAlarmID = 0;
            UI16 warn_num = 0;

            if((dwWarn[i] & 0xFF0FFFFF) == 0x0F000101) //伺服警报
            {
                wGroup = (dwWarn[i] & 0x00F00000) >> 20;
                //wAlarmID = VarAdrToInt(d_Pump1_STATE_SERVO_ERR + (wGroup-1)*0x100000);
            }

            //查找警报记录 获取警报信息
            warn_num = FindWarnId(dwWarn[i]);
            if(warn_num != 0xFF)
            {
                const char *ptrans;
                G10_WARNHELPRECD item = m_g10_warnhelprecd[warn_num];

                snprintf(strid,sizeof(strid)-1,"#%05X",ALARM_NUMBER(item.nId - 0x101));
                LabelSetText(lblalarmnum[warnpage], strid);

                if(pmap_alarm!=NULL)
                {
                    ptrans = GetMapTran(pmap_alarm,item.nId);

                    if(ptrans != NULL)
                    {
                        //通用是冷态启动防止时间 不是预温计时
                        if((VarAdrToInt(SYS_MACH_CUSTOMID) & 0xFF00) == 0x8B00)
                        {
                            if(item.nId == 0x0E100101)
                            {
                                ptrans = GetTextTran(TEXT_ALARM_E100101);
                            }
                            else if(item.nId == 0x0E200101)
                            {
                                ptrans = GetTextTran(TEXT_ALARM_E200101);
                            }
                        }

                        if((item.nId & 0xFF0FFFFF) == 0x0F000101) //伺服警报
                        {
                            snprintf(strid,sizeof(strid)-1,"%s(#%d%s Er.%03X)",ptrans,wGroup,GetTextTran(TEXT_DRIVER),item.subId & 0x0FFF);
                        }
                        else if(((item.nId & 0xFF0FFFFF)==0x0E00010E) || ((item.nId & 0xFF0FFFFF)==0x2F00010A)) //温度部件警报+模温部件警报
                        {
                            char buff[255];
                            char num[10];
                            UI16 wTempRec = 0;
                            int k;

                            strcpy(strid,ptrans);

                            //热电偶断线
                            if(item.res1[0] != 0)
                            {
                                strcat(strid," ");
                                snprintf(buff,sizeof(buff)-1,"%s",GetTextTran(TEXT_HEATOFF));
                                wTempRec = item.res1[0];
                                for(k = 0;k < 16;k++)
                                {
                                    if(wTempRec & (1 << k))
                                    {
                                        snprintf(num,sizeof(num)-1,"#%d ",k+1);
                                        strcat(buff,num);
                                    }
                                }
                                strcat(strid,buff);
                            }

                            //热电偶接反
                            if(item.res1[1] != 0)
                            {
                                strcat(strid," ");
                                snprintf(buff,sizeof(buff)-1,"%s",GetTextTran(TEXT_HEATREVERSE));
                                wTempRec = item.res1[1];
                                for(k = 0;k < 16;k++)
                                {
                                    if(wTempRec & (1 << k))
                                    {
                                        snprintf(num,sizeof(num)-1,"#%d ",k+1);
                                        strcat(buff,num);
                                    }
                                }
                                strcat(strid,buff);
                            }

                            //温度读取失败
                            if(item.res1[2] != 0)
                            {
                                strcat(strid," ");
                                snprintf(buff,sizeof(buff)-1,"%s",GetTextTran(TEXT_HEATREADFAIL));
                                wTempRec = item.res1[2];
                                for(k = 0;k < 16;k++)
                                {
                                    if(wTempRec & (1 << k))
                                    {
                                        snprintf(num,sizeof(num)-1,"#%d ",k+1);
                                        strcat(buff,num);
                                    }
                                }
                                strcat(strid,buff);
                            }
                        }
                        else
                        {
                            strcpy(strid,ptrans);
                        }
                    }
                    else
                    {
                        snprintf(strid,sizeof(strid)-1,"%x",item.nId);
                    }
                }
                memcpy(alarminfo[warnpage], strid, MAX_ALARM_CHAR_NUM);
                LabelSetText(lblalarm[warnpage], LabelAlarm_elidedText(strid));

                TimeToStr(strid,"yyyy-MM-dd hh:mm:ss",item.datetime);
                LabelSetText(lblalarmtm[warnpage], strid);
            }
            else//警报未找到 非正常情况 时间采用进入画面的时间
            {
                snprintf(strid,sizeof(strid)-1,"#%05X",ALARM_NUMBER(dwWarn[i] - 0x101));
                LabelSetText(lblalarmnum[warnpage], strid);

                snprintf(strid,sizeof(strid)-1,"%x",dwWarn[i]);
                if(pmap_alarm!=NULL)
                {
                    if(GetMapTran(pmap_alarm,dwWarn[i]) != NULL)
                    {
                        snprintf(strid,sizeof(strid)-1,"%s",GetMapTran(pmap_alarm,dwWarn[i]));
                    }
                }
                memcpy(alarminfo[warnpage], strid, MAX_ALARM_CHAR_NUM);
                LabelSetText(lblalarm[warnpage], LabelAlarm_elidedText(strid));

                TimeToStr(strid,"yyyy-MM-dd hh:mm:ss",default_time);
                LabelSetText(lblalarmtm[warnpage], strid);
            }

            warnpage++;
        }
    }

    if(warnpage == 0)
    {
        WGTSetVisible(page_help_pg.TB,FALSE);
        WGTSetVisible(page_help_pg.lblinfo,TRUE);

        if(PageFrameCurFocusWgt() != page_help_pg.btClose)
        {
            PageWgtSetFocus(page_help_pg.btClose);
        }
    }
    else
    {
        WGTSetVisible(page_help_pg.TB,TRUE);
        WGTSetVisible(page_help_pg.lblinfo,FALSE);

        for(i = 0;i < warnpage;i++)
        {
            WGTSetVisible(subpage_alarm[i], TRUE);
            WGTSetVisible(button_alarm[i], TRUE);
        }
        for(i = warnpage;i < WARN_NUM_MAX;i++)
        {
            WGTSetVisible(subpage_alarm[i], FALSE);
            WGTSetVisible(button_alarm[i], FALSE);
        }
    }
}

//显示警报详情
static void ShowAlarmDetail()
{
    char filename[255];
    UI16 wGroup;
    UI16 wAlarmID = 0;

    //20210914 dyl touch
    if(dwPanelErr == 0x34100101)//按键板通讯警报
    {
        if(GetCurrentLanguage() == LANG_zh_CN)
        {
            BrowserWgtSetURL(page_help_pg.TB_Tb2_bsalarm,NO_HELP);
        }
        else
        {
            BrowserWgtSetURL(page_help_pg.TB_Tb2_bsalarm,NO_HELP_EN);
        }

        return;
    }

    if((detail_alarm & 0xFF0FFFFF) != 0x0F000101)//非伺服警报
    {
        if(GetCurrentLanguage() == LANG_zh_CN)
        {
            snprintf(filename,sizeof(filename)-1,"%s%X%s",WARN_HELP_URL,detail_alarm,FILE_SUFFIX);
        }
        else
        {
            snprintf(filename,sizeof(filename)-1,"%s%X%s%s",WARN_HELP_URL,detail_alarm,"_en",FILE_SUFFIX);
        }
    }
    else
    {
        wGroup = (detail_alarm & 0x00F00000) >> 20;
        wAlarmID = VarAdrToInt(d_Pump1_STATE_SERVO_ERR + (wGroup-1)*0x100000);
        if(wAlarmID)
        {
            if(GetCurrentLanguage() == LANG_zh_CN)
            {
                snprintf(filename,sizeof(filename)-1,"%s%X%s",SERVOWARN_HELP_URL,wAlarmID & 0x0FFF,FILE_SUFFIX);
            }
            else
            {
                snprintf(filename,sizeof(filename)-1,"%s%X%s",SERVOWARN_HELP_URL_EN,wAlarmID & 0x0FFF,FILE_SUFFIX);
            }
        }
    }

    if(FileExist(filename))
    {
       BrowserWgtSetURL(page_help_pg.TB_Tb2_bsalarm,filename);
    }
    else
    {
        if(GetCurrentLanguage() == LANG_zh_CN)
        {
            BrowserWgtSetURL(page_help_pg.TB_Tb2_bsalarm,NO_HELP);
        }
        else
        {
            BrowserWgtSetURL(page_help_pg.TB_Tb2_bsalarm,NO_HELP_EN);
        }
    }
}

//选择tab页
static void PageHelp_SelPage(UI16 page_num)
{
    if(page_num != 0 && page_num != 1)
    {
        return;
    }

    if(page_num)
    {
        TabPageSetSubVisble(page_help_pg.TB, 0, FALSE);
        TabPageSetSubVisble(page_help_pg.TB, 1, TRUE);

        WGTSetVisible(page_help_pg.btBack, TRUE);
        WGTSetVisible(page_help_pg.btDown, TRUE);
        WGTSetVisible(page_help_pg.btUp, TRUE);
        WI32(page_help_pg.btClose->x_pos, 391);

        ShowAlarmDetail();
        PageWgtSetFocus(page_help_pg.btBack);
    }
    else
    {
        TabPageSetSubVisble(page_help_pg.TB, 0, TRUE);
        TabPageSetSubVisble(page_help_pg.TB, 1, FALSE);

        WGTSetVisible(page_help_pg.btBack, FALSE);
        WGTSetVisible(page_help_pg.btDown, FALSE);
        WGTSetVisible(page_help_pg.btUp, FALSE);
        WI32(page_help_pg.btClose->x_pos, 250);
        PageWgtSetFocus(page_help_pg.TB_Tb1_button1);
    }

    TabPageSelPage(page_help_pg.TB, page_num);
}

//设置当前选中的警报背景色
static void ShowCurAlarmSel()
{
    UI32 offset,i;
    PWGT pwgt;
    PWGT subpage_alarm[WARN_NUM_MAX]={
        page_help_pg.TB_Tb1_subpage1,page_help_pg.TB_Tb1_subpage2,page_help_pg.TB_Tb1_subpage3,
        page_help_pg.TB_Tb1_subpage4,page_help_pg.TB_Tb1_subpage5,page_help_pg.TB_Tb1_subpage6,
        page_help_pg.TB_Tb1_subpage7,page_help_pg.TB_Tb1_subpage8,page_help_pg.TB_Tb1_subpage9,
        page_help_pg.TB_Tb1_subpage10};

    pwgt = PageFrameCurFocusWgt();
    offset = WGTGetGOffset(pwgt);

#ifndef MPANEL_TC//20210914 dyl touch
    if(offset > 0 && offset <= WARN_NUM_MAX)
    {
        WGTSetVisible(page_help_pg.TB_Tb1_subpage11, TRUE);
        LabelSetText(page_help_pg.TB_Tb1_subpage11_lblalarminfo, alarminfo[offset-1]);
    }
    else
    {
        WGTSetVisible(page_help_pg.TB_Tb1_subpage11, FALSE);
    }
#endif

    for(i = 0; i < WARN_NUM_MAX; i++)
    {
        if(i == (offset - 1))
        {
            OBJSetValueByAttrName((POBJ)subpage_alarm[i],"backColor",0x00d7e1,0,NULL,ATTR_UINT);
        }
        else
        {
            OBJSetValueByAttrName((POBJ)subpage_alarm[i],"backColor",0xffffffff,0,NULL,ATTR_UINT);
        }
    }
}

static void CtlConnect()
{
}
static int Page_HelpOnInit(Page_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Page_HelpOnShow(Page_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    default_time = (UI32)time(NULL);
    PageHelp_SelPage(0);

#ifdef MPANEL_TC//20210914 dyl touch
    WGTSetVisible(ppg->TB_Tb1_subpage11,FALSE);
#else
    WGTSetVisible(ppg->TB_Tb1_subpage11,TRUE);
#endif

    return 0;
}
static int Page_HelpOnHide(Page_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    WGTSetVisible(page_help_pg.TB,TRUE);//进入页面聚焦在警报上
    WGTSetVisible(page_help_pg.lblinfo,FALSE);

    return 0;
}
static int Page_HelpOnUpdate(Page_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(TabPageGetCurSel(page_help_pg.TB) == 0)
    {
        ShowAlarmTable();
        ShowCurAlarmSel();
    }

    return 0;
}

static int Page_HelpOnChange(Page_Help_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 dwWarn[WARN_NUM_MAX]={g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10};
    UI32 offset;

    offset = WGTGetGOffset(pwgt);

#ifdef MPANEL_TC//20210914 dyl touch
    if(offset)
    {
        PageWgtSetFocus(pwgt);
    }
#endif

    if(pwgt == ppg->btnclose || pwgt == ppg->btClose)//20210914 dyl touch
    {
        PanelCloseDialogByName(page_help_pg_name);
    }
    else if(pwgt == ppg->btBack)
    {
        detail_alarm = dwWarn[0];//20210914 dyl touch
        PageHelp_SelPage(0);
    }
    else if(pwgt == ppg->btDown)
    {
        BrowserWgtPageDown(page_help_pg.TB_Tb2_bsalarm);
    }
    else if(pwgt == ppg->btUp)
    {
        BrowserWgtPageUp(page_help_pg.TB_Tb2_bsalarm);
    }
    else
    {
        if(offset > 0 && offset <= WARN_NUM_MAX)
        {
            detail_alarm = dwWarn[offset-1];
            PageHelp_SelPage(1);
        }
    }

    return 0;
}
