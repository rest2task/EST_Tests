#include "mainframe.h"
#include "oprintf.h"
#include "database.h"
#include "taskmoni.h"
#include "panel.h"

#define   MAX_ACT  4

static void SetActState(int nindex, UI16 wact, UI16 wstep, UI32 wsettime, UI32 woptime, UI32 wmovtime)
{
	UI32 wsetpid;
    UI32 addrstep[MAX_ACT] = {ACT_ACTNAME1,ACT_ACTNAME2, ACT_ACTNAME3, ACT_ACTNAME4};
    UI32 addrsettime[MAX_ACT] = {ACT_SETTIME1,ACT_SETTIME2 ,ACT_SETTIME3, ACT_SETTIME4};
    UI32 addroptime[MAX_ACT] = {ACT_ACTTIME1,ACT_ACTTIME2, ACT_ACTTIME3, ACT_ACTTIME4};

	if(nindex<0 || nindex >= MAX_ACT) 
		return;

	wsetpid = (wact << 16) + wstep;
	VarAdrSetInt(addrstep[nindex],wsetpid);
	VarAdrSetInt(addrsettime[nindex],wsettime);

	if((nindex == 0) && (OperateModeIndex()==0) && (woptime ==0)) //手动状态下，动作时间不清零
	{
		; //oprintf("not set optime");
	}
	else
		VarAdrSetInt(addroptime[nindex],woptime);

    // record automode act state for mainpage animation 20190220 hz
    if((OperateModeIndex() ==1) ||(OperateModeIndex() ==2) ||(OperateModeIndex() ==3)) //auto
        {
            if((wact == 0x2011)||(wact == 0x2012)) //close
            {
                if(VarAdrToUI16(ACT_STATE) != 3)
                    VarAdrSetInt(ACT_STATE, 3);
            }
            else if(wact == 0x1011) //inject
            {
                if(wstep >= 0x1c00)
                {
                    if(VarAdrToUI16(ACT_STATE) != 1) //hold
                         VarAdrSetInt(ACT_STATE, 1);
                }
            }
            else if(wact == 0x2013) //open
            {
                if(VarAdrToUI16(ACT_STATE) != 2)
                     VarAdrSetInt(ACT_STATE, 2);
            }
            else if(wact == 0x2113) //eject ret
            {
                if(VarAdrToUI16(ACT_STATE) != 0)
                     VarAdrSetInt(ACT_STATE, 0);
            }
        }
}

void ActCmdRx(PPROT_TSK_ST_MSG_PKT ppkt)
{
	UI16 i,j,k;
	UI16 num,nact;
	UI16 item_len;
    PPROT_TSK_ST_ITEM actdata =NULL;
	UI16 wsubstep,wtypeid,wstepms;
	BOOL bcanshow;
    UI16 *pdata;

	if(ppkt == NULL)
		return;

	num = ppkt->num;
	item_len = ppkt->item_len;
	if(num <= 0)
	{
		for(i=0; i<MAX_ACT; i++)
		{
			SetActState(i, 0, 0, 0, 0, 0);
		}
		return;
	}

	if(item_len < 7) 
		return;

    pdata = (UI16*)(ppkt+1);
    //actdata = (PPROT_TSK_ST_ITEM)(ppkt+1);
	//oprintf("RxCmd_Act num=%d, item len=%d, task_st=%d\n, act=%04x, step=%04x", num, item_len, actdata[1], actdata[0], actdata[2]);

	nact = 0;
    for(j=0; j<num; j++,pdata+=item_len)
	{
        actdata = (PPROT_TSK_ST_ITEM)pdata;
        if(actdata->tsk_st == 2)//task_st
		{
            wsubstep = actdata->step & 0xff00;
            wtypeid = actdata->act_id >> 8;
            wstepms = actdata->step_ms;

//            if(item_len <11)
//            {
//                actdata->act_tm_hi =0;
//                actdata->start_ms_hi = 0;
//            }

			//射出11段后没有0x10111900，结束步后不显示0xfe00,虚拟步不显示0x0f00,射出前面的部件不显示
			bcanshow = wstepms > 2 && (wtypeid == 0 || wtypeid >= 0x10) && wsubstep != 0x0f00 && wsubstep < 0xfe00 && (!(wtypeid == 0x1011 && wsubstep > 0x1900));
            bcanshow |= actdata->act_tm != 0;
			if(bcanshow)
			{
                if(item_len < 11)
                {
                    SetActState(nact++, actdata->act_id, actdata->step & 0xff00, actdata->act_tm, actdata->start_ms, actdata->mov_ms);
                }
                else
                {
                    if(actdata->act_tm != 0 && wsubstep == 0xff00)//20200930
                    {
                        //特殊处理只显示动作时间不显示动作名称
                        SetActState(nact++, 0, 0, actdata->act_tm_hi<<16|actdata->act_tm, actdata->start_ms_hi<<16|actdata->start_ms, actdata->mov_ms);
                    }
                    else
                    {
                        SetActState(nact++, actdata->act_id, actdata->step & 0xff00, actdata->act_tm_hi<<16|actdata->act_tm, actdata->start_ms_hi<<16|actdata->start_ms, actdata->mov_ms);
                    }
                }
			}
		}
	}
	for(k=nact; k<MAX_ACT; k++)
	{
		SetActState(k, 0, 0, 0, 0, 0);
	}
}


const char* WindowTitle()
{
	static PPAGE_FRAM  pf = NULL;
	static PPAGE_FRAM  pf_last = NULL;
	static LANG_ID lang_id = LANG_ERR;
	static LANG_ID lang_id_last = LANG_ERR;
	static char trans[MAX_PATH_LEN];

	pf = PanelCurPage();
	lang_id = GetCurrentLanguage();
	if(pf !=NULL)
	{
		if(pf != pf_last || lang_id !=lang_id_last)
		{
			pf_last = pf;
			lang_id_last = lang_id;
			trans[MAX_PATH_LEN -1] = '\0';
			strncpy(trans, PageFramTitle(pf), sizeof(trans) -1);
		}
	}
	return trans;
}
