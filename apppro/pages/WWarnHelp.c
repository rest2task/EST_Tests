#include "WWarnHelp.h"
#include "typedef.h"
#include "pagefram.h"
#include "panel.h"
#include "warndatarec.h"
#include "stdio.h"
#include "privdef.h"
#include "tabpagewgt.h"
#include "drivealarmrecord.h"
#include "editwgt.h"
#include "alarmrecord.h"
#include "labelwgt.h"

DEFINE_WARNHELP_MAP

CONNECT_WARNHELP_EVENT

#define IN1     0x33150001
#define OUT1    0x33150007
#define AD1     0x331501f5
#define INJPOS  0x331501fb
#define SETRPM  0x3315000d
#define REALRPM 0x33150011
#define REALTORQUE  0x331501fe
#define ID1     0x33150015
#define ACT1    0x33150018
#define TM1     0x33150202

static WDITEM item;

//20200408.cyx
static void SetPushWndTxt()
{
    UI16 wValue1 = (item.MachCfg.wReserve[0])/100;
    UI16 wValue2 = (item.MachCfg.wReserve[0])%100;
    UI16 g_pushcode = (item.MachCfg.wPushCode);
    char cpushxbak[8] = "";

    if((item.MachCfg.wReserve[0]) > 100)
    {
        sprintf(cpushxbak, "%d%c%c", g_pushcode, wValue1, wValue2);
    }
    else
    {
        if((item.MachCfg.wReserve[0]) == 0)
        {
            sprintf(cpushxbak, "%d", g_pushcode);
        }
        else
        {
            sprintf(cpushxbak, "%d%c", g_pushcode, wValue2);
        }
    }

    VarAdrSetStr(TMPSTR(9), cpushxbak);
}

