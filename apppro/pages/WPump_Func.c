#include "WPump_Func.h"
#include "typedef.h"
#include "pagefram.h"
#include "pumpfunc.h"
#include "tablectl.h"
#include "oprintf.h"
#include "tstring.h"
#include "privdef.h"
#include "questionDlg.h"
#include "keymod.h"
#include "key.h"
#include "database.h"
#include "usermod.h"
#include "information.h"

DEFINE_PUMP_FUNC_MAP

CONNECT_PUMP_FUNC_EVENT

PPUMP_INFO ppumpfos[1];
#define INRANGE(_x, _min, _max) (((_x) >= (_min) ? ((_x) <= (_max) ? 1 : 0) : 0))

int PUMPFUNC_CHANGE_FLAG; //表数据是否有改动的标志位
extern UI16 g_pumpUpdateFlag; //是否收到下位机发送的pump表

#define ID_IN  0
#define MIN_ALLOC_SIZE 512
#define TMP_FIRST_DATA  TMPVAL(100) //主泵、从泵数据关联的临时变量
#define FIRST_DATA_LENGT      8 //数据一行的个数

#define TMP_UNIT_DATA   TMPVAL(30) //部件数据关联的临时变量
#define TMP_ACT_DATA    TMPVAL(50) //动作数据关联的临时变量
#define TMP_NUM_TABLE    TMPVAL(5) //第几张表关联的临时变量
#define MAX_TABLE_NUM         3 //联动油路最大表数目

#define TMP_HYDR_LINK_OPNCHG    TMPVAL(6) //开模储料联动油路表是否可见和使用
#define TMP_HYDR_LINK_OPNEJT    TMPVAL(7) //开模托模联动油路 是否可见和使用
#define TMP_HYDR_LINK_OPNCORE    TMPVAL(8) //开模中子联动油路表是否可见和使用

#define MAX_PUMPLENTH    12 //PUMP算法一条数据长度 12BYTE=6UI16
#define MAX_PUMPNUM      20 //PUMP算法一页的最大行数
#define	GET_LOW_BIT(x,y)	((x >>  y) & 0x00000001)	/* 获取第y个bit */

static CUS_PORT wPumpData[MAX_PUMPNUM*3]; //存储在本地的表数据
static CUS_PORT wPumpData_send[MAX_PUMPNUM*3+1]; //从wPumpData获取数据，并复制到发送的表中
                                                //20250115 fqh （+1是因为在空表的情况下，与下位机约定好发送一条全0的数据）
static void UpdateDef(PPUMP_INFO ppumpfo, int index, UI32 addr);
static int GetPumpData();
static int ShowlocalPumpData();

static QUESTION g_question;

/* 20241223 fqh
 * 恢复为默认值（暂时未发送给下位机，点击保存后才会发送给下位机）
 */
