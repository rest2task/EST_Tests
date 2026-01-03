/******************************************************************************
  文 件 名   : protcmd.c
  版 本 号   : 初稿
  作    者   : ren chaohong
  生成日期   : 2013年5月17日
  最近修改   :
  功能描述   : 通信命令处理
  函数列表   :
              PackPktReqRetry
              PackPktRx
              PackPktRxEnd
              PackReqRx
              PackRxDataProc
              PackRxFail
              ProtAckRx
              ProtAckTx
              ProtDataReqRx
              ProtDataRspRx
              ProtPktRecv
              ProtSubReqRx
              ProtSubRspRx
              SendPackPktReq
  修改历史   :
  1.日    期   : 2013年5月17日
    作    者   : ren chaohong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "com.h"
#include "part.h"
#include "prot.h"
#include "protcmd.h"
#include "keycmd.h"
#include "monmmi.h"
#include "cfgdb.h"
#include "tbl.h"
#include "systempro.h"
#include "printf.h"
#include "actfork.h"
#include "ServoEcat.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern UI16 sys_rcv_flag;
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
BOOL ServoDataReqRx(PPRO_SDO_RX_PKT_HD ppkt, UI16 pkt_id);
BOOL ServoDataRspRx(PPRO_SDO_RX_PKT_HD ppkt, UI16 pkt_id);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
PACK_PKT_RX_ST pack_pkt_rx_st;
PACK_PKT_TX_ST pack_pkt_tx_st;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

__inline void ProtAckTx(UI16 rx_cmd, UI16 rx_id, UI8 prot_id)
{
    PPROT_ACK_PKT	ppkt;

    if ((ppkt = (PPROT_ACK_PKT)ProtNewPkt(ACK_CMD, rx_id, FALSE, sizeof(PROT_ACK_PKT))) != NULL)
    {
        ppkt->recv_cmd = rx_cmd;
        ppkt->recv_id = rx_id;
        ProtTxPkt(ppkt, prot_id);
    }
}

__inline void ProtAckRx(PPROT_ACK_PKT ppkt, UI16 id, UI8 prot_id)
{
    /*Neglect ACK part id*/
    ProtAckHandle(ppkt->recv_cmd, ppkt->recv_id, prot_id);
}

__inline void ProtSubReqRx(PPROT_SUB_REQ_PKT ppkt, UI16 id)
{
    BOOL re = FALSE;
    
    re = PartSubTx(GetPartByID(id), ppkt->sub_id_bits, TRUE);
    if((re == TRUE) && (id == MK_PART(MACHINE_ID, 1)) && ((((ppkt->sub_id_bits)>>MH_ID)&0x01) == 0x01))
    {   
        //QT面板请求部件数据用0x01命令
		if(sys_rcv_flag == 0)
			sys_rcv_flag = 0x01;
	}
}

static __inline void ProtDataReqRx(PPROT_DAT_REQ_PKT ppkt, UI16 id)
{
    BOOL re = FALSE;
    
    re = PartDataTx(GetPartByID(id), SUB_IND_ID(ppkt->sub_ind), SUB_IND_LEN(ppkt->sub_ind), ppkt->offset, TRUE);
    if((re == TRUE) && (id == MK_PART(MACHINE_ID, 1)) && (SUB_IND_ID(ppkt->sub_ind) == MH_ID) && (SUB_IND_LEN(ppkt->sub_ind) > 2))
	{   
	    //HMI面板请求部件数据用0x03命令
		if(sys_rcv_flag == 0)
			sys_rcv_flag = 0x01;
	}
}

static __inline void ProtSubRspRx(PPROT_SUB_RSP_PKT ppkt, UI16 id)
{
    PPART pp;
    UI16*	pn;
    UI16		num;
    int	i;

    if ((pp = GetPartByID(id)) == NULL)
    {
        return;
    }

    num = ppkt->sub_num;
    pn = ppkt->sub_ind + num;

    if (num > 0)
    {
        for (i = 0; i < num; i++)
        {
            PartDataRx(pp, SUB_IND_ID(ppkt->sub_ind[i]), SUB_IND_LEN(ppkt->sub_ind[i]), 0, pn, MMI_SUB_RX);
            pn += SUB_IND_LEN(ppkt->sub_ind[i]);
        }
    }
    else
    {
        /*接收到空数据，表示面板没有该部件*/
        PartNUllSubRx(pp, MMI_SUB_RX);
    }
}

