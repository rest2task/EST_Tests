#include "WServodataOperate.h"
#include "typedef.h"
#include "pagefram.h"
#include "dropwgt.h"
#include "database.h"
#include "driveoperate.h"
#include "oprintf.h"
#include "file.h"
#include "privdef.h"
#include "dirent.h"
#include "information.h"
#include "stdio.h"
#include "editwgt.h"

DEFINE_SERVODATAOPERATE_MAP

CONNECT_SERVODATAOPERATE_EVENT

#define DROPIMPORT_VALUE    TMPVAL(2)
#define DROPOUTPORT_VALUE   TMPVAL(4)
#define WRITE_DRIVEID	TMPVAL(11)
#define READ_DRIVEID	TMPVAL(21)
#define READ_REALDATA	TMPVAL(35)
static UI16 wCurType = NOOPERATEPAR;//当前操作为读数据还是写数据
static UI16 wCurPack;//当前请求的包号
static BOOL bRefreshUdisk;//当前U盘是否刷新
PRO_SDO_RX_LOG servo_read;

PRO_SDO_RX_LOG servo_write;

static void DriveCallback(BOOL result, PRO_SDO_RX_LOG* logs, int cnt, void* param)
{
    if (!result)
    {
        oprintf("result :0\n");
    }
    else
    {
         VarAdrSetInt(UPDATE_DRIVEPARA,1);
    }
}

static void Read_Callback(BOOL result, PRO_SDO_RX_LOG* logs, int cnt, void* param)
{
    if (!result)
    {
       VarAdrSetInt(READ_REALDATA, 0);
    }
    else
    {
       VarAdrSetInt(READ_REALDATA, logs->servo_val);
    }
}

static void Write_Callback(BOOL result, PRO_SDO_RX_LOG* logs, int cnt, void* param)
{
    if (!result)
    {
//       ShowMsg(VW_MSG_STOPSELFLEARN);
    }
    else
    {
//       ShowMsg(VW_MSG_MANUAL);
    }
}

static void UpdateUdiskList()
{
    DropSetItems(servodataoperate_pg.dropimport,"");
    if(FileExist(USB_DRIVEIMPORT_PATH))
    {
        DIR *dirp;
        struct dirent *dp;
        char buff[255];
        int len;
        dirp = opendir(USB_DRIVEIMPORT_PATH); //打开目录指针
        while ((dp = readdir(dirp)) != NULL) { //通过目录指针读目录
            //oprintf("in:%s\n", dp->d_name);
            sprintf(buff,"%s",dp->d_name);
            len = strlen(buff);
            if((buff[len-4] == '.') && (buff[len-3] == 'c') && (buff[len-2] == 's') && (buff[len-1] == 'v'))//过滤文件
            {
                buff[len-4] = '\0';//去掉后缀名
                DropAddItem(servodataoperate_pg.dropimport,buff);
            }
        }
        (void) closedir(dirp); //关闭目录
        bRefreshUdisk = true;
    }

    DropSetItems(servodataoperate_pg.dropoutport,"");
    if(FileExist(USB_DRIVEOUTPORT_PATH))
    {
        DIR *dirp;
        struct dirent *dp;
        char buff[255];
        int len;
        dirp = opendir(USB_DRIVEOUTPORT_PATH); //打开目录指针
        while ((dp = readdir(dirp)) != NULL) { //通过目录指针读目录
            //oprintf("out:%s\n", dp->d_name);
            sprintf(buff,"%s",dp->d_name);
            len = strlen(buff);
            if((buff[len-4] == '.') && (buff[len-3] == 'c') && (buff[len-2] == 's') && (buff[len-1] == 'v'))//过滤文件
            {
                buff[len-4] = '\0';//去掉后缀名
                if(!((buff[len-7] == '_') && (buff[len-6] == 'o') && (buff[len-5] == 'k')))
                {
                    DropAddItem(servodataoperate_pg.dropoutport,buff);
                }
            }
        }
        (void) closedir(dirp); //关闭目录
        bRefreshUdisk = true;
    }
}

