#ifndef VLVLOGIC_H
#define VLVLOGIC_H
#include "typedef.h"
#include "obj.h"
#include "wgt.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_E_PO_TBL 6
#define MAX_E_PO_FORK_HH 1 //比较项最大数目
#define MAX_TBL_ELEC_NUM 10 //新电气输出表最大个数 若大于10则要请求多个表

//电气表的输出点
typedef struct tyE_PO
{
    UI16 art_name;      //15-4位为部件ID, 3-0填0
    UI16 po_ind;        //输出点编号,只用低8位。
    UI16 onoff_delay;   //低8为输出点输状态及延时; 高8为做关闭操作时延时，以10ms单位
}E_PO, *PE_PO;

typedef struct tyEXP_VAR_PO
{
    UI16 type;      // 最高位置1表示为有符号数，低位& 0x7fff = VAR_TYPE类型,
    UI16 sub;       //部件子类号
    UI16 ind;       //CMP_CONST为常数的具体值，其他为索引值
    UI16 ref_id;    //变量所引用的部件ID
}EXP_VAR_PO, *PEXP_VAR_PO;

//逻辑比较项
typedef struct tyFORK_CMP_PO
{
    EXP_VAR_PO var1;    //参数1
    EXP_VAR_PO var2;    //参数2
    UI16 exp_op;        //参数1与参数2逻辑比较符 EXP_OP_TYPE
    UI16 lg_op;         //与前比较项的逻辑运算 LOGIC_TYPE
    UI16 rev[2];        //预留
}FORK_CMP_PO, *PFORK_CMP_PO;

//电气表项
typedef struct tyE_PO_ROW
{
    UI16 act_id;   			//动作ID
    UI16 step;     			//动作区间（HI8开始step,LOW8结束step）
    E_PO e_po[MAX_E_PO_TBL];		//电气阀自定义状态和编号
    UI16 add_condition; 		/*6个阀是否需要以下附加条件，按位0-5位代表6个阀*/
    UI16 en_oper_mode;                  /*0xffff表示忽略操作模式检查	,非0xffff则采用==比较操作  */
    UI16 cmp_num_po;                    /*比较项数目最大1个比较项*/
    FORK_CMP_PO cmp[MAX_E_PO_FORK_HH];  /*比较项，最大1个*/
}E_PO_ROW, *PE_PO_ROW;

void OnTable_ElecOut();
UI16 Get_ElecoutTable_Num();
void ClearTable_ElecLogic();

extern E_PO_ROW ElecoutTable[MAX_TBL_ELEC_NUM]; //新电气输出表

#define VLV_LOGIC_FILE  DATA_PATH"vlv_logic.map"

#ifdef __cplusplus
}
#endif
#endif // VLVLOGIC_H