static __inline void ProtDataRspRx(PPROT_DAT_RSP_PKT ppkt, UI16 id)
{
    PartDataRx(GetPartByID(id), SUB_IND_ID(ppkt->sub_ind), SUB_IND_LEN(ppkt->sub_ind), ppkt->offset, ppkt->sub_data, MMI_DATA_RX);
}

BOOL SendPackPktReq(PPACK_PKT_RX_ST pst, BOOL back)
{
    PPROT_PACK_PKT ppkt;

    if (pst == NULL)
    {
        return FALSE;
    }

    if ((ppkt = (PPROT_PACK_PKT)ProtNewPkt(PKT_REQ_CMD, pst->next_pkt_sn, back, sizeof(PROT_PACK_PKT))) != NULL)
    {
        ppkt->type_id = pst->type_id;
        pst->last_req_tx_10ms = Get10MsTick();
        return MainProtTxPkt(ppkt);
    }

    return FALSE;
}

void PackRxFail(PPACK_PKT_RX_ST pst)
{
    if (pst != NULL && pst->bstart)
    {
        if (pst->type_id > CFG_DB_PACK_BEGIN_TYPE)
        {
            CfgDbUpdateFail((CFG_DB_ID)(pst->type_id - CFG_DB_PACK_BEGIN_TYPE), pst);
        }
    }
}

BOOL PackRxDataProc(PPACK_PKT_RX_ST pst)
{
    if (pst->type_id > CFG_DB_PACK_BEGIN_TYPE &&  pst->type_id < CFG_DB_PACK_END_TYPE)
    {
        return CfgDbUpdate(pst);
    }
    else if (pst->type_id == MMI_PARA_FILE_TYPE)
    {
        return 	MMIParaFileSave(pst);
    }
    else
    {
        return FALSE;
    }
}

void PackPktRxEnd(PPACK_PKT_RX_ST pst)
{
    if (pst == NULL)
    {
        return;
    }

    if (pst->pcache != NULL)
    {
        FreeCache(pst->pcache);
    }

    pst->next_pkt_sn = PACK_FIN_SN;
    SendPackPktReq(pst, TRUE);
    memset(pst, 0, sizeof(*pst));
}

void PackPktReqRetry(PPACK_PKT_RX_ST pst)
{
    if (pst->bstart)
    {
        if (pst->req_retry <= PROT_TX_RETRY && (UI16)(Get10MsTick() - pst->last_req_tx_10ms) > (PROT_ACK_MIN_WAITE / 10 << (pst->req_retry - 1)))
        {
            SendPackPktReq(pst, FALSE);
            pst->req_retry++;
        }
        else if (pst->req_retry > PROT_TX_RETRY)
        {
            /*Fail*/
            PackRxFail(pst);
            PackPktRxEnd(pst);
        }
    }
}

