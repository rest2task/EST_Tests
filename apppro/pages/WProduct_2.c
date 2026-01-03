#include "WProduct_2.h"
#include "typedef.h"
#include "pagefram.h"
#include "prodmoni.h"
#include "information.h"
#include "privdef.h"
#include "file.h"
#include "progbarwgt.h"
#include "labelwgt.h"
#include "oprintf.h"
#include "time.h"
#include "stdio.h"
#include "dropexcel.h"
#include "panelrec.h"
#include "database.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_PRODUCT_2_MAP

CONNECT_PRODUCT_2_EVENT

#define TMP_STARTNO		TMPVAL(0)
#define TMP_PASSWORD    TMPVAL(300) //清除密码
#define TMP_JINYING_VISIBLE    TMPVAL(380) //20241226 fqh 金鹰成品监测 平均值，最大值等是否可见

enum GOFFSET
{
	ID_NO	 = 1,
	ID_CLEAR = 2,
    ID_EXP   = 3,
	ID_TYPE1 = 11,
	ID_TYPE2 = 12,
	ID_TYPE3 = 13,
	ID_TYPE4 = 14,
	ID_TYPE5 = 15,
	ID_TYPE6 = 16,
	ID_TYPE7 = 17,
};

int m_prodidx;

static BOOL b_expexcel_start = FALSE;
static void ExpExcel()
{
    static FILEHD stream;
    UI16 start;
    char text[40];
    UI16 real;
    UI32 filetime;
    const UI16 step = 100;//20230518 chj 1000->100 查出filewrite的最大数组，所以改小 原先的fileprintf方式有问题，每次调用都会写文件
    static UI16 cycle_num = 0;


    if(b_expexcel_start && (cycle_num == 0))
    {
        filetime = (UI32)time(NULL);
        TimeToStr(text,"yyyy-MM-dd_hh-mm-ss", filetime);
        ExProdmoniExcelHead(&stream, text);
    }

    if((cycle_num < step) && b_expexcel_start)
    {
        start = (PRODMONI_MAX_CNT / step) * cycle_num;
        real = (int)(cycle_num+1)*(90.0/step);
        ExProdmoniExcelBody(&stream, start, (PRODMONI_MAX_CNT / step));
        ProgbarSetCurValue(product_2_pg.progbar1, real);
        snprintf(text, sizeof(text), "%d%%", real);
        LabelSetText(product_2_pg.label199, text);
        cycle_num++;
    }

    if(cycle_num == step)
    {
        cycle_num = 0;
        b_expexcel_start = FALSE;
        WGTSetEnable(product_2_pg.button9, TRUE);
        WGTSetVisible(product_2_pg.label200, FALSE);
        FileClose(stream);
        ProgbarSetCurValue(product_2_pg.progbar1, 100);
        snprintf(text, sizeof(text), "%d%%", 100);
        LabelSetText(product_2_pg.label199, text);

    }
}

static void CtlConnect()
{
}
static int Product_2OnInit(Product_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
//    setDropExcelViewSize(ppg->dropexcel1,468,300);
//    setDropExcelViewSize(ppg->dropexcel2,468,300);
//    setDropExcelViewSize(ppg->dropexcel3,468,300);
//    setDropExcelViewSize(ppg->dropexcel4,468,300);
//    setDropExcelViewSize(ppg->dropexcel5,468,300);
//    setDropExcelViewSize(ppg->dropexcel6,468,300);

    return 0;
}
static int Product_2OnShow(Product_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

	ProdMoniPageIn(2);
    m_prodidx = g_dbProdIndex32;
    ProgbarSetCurValue(product_2_pg.progbar1, 0);
    WGTSetVisible(ppg->progbar1, FALSE);
    WGTSetVisible(ppg->label199, FALSE);
    WGTSetVisible(ppg->label200, FALSE);
    WGTSetEnable(ppg->button9, TRUE);

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
static int Product_2OnHide(Product_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch
    return 0;
}
static int Product_2OnUpdate(Product_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    if(m_prodidx != g_dbProdIndex32)
	{
		ProdMoniChgNo();
        m_prodidx = g_dbProdIndex32;
	}
    ExpExcel();
    return 0;
}
static int Product_2OnChange(Product_2_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
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
    case ID_EXP:
        if(check_usb())  //!FileExist(USB_PATH)
        {
            ShowMsg(VW_MSG_UDISKNOTEXIST);
            return FALSE;
        }
        if(VarAdrToUI16(OPMODE_INDEX) != MODE_MANUAL)//20220324 只有手动状态下才能保存
        {
            ShowMsg(VW_MSG_MANUAL);
            return FALSE;
        }
        if(!b_expexcel_start)
            b_expexcel_start = TRUE;
        WGTSetVisible(ppg->progbar1, TRUE);
        WGTSetVisible(ppg->label199, TRUE);
        WGTSetVisible(ppg->label200, TRUE);
        WGTSetEnable(ppg->button9, FALSE);
        PageWgtSetFocus(ppg->edit5);//20210513
        PanelRecAdd(pwgt, PNL_EXP_EXCEL, 0);
        break;
    case ID_TYPE1:
	case ID_TYPE2:
	case ID_TYPE3:
	case ID_TYPE4:
	case ID_TYPE5:
	case ID_TYPE6:
	case ID_TYPE7:
		ProdMoniChgType(goff - ID_TYPE1);
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
    else if(pwgt == ppg->btlast)//20200318
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
    else if(pwgt == ppg->btnext)//20200318
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
