#include "machcfg.h"
#include "oprintf.h" 
#include "database.h"
#include "command.h"
#include "file.h"
#include "protcmd.h"
#include "panel.h"
#include "information.h"
#include "tablectl.h" 
#include "ioredef.h"
#include "ctrlcmd.h"
#include "timecheck.h"

BOOL g_bMachCfgReset = FALSE;

static void SaveMachcfg(FILEHD file, int offset, void* src, UI16 wCount)
{
	int writebytes;

    FileSeek(file, offset);
    FileWrite(file, src, wCount, &writebytes);
}

static void LoadMachcfg(FILEHD file, int offset, void* dst, UI16 wCount)
{
	int readbytes;

    FileSeek(file, offset);
    FileRead(file, dst, wCount, &readbytes);
}

void SaveAllMachcfg()
{
    FILEHD filehd;
	UI16 wpos=0;
	UI16 part_sub_id,size;
	PREDEF_INFO pioinfo;

    if(FileExist(MACHINECFG_FILE))//20210120 保存之前先删除原有文件
    {
        FileDelete(MACHINECFG_FILE);
    }

    filehd = FileCreate(MACHINECFG_FILE);
    if(FILEHD_IS_OK(filehd))
    {
        //0x00
        part_sub_id = PART1_SUB_ID(MACHINE_ID,MACHSET);
        size = GetSubLenByID(part_sub_id);
        //20210120 保存数据前先保存部件号和部件长度
        SaveMachcfg(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, GetSubByID(part_sub_id,NULL), sizeof(UI16)*size);
        wpos = wpos +sizeof(UI16)*size;
        //0x07
        part_sub_id = PART1_SUB_ID(METER_ID,MACHSET);
        size = GetSubLenByID(part_sub_id);
        SaveMachcfg(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, GetSubByID(part_sub_id,NULL), sizeof(UI16)*size);
        wpos = wpos + sizeof(UI16)*size;

        //自定义标志　0x061A-IO输入物理点　0x061B-IO输入逻辑点　0x061C-IO输出物理点　0x061D-IO输出逻辑点
        pioinfo = IoredefGetInfo(TB_I_LOGIC);
        part_sub_id = 0x061A;
        size = sizeof(pioinfo->phy_sn);
        SaveMachcfg(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, pioinfo->phy_sn, sizeof(pioinfo->phy_sn));
        wpos += sizeof(pioinfo->phy_sn);

        part_sub_id = 0x061B;
        size = sizeof(pioinfo->log_sn);
        SaveMachcfg(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, pioinfo->log_sn, sizeof(pioinfo->log_sn));
        wpos += sizeof(pioinfo->log_sn);

        pioinfo = IoredefGetInfo(TB_O_LOGIC);
        part_sub_id = 0x061C;
        size = sizeof(pioinfo->phy_sn);
        SaveMachcfg(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, pioinfo->phy_sn, sizeof(pioinfo->phy_sn));
        wpos += sizeof(pioinfo->phy_sn);

        part_sub_id = 0x061D;
        size = sizeof(pioinfo->log_sn);
        SaveMachcfg(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveMachcfg(filehd, wpos, pioinfo->log_sn, sizeof(pioinfo->log_sn));
    }
    FileClose(filehd);
}

void LoadAllMachcfg()
{
    FILEHD filehd;
    UI16 wpos =0;
    UI16 part_sub_id,size;
    UI16 save_size;//20210120 文件保存的部件长度
    REDEF_DATA redef_data;
    int i;
    PREDEF_INFO pioinfos[2];
    pioinfos[0] = IoredefGetInfo(TB_I_LOGIC);
    pioinfos[1] = IoredefGetInfo(TB_O_LOGIC);

    for(i=0;i<8;i++)
    {
        redef_data.phy_sn[i]=0;
        redef_data.log_sn[i]=0;
    }
    IoredefRedefineTable(TB_I_LOGIC, &redef_data);
    IoredefRedefineTable(TB_O_LOGIC, &redef_data);
    for(i=0;i<8;i++)
    {
        pioinfos[0]->phy_sn[i]=0;
        pioinfos[0]->log_sn[i]=0;
        pioinfos[1]->phy_sn[i]=0;
        pioinfos[1]->log_sn[i]=0;
    }
    //RequestTable(FALSE);

    filehd = FileOpen(MACHINECFG_FILE);
    if(FILEHD_IS_OK(filehd))
    {
        //0x0012
        wpos = wpos + sizeof(UI16);//部件号 暂时不需要判断
        LoadMachcfg(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        part_sub_id = PART1_SUB_ID(MACHINE_ID,MACHSET);
        size = GetSubLenByID(part_sub_id);
        if(size > save_size)//20210120 保存时变量数与现有变量数取最小值
        {
            size = save_size;
        }
        LoadMachcfg(filehd, wpos, GetSubByID(part_sub_id,NULL), sizeof(UI16)*size);
        wpos = wpos +sizeof(UI16)*save_size;
        //0x0712
        wpos = wpos + sizeof(UI16);
        LoadMachcfg(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        part_sub_id = PART1_SUB_ID(METER_ID,MACHSET);
        size = GetSubLenByID(part_sub_id);
        if(size > save_size)
        {
            size = save_size;
        }
        LoadMachcfg(filehd, wpos, GetSubByID(part_sub_id,NULL), sizeof(UI16)*size);
        wpos = wpos + sizeof(UI16)*save_size;
        //0x061A
        wpos = wpos + sizeof(UI16);
        LoadMachcfg(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        size = sizeof(redef_data.phy_sn);
        if(size > save_size)
        {
            size = save_size;
        }
        LoadMachcfg(filehd, wpos, redef_data.phy_sn, size);
        wpos += save_size;
        //0x061B
        wpos = wpos + sizeof(UI16);
        LoadMachcfg(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        size = sizeof(redef_data.log_sn);
        if(size > save_size)
        {
            size = save_size;
        }
        LoadMachcfg(filehd, wpos, redef_data.log_sn, size);
        wpos += save_size;
        IoredefRedefineTable(TB_I_LOGIC, &redef_data);
        //0x061C
        wpos = wpos + sizeof(UI16);
        LoadMachcfg(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        size = sizeof(redef_data.phy_sn);
        if(size > save_size)
        {
            size = save_size;
        }
        LoadMachcfg(filehd, wpos, redef_data.phy_sn, size);
        wpos += save_size;
        //0x061D
        wpos = wpos + sizeof(UI16);
        LoadMachcfg(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        size = sizeof(redef_data.log_sn);
        if(size > save_size)
        {
            size = save_size;
        }
        LoadMachcfg(filehd, wpos, redef_data.log_sn, size);

        IoredefRedefineTable(TB_O_LOGIC, &redef_data);
        RequestTable(FALSE);
    }
    FileClose(filehd);
}

BOOL MachCfgReset()
{
	VarAdrSetInt(SYS_FL_CTRLDATE,0);
	ProtSysCtrl(CFG_RESET_CMD,0,NULL);//machine cfg reset cmd
	RequestCfgData();
	g_bMachCfgReset = TRUE;
	RequestTable(TRUE);//获得原始数据
	return TRUE;
}

void MachCfgExport()
{
	SaveAllMachcfg();
}