static void DataRestoreToDefault()
{
    CUS_PORT wPumpDataDefault1[3],wPumpDataDefault2[3],wPumpDataDefault3[5]; //表一，表二，表三默认数据
    wPumpDataDefault1[0].hydr_type = 1;
    wPumpDataDefault1[0].hydr_sn = 1;
    wPumpDataDefault1[0].part_id = 0x1010;
    wPumpDataDefault1[0].act_id = 0x0002;
    wPumpDataDefault1[0].port = 0b0010;
    wPumpDataDefault1[0].pump = 0x0000;
    wPumpDataDefault1[0].act_flag = 0;
    wPumpDataDefault1[1].hydr_type = 1;
    wPumpDataDefault1[1].hydr_sn = 2;
    wPumpDataDefault1[1].part_id = 0x1010;
    wPumpDataDefault1[1].act_id = 0x0003;
    wPumpDataDefault1[1].port = 0b0010;
    wPumpDataDefault1[1].pump = 0x0000;
    wPumpDataDefault1[1].act_flag = 0;
    wPumpDataDefault1[2].hydr_type = 1;
    wPumpDataDefault1[2].hydr_sn = 3;
    wPumpDataDefault1[2].part_id = 0x2010;
    wPumpDataDefault1[2].act_id = 0x0003;
    wPumpDataDefault1[2].port = 0b0001;
    wPumpDataDefault1[2].pump = 0x0000;
    wPumpDataDefault1[2].act_flag = 0;

    wPumpDataDefault2[0].hydr_type = 2;
    wPumpDataDefault2[0].hydr_sn = 1;
    wPumpDataDefault2[0].part_id = 0x2010;
    wPumpDataDefault2[0].act_id = 0x0003;
    wPumpDataDefault2[0].port = 0b0001;
    wPumpDataDefault2[0].pump = 0x0000;
    wPumpDataDefault2[0].act_flag = 0;
    wPumpDataDefault2[1].hydr_type = 2;
    wPumpDataDefault2[1].hydr_sn = 2;
    wPumpDataDefault2[1].part_id = 0x2110;
    wPumpDataDefault2[1].act_id = 0x0002;
    wPumpDataDefault2[1].port = 0b0010;
    wPumpDataDefault2[1].pump = 0x0000;
    wPumpDataDefault2[1].act_flag = 0;
    wPumpDataDefault2[2].hydr_type = 2;
    wPumpDataDefault2[2].hydr_sn = 3;
    wPumpDataDefault2[2].part_id = 0x2110;
    wPumpDataDefault2[2].act_id = 0x0003;
    wPumpDataDefault2[2].port = 0b0010;
    wPumpDataDefault2[2].pump = 0x0000;
    wPumpDataDefault2[2].act_flag = 0;

    wPumpDataDefault3[0].hydr_type = 3;
    wPumpDataDefault3[0].hydr_sn = 1;
    wPumpDataDefault3[0].part_id = 0x2010;
    wPumpDataDefault3[0].act_id = 0x0003;
    wPumpDataDefault3[0].port = 0b0001;
    wPumpDataDefault3[0].pump = 0x0000;
    wPumpDataDefault3[0].act_flag = 0;
    wPumpDataDefault3[1].hydr_type = 3;
    wPumpDataDefault3[1].hydr_sn = 2;
    wPumpDataDefault3[1].part_id = 0x2110;
    wPumpDataDefault3[1].act_id = 0x0002;
    wPumpDataDefault3[1].port = 0b0010;
    wPumpDataDefault3[1].pump = 0x0000;
    wPumpDataDefault3[1].act_flag = 0;
    wPumpDataDefault3[2].hydr_type = 3;
    wPumpDataDefault3[2].hydr_sn = 3;
    wPumpDataDefault3[2].part_id = 0x2110;
    wPumpDataDefault3[2].act_id = 0x0003;
    wPumpDataDefault3[2].port = 0b0010;
    wPumpDataDefault3[2].pump = 0x0000;
    wPumpDataDefault3[2].act_flag = 0;
    wPumpDataDefault3[3].hydr_type = 3;
    wPumpDataDefault3[3].hydr_sn = 4;
    wPumpDataDefault3[3].part_id = 0x2200;
    wPumpDataDefault3[3].act_id = 0x0001;
    wPumpDataDefault3[3].port = 0b0010;
    wPumpDataDefault3[3].pump = 0x0000;
    wPumpDataDefault3[3].act_flag = 0;
    wPumpDataDefault3[4].hydr_type = 3;
    wPumpDataDefault3[4].hydr_sn = 5;
    wPumpDataDefault3[4].part_id = 0x2200;
    wPumpDataDefault3[4].act_id = 0x0002;
    wPumpDataDefault3[4].port = 0b0010;
    wPumpDataDefault3[4].pump = 0x0000;
    wPumpDataDefault3[4].act_flag = 0;

    if(VarAdrToInt(TMP_NUM_TABLE) == 0){
        memset(&wPumpData[0],0,sizeof(CUS_PORT)*MAX_PUMPNUM);
        memcpy(&wPumpData[0],wPumpDataDefault1,sizeof(wPumpDataDefault1));
    }
    else if(VarAdrToInt(TMP_NUM_TABLE) == 1){
        memset(&wPumpData[MAX_PUMPNUM],0,sizeof(CUS_PORT)*MAX_PUMPNUM);
        memcpy(&wPumpData[MAX_PUMPNUM],wPumpDataDefault2,sizeof(wPumpDataDefault2));
    }
    else if(VarAdrToInt(TMP_NUM_TABLE) == 2){
        memset(&wPumpData[MAX_PUMPNUM*2],0,sizeof(CUS_PORT)*MAX_PUMPNUM);
        memcpy(&wPumpData[MAX_PUMPNUM*2],wPumpDataDefault3,sizeof(wPumpDataDefault3));
    }

    PUMPFUNC_CHANGE_FLAG = 1;
    ShowlocalPumpData();
}

