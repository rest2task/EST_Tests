/*mcu boot communite*/
#include "typedef.h"
#include "vardb.h"
#include "protcmd.h"
#include "monmcu.h"
#include "oprintf.h"
#include "database.h"
#include "tablectl.h"
#include "vlvlogic.h"
#include "pushtab.h"
/*production period 1, st 150ms 2, 128; rule 3  50ms 129; warn 4 150ms 130; tempture 5 500ms 131*/
static DATA_SET data_set_st[VAR_PERIOD_SET_NUM] = {{0, 0}, {0, 1},{150, 128 + 0x100}, {50, 129 + 0x100}, {200, 130 + 0x100}, {500, 131 + 0x100}};
static BOOL bdata_set_init = FALSE;
static BOOL bdata_request_init = FALSE;


static BOOL DataSetTran(PDATA_SET pset)
{
	int i;

	if(pset->mem_num < pset->num || pset->pvar_adr == NULL)
	{
		if(pset->pvar_adr != NULL)
		{
			OBJMemFree(pset->pvar_adr);
			pset->pvar_adr = NULL;
			pset->mem_num = 0;
		}
		
		pset->pvar_adr = (PDATA_SET_ADR)OBJMemMalloc(sizeof(*(pset->pvar_adr))*pset->num);
		if(pset->pvar_adr == NULL)
			return FALSE;
			
		pset->mem_num = pset->num;
	}

	//oprintf("set %d, num %d\r\n", pset->set_id, ptbl->num);
	for(i = 0; i < pset->num; i++)
	{
		/*here assume variant is continuous ordered*/
		pset->pvar_adr[i].pvar = VarAdrToVar(pset->pset_adr[i].address);
		pset->pvar_adr[i].len = pset->pset_adr[i].len;
		pset->total_len += pset->pset_adr[i].len;
		//oprintf("adr = %x, len = %d\r\n", padr[i].address, padr[i].len);
	}

	return TRUE;
}

void DataSetInit()
{
	int i;
	PQST_TBL ptbl;
    PDATA_SET pset;

	//ProtReqPartSubData(0x0010, 0x02,0, 48);
	if(bdata_set_init == TRUE)
		return;
		
    for(i = 0; i < ARRAY_NUM(data_set_st); i++)
	{
		ptbl = VarDBGetVarPeriodTbl(i);
        pset = data_set_st + i;
        if(ptbl != NULL && ptbl->num > 0)
		{			
			pset->num = ptbl->num;
			pset->bneedtran = TRUE;
			pset->pset_adr = (PVAR_ADR_ARRAY)(ptbl->pnode);
			pset->bused = TRUE;
            DataSetTran(pset);
		}
	}

	bdata_set_init = TRUE;
}

static void SendDataSetReq()
{
	int i;
	PDATA_SET pset;

	//ProtReqPartSubData(0x0010, 0x02,0, 48);

	DataSetInit();
	for(i = 0; i < ARRAY_NUM(data_set_st); i++)
	{
		pset = data_set_st + i;
		if(pset->bused && pset->num > 0)
        {
			ProtReqDataSet(pset->set_id, pset->period_ms, pset->num, pset->pset_adr);
		}
	}
}

#include "command.h"
void BootUpMsgRx(UI16 type, UI16 error, UI16 hw_ver, UI16 sw_ver)
{
    //20211210 dyl
    static BOOL flag = TRUE;
    if(flag)
    {
        flag = FALSE;
        SetDataRecv_IngFlag(TRUE);
    }

	/*configure MCU period send data set*/
	//oprintf("Bootup message\r\n");
    ProtReqPartSubData(PART1_ID(MACHINE_ID),MACHSET,0,GetSubLenByID(PART1_SUB_ID(MACHINE_ID,MACHSET)));//先请求配置表，数据分开请求 20200507

    RequestTable(TRUE);
    ProtReqTbl(TB_MACHINE,TB_CUR);//20191210获取机器配表
    //获取输出点电气表 //20200923
    ClearTable_ElecLogic();
    ProtReqTbl(TB_O_ELEC,TB_CUR);
    RequestCfgData();

    SendDataSetReq();
	SendPartSateData();

	bdata_request_init = TRUE;
}

BOOL IsBootUpMsgRx()
{
	return bdata_request_init;
}

/*add DataSet require and send packet to mcu
set_id: data set id;
period_ms: require mcu period send time, 0 just send once; 
ind_num: data set variant address number;
padr:variant address, if need tranlate (btranlate == true) receive data to variand and set value, padr memory must be 
valid and not be freed all the time;
btranlate: when true, need tranlate receive data to vaiant and set value;
rx_cb: call back function when data receive;*/
BOOL DataSetReqAdd(UI16 set_id, UI16 period_ms, UI16 ind_num, PVAR_ADR_ARRAY padr, BOOL btranlate, DATA_SET_RX_CB rx_cb)
{
	int i;
	PDATA_SET pset;
	BOOL ret = FALSE;

	if(ind_num <= 0 || padr == NULL)
		return FALSE;
		
	for(i = 0; i < ARRAY_NUM(data_set_st); i++)
	{
		if(data_set_st[i].set_id == set_id || data_set_st[i].bused == FALSE)
		{
			pset = data_set_st + i;
			pset->bused = TRUE;
			pset->bneedtran = btranlate;
			pset->num = ind_num;
			pset->set_id = set_id;
			pset->period_ms = period_ms;
			pset->rx_cb = rx_cb;
			pset->pset_adr = padr;
			if((ret = DataSetTran(pset)))
			{
				ret = ProtReqDataSet(pset->set_id, pset->period_ms, pset->num, padr);
			}
			break;
		}
	}

	return ret;
}

/*MCU period data set receive*/
void DataSetRx(UI16 set_id, int num, UI16* pdata)
{
	int i, j, len;
	PDATA_SET pset;
	PVAR pnow;
	UI16* pdat;

	if(!bdata_set_init)
	{
		DataSetInit();
		return;
	}
	
	/*Data set 0 is */	
	for(i = 0; i < ARRAY_NUM(data_set_st); i++)
	{
		if(data_set_st[i].set_id == set_id)
			break;
	}

	if(i >= ARRAY_NUM(data_set_st))
	{
		/*not found, return*/
		return;
	}

	pset = data_set_st + i;
	if(pset->pvar_adr == NULL)
	{
		return;
	}

	if(pset->bneedtran && pset->pvar_adr != NULL)
	{
		pdat = pdata;	
		for(i = 0; i < pset->num; i++)
		{
			/*here assume variant is continuous ordered*/
			pnow = pset->pvar_adr[i].pvar;
			len = pset->pvar_adr[i].len;
			
			for(j = 0; j < len; j++)
            {
                VarSetValueBackSet(pnow, *pdat);
                pdat++;
                pnow++;

                if(pdat >= pdata + num)
                    break;
            }

            if(pdat >= pdata + num)
                break;
        }
	}

	if(pset->rx_cb != NULL)
	{
		(*pset->rx_cb)(set_id, num, pdata, pset->num, pset->pvar_adr);
	}
}

DATA_SET_RX_CB DataSetCbSet(UI16 set_id, DATA_SET_RX_CB rx_cb)
{
	int i;
	DATA_SET_RX_CB pret = NULL;

	for(i = 0; i < ARRAY_NUM(data_set_st); i++)
	{
		if(data_set_st[i].set_id == set_id && data_set_st[i].rx_cb != rx_cb)
		{
			pret = data_set_st[i].rx_cb;
			data_set_st[i].rx_cb = rx_cb;
			break;
		}
	}

	return pret;	
}

