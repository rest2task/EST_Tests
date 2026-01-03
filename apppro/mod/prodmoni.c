#include "prodmoni.h"
#include "pushtab.h"
#include "verinfo.h"
#include "oprintf.h"
#include "stdio.h"
#include "file.h"
#include "common.h"
#include "database.h"
#include "panelcfg.h"
#include "prodmanage.h"
#include "time.h"
#include "tstring.h"
#define MIN_ALLOC_SIZE 512

#define INJECT_TIME_LAST            0x3316001d //上一模射出计时
#define COOL_TIME_LAST              0x3316001e //上一模冷却时间
#define INJECT_STATE_TM_INJ_RT      0x1016004e //实时计算的射出时间，用于面板显示, 0.00s
#define NOZZLE_STATE_TM_COOL_RT		0x11160004 //实时计算的冷却时间，用于面板显示, 0.00s

typedef UI16 (*PMD_GET_PT)();
typedef UI32 (*PMD_TRS_VAL)(UI16);
int JINYING_PRODMONI_COUNT = 0; //20241226 fqh 金鹰成品监测 产品数统计

//监控数据初始值  储料时间 关模时间 开模时间 开模终点 射出平均速度 保压最大压力
static UI16 Init_Cols[6] = {6,7,10,11,19,21};

static BOOL isUseProdMoniItem32()//20200324
{
    if((PanelCfgStyle() == G_TYPE) || (PanelCfgStyle() == P_TYPE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static UI16 PT_0() {return 0;}
static UI16 PT_1() {return 1;}
static UI16 PT_2() {return 2;}
static UI16 PT_CLSTM() 
{
    return 2;
}//20190226 YX
//20190715.cyx
static UI16 PT_2OR1()
{
    return 1;
}
static UI16 PT_INJTM()
{
    if(VarAdrToInt(SYS_FL_MACH_CODE1) & 0x08)  //code1
        return 3;
    else
        return 1;
}

//20200324
static UI16 PT_INJTM32()
{
    if ((VarAdrToInt(SYS_FL_MACH_CODE1) & 0x0008) != 0)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}

#define PMD_OFFSET(v) OFFSETOF(PProdMoniItem, v)
#define PMD_OFFSET32(v) OFFSETOF(PProdMoniItem32, v)//20200324

static UI16 PT_INJTUNIT()
{
    return 1;
}

typedef struct tyProdMoniInfo
{
    UI32 offset; //数据偏移
    UI32 addr;   //数据库地址
    PMD_GET_PT get_pt;  //小数点位数
    PMD_TRS_VAL trans_val; //数据转换函数
}ProdMoniInfo, *PProdMoniInfo;

//排列顺序按照成品2页面下拉框顺序
ProdMoniInfo prodMoniInfos[] =
{
    {PMD_OFFSET(tmCycle),				0x00160009, PT_1},		//制品时间
    {PMD_OFFSET(tmInject),				0x10160002,	PT_INJTM},	//射出时间
    {PMD_OFFSET(adPosi_InjectStart),	0x10160003,	PT_2OR1},		//射出起点
    {PMD_OFFSET(adPosi_InjectCushion),	0x10160005,	PT_2OR1},		//保压起点
    {PMD_OFFSET(adPosi_InjectHoldEnd),	0x10160004,	PT_2OR1},		//残料位置
    {PMD_OFFSET(adPosi_Charge),			0x1016000d,	PT_2OR1},		//储料位置
    {PMD_OFFSET(tmCharge),				0x1016000a,	PT_2},		//储料时间
    {PMD_OFFSET(tmCloseMold),			0x20160007, PT_2},		//关模计时
    {PMD_OFFSET(tmCloseLow),			0x20160009, PT_CLSTM},		//关模低压计时 20190226 YX
    {PMD_OFFSET(tmCloseHigh),			0x2016000b, PT_CLSTM},		//关模高压计时 20190226 YX
    {PMD_OFFSET(tmOpenMold),			0x2016000e, PT_2},		//开模计时
    {PMD_OFFSET(adPosi_OpenMold),		0x2016000d, PT_2OR1, TransMeter},//开模终点
    {PMD_OFFSET(Pres_TurnToHold),		0x10160007, PT_INJTUNIT},		//转保压压力 20190226 YX
    {PMD_OFFSET(Flow_TurnToHold),		0x10160010, PT_1},		//转保压速度
    {PMD_OFFSET(tmInjectBack),			0x1016000e, PT_2},		//射退计时
    {PMD_OFFSET(adPosi_Injectback),		0x10160011, PT_2OR1},		//射退终点
    {PMD_OFFSET(tmEject),				0x21160002, PT_2},		//托模计时
    {PMD_OFFSET(tmNozzleAdv),			0x11160002, PT_2},		//座台进计时
    {PMD_OFFSET(tmNozzleBack),			0x11160003, PT_2},		//座台退计时
    {PMD_OFFSET(Moni_AvgInjSpd),		0x10160013, PT_2OR1},		//射出平均速度
    {PMD_OFFSET(Moni_MaxInjPrs),		0x10160014, PT_INJTUNIT},		//射出最大压力 20190226 YX
    {PMD_OFFSET(Moni_MaxHoldPrs),		0x10160015, PT_INJTUNIT},		//保压最大压力 20190226 YX
    {PMD_OFFSET(Moni_MaxHoldMoveD),		0x10160016, PT_2OR1},		//保压移动距离
    {PMD_OFFSET(Moni_ClampMov),			0x0016001F, PT_1},		//全程计时
    {PMD_OFFSET(Mod_cnt1),		0x20160002, PT_0},		// 开模数记录
    {PMD_OFFSET(Mod_cnt2),		0x20160003, PT_0},		// 开模数记录
    //{PMD_OFFSET(Moni_InjSpd),			0x10160012, PT_1},		//射出速度
};

//排列顺序按照成品2页面下拉框顺序 //20200324
ProdMoniInfo prodMoniInfos32[] =
{
    {PMD_OFFSET32(tmCycle),				0x00160009, PT_2},          //制品时间
    {PMD_OFFSET32(tmInject),			0x10160002,	PT_INJTM32},	//射出时间
    {PMD_OFFSET32(adPosi_InjectStart),	0x10160003,	PT_1},          //射出起点
    {PMD_OFFSET32(adPosi_InjectCushion),0x10160005,	PT_1},          //保压起点
    {PMD_OFFSET32(adPosi_InjectHoldEnd),0x10160004,	PT_1},          //残料位置
    {PMD_OFFSET32(adPosi_Charge),		0x1016000d,	PT_1},          //储料位置
    {PMD_OFFSET32(tmCharge),			0x1016000a,	PT_2},          //储料时间
    {PMD_OFFSET32(tmCloseMold),			0x20160007, PT_2},          //关模计时
    {PMD_OFFSET32(tmCloseLow),			0x20160009, PT_2},          //关模低压计时
    {PMD_OFFSET32(tmCloseHigh),			0x2016000b, PT_2},          //关模高压计时
    {PMD_OFFSET32(tmOpenMold),			0x2016000e, PT_2},          //开模计时
    {PMD_OFFSET32(adPosi_OpenMold),		0x2016000d, PT_1, TransMeter},  //开模终点
    {PMD_OFFSET32(Pres_TurnToHold),		0x10160007, PT_1},          //转保压压力
    {PMD_OFFSET32(Flow_TurnToHold),		0x10160010, PT_1},          //转保压速度
    {PMD_OFFSET32(tmInjectBack),		0x1016000e, PT_2},          //射退计时
    {PMD_OFFSET32(adPosi_Injectback),	0x10160011, PT_1},          //射退终点
    {PMD_OFFSET32(tmEject),				0x21160002, PT_2},          //托模计时
    {PMD_OFFSET32(tmNozzleAdv),			0x11160002, PT_2},          //座台进计时
    {PMD_OFFSET32(tmNozzleBack),		0x11160003, PT_2},          //座台退计时
    {PMD_OFFSET32(Moni_AvgInjSpd),		0x10160013, PT_1},          //射出平均速度
    {PMD_OFFSET32(Moni_MaxInjPrs),		0x10160014, PT_1},          //射出最大压力
    {PMD_OFFSET32(Moni_MaxHoldPrs),		0x10160015, PT_1},          //保压最大压力
    {PMD_OFFSET32(Moni_MaxHoldMoveD),	0x10160016, PT_1},          //保压移动距离
    {PMD_OFFSET32(Moni_ClampMov),		0x0016001F, PT_2},          //全程计时
    {PMD_OFFSET32(Moni_Open1Time),      0x20160042, PT_2},          //开模一段计时 //20201217
    {PMD_OFFSET32(Moni_MaxLockPrs),		0x20160011, PT_1},          //锁模最大压力
    {PMD_OFFSET32(Mod_cnt1),		    0x20160002, PT_0},		    //开模数记录
    {PMD_OFFSET32(Mod_cnt2),		    0x20160003, PT_0},		    //开模数记录
};

#define  CMOD_WR_PATH   SAVE_PATH"new_prodmoni.dat"
#define  CMOD_WR_PATH32 SAVE_PATH"newprodmoni.dat"//20200324
#define  CMOD_WR_HEADSZ sizeof(ProdMoniHead)

ProdMoniDB		g_dbProdMoni;
ProdMoniDB32	g_dbProdMoni32;//20200324

static int		m_pageid = 1;
//static UI8		m_types[6] = {0,1,2,3,4,5};

#define PAGE_ROW_CNT	15
#define TMP_STARTNO		TMPVAL(0)
#define TMP_INDEX0		TMPVAL(1)	/*1-15*/
#define TMP_VAL0		TMPVAL(16)	/*16-105*/
#define TMP_VALPT0		TMPVAL(111)	/*111-200*/
#define TMP_TYPE0		TMPVAL(201) /*201-206*/
#define TMP_VALAVE0		TMPVAL(331)	//平均值
#define TMP_VALMAX0		TMPVAL(351)	//最大值
#define TMP_VALMIN0		TMPVAL(371)	//最小值

/************************************************************************/
/* private methods                                                      */
/************************************************************************/
static void ProdMoni__Load()
{
    int i;

    if(isUseProdMoniItem32())//20200324
    {
        if(!CModReadRaw(&g_dbProdMoni32, 0, sizeof(g_dbProdMoni32), CMOD_WR_PATH32) ||
                (g_dbProdMoni32.head.flag != MARK_USED) ||
                (g_dbProdMoni32.head.version != 1))
        {
            memset(&g_dbProdMoni32, 0 , sizeof(ProdMoniDB32));
            g_dbProdMoni32.head.flag = MARK_USED;
            g_dbProdMoni32.head.version = 1;

            // inital view columns
            for(i = 0; i < PRODMONI_VIEW_COL; ++i)
            {
                g_dbProdMoni32.head.view_cols[i] = Init_Cols[i];
            }
            ProdMoniSave();
        }

        // check out-of-range
        for(i = 0; i < PRODMONI_VIEW_COL; ++i)
        {
            if(g_dbProdMoni32.head.view_cols[i] > ARRAY_NUM(prodMoniInfos32))
            {
                g_dbProdMoni32.head.view_cols[i] = 0;
            }
        }

        //20241226 fqh 金鹰成品监测 开机产品数统计
        for(i = 0; i < PRODMONI_MAX_CNT; ++i)
        {
            if(g_dbProdMoni32.datas[i].tmCycle > 0)
            {
                JINYING_PRODMONI_COUNT++;
            }
        }
    }
    else
    {
        if(!CModReadRaw(&g_dbProdMoni, 0, sizeof(g_dbProdMoni), CMOD_WR_PATH) ||
                (g_dbProdMoni.head.flag != MARK_USED) ||
                (g_dbProdMoni.head.version != 1))
        {
            memset(&g_dbProdMoni, 0 , sizeof(ProdMoniDB));
            g_dbProdMoni.head.flag = MARK_USED;
            g_dbProdMoni.head.version = 1;

            // inital view columns
            for(i = 0; i < PRODMONI_VIEW_COL; ++i)
            {
                g_dbProdMoni.head.view_cols[i] = Init_Cols[i];
            }
            ProdMoniSave();
        }

        // check out-of-range
        for(i = 0; i < PRODMONI_VIEW_COL; ++i)
        {
            if(g_dbProdMoni.head.view_cols[i] > ARRAY_NUM(prodMoniInfos))
            {
                g_dbProdMoni.head.view_cols[i] = 0;
            }
        }
    }
}

static void ProdMoni__UpdateIndex(int startno)
{
    int i;
    for(i = 0; i < PAGE_ROW_CNT; ++i)
    {
        VarAdrSetInt(TMP_INDEX0 + i, startno + i);
    }
}

static PProdMoniInfo ProdMoni__FindInfo(int dataIndex)
{
    if(isUseProdMoniItem32())//20200324
    {
        if(dataIndex >= 0 && dataIndex < ARRAY_NUM(prodMoniInfos32))
        {
            return &prodMoniInfos32[dataIndex];
        }
    }
    else
    {
        if(dataIndex >= 0 && dataIndex < ARRAY_NUM(prodMoniInfos))
        {
            return &prodMoniInfos[dataIndex];
        }
    }

    return NULL;
}

static void ProdMoni__UpdateColumn(int startno, int column, int dataIndex)
{
    int i;
    PProdMoniInfo pinfo = ProdMoni__FindInfo(dataIndex);
    UI8* pdata = NULL;
    int PRODMONI_AVGMAXMIN[3];

    if(pinfo)
    {
        int pt = pinfo->get_pt();

        if(isUseProdMoniItem32())//20200324
        {
            for(i = 0; i < PAGE_ROW_CNT; ++i)
            {
                int realIndex = g_dbProdIndex32 - startno - i - 1;
                if(realIndex < 0)
                {
                    realIndex += PRODMONI_MAX_CNT;
                }
                pdata = (UI8*)&g_dbProdMoni32.datas[realIndex];

                VarAdrSetInt(TMP_VAL0 + column*PAGE_ROW_CNT + i,
                             *(UI32*)(pdata + pinfo->offset));

                VarAdrSetInt(TMP_VALPT0 + column*PAGE_ROW_CNT + i, pt);
            }

            //20241226 fqh 金鹰成品监测 平均值、最大值、最小值计算
           if(VarAdrToInt(p_PP_MACHSET_Prod_MONI_SENIOR)  && (VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)
           {
               ProductCntAvg(dataIndex, PRODMONI_AVGMAXMIN);
               VarAdrSetInt(TMP_VALAVE0 + column, PRODMONI_AVGMAXMIN[0]); //20241226 fqh 金鹰成品监测 平均值
               VarAdrSetInt(TMP_VALMAX0 + column, PRODMONI_AVGMAXMIN[1]); //20241226 fqh 金鹰成品监测 最大值
               VarAdrSetInt(TMP_VALMIN0 + column, PRODMONI_AVGMAXMIN[2]); //20241226 fqh 金鹰成品监测 最小值
           }
        }
        else
        {
            for(i = 0; i < PAGE_ROW_CNT; ++i)
            {
                int realIndex = g_dbProdIndex - startno - i - 1;
                if(realIndex < 0)
                {
                    realIndex += PRODMONI_MAX_CNT;
                }
                pdata = (UI8*)&g_dbProdMoni.datas[realIndex];

                VarAdrSetInt(TMP_VAL0 + column*PAGE_ROW_CNT + i,
                             *(UI16*)(pdata + pinfo->offset));

                VarAdrSetInt(TMP_VALPT0 + column*PAGE_ROW_CNT + i, pt);
            }
        }
    }
}

static void ProdMoni__Update(int startno)
{
    int i;
    ProdMoni__UpdateIndex(startno);

    for(i = 0; i < 6; ++i)
    {
        if(m_pageid == 1)
        {
            ProdMoni__UpdateColumn(startno, i, i);
        }
        else if(m_pageid == 2)
        {
            ProdMoni__UpdateColumn(startno, i, VarAdrToInt(TMP_TYPE0+i));
        }
    }
}

/************************************************************************/
/* MOD interface                                                        */
/************************************************************************/
void ProdMoniInital()
{
    ProdMoni__Load();
}

void ProdMoniChgNo()
{
    ProdMoni__Update(VarAdrToInt(TMP_STARTNO));
}

void ProdMoniPageIn( int pageid )
{
    m_pageid = pageid;
    VarAdrSetInt(TMP_STARTNO, 0);

    if(m_pageid == 2)
    {
        int i;
        for(i = 0; i < PRODMONI_VIEW_COL; ++i)
        {
            if(isUseProdMoniItem32())//20200324
            {
                VarAdrSetInt(TMP_TYPE0+i, g_dbProdMoni32.head.view_cols[i]);
            }
            else
            {
                VarAdrSetInt(TMP_TYPE0+i, g_dbProdMoni.head.view_cols[i]);
            }
        }
    }
    ProdMoni__Update(0);
}

void ProdMoniChgType( int index )
{
    if(isUseProdMoniItem32())//20200324
    {
        if(index >= 0 && index < PRODMONI_VIEW_COL)
        {
            int dataIndex = VarAdrToInt(TMP_TYPE0+index);
            g_dbProdMoni32.head.view_cols[index] = dataIndex;
            ProdMoni__UpdateColumn(VarAdrToInt(TMP_STARTNO), index, dataIndex);
            CModWriteHead(&g_dbProdMoni32, CMOD_WR_HEADSZ, CMOD_WR_PATH32, FALSE);
        }
    }
    else
    {
        if(index >= 0 && index < PRODMONI_VIEW_COL)
        {
            int dataIndex = VarAdrToInt(TMP_TYPE0+index);
            g_dbProdMoni.head.view_cols[index] = dataIndex;
            ProdMoni__UpdateColumn(VarAdrToInt(TMP_STARTNO), index, dataIndex);
            CMOD_WHEAD(&g_dbProdMoni);
        }
    }
}

void ProdMoniClearAll()
{
    if(isUseProdMoniItem32())//20200324
    {
        memset((char*)&g_dbProdMoni32+sizeof(ProdMoniHead), 0, sizeof(ProdMoniDB32)-sizeof(ProdMoniHead));
        g_dbProdMoni32.head.index = 0;

    }
    else
    {
        memset((char*)&g_dbProdMoni+sizeof(ProdMoniHead), 0, sizeof(ProdMoniDB)-sizeof(ProdMoniHead));
        g_dbProdMoni.head.index = 0;
    }

    JINYING_PRODMONI_COUNT = 0; //20241226 fqh 金鹰成品监测 计数值清零
    // keep view cloumns
    VarAdrSetInt(TMP_STARTNO, 0);
    ProdMoni__Update(0);
}

/************************************************************************/
/* public methods                                                       */
/************************************************************************/
void ProdMonitor()
{
    int i;

    if(isUseProdMoniItem32())//20200324
    {
        UI32 value;
        UI8* pdata = (UI8*)&g_dbProdMoni32.datas[g_dbProdIndex32];
        PProdMoniInfo pinfo = prodMoniInfos32;
        for(i = 0; i < ARRAY_NUM(prodMoniInfos32); ++i, ++pinfo)
        {

            value = VarAdrToInt(pinfo->addr);
            if(pinfo->addr == 0x20160002 || pinfo->addr == 0x20160003) //开模数16位地址
            {
                *(UI16*)(pdata + pinfo->offset) =
                        pinfo->trans_val ? pinfo->trans_val(value) : value;
            }
            else
            {
                *(UI32*)(pdata + pinfo->offset) =
                        pinfo->trans_val ? pinfo->trans_val(value) : value;
            }
        }
        g_dbProdMoni32.datas[g_dbProdIndex32].Moni_datetime = (UI32)time(NULL);//20211224 dyl 增加当前时间信息

        if(++g_dbProdIndex32 >= PRODMONI_MAX_CNT)
        {
            g_dbProdIndex32 = 0;
        }

        //20241226 fqh 金鹰成品监测 产品数量增加
        if(JINYING_PRODMONI_COUNT < PRODMONI_MAX_CNT)
        {
            JINYING_PRODMONI_COUNT++;
        }
    }
    else
    {
        UI16 value;
        UI8* pdata = (UI8*)&g_dbProdMoni.datas[g_dbProdIndex];
        PProdMoniInfo pinfo = prodMoniInfos;
        for(i = 0; i < ARRAY_NUM(prodMoniInfos); ++i, ++pinfo)
        {
            value = VarAdrToUI16(pinfo->addr);
            *(UI16*)(pdata + pinfo->offset) =
                    pinfo->trans_val ? pinfo->trans_val(value) : value;
        }

        g_dbProdMoni.datas[g_dbProdIndex].Moni_datetime = (UI32)time(NULL);//20211224 dyl 增加当前时间信息

        if(++g_dbProdIndex >= PRODMONI_MAX_CNT)
        {
            g_dbProdIndex = 0;
        }
    }

    //主画面射出bar 20190221 hz
    VarAdrSetInt(INJECT_TIME_LAST, VarAdrToInt(INJECT_STATE_TM_INJ_RT));
    VarAdrSetInt(COOL_TIME_LAST, VarAdrToInt(NOZZLE_STATE_TM_COOL_RT));
}


void ProdMoniSave()
{
    static UI16 prod_index =0;

    if(isUseProdMoniItem32())//20200324
    {
        if(prod_index == g_dbProdIndex32) return;
        prod_index = g_dbProdIndex32;
        CModWriteRaw(0, &g_dbProdMoni32, sizeof(g_dbProdMoni32), CMOD_WR_PATH32);
    }
    else
    {
        if(prod_index == g_dbProdIndex) return;
        //    oprintf("ProdMoniSave*******\r\n");
        prod_index = g_dbProdIndex;
        CModWriteRaw(0, &g_dbProdMoni, sizeof(g_dbProdMoni), CMOD_WR_PATH);
    }
}


static const char* GetPageLangStr(const char* page_name, const char* lang)
{
    char path[50];
    PHASH_TBL lang_tbl;
    strcpy(path, PAGE_PATH);
    strcat(path, page_name);
    strcat(path, ".lan");
    ParseCSVFile(path, &lang_tbl);
    return GetLanTran(lang_tbl, lang);
}

#define PRODUCT_2 PAGE_PRODUCT_2

/**
* @brief     :写表头
* @param     :文件句柄 文件名
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
/*//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL ExProdmoniExcelHead(FILEHD* stream, char* tem_str)
{
    char cbuf[255];
    char * p_str = NULL;

    sprintf(cbuf,"%s%s%s", USB_BOOT_PATH, tem_str,".csv");
    p_str = ConvToGBK_Dup(cbuf);

    if(!FileExist(p_str))
    {
        *stream = FileCreate(p_str);
    }
    else
    {
        FileDelete(p_str);
        *stream = FileCreate(p_str);
    }
    OBJMemFree(p_str);

    if(!FILEHD_IS_OK(*stream))
    {
        return FALSE;
    }
    else
    {
        UI8 bs[3] = { (UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
        FileWrite(*stream, bs, sizeof(bs), NULL);

        FilePrintf(*stream, "%s, ",GetPageLangStr(PRODUCT_2, "Current Time"));//20211224 dyl 增加当前时间信息

        FilePrintf(*stream, "%s, ",GetPageLangStr(PAGE_PRODMANAGE, "Real OpenCnt"));

        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "ProdTime"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "InjectTime"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "InjStartPos"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Hold StrPos"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "RestmillPos"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Charge Pos"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Chg Time"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Cls Time"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Cls LowPrs Time"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Cls HighPrs Time"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "OpenTime"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "OpenEndPos"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Turn Hold Prs"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Turn Hold Spd"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "SuckBk Tm"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "SuckEndPos"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Eject Time"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "NozAdv Tm"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "NozRet Tm"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Inj Avg Spd"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Max InjPrs"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Max HoldPrs"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Hold MoveDist"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Cycle Time"));
        FilePrintf( *stream, "%s, ",GetPageLangStr(PRODUCT_2, "Open#1 Tm"));//20201217
        FilePrintf( *stream, "%s\n",GetPageLangStr(PRODUCT_2, "Max LockPrs"));
        return TRUE;
    }
}*/
/**
* @brief     :写表的内容
* @param     :文件句柄，开始值，步长
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
/*//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL ExProdmoniExcelBody(FILEHD* stream, UI16 start, UI16 step)
{
    int j, i;
    char strtmp[255];

    if(!FILEHD_IS_OK(*stream))
    {
        return FALSE;
    }

    if(isUseProdMoniItem32())//20200324
    {
        for(j = 0; j<step; j++)
        {
            i = g_dbProdIndex32 - start - j - 1;

            if(i < 0)
            {
                i += PRODMONI_MAX_CNT;
            }
            if (g_dbProdMoni32.datas[i].Moni_datetime > 0)
            {
                TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",g_dbProdMoni32.datas[i].Moni_datetime);
                FilePrintf(*stream, "%s, ",strtmp);//20211224 dyl 增加当前时间信息
            }
            else
            {
                FilePrintf(*stream, "%d, ",0);//20211224 dyl 增加当前时间信息
            }

            FilePrintf(*stream, "%d, ", g_dbProdMoni32.datas[i].Mod_cnt1<<16 | g_dbProdMoni32.datas[i].Mod_cnt2);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmCycle/100.0);
            if(VarAdrToUI16(SYS_FL_MACH_CODE1) & 0x0008)
            {
                FilePrintf(*stream, "%.3f, ", g_dbProdMoni32.datas[i].tmInject/1000.0);
            }
            else
            {
                FilePrintf(*stream, "%.2f, ", g_dbProdMoni32.datas[i].tmInject/100.0);
            }
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].adPosi_InjectStart/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].adPosi_InjectCushion/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].adPosi_InjectHoldEnd/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].adPosi_Charge/10.0);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmCharge/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmCloseMold/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmCloseLow/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmCloseHigh/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmOpenMold/100.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].adPosi_OpenMold/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].Pres_TurnToHold/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].Flow_TurnToHold/10.0);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmInjectBack/100.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].adPosi_Injectback/10.0);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmEject/100.0);
            FilePrintf(*stream, "%.2f, ", g_dbProdMoni32.datas[i].tmNozzleAdv/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].tmNozzleBack/100.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].Moni_AvgInjSpd/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].Moni_MaxInjPrs/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].Moni_MaxHoldPrs/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].Moni_MaxHoldMoveD/10.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].Moni_ClampMov/100.0);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni32.datas[i].Moni_Open1Time/100.0);//20201217
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni32.datas[i].Moni_MaxLockPrs/10.0);

            FilePrintf(*stream, "\n");
        }
    }
    else
    {
        for(j = 0; j<step; j++)
        {
            i = g_dbProdIndex - start - j - 1;

            if(i < 0)
            {
                i += PRODMONI_MAX_CNT;
            }
            if(g_dbProdMoni.datas[i].Moni_datetime > 0)
            {
                TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",g_dbProdMoni.datas[i].Moni_datetime);
                FilePrintf(*stream, "%s, ",strtmp);//20211224 dyl 增加当前时间信息
            }
            else
            {
                FilePrintf(*stream, "%d, ",0);//20211224 dyl 增加当前时间信息
            }

            FilePrintf(*stream, "%d, ", g_dbProdMoni.datas[i].Mod_cnt1<<16 | g_dbProdMoni.datas[i].Mod_cnt2);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].tmCycle/10.0);
            if(VarAdrToUI16(SYS_FL_MACH_CODE1) & 0x0008)
            {
                FilePrintf(*stream, "%.3f, ", g_dbProdMoni.datas[i].tmInject/1000.0);
            }
            else
            {
                FilePrintf(*stream, "%.1f, ", g_dbProdMoni.datas[i].tmInject/10.0);
            }
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].adPosi_InjectStart/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].adPosi_InjectCushion/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].adPosi_InjectHoldEnd/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].adPosi_Charge/10.0);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmCharge/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmCloseMold/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmCloseLow/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmCloseHigh/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmOpenMold/100.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].adPosi_OpenMold/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].Pres_TurnToHold/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].Flow_TurnToHold/10.0);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmInjectBack/100.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].adPosi_Injectback/10.0);

            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmEject/100.0);
            FilePrintf(*stream, "%.2f, ", g_dbProdMoni.datas[i].tmNozzleAdv/100.0);
            FilePrintf(*stream, "%.2f, ",g_dbProdMoni.datas[i].tmNozzleBack/100.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].Moni_AvgInjSpd/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].Moni_MaxInjPrs/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].Moni_MaxHoldPrs/10.0);

            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].Moni_MaxHoldMoveD/10.0);
            FilePrintf(*stream, "%.1f, ",g_dbProdMoni.datas[i].Moni_ClampMov/10.0);

            FilePrintf(*stream, "\n");
        }
    }

    //        FileClose(stream);
    return TRUE;
}*/