/**
 * 显示本地的wPumpData的数据，没有去读与下位机的表的数据
 * 原因:恢复默认或者画面内切换表时，修改的数据暂时只存在本地，如果退出并且不保存，就可以放弃修改
 */

static int ShowlocalPumpData()
{
    int i;
    PPUMP_INFO pinfo;

    if(ppumpfos == NULL)
        return 0;

    pinfo = ppumpfos[ID_IN];


    //初始化为0
    for(i = 0;i<MAX_PUMPNUM;i++)
    {
       VarAdrSetInt(TMP_UNIT_DATA+i, 0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+1,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+2,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+3,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+4,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+5,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+6,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+7,0);

       UpdateDef(pinfo, i, 0);
     }

    for(i = 0;i<MAX_PUMPNUM;i++)
    {
        if(((i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)) >= 0) && ((i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)) < MAX_PUMPNUM*3))
        {
            if(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].hydr_sn)
            {
                VarAdrSetInt(TMP_UNIT_DATA+i, wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].part_id);
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,0));
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+1,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,1));
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+2,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,2));
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+3,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,3));
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+4,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,0));
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+5,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,1));
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+6,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,2));
                VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+7,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,3));

                UpdateDef(pinfo, i, (wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].part_id << 16)+ (wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].act_id<<12) +(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].act_flag));
            }
        }
    }
    //for(i_num = 0;i_num<MAX_PUMPNUM*3;i_num++ )
       //oprintf("wPumplocalData->hydr_sn[%d]:%d port:0x%x\n",i_num+1,wPumpData[i_num].hydr_sn,wPumpData[i_num].port);

}

/**
 * 将接收到的表数据存储到wPumpData中（接收数据），并且用wPumpData来显示画面内容
 * 原因：此处显示的是发送给下位机的表的真实内容，是为了修改数据并保存发送给下位机后，会重新请求下位机发送pump表，然后再将真实通信表内容显示在画面上，确保数据真实
 */
static int GetPumpData()
{
    int i;
    CUS_PORT * psubpartinfo =NULL;

    PPUMP_INFO pinfo;

    pinfo = ppumpfos[ID_IN];
    if(ppumpfos == NULL)
        return 0;

    PPROT_TBL_HD ptb_receive = GetTableByType(TB_PUMP,TB_CUR);

    if(ptb_receive ==NULL ||pinfo == NULL)
        return 0;

    if((ptb_receive != NULL))
    {
        psubpartinfo = (CUS_PORT*)(ptb_receive+1);
    }

    //初始化为0
    for(i = 0;i<MAX_PUMPNUM;i++)
    {
       VarAdrSetInt(TMP_UNIT_DATA+i, 0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+1,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+2,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+3,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+4,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+5,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+6,0);
       VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+7,0);

       UpdateDef(pinfo, i, 0);
     }

    if(psubpartinfo == NULL)
        return 0;

    for(i = 0;i<ptb_receive->row_num;i++,psubpartinfo++)
    {
        if(psubpartinfo != NULL && psubpartinfo->hydr_type == 1){
            memcpy(&wPumpData[psubpartinfo->hydr_sn-1],psubpartinfo,sizeof(CUS_PORT));
        }
        else if(psubpartinfo != NULL && psubpartinfo->hydr_type == 2){
            memcpy(&wPumpData[psubpartinfo->hydr_sn-1+MAX_PUMPNUM],psubpartinfo,sizeof(CUS_PORT));
        }
        else if(psubpartinfo != NULL && psubpartinfo->hydr_type == 3){
            memcpy(&wPumpData[psubpartinfo->hydr_sn-1+MAX_PUMPNUM*2],psubpartinfo,sizeof(CUS_PORT));
        }
    }

    for(i = 0;i<MAX_PUMPNUM;i++)
    {
        if(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].hydr_sn)
        {
            VarAdrSetInt(TMP_UNIT_DATA+i, wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].part_id);
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,0));
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+1,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,1));
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+2,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,2));
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+3,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].port,3));
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+4,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,0));
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+5,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,1));
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+6,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,2));
            VarAdrSetInt(TMP_FIRST_DATA+i*FIRST_DATA_LENGT+7,GET_LOW_BIT(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].pump,3));

            UpdateDef(pinfo, i, (wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].part_id << 16)+ (wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].act_id<<12) +(wPumpData[i + MAX_PUMPNUM*VarAdrToInt(TMP_NUM_TABLE)].act_flag));
        }
    }

}