static void CtlConnect()
{
}
static int ServodataOperateOnInit(ServodataOperate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ServodataOperateOnShow(ServodataOperate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int i;
    wCurType = NOOPERATEPAR;//当前操作为读数据还是写数据
    wCurPack = 0;//当前请求的包号
    VarAdrSetInt(UPDATE_DRIVEPARA,0);
    WGTSetVisible(ppg->stimport,FALSE);
    WGTSetVisible(ppg->stoutport,FALSE);
    bRefreshUdisk = FALSE;
    VarAdrSetInt(DROPIMPORT_VALUE,0);
    VarAdrSetInt(DROPOUTPORT_VALUE,0);

    VarAdrSetInt(CUR_PACKTYPE,PACKSERVOOPERATE);
    VarAdrSetInt(CUR_OPERATETYPE,OPTNULL);

    VarAdrSetInt(WRITE_DRIVEID,1);
    VarAdrSetInt(WRITE_DRIVEID+1,0);
    VarAdrSetInt(WRITE_DRIVEID+2,16);
    VarAdrSetInt(WRITE_DRIVEID+3,0);
    VarAdrSetInt(READ_DRIVEID,1);
    VarAdrSetInt(READ_DRIVEID+1,0);
    VarAdrSetInt(READ_DRIVEID+2,16);
    WGTSetEnable(ppg->btimportpanel, TRUE);
    WGTSetEnable(ppg->btoutportpanel, TRUE);
    VarAdrSetInt(READ_REALDATA, 0);

    return 0;
}
static int ServodataOperateOnHide(ServodataOperate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    VarAdrSetInt(CUR_PACKTYPE,PACKSERVOOPERATE);
    VarAdrSetInt(CUR_OPERATETYPE,OPTNULL);
    return 0;
}
static UI32  time_1000ms = 0;
static int ServodataOperateOnUpdate(ServodataOperate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    static BOOL last_usb_exist = FALSE;
    BOOL usb_exist = !check_usb();
    if(usb_exist)
    {
        if(!bRefreshUdisk)//当前U盘是否刷新
        {
            if(GetTick() - time_1000ms >=1000)
            {
                time_1000ms = GetTick();
                UpdateUdiskList();

            }
        }

    }
    else
    {
        bRefreshUdisk = false;
        if(last_usb_exist) //拔出
        {
            DropSetItems(ppg->dropimport,"");
            DropSetItems(ppg->dropoutport,"");
        }
    }
    last_usb_exist = usb_exist;

    if((wCurType == READDRIVEPAR) && VarAdrToInt(UPDATE_DRIVEPARA))//发送的读数据当前包成功收到回复
    {
        VarAdrSetInt(UPDATE_DRIVEPARA,0);
        if(VarAdrToInt(CUR_OPERATETYPE) == UDISKOPT)
        {
            char pbuff[64],sFileName[64];
            DropGetCurItemText(ppg->dropoutport,pbuff,64);
            sprintf(sFileName,"%s%s_ok.csv",SAVE_PATH,pbuff);

            if(SaveDriveDataToExcel(sFileName))
            {
                char scmd[128];
                sprintf(scmd, "cp -f %s %s", sFileName,USB_DRIVEOUTPORT_PATH);
                if (!osystem(scmd))
                {
                    ShowMsg(VW_MSG_UDISKSAVESUCCESSX);
                    sprintf(scmd,"rm %s",sFileName);
                    osystem(scmd);
                }
                else
                {
                    ShowMsg(VW_MSG_UDISKSAVEFAIL);
                }
            }
            else
            {
                ShowMsg(VW_MSG_UDISKSAVEFAIL);
            }
            WGTSetVisible(ppg->stoutport,FALSE);
        }
        wCurType = NOOPERATEPAR;
        VarAdrSetInt(CUR_OPERATETYPE,OPTNULL);
    }
    else if((wCurType == WRITEDRIVEPAR) && VarAdrToInt(UPDATE_DRIVEPARA))//发送的写数据当前包成功收到回复
    {
            VarAdrSetInt(UPDATE_DRIVEPARA,0);
            wCurType = NOOPERATEPAR;
            //本地导入导出功能
            if(VarAdrToInt(CUR_OPERATETYPE) == UDISKOPT)
            {
                ShowMsg(VW_MSG_UDISKREADSUCCESS);
                WGTSetVisible(ppg->stimport,FALSE);
            }
            VarAdrSetInt(CUR_OPERATETYPE,OPTNULL);
    }
    return 0;
}

static int ServodataOperateOnChange(ServodataOperate_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    char  pbuff[64],sFileName[100];
    PRO_SDO_RX_LOG* ptableDrive;
    UI16 old_state;
    if(pwgt == ppg->btimport)
    {
        if(VarAdrToUI16(OPMODE_INDEX)!=MODE_MANUAL)
        {
            ShowMsg(VW_MSG_MANUAL);
            return FALSE;
        }

        //20191106 YX 马达开启时不能导入参数,增加提示信息
        if (VarAdrToInt(MOTOR_STATE_STATE) != 0)
        {
            ShowMsg(VW_MSG_PLSCLSMOTOR);
            return FALSE;
        }

        if(check_usb())
        {
            ShowMsg(VW_MSG_UDISKNOTEXIST);
            return FALSE;
        }
        if(VarAdrToInt(CUR_OPERATETYPE) != OPTNULL)//当前导入导出操作未完成不可操作
        {
            ShowMsg(VM_MSG_CUROPTNOTFINISH);
            return FALSE;
        }

        DropGetCurItemText(ppg->dropimport,pbuff,64);
        sprintf(sFileName,"%s%s",USB_DRIVEIMPORT_PATH,pbuff);
        WGTSetVisible(ppg->stimport,TRUE);
        wCurType = WRITEDRIVEPAR;
        old_state = LoadDriveData(sFileName, wCurType);
        if(old_state)
        {
            if(old_state == 2)
            {
                WGTSetVisible(ppg->stimport,FALSE);
                ShowMsg(VM_MSG_EMPTY_DATA);
                return FALSE;
            }
            ptableDrive = FindDriveTable(wCurType);//查找需要发送的对应的表
            if(ptableDrive == NULL){
                WGTSetVisible(ppg->stimport,FALSE);
                ShowMsg(VW_MSG_UDISKREADFAIL);
                return FALSE;
            }
            //向下位机发送读写驱动器数据的命令
            //增加本地导入导出功能
            VarAdrSetInt(CUR_OPERATETYPE,UDISKOPT);//0:无导入导出操作；1:U盘导入导出；2:本地面板导入导出
            ServoCtlWrite(ptableDrive,GetPackCount(),DriveCallback,NULL);
            return TRUE;
        }
        wCurType = NOOPERATEPAR;
        WGTSetVisible(ppg->stimport,FALSE);
        ShowMsg(VW_MSG_UDISKREADFAIL);
        return FALSE;
    }
    else if(pwgt == ppg->btoutport)
    {
        if(VarAdrToUI16(OPMODE_INDEX)!=MODE_MANUAL)
        {
            ShowMsg(VW_MSG_MANUAL);
            return FALSE;
        }

        //20191106 YX 马达开启时不能导入参数,增加提示信息
        if (VarAdrToInt(MOTOR_STATE_STATE) != 0)
        {
            ShowMsg(VW_MSG_PLSCLSMOTOR);
            return FALSE;
        }

        if(check_usb())
        {
            ShowMsg(VW_MSG_UDISKNOTEXIST);
            return FALSE;
        }
        if(VarAdrToInt(CUR_OPERATETYPE) != OPTNULL)//当前导入导出操作未完成不可操作
        {
            ShowMsg(VM_MSG_CUROPTNOTFINISH);
            return FALSE;
        }

        DropGetCurItemText(ppg->dropoutport,pbuff,64);
        sprintf(sFileName,"%s%s",USB_DRIVEOUTPORT_PATH,pbuff);
        WGTSetVisible(ppg->stoutport,TRUE);
        wCurType = READDRIVEPAR;
        if(LoadDriveData(sFileName, wCurType))
        {
            wCurPack = 0;
            ptableDrive = FindDriveTable(wCurType);//查找需要发送的对应的表
            if(ptableDrive == NULL){
                WGTSetVisible(ppg->stoutport,FALSE);
                ShowMsg(VW_MSG_UDISKSAVEFAIL);
                return FALSE;
            }
            //向下位机发送读写驱动器数据的命令
            //增加本地导入导出功能
            VarAdrSetInt(CUR_OPERATETYPE,UDISKOPT);//0:无导入导出操作；1:U盘导入导出；2:本地面板导入导出
            ServoCtlRead(ptableDrive, GetPackCount(), DriveCallback ,NULL);
            return TRUE;
        }
        wCurType = NOOPERATEPAR;
        WGTSetVisible(ppg->stoutport,FALSE);
        ShowMsg(VW_MSG_UDISKSAVEFAIL);
        return FALSE;
    }
    else if(pwgt == ppg->btimportpanel)
    {
        if (VarAdrToInt(MOTOR_STATE_STATE) != 0)
        {
            ShowMsg(VW_MSG_PLSCLSMOTOR);
            return FALSE;
        }
        servo_write.servo_sn = VarAdrToInt(WRITE_DRIVEID);
        servo_write.servo_len =VarAdrToInt(WRITE_DRIVEID+2);
        servo_write.servo_func = 0x2000;
        servo_write.servo_addr = VarAdrToInt(WRITE_DRIVEID+1);
        servo_write.servo_val =  VarAdrToInt(WRITE_DRIVEID+3);
        ServoCtlWrite(&servo_write, 1, Write_Callback, NULL);
    }
    else if(pwgt == ppg->btoutportpanel)
    {
        servo_read.servo_sn = VarAdrToInt(READ_DRIVEID);
        servo_read.servo_len = VarAdrToInt(READ_DRIVEID+2);
        servo_read.servo_func = 0x2000;
        servo_read.servo_addr = VarAdrToInt(READ_DRIVEID+1);
        ServoCtlRead(&servo_read, 1, Read_Callback, NULL);
    }
    else if(pwgt == ppg->ed_addr)
    {
        if(VarAdrToInt(TMPVAL(13)) == 32)
        {
            EditSetMax(ppg->ed_data,0xFFFFFFFF);
        }
        else if(VarAdrToInt(TMPVAL(13)) == 16)
        {
            EditSetMax(ppg->ed_data,0xFFFF);
        }
    }
    return 0;
}
