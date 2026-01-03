#include "WSeq_Vlv_2.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "dropwgt.h"
#include "information.h"
#include "usermod.h"//20210914 dyl touch
#include "protcmd.h"
#include "ctrlcmd.h"
#include "labelwgt.h"
#include "oprintf.h"

DEFINE_SEQ_VLV_2_MAP

CONNECT_SEQ_VLV_2_EVENT

#define TMP_VLV_NUM	    TMPVAL(0)  //第一个顺序阀checkbox关联的临时变量
#define VLV_ALLNUM	    18 //一页顺序阀总数量

//设置顺序阀模式
static void SetSeqVlvMode(UI32 offset,UI32 value)
{
    PWGT pwgt_dlytm,pwgt_acttm,pwgt_uint1,pwgt_uint2;
    pwgt_dlytm = PageFramCurWgtByIndex(offset+1);
    pwgt_acttm = PageFramCurWgtByIndex(offset+2);
    pwgt_uint1 = PageFramCurWgtByIndex(offset+5);
    pwgt_uint2 = PageFramCurWgtByIndex(offset+6);

    int offset_n = (offset-1)/10/3, offset_m = (offset-1)/10 % 3;
    //20250108 fqh 顺序阀 根据顺序阀位置或时间修改关联变量和单位
    //oprintf("offset:%d value:%d offset_n:%d offset_m:%d\n",offset,value,offset_n,offset_m);

    switch(value)
    {
    case 0:
        WGTSetEnable(pwgt_dlytm, FALSE);
        WGTSetEnable(pwgt_acttm, FALSE);
        break;
    case 1:
        WGTSetEnable(pwgt_dlytm, TRUE);
        WGTSetEnable(pwgt_acttm, TRUE);
        WGTSetAttrByName(pwgt_dlytm, "text", d_airblow7_MOLDSET_TM_SEQ_DELAY + 0x100000*(offset_n)+ 5*(offset_m)); // 0x23130007, 顺序阀1延时
        WGTSetAttrByName(pwgt_acttm, "text", d_airblow7_MOLDSET_TM_SEQ_MOVING + 0x100000*(offset_n)+ 5*(offset_m)); // 0x23130008, 顺序阀1动作时间
        LabelSetText(pwgt_uint1,"s");
        LabelSetText(pwgt_uint2,"s");
        break;
    case 2:
        WGTSetEnable(pwgt_dlytm, TRUE);
        WGTSetEnable(pwgt_acttm, TRUE);
        WGTSetAttrByName(pwgt_dlytm, "text", d_airblow7_MOLDSET_ADPOSI_SEQSTART + 0x100000*(offset_n)+ 5*(offset_m)); // 0x23130009, 顺序阀1动作起始位置
        WGTSetAttrByName(pwgt_acttm, "text", d_airblow7_MOLDSET_ADPOSI_SEQEND + 0x100000*(offset_n)+ 5*(offset_m)); // 0x2313000A, 顺序阀1动作结束位置
        LabelSetText(pwgt_uint1,"mm");
        LabelSetText(pwgt_uint2,"mm");
        break;
    case 3:
        WGTSetEnable(pwgt_dlytm, TRUE);
        WGTSetEnable(pwgt_acttm, TRUE);
        WGTSetAttrByName(pwgt_dlytm, "text", d_airblow7_MOLDSET_TM_SEQ_DELAY + 0x100000*(offset_n)+ 5*(offset_m)); // 0x23130007, 顺序阀1延时
        WGTSetAttrByName(pwgt_acttm, "text", d_airblow7_MOLDSET_ADPOSI_SEQEND + 0x100000*(offset_n)+ 5*(offset_m)); // 0x2313000A, 顺序阀1动作结束位置
        LabelSetText(pwgt_uint1,"s");
        LabelSetText(pwgt_uint2,"mm");
        break;
    case 4:
        WGTSetEnable(pwgt_dlytm, TRUE);
        WGTSetEnable(pwgt_acttm, TRUE);
        WGTSetAttrByName(pwgt_dlytm, "text", d_airblow7_MOLDSET_ADPOSI_SEQSTART + 0x100000*(offset_n)+ 5*(offset_m)); // 0x23130009, 顺序阀1动作起始位置
        WGTSetAttrByName(pwgt_acttm, "text", d_airblow7_MOLDSET_TM_SEQ_MOVING + 0x100000*(offset_n)+ 5*(offset_m)); // 0x23130008, 顺序阀1动作时间
        LabelSetText(pwgt_uint1,"mm");
        LabelSetText(pwgt_uint2,"s");
        break;
    default:
        break;
    }
}

//20250108 fqh 顺序阀 强制输出
static void ForceVlvOutput(int sn)
{
    UI16 pdata[2];

    pdata[0] = sn + VLV_ALLNUM;//20250108 fqh 顺序阀 第二页从顺序阀19开始
    pdata[1] = VarAdrToInt(TMP_VLV_NUM + sn-1);
    ProtSysCtrl(FORCE_VLV_CMD, 2, pdata);
}

//20250108 fqh 顺序阀 刷新顺序阀的输出点状态
static void ShowVlvOutput()
{
    int i,i_n,i_m;
    for(i=0;i<VLV_ALLNUM;i++)
    {
        i_n = i/3;i_m = i%3;
        VarAdrSetInt(TMP_VLV_NUM+i,VarAdrToInt(d_airblow7_PO_O_AIR_SEQVLV1+0x100000*i_n + i_m)); //顺序阀是3个为一小组
    }
}

static void CtlConnect()
{
}
static int Seq_Vlv_2OnInit(Seq_Vlv_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Seq_Vlv_2OnShow(Seq_Vlv_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

    int i,value,offset;
    PWGT pwgt_mode;

    for(i = 0; i < 18; i++)
    {
        offset = 10*(i+1);//下拉框选项索引号是10的倍数
        pwgt_mode = PageFramCurWgtByIndex(offset);
        value = DropGetCurIndex(pwgt_mode);
        SetSeqVlvMode(offset, value);
    }

    return 0;
}
static int Seq_Vlv_2OnHide(Seq_Vlv_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Seq_Vlv_2OnUpdate(Seq_Vlv_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    //20250108 fqh 顺序阀 刷新顺序阀的输出点状态
    ShowVlvOutput();
    return 0;
}
static int Seq_Vlv_2OnChange(Seq_Vlv_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    UI32 addr,value,offset;
    PATTR pattr;

    pattr = (PATTR)msg_para;

    addr = OBJAttrGetAdr(pattr);
    value = VarAdrToInt(addr);
    offset = WGTGetGOffset(pwgt);

    if(offset%10 == 0  && offset < 200)//下拉框选项索引号是10的倍数
    {
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)
        {
            VarSendSaveSetIntByAdr(addr,msg_value);//原始值
            ShowMsg(VW_MSG_MANUAL);
            return -1;
        }

        SetSeqVlvMode(offset, value);
    }
    else if(offset > 200) //20250108 fqh 顺序阀 强制输出
    {
        ForceVlvOutput(offset - 200);
    }

    return 0;
}
