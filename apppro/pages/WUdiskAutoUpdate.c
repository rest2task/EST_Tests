#include "WUdiskAutoUpdate.h"
#include "typedef.h"
#include "pagefram.h"
#include "udisk.h"
#include "labelwgt.h"
#include "privdef.h"
#include "vardb.h"
#include "checkboxwgt.h"
#include "editwgt.h"
#include "file.h"
#include "progbarwgt.h"
#include "stdio.h"
#include "information.h"
#include "questionDlg.h"
#include "common.h"
#include "oprintf.h"
#include "taskmoni.h"
#include "time.h"

DEFINE_UDISKAUTOUPDATE_MAP

CONNECT_UDISKAUTOUPDATE_EVENT

extern void QtEventLoop(); //20190329 hz
static int FileRowCount(const char* value, int size)
{
    int row_count = 0;

    if(!value) return 0;

    while(*value && size--)
    {
        if(*value == '\n') row_count++;
        value++;
    }

    return row_count;
}

#ifdef IMX6
static char* pstr = NULL;
static int cur_size = 0;
#define  READ_BUF 2000
#ifdef A40I //20230518 chj A40i板子更新失败，恢复文件
static char updatebak[] = "cp -r /pages /home/updatebak_hmi/\ncp -f /opt/HmiApp /home/updatebak_hmi/\n";
#else
static char updatebak[] = "cp -rf /pages/data /home/updatebak_hmi/pages/\n"
                          "cp -rf /pages/form /home/updatebak_hmi/pages/\n"
                          "cp -rf /pages/lua /home/updatebak_hmi/pages/\n"
                          "cp -rf /opt/HmiApp /home/updatebak_hmi/\n";

static char updatebak_pages_file[] = "/home/updatebak_hmi/pages";
#endif
static char updatebak_file[] = "/home/updatebak_hmi/";

static int SetUdiskCmdList(const char* filepath)//20220316 dyl
{
    FILEHD hfile;
    int row, i;
    char cmd[255];
    int readcnt;
    int exit_code;
    char* pstr = NULL;
    char* temp_str = NULL;

    #ifdef A40I
        MkDir(updatebak_file);
    #else
        MkDir(updatebak_file);
        MkDir(updatebak_pages_file);
    #endif

    hfile = FileOpen(filepath);
    if(FILEHD_IS_OK(hfile))
    {
        int size;
        size = FileGetSize(hfile, filepath);
        size += sizeof(updatebak);

        if(pstr == NULL)
        {
            pstr = OBJMemMalloc(READ_BUF);
            cur_size = READ_BUF;
        }

        if(size > cur_size)
        {
            OBJMemFree(pstr);
            pstr = OBJMemMalloc(size);
            cur_size = size;
        }

        strncpy(pstr, updatebak, sizeof(updatebak));
        if(pstr != NULL)
        {
            FileRead(hfile, pstr+sizeof(updatebak)-1, size, &readcnt);
            FileClose(hfile);
        }
    }
    else
    {
        FileClose(hfile);
    }


    row = FileRowCount(pstr,readcnt+sizeof(updatebak));
    if(row<=0)
    {
        ShowMsg(VW_MSG_UDISKNOFILE);
        return FALSE;
    }

    ProgbarSetMaxValue(udiskautoupdate_pg.panelbar1, row);
    ProgbarSetCurValue(udiskautoupdate_pg.panelbar1, 0);

    for(i=0;i<row;i++)
    {
        ProgbarSetCurValue(udiskautoupdate_pg.panelbar1,i+1);
        if(*pstr =='\n') //过滤空行
        {
            pstr=pstr+1;
            continue;
        }
        sscanf(pstr, "%[^\n]", cmd);
        pstr+=strlen(cmd)+1; //1:回车
        oprintf("cmd=%s\r\n",cmd);

        if((cmd[0]=='#')) //过滤注销命令
        {
            continue;
        }
        else if((cmd[0]==' '))
        {
            continue;
        }
        while(system_deadline(cmd, 35, &exit_code,FALSE))//20230518 chj 面板更新比较
        {
            ;
        }
        if(exit_code == -1)
        {
            if(temp_str){
                OBJMemFree(temp_str);
            }
            BakToPanel();
            ShowMsg(VW_MSG_UDISKABNORMAL);
            return FALSE;
        }
        else if(exit_code == 1 && strstr(cmd, "cp"))
        {
            if(temp_str){
                OBJMemFree(temp_str);
            }
            BakToPanel();
            ShowMsg(VW_MSG_UDISKNOFILE);
            return FALSE;
        }
        PageFramReFresh(PanelTopDialog(),TRUE);
    }
    if(temp_str){
        OBJMemFree(temp_str);
    }
    SysSync();
    return TRUE;
}
#endif

