/******************************************
*this file for M791&M782 20190505 hz
******************************************/

#include "file.h"
#include "poweroff.h"
#include "database.h"
#include "obj.h"
#include "common.h"
#include "time.h"
#include "prodmanage.h"
#include "verinfo.h"
#include "oprintf.h"
#include "panelcfg.h"
#include "keyrec.h"
#include "keyprot2.h"

#define POWER_OFF_PATH  SAVE_PATH"poweroff.dat"

UI8 g_cur_hour, g_cur_day, g_cur_month;
/************************************************************************/
/* 得到断电保护数据   M791&M782             20190104 hz                   */
/************************************************************************/
int GetProtectData(void* data)
{
    UI32 opencnt;
    pPOWERDOWNDATA s;

    s = (pPOWERDOWNDATA)data;

    // 手动模式下不保存 20190109 hz
    //去除手动模式下不保存  20190626 hz
//    if (OperateModeIndex() == MODE_MANUAL)
//    {
//        s->flag = 0;
//        return 0;
//    }

    opencnt = VarAdrToInt(PROD_TOTAL_SHOTCNT);
    s->totalopen = opencnt;
    s->flag = 0xeb90;

    s->cur_hour = g_cur_hour;
    s->cur_day = g_cur_day;
    s->cur_month = g_cur_month;

    if(PanelCfgStyle() == G_TYPE)//20201104
    {
        s->hour_cnt = g_G10ProdCnt.prod_cnt[g_cur_month][g_cur_day-1][g_cur_hour];
        s->hour_opencnt = g_G10ProdCnt.open_cnt[g_cur_month][g_cur_day-1][g_cur_hour];
        s->hour_power = g_G10ProdCnt.power[g_cur_month][g_cur_day-1][g_cur_hour];
        s->Power.dwAutoPower = g_dbPower.dwAutoPower;
        s->Power.dwCurrentPower = g_dbPower.dwCurrentPower;
        s->Power.dwOpenCnt = g_dbPower.dwOpenCnt;
        s->Power.dwOtherPower = g_dbPower.dwOtherPower;
        s->Power.dwTotalPower = g_dbPower.dwTotalPower;
        s->Power.tmModify = g_dbPower.tmModify;
    }
    else
    {
        s->DayPower.DayFlag = g_dbDayPower[0].DayFlag;
        s->DayPower.dwCount = g_dbDayPower[g_cur_hour].dwCount;
        s->DayPower.dwPower = g_dbDayPower[g_cur_hour].dwPower;
        s->DayPower.dwProduct = g_dbDayPower[g_cur_hour].dwProduct;

        s->MonthPower.MonthFlag = g_dbMonthPower[0].MonthFlag;
        s->MonthPower.dwCount = g_dbMonthPower[g_cur_day-1].dwCount;
        s->MonthPower.dwPower = g_dbMonthPower[g_cur_day-1].dwPower;
        s->MonthPower.dwProduct = g_dbMonthPower[g_cur_day-1].dwProduct;

        s->YearPower.YearFlag = g_dbYearPower[0].YearFlag;
        s->YearPower.dwCount = g_dbYearPower[g_cur_month].dwCount;
        s->YearPower.dwPower = g_dbYearPower[g_cur_month].dwPower;
        s->YearPower.dwProduct = g_dbYearPower[g_cur_month].dwProduct;
    }
    s->pack_open_cnt = VarAdrToInt(CLAMP_STATE_CURPACKMOLD);
    if((PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))//20200410
    {
        s->auto_lub_cnt = VarAdrToInt(LUB1_STATE_CN_CLAMPLUBRIC);
        s->auto_lub_cnt2 = VarAdrToInt(LUB2_STATE_CN_CLAMPLUBRIC);
        s->auto_lub_cnt3 = VarAdrToInt(LUB3_STATE_CN_CLAMPLUBRIC);
    }
    else
    {
        s->auto_lub_cnt = VarAdrToInt(CLAMP_STATE_CLAMPLUBRIC);
    }

//    if(PanelCfgSort() == HYDRAULIC){
//        s->totalopennocle += VarAdrToInt(POWER_ON_MOLD);
//    }
    //20211207 dyl 开模总数不清零
    s->totalopennocle += VarAdrToInt(p_PP_STATE_SHOT_TOTAL_CNT);

    //20220923 dyl 增加良品数保存
    s->prod_goodcnt = VarAdrToUI16(CLAMP_STATE_VALIDMOLD);

    s->flagend = 0xeb91;//20230518 优化断电保护
    //oprintf("GetProtectData------- \n");20240605 chj 去掉打印，节约时间

    return 1;
}

