#include "servoconfig.h"
#include "oprintf.h"
#include "file.h"
#include "stdio.h"
#include "language.h"

#define MAX_READ_COLUMN 11 //单行读取数据个数
#define MAX_READ_ROW    5000 //单独一个.csv文件读取的最大行数
#define MAX_SELECT_READ_COLUMN 4 //伺服配置SELECT表单行读取数据个数
#define MAX_SELECT_NUM  2000 //伺服配置SELECT表最大行数

PSERVOCONFIG H00_PARA = NULL;//H00参数数据存放指针
PSERVOCONFIG H01_PARA = NULL;//H01参数数据存放指针
PSERVOCONFIG H02_PARA = NULL;//H02参数数据存放指针
PSERVOCONFIG H03_PARA = NULL;//H03参数数据存放指针
PSERVOCONFIG H04_PARA = NULL;//H04参数数据存放指针
PSERVOCONFIG H05_PARA = NULL;//H05参数数据存放指针
PSERVOCONFIG H06_PARA = NULL;//H06参数数据存放指针
PSERVOCONFIG H07_PARA = NULL;//H07参数数据存放指针
PSERVOCONFIG H08_PARA = NULL;//H08参数数据存放指针
PSERVOCONFIG H09_PARA = NULL;//H09参数数据存放指针
PSERVOCONFIG H0A_PARA = NULL;//H0A参数数据存放指针
PSERVOCONFIG H0B_PARA = NULL;//H0B参数数据存放指针
PSERVOCONFIG H0C_PARA = NULL;//H0C参数数据存放指针
PSERVOCONFIG H0D_PARA = NULL;//H0D参数数据存放指针
PSERVOCONFIG H17_PARA = NULL;//H17参数数据存放指针
UI16 H00_PARA_NUM = 0;//H00参数数量
UI16 H01_PARA_NUM = 0;//H01参数数量
UI16 H02_PARA_NUM = 0;//H02参数数量
UI16 H03_PARA_NUM = 0;//H03参数数量
UI16 H04_PARA_NUM = 0;//H04参数数量
UI16 H05_PARA_NUM = 0;//H05参数数量
UI16 H06_PARA_NUM = 0;//H06参数数量
UI16 H07_PARA_NUM = 0;//H07参数数量
UI16 H08_PARA_NUM = 0;//H08参数数量
UI16 H09_PARA_NUM = 0;//H09参数数量
UI16 H0A_PARA_NUM = 0;//H0A参数数量
UI16 H0B_PARA_NUM = 0;//H0B参数数量
UI16 H0C_PARA_NUM = 0;//H0C参数数量
UI16 H0D_PARA_NUM = 0;//H0D参数数量
UI16 H17_PARA_NUM = 0;//H17参数数量

PHASH_TBL Servo_Config_Select = NULL;//伺服配置选择表
PCONFIGSELECT CONFIG_SELECT_VALUE = NULL;//伺服配置选择表value值
UI16 CONFIG_SELECT_NUM = 0;//伺服配置选择表行数

static LANG_ID servoconfig_lang = LANG_ERR;//记录当前读取文件语言

/**
 * @brief      : 解析驱动器配置SELECT表csv格式文件
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200722
 */
