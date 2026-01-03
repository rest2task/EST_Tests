#include "udisk.h"
#include "oprintf.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "file.h"
#include "information.h"
#include "keydef.h"
#include "prot.h"
#include "pagename.h"
#include "pagename.h"
#include "database.h"
#include "touch.h" //20210914 dyl touch
#include "pushtab.h"//20210914 dyl touch
#include "verinfo.h"

#ifdef IMX6
#include "sys/wait.h"
#include "unistd.h"
#include "stdlib.h"
#include "fcntl.h"
#include "pthread.h"
#include <dirent.h>

//目前此功能只有主动更新才有，自动更新不做此处理 //20230518 chj 面板更新比较
char pstrdata[128] = "\0";
char pstrform[128] = "\0";
char pstrlua[128] = "\0";
char pstrHmiApp[128] = "\0";

BOOL Compare_filename()
{
    FILEHD filehd;
    char dmesg[100] = "\0";
    char scmd[128] = "\0";

    DIR* dir = opendir("/Compare");
    if (dir == NULL) {
        oprintf("opendir failed\n");
        return FALSE;
    }
    BOOL bdiff = TRUE;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if((strcmp(".", entry->d_name) != 0) && (strcmp("..", entry->d_name) != 0))
        {
            if ((entry->d_type == DT_DIR))//比较文件，用diff判断U盘内的和面板Compare内的文件是否一致
            {
                if((strcmp(entry->d_name,"data") == 0) && (pstrdata[0] != '\0'))
                {
                    snprintf(scmd,sizeof(scmd)-1, "diff -r -q  /Compare/%s  %s >> /Compare.txt",entry->d_name,pstrdata);
                }
                else if((strcmp(entry->d_name,"form") == 0) && (pstrform[0] != '\0'))
                {
                    snprintf(scmd,sizeof(scmd)-1, "diff -r -q  /Compare/%s  %s >> /Compare.txt",entry->d_name,pstrform);

                }
                else if((strcmp(entry->d_name,"lua") == 0) && (pstrlua[0] != '\0'))
                {
                    snprintf(scmd,sizeof(scmd)-1, "diff -r -q  /Compare/%s  %s >> /Compare.txt",entry->d_name,pstrlua);

                }
                else
                {
                    ;
                }
            }
            else if((entry->d_type == DT_REG))//比较文件，用diff判断U盘内的和面板Compare内的文件是否一致
            {
                 if((strcmp(entry->d_name,"HmiApp") == 0) && (pstrHmiApp[0] != '\0'))
                {
                    snprintf(scmd,sizeof(scmd)-1, "diff -r -q  /Compare/%s  %s >> /Compare.txt",entry->d_name,pstrHmiApp);
                }
                else
                {
                    ;
                }
            }
            else
            {
                ;
            }
            oprintf("diff--scmd--:%s\n",scmd);
            if(osystem(scmd) != 0)//判断命令是否都成功
            {
                bdiff = FALSE;
                break;
            }
        }
    }

    //判断Compare.txt文件，查看此文件是否为空
    filehd = FileOpen("/Compare.txt");
    if(FILEHD_IS_OK(filehd))
    {
        FileRead(filehd, dmesg, sizeof(dmesg), NULL);
    }

    BOOL bok = FALSE;
    if (dmesg[0] == '\0')
    {
        if(bdiff)
        {
           bok = TRUE;
           oprintf("diff mesg is empty\n");
        }
        else {

           bok = FALSE;
           oprintf("bdiff cmd is fail\n");
        }
    }
    else
    {
        bok = FALSE;
        oprintf("diff mesg is not empty\n");
    }

    FileClose(filehd);
    closedir(dir);

    return bok;
}


