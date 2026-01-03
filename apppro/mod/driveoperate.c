#include "driveoperate.h"
#include "file.h"
#include "common.h"
#include "oprintf.h"
#include "protcmd.h"
#include "stdio.h"
#include "texttbl.h"
#include "tstring.h"
#include "oprintf.h"
#define MIN_ALLOC_SIZE 512

#define TEXT_DRVIE_NO       106 //Drive NO.     驱动器编号
#define TEXT_DRVIE_LENGTH   107 //Drive Length  驱动器数据长度
#define TEXT_DRVIE_FUNC     108 //Drive Func    驱动器功能码
#define TEXT_DRVIE_ADDR     109 //Drive Addr    驱动器地址
#define TEXT_DRVIE_VALUE    110 //Drive Value   驱动器数据

//本地导入导出功能
#define MAX_DRIVECNT  4 //驱动器个数
UI16 wDriveRow[MAX_DRIVECNT] = {0};//各个轴请求的驱动器数据个数，用于本地面板读取驱动器参数用
#define NUM_ROWS_MAXCOUNT    5000 //单独一个.csv文件的行数最大值
#define NUM_COLUMNS_MAXCOUNT 5    //5列
#define NUM_ROWS_LOCAL_MAXCOUNT    5000  //4个.csv文件的行数最大值

//20190926 YX
//驱动器整个结构体哈希表 （用于保存读/写表，也可以直接用静态变量表示，无需哈希表）
static PHASH_TBL g_type  = NULL;
#define ADD_PACK(TYPE,PACK)           HashTblAddPairByInt(g_type,TYPE,(void*)PACK)
#define KEY_MAXCOUNTX        (1000)


static  UI16 s_wCount;//U盘导入excel数据行数

//保存一个（共4个）.csv时读取数据包，开始的行数和结束的行数
UI32 s_wRealRow,s_wStartRow;

/**
 * @brief         : 读取每台驱动器从数据包的第几行开始和第几行结束
 *
 * @param      : 驱动器编号（射胶0，开关模1，顶针2，熔胶3）
 * @return      :
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191015）
 */
static void GetDriveRow(UI16 wNum)
{
    s_wStartRow = 0;
    s_wRealRow = 0;
    int i;
    for(i = 0; i <= wNum; i++)
    {
        s_wRealRow += wDriveRow[i];
    }

    if(wNum == 0)
    {
        s_wStartRow = 0;
    }
    else
    {
        s_wStartRow = s_wRealRow - wDriveRow[wNum];
    }
}

UI16 GetPackCount()
{
    return s_wCount;
}