//保存PUMP表,部件id数据存储到wPumpData中（发送部件id数据）
static void SaveUNIT_Data(int index){

    if((index >= 0) && (index < MAX_PUMPNUM))
    {
        if(VarAdrToInt(TMP_NUM_TABLE) == 0)//表1
        {
            if(VarAdrToInt(TMP_UNIT_DATA+index)) //部件id不为0才赋值
            {
                wPumpData[index].hydr_type = 1;
                wPumpData[index].hydr_sn = index+1;
                wPumpData[index].part_id = VarAdrToInt(TMP_UNIT_DATA+index);
                wPumpData[index].act_id = (VarAdrToInt(TMP_ACT_DATA+index) & 0xF000)>>12; //当选了部件时对动作id也初始化
            }
            else //部件id不为0才赋值
            {
                wPumpData[index].hydr_type = 0;
                wPumpData[index].hydr_sn = 0;
                wPumpData[index].part_id = 0;
                wPumpData[index].act_id = 0;
            }
        }
        else if(VarAdrToInt(TMP_NUM_TABLE) == 1)//表2
        {
            if(VarAdrToInt(TMP_UNIT_DATA+index)) //部件id不为0才赋值
            {
                wPumpData[index+MAX_PUMPNUM].hydr_type = 2;
                wPumpData[index+MAX_PUMPNUM].hydr_sn = index+1;
                wPumpData[index+MAX_PUMPNUM].part_id = VarAdrToInt(TMP_UNIT_DATA+index);
                wPumpData[index+MAX_PUMPNUM].act_id= (VarAdrToInt(TMP_ACT_DATA+index) & 0xF000)>>12;
            }
            else
            {
                wPumpData[index+MAX_PUMPNUM].hydr_type = 0;
                wPumpData[index+MAX_PUMPNUM].hydr_sn = 0;
                wPumpData[index+MAX_PUMPNUM].part_id = 0;
                wPumpData[index+MAX_PUMPNUM].act_id = 0;
            }
        }
        else if(VarAdrToInt(TMP_NUM_TABLE) == 2)//表3
        {
            if(VarAdrToInt(TMP_UNIT_DATA+index)) //部件id不为0才赋值
            {
                wPumpData[index+MAX_PUMPNUM*2].hydr_type = 3;
                wPumpData[index+MAX_PUMPNUM*2].hydr_sn = index+1;
                wPumpData[index+MAX_PUMPNUM*2].part_id = VarAdrToInt(TMP_UNIT_DATA+index);
                wPumpData[index+MAX_PUMPNUM*2].act_id= (VarAdrToInt(TMP_ACT_DATA+index) & 0xF000)>>12;
            }
            else
            {
                wPumpData[index+MAX_PUMPNUM*2].hydr_type = 0;
                wPumpData[index+MAX_PUMPNUM*2].hydr_sn = 0;
                wPumpData[index+MAX_PUMPNUM*2].part_id = 0;
                wPumpData[index+MAX_PUMPNUM*2].act_id = 0;
            }
        }
    }
}