void PackPktRx(PPACK_PKT_RX_ST pst, UI16 pkt_sn, int len, PPROT_PACK_PKT ppkt)
{
    UI8* psrc;

    if (ppkt == NULL || pst == NULL)
    {
        return;
    }

    if (!pst->bstart || pst->type_id != ppkt->type_id || pst->pcache == NULL)
    {
        if (!pst->bstart || pst->pcache == NULL)
        {
            if ((pst->pcache = GetCache(&pst->cache_size)) != NULL)
            {
                pst->cache_fill = 0;
            }
            else
            {
                PackRxFail(pst);
                PackPktRxEnd(pst);
                return;
            }
        }

        if (pst->type_id != ppkt->type_id)
        {
            PackRxFail(pst);
        }

        pst->bstart = TRUE;
        pst->type_id = ppkt->type_id;
        pst->cur_len = 0;
        pst->next_pkt_sn = 0;
        pst->last_rx_10ms = 0;
        pst->last_req_tx_10ms = 0;
        pst->req_retry = 0;
        pst->total_pkt = 2;
        pst->total_len = 2 * 1024;
    }

    if (pkt_sn == pst->next_pkt_sn)
    {
        if (pkt_sn == 0)
        {
            pst->total_pkt = ((PPROT_FIRST_PACK_PKT)ppkt)->total_pkt_num;
            pst->total_len = ((PPROT_FIRST_PACK_PKT)ppkt)->total_data_len;
            len -= sizeof(PROT_FIRST_PACK_PKT);
            psrc = (UI8*)((PPROT_FIRST_PACK_PKT)ppkt + 1);
            if (pst->pcache == NULL)
            {
                pst->cache_size = pst->total_len;
                pst->pcache = GetCache(&pst->cache_size);
            }
        }
        else
        {
            len -= sizeof(PROT_PACK_PKT);
            psrc = (UI8*)((PPROT_PACK_PKT)ppkt + 1);
        }

        if (pst->pcache != NULL && pst->cache_size > len)
        {
            if (pst->cache_size < pst->cache_fill + len)
            {
                if (pst->cache_fill < pst->cache_size)
                {
                    UI16 tmp_len;

                    /*填满整个缓存*/
                    tmp_len = pst->cache_size - pst->cache_fill;
                    memcpy(pst->pcache + pst->cache_fill, psrc, tmp_len);
                    psrc += tmp_len;
                    pst->cur_len += tmp_len;
                    len -= tmp_len;
                    pst->cache_fill = pst->cache_size;
                }

                /*write data*/
                if (!PackRxDataProc(pst))
                {
                    /*数据处理出错或完成，结束接收*/
                    PackPktRxEnd(pst);
                    return;
                }

                pst->cache_fill = 0;
            }

            memcpy(pst->pcache + pst->cache_fill, psrc, len);
            pst->cache_fill += len;
            pst->cur_len += len;
            pst->next_pkt_sn++;

            if (pst->next_pkt_sn >= pst->total_pkt || pst->cur_len >= pst->total_len )
            {
                /*最后一包数据，write data*/
                if (PackRxDataProc(pst))
                {
                    PackRxFail(pst);
                }

                PackPktRxEnd(pst);
            }
            else
            {
                SendPackPktReq(pst, FALSE);
                pst->req_retry = 1;
                pst->last_rx_10ms = Get10MsTick();
            }
        }
        else /*无cache, 接收结束*/
        {
            PackPktRxEnd(pst);
        }
    }
}