BOOL ShowAllRec()
{
    UI32 value;
    char cbuf[20];
    int i;
    DRIVEALARMITEM drivealarmitem;
    DB_G10_WARNRECORD alarmitem;

    value = GetCurWarnId();

    //20200810 标题显示警报
    alarmitem = G10_AlarmReadRecord(value);
    if(alarmitem.flag == MARK_USED)
    {
        if(GetMapTran(pmap_alarm,alarmitem.nId) != NULL)
        {
            LabelSetText(warnhelp_pg.lbltitle, GetMapTran(pmap_alarm,alarmitem.nId));
        }
        else
        {
            snprintf(cbuf, sizeof(cbuf)-1, "%X", alarmitem.nId);
            LabelSetText(warnhelp_pg.lbltitle, cbuf);
        }
    }

    //主机警报记录 //20200810
    item = HostAlarm_ReadRecord(value);
    if(HostAlarm_HaveRecord(value,alarmitem.nId,alarmitem.datetime))
    {
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 0, TRUE);
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 1, TRUE);
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 2, TRUE);
        TabPageSelPage(warnhelp_pg.tabpage1, 0);

        //time
        VarNameSetStr("p.PP.TMP.TMPSTR0", (const char*)item.time);
        //open cnt
        VarNameSetInt("p.PP.TMP.TMPVAL0",item.WarnData.wCn_MoldNum_H<<16 | item.WarnData.wCn_MoldNum_L);
        //io
        for(i = 0; i < 6; ++i)
        {
            VarAdrSetInt(IN1+i,item.WarnData.wPI_Input[i]);
            VarAdrSetInt(OUT1+i,item.WarnData.wPO_Output[i]);
        }
        //AD
        for(i = 0; i < ADMETER_CH_NUM; ++i)
        {
            if(item.wADPosi_Symbol[i] == 1)
            {
                snprintf(cbuf, sizeof(cbuf)-1, "%.1f", item.WarnData.wADPosi_Relative[i]/10.);
            }
            else
            {
                snprintf(cbuf, sizeof(cbuf)-1, "-%.1f", item.WarnData.wADPosi_Relative[i]/10.);
            }
            VarAdrSetStr(AD1+i,(const char*)cbuf);
            VarAdrSetInt(p_PP_TMP_TMPVAL13 + i, item.WarnData.wAd_Relative[i]);
        }

        //AD
        VarAdrSetInt(p_PP_TMP_TMPVAL21, item.WarnData.wPort_Adtemp);
        VarAdrSetInt(p_PP_TMP_TMPVAL22, item.WarnData.wAd_value);
        VarAdrSetInt(p_PP_TMP_TMPVAL23, item.WarnData.wTemp_lm37);
        for(i = 0; i < 10; ++i)
        {
            VarAdrSetInt(p_PP_TMP_TMPVAL24+i, item.WarnData.wTemp_real[i]);
        }

        //act
        VarAdrSetInt(p_PP_TMP_TMPVAL40, item.Act_Data[0].wAct_Id);
        VarAdrSetInt(p_PP_TMP_TMPVAL41, item.Act_Data[0].wStep);
        VarAdrSetInt(p_PP_TMP_TMPVAL42, item.Act_Data[0].wStart_ms);
        VarAdrSetInt(p_PP_TMP_TMPVAL43, item.Act_Data[1].wAct_Id);
        VarAdrSetInt(p_PP_TMP_TMPVAL44, item.Act_Data[1].wStep);
        VarAdrSetInt(p_PP_TMP_TMPVAL45, item.Act_Data[1].wStart_ms);
        VarAdrSetInt(p_PP_TMP_TMPVAL46, item.Act_Data[2].wAct_Id);
        VarAdrSetInt(p_PP_TMP_TMPVAL47, item.Act_Data[2].wStep);
        VarAdrSetInt(p_PP_TMP_TMPVAL48, item.Act_Data[2].wStart_ms);

        //daoutput
        for(i=0; i<8; ++i)
        {
            VarAdrSetInt(p_PP_TMP_TMPVAL49+i, item.WarnData.wDA_Value[i]);
        }

        //driver
        for(i=0; i<6; ++i)
        {
            VarAdrSetInt(p_PP_TMP_TMPVAL57+i, item.WarnData.wServoErr[i]);
        }
        for(i=0; i<6; ++i)
        {
            if (item.WarnData.wEle_SpdOut_RPM[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-item.WarnData.wEle_SpdOut_RPM[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", item.WarnData.wEle_SpdOut_RPM[i]);
            VarAdrSetStr(p_PP_TMP_TMPSTR20+i,(const char*)cbuf);
        }
        for(i=0; i<6; ++i)
        {
            if (item.WarnData.wEle_Real_Spd[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-item.WarnData.wEle_Real_Spd[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", item.WarnData.wEle_Real_Spd[i]);
            VarAdrSetStr(p_PP_TMP_TMPSTR26+i,(const char*)cbuf);
        }
        for(i=0; i<6; ++i)
        {
            if (item.WarnData.wEle_Real_Torque[i] >= 32768)
                snprintf(cbuf, sizeof(cbuf)-1, "-%d", 65536-item.WarnData.wEle_Real_Torque[i]);
            else
                snprintf(cbuf, sizeof(cbuf)-1, "%d", item.WarnData.wEle_Real_Torque[i]);
            VarAdrSetStr(p_PP_TMP_TMPSTR32+i,(const char*)cbuf);
        }
//--------record 1 end-----------

//--------record 2 start----------
        //panel
        VarAdrSetInt(p_PP_TMP_TMPVAL69, item.VersionInfo.wPanelType);
        VarAdrSetInt(p_PP_TMP_TMPVAL70, item.VersionInfo.wPanelFactory);
        if(item.VersionInfo.wPanelVer1 < 10){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u%u", 0,0,0,item.VersionInfo.wPanelVer1);
        }
        else if(item.VersionInfo.wPanelVer1 < 100){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u", 0,0,item.VersionInfo.wPanelVer1);
        }
        else if(item.VersionInfo.wPanelVer1 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0,item.VersionInfo.wPanelVer1);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", item.VersionInfo.wPanelVer1);
        }
        VarAdrSetStr(p_PP_TMP_TMPSTR51, cbuf);
        if(item.VersionInfo.wPanelVer2 < 10){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u%u", 0,0,0,item.VersionInfo.wPanelVer2);
        }
        else if(item.VersionInfo.wPanelVer2 < 100){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u", 0,0,item.VersionInfo.wPanelVer2);
        }
        else if(item.VersionInfo.wPanelVer2 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0,item.VersionInfo.wPanelVer2);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", item.VersionInfo.wPanelVer2);
        }
        VarAdrSetStr(p_PP_TMP_TMPSTR52, cbuf);
        VarAdrSetInt(p_PP_TMP_TMPVAL71, item.VersionInfo.wPanelDate1*10000+item.VersionInfo.wPanelDate2);

        //host
        VarAdrSetInt(p_PP_TMP_TMPVAL72, item.VersionInfo.wHostFactory);

        if(item.VersionInfo.wHostVer1 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0, item.VersionInfo.wHostVer1);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", item.VersionInfo.wHostVer1);
        }
        VarAdrSetStr(p_PP_TMP_TMPSTR54, cbuf);

        if(item.VersionInfo.wHostVer2 < 10){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u%u", 0,0,0, item.VersionInfo.wHostVer2);
        }
        else if(item.VersionInfo.wHostVer2 < 100){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u%u", 0,0,item.VersionInfo.wHostVer2);
        }
        else if(item.VersionInfo.wHostVer2 < 1000){
            snprintf(cbuf, sizeof(cbuf)-1, "%u%u", 0,item.VersionInfo.wHostVer2);
        }
        else{
            snprintf(cbuf, sizeof(cbuf)-1, "%u", item.VersionInfo.wHostVer2);
        }
        VarAdrSetStr(p_PP_TMP_TMPSTR55, cbuf);
        VarAdrSetInt(p_PP_TMP_TMPVAL73, item.VersionInfo.wHostDate1+(item.VersionInfo.wHostDate2&255)*65536 + 20000000);
        VarAdrSetInt(p_PP_TMP_TMPVAL74, item.VersionInfo.wBoot1);
        snprintf(cbuf, sizeof(cbuf)-1, "%u", (item.VersionInfo.wBoot1%100+2000)*10000+item.VersionInfo.wBoot2);
        VarAdrSetStr(p_PP_TMP_TMPSTR56, cbuf);
        snprintf(cbuf, sizeof(cbuf)-1, "%u", item.VersionInfo.wHardware&0x1FFF);
        VarAdrSetStr(p_PP_TMP_TMPSTR53, cbuf);

        VarAdrSetInt(p_PP_TMP_TMPVAL75, item.MachCfg.wver);
        VarAdrSetInt(p_PP_TMP_TMPVAL76, item.MachCfg.wCode0);
        VarAdrSetInt(p_PP_TMP_TMPVAL77, item.MachCfg.wCode1);
        VarAdrSetInt(p_PP_TMP_TMPVAL78, item.MachCfg.wCode2);
        VarAdrSetInt(p_PP_TMP_TMPVAL79, item.MachCfg.wCode3);
        VarAdrSetInt(p_PP_TMP_TMPVAL80, item.MachCfg.wCode4);
        VarAdrSetInt(p_PP_TMP_TMPVAL81, item.MachCfg.wCode5);
        VarAdrSetInt(p_PP_TMP_TMPVAL82, item.MachCfg.wFactory);
        VarAdrSetInt(p_PP_TMP_TMPVAL83, item.MachCfg.wCode7);
        VarAdrSetInt(p_PP_TMP_TMPVAL84, item.MachCfg.wHydraulic);
        VarAdrSetInt(p_PP_TMP_TMPVAL85, item.MachCfg.wVerInfo);
        VarAdrSetInt(p_PP_TMP_TMPVAL86, item.MachCfg.wTemp);
        VarAdrSetInt(p_PP_TMP_TMPVAL87, item.MachCfg.wMeter);
        VarAdrSetInt(p_PP_TMP_TMPVAL88, item.MachCfg.wCode11);
        VarAdrSetInt(p_PP_TMP_TMPVAL89, item.MachCfg.wCode12);
        VarAdrSetInt(p_PP_TMP_TMPVAL90, item.MachCfg.wSystem);
        VarAdrSetInt(p_PP_TMP_TMPVAL91, item.MachCfg.wSysPrs);
        VarAdrSetInt(p_PP_TMP_TMPVAL92, item.MachCfg.wCode16);
        VarAdrSetInt(p_PP_TMP_TMPVAL93, item.MachCfg.wRpmCnt);
//        VarAdrSetInt(p_PP_TMP_TMPVAL94, item.MachCfg.wPushCode);
        SetPushWndTxt();
        VarAdrSetInt(p_PP_TMP_TMPVAL95, item.MachCfg.wCode43);
        VarAdrSetInt(p_PP_TMP_TMPVAL96, item.MachCfg.wCode48);
        VarAdrSetInt(p_PP_TMP_TMPVAL97, item.MachCfg.wCode49);
        VarAdrSetInt(p_PP_TMP_TMPVAL98, item.MachCfg.wCode50);
        VarAdrSetInt(p_PP_TMP_TMPVAL99, item.MachCfg.wCode51);
        VarAdrSetInt(p_PP_TMP_TMPVAL200, item.MachCfg.wCode52);
        VarAdrSetInt(p_PP_TMP_TMPVAL201, item.MachCfg.wCode53);
        VarAdrSetInt(p_PP_TMP_TMPVAL202, item.MachCfg.wCode54);
        VarAdrSetInt(p_PP_TMP_TMPVAL203, item.MachCfg.wQVR);
    }
    else
    {
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 0, FALSE);
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 1, FALSE);
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 2, FALSE);
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 3, TRUE);//不存在主机警报则肯定存在驱动器警报
        TabPageSelPage(warnhelp_pg.tabpage1, 3);

