#include "drivealarmrecord.h"
#include "protcmd.h"
#include "oprintf.h"
#include "file.h"
#include "string.h"

DRIVEALARMRECORD m_drivealarmrecd;
static BOOL rec_drive_data = FALSE;

HOSTALARMRECORD m_hostalarmrecd;

#define MAX_REQ_NUM 10 //最大请求数量
//20200814 声明该结构体用于伺服警报时参数依次请求
typedef struct tyREQDRIVEPARAM
{
    UI16 flag;
    UI16 alarmservoid;
    UI16 alarmcode;
    UI16 cur_no;
}REQDRIVEPARAM;
static REQDRIVEPARAM req_param[MAX_REQ_NUM];
static UI16 req_param_num = 0;//请求数量

//驱动器参数交互
#define SERVO_PARAREC_NO       14
static PRO_SDO_RX_LOG servo_read[SERVO_PARAREC_NO];
static UI16 ServoAdd[SERVO_PARAREC_NO] =   //驱动器地址指针
{
        0x0100, //MCU 版本
        0x0101, //FPGA 版本
        0x0B37, //故障时电机转速
        0x0B38, //故障时电机U相电流
        0x0B39, //故障时电机V相电流
        0x0B40, //故障时母线电压
        0x0B41, //故障时端子输入状态
        0x0B42, //故障时端子输出状态
        0x0B43, //参数异常的功能码组
        0x0B44, //参数异常的功能码组内偏置
        0x0B46, //故障时FPGA给出的绝对编码器故障信息
        0x0B47, //故障时FPGA给出的系统状态信息
        0x0B48, //故障时FPGA给出的系统故障信息
        0x0B35, //故障时间戳
};

static UI8 ServoLen[SERVO_PARAREC_NO] =   //驱动器数据长度指针
{
        16, //MCU 版本
        16, //FPGA 版本
        16, //故障时电机转速
        16, //故障时电机U相电流
        16, //故障时电机V相电流
        16, //故障时母线电压
        16, //故障时端子输入状态
        16, //故障时端子输出状态
        16, //参数异常的功能码组
        16, //参数异常的功能码组内偏置
        16, //故障时FPGA给出的绝对编码器故障信息
        16, //故障时FPGA给出的系统状态信息
        16, //故障时FPGA给出的系统故障信息
        32, //故障时间戳
};

static BOOL CheckFileValid(FILEHD filehd)
{
    if(FILEHD_IS_OK(filehd))
    {
        return TRUE;
    }
    else
    {
        FileClose(filehd);
        return FALSE;
    }
}

static BOOL DriveAlarm_WriteRecord(UI16 itmenum)
{
    int index;

    FILEHD filehd;
    filehd = FileOpen(DRIVEALARM_SAVE_PATH);
    index = sizeof(DRIVEALARMHEAD) + sizeof(DRIVEALARMITEM)*itmenum;
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_drivealarmrecd,sizeof(DRIVEALARMHEAD),NULL);
        FileSeek(filehd,index);
        FileWrite(filehd,(char*)(&m_drivealarmrecd) + index,sizeof(DRIVEALARMITEM),NULL);
        FileClose(filehd);
        return TRUE;
    }
    return FALSE;
}

DRIVEALARMITEM DriveAlarm_ReadRecord(UI32 index)
{
    DRIVEALARMITEM item;
    if(index < G10_MAX_RECORDS)
    {
        item = m_drivealarmrecd.item[index];
    }
    return item;
}

void DriveAlarm_LoadRecord()
{
    FILEHD filehd;

    filehd = FileOpen(DRIVEALARM_SAVE_PATH);

    if(CheckFileValid(filehd))
    {
        FileRead(filehd,&m_drivealarmrecd,sizeof(DRIVEALARMRECORD),NULL);
        FileClose(filehd);
    }
    else
    {
        filehd = FileCreate(DRIVEALARM_SAVE_PATH);
        FileClose(filehd);
    }
}

void DriveAlarm_ClearRecord()
{
    int i;

    FileDelete(DRIVEALARM_SAVE_PATH);
    DriveAlarm_LoadRecord();

    m_drivealarmrecd.head.cur_no = 0;
    for(i=0;i<G10_MAX_RECORDS;i++)
    {
        m_drivealarmrecd.item[i].flag = 0;
        //DriveAlarm_WriteRecord(i);
    }
}

/**
 * @brief      : 是否有驱动器警报记录
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200810
 */
BOOL DriveAlarm_HaveRecord(UI32 index, UI16 subId)
{
    if(index < G10_MAX_RECORDS)
    {
        if(m_drivealarmrecd.item[index].flag == MARK_USED && m_drivealarmrecd.item[index].alarmcode == subId)
        {
            return TRUE;
        }
    }

    return FALSE;
}