//20241226 fqh 金鹰成品监测 成品监测画面平均值最大值最小值统计
void ProductCntAvg(int dataIndex, int*Prodmoni_AMM)
{
    UI32 dwAll = 0, dwMax = 0, dwMin = 0xFFFFFFFF;;
    UI32 dwValue = 0;
    UI32 dwAverage = 0;
    UI8* pdata = NULL;
    UI16  nRec;
    int i;
    PProdMoniInfo pinfo = ProdMoni__FindInfo(dataIndex);

    nRec = g_dbProdIndex32;
    if(pinfo)
    {
        if(JINYING_PRODMONI_COUNT > 0)
        {
            for(i = 0; i < JINYING_PRODMONI_COUNT; i++)
            {
                if(nRec==0) nRec = PRODMONI_MAX_CNT - 1;
                else nRec--;

                pdata = (UI8*)&g_dbProdMoni32.datas[nRec];

                dwAll += *(UI32*)(pdata + pinfo->offset);

                dwValue = *(UI32*)(pdata + pinfo->offset);

                if(dwMax < dwValue) dwMax = dwValue;
                if(dwMin > dwValue) dwMin = dwValue;
            }
            dwAverage = dwAll/JINYING_PRODMONI_COUNT;
            Prodmoni_AMM[0] = dwAverage;
            Prodmoni_AMM[1] = dwMax;
            Prodmoni_AMM[2] = dwMin;
        }
        else
        {
            dwAverage = 0;
            Prodmoni_AMM[0] = 0;
            Prodmoni_AMM[1] = 0;
            Prodmoni_AMM[2] = 0;
        }
    }


}

