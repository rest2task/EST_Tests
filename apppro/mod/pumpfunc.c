//#include "ioredef.h"
#include "oprintf.h" 
#include "protcmd.h"
#include "tablectl.h"
#include "sclist.h"
#include "sccsv.h"
#include "common.h"
#include "language.h"
#include "stdlib.h"
#include "tstring.h"
#include "file.h"
#include "verinfo.h"
#include "panelcfg.h"
#include "vlvlogic.h"
#include "pumpfunc.h"

PUMP_INFO pump_infos[1] = {
    {TB_PUMP, PUMP_FUNC_FILE,  8},//20200317
};


//链表插入比较函数
static int _Compare(const void* left, const void* right)
{
	return ((PIntNode)left)->value - ((PIntNode)right)->value;
}

//得到csv文件的id列
static int _GetCSVIdCol(sccsv_t csv)
{
    int col;
    for(col = 0; col < csv->clen; ++col)
    {
        if(strcmp(sccsv_get(csv, 0, col), "id") == 0)
            return col;
    }
	return -1;
}


static void Pump__LoadList(PPUMP_INFO pinfo)
{
    sccsv_t csv;
    int i, id_col;
    char* stop_str;
    sclist_t* pphead = (sclist_t*)&pinfo->addr_list;
    csv = sccsv_new(pinfo->map_path);
    if(!csv)
    {

        OWARN("Can't prase %s!", pinfo->map_path);
    }
    else
    {
        id_col = _GetCSVIdCol(csv);
        if(id_col != -1)
        {
            for(i = 1; i < csv->rlen; ++i)
            {
                PIntNode pnode = (PIntNode)OBJMemMalloc(sizeof(IntNode));
                pnode->value = strtol(sccsv_get(csv, i, id_col), &stop_str, 0);
                sclist_sortadd(pphead, _Compare, pnode); //增序插入
            }
        }
    }
    sccsv_die(&csv);
}


void PumpInital()
{
    int i;
    for(i = 0; i < ARRAY_NUM(pump_infos); ++i)
    {
        Pump__LoadList(&pump_infos[i]);
        ParseCSVFile(pump_infos[i].map_path, &pump_infos[i].name_tbl);
    }
}


PPUMP_INFO PumpGetInfo(int table_type)
{
    int i;
    for(i = 0; i < ARRAY_NUM(pump_infos); ++i)
    {
        if(pump_infos[i].table_type == table_type)
        {
            return &pump_infos[i];
        }
    }
    return NULL;
}

