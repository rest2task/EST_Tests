#include "WVlv_LogicTable.h"
#include "typedef.h"
#include "pagefram.h"
#include "vlvlogic.h"
#include "oprintf.h"
#include "database.h"
#include "stdio.h"
#include "labelwgt.h"
#include "checkboxwgt.h"
#include "mainframe.h"
#include "ioredef.h"

DEFINE_VLV_LOGICTABLE_MAP

CONNECT_VLV_LOGICTABLE_EVENT

static PHASH_TBL  pmap_step =NULL;
static PHASH_TBL  ptbl_vlv_logic = NULL;
static char data_buff[100];
static UI16 cur_page = 1;

#define COMPAREITEM_FRONT   0xFA00 //比较项类型查找前置 查找值为1对应表中FA01 表格复用来减少表格数量
#define LGCOMPARESIGN_FRONT 0xFB00 //逻辑比较符查找前置

//从vlvlogic.map找翻译
static char* VlvLogic_GetMapTran(UI32 data)
{
    snprintf(data_buff, sizeof(data_buff), "%x", data);

    if(ptbl_vlv_logic == NULL)
    {
        return data_buff;
    }

    if(GetMapTran(ptbl_vlv_logic, data) != NULL)
    {
        snprintf(data_buff, sizeof(data_buff), "%s", GetMapTran(ptbl_vlv_logic, data));
    }

    oprintf("VlvLogic:data_buff = %s\n",data_buff);
    return data_buff;
}

//从step.map找翻译
static char* Step_GetMapTran(UI32 data)
{
    snprintf(data_buff, sizeof(data_buff), "%x", data);

    if(pmap_step == NULL)
    {
        return data_buff;
    }

    if(GetMapTran(pmap_step, data) != NULL)
    {
        snprintf(data_buff, sizeof(data_buff), "%s", GetMapTran(pmap_step, data));
    }

    oprintf("Step:data = %x, data_buff = %s\n",data,data_buff);
    return data_buff;
}

//从output.map找翻译
static char* Output_GetMapTran(UI32 data)
{
    snprintf(data_buff, sizeof(data_buff), "%x", data);

    if(pmap_output == NULL)
    {
        return data_buff;
    }

    if(GetMapTran(pmap_output, data) != NULL)
    {
        snprintf(data_buff, sizeof(data_buff), "%s", GetMapTran(pmap_output, data));
    }

    oprintf("Output:data = %x, data_buff = %s\n",data,data_buff);
    return data_buff;
}