/**
* @brief     :写表头
* @param     :文件句柄 文件名
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL ExProdmoniExcelHead(FILEHD* stream, char* tem_str)
{
    TSTRING_CREATE(str_tran);

    char cbuf[255];
    char * p_str = NULL;

    sprintf(cbuf,"%s%s%s", USB_BOOT_PATH, tem_str,".csv");
    p_str = ConvToGBK_Dup(cbuf);

    if(!FileExist(p_str))
    {
        *stream = FileCreate(p_str);
    }
    else
    {
        FileDelete(p_str);
        *stream = FileCreate(p_str);
    }
    OBJMemFree(p_str);

    if(!FILEHD_IS_OK(*stream))
    {
        TSTRING_DESTROY(str_tran);
        return FALSE;
    }
    else
    {
        UI8 bs[3] = { (UI8)0xEF, (UI8)0xBB, (UI8)0xBF};
        FileWriteUdisk(*stream, bs, sizeof(bs), NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync

        tstring_appends_fix(&str_tran,GetPageLangStr(PRODUCT_2, "Current Time"), MIN_ALLOC_SIZE);//20211224 dyl 增加当前时间信息
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PAGE_PRODMANAGE, "Real OpenCnt"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "ProdTime"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "InjectTime"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "InjStartPos"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Hold StrPos"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "RestmillPos"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Charge Pos"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Chg Time"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Cls Time"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Cls LowPrs Time"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Cls HighPrs Time"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "OpenTime"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "OpenEndPos"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Turn Hold Prs"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Turn Hold Spd"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "SuckBk Tm"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "SuckEndPos"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Eject Time"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "NozAdv Tm"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "NozRet Tm"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Inj Avg Spd"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Max InjPrs"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Max HoldPrs"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Hold MoveDist"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Cycle Time"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Open#1 Tm"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_appends_fix(&str_tran, GetPageLangStr(PRODUCT_2, "Max LockPrs"), MIN_ALLOC_SIZE);
        tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);

        FileWriteUdisk(*stream, str_tran.str, str_tran.len, NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync

        TSTRING_DESTROY(str_tran);

        return TRUE;
    }
}
/**
* @brief     :写表的内容
* @param     :文件句柄，开始值，步长
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191101
*/
//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL ExProdmoniExcelBody(FILEHD* stream, UI16 start, UI16 step)
{
    TSTRING_CREATE(str_tran);
    char tmp[20];
    char strtmp[255];
    int j, i;

    if(!FILEHD_IS_OK(*stream))
    {
        TSTRING_DESTROY(str_tran);
        return FALSE;
    }

    if(isUseProdMoniItem32())//20200324
    {
        for(j = 0; j<step; j++)
        {
            i = g_dbProdIndex32 - start - j - 1;

            if(i < 0)
            {
                i += PRODMONI_MAX_CNT;
            }

            if (g_dbProdMoni32.datas[i].Moni_datetime > 0)
            {
                TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",g_dbProdMoni32.datas[i].Moni_datetime);
                tstring_appends_fix(&str_tran, strtmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }
            else
            {
                tstring_append_fix(&str_tran, '0', MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }

            WordToStr(tmp,g_dbProdMoni32.datas[i].Mod_cnt1<<16 | g_dbProdMoni32.datas[i].Mod_cnt2,0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni32.datas[i].tmCycle,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            if(VarAdrToUI16(SYS_FL_MACH_CODE1) & 0x0008)
            {
                WordToStr(tmp, g_dbProdMoni32.datas[i].tmInject,3);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }
            else
            {
                WordToStr(tmp, g_dbProdMoni32.datas[i].tmInject,2);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }
            WordToStr(tmp, g_dbProdMoni32.datas[i].adPosi_InjectStart,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].adPosi_InjectCushion,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].adPosi_InjectHoldEnd,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].adPosi_Charge,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmCharge,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmCloseMold,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmCloseLow,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmCloseHigh,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmOpenMold,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].adPosi_OpenMold,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].Pres_TurnToHold,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].Flow_TurnToHold,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmInjectBack,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].adPosi_Injectback,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmEject,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmNozzleAdv,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].tmNozzleBack,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].Moni_AvgInjSpd,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].Moni_MaxInjPrs,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].Moni_MaxHoldPrs,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp, g_dbProdMoni32.datas[i].Moni_MaxHoldMoveD,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni32.datas[i].Moni_ClampMov,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni32.datas[i].Moni_Open1Time,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni32.datas[i].Moni_MaxLockPrs,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);

        }
        oprintf("str_tran1.len:%ld\n",str_tran.len);
        FileWriteUdisk(*stream, str_tran.str, str_tran.len, NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync
    }
    else
    {
        for(j = 0; j<step; j++)
        {
            i = g_dbProdIndex - start - j - 1;

            if(i < 0)
            {
                i += PRODMONI_MAX_CNT;
            }

            if (g_dbProdMoni.datas[i].Moni_datetime > 0)
            {
                TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",g_dbProdMoni.datas[i].Moni_datetime);
                tstring_appends_fix(&str_tran, strtmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }
            else
            {
                tstring_append_fix(&str_tran, '0', MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }

            WordToStr(tmp,g_dbProdMoni.datas[i].Mod_cnt1<<16 | g_dbProdMoni.datas[i].Mod_cnt2,0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmCycle,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            if(VarAdrToUI16(SYS_FL_MACH_CODE1) & 0x0008)
            {
                WordToStr(tmp,g_dbProdMoni.datas[i].tmInject,3);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }
            else
            {
                WordToStr(tmp,g_dbProdMoni.datas[i].tmInject,1);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);
            }

            WordToStr(tmp,g_dbProdMoni.datas[i].adPosi_InjectStart,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].adPosi_InjectCushion,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].adPosi_InjectHoldEnd,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].adPosi_Charge,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmCharge,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmCloseMold,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmCloseLow,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmCloseHigh,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmOpenMold,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].adPosi_OpenMold,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Pres_TurnToHold,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Flow_TurnToHold,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmInjectBack,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].adPosi_Injectback,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmEject,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmNozzleAdv,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].tmNozzleBack,2);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Moni_AvgInjSpd,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Moni_MaxInjPrs,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Moni_MaxHoldPrs,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Moni_MaxHoldMoveD,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Moni_ClampMov,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Moni_ClampMov,1);//此处为了对齐上面的32位的结构体导出excel chj
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,g_dbProdMoni.datas[i].Moni_ClampMov,1);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);

        }
        oprintf("str_tran2.len:%ld\n",str_tran.len);
        FileWriteUdisk(*stream, str_tran.str, str_tran.len, NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync
    }

    TSTRING_DESTROY(str_tran);
    //        FileClose(stream);
    return TRUE;
}