//保存PUMP表,表数据存储到wPumpData中（发送动作id数据）
static void SaveACT_Data(int index){

    if((index >= 0) && (index < MAX_PUMPNUM))
    {
        if(VarAdrToInt(TMP_NUM_TABLE) == 0)//表1
        {
           wPumpData[index].act_id= (VarAdrToInt(TMP_ACT_DATA+index) & 0xF000)>>12;
           wPumpData[index].act_flag= VarAdrToInt(TMP_ACT_DATA+index) & 0x0F;
        }
        else if(VarAdrToInt(TMP_NUM_TABLE) == 1)//表2
        {
            wPumpData[index+MAX_PUMPNUM].act_id= (VarAdrToInt(TMP_ACT_DATA+index) & 0xF000)>>12;
            wPumpData[index+MAX_PUMPNUM].act_flag= VarAdrToInt(TMP_ACT_DATA+index) & 0x0F;
        }
        else if(VarAdrToInt(TMP_NUM_TABLE) == 2)//表3
        {
            wPumpData[index+MAX_PUMPNUM*2].act_id= (VarAdrToInt(TMP_ACT_DATA+index) & 0xF000)>>12;
            wPumpData[index+MAX_PUMPNUM*2].act_flag= VarAdrToInt(TMP_ACT_DATA+index) & 0x0F;
        }
    }
}

//保存PUMP表,表数据存储到wPumpData中（发送主油路和从站配置数据）
static void SaveFIRST_Data(int index){

    if((index >= 0) && (index < MAX_PUMPNUM))
    {
        if(VarAdrToInt(TMP_NUM_TABLE) == 0)//表1
        {
           wPumpData[index].port= VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT)+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+1)*2+
                                          VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+2)*4+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+3)*8;
           wPumpData[index].pump= VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+4)+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+5)*2+
                                          VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+6)*4+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+7)*8;
        }
        else if(VarAdrToInt(TMP_NUM_TABLE) == 1)//表2
        {
            wPumpData[index+MAX_PUMPNUM].port= VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT)+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+1)*2+
                                          VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+2)*4+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+3)*8;
            wPumpData[index+MAX_PUMPNUM].pump= VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+4)+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+5)*2+
                                           VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+6)*4+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+7)*8;

        }
        else if(VarAdrToInt(TMP_NUM_TABLE) == 2)//表3
        {
            wPumpData[index+MAX_PUMPNUM*2].port= VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT)+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+1)*2+
                                          VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+2)*4+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+3)*8;
            wPumpData[index+MAX_PUMPNUM*2].pump= VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+4)+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+5)*2+
                                           VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+6)*4+VarAdrToInt(TMP_FIRST_DATA+index*FIRST_DATA_LENGT+7)*8;
        }
    }
}


//发送PUMP表
static void SendData()
{
    PPROT_TBL_HD ptb_send = GetTableByType(TB_PUMP, TB_CUR);
    int i,j=1;//j是实际发送的num数目
    //20250115 fqh （j从1开始是因为在空表的情况下，与下位机约定好发送一条全0的数据）

    if(ptb_send == NULL)
    {
        return;
    }

    /*for(i = 0;i<MAX_PUMPNUM*3;i++ )
       oprintf("wPumpData->hydr_sn[%d]:%d part_id:0x%x port:0x%x\n",i+1,wPumpData[i].hydr_sn,wPumpData[i].part_id,wPumpData[i].port);
    */
    memset(wPumpData_send, 0, sizeof(wPumpData_send)); //发送数据初始化0

    for(i = 0;i<MAX_PUMPNUM*3;i++)
    {
        if(wPumpData[i].part_id)
        {
            memcpy(&wPumpData_send[j],&wPumpData[i],sizeof(CUS_PORT));
            j++;
        }
    }

    ptb_send->row_len = MAX_PUMPLENTH;
    ptb_send->row_num = j;
    ptb_send->type = TB_PUMP;
    ptb_send->sn = 1;

    /*for(i = 0;i<j;i++ )
      oprintf("send->hydr_sn[%d]:%d part_id:0x%x act_id:0x%x port:0x%x pump:0x%x act_flag:%d\n",i+1,
              wPumpData_send[i].hydr_sn,wPumpData_send[i].part_id,wPumpData_send[i].act_id,wPumpData_send[i].port,wPumpData_send[i].pump,wPumpData_send[i].act_flag);
    */
    memcpy((UI8*)(ptb_send+1), wPumpData_send, sizeof(CUS_PORT)*j);

    ProtSendTbl(ptb_send);

    ProtReqTbl(TB_PUMP,TB_CUR); //请求下位机发送表
}