static BOOL LoadServoConfig_Select(char filename[])
{
    char buffer[1024];
    char *line,*celldata;

    int row = 0;
    int column= 0;
    char key[128];
    CONFIGSELECT* new_node = NULL;
    CONFIGSELECT* iter = NULL;

    FILE* stream;
    memset(buffer, 0, sizeof(buffer));

    if(!FileExist(filename))
    {
        oprintf("%s is not exist\n",filename);
        return FALSE;
    }
    else
    {
        stream = fopen(filename,"r");
        oprintf("open %s successful\n",filename);
    }

    if(Servo_Config_Select == NULL)
    {
        Servo_Config_Select = HashTblAlloc(MAX_SELECT_NUM);
    }

    while((line = fgets(buffer, sizeof(buffer), stream)) != NULL)//当没有读取到文件末尾时循环继续
    {
        celldata = strtok(buffer,",");//以逗号分隔

        while(celldata != NULL && row > 0)//读取每一列的数据,第一行数据不读取
        {
            if(column == 1)//NUM
            {
                new_node = OBJMemMalloc(sizeof(CONFIGSELECT));
                new_node->pnext = NULL;
                snprintf(key,sizeof(key),"%s",celldata);
            }
            else if(column == 2)//VALUE
            {
                strcat(key, "-");
                strcat(key, celldata);
            }
            else if(column == 3)//DESCRIPTION
            {
                snprintf(new_node->desc,sizeof(new_node->desc),"%s",celldata);
                if(new_node->desc[strlen(new_node->desc)-1] == '\n')//去除末尾换行符
                {
                    new_node->desc[strlen(new_node->desc)-1] = '\0';
                }
                if(new_node->desc[strlen(new_node->desc)-1] == '\r')//去除末尾换行符
                {
                    new_node->desc[strlen(new_node->desc)-1] = '\0';
                }
            }

            if(++column >= MAX_SELECT_READ_COLUMN)//只需要读取到第MAX_SELECT_READ_COLUMN列
            {
                CONFIG_SELECT_NUM++;
                if(CONFIG_SELECT_VALUE == NULL)
                {
                    CONFIG_SELECT_VALUE = new_node;
                    CONFIG_SELECT_VALUE->pnext = NULL;
                }
                else
                {
                    iter = CONFIG_SELECT_VALUE;
                    while (iter != NULL)
                    {
                        if(iter->pnext == NULL)
                        {
                            iter->pnext = new_node;
                            break;
                        }
                        iter = iter->pnext;
                    }
                }
                HashTblAddPair(Servo_Config_Select, key, new_node->desc);

                break;
            }
            //下一列参数值
            celldata = strtok(NULL, ",");
        }
        row++;
        column = 0;
        if(row >= MAX_SELECT_NUM)
        {
            break;
        }
    }

    fclose(stream);

    return TRUE;
}

/**
 * @brief      : H参数类型判断
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200722
 */
static SERVOCONFIG_DATATYPE DataTypeJudge(char* num)
{
    if(strcmp(num,"H00") == 0)
    {
        return TYPE_H00;
    }
    else if(strcmp(num,"H01") == 0)
    {
        return TYPE_H01;
    }
    else if(strcmp(num,"H02") == 0)
    {
        return TYPE_H02;
    }
    else if(strcmp(num,"H03") == 0)
    {
        return TYPE_H03;
    }
    else if(strcmp(num,"H04") == 0)
    {
        return TYPE_H04;
    }
    else if(strcmp(num,"H05") == 0)
    {
        return TYPE_H05;
    }
    else if(strcmp(num,"H06") == 0)
    {
        return TYPE_H06;
    }
    else if(strcmp(num,"H07") == 0)
    {
        return TYPE_H07;
    }
    else if(strcmp(num,"H08") == 0)
    {
        return TYPE_H08;
    }
    else if(strcmp(num,"H09") == 0)
    {
        return TYPE_H09;
    }
    else if(strcmp(num,"H0A") == 0)
    {
        return TYPE_H0A;
    }
    else if(strcmp(num,"H0B") == 0)
    {
        return TYPE_H0B;
    }
    else if(strcmp(num,"H0C") == 0)
    {
        return TYPE_H0C;
    }
    else if(strcmp(num,"H0D") == 0)
    {
        return TYPE_H0D;
    }
    else if(strcmp(num,"H17") == 0)
    {
        return TYPE_H17;
    }

    return SERVOCONFIG_DATATYPE_END;
}

/**
 * @brief      : 伺服配置数据读入
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200722
 */
