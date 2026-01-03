#include "WIO_Redefine.h"
#include "typedef.h"
#include "pagefram.h"
#include "oprintf.h" 
#include "protcmd.h"
#include "tablectl.h"
#include "dropwgt.h"
#include "editwgt.h"
#include "ioredef.h"
#include "tstring.h"
#include "database.h"
#include "dropexcel.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_IO_REDEFINE_MAP

CONNECT_IO_REDEFINE_EVENT

#define TMP_PASSWORD TMPVAL(4)
#define INRANGE(_x, _min, _max) (((_x) >= (_min) ? ((_x) <= (_max) ? 1 : 0) : 0))

extern UI16 g_ioUpdateFlag;
PREDEF_INFO pioinfos[2];
#define ID_IN  0
#define ID_OUT 1
#define MIN_ALLOC_SIZE 512

#define DROPEXCEL_NAME(NUM,pbuff,size)   joinStrAndNum("dropexcel",NUM,pbuff,size)

//输入点强制输入标识
static BOOL UseForce[MAX_REDEFINE];
static BOOL ForceInput[MAX_REDEFINE];
#define TMP_CHECK0	TMPVAL(150)//20190516.cyx

//通过输入点查找当前是否强制输入 //20190516.cyx
static void GetForceBySn(int id)
{
    PPROT_TBL_HD ptb = GetTableByType(TB_I_LOGIC,TB_CUR);
    if(ptb != NULL)
    {
        TBL_INPUT *pinput=NULL;
        pinput = TableFindInput(ptb,pioinfos[ID_IN]->phy_sn[id]);
        if(pinput!=NULL)
        {
            if((((pinput->cfg)>>6)&0x01) == 1)
            {
                UseForce[id]=true;
            }
            else
            {
                UseForce[id]=false;
            }
            if((((pinput->cfg)>>5)&0x01) == 1)
            {
                ForceInput[id]=true;
            }
            else
            {
                ForceInput[id]=false;
            }
        }
        else
        {
            UseForce[id]=false;
            ForceInput[id]=false;
        }
    }
    else
    {
        UseForce[id]=false;
        ForceInput[id]=false;
    }
}