//20241223 fqh 应对新老主机互刷时可能出现的pump表没有创建的情况，则创建
static void OpenPumpCalShow()
{
    PPROT_TBL_HD ptb_send = GetTableByType(TB_PUMP, TB_CUR);
    UI16 wTabData[MAX_PUMPNUM * (MAX_PUMPLENTH/2)]= {0};

    if(ptb_send == NULL)
    {
        oprintf("ptb is NULL\n");
        //20241223 fqh 此处初始化大小与实际放的无关，实际发送在SendData函数中会对ptb_send重新赋值
        ptb_send = (PPROT_TBL_HD)OBJMemMalloc(MAX_PUMPNUM * MAX_PUMPLENTH+sizeof(PROT_TBL_HD));
        ptb_send->row_len = MAX_PUMPLENTH;
        ptb_send->row_num = MAX_PUMPNUM;
        ptb_send->type = TB_PUMP;
        ptb_send->sn = 1;
        memcpy((UI8*)(ptb_send+1), wTabData, MAX_PUMPNUM * MAX_PUMPLENTH);
        AddPumpTable(ptb_send);
    }

}

// 更新定义下拉框数据
static void UpdateDef(PPUMP_INFO ppumpfo, int index, UI32 addr)
{
    PIntNode pnode;
    TSTRING_CREATE(str_data);
    TSTRING_CREATE(str_tran);
    int j = 0;
    int find_value = 0;
    char tmp[20];

    if(ppumpfo != NULL)
    {
        SC_FOREACH(pnode, ppumpfo->addr_list)
        {
            //oprintf("addr:0x%x pnode->value:0x%x\n",addr,pnode->value);
            if(pnode!= NULL && (pnode->value >> 16) == (addr >> 16))
            {
                tstring_appends_fix(&str_data, GetMapTran(ppumpfo->name_tbl, pnode->value), MIN_ALLOC_SIZE);
                tstring_append_fix(&str_data, ';', MIN_ALLOC_SIZE);
                if(pnode->value == addr)
                {
                    find_value = pnode->value;
                }
                if(find_value == 0) //为了在选定部件号后，给动作号一个初始值，显示画面也会显示一个初始值
                {
                    find_value = pnode->value;
                }
                WordToStr(tmp, pnode->value, 0);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ';', MIN_ALLOC_SIZE);
                ++j;
            }
        }

        VarAdrSetStr(ppumpfo->tmp_def_data + index, str_data.str);
        VarAdrSetStr(ppumpfo->tmp_def_tran + index, str_tran.str);
        VarAdrSetInt(ppumpfo->tmp_def + index, find_value);
        TSTRING_DESTROY(str_data);
        TSTRING_DESTROY(str_tran);

    }
}
static int switch_key=0;
/************************************************************************/
/* 切页面信息提示框“保存”按钮                                         */
/************************************************************************/
static void SaveButton()
{
    if(VarAdrToUI16(OPMODE_INDEX) == MODE_MANUAL)//20200109.cyx 只有手动状态下才能保存
    {
        SendData();
    }

    PUMPFUNC_CHANGE_FLAG = 0;//数据有修改标记还原
    //返回物理按键
    if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6)
    {
        switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
    }
    else if(switch_key ==GUI_KEY_FUN7)
    {
        switch_key = KEY_FUN7;
    }
    else if(switch_key == GUI_KEY_MAINPAGE)
    {
        switch_key = KEY_MAIN;
    }
    else if(switch_key == GUI_KEY_FUN8)
    {
        switch_key = KEY_PC;
    }
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
    SendKey(switch_key);
    //oprintf("switch_key==%x\r\n",switch_key);
}