#define MAX_VLVNUM 6 //最大阀数量
static void ShowCurLogicTable(UI16 page)
{
    char buff[100],buff2[100],stepbuff[100],itembuff[255];
    int i;

    UI32 act_step_start = 0,act_step_end = 0,vlv_ouput = 0;
    UI32 temp_compare = 0;
    PWGT lblvlvname[MAX_VLVNUM] = {vlv_logictable_pg.lblvlvname1, vlv_logictable_pg.lblvlvname2, vlv_logictable_pg.lblvlvname3,
                                   vlv_logictable_pg.lblvlvname4, vlv_logictable_pg.lblvlvname5, vlv_logictable_pg.lblvlvname6};
    PWGT lblvlvnum[MAX_VLVNUM] = {vlv_logictable_pg.lblvlvnum1, vlv_logictable_pg.lblvlvnum2, vlv_logictable_pg.lblvlvnum3,
                                  vlv_logictable_pg.lblvlvnum4, vlv_logictable_pg.lblvlvnum5, vlv_logictable_pg.lblvlvnum6};
    PWGT lblvlvstate[MAX_VLVNUM] = {vlv_logictable_pg.lblvlvstate1, vlv_logictable_pg.lblvlvstate2, vlv_logictable_pg.lblvlvstate3,
                                    vlv_logictable_pg.lblvlvstate4, vlv_logictable_pg.lblvlvstate5, vlv_logictable_pg.lblvlvstate6};
    PWGT lblvlvondelay[MAX_VLVNUM] = {vlv_logictable_pg.lblvlvondelay1, vlv_logictable_pg.lblvlvondelay2, vlv_logictable_pg.lblvlvondelay3,
                                      vlv_logictable_pg.lblvlvondelay4, vlv_logictable_pg.lblvlvondelay5, vlv_logictable_pg.lblvlvondelay6};
    PWGT lblvlvoffdelay[MAX_VLVNUM] = {vlv_logictable_pg.lblvlvoffdelay1, vlv_logictable_pg.lblvlvoffdelay2, vlv_logictable_pg.lblvlvoffdelay3,
                                       vlv_logictable_pg.lblvlvoffdelay4, vlv_logictable_pg.lblvlvoffdelay5, vlv_logictable_pg.lblvlvoffdelay6};
    PWGT cbvlv[MAX_VLVNUM] = {vlv_logictable_pg.cbvlv1, vlv_logictable_pg.cbvlv2, vlv_logictable_pg.cbvlv3,
                              vlv_logictable_pg.cbvlv4, vlv_logictable_pg.cbvlv5, vlv_logictable_pg.cbvlv6};

    if(page == 0)
    {
        LabelSetText(vlv_logictable_pg.lblcurtable, "0/0");
        LabelSetText(vlv_logictable_pg.lblmonitoract, "");
        LabelSetText(vlv_logictable_pg.lblactinterval, "");
        LabelSetText(vlv_logictable_pg.lbloperatemode, "");
        LabelSetText(vlv_logictable_pg.lblcompareitemnum, "");
        LabelSetText(vlv_logictable_pg.lblcompareitem, "");

        for(i = 0; i < MAX_VLVNUM; i++)
        {
            LabelSetText(lblvlvname[i], "");
            LabelSetText(lblvlvnum[i], "");
            LabelSetText(lblvlvstate[i], "");
            LabelSetText(lblvlvondelay[i], "");
            LabelSetText(lblvlvoffdelay[i], "");
            CheckboxSetCheck(cbvlv[i], FALSE);
        }

        return;
    }

    if(page > Get_ElecoutTable_Num())
    {
        oprintf("Err page %d\n", page);
        return;
    }

    //显示当前页
    snprintf(buff,sizeof(buff), "%d/%d",page,Get_ElecoutTable_Num());
    LabelSetText(vlv_logictable_pg.lblcurtable, buff);

    //监控动作
    LabelSetText(vlv_logictable_pg.lblmonitoract, VlvLogic_GetMapTran(ElecoutTable[page-1].act_id));

    //动作区间 高8位开始 低8位结束
    act_step_start = (ElecoutTable[page-1].act_id << 16) + (ElecoutTable[page-1].step&0xFF00);
    snprintf(buff,sizeof(buff), "%s",Step_GetMapTran(act_step_start));
    act_step_end = (ElecoutTable[page-1].act_id << 16) + ((ElecoutTable[page-1].step&0xFF) << 8);
    snprintf(stepbuff,sizeof(stepbuff), "%s-%s",buff,Step_GetMapTran(act_step_end));
    LabelSetText(vlv_logictable_pg.lblactinterval, stepbuff);

    //操作模式
    LabelSetText(vlv_logictable_pg.lbloperatemode, VlvLogic_GetMapTran(ElecoutTable[page-1].en_oper_mode));

    //比较项个数
    snprintf(buff, sizeof(buff), "%d", ElecoutTable[page-1].cmp_num_po);
    LabelSetText(vlv_logictable_pg.lblcompareitemnum, buff);

    //比较项
    if(ElecoutTable[page-1].cmp_num_po > 0)
    {
        if(ElecoutTable[page-1].cmp[0].var1.type == 1)//常数
        {
            snprintf(buff,sizeof(buff), "%d",ElecoutTable[page-1].cmp[0].var1.ind);
        }
        else if(ElecoutTable[page-1].cmp[0].var1.type == 4)//子类数据
        {
            temp_compare = (ElecoutTable[page-1].cmp[0].var1.ref_id & 0xFFF0) + ElecoutTable[page-1].cmp[0].var1.sub;
            temp_compare = (temp_compare << 16) + ElecoutTable[page-1].cmp[0].var1.ind;
            snprintf(buff,sizeof(buff), "%s",VlvLogic_GetMapTran(temp_compare));
        }
        else
        {
            temp_compare = COMPAREITEM_FRONT + ElecoutTable[page-1].cmp[0].var1.type;
            snprintf(buff,sizeof(buff), "%s",VlvLogic_GetMapTran(temp_compare));
        }

        if(ElecoutTable[page-1].cmp[0].var2.type == 1)//常数
        {
            snprintf(buff2,sizeof(buff2), "%d",ElecoutTable[page-1].cmp[0].var2.ind);
        }
        else if(ElecoutTable[page-1].cmp[0].var2.type == 4)//子类数据
        {
            temp_compare = (ElecoutTable[page-1].cmp[0].var2.ref_id & 0xFFF0) + ElecoutTable[page-1].cmp[0].var2.sub;
            temp_compare = (temp_compare << 16) + ElecoutTable[page-1].cmp[0].var2.ind;
            snprintf(buff2,sizeof(buff2), "%s",VlvLogic_GetMapTran(temp_compare));
        }
        else
        {
            temp_compare = COMPAREITEM_FRONT + ElecoutTable[page-1].cmp[0].var2.type;
            snprintf(buff2,sizeof(buff2), "%s",VlvLogic_GetMapTran(temp_compare));
        }

        temp_compare = LGCOMPARESIGN_FRONT + ElecoutTable[page-1].cmp[0].exp_op;
        snprintf(itembuff,sizeof(itembuff), "%s %s %s",buff,VlvLogic_GetMapTran(temp_compare),buff2);
        LabelSetText(vlv_logictable_pg.lblcompareitem, itembuff);
    }
    else
    {
        LabelSetText(vlv_logictable_pg.lblcompareitem, "");
    }

    //阀
    for(i = 0; i < MAX_VLVNUM; i++)
    {
        //阀名称
        vlv_ouput = (ElecoutTable[page-1].e_po[i].art_name << 16) + ElecoutTable[page-1].e_po[i].po_ind;
        if(vlv_ouput == 0)
        {
            LabelSetText(lblvlvname[i], "");
            LabelSetText(lblvlvnum[i], "");
            LabelSetText(lblvlvstate[i], "");
            LabelSetText(lblvlvondelay[i], "");
            LabelSetText(lblvlvoffdelay[i], "");
            CheckboxSetCheck(cbvlv[i], FALSE);

            continue;
        }
        LabelSetText(lblvlvname[i], Output_GetMapTran(vlv_ouput));

        //阀编号
        snprintf(buff, sizeof(buff), "%d", ElecoutTable[page-1].e_po[i].po_ind);
        LabelSetText(lblvlvnum[i], buff);

        //阀状态 最低位代表开关 高7位代表延时 单位10ms
        snprintf(buff, sizeof(buff), "%d", (ElecoutTable[page-1].e_po[i].onoff_delay & 0x1));
        LabelSetText(lblvlvstate[i], buff);

        //开延时 最低位代表开关 高7位代表延时 单位10ms
        snprintf(buff, sizeof(buff), "%d", (ElecoutTable[page-1].e_po[i].onoff_delay & 0xFF) >> 1);
        LabelSetText(lblvlvondelay[i], buff);

        //关延时 最低位无用 高7位代表延时 时间单位10ms
        snprintf(buff, sizeof(buff), "%d", ((ElecoutTable[page-1].e_po[i].onoff_delay & 0xFF00) >> 9));
        LabelSetText(lblvlvoffdelay[i], buff);

        //附加条件
        if(ElecoutTable[page-1].add_condition & (1 << i))
        {
            CheckboxSetCheck(cbvlv[i], TRUE);
        }
        else
        {
            CheckboxSetCheck(cbvlv[i], FALSE);
        }
    }
}

static void CtlConnect()
{
}
static int Vlv_LogicTableOnInit(Vlv_LogicTable_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    ParseCSVFile(ACTSETP_FILE,&pmap_step);
    ParseCSVFile(VLV_LOGIC_FILE, &ptbl_vlv_logic);

    return 0;
}
static int Vlv_LogicTableOnShow(Vlv_LogicTable_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(Get_ElecoutTable_Num() > 0)
    {
        cur_page = 1;
    }
    else
    {
        cur_page = 0;
    }

    ShowCurLogicTable(cur_page);

    return 0;
}
static int Vlv_LogicTableOnHide(Vlv_LogicTable_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Vlv_LogicTableOnUpdate(Vlv_LogicTable_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Vlv_LogicTableOnChange(Vlv_LogicTable_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt==ppg->btlast)
    {
        if(cur_page > 1)
        {
            cur_page--;
            ShowCurLogicTable(cur_page);
        }
    }
    else if(pwgt==ppg->btnext)
    {
        if(cur_page < Get_ElecoutTable_Num())
        {
            cur_page++;
            ShowCurLogicTable(cur_page);
        }
    }

    return 0;
}
