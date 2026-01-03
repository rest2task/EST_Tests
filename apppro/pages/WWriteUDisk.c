#include "WWriteUDisk.h"
#include "typedef.h"
#include "pagefram.h"
#include "panel.h"
#include "common.h"
#include "information.h"
#include "editwgt.h"
#include "listboxwgt.h"
#include "file.h"
#include "mod.h"
#include "stdio.h"
#include "oprintf.h"
#include "database.h"
#include "moldset.h"
#include "privdef.h"
#include "opencal.h"

DEFINE_WRITEUDISK_MAP

CONNECT_WRITEUDISK_EVENT

#define ADDR_FILENAME			0x331700D6		//文件名称对应地址

UI16 cPrivilege=0;

static void UpdateList(WriteUDisk_PG* ppg) 
{
	const char*   sText;

	sText = ListBoxGetText(ppg->listbox1);
	EditSetText(ppg->edit1,sText);
} 

static void DeleteUdiskFile(WriteUDisk_PG* ppg)
{
	char* pstr;
	char* suffix;
	char cmd[256]="\0";
	if (check_usb())
	{
		ShowMsg(VW_MSG_UDISKNOTEXIST);
		return;
	}

	if (cPrivilege==1)
	{
		suffix=".mdh";
	}
	else if(cPrivilege==2)
	{
		suffix=".psh";
	}
	else if(cPrivilege==3)  
	{
		suffix=".cfg";
	}
	else if(cPrivilege==4)  
	{
		suffix=".mac";
	}
	else 
	{
		suffix=".wrk";//wok
	}

	pstr = VarAdrToStr(ADDR_FILENAME);
    pstr = ConvToGBK_Dup(pstr);//20220518
    snprintf(cmd,sizeof(cmd)-1, "rm %s%s%s",USB_PARAM_PATH,pstr,suffix);
    OBJMemFree(pstr);//20220518
	//oprintf("cmddxx=%s\r\n",cmd);
	if (!osystem(cmd))
	{
		ListBoxSetSuffix(ppg->listbox1,suffix);
		//UpdateList(ppg);
	}
}

