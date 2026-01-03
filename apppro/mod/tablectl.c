#include "tablectl.h"
#include "stdio.h"
#include "protcmd.h"
#include <string.h>
#include "oprintf.h"
#include "strhash.h"
#include "part.h"
#include "ioredef.h"
#include "command.h"
#include "panelcfg.h"
#include "database.h"
#include "opencal.h"

#define TABLE_MAX_CNT			(10)		//表最大个数
#define TABLE_HDFLAG			(0)			//表头标志
#define TABLE_IO_ROW_LEN		(6)
#define TABLE_IO_CNT			(128)
#define PART_IO					0x0616

#define IO_POINT_CNT			(8)

#define MAX_SERVO_CURRENT_NUM   6//最大伺服个数

static UI16 oldinData[IO_POINT_CNT]={0};
static UI16 oldoutData[IO_POINT_CNT]={0};

static TABLELIST g_tableList;

static PPROT_TBL_HD CreateNewTable(PPROT_TBL_PKT ppkt, I32 dat_len);
static void AddTable(PTABLELIST plist, PPROT_TBL_HD ptb);
static void TableWrite(PPROT_TBL_HD ptb,UI16 startrow,UI16 rownum ,UI8 * data);
UI16 g_pumpUpdateFlag;

void TableRecCtl(PPROT_TBL_PKT ppkt, I32 dat_len,I32 row_len)
{
	PPROT_TBL_HD ptb = NULL;
	UI16 startRow = 0;
	ptb = GetTableByType(ppkt->type,ppkt->sn);
    oprintf("recive type=%d sn=%d start_row=%d row_num=%d dat_len=%d row_len=%d\n",ppkt->type,ppkt->sn,ppkt->start_row,ppkt->row_num,dat_len,row_len);
    if(ppkt->type == TB_OPENCAL)//20200727.cyx AI记忆开模算法
    {
        UI16 wTabData[MAX_OPENTABROW * (MAX_OPENROWLENTH/2)]= {0};

        if(ptb == NULL)
        {
            ptb = (PPROT_TBL_HD)OBJMemMalloc(MAX_OPENTABROW * MAX_OPENROWLENTH+sizeof(PROT_TBL_HD));
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
        startRow = ppkt->start_row;//0表示第一行数据,1表示第二行数据...
        //下位机上传的数据不包含PROT_TBL_HD表头,与输入输出表等区分
        memcpy((UI8*)(ptb+1)+(startRow*MAX_OPENROWLENTH), ((UI8 *)ppkt)+sizeof(PROT_TBL_PKT), ppkt->row_num*MAX_OPENROWLENTH);

        //AI记忆开模算法，数据单元12个UI16 最大行数20行
        memcpy(wTabData, (UI8*)(ptb+1), MAX_OPENTABROW*MAX_OPENROWLENTH);
        SaveOpenCalRec(wTabData, MAX_OPENTABROW);
        return;
    }
    else if(ppkt->type == TB_PUMP) //20241223 fqh 接收pump表
    {
        if(ptb == NULL)
        {
            int size = row_len*ppkt->row_num + sizeof(PROT_TBL_PKT);
            if(dat_len < (size + sizeof(PROT_TBL_HD)))
                return;
            if((ptb = CreateNewTable(ppkt,size) )== NULL)
            {
                return;
            }
            ptb->row_len = row_len;
            ptb->row_num = ppkt->row_num;
            ptb->type = ppkt->type;
            ptb->sn = (UI8)(ppkt->sn);
            AddTable(&g_tableList,ptb);
        }
        startRow = ppkt->start_row;
        g_pumpUpdateFlag |= 0x1;
    }
    else if(ppkt->start_row == TABLE_HDFLAG)//带有表头
	{
		if(ptb == NULL)
		{
            int size = row_len*ppkt->row_num + sizeof(PROT_TBL_PKT);
            if(dat_len < (size + sizeof(PROT_TBL_HD)))
                return;
            if((ptb = CreateNewTable(ppkt,size) )== NULL)
			{
				return;
			}
            ptb->row_len = row_len;
            ptb->row_num = ppkt->row_num;
            ptb->type = ppkt->type;
            ptb->sn = (UI8)(ppkt->sn);
			AddTable(&g_tableList,ptb);
		}
		startRow = ppkt->start_row;
	}
	else
	{
		if(ptb == NULL)
		{
			if((ppkt->type == TB_I_LOGIC) || (ppkt->type == TB_O_LOGIC))
			{
                //int row_len = TABLE_IO_ROW_LEN;
				int ncount = TABLE_IO_CNT;//留有空间，以用于重定义扩展
				if((ptb = (PPROT_TBL_HD)OBJMemMalloc(row_len*ncount+sizeof(PROT_TBL_HD))) == NULL)
				{
					return;
				}
                //ptb->row_len = TABLE_IO_ROW_LEN;
                ptb->row_len = row_len;
				ptb->row_num = ppkt->row_num;
				ptb->type = ppkt->type;
				ptb->sn = (UI8)(ppkt->sn);
				AddTable(&g_tableList,ptb);
			}
		}
		startRow = ppkt->start_row - 1;
	}
	TableWrite(ptb,startRow,ppkt->row_num,((UI8 *)ppkt)+sizeof(PROT_TBL_PKT) + sizeof(PROT_TBL_HD));
	
	memset(oldinData,0,sizeof(oldinData));
	memset(oldoutData,0,sizeof(oldoutData));
	ClearInOutPoint();
	IoredefCheckUpdate(ppkt->type, ppkt->sn);

    //20191210
    if(ppkt->type == TB_MACHINE)
    {
        //if(PanelCfgStyle() == G_TYPE)
        {
            UI16 num1, num2, bit;
            bit = 0;
            VarAdrSetInt(PART_REQ_STATE, 0);

            num1 = GetSubPartNum(TEMPEXT_ID);
            if(num1 != 0)
                setone(bit, 0);
            num1 = num1?num1:1;
            VarAdrSetInt(TEMP_CTRL_PART, num1);  //查找扩展温度部件

            num2 = GetSubPartNum(PUMP_ID);
            if(num2 != 0)
                setone(bit, 1);
            num2 = num2?num2:1;
            if(num2 > MAX_SERVO_CURRENT_NUM)
            {
                num2 = MAX_SERVO_CURRENT_NUM;
            }
            VarAdrSetInt(SERVO_CURRENT_NUM, num2);  //查找伺服部件

//            num3 = GetSubPartNum(MOLDTEMP_ID);
//            if(num3 != 0)
//                setone(bit, 2);
//            num3 = num3?num3:1;
            VarAdrSetInt(MT_CTRL_PART, GetSubPartNum(MOLDTEMP_ID));   //查找模温部件 //20200407HJM 模温功能
            VarAdrSetInt(PART_REQ_STATE, bit);
            if(VarAdrToInt(MT_BOARD_COUNT) > VarAdrToInt(MT_CTRL_PART))
            {
                VarAdrSetInt(MT_BOARD_COUNT, VarAdrToInt(MT_CTRL_PART));
                VarAdrSaveInt(MT_BOARD_COUNT);
            }
            //oprintf("TEMP_CTRL_PART  =%d   s=%d\n", GetSubPartNum(TEMPEXT_ID),GetSubPartNum(PUMP_ID));
        }
    }
}

static PPROT_TBL_HD CreateNewTable(PPROT_TBL_PKT ppkt, I32 dat_len)
{
    PPROT_TBL_HD ptb_read = (PPROT_TBL_HD)(ppkt+1);
    PPROT_TBL_HD ptb = NULL;
    //int len = dat_len-sizeof(PROT_TBL_PKT);

    if(ppkt == NULL )
        return NULL;

    ptb = (PPROT_TBL_HD)OBJMemMalloc(TABLE_IO_CNT*ptb_read->row_len); //20190826 hzh 预留重定义的空间
    memcpy(ptb, ptb_read, dat_len);

    return ptb;
}

void RequestTable(BOOL isOri)//是否获得原始数据
{
	//获取IO定义表
	ProtReqTbl(TB_O_LOGIC,TB_CUR);
	ProtReqTbl(TB_I_LOGIC,TB_CUR);
	if(isOri)//获取IO定义出厂表
	{
		ProtReqTbl(TB_O_LOGIC,TB_ORI);
		ProtReqTbl(TB_I_LOGIC,TB_ORI);
	}
}

/**
 * @brief ReplaceTable 替换原先的表，并删除
 * @param plist
 * @param oldptb 原先的表
 * @param newptb 新的表
 */
static void ReplaceTable(PTABLELIST plist, PPROT_TBL_HD oldptb,PPROT_TBL_HD newptb)
{
    if((oldptb != NULL) && oldptb->type < (TB_PUMP+1) && (plist!=NULL)&& (newptb!=NULL))//fqh 新增表时,oldptb->type的范围要保持最新
    {
        while(plist->pcur)
        {
            if(plist->pcur == oldptb)
            {
                plist->pcur = newptb;
                OBJMemFree(oldptb);
                return;
            }
            plist = plist->pnext;
        }
    }
}

/**
 * @brief TableWrite    不支持startrow非零的写入
 * @param ptb
 * @param startrow
 * @param rownum
 * @param data
 */
static void TableWrite(PPROT_TBL_HD ptb,UI16 startrow,UI16 rownum ,UI8 * data)
{
    if(startrow != 0)//不支持startrow非零的写入
        return;
	if(ptb != NULL)
	{
		int noff = startrow * ptb->row_len;
		int nsize = rownum * ptb->row_len;
        int osize = ptb->row_len*ptb->row_num;
        if((noff+nsize) > osize)
        {
            PPROT_TBL_HD newptb = (PPROT_TBL_HD)OBJMemMalloc(noff+nsize+sizeof(PROT_TBL_HD));
            newptb->row_len = ptb->row_len;
            newptb->row_num = rownum;
            newptb->type = ptb->type;
            newptb->sn = ptb->sn;
            ReplaceTable(&g_tableList,ptb,newptb);
            ptb = newptb;
        }
        else
        {
            ptb->row_num = rownum;//20200703 表头行数以主机上传为主
            memset((UI8*)(ptb+1),0,ptb->row_len*ptb->row_num);//清除原先表的数据
            //memset((UI8*)(ptb+1)+noff,0,nsize);//清除原先表的数据
        }
        memcpy((UI8*)(ptb+1)+noff,data,nsize);
	}
}

static void AddTable(PTABLELIST plist, PPROT_TBL_HD ptb)
{
    if(ptb->type < (TB_PUMP+1) && (plist!=NULL))
	{
		while( plist->pcur != NULL)
		{
			plist = plist->pnext;
		}
		plist->pcur = ptb;
		plist->pnext = (PTABLELIST)OBJMemMalloc(sizeof(TABLELIST));
	}
}

/**
 * @brief      : AI记忆开模算法表加入g_tableList
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200730
 */
void AddOpenCalTable(PPROT_TBL_HD ptb)
{
    PTABLELIST plist = &g_tableList;
    if(ptb->type < (TB_OPENCAL+1) && (plist!=NULL))
    {
        while( plist->pcur != NULL)
        {
            plist = plist->pnext;
        }
        plist->pcur = ptb;
        plist->pnext = (PTABLELIST)OBJMemMalloc(sizeof(TABLELIST));
    }
}

/**
 * @brief      : Pump表加入g_tableList
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : fqh
 * @date       : 20241223
 */
void AddPumpTable(PPROT_TBL_HD ptb)
{
    PTABLELIST plist = &g_tableList;
    if(ptb->type < (TB_PUMP+1) && (plist!=NULL))
    {
        while( plist->pcur != NULL)
        {
            plist = plist->pnext;
        }
        plist->pcur = ptb;
        plist->pnext = (PTABLELIST)OBJMemMalloc(sizeof(TABLELIST));
    }
}

PPROT_TBL_HD GetTableByType(UI16 type,UI16 sn)
{
    if(type < (TB_PUMP+1) )
	{
		PTABLELIST plist = &g_tableList;
		PPROT_TBL_HD ptb = plist->pcur;
		while( (plist != NULL) && (ptb != NULL) )
		{
			if((ptb->type == type) && (ptb->sn == sn))
			{
				return ptb;
			}
			plist = plist->pnext;
			ptb = plist->pcur;
		}
	}
	
	return NULL;
}

TBL_INPUT * TableFindInput(PPROT_TBL_HD ptb,UI16 sn)
{
	TBL_INPUT * pinput;
	if((ptb != NULL))
	{
		int i=0;
		pinput = (TBL_INPUT*)(ptb+1);
		for(i=0;i<ptb->row_num;i++,pinput++)
		{
			if(pinput->sn == sn)
			{
				return pinput;
			}
		}
	}
	return NULL;
}
TBL_OUTPUT * TableFindOutput(PPROT_TBL_HD ptb,UI16 sn)
{
	return TableFindInput(ptb,sn);
}


/**************************************************************
IO点数据分配到dic数据中
****************************************************************/
void ProcessIo()
{
	int k=0;
	UI32 wData;
	UI32 wchg;
	//输入点
	{
		PPROT_TBL_HD ptb = GetTableByType(TB_I_LOGIC,TB_CUR);
		for(k=0 ;k < IO_POINT_CNT;k++)
		{
            wData = VarAdrToInt(PI_INPUT1+k);
			wchg = oldinData[k] ^ wData;
			oldinData[k] = (UI16)wData;
			if((wchg != 0) && (ptb != NULL))
			{
				TBL_INPUT *pinput=NULL;
				int i;
				int n = k;
				n <<= 4;
				for(i=0;i<16;i++ ,n++,wData>>=1,wchg >>= 1)
				{
					if((wchg & 1) != 0 )
					{
						pinput = TableFindInput(ptb,n+1);
						if((pinput != NULL) && (pinput->part_id != 0))
						{
							UI32 dwinput = (pinput->part_id + 1) << 16;
							dwinput += pinput->ind;
							VarAdrSetInt(dwinput,(wData & 1));
						}
					}
				}
			}
		}
	}
	
	//输出点
	{
		PPROT_TBL_HD ptb = GetTableByType(TB_O_LOGIC,TB_CUR);
		for(k=0 ;k < IO_POINT_CNT;k++)
		{
            wData = VarAdrToInt(PO_OUTPUT1+k);
			wchg = oldoutData[k] ^ wData;
			oldoutData[k] =(UI16) wData;
			if((wchg != 0) && (ptb != NULL))
			{
				TBL_OUTPUT *poutput=NULL;
				int i;
				int n = k;
				n <<= 4;
				for(i=0;i<16;i++ ,n++,wData>>=1,wchg >>= 1)
				{
					if((wchg & 1) != 0 )
					{
						poutput = TableFindOutput(ptb,n+1);
						if((poutput != NULL) && (poutput->part_id != 0))
						{
							UI32 dwoutput = (poutput->part_id) << 16;
							dwoutput += poutput->ind;
							VarAdrSetInt(dwoutput,(wData & 1));
						}
					}
				}
			}
		}
	}
}

/*send table to main*/
BOOL ProtSendTbl(PPROT_TBL_HD ptable)
{
	PPROT_TBL_PKT ppkt;
	int data_len;
	if(!ptable) return FALSE;

	data_len = sizeof(PROT_TBL_HD) + ptable->row_len*ptable->row_num;

	if((ppkt = (PPROT_TBL_PKT)ProtNewPkt(TBL_RSP_CMD, 0, TRUE, sizeof(PROT_TBL_PKT) + data_len)) != NULL)
	{
		ppkt->type = ptable->type;
		ppkt->sn = ptable->sn;
		ppkt->start_row = 0; //0表示包含表头
		ppkt->row_num = ptable->row_num;
		memcpy(ppkt+1, ptable, data_len);
		return MainProtTxPkt(ppkt);
	}

	return FALSE;		
}

/**
 * @brief      : 找到子类信息表
 *
 * @param      : 类别
 * @return     :类别信息表
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : 作者（YBC）
 * @date       : 日期（20191210）
 */
SUB_PART_INFO * TableFindSubPartInfo(PPROT_TBL_HD ptb,UI8 part_type)
{
    SUB_PART_INFO * psubpartinfo =NULL;

    if((ptb != NULL))
    {
        int i=0;
        psubpartinfo = (SUB_PART_INFO*)(ptb+1);
        for(i=0;i<ptb->row_num;i++,psubpartinfo++)
        {
            if(psubpartinfo->part_type == part_type)
            {
                return psubpartinfo;
            }
        }
    }
    return NULL;
}

/**
 * @brief      : 找到对应类别的请求子类数
 *
 * @param      : 类别
 * @return     : 请求的子类数
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : 作者（YBC）
 * @date       : 日期（20191210）
 */
UI8  GetSubPartNum(UI8 part_type)
{
    SUB_PART_INFO * psubpartinfo =NULL;

    PPROT_TBL_HD ptb = GetTableByType(TB_MACHINE,TB_CUR);

    if(ptb ==NULL)
        return 0;
    psubpartinfo = TableFindSubPartInfo(ptb,part_type);
    if(psubpartinfo !=NULL)
    {
        return psubpartinfo->num;
    }

    return 0;
}