//        for(i = 0; i < 100; ++i)
//        {
//            VarAdrSetInt(0x33150000+i, 0);
//        }
//        for(i = 0; i < 4; ++i)
//        {
//            VarAdrSetInt(0x331500c8+i, 0);
//        }
//        for(i = 0; i < 60; ++i)
//        {
//            VarAdrSetStr(0x331501f4+i, "0");
//        }
//        VarAdrSetStr(TMPSTR(94), "0");
    }

    //20200810
    drivealarmitem = DriveAlarm_ReadRecord(value);
    if(DriveAlarm_HaveRecord(value,alarmitem.subId))
    {
        char cbuf[10];
        snprintf(cbuf,sizeof(cbuf),"Er.%X",drivealarmitem.alarmcode & 0x0FFF);

        TabPageSetSubVisble(warnhelp_pg.tabpage1, 3, TRUE);

        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit14, drivealarmitem.alarmservoid, 0);
        EditSetText(warnhelp_pg.tabpage1_Tb4_edit15, cbuf);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit16, drivealarmitem.H0100, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit17, drivealarmitem.H0101, 0);
        if(drivealarmitem.H0B37 > 32768)
        {
            sprintf(cbuf,"%d", drivealarmitem.H0B37 - 65536);
        }
        else
        {
            sprintf(cbuf,"%d", drivealarmitem.H0B37);
        }
        EditSetText(warnhelp_pg.tabpage1_Tb4_edit18, cbuf);
        if(drivealarmitem.H0B38 > 32768)
        {
            sprintf(cbuf,"%.2f", (drivealarmitem.H0B38 - 65536)/100.0);
        }
        else
        {
            sprintf(cbuf,"%.2f", drivealarmitem.H0B38/100.0);
        }
        EditSetText(warnhelp_pg.tabpage1_Tb4_edit19, cbuf);
        if(drivealarmitem.H0B39 > 32768)
        {
            sprintf(cbuf,"%.2f", (drivealarmitem.H0B39 - 65536)/100.0);
        }
        else
        {
            sprintf(cbuf,"%.2f", drivealarmitem.H0B39/100.0);
        }
        EditSetText(warnhelp_pg.tabpage1_Tb4_edit20, cbuf);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit21, drivealarmitem.H0B40, 1);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit22, drivealarmitem.H0B41, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit23, drivealarmitem.H0B42, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit24, drivealarmitem.H0B46, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit25, drivealarmitem.H0B47, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit26, drivealarmitem.H0B48, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit27, drivealarmitem.H0B43, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit28, drivealarmitem.H0B44, 0);
        EditSetNum(warnhelp_pg.tabpage1_Tb4_edit29, drivealarmitem.H0B35, 1);
    }
    else
    {
        TabPageSetSubVisble(warnhelp_pg.tabpage1, 3, FALSE);
    }

    return TRUE;
}
static void CtlConnect()
{
}
static int WarnHelpOnInit(WarnHelp_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int WarnHelpOnShow(WarnHelp_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;
    for(i = 0; i < 100; ++i)
    {
        VarAdrSetInt(0x33150000+i, 0);
    }
    for(i = 0; i < 4; ++i)
    {
        VarAdrSetInt(0x331500c8+i, 0);
    }
    for(i = 0; i < 60; ++i)
    {
        VarAdrSetStr(0x331501f4+i, "0");
    }
    VarAdrSetStr(TMPSTR(9), "0");

    ShowAllRec();
    return 0;
}
static int WarnHelpOnHide(WarnHelp_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int WarnHelpOnUpdate(WarnHelp_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int WarnHelpOnChange(WarnHelp_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == ppg->btClose)
    {
        PanelCloseDialogByName(warnhelp_pg_name);
    }
    else if(pwgt == ppg->btNext)
    {
        TabPageSelNext(ppg->tabpage1, TRUE);
    }
    return 0;
}