void PackReqRx(PPACK_PKT_TX_ST pst, UI16 pkt_sn, UI16 type_id)
{
    UI8* ppkt;
    int tmp_len;

    if (pst == NULL)
    {
        return;
    }

    if (!pst->bstart && pkt_sn != 0)
    {
        return;
    }

	if(pkt_sn != 0 && (pst->type_id != type_id  || (pkt_sn >= pst->pkt_num && pkt_sn != PACK_FIN_SN)))
    {
        return;
    }


    if (pkt_sn == PACK_FIN_SN)
    {
        if (pst->bstart)
        {
            memset(pst, 0, sizeof(*pst));
        }

        return;
    }

    if (pkt_sn == 0)
    {
        if ((!pst->bstart) || pst->type_id != type_id)
        {
            /*如果读主机的配置数据，需要先读取到缓存*/
            if (type_id >= CFG_DB_PACK_BEGIN_TYPE && type_id < CFG_DB_PACK_END_TYPE)
            {
                if (pst->pcache == NULL)
                {
                    pst->pcache = GetCache(&pst->cache_size);
                }

                if (pst->pcache == NULL)
                {
                    return;
                }

                if (!CfgDbUpload((CFG_DB_ID)(type_id - CFG_DB_PACK_BEGIN_TYPE), pst))
                {
                    FreeCache(pst->pcache);
                    return;
                }

                //释放缓存，在整个上传过程中应确保其他函数不会再使用数据库缓存。
                FreeCache(pst->pcache);
            }
            else if (type_id == MMI_PARA_FILE_TYPE)
            {
                if (!MMIParaFileRead(pst))
                {
                    db_printf("No para file or file check error\r\n");
                    return;
                }
            }
            else
            {
                return;
            }

            pst->type_id = type_id;
            pst->bstart = TRUE;
            pst->pkt_num = pst->len / PACK_PKT_SIZE;
            pst->last_pkt_len = pst->len % PACK_PKT_SIZE;

            /*总的数据长度为0，也发一个数据长度为0 的包*/
            if (pst->last_pkt_len > 0 || pst->len == 0)
            {
                pst->pkt_num++;
            }
            else
            {
                pst->last_pkt_len = PACK_PKT_SIZE;
            }

            db_printf("pack file: pkt_num = %d, len = %d, last_pkt_len = %d\r\n", pst->pkt_num, pst->len, pst->last_pkt_len);
        }

        tmp_len = sizeof(PROT_FIRST_PACK_PKT);
    }
    else
    {
        tmp_len = sizeof(PROT_PACK_PKT);
    }

    if (pkt_sn + 1 == pst->pkt_num)
    {
        tmp_len += pst->last_pkt_len;
    }
    else
    {
        tmp_len += PACK_PKT_SIZE;
    }

    if ((ppkt = (UI8*)ProtNewPkt(PKT_RSP_CMD, pkt_sn, FALSE, tmp_len)) == NULL)
    {
        return;
    }


    if (pkt_sn + 1 == pst->pkt_num)
    {
        tmp_len = pst->last_pkt_len;
    }
    else
    {
        tmp_len = PACK_PKT_SIZE;
    }

    ((PPROT_PACK_PKT)ppkt)->type_id = type_id;
    if (pkt_sn == 0)
    {
        ((PPROT_FIRST_PACK_PKT)ppkt)->total_pkt_num = pst->pkt_num;
        ((PPROT_FIRST_PACK_PKT)ppkt)->total_data_len = pst->len;
        memcpy(ppkt + sizeof(PROT_FIRST_PACK_PKT), pst->pcache + pkt_sn * PACK_PKT_SIZE, tmp_len);
    }
    else
    {
        memcpy(ppkt + sizeof(PROT_PACK_PKT), pst->pcache + pkt_sn * PACK_PKT_SIZE, tmp_len);
    }

    MainProtTxPkt(ppkt);
}

void SendPack_Test(UI16 rx_cmd, UI16 rx_id, UI8 prot_id)		//通讯测试命令，被动接收，面板发一包，回一包
{
	PPROT_DAT_TEST	ppkt;

	if ((ppkt = (PPROT_DAT_TEST)ProtNewPkt(COMM_TX_TEST_UDP, rx_id, FALSE, sizeof(PPROT_DAT_TEST))) != NULL)
	{
		ppkt->recv_cmd = rx_cmd;
		ppkt->recv_id = rx_id;
		ProtTxPkt(ppkt, prot_id);
	}
}

