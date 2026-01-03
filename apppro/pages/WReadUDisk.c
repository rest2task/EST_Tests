#include "WReadUDisk.h"
#include "typedef.h"
#include "pagefram.h"
#include "information.h"
#include "mod.h"
#include "stdio.h"
#include "file.h"
#include "database.h"
#include "editwgt.h"
#include "listboxwgt.h"
#include "oprintf.h"
#include "moldset.h"
#include "privdef.h"
#include "opencal.h"

DEFINE_READUDISK_MAP

CONNECT_READUDISK_EVENT

#define ADDR_FILENAME			0x331700D6		//文件名称对应地址
UI16 cPrivilegex=0;

static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button9OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(readudisk_pg.button9, WGT_CLICK_MSG, button9OnClick);
    WgtMsgConnect(readudisk_pg.button8, WGT_CLICK_MSG, button8OnClick);
}
static int ReadUDiskOnInit(ReadUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ReadUDiskOnShow(ReadUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	cPrivilegex = VarAdrToUI16(UDISK_WR_PRIVILEGE);

	if(!check_usb())
	{
        ListBoxSetDir(ppg->listbox1,USB_PARAM_PATH);
	}

	if(cPrivilegex ==1)
	{
		ListBoxSetSuffix(ppg->listbox1,".mdh");
	}
	else if(cPrivilegex ==2)
	{
		ListBoxSetSuffix(ppg->listbox1,".psh");
	}
	else if(cPrivilegex ==3)
	{
		ListBoxSetSuffix(ppg->listbox1,".cfg");
	}
	else if(cPrivilegex ==4)
	{
		ListBoxSetSuffix(ppg->listbox1,".mac");
	}
	else
	{
		ListBoxSetSuffix(ppg->listbox1,".wrk");//wok
	}

    VarAdrSetStr(ADDR_FILENAME, "");

	PageWgtSetFocus(ppg->listbox1);

	//bret = FALSE;
	VarAdrSetInt(UDIKS_READ_FLAG,0);
    return 0;
}
static int ReadUDiskOnHide(ReadUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ReadUDiskOnUpdate(ReadUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int ReadUDiskOnChange(ReadUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	const char* sText;

	//列表框选项
	if(pwgt == ppg->listbox1)
	{
		sText = ListBoxGetText(pwgt);
		EditSetText(ppg->edit1,sText);
	}
    else if(pwgt == PageGetWgtByName(readudisk_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(readudisk_pg_name);//20191223.cyx
    }

    return 0;
}
static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	char cmd[256]="\0";
    char cmd2[256]="\0";//20200730.cyx
	char* pstr;
    BOOL file_chk1,file_chk2;//20210531.cyx

	if (check_usb()){
		ShowMsg(VW_MSG_UDISKNOTEXIST);
		return FALSE;
	}

    if(VarAdrToUI16(OPMODE_INDEX)!=MODE_MANUAL) //20191223.cyx
    {
        ShowMsg(VW_MSG_MANUAL);
        return 0;
    }

	pstr  = VarAdrToStr(ADDR_FILENAME);
    pstr  = ConvToGBK_Dup(pstr);

	if (cPrivilegex==1)
	{
        char fname[128];
        snprintf(fname,sizeof(fname)-1, "%s%s.mdh", USB_PARAM_PATH,pstr);
        file_chk1 = CheckMoldFile(fname, FALSE);
        snprintf(fname,sizeof(fname)-1, "%s%s.opencal", USB_PARAM_PATH,pstr);
        file_chk2 = CheckOpenCalFile(fname, FALSE);
        snprintf(cmd,sizeof(cmd)-1, "cp %s%s.mdh  %smoldsetxtemp.dat",USB_PARAM_PATH,pstr,SAVE_PATH);
        snprintf(cmd2,sizeof(cmd2)-1, "cp %s%s.opencal  %sopentabcalxtemp.dat",USB_PARAM_PATH,pstr,SAVE_PATH);
	}
	else if(cPrivilegex==2)
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %s%s.psh  %spushtab.dat",USB_PARAM_PATH,pstr,SAVE_PATH);
	}
	else if (cPrivilegex==3)
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %s%s.cfg  %smachcfg.dat",USB_PARAM_PATH,pstr,SAVE_PATH);
	}
	else if (cPrivilegex==4)
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %s%s.mac  %smachtype.dat",USB_PARAM_PATH,pstr,SAVE_PATH);
	}
	else
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %s%s.wrk  %sest_cur.tag",USB_PARAM_PATH,pstr,DATA_PATH);
	}
    OBJMemFree(pstr);

	//oprintf("cmd===%s\r\n",cmd);

    //20210531.cyx
    if(cPrivilegex==1)
    {
        if(!file_chk1 || !file_chk2)
        {
            ShowMsg(VM_MSG_FILEFORMATERR);
            return 0;
        }
    }

    if (!osystem(cmd))
	{
		if (cPrivilegex==1)
		{
			MoldInput();
		}

		PanelSetJumpFlag(FALSE);//跳转刷新标记
		PanelCloseDialogByName(readudisk_pg_name);

		if(cPrivilegex==0) 
		{
			ShowMsg(VW_MSG_WORKPARREADSUCCESS);
		}
		else if(cPrivilegex==1) 
		{
            if (!osystem(cmd2))
            {
                OpenCalInput();
                ShowMsg(VW_MSG_MOLDPARREADSUCCESS);
            }
            else
            {
                ShowMsg(VW_MSG_MOLDPARREADFAIL);
            }
		}
		else if(cPrivilegex==2)
		{
			ShowMsg(VW_MSG_PUSHTABLEREADSUCCESS);
		}
		else if(cPrivilegex==3)
		{
			ShowMsg(VW_MSG_CONFIGREADSUCCESS);
		}
		else if(cPrivilegex==4)
		{
			ShowMsg(VW_MSG_CONFIGFILEREADSUCCESS);
		}
		else 
		{
			ShowMsg(VW_MSG_UDISKREADSUCCESSX);
		}

        //bret=TRUE;
        VarAdrSetInt(UDIKS_READ_FLAG,1);
	}
	else{
		if(cPrivilegex==0) 
		{
			ShowMsg(VW_MSG_WORKPARREADFAIL);
		}
		else if(cPrivilegex==1)
		{
			ShowMsg(VW_MSG_MOLDPARREADFAIL);
		}
		else if(cPrivilegex==2) 
		{
			ShowMsg(VW_MSG_PUSHTABLEREADFAIL);
		}
		else if(cPrivilegex==3)
		{
			ShowMsg(VW_MSG_CONFIGREADFAIL);
		}
		else if(cPrivilegex==4) 
		{
			ShowMsg(VW_MSG_CONFIGFILEREADFAIL);
		}
		else 
		{
			ShowMsg(VW_MSG_UDISKREADFAIL);
		}
	}

    return 0;
}
static int button9OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PanelCloseDialogByName(readudisk_pg_name);
    return 0;
}
