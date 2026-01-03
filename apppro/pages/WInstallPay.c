#include "WInstallPay.h"
#include "typedef.h"
#include "pagefram.h"
#include "pay.h"
#include "information.h"
#include "stdio.h"
#include "verinfo.h"
#include "oprintf.h"
#include "time.h"

DEFINE_INSTALLPAY_MAP

CONNECT_INSTALLPAY_EVENT

//machine code
#define TMP_MACHINE2	TMPSTR(0)  //机器编码 临时字符串0

#define TMP_DATE_ISSUE	TMPSTR(1)  //签发日期
#define TMP_LAST_TOTAL	TMPSTR(2)  //剩余和总天数
#define TMP_PASSWORDX	TMPSTR(3)  //密码
#define TMP_OLD_SHOW    TMPVAL(8)  //老模式显示
#define TMP_NEW_SHOW    TMPVAL(9)  //新模式显示

#define TMP_CPERIODS	TMPVAL(1) //分期数         临时变量1
#define TMP_DOYEAR		TMPVAL(4) //出厂日期年     临时变量4
#define TMP_DOMONTH		TMPVAL(5) //出厂日期月     临时变量5
#define TMP_DODAY		TMPVAL(6) //出厂日期日     临时变量6

#define TMP_CNOWNUM	    TMPVAL(13) //当前期数       临时变量13
//password
#define TMP_PASSWORD    TMPVAL(31)

#define TMP_PANELVER1	    TMPSTR(4)
#define TMP_PANELVER2	    TMPSTR(5)
#define TMP_PANELDATE	    TMPVAL(14)

const char* VerDateToStrx(UI16 value)
{
    static char cbuf[10];

    if(value<10)
        snprintf(cbuf,sizeof(cbuf)-1,"%u%u%u%u",0,0,0,value);
    else if(value<100)
        snprintf(cbuf,sizeof(cbuf)-1,"%u%u%u",0,0,value);
    else if(value<1000)
        snprintf(cbuf,sizeof(cbuf)-1,"%u%u",0,value);
    else
        snprintf(cbuf,sizeof(cbuf)-1,"%u",value);

    return cbuf;
}
/************************************************************************/
/* 停机画面进入初始化                                                   */
/************************************************************************/
static void InstallPayPageIn()
{
    char time_str[32];
    char last_time[16];
    UI32 pnldate;
    VarAdrSetInt(TMP_PASSWORD,0);
    VarAdrSetStr(TMP_PASSWORDX, "");
    if(m_operation.cState == 1){
        VarAdrSetStr(TMP_MACHINE2, m_operation.machine2);
        VarAdrSetInt(TMP_DOYEAR, m_operation.doYear);
        VarAdrSetInt(TMP_DOMONTH, m_operation.doMonth);
        VarAdrSetInt(TMP_DODAY, m_operation.doDay);
        VarAdrSetInt(TMP_CPERIODS, m_operation.cPeriods);
        VarAdrSetInt(TMP_CNOWNUM, m_operation.cNowNum);

        VarAdrSetInt(TMP_OLD_SHOW, 1);
        VarAdrSetInt(TMP_NEW_SHOW, 0);
    }
    else if(m_operation.cState == 2){
        VarAdrSetStr(TMP_MACHINE2, onlyid.only_id);
        m_operation.ds9Year = m_operation.doYear;
        m_operation.ds9Month = m_operation.doMonth;
        m_operation.ds9Day = m_operation.doDay;
        snprintf(time_str, sizeof(time_str)-1, "%02d.%02d.%02d %02d:%02d:%02d", m_operation.ds9Year,m_operation.ds9Month,
                  m_operation.ds9Day,m_operation.ds10Year,m_operation.ds10Month,m_operation.ds10Day);
        snprintf(last_time, sizeof(last_time)-1, "%d / %d", m_operation.ds8Month, m_operation.ds8Day);

        VarAdrSetStr(TMP_DATE_ISSUE, time_str);
        VarAdrSetStr(TMP_LAST_TOTAL, last_time);

        VarAdrSetInt(TMP_OLD_SHOW, 0);
        VarAdrSetInt(TMP_NEW_SHOW, 1);

        VarAdrSetStr(TMP_PANELVER1,VerDateToStrx(m_verinfo.wPanelViewVer1));
        //panel ver2
        VarAdrSetStr(TMP_PANELVER2,VerDateToStrx(m_verinfo.wPanelViewVer2));

        //panel date
        pnldate = m_verinfo.wPanelDate1*10000+m_verinfo.wPanelDate2;
        VarAdrSetInt(TMP_PANELDATE,pnldate);
    }
}