/************************************************************************/
/* 断电保护数据赋值      M791&M782          20190104 hz                   */
/************************************************************************/
void SetProtectData(void* data)
{
    UI32 opencnt;

    pPOWERDOWNDATA s;
    s = (pPOWERDOWNDATA)data;
//去除手动模式下不保存  20190626 hz
    //emwin，sd卡首次刷程序乱码 20190730 hz
    if ((s->flag == 0xeb90) && (s->flagend == 0xeb91)) //20230518 优化断电保护
    {
        s->flag = 0;//断电保存数据仅保存一次
        s->flagend = 0;//20230518 优化断电保护
        opencnt = s->totalopen;
        VarAdrSetInt(PROD_TOTAL_SHOTCNT, opencnt);

        VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM0,opencnt >> 16);
        VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM1,opencnt & 0xFFFF);
        VarSendByAdr(CLAMP_STATE_MOLDOPNNUM0);
        VarSendByAdr(CLAMP_STATE_MOLDOPNNUM1);
        VarAdrSetInt(PROD_SHOTCNT,(VarAdrToInt(CLAMP_STATE_MOLDOPNNUM0)<<16 | VarAdrToInt(CLAMP_STATE_MOLDOPNNUM1)));
        VarAdrSaveInt(PROD_SHOTCNT);//20190117
        //20211207 dyl 开模总数不清零
        VarAdrSetInt(p_PP_STATE_SHOT_TOTAL_CNT, s->totalopennocle);
        VarAdrSetInt(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL,s->totalopennocle >> 16);
        VarAdrSetInt(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL,s->totalopennocle & 0xFFFF);
        VarSendByAdr(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL);
        VarSendByAdr(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL);
        VarAdrSetInt(p_PP_MACHSET_SHOTCNT_TOTAL,(VarAdrToInt(d_clamp1_STATE_CN_MOLDOPNNUM0_NCL)<<16 | VarAdrToInt(d_clamp1_STATE_CN_MOLDOPNNUM1_NCL)));
        VarAdrSaveInt(p_PP_MACHSET_SHOTCNT_TOTAL);

        VarAdrSetInt(CLAMP_STATE_CURPACKMOLD, s->pack_open_cnt);
        VarAdrSetInt(PROD_PKGCNT, s->pack_open_cnt);
        VarAdrSaveInt(PROD_PKGCNT);
        VarSendByAdr(CLAMP_STATE_CURPACKMOLD);
        //20220923 dyl 增加良品数保存
        VarAdrSetInt(CLAMP_STATE_VALIDMOLD, s->prod_goodcnt);
        VarAdrSetInt(PROD_GOODCNT, s->prod_goodcnt);
        VarAdrSaveInt(PROD_GOODCNT);
        VarSendByAdr(CLAMP_STATE_VALIDMOLD);

        if((PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))//20200410
        {
            VarAdrSetInt(LUB1_STATE_CN_CLAMPLUBRIC, s->auto_lub_cnt);
            VarAdrSetInt(PROD_CLAMPLUBRIC, s->auto_lub_cnt);
            VarAdrSaveInt(PROD_CLAMPLUBRIC);
            VarSendByAdr(LUB1_STATE_CN_CLAMPLUBRIC);

            VarAdrSetInt(LUB2_STATE_CN_CLAMPLUBRIC, s->auto_lub_cnt2);
            VarAdrSetInt(PROD_CLAMPLUBRIC2, s->auto_lub_cnt2);
            VarAdrSaveInt(PROD_CLAMPLUBRIC2);
            VarSendByAdr(LUB2_STATE_CN_CLAMPLUBRIC);

            VarAdrSetInt(LUB3_STATE_CN_CLAMPLUBRIC, s->auto_lub_cnt3);
            VarAdrSetInt(PROD_CLAMPLUBRIC3, s->auto_lub_cnt3);
            VarAdrSaveInt(PROD_CLAMPLUBRIC3);
            VarSendByAdr(LUB3_STATE_CN_CLAMPLUBRIC);
        }
        else
        {
            VarAdrSetInt(CLAMP_STATE_CLAMPLUBRIC, s->auto_lub_cnt);
            VarAdrSetInt(PROD_CLAMPLUBRIC, s->auto_lub_cnt);
            VarAdrSaveInt(PROD_CLAMPLUBRIC);
            VarSendByAdr(CLAMP_STATE_CLAMPLUBRIC);
        }

        //日月年产量能耗
        //InitProdPower();//20230518 chj 优化断电保护，将函数移到外面

        if(PanelCfgStyle() == G_TYPE)//20201104
        {
            struct tm* t;
            time_t time_now;
            time(&time_now);
            t = localtime(&time_now);

            //20250318 jhh 跨年产量能耗数据保存 断电数据默认保存在G10ProdCnt缓存中，并写入ProdCnt.dat文件中
            if(s->cur_month >= 0 && s->cur_month < 12 && s->cur_day >= 1 && s->cur_day <= 31 && s->cur_hour >= 0 && s->cur_hour < 24)
            {
                g_G10ProdCnt.prod_cnt[s->cur_month][s->cur_day-1][s->cur_hour] = s->hour_cnt;
                g_G10ProdCnt.open_cnt[s->cur_month][s->cur_day-1][s->cur_hour] = s->hour_opencnt;
                g_G10ProdCnt.power[s->cur_month][s->cur_day-1][s->cur_hour] = s->hour_power;
            }
            if(g_G10ProdCnt.year != t->tm_year)
            {
                //只对向后跨年做文件拷贝处理，其余情况无操作
                if(g_G10ProdCnt.year < t->tm_year)//正常年份向后跨一年(2024->2025)
                {
                    memcpy(&g_G10ProdCntLast, &g_G10ProdCnt, sizeof(G10PRODCNT));
                    G10SaveProdCntLast();
                    memset(&g_G10ProdCnt, 0, sizeof(G10PRODCNT));
                }
                else
                {
                    ;
                }
                g_G10ProdCnt.year = t->tm_year;
            }

            G10SaveProdCnt();

            g_dbPower.dwAutoPower = s->Power.dwAutoPower;
            g_dbPower.dwCurrentPower = s->Power.dwCurrentPower;
            g_dbPower.dwOpenCnt = s->Power.dwOpenCnt;
            g_dbPower.dwOtherPower = s->Power.dwOtherPower;
            g_dbPower.dwTotalPower = s->Power.dwTotalPower;
            g_dbPower.tmModify = s->Power.tmModify;
            SavePowerMeter();//20230518 chj 假如断电保护文件有问题，需要面板定时保存下电表能耗数据
            VarSendSaveSetIntByAdr(d_machine1_STATE_MODOPENNUM1, g_dbPower.dwOpenCnt);
        }
        else
        {
            g_dbDayPower[0].DayFlag = s->DayPower.DayFlag;
            g_dbDayPower[s->cur_hour].dwCount = s->DayPower.dwCount;
            g_dbDayPower[s->cur_hour].dwPower = s->DayPower.dwPower;
            g_dbDayPower[s->cur_hour].dwProduct = s->DayPower.dwProduct;

            g_dbMonthPower[0].MonthFlag = s->MonthPower.MonthFlag;
            g_dbMonthPower[s->cur_day-1].dwCount = s->MonthPower.dwCount;
            g_dbMonthPower[s->cur_day-1].dwPower = s->MonthPower.dwPower;
            g_dbMonthPower[s->cur_day-1].dwProduct = s->MonthPower.dwProduct;

            g_dbYearPower[0].YearFlag = s->YearPower.YearFlag;
            g_dbYearPower[s->cur_month].dwCount = s->YearPower.dwCount;
            g_dbYearPower[s->cur_month].dwPower = s->YearPower.dwPower;
            g_dbYearPower[s->cur_month].dwProduct = s->YearPower.dwProduct;

            WriteProdPower(DAYPOWERFILE, g_dbDayPower, sizeof (DB_DAYPOWER)*DAY_HOUR);
            WriteProdPower(MONTHPOWERFILE, g_dbMonthPower, sizeof (DB_MONTHPOWER)*MON_DAY);
            WriteProdPower(YEARPOWERFILE, g_dbYearPower, sizeof (DB_YEARPOWER)*YEAR_MON);
        }

        //日月年产量能耗 END

        oprintf("SetProtectData------- \n");
    }
    else
    {
        oprintf("SetProtectData error------- \n");
    }
}