void ProtPktRecv(PPROT_HD phd, UI8 prot_id)
{
    if ((phd->cmd & CMD_ACK_BIT) && phd->cmd != ACK_CMD)
    {
        ProtAckTx(phd->cmd, phd->id, prot_id);
    }

    switch (phd->cmd & CMD_BITS)
    {
        case ACK_CMD:
            //db_printf("ACK_CMD recv, id = %04x\r\n", phd->id);
            ProtAckRx((PPROT_ACK_PKT)(phd + 1), phd->id, prot_id);
            break;
        case SUB_REQ_CMD:
            db_printf("SUB_REQ_CMD recv, id = %04x\r\n", phd->id);
            ProtSubReqRx((PPROT_SUB_REQ_PKT)(phd + 1), phd->id);
            break;
        case DAT_REQ_CMD:
            db_printf("DAT_REQ_CMD recv, id = %04x\r\n", phd->id);
            ProtDataReqRx((PPROT_DAT_REQ_PKT)(phd + 1), phd->id);
            break;
        case SUB_RSP_CMD:
            db_printf("SUB_RSP_CMD recv, id = %04x\r\n", phd->id);
            ProtSubRspRx((PPROT_SUB_RSP_PKT)(phd + 1), phd->id);
            break;
        case SUB_ACTV_CMD:
            db_printf("SUB_ACTV_CMD recv, id = %04x\r\n", phd->id);
            ProtSubRspRx((PPROT_SUB_RSP_PKT)(phd + 1), phd->id);
            break;
        case DAT_RSP_CMD:
            db_printf("DAT_RSP_CMD recv, id = %04x\r\n", phd->id);
            ProtDataRspRx((PPROT_DAT_RSP_PKT)(phd + 1), phd->id);
            break;
        case DAT_ACTV_CMD:
            db_printf("DAT_ACTV_CMD recv, id = %04x\r\n", phd->id);
            ProtDataRspRx((PPROT_DAT_RSP_PKT)(phd + 1), phd->id);
            break;
        case KEYPAD_CMD:
            db_printf("KEY, %x, %x, %d\r\n", ((PPROT_KEYPAD_CMD_PKT)(phd + 1))->keycode, ((PPROT_KEYPAD_CMD_PKT)(phd + 1))->keystatus, Get100MsTick());
            ComKeyRecv((PPROT_KEYPAD_CMD_PKT)(phd + 1));
            break;
        case TBL_REQ_CMD://表上传到面板
            db_printf("TBL_REQ_CMD recv, type = %04x, sn = %d, start= %d, num = %d\r\n", ((PPROT_TBL_PKT)(phd + 1))->type,  ((PPROT_TBL_PKT)(phd + 1))->sn,
                      ((PPROT_TBL_PKT)(phd + 1))->start_row, ((PPROT_TBL_PKT)(phd + 1))->row_num);
            TBLReqRx((PPROT_TBL_PKT)(phd + 1));
            break;
        case TBL_RSP_CMD://表下发到主机
            db_printf("TBL_RSP_CMD recv, type = %04x, sn = %d, start= %d, num = %04x \r\n", ((PPROT_TBL_PKT)(phd + 1))->type,  ((PPROT_TBL_PKT)(phd + 1))->sn,
                      ((PPROT_TBL_PKT)(phd + 1))->start_row, ((PPROT_TBL_PKT)(phd + 1))->row_num);

            TBLRspRx((PPROT_TBL_PKT)(phd + 1), phd->len - PROT_PACK_LEN);
            break;

        case MULTI_TBL_REQ_CMD:
            db_printf("MULTI_TBL_REQ_CMD recv, req_num = %d\r\n", ((PPROT_MTBL_REQ_PKT)(phd + 1))->req_num);
            break;

        case MULTI_TBL_RSP_CMD:
            db_printf("MULTI_TBL_RSP_CMD recv, req_num = %d\r\n", ((PPROT_MTBL_REQ_PKT)(phd + 1))->req_num);
            break;

        case DATASET_REQ_CMD:
            db_printf("DATASET_REQ_CMD recv, set_id = %d, period_ms=%d, ind_num = %d\r\n", ((PPROT_DATSET_REQ_PKT)(phd + 1))->set_id, ((PPROT_DATSET_REQ_PKT)(phd + 1))->period_ms,
                      ((PPROT_DATSET_REQ_PKT)(phd + 1))->ind_num);
            DataSetCmdRx((PPROT_DATSET_REQ_PKT)(phd + 1));
            break;

        case DATASET_RSP_CMD:
            db_printf("DATASET_RSP_CMD recv, set_id = %d\r\n", ((PPROT_DATSET_RSP_PKT)(phd + 1))->set_id);
            break;

        case DATREC_REQ_CMD:
            db_printf("DATREC_REQ_CMD recv, rec_id = %d, %x, %d, %d, %d\r\n", ((PPROT_DATREC_REQ_PKT)(phd + 1))->rec_id, ((PPROT_DATREC_REQ_PKT)(phd + 1))->trigger_id,
                      ((PPROT_DATREC_REQ_PKT)(phd + 1))->rec_period, ((PPROT_DATREC_REQ_PKT)(phd + 1))->rec_total_ms, ((PPROT_DATREC_REQ_PKT)(phd + 1))->ind_num);
            DataRecCmdRx((PPROT_DATREC_REQ_PKT)(phd + 1), prot_id, phd->len,DATREC_REQ_CMD);		//prot_id为0，代表是面板发下来的数据   1  代表是pc示波器发下来的数据
            break;

        case DATREC_OP_CMD:
            db_printf("DATREC_OP_CMD recv, rec_id = %d, op =%d\r\n", ((PPROT_DATREC_OP_PKT)(phd + 1))->rec_id, ((PPROT_DATREC_OP_PKT)(phd + 1))->op_type);
            DataRecOPRx((PPROT_DATREC_OP_PKT)(phd + 1), prot_id);
            break;
			
		case DATREC_QXRC_CMD:		//后台监控数据接收命令，为pc机电脑上的
			DataRecCmdRx((PPROT_DATREC_REQ_PKT)(phd + 1), prot_id, phd->len,DATREC_QXRC_CMD);
			break;		

		case DATREC_QXOP_CMD:		//后台监控数据停止命令 ，为PC电脑发送给面板，面板再发给主机	
            db_printf("DATREC_QXOP_CMD recv, rec_id = %d, op =%d\r\n", ((PPROT_DATREC_OP_PKT)(phd + 1))->rec_id, ((PPROT_DATREC_OP_PKT)(phd + 1))->op_type);
            DataRec_Stop((PPROT_DATREC_OP_PKT)(phd + 1), prot_id);
            break;
			
        case PKT_REQ_CMD:
            db_printf("PKT_REQ_CMD recv, pkt_sn = %d, type_id%04x, prot_id = %d\r\n", phd->id, ((PPROT_PACK_PKT)(phd + 1))->type_id, prot_id);
            PackReqRx(&pack_pkt_tx_st, phd->id,   ((PPROT_PACK_PKT)(phd + 1))->type_id);
            break;
        case PKT_RSP_CMD:
            db_printf("PKT_RSP_CMD recv, pkt_sn = %d, type_id%04x, port_id = %d\r\n", phd->id, ((PPROT_PACK_PKT)(phd + 1))->type_id, prot_id);
            PackPktRx(&pack_pkt_rx_st, phd->id, phd->len - PROT_PACK_LEN, (PPROT_PACK_PKT)(phd + 1));
            break;
        case SYS_CTRL_CMD://小命令语句
            db_printf("SYS_CTRL_CMD recv, ctrl_cmd = %04x, data_len = %04x\r\n", ((PPROT_CTRL_CMD_PKT)(phd + 1))->ctrl_cmd, ((PPROT_CTRL_CMD_PKT)(phd + 1))->data_len);
            SysAdjCfgCmdRx(((PPROT_CTRL_CMD_PKT)(phd + 1))->ctrl_cmd, ((PPROT_CTRL_CMD_PKT)(phd + 1))->data_len, (UI16*)((PPROT_CTRL_CMD_PKT)(phd + 1) + 1));
            break;

        case UPGRADE_RESET_CMD:
            db_printf("UPGRADE_RESET_CMD recv, type: %d\r\n", *(UI16*)(phd + 1));
            UpgradeCmdRx(*(UI16*)(phd + 1));
            break;
        case PKT_SERVO_REQ_CMD://读驱动器数据
            db_printf("PKT_SERVO_REQ_CMD recv, id = %d\r\n", phd->id);
			if(phd->len > 0x0A)
			{
				ServoDataReqRx((PPRO_SDO_RX_PKT_HD)(phd + 1), phd->id);
			}
            else
            {
				db_printf("PKT_SERVO_REQ_CMD recv data is err len = %d\r\n", phd->len);
			}
            break;
        case PKT_SERVO_RSP_CMD://写驱动器数据
            db_printf("PKT_SERVO_RSP_CMD recv, id = %d\r\n", phd->id);
			if(phd->len > 0x0A)
			{
            	ServoDataRspRx((PPRO_SDO_RX_PKT_HD)(phd + 1), phd->id);
			}
			else
			{	
				db_printf("PKT_SERVO_RSP_CMD recv data is err len = %d\r\n", phd->len);
			}
            break;

		case COMM_RX_TEST_UDP:
			SendPack_Test(phd->cmd, phd->id, prot_id);
			break;
		case MOTOR_TURNING_CMD:
			db_printf("MOTOR_TURNING_CMD recv, id = %d\r\n", phd->id);
			if(phd->len > 0x0A)
			{
            	ServoSdoTurning((PPRO_SDO_RX_PKT_HD)(phd + 1), phd->id);
			}
			else
			{	
				db_printf("PKT_SERVO_RSP_CMD recv data is err len = %d\r\n", phd->len);
			}
			break;
			
        default:
            db_printf("unknow cmd recv, id = %04x\r\n", phd->id);
            break;
    }
}