//接收伺服数据
static void Read_Callback(BOOL result, PRO_SDO_RX_LOG* logs, int cnt, void* param)
{
    UI16 curno;
    UI16 data[3];
    UI16 i;

    memcpy(data, param, sizeof(data));
    OBJMemFree(param);
    param = NULL;

    oprintf("DRIVEALARMRECORD result:%d,cnt:%d data = %d,%d,%d\n",result,cnt,data[0],data[1],data[2]);
    if (result)
    {
        curno = data[2];
        m_drivealarmrecd.head.cur_no = curno;
        m_drivealarmrecd.item[curno].flag = MARK_USED;
        m_drivealarmrecd.item[curno].alarmservoid = data[0];
        m_drivealarmrecd.item[curno].alarmcode = data[1];

        m_drivealarmrecd.item[curno].H0100 = (logs+0)->servo_val;
        m_drivealarmrecd.item[curno].H0101 = (logs+1)->servo_val;
        m_drivealarmrecd.item[curno].H0B37 = (logs+2)->servo_val;
        m_drivealarmrecd.item[curno].H0B38 = (logs+3)->servo_val;
        m_drivealarmrecd.item[curno].H0B39 = (logs+4)->servo_val;
        m_drivealarmrecd.item[curno].H0B40 = (logs+5)->servo_val;
        m_drivealarmrecd.item[curno].H0B41 = (logs+6)->servo_val;
        m_drivealarmrecd.item[curno].H0B42 = (logs+7)->servo_val;
        m_drivealarmrecd.item[curno].H0B43 = (logs+8)->servo_val;
        m_drivealarmrecd.item[curno].H0B44 = (logs+9)->servo_val;
        m_drivealarmrecd.item[curno].H0B46 = (logs+10)->servo_val;
        m_drivealarmrecd.item[curno].H0B47 = (logs+11)->servo_val;
        m_drivealarmrecd.item[curno].H0B48 = (logs+12)->servo_val;
        m_drivealarmrecd.item[curno].H0B35 = (logs+13)->servo_val;

        DriveAlarm_WriteRecord(curno);
        rec_drive_data = TRUE;
    }

    //20200814 收到多个请求指令时先请求第一个，请求完成再发送第二个请求...
    req_param_num--;
    if(req_param_num > 0)
    {
        data[0] = req_param[1].alarmservoid;
        data[1] = req_param[1].alarmcode;
        data[2] = req_param[1].cur_no;

        for(i = 0; i < req_param_num;i++)
        {
            req_param[i].alarmservoid = req_param[i+1].alarmservoid;
            req_param[i].alarmcode = req_param[i+1].alarmcode;
            req_param[i].cur_no = req_param[i+1].cur_no;
        }

        DriveAlarm_ReqServoIdData(data[0],data[1],data[2]);
    }
    else
    {
        req_param[0].flag = 0;
    }
}

/**
 * @brief      : 请求伺服数据
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200810
 */
void DriveAlarm_ReqServoIdData(UI16 servo_id, UI32 alarmcode, UI16 curno)
{
    int i;
    UI16 *param = NULL;
    UI16 data[3] = {0};

    //20200814 收到多个请求指令时先请求第一个，请求完成再发送第二个请求...
    if(req_param[0].flag == MARK_USED && req_param_num > 0)
    {
        if(req_param_num >= MAX_REQ_NUM)
        {
            oprintf("DriveAlarm_ReqServoIdData req_param_num enough!\n");
            return;
        }

        if(req_param[req_param_num].flag == MARK_USED)
        {
            req_param[req_param_num].flag = 0;
        }
        else
        {
            for(i = 1; i < MAX_REQ_NUM; ++i)
            {
                if(req_param[i].flag != MARK_USED)
                {
                    req_param[i].flag = MARK_USED;
                    req_param[i].alarmservoid = servo_id;
                    req_param[i].alarmcode = alarmcode;
                    req_param[i].cur_no = curno;
                    req_param_num++;
                    oprintf("DriveAlarm_ReqServoIdData req_param_num %d return\n",req_param_num);
                    return;
                }
            }
        }
    }
    else
    {
        req_param[0].flag = MARK_USED;
        req_param[0].alarmservoid = servo_id;
        req_param[0].alarmcode = alarmcode;
        req_param[0].cur_no = curno;
        req_param_num++;
    }
    oprintf("DriveAlarm_ReqServoIdData start req_param_num %d\n",req_param_num);

    for(i = 0; i < SERVO_PARAREC_NO; ++i)
    {
        servo_read[i].servo_sn = servo_id;
        servo_read[i].servo_len = ServoLen[i];
        servo_read[i].servo_func = 0x2000;
        servo_read[i].servo_addr = ServoAdd[i];
    }

    data[0] = servo_id;
    data[1] = alarmcode;
    data[2] = curno;

    param = OBJMemMalloc(sizeof(data));
    memcpy(param, data, sizeof(data));

    ServoCtlRead(servo_read, SERVO_PARAREC_NO, Read_Callback, param);
    return;
}