//通过输入点查找当前是否是反逻辑 //20190516.cyx
static BOOL GetCFGBySn(int id)
{
    PPROT_TBL_HD ptb = GetTableByType(TB_I_LOGIC,TB_CUR);
    if(ptb != NULL)
    {
        TBL_INPUT *pinput=NULL;
        pinput = TableFindInput(ptb,pioinfos[ID_IN]->phy_sn[id]);
        if(pinput!=NULL)
        {
            if(((pinput->cfg)>>7) == 1)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    return FALSE;
}

//初始化强制输出与反逻辑 //20190516.cyx
static void IO_RedefineShow()
{
    int i;
    BOOL value;
    for(i = 0; i < MAX_REDEFINE; ++i)
    {
        if(VarAdrToInt(pioinfos[ID_IN]->tmp_id + i))
        {
            value = GetCFGBySn(i);
            VarAdrSetInt(TMP_CHECK0+i,(UI32)value);
            GetForceBySn(i);
        }
        else
        {
            VarAdrSetInt(TMP_CHECK0+i,0);
        }
    }

}

// 更新定义下拉框数据
static void UpdateDef(PREDEF_INFO pinfo, int index, UI32 addr)
{
	PIntNode pnode;
	TSTRING_CREATE(str_data);
	TSTRING_CREATE(str_tran);
	int j = 0;
	int find_value = 0;
	char tmp[20];

    if(pinfo != NULL)
    {
        SC_FOREACH(pnode, pinfo->addr_list)
        {
            if((pnode->value >> 16) == (addr >> 16))
            {
                tstring_appends_fix(&str_data, GetMapTran(pinfo->name_tbl, pnode->value), MIN_ALLOC_SIZE);
                tstring_append_fix(&str_data, ';', MIN_ALLOC_SIZE);
                if(pnode->value == addr)
                {
                    find_value = pnode->value;
                }
                WordToStr(tmp, pnode->value, 0);
                tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
                tstring_append_fix(&str_tran, ';', MIN_ALLOC_SIZE);
                ++j;
            }
        }
        VarAdrSetStr(pinfo->tmp_def_data + index, str_data.str);
        VarAdrSetStr(pinfo->tmp_def_tran + index, str_tran.str);
        VarAdrSetInt(pinfo->tmp_def + index, find_value);
        TSTRING_DESTROY(str_data);
        TSTRING_DESTROY(str_tran);

    }
}

// 页面进入，临时变量赋值
static void UpdatePage(PREDEF_INFO pinfo)
{
	int i;

    if(pinfo != NULL)
    {
        VarAdrSetInt(pinfo->tmp_exchg0, 0);
        VarAdrSetInt(pinfo->tmp_exchg1, 0);

        if(!pinfo->addr_list || !pinfo->name_tbl) return;

        for(i = 0; i < MAX_REDEFINE; ++i)
        {
            //输入点表地址部件号+1
            UI32 addr = pinfo->log_sn[i];
            if(pinfo->table_type == TB_I_LOGIC && (addr!=0))
            {
                addr |= 0x10000;
            }

            //更新物理点号
            VarAdrSetInt(pinfo->tmp_id + i, pinfo->phy_sn[i]);
            //更新下拉框
            if(pinfo->phy_sn[i])
            {
                VarAdrSetInt(pinfo->tmp_comp + i, addr >> 16);
                UpdateDef(pinfo, i, addr);
            }
            else
            {
                //物理点为0清空下拉框
                VarAdrSetInt(pinfo->tmp_comp + i, 0);
                UpdateDef(pinfo, i, 0);
            }
        }
    }
}

static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(io_redefine_pg.button43, WGT_CLICK_MSG, button43OnClick);
}
static int IO_RedefineOnInit(IO_Redefine_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
//        int i;
//        char buff[20];
//        PWGT pwgt_dropexcel;

//        for(i = 0; i < 8; i++)
//        {
//            pwgt_dropexcel = PageGetWgtByName(io_redefine_pg_name,DROPEXCEL_NAME(i+1,buff,20));
//            setDropExcelViewSize(pwgt_dropexcel,470,330);
//            pwgt_dropexcel = PageGetWgtByName(io_redefine_pg_name,DROPEXCEL_NAME(i+9,buff,20));
//            setDropExcelViewSize(pwgt_dropexcel,470,330);
//            pwgt_dropexcel = PageGetWgtByName(io_redefine_pg_name,DROPEXCEL_NAME(i+17,buff,20));
//            setDropExcelViewSize(pwgt_dropexcel,430,375);
//            pwgt_dropexcel = PageGetWgtByName(io_redefine_pg_name,DROPEXCEL_NAME(i+25,buff,20));
//            setDropExcelViewSize(pwgt_dropexcel,500,360);
//        }

	pioinfos[ID_IN] = IoredefGetInfo(TB_I_LOGIC);
	pioinfos[ID_OUT] = IoredefGetInfo(TB_O_LOGIC);

	if(!pioinfos[ID_IN] || !pioinfos[ID_OUT])
	{
		OWARN("Can't get io info");
	}

	pioinfos[ID_IN]->tmp_exchg0   = TMPVAL(0);
	pioinfos[ID_IN]->tmp_exchg1   = TMPVAL(1);
	pioinfos[ID_IN]->tmp_id       = TMPVAL(10);
	pioinfos[ID_IN]->tmp_comp     = TMPVAL(30);
	pioinfos[ID_IN]->tmp_def      = TMPVAL(50);
	pioinfos[ID_IN]->tmp_def_data = TMPSTR(0);
	pioinfos[ID_IN]->tmp_def_tran = TMPSTR(10);

	pioinfos[ID_OUT]->tmp_exchg0  = TMPVAL(2);
	pioinfos[ID_OUT]->tmp_exchg1  = TMPVAL(3);
	pioinfos[ID_OUT]->tmp_id      = TMPVAL(60);
	pioinfos[ID_OUT]->tmp_comp    = TMPVAL(80);
	pioinfos[ID_OUT]->tmp_def     = TMPVAL(100);
	pioinfos[ID_OUT]->tmp_def_data= TMPSTR(20);
	pioinfos[ID_OUT]->tmp_def_tran= TMPSTR(30);

    return 0;
}
static int IO_RedefineOnShow(IO_Redefine_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_MODELOCK);//20210914 dyl touch

	//界面变量初始化
    VarAdrSetInt(TMP_PASSWORD, 1234);
	UpdatePage(pioinfos[ID_IN]);
	UpdatePage(pioinfos[ID_OUT]);
    IO_RedefineShow();//20190516.cyx

	WGTSetEnable(ppg->button43, FALSE);  //20181019hz

	//请求当前表
	RequestTable(FALSE);

    return 0;
}
static int IO_RedefineOnHide(IO_Redefine_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    return 0;
}
static int IO_RedefineOnUpdate(IO_Redefine_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageProtect();//20210914 dyl touch //PAGE_PROTECT

	if(g_ioUpdateFlag & 0x1)
	{
		UpdatePage(pioinfos[ID_IN]);
        IO_RedefineShow();//20190516.cyx
	}
	if(g_ioUpdateFlag & 0x2)
	{
		UpdatePage(pioinfos[ID_OUT]);
	}

	g_ioUpdateFlag = 0;
    return 0;
}
static int IO_RedefineOnChange(IO_Redefine_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	int addr, index, i;
	PREDEF_INFO pinfo;
	PATTR pattr;
	
	pattr = (PATTR)msg_para;
	addr = OBJAttrGetAdr(pattr);

	for(i = 0; i < ARRAY_NUM(pioinfos); ++i)
	{
		pinfo = pioinfos[i];
		if(INRANGE(addr, pinfo->tmp_id, pinfo->tmp_id + MAX_REDEFINE))
		{
			index = addr - pinfo->tmp_id;
			if(RU32(*pattr) == 0)
			{
				VarAdrSetInt(pinfo->tmp_comp + index, 0);
				UpdateDef(pinfo, index, 0);
			}
			break;
		}
		else if(INRANGE(addr, pinfo->tmp_comp, pinfo->tmp_comp + MAX_REDEFINE))
		{
			int comp_adr;

			index = addr - pinfo->tmp_comp;
			comp_adr = RU32(*pattr) << 16;
			UpdateDef(pinfo, index, comp_adr);
			break;
		}
	}

	if(pwgt==ppg->button43)
	{
		PageWgtSetFocus(ppg->edit5);
	}
    
	//20181019hz
	if (pwgt == ppg->edit50)
	{
		if (VarAdrToInt(TMP_PASSWORD) == 9510)
		{
			WGTSetEnable(ppg->button43, TRUE);
		} 
		else
		{
			WGTSetEnable(ppg->button43, FALSE);
		}
	}

    return 0;
}
static int button43OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	int i, j;
	PREDEF_INFO pinfo;
	REDEF_DATA redef_data;

    for(i = 0; i < MAX_REDEFINE; ++i)//20190516.cyx
    {
        SetIPointLG(i,VarAdrToInt(TMP_CHECK0+i));
        SetUseForce(i,UseForce[i]);
        SetForceInput(i,ForceInput[i]);
    }

	for(i = 0; i < ARRAY_NUM(pioinfos); ++i)
	{
		pinfo = pioinfos[i];

		redef_data.swap_sn[0] = VarAdrToInt(pinfo->tmp_exchg0);
		redef_data.swap_sn[1] = VarAdrToInt(pinfo->tmp_exchg1);

		for(j = 0; j < MAX_REDEFINE; ++j)
		{
			redef_data.phy_sn[j] = VarAdrToInt(pinfo->tmp_id + j);
			redef_data.log_sn[j] = VarAdrToInt(pinfo->tmp_def + j) & 0xFFF0FFFF;
		}
		IoredefRedefineTable(pinfo->table_type, &redef_data);
	}

	RequestTable(FALSE);

    return 0;
}