static UI64 TransToWord(char* p)
{
    UI64 w = 0;
    while (*p<='9' && *p>='0')
    {
        w=w*10+(*p-'0');
        p++;
    }
    return w;
}
/************************************************************************/
/* 解密密码确认                                                         */
/************************************************************************/
static BOOL DecryptPassword()
{
    UI32 password;
    UI64 passwordx;
    struct tm* t;
    time_t time_now;
    char input_pass[13];
    int nret;
    char time_str[32];
    char last_time[16];

    nret = -1;

    if(m_operation.cState == 1){
        password = VarAdrToInt(TMP_PASSWORD);
        nret = Pay(password);
        if(nret >= 0)
        {
            //VarAdrSetInt(TMP_CSTATE,m_operation.cState);
            VarAdrSetInt(TMP_CNOWNUM,m_operation.cNowNum);
            PanelShowPage(PanelMainPage());
            return TRUE;
        }
        else
        {
            /*提示密码不正确*/
            ShowMsg(VW_MSG_INVALID_PASSWD);
        }
    }
    else if(m_operation.cState == 2){
        strncpy(input_pass, VarAdrToStr(TMP_PASSWORDX), sizeof(input_pass));
        if(strlen(input_pass) == 11){
            passwordx = TransToWord(input_pass);

            nret = Payx(passwordx);
            if(nret == -2){
                PanelShowPage(PanelMainPage());
            }
            else if(nret == -1){
                ShowMsg(VW_MSG_INVALID_PASSWD);
            }
            else if(nret >= 367){
                PanelShowPage(PanelMainPage());
            }
            else if(nret > 0){
                time(&time_now);
                t = localtime(&time_now);
                m_operation.ds9Year = t->tm_year+1900-2000;
                m_operation.ds9Month = t->tm_mon+1;
                m_operation.ds9Day = t->tm_mday;
                m_operation.doYear = m_operation.ds9Year;
                m_operation.doMonth = m_operation.ds9Month;
                m_operation.doDay = m_operation.ds9Day;
                m_operation.ds10Year =t->tm_hour;
                m_operation.ds10Month = t->tm_min;
                m_operation.ds10Day = t->tm_sec;
                snprintf(time_str, sizeof(time_str)-1, "%02d.%02d.%02d %02d:%02d:%02d\0", m_operation.ds9Year,m_operation.ds9Month,
                         m_operation.ds9Day,m_operation.ds10Year,m_operation.ds10Month,m_operation.ds10Day);
                m_operation.ds8Month = nret;
                m_operation.ds8Day = nret;
                snprintf(last_time, sizeof(last_time)-1, "%d / %d\0", m_operation.ds8Month, m_operation.ds8Day);

                VarAdrSetStr(TMP_DATE_ISSUE, time_str);
                VarAdrSetStr(TMP_LAST_TOTAL, last_time);
                SavePayData();
            }
        }
        else{
            ShowMsg(VW_MSG_INVALID_PASSWD);
        }
    }
	return FALSE;
}

static void CtlConnect()
{
}
static int InstallPayOnInit(InstallPay_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int InstallPayOnShow(InstallPay_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	InstallPayPageIn();
    if(m_operation.cState == 1)
    {
        PageWgtSetFocus(ppg->edit57);
    }
    else
    {
        PageWgtSetFocus(ppg->edit60);
    }

    //20241017 chj 海星厂家要求，增加提示信息
    if((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x8700)
    {
        if(m_operation.bTimeOut == 1)
        {
            WGTSetVisible(ppg->labelstopinfo,TRUE);
            WGTSetVisible(ppg->labelnearinfo,FALSE);
        }
        else if(m_operation.bTimeOut == 2)
        {
            WGTSetVisible(ppg->labelstopinfo,FALSE);
            WGTSetVisible(ppg->labelnearinfo,TRUE);
        }
        else
        {
            WGTSetVisible(ppg->labelstopinfo,FALSE);
            WGTSetVisible(ppg->labelnearinfo,FALSE);
        }
    }
    else
    {
        WGTSetVisible(ppg->labelstopinfo,FALSE);
        WGTSetVisible(ppg->labelnearinfo,FALSE);
    }


    return 0;
}
static int InstallPayOnHide(InstallPay_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int InstallPayOnUpdate(InstallPay_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int InstallPayOnChange(InstallPay_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	DecryptPassword();

    return 0;
}