static BOOL UdiskPanelUpdate(UdiskAutoUpdate_PG* ppg)
{
    BOOL cmd_ok;
    cmd_ok = FALSE;

    PageFramReFresh(PanelTopDialog(),TRUE);
    QtEventLoop();

#ifdef IMX6
    //20190402 hz
    if(isExist_helpfxv7777()){
        cmd_ok = SetUdiskCmdList(HELPFXV7777_PATH);
    }
    else if(isExist_helpfx9999()){
        cmd_ok = SetUdiskCmdList(HELPFX9999_PATH);
    }
#endif //end IMX6

    if(cmd_ok == TRUE)
    {
        FileDelete(HELPFX9999_PATH);
        FileDelete(HELPFXV7777_PATH);
        SysSync();
        PanelMainRestart();
    }
    return TRUE;
}
static void CtlConnect()
{
}
static int UdiskAutoUpdateOnInit(UdiskAutoUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static time_t start_15s;
static int update_tick = 0;
static int UdiskAutoUpdateOnShow(UdiskAutoUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    update_tick = 0;
    if(isExist_helpfxv7777()){
        VarAdrSetInt(p_PP_MACHSET_INFORMATION_INDEX, VW_MSG_AUTOUPDATE);
        WGTSetVisible(ppg->runkey, FALSE);
        WGTSetVisible(ppg->cb1, FALSE);
        WGTSetVisible(ppg->cb2, FALSE);
        WGTSetVisible(ppg->cb3, FALSE);
        WGTSetVisible(ppg->panelbar1, FALSE);
        WGTSetVisible(ppg->label51, FALSE);
        start_15s = time(0);
    }
    else if(isExist_helpfx9999()){
        VarAdrSetInt(p_PP_MACHSET_INFORMATION_INDEX, VW_MSG_PRESSRUNKEY);
        WGTSetVisible(ppg->runkey, TRUE);
        WGTSetVisible(ppg->cb1, TRUE);
        WGTSetVisible(ppg->cb2, TRUE);
        WGTSetVisible(ppg->cb3, TRUE);
        CheckboxSetCheck(ppg->cb1, FALSE);
        CheckboxSetCheck(ppg->cb2, FALSE);
        CheckboxSetCheck(ppg->cb3, FALSE);
        WGTSetVisible(ppg->panelbar1, FALSE);
        WGTSetVisible(ppg->label51, FALSE);
    }
    return 0;
}
static int UdiskAutoUpdateOnHide(UdiskAutoUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int UdiskAutoUpdateOnUpdate(UdiskAutoUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int runketcnt;
    int remain;
    if(isExist_helpfxv7777()){
        remain = time(0) - start_15s;
        if(remain <= 15){
            VarAdrSetInt(p_PP_TMP_TMPVAL473, 15 - remain);
        }
        else{
            if(update_tick == 0){
                update_tick++;
                WGTSetVisible(ppg->panelbar1, TRUE);
                WGTSetVisible(ppg->label51, TRUE);
            }
            else if(update_tick == 1){
                update_tick++;
                UdiskPanelUpdate(ppg);
            }
        }
    }
    else if(isExist_helpfx9999()){
        if(isOkRunKey()){
            if(update_tick == 0){
                update_tick++;
                VarAdrSetInt(p_PP_TMP_TMPVAL473, 0);
                CheckboxSetCheck(ppg->cb1, TRUE);
                CheckboxSetCheck(ppg->cb2, TRUE);
                CheckboxSetCheck(ppg->cb3, TRUE);
                WGTSetVisible(ppg->panelbar1, TRUE);
                WGTSetVisible(ppg->label51, TRUE);
            }
            else if(update_tick == 1){
                update_tick++;
                UdiskPanelUpdate(ppg);
            }
        }
        else{
            runketcnt = RunKeyCnt();
            switch (runketcnt) {
            case 1:
                CheckboxSetCheck(ppg->cb1, TRUE);
                CheckboxSetCheck(ppg->cb2, FALSE);
                CheckboxSetCheck(ppg->cb3, FALSE);
                break;
            case 2:
                CheckboxSetCheck(ppg->cb1, TRUE);
                CheckboxSetCheck(ppg->cb2, TRUE);
                CheckboxSetCheck(ppg->cb3, FALSE);
                break;
            default:
                break;
            }
            VarAdrSetInt(p_PP_TMP_TMPVAL473, Countdown6s());
            if(Countdown6s() == 0){
                CheckboxSetCheck(ppg->cb1, FALSE);
                CheckboxSetCheck(ppg->cb2, FALSE);
                CheckboxSetCheck(ppg->cb3, FALSE);
            }
        }
    }
    return 0;
}
static int UdiskAutoUpdateOnChange(UdiskAutoUpdate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