/**
* @brief     :逐条的执行命令
* @param     :cmd 执行的命令，sec 执行命令超时的时间，exit_code 退出码
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200731
*/
int system_deadline(const char* command, int deadline, int* exit_code,BOOL bCompare)//20230518 chj 面板更新比较
{
    static int pid = 0;
    static int timeout = 0;

    if(pid == 0)
    {
        timeout = deadline;
        pid = vfork();
    }

    if(pid < 0)
    {
        perror("vfork");
        goto error;
    }
    else if(pid == 0)
    {
        if(!FileExist("/Compare/"))
        {
            oprintf("mkdir Compare\n");
            MkDir("/Compare/");
        }
        if(FileExist(USB_PATH"NoCompare/"))//留个后门
        {
            oprintf("find  NoCompare\n");
            bCompare = FALSE;
        }

        char scmd[128] = "\0";
        char sname[128] = "\0";
        if(strstr(command, "data") != NULL
           && strstr(command, HMI_ROOT_PATH) != NULL
           && strstr(command, "/home/updatebak_hmi") == NULL
           && strstr(command, "cp") != NULL && bCompare
           &&(GetCharPos(command,HMI_ROOT_PATH) > GetCharPos(command,ROOT_PREFIX)))
        {
            if(!FileExist("/Compare/data"))
            {
                oprintf("mkdir Compare/data\n");
                MkDir("/Compare/data");
            }
            bneedCompare = TRUE;
            strncpy(scmd, command,sizeof(scmd)-1);
            ReplaceChar(scmd, HMI_ROOT_PATH, "/Compare/");
            strncpy(sname, command,sizeof(sname)-1);
            strtokchar(sname,pstrdata);
            oprintf("Need compare----:%s,%s\n",scmd,pstrdata);
            if(-1 == execl("/bin/sh", "sh", "-c", scmd, NULL))
            {
                exit(1);
            }
        }
        else if(strstr(command, "form") != NULL
             && strstr(command, HMI_ROOT_PATH) != NULL
             && strstr(command, "/home/updatebak_hmi") == NULL
             && strstr(command, "cp") != NULL && bCompare
             &&(GetCharPos(command,HMI_ROOT_PATH) > GetCharPos(command,ROOT_PREFIX)))
        {
            if(!FileExist("/Compare/form"))
            {
                oprintf("mkdir Compare/form\n");
                MkDir("/Compare/form");
            }
            bneedCompare = TRUE;
            strncpy(scmd, command,sizeof(scmd)-1);
            ReplaceChar(scmd, HMI_ROOT_PATH, "/Compare/");
            strncpy(sname, command,sizeof(sname)-1);
            strtokchar(sname,pstrform);
            oprintf("Need compare----:%s,%s\n",scmd,pstrform);
            if(-1 == execl("/bin/sh", "sh", "-c", scmd, NULL))
            {
                exit(1);
            }
        }
        else if(strstr(command, "lua") != NULL
             && strstr(command, HMI_ROOT_PATH) != NULL
             && strstr(command, "/home/updatebak_hmi") == NULL
             && strstr(command, "cp") != NULL && bCompare
             &&(GetCharPos(command,HMI_ROOT_PATH) > GetCharPos(command,ROOT_PREFIX)))
        {
            if(!FileExist("/Compare/lua"))
            {
                oprintf("mkdir Compare/lua\n");
                MkDir("/Compare/lua");
            }
            bneedCompare = TRUE;
            strncpy(scmd, command,sizeof(scmd)-1);
            ReplaceChar(scmd, HMI_ROOT_PATH, "/Compare/");

            strncpy(sname, command,sizeof(sname)-1);
            strtokchar(sname,pstrlua);
            oprintf("Need compare----:%s,%s\n",scmd,pstrlua);

            if(-1 == execl("/bin/sh", "sh", "-c", scmd, NULL))
            {
                exit(1);
            }
        }
        else if(strstr(command, "HmiApp") != NULL
             && strstr(command, "/opt/") != NULL
             && strstr(command, "/home/updatebak_hmi") == NULL
             &&(strstr(command, "HmiAppx") == NULL)
             && strstr(command, "cp") != NULL && bCompare
             &&(GetCharPos(command,"/opt/") > GetCharPos(command,ROOT_PREFIX)))
        {
            bneedCompare = TRUE;
            strncpy(scmd, command,sizeof(scmd)-1);
            ReplaceChar(scmd, "/opt/", "/Compare/");
            strncpy(sname, command,sizeof(sname)-1);
            strtokchar(sname,pstrHmiApp);
            oprintf("Need compare----:%s,%s\n",scmd,pstrHmiApp);
            if(-1 == execl("/bin/sh", "sh", "-c", scmd, NULL))
            {
                exit(1);
            }
        }
        else {
            if(-1 == execl("/bin/sh", "sh", "-c", command, NULL))
            {
                exit(1);
            }
        }

        // never enter
    }
    else
    {
        int wstatus;
        int ret = waitpid(pid, &wstatus, WNOHANG);
        if(ret == -1)
        {
            perror("waitpid");
            goto error;
        }
        else if(ret == 0)
        {
            if(timeout == 0)
            {
                printf("timeout kill %d\n", pid);
                kill(pid, SIGKILL);
            }
            --timeout;
        }
        else
        {
            if(WIFSIGNALED(wstatus))
            {
                printf("sub process terminated by signal!\n");
                goto error;
            }
            else if(WIFEXITED(wstatus))
            {
                if(exit_code)
                {
                    *exit_code = WEXITSTATUS(wstatus);
                }
                pid = 0;
                return 0;
            }
        }
        sleep(1);
    }
    return 1;

error:
    if(exit_code)
    {
        *exit_code = -1;
    }
    pid = 0;
    return 0;
}
#endif

