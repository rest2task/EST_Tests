#include "ioredef.h"
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

#define JOIN_ADDR(PARTID, SUBID) (((UI32)(PARTID)) << 16 | ((SUBID) & 0xFFFF))
#define IO_POINT_CNT			(8)

UI16 g_ioUpdateFlag;
UI32 g_currentnum;
REDEF_INFO io_infos[2] = {
    {TB_I_LOGIC, INPUT_FILE,  IO_INTPUT_GROUPS*8},//20200317
    {TB_O_LOGIC, OUTPUT_FILE, 96},
};

//20190513.cyx
IO_SPEC g_iospec;

void InitRedefData()
{
    memset(g_iospec.IPointLG,0,sizeof(g_iospec.IPointLG));
    memset(g_iospec.IPoint,0,sizeof(g_iospec.IPoint));
    memset(g_iospec.bUseForce,0,sizeof(g_iospec.bUseForce));//输入点强制输入标识,用于IO设置界面
    memset(g_iospec.bUseForceRec,0,sizeof(g_iospec.bUseForceRec));//输入点强制输入标识,用于IO设置界面
    memset(g_iospec.bForceInput,0,sizeof(g_iospec.bForceInput));//输入点强制输入标识,用于IO设置界面
    memset(g_iospec.bForceInputRec,0,sizeof(g_iospec.bForceInputRec));//输入点强制输入标识,用于IO设置界面
    g_iospec.m_bFirstInput = true;//开机后首次接收输入点表
}

// 移除一个点
static void Ioredef__RemoveIO(PPROT_TBL_HD phd, int sn)
{
    int i;
    TBL_INPUT* pio = (TBL_INPUT*)(phd+1);
    for(i = 0; i < phd->row_num; i++, pio++){
        if(pio->sn == sn){
            phd->row_num--;
            memmove(pio, pio+1, sizeof(TBL_INPUT)*(phd->row_num-i));
            break;
        }
    }
}

// 开机交互时由重定义表生成新当前表，整张表处理完后整体发送给下位机，避免中途多次发送导致主机写flash 20200716
static void IoredefCfgRedefineTable(int tbltype, PREDEF_DATA pdata,PPROT_TBL_HD ptable)
{
    int i;
    PREDEF_INFO pinfo = IoredefGetInfo(tbltype);
    PPROT_TBL_HD ptableori = GetTableByType(pinfo->table_type, TB_ORI);

    if(ptable == NULL || ptableori == NULL) return;

    for(i = 0; i < MAX_REDEFINE; i++){
        int sn = pdata->phy_sn[i];
        UI32 dwdef = pdata->log_sn[i];

        if(sn == 0 && pinfo->phy_sn[i]){ //还原
            int sn_old = pinfo->phy_sn[i];
            TBL_INPUT* pinOri = TableFindInput(ptableori, sn_old);
            TBL_INPUT* pinCur = TableFindInput(ptable, sn_old);
            if(pinOri != NULL){
                if(pinCur == NULL){
                    pinCur = (TBL_INPUT*)(ptable+1) + ptable->row_num;
                    ptable->row_num++;
                    pinCur->sn = sn_old;
                }
                pinCur->part_id = pinOri->part_id;
                pinCur->ind = pinOri->ind;
                pinCur->cfg = pinOri->cfg;
                ODEBUG("recover[%d] sn=%d, addr=0x%x", i, sn_old, JOIN_ADDR(pinCur->part_id, pinCur->ind));
            }
            else{
                if(pinCur != NULL){
                    Ioredef__RemoveIO(ptable, sn_old);
                    ODEBUG("remove[%d] sn=%d", i, sn_old);
                }
            }
        }
        else if(sn != 0){//重定义
            TBL_INPUT* pinCur = TableFindInput(ptable, sn);
            if(pinCur == NULL){//增加一个输入点
                pinCur = (TBL_INPUT*)(ptable+1) + ptable->row_num;
                ptable->row_num++;
                pinCur->sn = sn;
                pinCur->cfg = 0;//20190603
            }

            pinCur->part_id = dwdef >> 16 & 0xFFF0;
            pinCur->ind = (UI8)dwdef;
            pinCur->cfg = 0;//20190603
            ODEBUG("redefine[%d] sn=%d, addr=0x%x", i, sn, JOIN_ADDR(pinCur->part_id, pinCur->ind));
            /* TODO: 反逻辑处理还没做 */

            /* 20190325 YX 反逻辑处理 */
            if(((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))&&(tbltype == TB_I_LOGIC))
            {
                if(g_iospec.IPointLG[i])
                {
                    pinCur->cfg = pinCur->cfg|0x80;//最高位置1代表为反逻辑
                }
                else
                {
                    pinCur->cfg = pinCur->cfg&0x7f;//最高位置0代表为正逻辑
                }
                if(g_iospec.bUseForce[i])//是否使用强制输入
                {
                    pinCur->cfg = pinCur->cfg|0x40;//高位第二位1代表使用强制输入
                }
                else
                {
                    pinCur->cfg = pinCur->cfg&0xbf;//高位第二位0代表不使用强制输入
                }
                if(g_iospec.bForceInput[i])//强制输入或强制不输入
                {
                    pinCur->cfg = pinCur->cfg|0x20;//高位第三位1代表强制输入
                }
                else
                {
                    pinCur->cfg = pinCur->cfg&0xdf;//高位第三位0代表强制不输入
                }
            }
        }
    }
}