/**
* @brief     :根据当前语言保存伺服数据到.csv文件
* @param     :文件名
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
UI16 SaveDriveDataToExcel(char filename[])
{
    TSTRING_CREATE(str_tran);

    char tmp[20];
    FILEHD stream;
    char * p_str;
    int j;
    p_str = ConvToGBK_Dup(filename);//中文编码转换
    if(!FileExist(p_str))
    {
       stream = FileCreate(p_str);
    }
    else
    {
       stream = FileOpen(p_str);
    }

    OBJMemFree(p_str);
    p_str = NULL;

    if(!FILEHD_IS_OK(stream))
    {
        TSTRING_DESTROY(str_tran);
        return FALSE;
    }
    else
    {
        UI8 bs[3] = { (UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
        FileWrite(stream, bs, sizeof(bs), NULL);//20190923.cyx 增加BOM头，解决office打开乱码问题

        //写入第一行中文 [驱动器编号],[驱动器数据长度],[驱动器功能码],[驱动器地址],[驱动器数据]
        tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_NO), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_LENGTH), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_FUNC), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_ADDR), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_VALUE), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);

        PRO_SDO_RX_LOG* tbDrivePar = FindDriveTable(READDRIVEPAR);//查找读驱动器数据表
        if(tbDrivePar == NULL)
        {
            TSTRING_DESTROY(str_tran);
            FileClose(stream);
            return FALSE;
        }

        for(j = 0; j < s_wCount; j++,tbDrivePar ++)// 写入请求到的数据
        {
            WordToStr(tmp,tbDrivePar->servo_sn,0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,tbDrivePar->servo_len,0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,tbDrivePar->servo_func,HEXFLAG);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,tbDrivePar->servo_addr,HEXFLAG);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,tbDrivePar->servo_val,0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);
        }
        oprintf("str_tran.len:%ld\n",str_tran.len);
        FileWrite(stream, str_tran.str, str_tran.len, NULL);
        TSTRING_DESTROY(str_tran);
        FileClose(stream);
        return TRUE;
    }
}

//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL SaveDriveDataToExcel_Local(char filename[])
{
    TSTRING_CREATE(str_tran);

    FILEHD stream;
    char * p_str;
    PRO_SDO_RX_LOG* tbDrivePar;
    UI16 i,j;

    char cbuf[255];
    char tmp[20];

    PRO_SDO_RX_LOG* ptableDrive = FindDriveTable(READDRIVEPAR);//查找读驱动器数据表
    if(ptableDrive == NULL)
    {
        TSTRING_DESTROY(str_tran);
        return FALSE;
    }
    tbDrivePar = (PRO_SDO_RX_LOG*)(ptableDrive);

    for(i = 0; i < MAX_DRIVECNT; i++)
    {
        if (wDriveRow[i] != 0)//判断所勾选的驱动器是否请求到数据
        {
            sprintf(cbuf,"%s%d.csv", filename ,i+1);

            p_str = ConvToGBK_Dup(cbuf);//中文编码转换

            if(!FileExist(p_str))
            {
                stream = FileCreate(p_str);
            }
            else
            {
                stream = FileOpen(p_str);
            }

            OBJMemFree(p_str);

            p_str = NULL;

            if(!FILEHD_IS_OK(stream))
            {
                TSTRING_DESTROY(str_tran);
                return FALSE;
            }
            else
            {
                UI8 bs[3] = { (UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
                FileWrite(stream, bs, sizeof(bs), NULL);//20190923.cyx 增加BOM头，解决office打开乱码问题
                tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_NO), MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_LENGTH), MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_FUNC), MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_ADDR), MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                tstring_appends_fix(&str_tran, GetTextTran(TEXT_DRVIE_VALUE), MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);

                GetDriveRow(i);

                for(j = s_wStartRow; j < s_wRealRow ; j++,tbDrivePar ++)
                {
                    WordToStr(tmp,tbDrivePar->servo_sn,0);
                    tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                    WordToStr(tmp,tbDrivePar->servo_len,0);
                    tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                    WordToStr(tmp,tbDrivePar->servo_func,HEXFLAG);
                    tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                    WordToStr(tmp,tbDrivePar->servo_addr,HEXFLAG);
                    tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

                    WordToStr(tmp,tbDrivePar->servo_val,0);
                    tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                    tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
                    tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);
                }
            }
            oprintf("str_tran.len:%ld\n",str_tran.len);
            FileWrite(stream, str_tran.str, str_tran.len, NULL);
            TSTRING_DESTROY(str_tran);
            FileClose(stream);
        }
    }

    for(i = 0;i < MAX_DRIVECNT;i++)
    {
        wDriveRow[i] = 0;
    }
    return TRUE;
}

/**
* @brief     :用来检测伺服数据csv文件中逗号分隔符字符串是否存在空项（两个逗号之间无字符）
* @param     :伺服csv文件中一行字符串,一行有五项数据
* @return    :true 完整  false 不完整
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
BOOL FiveCommon(const char * string){
    int num;
    const char *s1, *s2;
    num = 0;
    s1 = string;
    s2 = s1;
    if(*s1 == ',')
        return FALSE;
    while ((s1 = strchr(s1, ',')) != NULL) {
        s2 = s1;
        if(*(++s2) == ',')
            return FALSE;
        ++s1;
        num++;
    }
    if(num == 5)
        return TRUE;
    return FALSE;
}

/**
* @brief     :从伺服csv文件中加载数据
* @param     : 文件名，操作类型
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
UI16 LoadDriveData(char filename[],int type)
{
    UI8* pdst;
    PRO_SDO_RX_LOG* tbDrivePar;
    UI16 datarecord[NUM_COLUMNS_MAXCOUNT][NUM_ROWS_MAXCOUNT];
    BOOL bDriveFirstRow = TRUE;//当前读取驱动器表首行
    char filePath[100];
    char *line,* drivedata;
    char buffer[1024];

    int row = 0;
    int column= 0;
    int i =0;

    memset(buffer, 0, sizeof(buffer));
    memset(filePath, 0, sizeof(filePath));
    sprintf(filePath,"%s%s",filename,".csv");

    FILE* stream;

    if(!FileExist(filePath))
    {
       return 0;
    }
    else
    {
        stream = fopen(filePath,"r");
        oprintf("open file successful\n");
    }

    //[驱动器编号],[驱动器数据长度],[驱动器功能码],[驱动器地址],[驱动器数据]
    while((line = fgets(buffer, sizeof(buffer), stream)) != NULL)//当没有读取到文件末尾时循环继续
    {  

        if(bDriveFirstRow)//去掉第一行中文
        {
            bDriveFirstRow = FALSE;
        }
        else
        {
            if(!FiveCommon(line))
                return 2;
            drivedata = strtok(buffer,",");//以逗号分隔

            while(drivedata != NULL)//读取每一列的数据
            {
               if(column == 2 || column == 3)//当数据为驱动器功能码驱动器地址，值为16进制值
               {
                   datarecord[column][row] = StrToWord(drivedata,HEXFLAG);
               }
               else
               {
                   datarecord[column][row] = StrToWord(drivedata,0);
               }
               if(column == 4)//只需要读取到第四列
               {
                   break;
               }
               //下一列参数值
               drivedata = strtok(NULL, ",");
               column++;
            }
            row++;
            column = 0;
            if(row >= NUM_ROWS_MAXCOUNT)
            {
                break;
            }
        }
    }

    fclose(stream);

    s_wCount = row;//得到当前驱动器行数


    pdst = OBJMemMalloc((row) * sizeof(PRO_SDO_RX_LOG));

    tbDrivePar = (PRO_SDO_RX_LOG*)pdst;//指向第一行数据首地址

    //type--1:读数据 2:写数据
    if (type == 1)//向下位机请求读数据
    {
        for (i=0; i<row; i++,tbDrivePar ++)
        {
            tbDrivePar->servo_sn = datarecord[0][i];
            tbDrivePar->servo_len = datarecord[1][i];
            tbDrivePar->servo_func = datarecord[2][i];
            tbDrivePar->servo_addr = datarecord[3][i];
            tbDrivePar->servo_val= 0;
        }
    }
    else//向下位机请求写数据
    {
        for (i=0; i<row; i++,tbDrivePar ++)
        {
            tbDrivePar->servo_sn = datarecord[0][i];
            tbDrivePar->servo_len = datarecord[1][i];
            tbDrivePar->servo_func = datarecord[2][i];
            tbDrivePar->servo_addr = datarecord[3][i];
            tbDrivePar->servo_val = datarecord[4][i];
        }
    }
    AddDriveTable(type,(PRO_SDO_RX_LOG*)pdst);
    return 1;
}

BOOL LoadDriveData_Local(char filename[],int type,UI16 wSelSn)
{
    int i;
    UI8 *pdst;
    PRO_SDO_RX_LOG* tbDrivePar;
    int allrow = 0;
    char filePath[100];
    BOOL bDriveFirstRow;//当前刚读取驱动器表首行
    UI16 datarecord[NUM_COLUMNS_MAXCOUNT][NUM_ROWS_LOCAL_MAXCOUNT];
    int column= 0;
    char buffer[1024];
    char *drivedata;

    for (i=0; i<MAX_DRIVECNT; i++)
    {
        wDriveRow[i] = 0;
    }

    for (i=0; i<MAX_DRIVECNT; i++)
    {
        bDriveFirstRow = TRUE;

        if ((1<<i) & wSelSn)
        {
             memset(filePath, 0, sizeof(filePath));
             memset(buffer, 0, sizeof(buffer));
             sprintf(filePath,"%s%d%s",filename,i+1,".csv");

             FILE* stream;

             if(!FileExist(filePath))
             {
                return FALSE;
             }
             else
             {
                stream = fopen(filePath,"r");
                oprintf("open file successful");
             }
             while((fgets(buffer, sizeof(buffer), stream))!=NULL)//当没有读取到文件末尾时循环继续
             {
                 if(bDriveFirstRow)
                 {
                     bDriveFirstRow = FALSE;
                 }
                 else
                 {
                     drivedata = strtok(buffer, ",");
                     while(drivedata != NULL)//读取每一行的数据
                     {
                         if(column == 2 || column == 3)
                         {
                             datarecord[column][allrow] = StrToWord(drivedata,HEXFLAG);
                         }
                         else
                         {
                             datarecord[column][allrow] = StrToWord(drivedata,0);
                         }

                         if(column == 4)
                         {
                             break;
                         }
                         column++;
                         drivedata = strtok(NULL, ",");
                     }
                     wDriveRow[i]++; //每个驱动器有几行
                     allrow++;//所有驱动器共有几行
                     column = 0;
                     if(allrow >= NUM_ROWS_LOCAL_MAXCOUNT)
                     {
                         break;
                     }
                 }
             }
             fclose(stream);
         }
    }
    s_wCount = allrow;
    pdst = OBJMemMalloc((allrow) * sizeof(PRO_SDO_RX_LOG));
    tbDrivePar = (PRO_SDO_RX_LOG*)pdst;
    //type--1:读数据 2:写数据
    if (type == READDRIVEPAR)//向下位机请求读数据
    {
        for (i = 0; i<allrow; i++,tbDrivePar ++)
        {
            tbDrivePar->servo_sn = datarecord[0][i];
            tbDrivePar->servo_len = datarecord[1][i];
            tbDrivePar->servo_func = datarecord[2][i];
            tbDrivePar->servo_addr = datarecord[3][i];
            tbDrivePar->servo_val = 0;

        }
    }
    else//向下位机请求写数据
    {
        for (i = 0; i<allrow; i++,tbDrivePar ++)
        {
            tbDrivePar->servo_sn = datarecord[0][i];
            tbDrivePar->servo_len = datarecord[1][i];
            tbDrivePar->servo_func = datarecord[2][i];
            tbDrivePar->servo_addr = datarecord[3][i];
            tbDrivePar->servo_val = datarecord[4][i];
        }
    }
    AddDriveTable(type,(PRO_SDO_RX_LOG*)pdst);
    return TRUE;
}


/*******************以下为伺服操作功能新加***************************/ //20190926 YX

void RemoveDriveTable(int tbltype)
{
    if(g_type == NULL)//是否需要判断第一遍哈希表为空时，直接返回
        return;

    if(HashTblRemoveByInt(g_type,tbltype))
    {
        oprintf("remove success");
    }
    else
    {
        oprintf("remove fail");
    }

    return;
}

void AddDriveTable(int type,PRO_SDO_RX_LOG *ptableDrive)
{

    RemoveDriveTable(type);

    if(g_type == NULL)
    {
        g_type = HashTblAlloc(KEY_MAXCOUNTX);
        ADD_PACK(type, ptableDrive);
    }
    else
    {
        ADD_PACK(type, ptableDrive);
    }
}

/*********************************end***************************/

PRO_SDO_RX_LOG*  FindDriveTable(int type)
{

    PRO_SDO_RX_LOG* ptable = HashTblFindPairByInt(g_type,type);

    return ptable;
}