/*************************触摸屏不灵U盘自动更新*********************************/

BOOL isExist_helpfx9999()
{
    if(FileExist(HELPFX9999_PATH))
        return TRUE;
    else
        return FALSE;
}

BOOL isExist_helpfxv7777()
{
    if(FileExist(HELPFXV7777_PATH))
        return TRUE;
    else
        return FALSE;
}

static BOOL isExist_SpecialUpdate()
{
    if(isExist_helpfx9999() || isExist_helpfxv7777())
        return TRUE;
    else
        return FALSE;
}

static UI16 g_wKeyRunCnt = 0;
static BOOL isok_runkey = FALSE;
static time_t start_t; //第一次按键按下的时间
/**
* @brief     :通讯断开，特殊文件存在，连续按三次运行键
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200804
*/
void AutoUdiskRunKeyCnt(UI32 wkey, int press)
{
    if(!isok_runkey){
    if(press && wkey == _KEY_PANEL_RUN && !MainProtComIsOK() && PanelTopDialog() == PanelFindPage(DIALOG_UDISKAUTOUPDATE))
    {
        if (isExist_helpfx9999())//存在特殊更新包
        {
            if(g_wKeyRunCnt == 0)
            {
                start_t = time(0);
                g_wKeyRunCnt++;
                return;
            }
            if(time(0) - start_t < 6)
            {
                g_wKeyRunCnt++;
                if(g_wKeyRunCnt >= 3)
                {
                    g_wKeyRunCnt = 3;
                    isok_runkey = TRUE;
                }
            }
            else
            {
                g_wKeyRunCnt = 0;
                isok_runkey = FALSE;
            }
        }
        else
        {
            g_wKeyRunCnt = 0;
            isok_runkey = FALSE;
        }
    }
    else if(wkey != _KEY_PANEL_RUN)
    {
            g_wKeyRunCnt = 0;
            isok_runkey = FALSE;
    }
    }
    return;
}

/**
* @brief     :6s连续三次运行是否准备完毕
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200804
*/
BOOL isOkRunKey()
{
    return isok_runkey;
}

UI16 RunKeyCnt()
{
    return g_wKeyRunCnt;
}


UI32 Countdown6s()
{
    int remain;
    if(g_wKeyRunCnt != 0){
        remain = time(0) - start_t;
        if(remain < 6){
            return 6-remain;
        }
        else {
            return 0;
        }
    }
    else{
        return 0;
    }
}

void AutoUdiskUpdate()
{
    //20210512 主循环减少check_usb函数的调用
    if(!MainProtComIsOK() && VarAdrToInt(USB_VISIBLE) && isExist_SpecialUpdate())
    {
        if(PanelTopDialog() != PanelFindPage(DIALOG_UDISKAUTOUPDATE))
            PanelShowDialogByName(DIALOG_UDISKAUTOUPDATE);
    }
    else
    {
        if(PanelTopDialog() == PanelFindPage(DIALOG_UDISKAUTOUPDATE))
            PanelCloseDialogByName(DIALOG_UDISKAUTOUPDATE);
    }
    if(g_wKeyRunCnt != 0 && !isok_runkey){
        if(time(0) - start_t >= 6){
            g_wKeyRunCnt = 0;
            isok_runkey = FALSE;
        }
    }
}