//20190325  YX 开机后首次接收输入点表并为强制输入状态置为不使用
static void MakeRedefineInput()
{
    PPROT_TBL_HD ptable    = GetTableByType(TB_I_LOGIC, TB_CUR);
    PPROT_TBL_HD ptableori = GetTableByType(TB_I_LOGIC, TB_ORI);

    if(ptable == NULL || ptableori == NULL)
    {
        return;
    }

    //20190911.cyx 修改原模式
    {
        int i;
        TBL_INPUT * pinput;
        pinput = (TBL_INPUT*)(ptable+1);
        for(i = 0;i < ptable->row_num;i++,pinput++)
        {
            UI32 nindex = pinput->sn-1;
            if(nindex < 100)
            {
                if(((pinput->cfg)>>7) == 1)
                {
                    g_iospec.IPoint[nindex] = true;
                }
                else
                {
                    g_iospec.IPoint[nindex] = false;
                }
                //是否使用强制输入
                if((((pinput->cfg)>>6)&0x01) == 1)
                {
                    g_iospec.bUseForceRec[nindex] = true;
                }
                else
                {
                    g_iospec.bUseForceRec[nindex] = false;
                }
                //强制输入或强制不输入
                if((((pinput->cfg)>>5)&0x01) == 1)
                {
                    g_iospec.bForceInputRec[nindex] = true;
                }
                else
                {
                    g_iospec.bForceInputRec[nindex] = false;
                }
            }
        }
    }

    if(g_iospec.m_bFirstInput)
    {
        int i,j;
        REDEF_DATA pdata;
        g_iospec.m_bFirstInput = FALSE;
        for(i = 0; i < IO_INTPUT_GROUPS; i++)
        {
            for(j = 0; j < 8; j++)
            {
                TBL_INPUT* pinput = TableFindInput(ptable,i*8+j+1);
                pdata.phy_sn[j] = i*8+j+1;
                if(pinput != NULL)
                {
                    pdata.log_sn[j] = ((pinput->part_id + 1) << 16) + pinput->ind;
                }
                else
                {
                    pdata.log_sn[j] = 0;
                }
                if(pdata.log_sn[j] == 0x10000)
                {
                    pdata.log_sn[j] = 0;
                }
                g_iospec.IPointLG[j] = g_iospec.IPoint[i*8+j];
                g_iospec.bUseForce[j] = false;
                g_iospec.bForceInput[j] = false;
            }
            pdata.swap_sn[0] = 0;
            pdata.swap_sn[1] = 0;
            IoredefCfgRedefineTable(TB_I_LOGIC, &pdata,ptable); //20200716
        }
        ProtSendTbl(ptable);//20200716
        RequestTable(FALSE);
        return;
    }
}

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

// 载入IO地址链表
static void Ioredef__LoadList(PREDEF_INFO pinfo)
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

void IoredefInital()
{
	int i;
	for(i = 0; i < ARRAY_NUM(io_infos); ++i)
	{
		Ioredef__LoadList(&io_infos[i]);
		ParseCSVFile(io_infos[i].map_path, &io_infos[i].name_tbl);
	}
}

