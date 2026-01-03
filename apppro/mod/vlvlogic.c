#include "vlvlogic.h"
#include "protcmd.h"
#include "tablectl.h"
#include "oprintf.h"

E_PO_ROW ElecoutTable[MAX_TBL_ELEC_NUM]; //新电气输出表
static UI16 g_elecouttablenum = 0; //记录上传表的总数

/**
 * @brief      : 接收输出点电气表
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200923
 */
void OnTable_ElecOut()
{
    PPROT_TBL_HD ptb = GetTableByType(TB_O_ELEC, TB_CUR);
    E_PO_ROW* elecout = NULL;

    if(ptb == NULL) return;

    if(ptb->row_len != sizeof(E_PO_ROW)) return;

    elecout = (E_PO_ROW*)(ptb+1);

    memset(ElecoutTable, 0, sizeof(ElecoutTable));

    if(ptb->row_num > MAX_TBL_ELEC_NUM)
    {
        ptb->row_num = MAX_TBL_ELEC_NUM;
    }

    g_elecouttablenum = ptb->row_num;//记录上传表的总数

    memcpy(ElecoutTable, elecout, ptb->row_len*ptb->row_num);
}

/**
 * @brief      : 获取输出点电气表个数
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200923
 */
UI16 Get_ElecoutTable_Num()
{
    return g_elecouttablenum;
}

/**
 * @brief      : 清除阀逻辑表数据，下位机空表时不回复，请求前先清空
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201124
 */
void ClearTable_ElecLogic()
{
    PPROT_TBL_HD ptb = GetTableByType(TB_O_ELEC, TB_CUR);
    E_PO_ROW* elecout = NULL;

    if(ptb == NULL) return;

    if(ptb->row_len != sizeof(E_PO_ROW)) return;

    elecout = (E_PO_ROW*)(ptb+1);

    memset(ElecoutTable, 0, sizeof(ElecoutTable));
    g_elecouttablenum = 0; //记录上传表的总数

    memcpy(elecout, ElecoutTable, ptb->row_len*ptb->row_num);
}
