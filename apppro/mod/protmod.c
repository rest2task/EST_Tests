#include "protcmd.h"
#include "curvesctl.h"
#include "monmcu.h"
#include "keymod.h"
#include "mainframe.h"
#include "tablectl.h"
#include "protmod.h"
#include "ctrlcmd.h"
#include "protcmd.h"
#include "warndatarec.h"
#include "selfstudy.h"
#include "opencal.h"
#include "ctrlcmd.h"

static BOOL g_bMainHand = FALSE;//接收主机握手信号标记

BOOL b_RecMainNeedReStartCmd = FALSE;//20230518 chj 收到主机请求重启命令，弹出重启框，重启主机
BOOL b_RecMainNeedPO_TIMEOUT_EXIT_CMD = FALSE;//IO强制输出超时，通知面板清除IO强制输出表 20250305 chj
/**
* @brief     :数据表接收处理函数
* @param     :包数据 数据长度
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
BOOL ProtTBLRspRx(PPROT_TBL_PKT ppkt, I32 dat_len)
{
	PPROT_TBL_HD phd;

	if(ppkt == NULL)
		return FALSE;

    if(ppkt->type == TB_OPENCAL)//20200727.cyx AI记忆开模算法
    {
        TableRecCtl(ppkt,dat_len,MAX_OPENROWLENTH);
        return TRUE;
    }

	/*检查是否是一个完整的表*/
	if(ppkt->start_row == 0)
	{
		phd = (PPROT_TBL_HD)(ppkt + 1);
		if(phd->type != ppkt->type ||phd->sn != ppkt->sn)
			return FALSE;

		if(phd->row_num > ppkt->row_num)
			return FALSE;

		if((UI32)dat_len < phd->row_num*phd->row_len + sizeof(PROT_TBL_HD) + sizeof(PROT_TBL_PKT))
			return FALSE;

        /*need to do, send tbl to upper layer*/
        TableRecCtl(ppkt,dat_len,phd->row_len);
        return TRUE;
	}
    return FALSE;
}

/**
 * @brief      : 请求数据表处理函数
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200727
 */
#include "oprintf.h"
BOOL ProtTBLReqRx(PPROT_TBL_PKT ppkt, I32 dat_len)
{
    PPROT_TBL_HD ptb = NULL;

    if(ppkt == NULL)
        return FALSE;

    oprintf("############ ProtTBLReqRx ppkt->type=%d,ppkt->sn=%d ##########\n",ppkt->type,ppkt->sn);

    if(ppkt->type != TB_OPENCAL)//目前特殊处理，不做标准处理，以后做标准去掉此条件即可
    {
        return FALSE;
    }

    ptb = GetTableByType(ppkt->type,ppkt->sn);
    if(ptb == NULL)
    {
        if(ppkt->type == TB_OPENCAL)
        {
            ptb = (PPROT_TBL_HD)OBJMemMalloc(MAX_OPENTABROW * MAX_OPENROWLENTH+sizeof(PROT_TBL_HD));
            UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};
            if(!LoadOpenCalRec(wTabData))
            {
                SaveOpenCalRec(wTabData,MAX_OPENTABROW);
            }
            ptb->row_len = MAX_OPENROWLENTH;
            ptb->row_num = MAX_OPENTABROW;
            ptb->type = ppkt->type;
            ptb->sn = (UI8)(ppkt->sn);
            memcpy((UI8*)(ptb+1), wTabData, MAX_OPENTABROW * MAX_OPENROWLENTH);
            AddOpenCalTable(ptb);
        }
    }

    ProtSendTbl(ptb);//20200727.cyx 发送AI记忆开模算法表
    return TRUE;
}

/**
* @brief     :收到主机控制面板命令
* @param     :包数据 数据长度
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :chj
* @date      :20230518 chj
*/
int RxCmd_Main_Ctrl_Panel_Data(UI16* pdata)
{
    if(pdata == NULL)
    {
        return FALSE;
    }
    if(pdata[0] == CFG_CHANGE_NEED_RESTART_CMD)
    {
        b_RecMainNeedReStartCmd = TRUE;
    }
    else if(pdata[0] == PO_TIMEOUT_EXIT_CMD)//IO强制输出超时，通知面板清除IO强制输出表 20250305 chj
    {
        b_RecMainNeedPO_TIMEOUT_EXIT_CMD = TRUE;
    }
    else {
        ;
    }
    return TRUE;
}

