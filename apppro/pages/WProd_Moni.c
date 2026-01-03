#include "WProd_Moni.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodmoni.h"
#include "kjump.h"
#include "panelrec.h"
#include "usermod.h"//20210914 dyl touch
#include "dbenum.h"

DEFINE_PROD_MONI_MAP

CONNECT_PROD_MONI_EVENT

#define TMP_STARTNO		TMPVAL(0)
#define TMP_PASSWORD    TMPVAL(300) //清除密码
#define TMP_JINYING_VISIBLE    TMPVAL(380) //20241226 fqh 金鹰成品监测 平均值，最大值等是否可见

enum GOFFSET
{
	ID_NO	 = 1,
	ID_CLEAR = 2
};

int m_prodid;

static void CtlConnect()
{
}
static int Prod_MoniOnInit(Prod_Moni_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int Prod_MoniOnShow(Prod_Moni_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

	ProdMoniPageIn(1);
    m_prodid = g_dbProdIndex32;
 
	//key guide
    //PowerQCGuideSwtich();
    VarAdrSetInt(TMP_PASSWORD,0);//20191126.cyx
    WGTSetEnable(ppg->btok,FALSE);

    //20241226 fqh 金鹰成品监测 满足条件后平均值最大值最小值才可见
    if(VarAdrToInt(p_PP_MACHSET_Prod_MONI_SENIOR) && (VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)
    {
        VarAdrSetInt(TMP_JINYING_VISIBLE, 1);
    }
    else
    {
        VarAdrSetInt(TMP_JINYING_VISIBLE, 0);
    }

    return 0;
}
static int Prod_MoniOnHide(Prod_Moni_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Prod_MoniOnUpdate(Prod_Moni_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(m_prodid != g_dbProdIndex32)
	{
		ProdMoniChgNo();
        m_prodid = g_dbProdIndex32;
	}
    return 0;
}
static int Prod_MoniOnChange(Prod_Moni_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 goff;

	goff = WGTGetGOffset(pwgt);

	switch(goff)
	{
	case ID_CLEAR:
        PanelRecAdd(pwgt, PNL_CLEAR_ALL_REC, 0);
		ProdMoniClearAll();
		break;
	case ID_NO:
		ProdMoniChgNo();
		break;
	default:
		break;
	}

    if(pwgt == ppg->edpassword)//20191126.cyx
    {
        if(VarAdrToInt(TMP_PASSWORD) == 9595)
        {
            WGTSetEnable(ppg->btok,TRUE);
        }
        else
        {
            WGTSetEnable(ppg->btok,FALSE);
        }
    }
    else if(pwgt == ppg->btlast) //20200318
    {
        if(VarAdrToInt(TMP_STARTNO) <= 15)
        {
            VarAdrSetInt(TMP_STARTNO,0);
        }
        else
        {
            VarAdrSetInt(TMP_STARTNO,VarAdrToInt(TMP_STARTNO)-15);
        }
        ProdMoniChgNo();
    }
    else if(pwgt == ppg->btnext) //20200318
    {
        if(VarAdrToInt(TMP_STARTNO) >= 9970)
        {
            VarAdrSetInt(TMP_STARTNO,9985);
        }
        else
        {
            VarAdrSetInt(TMP_STARTNO,VarAdrToInt(TMP_STARTNO)+15);
        }
        ProdMoniChgNo();
    }

    return 0;
}