/*************************触摸屏不灵U盘自动更新-end*********************************/


//20210914 dyl touch
//20210918.dxl 触摸屏检测　>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOL DetectCheckFile_VIPCmd()//检测U盘中是否存在特殊更新命令checkfxv6666
{
    if(FileExist(CHECKFXV6666_PATH))
        return TRUE;
    else
        return FALSE;
}

BOOL CheckTouchScreen()//20220927 CHJ + //20221008 dyl
{
    if(CheckTouchScreenHeLiTai())
    {
        return TRUE;
    }
    else
    {
        if(CheckTouchScreenXiChu())
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

BOOL CheckTouchScreenXiChu()//西楚 //20221008 dyl
{
    BOOL btouch_ok = FALSE;
    FILEHD filehd;
    char dmesg[80000];
    const char* TouchTest = "SensorTest, Success";
    osystem("rm -f /FAIL*.csv");
//    osystem("/opt/Linux_Daemon_ITS_V3_0_7_2_arm SensorTest I2C V3 /dev/ilitek_ctrl 41 3 /home/fa/SensorTest.dat >/tmp/touchcheck.txt");//开机信息保存到openinfo.txt文件中
    //20221008 dyl
    osystem("rm -f /Log/FAIL*.csv");//20240923 chj 统一为osystem
    if(GetPanelType() == PANEL_VT15)
    {
        osystem("/opt/Linux_Daemon_ITS_V3_0_7_2_arm SensorTest I2C V3 /dev/ilitek_ctrl 41 3 /home/fa/ilitek_ts/437-iic.dat >/tmp/touchcheck.txt");//开机信息保存到openinfo.txt文件中
    }
    else
    {
        osystem("/opt/Linux_Daemon_ITS_V3_0_7_2_arm SensorTest I2C V3 /dev/ilitek_ctrl 41 3 /home/fa/ilitek_ts/532_I2C.dat >/tmp/touchcheck.txt");//开机信息保存到openinfo.txt文件中
    }
    filehd = FileOpen(TOUCHSREEN_FILE);
    FileRead(filehd, dmesg, sizeof(dmesg), NULL);
    if(strstr(dmesg, TouchTest))
    {
        btouch_ok = TRUE;
    }
    else
    {
        btouch_ok = FALSE;
    }
//    oprintf("text-size1=%d",FileGetSize(filehd,TOUCHSREEN_FILE));
    FileClose(filehd);

    return btouch_ok;
}

BOOL CheckTouchScreenHeLiTai()//合力泰  //20220927 CHJ + //20221008 dyl
{
    BOOL btouch_ok = FALSE;
    FILEHD filehd;
    const char* TouchTest="SensorTest, Success";
    char dmesg[80000];
    osystem("rm -f /FAIL*.csv");//20240923 chj 统一为osystem
    osystem("rm -f /Log/FAIL*.csv");
    osystem("/opt/Linux_Daemon_ITS_V3_0_9_1_arm SensorTest I2C V3 /dev/ilitek_ctrl 41 /home/fa/ilitek_ts/SensorTest.ini >/tmp/touchcheck.txt");//开机信息保存到openinfo.txt文件中
    filehd = FileOpen(TOUCHSREEN_FILE);
    FileRead(filehd, dmesg, sizeof(dmesg), NULL);
    if(strstr(dmesg, TouchTest))
    {
        btouch_ok = TRUE;
    }
    else
    {
        btouch_ok = FALSE;
    }
    FileClose(filehd);

    return btouch_ok;
}

extern void QtEventLoop();
void DecectTouchTest()
{
    static BOOL bCheckTouch = FALSE;
    if(IsDataRecvIng())//20220106 dyl 通讯正常但数据还未交互完成
    {
        return ;
    }
    if (VarAdrToInt(USB_VISIBLE))
    {
    }
    else
    {
        bCheckTouch = FALSE;
        SetValOfScreenCheck(FALSE);
    }
    //20210918.dxl
    if ((OperateModeIndex() == MODE_MANUAL) && !check_usb())//手动下 + U盘存在
    {
        if (!bCheckTouch)
        {
            if (DetectCheckFile_VIPCmd())//检测U盘中是否存在特殊更新命令checkfxv6666
            {
                ShowMsg(VW_MSG_AUTO_CHECKING);
                //force refresh
                PageFramReFresh(PanelCurPage(),TRUE);
                #ifdef USE_QT
                    QtEventLoop();  //20190329 hz
                #else
                    GUX_Exec();
                #endif
                if (IsScreenChecking())
                {
                    if(CheckTouchScreen())
                    {
                        ShowMsg(VW_MSG_AUTO_CHECK_OK);
                    }
                    else
                    {
                        ShowMsg(VW_MSG_AUTO_CHECK_ERR);
                    }
                    SetValOfScreenCheck(FALSE);
                    bCheckTouch = TRUE;
                }
            }
        }
    }
}
//20210918.dxl 触摸屏检测　<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//20230518 chj 面板更新比较
//比较完后，将Compare的内容拷贝到pages下
void CompareToPanel()
{
    osystem("cp -rf /Compare/data /pages/");
    osystem("cp -rf /Compare/form /pages/");
    osystem("cp -rf /Compare/lua /pages/");
    osystem("cp -rf /Compare/HmiApp /opt/");
    SysSync();
    oprintf("CompareToPanel-----------\n");
}

//在cp命令或者校验失败的情况下，将updatebak_hmi的内容拷贝到pages下
void BakToPanel()
{
#ifdef IMX6
    osystem("cp -r /home/updatebak_hmi/pages/* /pages/");
    osystem("cp -f /home/updatebak_hmi/HmiApp /opt/HmiApp");
    SysSync();
    oprintf("BakToPanel------\n");
#endif
}

void ReplaceChar(char *str, const char *oldStr, const char *newStr)
{
    if(str != NULL && oldStr != NULL && newStr != NULL)
    {
        char *pos, temp[1000];
        int index = 0;
        int oldStrLen = strlen(oldStr);
        int newStrLen = strlen(newStr);

        while ((pos = strstr(str, oldStr)) != NULL) {
            strcpy(temp, str);
            index = pos - str;
            str[index] = '\0';
            strcat(str, newStr);
            strcat(str, temp + index + oldStrLen);
        }
    }
}

void delchar(char *str, char ch)
{
    if(str != NULL)
    {
        int i = 0, j = 0;
        while (str[i]) {
            if (str[i] != ch) {
                str[j++] = str[i];
            }
            i++;
        }
        str[j] = '\0';
    }
}

void strtokchar( char *oldStr, char *newStr)
{
    if(oldStr != NULL && newStr != NULL)
    {
        char *token;
        char newname[128] = "\0";
        char oldname[128] = "\0";
        int oldStrLen = strlen(oldStr);
        int newStrLen = strlen(newStr);
        if((oldStrLen >= 128) || (newStrLen >= 128))
        {
            return;
        }
        strcpy(oldname, oldStr);
        const char seps[2] = " ";
        token = strtok(oldname,seps);
        while(token !=NULL)
        {
             token=strtok(NULL,seps);
             if(token != NULL)
             {
                 if(strstr(token,USB_PATH) != NULL )
                 {
                    strcpy(newname, token);
                    delchar(newname,'*');
                    strcpy(newStr, newname);
                    break;
                 }
             }
             else {
                 break;
             }
        }
    }
}

int GetCharPos(const char *str, char *seps)
{
    if(str != NULL && seps != NULL)
    {
        char *ptr = strstr(str, seps);

        if (ptr != NULL)
        {
            int position = ptr - str;
            //oprintf("The character is found at position %d\n", position);
            return position;
        }
    }

    return -1;
}

void ClearCompareFile()
{
    osystem("rm -rf /Compare/*");
    osystem("rm -rf /Compare.txt");
    SysSync();
    oprintf("rm -rf /Compare\n");
    bneedCompare = FALSE;//20230518 chj 面板更新比较
}