/**************************
 * this part for M791
***************************/

#ifndef WIN32
#ifdef IMX6

#ifdef M788_PANEL //TI和A40i断电保护都用同一种断电保护
#include "linux/input.h"
#include "fcntl.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/time.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "stdio.h"

static int powerfile = -1;
static BOOL bOnlyOne = FALSE;
static BOOL bKillSync = FALSE; //开始进行重启ch_sync计时 //20230518 chj 断电的时候，可能会出现同步写文件，这里先关闭，后续再开启
static BOOL bNeedStartSync = FALSE;//需要重启同步文件
static const char* process_name = "ch_sync";
static pid_t pid = -1;

static BOOL bNeedStartOpen = FALSE;// 20240605 chj 假如断电误触发，powerfile文件被关闭，需要定时再开启下。

int get_pid_by_name(const char* process_name) {
    DIR* dir = opendir("/proc");
    if (dir == NULL) {
        oprintf("opendir failed\n");
        return -1;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            // 检查目录名是否为纯数字（进程PID为数字）
            int len = strlen(entry->d_name);
            int i;
            for (i = 0; i < len; i++) {
                if (entry->d_name[i] < '0' || entry->d_name[i] > '9') {
                    //oprintf("opendir failed\n");
                    break;
                }
            }
            if (i == len) {
                // 读取进程的命令行路径
                char cmdline_path[256];
                //oprintf("opendir failed pid =%s\n",entry->d_name);
                snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%s/comm", entry->d_name);
                FILE* cmdline_file = fopen(cmdline_path, "rb");
                if (cmdline_file != NULL) {
                    char cmdline[256];
                    if(fgets(cmdline,sizeof(cmdline),cmdline_file) != NULL)
                    {
                        // 去除命令行中的换行符
                        cmdline[strlen(cmdline) - 1] = '\0';
                        // 比较进程名
                        //oprintf("%s,%s\n",cmdline,process_name);
                        if (strcmp(cmdline, process_name) == 0) {
                            //oprintf("opendir success\n");
                            fclose(cmdline_file);
                            closedir(dir);
                            return atoi(entry->d_name);
                        }
                    }
                    fclose(cmdline_file);
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

void signalSavePoweroffData(int temp)
{
    //20220822 dyl 断电保存优化：断电关屏
    //SetLCD(FALSE);//驱动指令控制  //20230517 chj 实测这些代码影响断电数据，去掉后，断24V，也可以保存全
    //Send_LCDOnOff(FALSE);//驱动指令控制

    oprintf("recive signal %d\r\n", temp);
    if(!bOnlyOne){

        if((pid != -1) && (bKillSync == FALSE) && (temp != 99))//99为更新应用程序
        {
            if (kill(pid,SIGKILL) == 0)//20230518 chj 断电的时候，可能会出现同步写文件，这里先关闭，后续再开启
            {
                oprintf("kill ch_sync\n");
                bKillSync = TRUE;
                bNeedStartSync = TRUE;
                pid = -1;
            }
            else
            {
                oprintf("process %d killed failed",pid);
            }
        }

        POWERDOWNDATA data;
        memset(&data, 0, sizeof(POWERDOWNDATA));
        GetProtectData(&data);

        bNeedStartOpen = TRUE;// 20240605 chj 假如断电误触发，powerfile文件被关闭，需要定时再开启下。
        if(powerfile <= 0)
        {
            oprintf("Open PowerOff.dat failed\r\n");
            return;
        }
        write(powerfile, &data, sizeof(POWERDOWNDATA));
        close(powerfile);

        powerfile = -1;
        bOnlyOne = TRUE;
        oprintf("PowerOff SAVE END, data size: %d\r\n", sizeof(POWERDOWNDATA));

        //20220822 dyl 断电保存优化：写文件后延迟1s进行其他操作
        UI32 TimeStart = GetTick();
        while (1)
        {
            UI32 TimeStop = GetTick();
            if ((TimeStop-TimeStart) > (1000*1))//1s
            {
                break;
            }
        }
    }
}

void SavePoweroffData()
{
    int temp = 99;
    signalSavePoweroffData(temp);
}

void PowerOffInint()
{
    pid = get_pid_by_name(process_name);//20230518 chj 断电的时候，可能会出现同步写文件，这里先关闭，后续再开启
    if(pid != -1)
    {
        oprintf("find ch_sync pid:%d\n",pid);
        if (kill(pid,SIGKILL) == 0)
        {
            oprintf("kill ch_sync pid:%d\n",pid);
            pid = -1;
        }
    }
    else {
        ;
    }

    FILEHD datafile;
    POWERDOWNDATA data;
    datafile = FileOpen(POWER_OFF_PATH);

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    g_cur_hour = t->tm_hour;
    g_cur_day = t->tm_mday;
    g_cur_month = t->tm_mon;

    if(FILEHD_IS_OK(datafile))
    {
        FileRead(datafile, &data, sizeof(POWERDOWNDATA), NULL);
        SetProtectData(&data);
        FileClose(datafile);//读文件操作完后要先关闭文件才能进行写操作
        datafile = FileOpen(POWER_OFF_PATH);
        FileWrite(datafile, &data, sizeof(POWERDOWNDATA), NULL);//保存flag
        FileClose(datafile);
    }
    else
    {
        datafile = FileCreate(POWER_OFF_PATH);
        memset(&data, 0, sizeof(POWERDOWNDATA));
        FileWrite(datafile, &data, sizeof(POWERDOWNDATA), NULL);
        FileClose(datafile);
    }

    powerfile = open(POWER_OFF_PATH, O_RDWR | O_SYNC);

    int nfd = open("/dev/lvd", O_RDWR,S_IRUSR|S_IWUSR);
    if (nfd < 0)
    {
        oprintf("lvd open err !\n");
    }
    else
    {
        oprintf("lvd ok !\n");
        //启动信号驱动机制
        signal(SIGIO,signalSavePoweroffData);
        /*  将SIGIO信号同Process_PowerOff函数关联起来，一旦产生SIGIO信号,就会执行Process_PowerOff，   */
        fcntl(nfd, F_SETOWN, getpid());
        /* nfd 是打开的设备文件描述符,  F_SETOWN用来决定操作是干什么的,getpid()是个系统调用，
        功能是返回当前进程的进程号整个函数的功能是STDIN_FILENO设置这个设备文件的拥有者为当前进程。  */
        int oflags = fcntl(nfd, F_GETFL);
        /*得到打开文件描述符的状态*/
        fcntl(nfd, F_SETFL, oflags | FASYNC);
        /*   设置文件描述符的状态为oflags | FASYNC属性,一旦文件描述符被设置成具有FASYNC属性的状态，
        也就是将设备文件切换到异步操作模式。这时系统就会自动调用驱动程序的fasync方法。  */
    }
}

void PowerOffMonitor()
{
    static int count = 0;
    static UI32 time_10ms = 0;
    if(bOnlyOne){
        if(GetTick() - time_10ms >= 10)
        {
            count++;
            time_10ms = GetTick();
        }
        if(count > 100){
            bOnlyOne = FALSE;
            count = 0;
            oprintf("PowerOff OnlyOne Again\r\n");
        }
    }

    /* 20240605 chj 后续版本不再使用ch_sync，所以不再开启
    //此处防止误信号，10s后重新开启
    static int countSync = 0; //20230518 chj 断电的时候，可能会出现同步写文件，这里先关闭，后续再开启
    static UI32 time_Sync_10ms = 0;
    if(bKillSync){
        if(GetTick() - time_Sync_10ms >= 10)
        {
            countSync++;
            time_Sync_10ms = GetTick();
            //oprintf("Sync_ch tickcnt:%d\r\n",countSync);
        }
        if((countSync > 1000) && bNeedStartSync){//10s
#ifdef A40I //此处需要注意，查看ch_sync的位置
            osystem("ch_sync &");
#else
            osystem("/opt/ch_sync &");
#endif
            bNeedStartSync = FALSE;
            oprintf("Restart ch_sync\r\n");
        }
        if(countSync > 1200){//12s
            bKillSync = FALSE;
            countSync = 0;
            pid = get_pid_by_name(process_name);
            oprintf("ReGet ch_sync pid :%d",pid);
        }
    }
    */

    // 20240605 chj 假如断电误触发，powerfile文件被关闭，需要定时再开启下。
    static int countPowerfile = 0;
    static UI32 time_Powerfile_1s = 0;
    if(bNeedStartOpen){
        if(GetTick() - time_Powerfile_1s >= 1000)
        {
            countPowerfile++;
            time_Powerfile_1s = GetTick();
            //oprintf("countPowerfile tickcnt:%d\r\n",countPowerfile);
        }
        if((countPowerfile > 60) && (powerfile <= 0)){//60s

            powerfile = open(POWER_OFF_PATH, O_RDWR | O_SYNC);
            bNeedStartOpen = FALSE;
            countPowerfile = 0;
            time_Powerfile_1s = 0;
            oprintf("Reopen POWER_OFF_PATH\r\n");
        }
    }
}

#else

#ifdef A40I //20220614 dyl

#include "linux/input.h"
#include "fcntl.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

static int powerfile = -1;
static BOOL bOnlyOne = FALSE;

void signalSavePoweroffData(int temp)
{
    oprintf("recive signal %d\r\n", temp);
    if(!bOnlyOne){
        POWERDOWNDATA data;
        memset(&data, 0, sizeof(POWERDOWNDATA));
        GetProtectData(&data);

        if(powerfile <= 0)
        {
            oprintf("Open PowerOff.dat failed\r\n");
            return;
        }
        write(powerfile, &data, sizeof(POWERDOWNDATA));
        close(powerfile);
        powerfile = -1;
        bOnlyOne = TRUE;
        oprintf("PowerOff SAVE END, data size: %d\r\n", sizeof(POWERDOWNDATA));
    }
}

void SavePoweroffData()
{
    int temp = 99;
    signalSavePoweroffData(temp);
}

void PowerOffInint()
{
    FILEHD datafile;
    POWERDOWNDATA data;
    datafile = FileOpen(POWER_OFF_PATH);

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    g_cur_hour = t->tm_hour;
    g_cur_day = t->tm_mday;
    g_cur_month = t->tm_mon;

    if(FILEHD_IS_OK(datafile))
    {
        FileRead(datafile, &data, sizeof(POWERDOWNDATA), NULL);
        SetProtectData(&data);
        FileClose(datafile);//读文件操作完后要先关闭文件才能进行写操作
        datafile = FileOpen(POWER_OFF_PATH);
        FileWrite(datafile, &data, sizeof(POWERDOWNDATA), NULL);//保存flag
        FileClose(datafile);
    }
    else
    {
        datafile = FileCreate(POWER_OFF_PATH);
        memset(&data, 0, sizeof(POWERDOWNDATA));
        FileWrite(datafile, &data, sizeof(POWERDOWNDATA), NULL);
        FileClose(datafile);
    }

    powerfile = open(POWER_OFF_PATH, O_RDWR | O_SYNC);

    int nfd = open("/dev/lvd", O_RDWR,S_IRUSR|S_IWUSR);
    if (nfd < 0)
    {
        oprintf("lvd open err !\n");
    }
    else
    {
        oprintf("lvd ok !\n");
        //启动信号驱动机制
        signal(SIGIO,signalSavePoweroffData);
        /*  将SIGIO信号同Process_PowerOff函数关联起来，一旦产生SIGIO信号,就会执行Process_PowerOff，   */
        fcntl(nfd, F_SETOWN, getpid());
        /* nfd 是打开的设备文件描述符,  F_SETOWN用来决定操作是干什么的,getpid()是个系统调用，
        功能是返回当前进程的进程号整个函数的功能是STDIN_FILENO设置这个设备文件的拥有者为当前进程。  */
        int oflags = fcntl(nfd, F_GETFL);
        /*得到打开文件描述符的状态*/
        fcntl(nfd, F_SETFL, oflags | FASYNC);
        /*   设置文件描述符的状态为oflags | FASYNC属性,一旦文件描述符被设置成具有FASYNC属性的状态，
        也就是将设备文件切换到异步操作模式。这时系统就会自动调用驱动程序的fasync方法。  */
    }
}

void PowerOffMonitor()
{
    static int count = 0;
    static UI32 time_10ms = 0;
    if(bOnlyOne){
        if(GetTick() - time_10ms >= 10)
        {
            count++;
            time_10ms = GetTick();
        }
        if(count > 100){
            bOnlyOne = FALSE;
            count = 0;
            oprintf("PowerOff OnlyOne Again\r\n");
        }
    }
}

#else

#include "linux/input.h"
#include "fcntl.h"
#include "unistd.h"

static int event0file = -1;
static int powerfile = -1;
void PowerOffInint()
{
    FILEHD datafile;
    POWERDOWNDATA data;
    event0file = open("/dev/event0", O_RDONLY|O_NDELAY);
    datafile = FileOpen(POWER_OFF_PATH);

    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);
    g_cur_hour = t->tm_hour;
    g_cur_day = t->tm_mday;
    g_cur_month = t->tm_mon;

    if(FILEHD_IS_OK(datafile))
    {
        FileRead(datafile, &data, sizeof(POWERDOWNDATA), NULL);
        SetProtectData(&data);
        FileWrite(datafile, &data, sizeof(POWERDOWNDATA), NULL);//保存flag
        FileClose(datafile);
    }
    else
    {
        datafile = FileCreate(POWER_OFF_PATH);
        memset(&data, 0, sizeof(POWERDOWNDATA));
        FileWrite(datafile, &data, sizeof(POWERDOWNDATA), NULL);
        FileClose(datafile);
    }

    powerfile = open(POWER_OFF_PATH, O_RDWR | O_SYNC);
}

void SavePoweroffData()
{
    POWERDOWNDATA data;
    memset(&data, 0, sizeof(POWERDOWNDATA));
    GetProtectData(&data);

    if(powerfile <= 0)
    {
        oprintf("Open PowerOff.dat failed\n");
        return;
    }
    write(powerfile, &data, sizeof(POWERDOWNDATA));
    close(powerfile);
    powerfile = -1;
}

void PowerOffMonitor()
{
    int count;
    static UI32 time_10ms = 0;
    struct input_event t;

    memset(&t,0,sizeof(struct input_event));

    if (event0file)
    {
        if(GetTick() - time_10ms >= 10)
        {
            while(1)
            {
                count = read(event0file, &t, sizeof(struct input_event));

                if(count > 0){
                    switch(t.type){
                    case EV_KEY:
                        if(t.code == 1 && t.value == 1)
                        {
                            //20220822 dyl 断电保存优化：断电关屏
                            SetLCD(FALSE);//驱动指令控制
                            Send_LCDOnOff(FALSE);//驱动指令控制
                            SavePoweroffData();
                        }
                        break;
                    default:
                        break;
                    }
                }
                else
                    break;
            }
            time_10ms = GetTick();
        }
    }
}
#endif

#endif
#endif
#endif