static void CancelButton()
{
    PUMPFUNC_CHANGE_FLAG = 0;//数据有修改标记还原
    if(switch_key>=GUI_KEY_FUN1 && switch_key<=GUI_KEY_FUN6) //F1-F6
    {
        switch_key = (switch_key-GUI_KEY_FUN1)+ KEY_FUN1;
    }
    else if(switch_key ==GUI_KEY_FUN7)  //F7
    {
        switch_key = KEY_FUN7;
    }
    else if(switch_key == GUI_KEY_MAINPAGE)
    {
        switch_key = KEY_MAIN;
    }
    else if(switch_key == GUI_KEY_FUN8) //PC
    {
        switch_key = KEY_PC;
    }
    else if(switch_key == GUI_KEY_NEXTPAGE)//20191126.cyx
    {
        switch_key = KEY_NEXTPAGE;
    }
    SendKey(switch_key);
}
/************************************************************************/
/* 配置修改回调函数                                                     */
/************************************************************************/
static void PumpChange(int key,int press)
{
    if(press)
    {
        g_question.pOkfunc = SaveButton;
        g_question.pCancelfunc = CancelButton;
        switch_key = key;

        //20200109.cyx 非手动，自动登出时默认不保存，不弹框
        if((VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL) || VarAdrToInt(USER_STATE))
        {
            CancelButton();
        }
        else
        {
            ShowQuestion(VW_MSG_SAVEDATA,g_question);
        }
    }
}