static void ServeConfigDataInput(SERVOCONFIG_DATATYPE data_type, SERVOCONFIG* new_node)
{
    SERVOCONFIG* iter = NULL;

    if(data_type == TYPE_H00)
    {
        H00_PARA_NUM++;
        if(H00_PARA == NULL)
        {
            H00_PARA = new_node;
            H00_PARA->pnext = NULL;
        }
        else
        {
            iter = H00_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H01)
    {
        H01_PARA_NUM++;
        if(H01_PARA == NULL)
        {
            H01_PARA = new_node;
            H01_PARA->pnext = NULL;
        }
        else
        {
            iter = H01_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H02)
    {
        H02_PARA_NUM++;
        if(H02_PARA == NULL)
        {
            H02_PARA = new_node;
            H02_PARA->pnext = NULL;
        }
        else
        {
            iter = H02_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H03)
    {
        H03_PARA_NUM++;
        if(H03_PARA == NULL)
        {
            H03_PARA = new_node;
            H03_PARA->pnext = NULL;
        }
        else
        {
            iter = H03_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H04)
    {
        H04_PARA_NUM++;
        if(H04_PARA == NULL)
        {
            H04_PARA = new_node;
            H04_PARA->pnext = NULL;
        }
        else
        {
            iter = H04_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H05)
    {
        H05_PARA_NUM++;
        if(H05_PARA == NULL)
        {
            H05_PARA = new_node;
            H05_PARA->pnext = NULL;
        }
        else
        {
            iter = H05_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H06)
    {
        H06_PARA_NUM++;
        if(H06_PARA == NULL)
        {
            H06_PARA = new_node;
            H06_PARA->pnext = NULL;
        }
        else
        {
            iter = H06_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H07)
    {
        H07_PARA_NUM++;
        if(H07_PARA == NULL)
        {
            H07_PARA = new_node;
            H07_PARA->pnext = NULL;
        }
        else
        {
            iter = H07_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H08)
    {
        H08_PARA_NUM++;
        if(H08_PARA == NULL)
        {
            H08_PARA = new_node;
            H08_PARA->pnext = NULL;
        }
        else
        {
            iter = H08_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H09)
    {
        H09_PARA_NUM++;
        if(H09_PARA == NULL)
        {
            H09_PARA = new_node;
            H09_PARA->pnext = NULL;
        }
        else
        {
            iter = H09_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H0A)
    {
        H0A_PARA_NUM++;
        if(H0A_PARA == NULL)
        {
            H0A_PARA = new_node;
            H0A_PARA->pnext = NULL;
        }
        else
        {
            iter = H0A_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H0B)
    {
        H0B_PARA_NUM++;
        if(H0B_PARA == NULL)
        {
            H0B_PARA = new_node;
            H0B_PARA->pnext = NULL;
        }
        else
        {
            iter = H0B_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H0C)
    {
        H0C_PARA_NUM++;
        if(H0C_PARA == NULL)
        {
            H0C_PARA = new_node;
            H0C_PARA->pnext = NULL;
        }
        else
        {
            iter = H0C_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H0D)
    {
        H0D_PARA_NUM++;
        if(H0D_PARA == NULL)
        {
            H0D_PARA = new_node;
            H0D_PARA->pnext = NULL;
        }
        else
        {
            iter = H0D_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
    else if(data_type == TYPE_H17)
    {
        H17_PARA_NUM++;
        if(H17_PARA == NULL)
        {
            H17_PARA = new_node;
            H17_PARA->pnext = NULL;
        }
        else
        {
            iter = H17_PARA;
            while (iter != NULL)
            {
                if(iter->pnext == NULL)
                {
                    iter->pnext = new_node;
                    break;
                }
                iter = iter->pnext;
            }
        }
    }
}

/**
 * @brief      : 解析驱动器配置参数csv格式文件
 * @param      : 文件名
 * @return     : 文件不存在返回FALSE
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200721
 */
BOOL LoadServoConfig()
{
    char buffer[1024];
    char *line,*celldata;
    char num[4];
    char filename[128];
    char fileselectname[128];

    int row = 0;
    int column= 0;
    SERVOCONFIG_DATATYPE data_type = SERVOCONFIG_DATATYPE_END;//数据类型 1-H00 2-H01...
    SERVOCONFIG* new_node = NULL;

    FILE* stream;
    memset(buffer, 0, sizeof(buffer));

    //加载要读取的文件名 如已读取则返回 未读取则清空内存后再读取
    if(GetCurrentLanguage() == LANG_zh_CN)
    {
        if(servoconfig_lang == LANG_zh_CN)
        {
            return FALSE;
        }
        else
        {
            if(servoconfig_lang != LANG_ERR)
            {
                ClearHPara();
            }
            servoconfig_lang = LANG_zh_CN;
            snprintf(filename, sizeof(filename), "%s", SERVOCONFIG_FILE_CH_NAME);
            snprintf(fileselectname, sizeof(fileselectname), "%s", SERVOCONFIG_FILE_SELECT_CH_NAME);
        }
    }
    else
    {
        if(servoconfig_lang == LANG_en)
        {
            return FALSE;
        }
        else
        {
            if(servoconfig_lang != LANG_ERR)
            {
                ClearHPara();
            }
            servoconfig_lang = LANG_en;
            snprintf(filename, sizeof(filename), "%s", SERVOCONFIG_FILE_EN_NAME);
            snprintf(fileselectname, sizeof(fileselectname), "%s", SERVOCONFIG_FILE_SELECT_EN_NAME);
        }
    }

    if(!FileExist(filename))
    {
        oprintf("%s is not exist\n",filename);
        return FALSE;
    }
    else
    {
        LoadServoConfig_Select(fileselectname);
        stream = fopen(filename,"r");
        oprintf("open %s successful\n",filename);
    }

    while((line = fgets(buffer, sizeof(buffer), stream)) != NULL)//当没有读取到文件末尾时循环继续
    {
        celldata = strtok(buffer,",");//以逗号分隔

        while(celldata != NULL)//读取每一列的数据
        {
            if(column == 0)
            {
                snprintf(num,sizeof(num),"%s",celldata);//截取H0000->H00

                //GroupNO judge
                data_type = DataTypeJudge(num);
            }

            if(data_type != SERVOCONFIG_DATATYPE_END)
            {
                if(column == 0)//参考编码
                {
                    new_node = OBJMemMalloc(sizeof(SERVOCONFIG));
                    new_node->pnext = NULL;
                    snprintf(new_node->num,sizeof(new_node->num),"%s",celldata);
                }
                else if(column == 2)//参考名称
                {
                    snprintf(new_node->name,sizeof(new_node->name),"%s",celldata);
                }
                else if(column == 4)//参考值
                {
                    new_node->curvalue = StrToWord(celldata, 0);
                }
                else if(column == 5)//出厂值
                {
                    new_node->defvalue = StrToWord(celldata, 0);
                }
                else if(column == 8)//单位
                {
                    snprintf(new_node->unit,sizeof(new_node->unit),"%s",celldata);
                }
                else if(column == 10)//长度
                {
                    new_node->width = StrToWord(celldata, 0);
                }
            }

            if(++column >= MAX_READ_COLUMN)//只需要读取到第MAX_READ_COLUMN列
            {
                if(data_type != SERVOCONFIG_DATATYPE_END)
                {
                    //数据存入结构体指针
                    ServeConfigDataInput(data_type, new_node);
                }

                break;
            }
            //下一列参数值
            celldata = strtok(NULL, ",");
        }
        row++;
        data_type = 0;
        column = 0;
        if(row >= MAX_READ_ROW)
        {
            break;
        }
    }

    fclose(stream);

    return TRUE;
}

/**
 * @brief      : 获取指定类型的H参数个数
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200721
 */
UI16 GetHParaNum(int data_type)
{
    if(data_type == TYPE_H00)
    {
        return H00_PARA_NUM;
    }
    else if(data_type == TYPE_H01)
    {
        return H01_PARA_NUM;
    }
    else if(data_type == TYPE_H02)
    {
        return H02_PARA_NUM;
    }
    else if(data_type == TYPE_H03)
    {
        return H03_PARA_NUM;
    }
    else if(data_type == TYPE_H04)
    {
        return H04_PARA_NUM;
    }
    else if(data_type == TYPE_H05)
    {
        return H05_PARA_NUM;
    }
    else if(data_type == TYPE_H06)
    {
        return H06_PARA_NUM;
    }
    else if(data_type == TYPE_H07)
    {
        return H07_PARA_NUM;
    }
    else if(data_type == TYPE_H08)
    {
        return H08_PARA_NUM;
    }
    else if(data_type == TYPE_H09)
    {
        return H09_PARA_NUM;
    }
    else if(data_type == TYPE_H0A)
    {
        return H0A_PARA_NUM;
    }
    else if(data_type == TYPE_H0B)
    {
        return H0B_PARA_NUM;
    }
    else if(data_type == TYPE_H0C)
    {
        return H0C_PARA_NUM;
    }
    else if(data_type == TYPE_H0D)
    {
        return H0D_PARA_NUM;
    }
    else if(data_type == TYPE_H17)
    {
        return H17_PARA_NUM;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      : 获取指定类型指定参数指定id的数据的值
 * @param      : data_type-H参数类型 data_name-结构体成员变量名
 *               data_id-第几个数据 buff-非字符串成员需要存放在buff里
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200721
 */
char* GetHPara(int data_type, char* data_name, UI16 data_id, char* buff)
{
    SERVOCONFIG* iter = NULL;
    UI16 iter_num = 0;
    int i;
    char key[128];

    if(data_type == TYPE_H00)
    {
        iter = H00_PARA;
        iter_num = H00_PARA_NUM;
    }
    else if(data_type == TYPE_H01)
    {
        iter = H01_PARA;
        iter_num = H01_PARA_NUM;
    }
    else if(data_type == TYPE_H02)
    {
        iter = H02_PARA;
        iter_num = H02_PARA_NUM;
    }
    else if(data_type == TYPE_H03)
    {
        iter = H03_PARA;
        iter_num = H03_PARA_NUM;
    }
    else if(data_type == TYPE_H04)
    {
        iter = H04_PARA;
        iter_num = H04_PARA_NUM;
    }
    else if(data_type == TYPE_H05)
    {
        iter = H05_PARA;
        iter_num = H05_PARA_NUM;
    }
    else if(data_type == TYPE_H06)
    {
        iter = H06_PARA;
        iter_num = H06_PARA_NUM;
    }
    else if(data_type == TYPE_H07)
    {
        iter = H07_PARA;
        iter_num = H07_PARA_NUM;
    }
    else if(data_type == TYPE_H08)
    {
        iter = H08_PARA;
        iter_num = H08_PARA_NUM;
    }
    else if(data_type == TYPE_H09)
    {
        iter = H09_PARA;
        iter_num = H09_PARA_NUM;
    }
    else if(data_type == TYPE_H0A)
    {
        iter = H0A_PARA;
        iter_num = H0A_PARA_NUM;
    }
    else if(data_type == TYPE_H0B)
    {
        iter = H0B_PARA;
        iter_num = H0B_PARA_NUM;
    }
    else if(data_type == TYPE_H0C)
    {
        iter = H0C_PARA;
        iter_num = H0C_PARA_NUM;
    }
    else if(data_type == TYPE_H0D)
    {
        iter = H0D_PARA;
        iter_num = H0D_PARA_NUM;
    }
    else if(data_type == TYPE_H17)
    {
        iter = H17_PARA;
        iter_num = H17_PARA_NUM;
    }
    else
    {
        oprintf("error data_type\n");
        return NULL;
    }

    if(data_id > iter_num)
    {
        oprintf("data_id too large\n");
        return NULL;
    }
    else if(iter == NULL)
    {
        return NULL;
    }

    //获取指定id的指针
    for(i = 1; i < data_id; i++)
    {
        iter = iter->pnext;
    }

    //返回请求的数据
    if(strcmp(data_name,"num") == 0)
    {
        return iter->num;
    }
    else if(strcmp(data_name,"name") == 0)
    {
        return iter->name;
    }
    else if(strcmp(data_name,"curvalue") == 0)
    {
        snprintf(key, sizeof(key), "%s-%d", iter->num, iter->curvalue);
        if(HashTblFindPair(Servo_Config_Select, key) != NULL)
        {
            return HashTblFindPair(Servo_Config_Select, key);
        }
        else
        {
            if(buff == NULL)
            {
                return NULL;
            }
            WordToStr(buff,iter->curvalue,0);
            return buff;
        }
    }
    else if(strcmp(data_name,"defvalue") == 0)
    {
        if(buff == NULL)
        {
            return NULL;
        }
        WordToStr(buff,iter->defvalue,0);
        return buff;
    }
    else if(strcmp(data_name,"unit") == 0)
    {
        return iter->unit;
    }
    else if(strcmp(data_name,"width") == 0)
    {
        if(buff == NULL)
        {
            return NULL;
        }
        WordToStr(buff,iter->width,0);
        return buff;
    }
    else
    {
        oprintf("error data_name\n");
        return NULL;
    }

    return NULL;
}

/**
 * @brief      : 清除保存的SELECT表
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200722
 */
void ClearConfigSelect()
{
    CONFIGSELECT* iter = NULL;
    CONFIGSELECT* temp = NULL;

    if(Servo_Config_Select != NULL)
    {
        OBJMemFree(Servo_Config_Select);
        Servo_Config_Select = NULL;
    }

    for(iter = CONFIG_SELECT_VALUE; CONFIG_SELECT_NUM > 0; CONFIG_SELECT_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    CONFIG_SELECT_VALUE = NULL;
}

/**
 * @brief      : 清除保存的H参数
 * @param      :
 * @return     :
 * @retval     :
 * @note       : 在语言切换的时候需先释放原先的表再加载另一个文件
 * @attention  :
 * @author     : cyx
 * @date       : 20200721
 */
void ClearHPara()
{
    SERVOCONFIG* iter = NULL;
    SERVOCONFIG* temp = NULL;

    ClearConfigSelect();//清空参数前先清空选择表

    for(iter = H00_PARA; H00_PARA_NUM > 0; H00_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H01_PARA; H01_PARA_NUM > 0; H01_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H02_PARA; H02_PARA_NUM > 0; H02_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H03_PARA; H03_PARA_NUM > 0; H03_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H04_PARA; H04_PARA_NUM > 0; H04_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H05_PARA; H05_PARA_NUM > 0; H05_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H06_PARA; H06_PARA_NUM > 0; H06_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H07_PARA; H07_PARA_NUM > 0; H07_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H08_PARA; H08_PARA_NUM > 0; H08_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H09_PARA; H09_PARA_NUM > 0; H09_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H0A_PARA; H0A_PARA_NUM > 0; H0A_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H0B_PARA; H0B_PARA_NUM > 0; H0B_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H0C_PARA; H0C_PARA_NUM > 0; H0C_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H0D_PARA; H0D_PARA_NUM > 0; H0D_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    for(iter = H17_PARA; H17_PARA_NUM > 0; H17_PARA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    H00_PARA = NULL;
    H01_PARA = NULL;
    H02_PARA = NULL;
    H03_PARA = NULL;
    H04_PARA = NULL;
    H05_PARA = NULL;
    H06_PARA = NULL;
    H07_PARA = NULL;
    H08_PARA = NULL;
    H09_PARA = NULL;
    H0A_PARA = NULL;
    H0B_PARA = NULL;
    H0C_PARA = NULL;
    H0D_PARA = NULL;
    H17_PARA = NULL;
}

/**
 * @brief      : 设置参考值为当前值
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200729
 */
void SetHPara_CurValue(int data_type, UI16 data_id, UI32 servo_val)
{
    SERVOCONFIG* iter = NULL;
    UI16 iter_num = 0;
    int i;

    if(data_type == TYPE_H00)
    {
        iter = H00_PARA;
        iter_num = H00_PARA_NUM;
    }
    else if(data_type == TYPE_H01)
    {
        iter = H01_PARA;
        iter_num = H01_PARA_NUM;
    }
    else if(data_type == TYPE_H02)
    {
        iter = H02_PARA;
        iter_num = H02_PARA_NUM;
    }
    else if(data_type == TYPE_H03)
    {
        iter = H03_PARA;
        iter_num = H03_PARA_NUM;
    }
    else if(data_type == TYPE_H04)
    {
        iter = H04_PARA;
        iter_num = H04_PARA_NUM;
    }
    else if(data_type == TYPE_H05)
    {
        iter = H05_PARA;
        iter_num = H05_PARA_NUM;
    }
    else if(data_type == TYPE_H06)
    {
        iter = H06_PARA;
        iter_num = H06_PARA_NUM;
    }
    else if(data_type == TYPE_H07)
    {
        iter = H07_PARA;
        iter_num = H07_PARA_NUM;
    }
    else if(data_type == TYPE_H08)
    {
        iter = H08_PARA;
        iter_num = H08_PARA_NUM;
    }
    else if(data_type == TYPE_H09)
    {
        iter = H09_PARA;
        iter_num = H09_PARA_NUM;
    }
    else if(data_type == TYPE_H0A)
    {
        iter = H0A_PARA;
        iter_num = H0A_PARA_NUM;
    }
    else if(data_type == TYPE_H0B)
    {
        iter = H0B_PARA;
        iter_num = H0B_PARA_NUM;
    }
    else if(data_type == TYPE_H0C)
    {
        iter = H0C_PARA;
        iter_num = H0C_PARA_NUM;
    }
    else if(data_type == TYPE_H0D)
    {
        iter = H0D_PARA;
        iter_num = H0D_PARA_NUM;
    }
    else if(data_type == TYPE_H17)
    {
        iter = H17_PARA;
        iter_num = H17_PARA_NUM;
    }
    else
    {
        oprintf("error data_type\n");
        return;
    }

    if(data_id > iter_num)
    {
        oprintf("data_id too large\n");
        return;
    }
    else if(iter == NULL)
    {
        return;
    }

    //获取指定id的指针
    for(i = 1; i < data_id; i++)
    {
        iter = iter->pnext;
    }

    //设置数据
    iter->curvalue = servo_val;
}