void IoredefCheckUpdate(int tbltype, int tblsn)
{
	if(tblsn == TB_CUR)
	{
		if(tbltype == TB_I_LOGIC)
		{
            if((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))
            {
                MakeRedefineInput();//20190517.cyx
            }
            IoredefMakeRedefine(IoredefGetInfo(tbltype)); //生成重定义表
            g_ioUpdateFlag |= 0x1;
		}
		else if(tbltype == TB_O_LOGIC)
		{
			IoredefMakeRedefine(IoredefGetInfo(tbltype)); //生成重定义表
			g_ioUpdateFlag |= 0x2;
		}
        else if(tbltype == TB_O_ELEC) //20200923
        {
            OnTable_ElecOut();
        }
	}

    if(((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))&&(g_iospec.m_bFirstInput))
    {
        MakeRedefineInput();//20190517.cyx
    }
}

PREDEF_INFO IoredefGetInfo(int table_type)
{
	int i;
	for(i = 0; i < ARRAY_NUM(io_infos); ++i)
	{
		if(io_infos[i].table_type == table_type)
		{
			return &io_infos[i];
		}
	}
	return NULL;
}

// 比较当前和原始表，生成重定义列表
void IoredefMakeRedefine(PREDEF_INFO pinfo)
{
    int n,i;
    PPROT_TBL_HD ptable    = GetTableByType(pinfo->table_type, TB_CUR);
    PPROT_TBL_HD ptableori = GetTableByType(pinfo->table_type, TB_ORI);

    if(ptable == NULL || ptableori == NULL) return;
    memset(pinfo->phy_sn, 0, sizeof(pinfo->phy_sn));
    memset(pinfo->log_sn, 0, sizeof(pinfo->log_sn));

    for(i = 1, n = 0; i <= pinfo->io_cnt && n < MAX_REDEFINE; i++){
        TBL_INPUT* pinOri = TableFindInput(ptableori, i);
        TBL_INPUT* pinCur = TableFindInput(ptable, i);
        if(pinOri == NULL && pinCur != NULL){//增加点
            if((pinfo->table_type == TB_I_LOGIC) && ((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE)))
            {
                if(((pinCur->part_id << 16) + pinCur->ind)==0x10000 || ((pinCur->part_id << 16) + pinCur->ind)==0)//20191021.cyx IO重定义过滤显示空白点
                {
                    continue;
                }
            }

            pinfo->phy_sn[n] = i;
            pinfo->log_sn[n] = (pinCur->part_id << 16) + pinCur->ind;

            if(pinfo->log_sn[n]==0x10000)
            {
                pinfo->log_sn[n]=0;
            }
            n++;
        }
        else if(pinOri != NULL && pinCur == NULL){//删除点
            pinfo->phy_sn[n] = i;
            pinfo->log_sn[n] = 0;
            n++;
        }
        else if(pinOri != NULL && pinCur != NULL){//修改点
            if(pinOri->part_id != pinCur->part_id || pinOri->ind != pinCur->ind
                    || (pinOri->part_id == pinCur->part_id && pinOri->ind == pinCur->ind && ((pinOri->cfg&0x80) != (pinCur->cfg&0x80))//20200109.cyx cfg属性不同时只判断反逻辑属性是否不同
                       && (pinfo->table_type == TB_I_LOGIC) && ((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE)))
                    ){//20191021.cyx 反逻辑及强制输入改变记录
                pinfo->phy_sn[n] = i;
                pinfo->log_sn[n] = (pinCur->part_id << 16) + pinCur->ind;

                if(pinfo->log_sn[n]==0x10000)
                {
                    pinfo->log_sn[n]=0;
                }
                n++;
            }
        }
    }
}