static void CtlConnect()
{
}
static int Pump_FuncOnInit(Pump_Func_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ppumpfos[ID_IN] = PumpGetInfo(TB_PUMP);

    SetPumpFuncCB(PumpChange); //202412 fqh 修改数据后切换画面跳出保存
    if(!ppumpfos[ID_IN] )
    {
        OWARN("Can't get io info");
    }

    ppumpfos[ID_IN]->tmp_comp     = TMPVAL(30);
    ppumpfos[ID_IN]->tmp_def      = TMPVAL(50);
    ppumpfos[ID_IN]->tmp_def_data = TMPSTR(0);
    ppumpfos[ID_IN]->tmp_def_tran = TMPSTR(20);

    return 0;
}
static int Pump_FuncOnShow(Pump_Func_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) < 0 || VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) >= MAX_TABLE_NUM)
        {
            VarAdrSetInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM,0); //fqh 联动油路表标记合理范围为0-2
            VarAdrSaveInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM);
        }
        VarAdrSetInt(TMP_NUM_TABLE,VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM));
        if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) == 0)
        {
            VarAdrSetInt(TMP_HYDR_LINK_OPNCHG,1);
            VarAdrSetInt(TMP_HYDR_LINK_OPNEJT,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNCORE,0);
        }
        else if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) == 1)
        {
            VarAdrSetInt(TMP_HYDR_LINK_OPNCHG,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNEJT,1);
            VarAdrSetInt(TMP_HYDR_LINK_OPNCORE,0);
        }
        else if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) == 2)
        {
            VarAdrSetInt(TMP_HYDR_LINK_OPNCHG,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNEJT,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNCORE,1);
    }

    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

    memset(wPumpData, 0, sizeof(wPumpData)); //本地数据初始化0
    ShowlocalPumpData();//画面数据初始化0

    OpenPumpCalShow();
    ProtReqTbl(TB_PUMP,TB_CUR); //请求下位机发送表
    return 0;
}
static int Pump_FuncOnHide(Pump_Func_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Pump_FuncOnUpdate(Pump_Func_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

    if(g_pumpUpdateFlag & 0x1)
    {
        GetPumpData();//202412 fqh 收到下位机发送的表后，刷新画面
    }
    g_pumpUpdateFlag = 0;
    return 0;
}
static int Pump_FuncOnChange(Pump_Func_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    int addr, index, i;
    PPUMP_INFO pinfo;
    PATTR pattr;

    pattr = (PATTR)msg_para;
    addr = OBJAttrGetAdr(pattr);

    if(pwgt!=ppg->cbTable && pwgt!=ppg->stbt1 && pwgt!=ppg->stbt2 && pwgt!=ppg->stbt3 && pwgt!=ppg->btsave)
    {
        PUMPFUNC_CHANGE_FLAG = 1;//20241223 fqh 数据有修改用于提示框
    }

    if(addr >= TMP_UNIT_DATA && addr< TMP_UNIT_DATA+MAX_PUMPNUM)
    {
        pinfo = ppumpfos[ID_IN];

        if(pinfo != NULL && ppumpfos !=NULL)
        {
            int comp_adr;

            index = addr - pinfo->tmp_comp;
            comp_adr = RU32(*pattr) << 16;
            //oprintf("addr:0x%x pinfo->tmp_id:0x%x index:%d comp_adr:0x%x\n",addr,pinfo->tmp_comp,index,comp_adr);
            UpdateDef(pinfo, index, comp_adr);
        }

    }

    //修改部件号
    if(addr >= TMP_UNIT_DATA && addr < TMP_UNIT_DATA+MAX_PUMPNUM)
    {
        index = addr - TMP_UNIT_DATA;
        SaveUNIT_Data(index);
    }
    //修改动作
    else if(addr >= TMP_ACT_DATA && addr < TMP_ACT_DATA+MAX_PUMPNUM)
    {
        index = addr - TMP_ACT_DATA;
        SaveACT_Data(index);
    }
     //修改主泵和从泵
    else if(addr >= TMP_FIRST_DATA && addr < TMP_FIRST_DATA+MAX_PUMPNUM*FIRST_DATA_LENGT)
    {
        index = addr - TMP_FIRST_DATA;
        SaveFIRST_Data(index/8); //8个数据为一行
    }
    else if((addr == d_machine1_MOLDSET_FL_HYDR_LINK_OPNCHG) || (addr == d_machine1_MOLDSET_FL_HYDR_LINK_OPNEJT) || (addr == d_machine1_MOLDSET_FL_HYDR_LINK_OPNCORE))
    {
        //20250306 jhh 非手动情况下提示
        if(VarAdrToInt(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }
    }

    if(pwgt==ppg->btsave)
    {
        PUMPFUNC_CHANGE_FLAG = 0;//数据有修改标记还原
        SendData();
    }
    else if(pwgt==ppg->cbTable)
    {
        if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) == 0)
        {
            VarAdrSetInt(TMP_HYDR_LINK_OPNCHG,1);
            VarAdrSetInt(TMP_HYDR_LINK_OPNEJT,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNCORE,0);
        }
        else if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) == 1)
        {
            VarAdrSetInt(TMP_HYDR_LINK_OPNCHG,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNEJT,1);
            VarAdrSetInt(TMP_HYDR_LINK_OPNCORE,0);
        }
        else if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) == 2)
        {
            VarAdrSetInt(TMP_HYDR_LINK_OPNCHG,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNEJT,0);
            VarAdrSetInt(TMP_HYDR_LINK_OPNCORE,1);
        }
        if(VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) >= 0 && VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM) < MAX_TABLE_NUM)
        {
            VarAdrSetInt(TMP_NUM_TABLE,VarAdrToInt(p_PP_MACHSET_PUMP_FUNC_TABLE_NUM));
        }
        ShowlocalPumpData();//切换表显示不同内容，画面内切换表时看到的数据只是本地数据
    }
    else if(pwgt==ppg->btRestore)
    {
        PUMPFUNC_CHANGE_FLAG = 0; //这里将修改标志位置0是因为弹框时文字要为确定而不是保存 在后面DataRestoreToDefault会重新置为1
        g_question.pOkfunc = DataRestoreToDefault;//恢复为默认值
        g_question.pCancelfunc = NULL;
        ShowQuestion(VW_MSG_RESTORETODEFAULT,g_question);
    }
    return 0;
}