/**
* @brief     :应用层数据包接收函数,可自定义协议命令
* @param     :phd 包头 prot_id 下位机为0
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static UI32 BootTimeStart = 0;
void ModPktRecv(PPROT_HD phd, UI8 prot_id)
{
	switch(phd->cmd & CMD_BITS)
	{
		case BOOTUP_MSG:
            if((GetTick()-BootTimeStart) > (1000*2))//20230518 chj 防止多次接收，2s收取下主机的bootup命令
            {
                PPROT_BOOTUP_MSG_PKT ppkt = (PPROT_BOOTUP_MSG_PKT)(phd + 1);
                //db_printf("Bootup message, type = %d, error = %d\r\n", ppkt->boot_type, ppkt->error_bits);
                //ProtDataRspRx((PROT_BOOTUP_MSG_PKT)(phd + 1), phd->id);
                BootUpMsgRx(ppkt->boot_type, ppkt->error_bits, ppkt->hw_ver, ppkt->sw_ver);
                BootTimeStart = GetTick();
            }
            g_bMainHand = TRUE;
			break;

		case DATASET_RSP_CMD:
			{
				PPROT_DATSET_RSP_PKT ppkt = (PPROT_DATSET_RSP_PKT)(phd + 1);
				//db_printf("DATASET_RSP_CMD recv, set_id = %d\r\n", ppkt->set_id);
				DataSetRx(ppkt->set_id, (((UI8*)phd + phd->len - PROT_TAIL_LEN) -  (UI8*)(ppkt + 1))/2,(UI16*)(ppkt + 1));
			}
			break;

		case DATREC_RSP_CMD:
			DataRecRspRx((PPROT_DATREC_RSP_PKT)(phd + 1));
			break;

		case TSK_ST_MSG:
			//db_printf("TSK_ST_MSG recv, num = %d\r\n", ((PPROT_TSK_ST_MSG_PKT)(phd + 1))->num);
			//PackPktRx(PackRxTypeFind(((PPROT_PACK_PKT)(phd + 1))->type_id), phd->id, phd->len - PROT_PACK_LEN, (PPROT_PACK_PKT)(phd + 1));
			ActCmdRx((PPROT_TSK_ST_MSG_PKT)(phd + 1));
			break;

		case KEYPAD_CMD:
			/*receive key from MCU, just only manualkey now*/			
			MCUKeyRx(((PPROT_KEYPAD_RSP_PKT)(phd + 1))->keycode);
			break;
        case TBL_REQ_CMD:
            ProtTBLReqRx((PPROT_TBL_PKT)(phd + 1), phd->len - PROT_PACK_LEN);
            break;
		case TBL_RSP_CMD:
			/*db_printf("TBL_RSP_CMD recv, type = %04x, sn = %d, start= %d, num = %d\r\n", ((PPROT_MTBL_REQ_PKT)(phd + 1))->req_num,  ((PPROT_TBL_PKT)(phd + 1))->sn,
					  ((PPROT_TBL_PKT)(phd + 1))->start_row, ((PPROT_TBL_PKT)(phd + 1))->row_num);*/
			ProtTBLRspRx((PPROT_TBL_PKT)(phd + 1), phd->len - PROT_PACK_LEN);
			break;
        case COMM_RX_ALARM_DATA://20191018.cyx hz下位机警报功能
            RxCmd_WarnData((UI16*)(phd+1));
            break;
        case COMM_RX_DATA_RSP_CMD:
            RxCmd_MoniDat((UI16*)(phd+1));
            break;
        case COMM_RX_SELFSTUDY:
            RxCmd_Study((UI16*)(phd+1));
            break;
        case SYS_MAIN_CTRL_PANELT_CMD://20230518 chj 收到主机请求重启命令，弹出重启框，重启主机
            RxCmd_Main_Ctrl_Panel_Data((UI16*)(phd+1));
            break;
		default:
			break;
	}
}

//获取主机握手信号标记
BOOL GetMainHand()
{
    return g_bMainHand;
}

//重置主机握手信号标记
void InitMainHand()
{
    g_bMainHand = FALSE;
}