// 由重定义表生成新当前表，发送给下位机
void IoredefRedefineTable(int tbltype, PREDEF_DATA pdata)
{
	int i, FromPoint,ToPoint;
	TBL_INPUT* FromPin;
	TBL_INPUT* ToPin;
	TBL_INPUT* FromPinori;
	TBL_INPUT* ToPinori;
	PREDEF_INFO pinfo = IoredefGetInfo(tbltype);
	PPROT_TBL_HD ptable    = GetTableByType(pinfo->table_type, TB_CUR);
	PPROT_TBL_HD ptableori = GetTableByType(pinfo->table_type, TB_ORI);
	if(ptable == NULL || ptableori == NULL) return;

	for(i = 0; i < MAX_REDEFINE; i++){
		int sn = pdata->phy_sn[i];
		UI32 dwdef = pdata->log_sn[i];

		if(sn == 0 && pinfo->phy_sn[i]){ //还原
			int sn_old = pinfo->phy_sn[i];
			TBL_INPUT* pinOri = TableFindInput(ptableori, sn_old);
			TBL_INPUT* pinCur = TableFindInput(ptable, sn_old);
			if(pinOri != NULL){
				if(pinCur == NULL){
					pinCur = (TBL_INPUT*)(ptable+1) + ptable->row_num;
					ptable->row_num++;
					pinCur->sn = sn_old;
				}
				pinCur->part_id = pinOri->part_id;
				pinCur->ind = pinOri->ind;
				pinCur->cfg = pinOri->cfg;
				ODEBUG("recover[%d] sn=%d, addr=0x%x", i, sn_old, JOIN_ADDR(pinCur->part_id, pinCur->ind));
			}
			else{
				if(pinCur != NULL){
					Ioredef__RemoveIO(ptable, sn_old);
					ODEBUG("remove[%d] sn=%d", i, sn_old);
				}
			}
		}
		else if(sn != 0){//重定义
			TBL_INPUT* pinCur = TableFindInput(ptable, sn);
			if(pinCur == NULL){//增加一个输入点
				pinCur = (TBL_INPUT*)(ptable+1) + ptable->row_num;
				ptable->row_num++;
				pinCur->sn = sn;
				pinCur->cfg = 0;//20190603
			}

            pinCur->part_id = dwdef >> 16 & 0xFFF0;
			pinCur->ind = (UI8)dwdef;
			pinCur->cfg = 0;//20190603
            ODEBUG("redefine[%d] sn=%d, addr=0x%x", i, sn, JOIN_ADDR(pinCur->part_id, pinCur->ind));
			/* TODO: 反逻辑处理还没做 */
			
			/* 20190325 YX 反逻辑处理 */
            if(((PanelCfgStyle()==G_TYPE) || (PanelCfgStyle()==P_TYPE))&&(tbltype == TB_I_LOGIC))
            {
                if(g_iospec.IPointLG[i])
                {
                    pinCur->cfg = pinCur->cfg|0x80;//最高位置1代表为反逻辑
                }
                else
                {
                    pinCur->cfg = pinCur->cfg&0x7f;//最高位置0代表为正逻辑
                }
                if(g_iospec.bUseForce[i])//是否使用强制输入
                {
                    pinCur->cfg = pinCur->cfg|0x40;//高位第二位1代表使用强制输入
                }
                else
                {
                    pinCur->cfg = pinCur->cfg&0xbf;//高位第二位0代表不使用强制输入
                }
                if(g_iospec.bForceInput[i])//强制输入或强制不输入
                {
                    pinCur->cfg = pinCur->cfg|0x20;//高位第三位1代表强制输入
                }
                else
                {
                    pinCur->cfg = pinCur->cfg&0xdf;//高位第三位0代表强制不输入
                }
            }
		}
	}

	FromPoint = pdata->swap_sn[0];
	ToPoint = pdata->swap_sn[1];
	FromPin = TableFindInput(ptable, FromPoint);
	ToPin = TableFindInput(ptable, ToPoint);
	FromPinori = TableFindInput(ptableori, FromPoint);
	ToPinori = TableFindInput(ptableori, ToPoint);    

	if(FromPoint == 0 || ToPoint == 0 ){
		;
	}
	else if(FromPin == NULL && ToPin == NULL)
	{
		;
	}
	else if(ToPin == NULL){
		ToPin= (TBL_INPUT*)(ptable+1) + ptable->row_num;
		ptable->row_num++;
		ToPin->sn = ToPoint;
		ToPin->part_id = FromPin->part_id;
		ToPin->ind = FromPin->ind;
		ToPin->cfg = FromPin->cfg;
		if(FromPinori == NULL)
			Ioredef__RemoveIO(ptable, FromPoint);
		else{
			FromPin->part_id = 0;
			FromPin->ind = 0;
			FromPin->cfg = 0;
		}
		ODEBUG("ToPin sn=%d, addr=0x%x", ToPin->sn, JOIN_ADDR(ToPin->part_id, ToPin->ind));
	}
	else if(FromPin == NULL){
		FromPin = (TBL_INPUT*)(ptable+1) + ptable->row_num;
		ptable->row_num++;
		FromPin->sn = FromPoint;
		FromPin->part_id = ToPin->part_id;
		FromPin->ind = ToPin->ind;
		FromPin->cfg = ToPin->cfg;
		if(ToPinori == NULL)
			Ioredef__RemoveIO(ptable, ToPoint);
		else{
			ToPin->part_id = 0;
			ToPin->ind = 0;
			ToPin->cfg = 0;
		}
	}
	else{
		UI16 frompart_id = FromPin->part_id;
		UI8 fromind = FromPin->ind;
		UI8 fromcfg = FromPin->cfg;
		FromPin->part_id = ToPin->part_id;
		FromPin->ind = ToPin->ind;
		FromPin->cfg = ToPin->cfg;
		ToPin->part_id = frompart_id;
		ToPin->ind = fromind;
		ToPin->cfg = fromcfg;
		ODEBUG("FromPin sn=%d, addr=0x%x", FromPin->sn, JOIN_ADDR(FromPin->part_id, FromPin->ind));
		ODEBUG("ToPin sn=%d, addr=0x%x", ToPin->sn, JOIN_ADDR(ToPin->part_id, ToPin->ind));
		if(FromPin->part_id==0 && FromPin->ind == 0 && FromPinori == NULL)
		{
			Ioredef__RemoveIO(ptable, FromPoint);
		}
		if(ToPin->part_id == 0 && ToPin->ind ==0 && ToPinori == NULL)
		{
			Ioredef__RemoveIO(ptable, ToPoint);
		}
    }

	ProtSendTbl(ptable);
}