/*** 下位机上传警报分析记录 ***/
static BOOL HostAlarm_WriteRecord(UI16 itmenum)
{
    int index;

    FILEHD filehd;
    filehd = FileOpen(HOSTALARM_SAVE_PATH);
    index = sizeof(HOSTALARMHEAD) + sizeof(WDITEM)*itmenum;
    if(CheckFileValid(filehd))
    {
        FileSeek(filehd,0);
        FileWrite(filehd,&m_hostalarmrecd,sizeof(HOSTALARMHEAD),NULL);
        FileSeek(filehd,index);
        FileWrite(filehd,(char*)(&m_hostalarmrecd) + index,sizeof(WDITEM),NULL);
        FileClose(filehd);
        return TRUE;
    }
    return FALSE;
}

WDITEM HostAlarm_ReadRecord(UI32 index)
{
    WDITEM item;
    if(index < G10_MAX_RECORDS)
    {
        item = m_hostalarmrecd.item[index];
    }
    return item;
}

void HostAlarm_LoadRecord()
{
    FILEHD filehd;

    filehd = FileOpen(HOSTALARM_SAVE_PATH);

    if(CheckFileValid(filehd))
    {
        FileRead(filehd,&m_hostalarmrecd,sizeof(HOSTALARMRECORD),NULL);
        FileClose(filehd);
    }
    else
    {
        filehd = FileCreate(HOSTALARM_SAVE_PATH);
        FileClose(filehd);
    }
}

void HostAlarm_ClearRecord()
{
    int i;

    FileDelete(HOSTALARM_SAVE_PATH);
    HostAlarm_LoadRecord();

    m_hostalarmrecd.head.cur_no = 0;
    for(i=0;i<G10_MAX_RECORDS;i++)
    {
        m_hostalarmrecd.item[i].rev[0] = 0;//rev[0]用作flag
        //HostAlarm_WriteRecord(i);
    }
}

/**
 * @brief      : 是否有主机警报记录
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200810
 */
BOOL HostAlarm_HaveRecord(UI32 index, UI32 warnid, UI32 datetime)
{
    if(index < G10_MAX_RECORDS)
    {
        UI32 alarmId = (m_hostalarmrecd.item[index].WarnData.wCurrent_Err1<<16) | m_hostalarmrecd.item[index].WarnData.wCurrent_Err0;
        if(m_hostalarmrecd.item[index].rev[0] == MARK_USED && alarmId == warnid)
        {
            if(datetime > 0)
            {
                char temp[20];
                TimeToStr(temp, "yyyy-MM-dd", datetime);
                if(strstr(m_hostalarmrecd.item[index].time, temp) != NULL)
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @brief      : 添加主机警报记录 跟随警报记录
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200810
 */
void AddHostAlarm(WDITEM item)
{
    UI16 curno;
    UI16 partid,errid;
    UI32 warnId = (item.WarnData.wCurrent_Err1<<16) | item.WarnData.wCurrent_Err0;

    curno = m_g10_warnrecd.warnHead.cur_no;
    memcpy(&m_hostalarmrecd.item[curno], &item, sizeof(WDITEM));
    m_hostalarmrecd.head.cur_no = curno;
    //m_hostalarmrecd.item[curno].rev[0] = MARK_USED;//rev[0]用作flag //20241218 chj 相同警报不记录 2s改为10分钟

    if((warnId & 0xFF0FFFFF) == 0x0F000101)//伺服警报
    {
        partid = (warnId&0x00F00000) >> 20;
        errid = item.WarnData.wServoErr[partid-1];
        if(G10_AlarmWRRecord(warnId,errid))
        {
            m_hostalarmrecd.item[curno].rev[0] = MARK_USED;//rev[0]用作flag //20241218 chj 相同警报不记录 2s改为10分钟
            HostAlarm_WriteRecord(curno);
            DriveAlarm_ReqServoIdData(partid, errid, curno);
        }
    }
    else
    {
        if(G10_AlarmWRRecord(warnId, 0))//先收到下位机上传的警报分析 再收到130警报
        {
            m_hostalarmrecd.item[curno].rev[0] = MARK_USED;//rev[0]用作flag //20241218 chj 相同警报不记录 2s改为10分钟
            HostAlarm_WriteRecord(curno);
        }
    }

}

/**
 * @brief      : 初始化详细信息状态
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200810
 */
void InitAlarmExpState()
{
    rec_drive_data = FALSE;
}

/**
 * @brief      : 获取详细信息状态
 * @param      :
 * @return     : TRUE表示警报详细信息有更新
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200810
 */
BOOL GetAlarmExpState()
{
    return rec_drive_data;
}
