#include "WSys_Para_2.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"

DEFINE_SYS_PARA_2_MAP

CONNECT_SYS_PARA_2_EVENT

#define PART_OFFSET 0x00100000 //部件偏移

static void  SetCoreParam()
{
    UI32 coreselect = VarAdrToInt(p_PP_MACHSET_CORE_SELECT_SYSPAR);
    UI32 offset = PART_OFFSET*coreselect;
    WGTSetAttrByName(sys_para_2_pg.ed31,EDIT_TEXT_NAME,d_coreA_MACHSET_TMDLY_VLV_ONCOREF+offset);
    WGTSetAttrByName(sys_para_2_pg.ed41,EDIT_TEXT_NAME,d_coreA_MACHSET_TMDLY_VLV_OFFCOREF+offset);
    WGTSetAttrByName(sys_para_2_pg.edit229,EDIT_TEXT_NAME,d_coreA_MACHSET_COREF_HYDELAY+offset);
    WGTSetAttrByName(sys_para_2_pg.ed71,EDIT_TEXT_NAME,d_coreA_MACHSET_TM_COREINENDDLY+offset);
    WGTSetAttrByName(sys_para_2_pg.ed51,EDIT_TEXT_NAME,d_coreA_MACHSET_TMDLY_VLV_ONCOREB+offset);
    WGTSetAttrByName(sys_para_2_pg.ed61,EDIT_TEXT_NAME,d_coreA_MACHSET_TMDLY_VLV_OFFCOREB+offset);
    WGTSetAttrByName(sys_para_2_pg.edit230,EDIT_TEXT_NAME,d_coreA_MACHSET_COREB_HYDELAY+offset);
    WGTSetAttrByName(sys_para_2_pg.ed81,EDIT_TEXT_NAME,d_coreA_MACHSET_TM_COREOUTENDDLY+offset);

    if(coreselect == 0)
    {
        WGTSetVisible(sys_para_2_pg.subpage1, 1);
        WGTSetVisible(sys_para_2_pg.subpage2, 0);
        WGTSetVisible(sys_para_2_pg.subpage3, 0);
        WGTSetVisible(sys_para_2_pg.subpage4, 0);
        WGTSetVisible(sys_para_2_pg.subpage5, 0);
        WGTSetVisible(sys_para_2_pg.subpage6, 0);
    }
    else if(coreselect == 1)
    {
        WGTSetVisible(sys_para_2_pg.subpage1, 0);
        WGTSetVisible(sys_para_2_pg.subpage2, 1);
        WGTSetVisible(sys_para_2_pg.subpage3, 0);
        WGTSetVisible(sys_para_2_pg.subpage4, 0);
        WGTSetVisible(sys_para_2_pg.subpage5, 0);
        WGTSetVisible(sys_para_2_pg.subpage6, 0);
    }
    else if(coreselect == 2)
    {
        WGTSetVisible(sys_para_2_pg.subpage1, 0);
        WGTSetVisible(sys_para_2_pg.subpage2, 0);
        WGTSetVisible(sys_para_2_pg.subpage3, 1);
        WGTSetVisible(sys_para_2_pg.subpage4, 0);
        WGTSetVisible(sys_para_2_pg.subpage5, 0);
        WGTSetVisible(sys_para_2_pg.subpage6, 0);
     }
    else if(coreselect == 3)
    {
        WGTSetVisible(sys_para_2_pg.subpage1, 0);
        WGTSetVisible(sys_para_2_pg.subpage2, 0);
        WGTSetVisible(sys_para_2_pg.subpage3, 0);
        WGTSetVisible(sys_para_2_pg.subpage4, 1);
        WGTSetVisible(sys_para_2_pg.subpage5, 0);
        WGTSetVisible(sys_para_2_pg.subpage6, 0);
     }
    else if(coreselect == 4)
    {
        WGTSetVisible(sys_para_2_pg.subpage1, 0);
        WGTSetVisible(sys_para_2_pg.subpage2, 0);
        WGTSetVisible(sys_para_2_pg.subpage3, 0);
        WGTSetVisible(sys_para_2_pg.subpage4, 0);
        WGTSetVisible(sys_para_2_pg.subpage5, 1);
        WGTSetVisible(sys_para_2_pg.subpage6, 0);
     }
    else if(coreselect == 5)
    {
        WGTSetVisible(sys_para_2_pg.subpage1, 0);
        WGTSetVisible(sys_para_2_pg.subpage2, 0);
        WGTSetVisible(sys_para_2_pg.subpage3, 0);
        WGTSetVisible(sys_para_2_pg.subpage4, 0);
        WGTSetVisible(sys_para_2_pg.subpage5, 0);
        WGTSetVisible(sys_para_2_pg.subpage6, 1);
     }
}

static void CtlConnect()
{
}
static int Sys_Para_2OnInit(Sys_Para_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_Para_2OnShow(Sys_Para_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetCoreParam();
    return 0;
}
static int Sys_Para_2OnHide(Sys_Para_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_Para_2OnUpdate(Sys_Para_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Sys_Para_2OnChange(Sys_Para_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(pwgt == ppg->cb12)//曲线显示
    {
        SetCoreParam();
    }
    return 0;
}