/***********************************************
 *BriefCHN     : 一键清除IO强制输入
 *BriefENG     : Introduce Function function
 *Param        :
 *Return Value :
 *Author       : YX
 *Date         : 20190325
************************************************/
void ClearFoceInPoint()
{
    int i,j;
    REDEF_DATA pdata;
    PPROT_TBL_HD ptable = GetTableByType(TB_I_LOGIC, TB_CUR);
    if(ptable == NULL)
    {
        return;
    }

    for(i = 0; i < IO_INTPUT_GROUPS; i++)
    {
        for(j = 0; j < 8; j++)
        {
            TBL_INPUT* pinput = TableFindInput(ptable,i*8+j+1);
            pdata.phy_sn[j] = i*8+j+1;
            if(pinput != NULL)
            {
                pdata.log_sn[j] = ((pinput->part_id + 1) << 16) + pinput->ind;
            }
            else
            {
                pdata.log_sn[j] = 0;
            }
            if(pdata.log_sn[j] == 0x10000)
            {
                pdata.log_sn[j] = 0;
            }
            g_iospec.IPointLG[j] = g_iospec.IPoint[i*8+j];
            g_iospec.bUseForce[j] = false;
            g_iospec.bForceInput[j] = false;
        }
        pdata.swap_sn[0] = 0;
        pdata.swap_sn[1] = 0;
        IoredefCfgRedefineTable(TB_I_LOGIC, &pdata,ptable);  //20200716
    }
    ProtSendTbl(ptable);//20200716
    RequestTable(FALSE);
}

//20190325 YX
void SetCurrentIoNum(UI32 wNum)
{
    g_currentnum = wNum;
}
//20190325 YX
UI32 GetCurrentIoNum()
{
    return g_currentnum;
}
//20190325 YX
UI8 GetUseForceRec(UI16 wNum)
{    
    UI16 value;
    if(g_iospec.bUseForceRec[wNum])//是否使用强制输入的记录
    {
        value = 1;
    }
    else
    {
        value = 0;
    }
    return value;
}
//20190517.cyx
UI8 GetIPoint(UI16 wNum)
{
    UI16 value;
    if(g_iospec.IPoint[wNum])//输入点反逻辑标识记录
    {
        value = 1;
    }
    else
    {
        value = 0;
    }
    return value;
}
//20190325 YX
UI8 GetForceInputRec(UI16 wNum)
{
    UI16 value;
    if(g_iospec.bForceInputRec[wNum])//是否强制设置记录
    {
        value = 1;
    }
    else
    {
        value = 0;
    }
    return value;
}
//20190520.cyx
void SetIPointLG(int i,BOOL state)
{
    if(i < MAX_REDEFINE)
    {
       g_iospec.IPointLG[i] = state;
    }
}
void SetUseForce(int i,BOOL state)
{
    if(i < MAX_REDEFINE)
    {
       g_iospec.bUseForce[i] = state;
    }
}
void SetForceInput(int i,BOOL state)
{
    if(i < MAX_REDEFINE)
    {
       g_iospec.bForceInput[i] = state;
    }
}