static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static int button9OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para);
static void CtlConnect()
{
    WgtMsgConnect(writeudisk_pg.button9, WGT_CLICK_MSG, button9OnClick);
    WgtMsgConnect(writeudisk_pg.button10, WGT_CLICK_MSG, button10OnClick);
    WgtMsgConnect(writeudisk_pg.button8, WGT_CLICK_MSG, button8OnClick);
}
static int WriteUDiskOnInit(WriteUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int WriteUDiskOnShow(WriteUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	cPrivilege = VarAdrToUI16(UDISK_WR_PRIVILEGE);
	
	if(!check_usb())
	{
#ifndef WIN32
        ListBoxSetDir(ppg->listbox1,USB_PARAM_PATH);
#else
        ListBoxSetDir(ppg->listbox1,USB_PARAM_PATH);
#endif
	}
	
	//oprintf("cPrivilege=%d\r\n",cPrivilege);
	if(cPrivilege ==1)
	{
		ListBoxSetSuffix(ppg->listbox1,".mdh");
	}
	else if(cPrivilege ==2)
	{
		ListBoxSetSuffix(ppg->listbox1,".psh");
	}
	else if(cPrivilege ==3)
	{
		ListBoxSetSuffix(ppg->listbox1,".cfg");
	}
	else if(cPrivilege ==4)
	{
		ListBoxSetSuffix(ppg->listbox1,".mac");
	}
	else
	{
		ListBoxSetSuffix(ppg->listbox1,".wrk");//wok
	}

    VarAdrSetStr(ADDR_FILENAME, "");

	UpdateList(ppg); 
	PageWgtSetFocus(ppg->edit1);

    return 0;
}
static int WriteUDiskOnHide(WriteUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int WriteUDiskOnUpdate(WriteUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int WriteUDiskOnChange(WriteUDisk_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	const char* sText;

	//列表框选项
	if(pwgt == ppg->listbox1)
	{
		sText = ListBoxGetText(pwgt);
		EditSetText(ppg->edit1,sText);
	}
	//删除按钮
    else if(pwgt == ppg->button10)
	{
		DeleteUdiskFile(ppg);
	}
    else if(pwgt == PageGetWgtByName(writeudisk_pg_name,"btnclose"))
    {
        PanelCloseDialogByName(writeudisk_pg_name);//20191223.cyx
    }

    return 0;
}
static int button8OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	char cmd[256]="\0";
    char cmd2[256]="\0";//20200730.cyx
	char* pstr;

	if (check_usb())
	{
		ShowMsg(VW_MSG_UDISKNOTEXIST);
		return 0;
	}

    if(VarAdrToUI16(OPMODE_INDEX)!=MODE_MANUAL) //20191223.cyx
    {
        ShowMsg(VW_MSG_MANUAL);
        return -1;
    }

	if(!FileExist(USB_PARAM_PATH))
	{
		MkDir(USB_PARAM_PATH);
	}
    pstr = ConvToGBK_Dup(VarAdrToStr(ADDR_FILENAME));

	oprintf("pstr=%s\n",pstr);

	if((strlen(pstr)<1) || (*pstr==' '))
	{
		ShowMsg(VW_MSG_UDISKSAVEFAIL);
		return 0;
	}
    else
    {
        int i,j;
        char invalid[9] = {'<','>','/','\\','|',':','"','*','?'};//20191223.cyx 文件名不能包含下列任何字符
        for(i = 0;i < strlen(pstr);i++)
        {
            for(j = 0;j < 9;j++)
            {
                if(*(pstr+i) == invalid[j])
                {
                    ShowMsg(VW_MSG_FILENAMEERROR);
                    OBJMemFree(pstr);
                    return 0;
                }
            }
        }
    }

	if (cPrivilege==1)
	{
		char filepath[64];
		snprintf(filepath,sizeof(filepath)-1,"%s%s.mdh",USB_PARAM_PATH,pstr);
		MoldExportTo(cmd,filepath,sizeof(cmd));
        snprintf(filepath,sizeof(filepath)-1,"%s%s.opencal",USB_PARAM_PATH,pstr);
        OpenCalExportTo(cmd2,filepath,sizeof(cmd2));
	}
	else if (cPrivilege==2)
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %spushtab.dat %s%s.psh",SAVE_PATH, USB_PARAM_PATH,pstr);
	}
	else if (cPrivilege==3)
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %smachcfg.dat %s%s.cfg",SAVE_PATH, USB_PARAM_PATH,pstr);
	}
	else if (cPrivilege==4) 
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %smachtype.dat %s%s.mac",SAVE_PATH, USB_PARAM_PATH,pstr);
	}
	else 
	{
        snprintf(cmd,sizeof(cmd)-1, "cp %sest_cur.tag %s%s.wrk",DATA_PATH, USB_PARAM_PATH,pstr);
	}

    OBJMemFree(pstr); //20191223.cyx

	//oprintf("cmddd=%s\r\n",cmd);

	PanelSetJumpFlag(FALSE);//跳转刷新标记
	oprintf("PanelSetJumpFlag\r\n");
	PanelCloseDialogByName(writeudisk_pg_name);

	if(!osystem(cmd))
	{
		if(cPrivilege==0)
		{
			ShowMsg(VW_MSG_WORKPARSAVESUCCESS);
		}
		else if(cPrivilege==1)
		{
            if(!osystem(cmd2))//20200730.cyx
            {
                ShowMsg(VW_MSG_MOLDPARSAVESUCCESS);
            }
            else
            {
                ShowMsg(VW_MSG_MOLDPARSAVEFAIL);
            }
		}
		else if(cPrivilege==2)
		{
			ShowMsg(VW_MSG_PUSHTABLESAVESUCCESS);
		}
		else if(cPrivilege==3)
		{
			ShowMsg(VW_MSG_CONFIGSAVESUCCESS);
		}
		else if(cPrivilege==4) 
		{
			ShowMsg(VW_MSG_CONFIGFILESAVESUCCESS);
		}
		else 
		{
			ShowMsg(VW_MSG_UDISKSAVESUCCESSX);
		}
	}
	else{
		
		if(cPrivilege==0) 
		{
			ShowMsg(VW_MSG_WORKPARSAVEFAIL);
		}
		else if(cPrivilege==1)
		{
			ShowMsg(VW_MSG_MOLDPARSAVEFAIL);
		}
		else if(cPrivilege==2)
		{
			ShowMsg(VW_MSG_PUSHTABLESAVEFAIL);
		}
		else if(cPrivilege==3) 
		{
			ShowMsg(VW_MSG_CONFIGSAVEFAIL);
		}
		else if(cPrivilege==4)
		{
			ShowMsg(VW_MSG_CONFIGFILESAVEFAIL);
		}
		else
		{
			ShowMsg(VW_MSG_UDISKSAVEFAIL);
		}
	}

	//PanelSetJumpFlag(FALSE)	

    return 0;
}
static int button10OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int button9OnClick(PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	PanelCloseDialogByName(writeudisk_pg_name);
    return 0;
}