//20190325 YX
void RedefForcePoint(UI16 wNum , BOOL wbforce1 ,BOOL wbforce2,BOOL wbflagUseForce,BOOL wbflagForceInput)
{
    int i;
    UI8  bSameInput = 0;
    BOOL bFineInput = false;
    PREDEF_INFO pioinfo = NULL;
    REDEF_DATA pdata;

    pioinfo = IoredefGetInfo(TB_I_LOGIC);
    if(pioinfo == NULL)//20190920.cyx 增加保护
    {
        return;
    }
    for(i=0; i<8; i++)
    {
        if(pioinfo->phy_sn[i] == wNum+1)
        {
            bSameInput = i;
            bFineInput = TRUE;
            break;
        }
    }
    if(!bFineInput)
    {
        for(i=0; i<8; i++)//从最上开始查找最近的无重定义的点
        {
            if(pioinfo->phy_sn[i] == 0)
            {
                bSameInput = i;
                bFineInput = TRUE;
                break;
            }
        }
    }
    for(i=0; i<8; i++)
    {
        if(i == bSameInput)
        {
            TBL_INPUT* pinput = NULL;
            PPROT_TBL_HD ptable = GetTableByType(TB_I_LOGIC, TB_CUR);
            if(ptable == NULL)
            {
                return;
            }
            pinput = TableFindInput(ptable,wNum+1);
            if(pinput == NULL)//20190920.cyx 增加保护
            {
                return;
            }
            pdata.phy_sn[i] = wNum+1;
            pdata.log_sn[i] = ((pinput->part_id+1) << 16) + pinput->ind;
            g_iospec.IPointLG[i] = g_iospec.IPoint[wNum];//面板无IO点表记录时强制输入点记录错误
            if(wbforce1)
            {
                if(wbflagUseForce)
                {
                    g_iospec.bUseForce[i] = 1;
                }
                else
                {
                    g_iospec.bUseForce[i] = 0;
                }
            }
            else
            {
                g_iospec.bUseForce[i] = g_iospec.bUseForceRec[wNum];// 面板无IO点表记录时强制输入点记录错误
            }
            if(wbforce2)
            {
                if(wbflagForceInput)// 0:check 1:not check
                {
                    g_iospec.bForceInput[i] = 0;
                }
                else
                {
                    g_iospec.bForceInput[i] = 1;
                }
            }
            else
            {
                g_iospec.bForceInput[i] = g_iospec.bForceInputRec[wNum];// 面板无IO点表记录时强制输入点记录错误
            }
        }
        else
        {
            pdata.phy_sn[i] = pioinfo->phy_sn[i];
            pdata.log_sn[i] = pioinfo->log_sn[i];

            if(pdata.log_sn[i] == 0xFFFFFFFF)
            {
               pdata.log_sn[i] = 0;
            }
            // 面板无IO点表记录时强制输入点记录错误
            if (pioinfo->phy_sn[i] == 0)
            {
                g_iospec.IPointLG[i] = FALSE;
                g_iospec.bUseForce[i] = FALSE;
                g_iospec.bForceInput[i] = FALSE;
            }
            else
            {
                if((pioinfo->phy_sn[i]-1) < MAX_100)//20190920.cyx 增加保护
                {
                    g_iospec.IPointLG[i] = g_iospec.IPoint[pioinfo->phy_sn[i]-1];
                    g_iospec.bUseForce[i] = g_iospec.bUseForceRec[pioinfo->phy_sn[i]-1];
                    g_iospec.bForceInput[i] = g_iospec.bForceInputRec[pioinfo->phy_sn[i]-1];
                }
            }
        }
    }
    pdata.swap_sn[0]=0;
    pdata.swap_sn[1]=0;
    IoredefRedefineTable(TB_I_LOGIC,&pdata);
    RequestTable(FALSE);
}

PHASH_TBL pmap_input = NULL;
PHASH_TBL pmap_output = NULL;

/**
 * @brief      : 加载IO输入输出表
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20191203
 */
void InitIOMap()
{
    ParseCSVFile(INPUT_FILE,&pmap_input);
    ParseCSVFile(OUTPUT_FILE,&pmap_output);
}
