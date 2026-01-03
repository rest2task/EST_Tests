#include "dataOperation.h"
#include "database.h"
#include "protcmd.h"
#include "monmcu.h"
#include "oprintf.h"
#include "stdio.h"
#include "string.h"
#include "questionDlg.h"
#include "file.h"
#include "servo.h"
#include "stdlib.h"
#include "machcfg.h"
#include "pushtab.h"
#include "vardb.h"
#include "command.h"
#include "canmod.h"
#include "dataCfgtb.h"
#include "common.h"
#include "information.h"
#include "ctrlcmd.h"
#include "panelcfg.h"
#include "WData_Operation.h"
#include "panelrec.h"
#include "moldset.h"
#include "opencal.h"
#include "tstring.h"
#include "oprintf.h"
#define MIN_ALLOC_SIZE 512

void Question_OK_RestoreToSave();
void Question_OK_RestoreToDefault();
void Question_OK_ExportToUdisk();
static BOOL bServoOperateDelay = false;
static BOOL bServoCanOperate = false;
BOOL bReadPanel;//读取面板数据标志

static UI16 OverwriteType=0;
static QUESTION g_question;
UI8 nSecond=0;//超时记录的次数
UI32 nCounter=0;//记录当前循环周期时间
int g_opState = 0;//当前的操作状态
static int g_curSel=0;		//当前选择的
/************************************************************************/
/* 伺服是否可以操作                                             */
/************************************************************************/
BOOL getServoOperateEnable()
{
	return bServoCanOperate;
}

/**
* @brief     :伺服操作相关状态位初始化
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void ModInit()
{
	OverwriteType=0;
	bServoOperateDelay = false;
    if( ((VarAdrToInt(SYS_FL_MACH_CODE0)&0x0004) == 0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) && (VarAdrToInt(MOTOR_STATE_STATE) ==0) && (getServoIsOnline()!=0))
	{
		bServoCanOperate=TRUE;
	}
	else
	{
		bServoCanOperate=FALSE;
	}
}

UI32 getBgCYCTime()
{
	return VarAdrToInt(PROG_CYCTIME);
}

static void resetTimer()
{
	nCounter=getBgCYCTime();
	nSecond=0;
}
//伺服CAN通信状态
int getServoIsOnline()
{
	int i;

	for(i=0;i<7;i++)
	{
		if(((VarAdrToInt(SERVO_STATE_CANST1+i)&0x80)) && (((VarAdrToInt(SERVO_STATE_CANST1+i)&0x07) == 3)||((VarAdrToInt(SERVO_STATE_CANST1+i)&0x07) == 0))) //0x80代表在线不在线
		{
			return (i+1);
		}
	}
	
	return 0;
}


void RebootOK(PCPAR para)
{
#ifdef WIN32
	oprintf("reboot\n");
#else
	osystem("reboot");
#endif
}

static void SaveDACfgFile(FILEHD file, int offset, void* src, UI16 wCount)
{
    int writebytes;

    FileSeek(file, offset);
    FileWrite(file, src, wCount, &writebytes);
}

/**
* @brief     :重置面板部件指定类别
* @param     :部件类别号
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
static void PanelPartReset(UI8 subID)
{
	UI16 part_sub_id,size;
	PVAR psubvar = NULL;
	part_sub_id = PART1_SUB_ID(PANELDATA_ID,subID);
	psubvar = GetSubVarById(part_sub_id,&size);
	if(psubvar == NULL || size== 0)
		return ;
	while((size--)&& VarToVarAdr(psubvar)!= 0)
	{
		if(VarChkAttrBit(psubvar, VAR_NOT_SAVE_BIT))
		{
			VarSaveInt(psubvar);
		}
		psubvar++;
	}
}

/************************************************************************/
/* 
把0x33中带有保存的数据赋值到  VARDB_WORK_FILE中
*/
/************************************************************************/
void SavePanelData()
{
	PanelPartReset(MACHSET);
	PanelPartReset(MOLDSET);
    //20190719 hz
    PanelPartReset(NETSET);
    PanelPartReset(MACH2);
    PanelPartReset(STANDV);
}

/**
* @brief     :DA数据保存到文件
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void SaveDAData()
{
    FILEHD filehd;
	UI16 wpos=0;
	UI16 part_sub_id,size;

    if(FileExist(PANEL_DACFG))//20210120 保存之前先删除原有文件
    {
        FileDelete(PANEL_DACFG);
    }

    filehd = FileCreate(PANEL_DACFG);
    if(FILEHD_IS_OK(filehd))
    {
        //0x09 液压动力部件,归零数据
        part_sub_id = PART1_SUB_ID(HYDR_ID,ACTPARA);
        size = GetSubLenByID(part_sub_id);
        //20210120 保存数据前先保存部件号和部件长度
        SaveDACfgFile(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveDACfgFile(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveDACfgFile(filehd, wpos, GetSubByID(part_sub_id,NULL), sizeof(UI16)*size);
        wpos += size * sizeof(UI16);

        //0x10 背压通道
        part_sub_id = PART1_SUB_ID(INJECT_ID,HYDR);
        size = sizeof(UI16);
        SaveDACfgFile(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveDACfgFile(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveDACfgFile(filehd, wpos,GetSubByID(part_sub_id,NULL)+0x15,sizeof(UI16));
        wpos += sizeof(UI16);

        //0x09 DA输出储料
        part_sub_id = PART1_SUB_ID(HYDR_ID,MOLDSET);
        size = sizeof(UI16);
        SaveDACfgFile(filehd, wpos, &part_sub_id, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveDACfgFile(filehd, wpos, &size, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        SaveDACfgFile(filehd, wpos, GetSubByID(part_sub_id,NULL)+0x01, sizeof(UI16));
    }
    FileClose(filehd);
}

static void LoadDAcfgFile(FILEHD file, int offset, void* dst, UI16 wCount)
{
    int readbytes;

    FileSeek(file, offset);
    FileRead(file, dst, wCount, &readbytes);
}

/**
* @brief     :发送液压动力部件数据, 保存DA曲线数据, 发送背压通道子类，DA输出储料子类数据
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void LoadDACfgData()
{
    FILEHD filehd;
	UI16 wpos =0;
	UI16 part_sub_id,size;
	UI16 part_id;
    UI16 save_size;//20210120 文件保存的部件长度

    filehd = FileOpen(PANEL_DACFG);
    if(FILEHD_IS_OK(filehd))
    {
        //液压动力部件,归零数据
        wpos = wpos + sizeof(UI16);//部件号 暂时不需要判断
        LoadDAcfgFile(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        part_sub_id = PART1_SUB_ID(HYDR_ID,ACTPARA);
        size = GetSubLenByID(part_sub_id);
        if(size > save_size)//20210120 保存时变量数与现有变量数取最小值
        {
            size = save_size;
        }
        LoadDAcfgFile(filehd, wpos, GetSubByID(part_sub_id,NULL),sizeof(UI16)*size);
        wpos = wpos +sizeof(UI16)*save_size;
        //m_pdrv->CreateACommTask(COMM_TX_SUBCLASS,0x0910,1<<4);
        part_id = PART1_ID(HYDR_ID);
        PartSubTx(GetPartByID(part_id), 1<<ACTPARA, FALSE);

        //SendComm_Command(COMM_SUBCMD_DACURVESAVE);
        ProtSysCtrl(DA_CUR_SAVE_CMD,0,NULL);

        //背压通道
        wpos = wpos + sizeof(UI16);
        LoadDAcfgFile(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        part_sub_id = PART1_SUB_ID(INJECT_ID,HYDR);
        size = sizeof(UI16);
        if(size > save_size)//20210120 保存时变量数与现有变量数取最小值
        {
            size = save_size;
        }
        LoadDAcfgFile(filehd, wpos, GetSubByID(part_sub_id,NULL)+0x15, sizeof(UI16));
        wpos = wpos +sizeof(UI16);
        size = GetSubLenByID(part_sub_id);
        //SendComm_Data(0x10150015);
        PartDataTx(GetPartByID(PART1_ID(INJECT_ID)), (UI8)part_sub_id,size,0x15, FALSE);

        //DA输出储料
        wpos = wpos + sizeof(UI16);
        LoadDAcfgFile(filehd, wpos, &save_size, sizeof(UI16));
        wpos = wpos + sizeof(UI16);
        part_sub_id = PART1_SUB_ID(HYDR_ID,MOLDSET);
        size = sizeof(UI16);
        if(size > save_size)//20210120 保存时变量数与现有变量数取最小值
        {
            size = save_size;
        }
        LoadDAcfgFile(filehd, wpos, GetSubByID(part_sub_id,NULL)+0x01,sizeof(UI16));
        size = GetSubLenByID(part_sub_id);
        //SendComm_Data(0x09130001);
        PartDataTx(GetPartByID(PART1_ID(HYDR_ID)), (UI8)part_sub_id,size,0x01, FALSE);
    }
    FileClose(filehd);
}


/**
* @brief     :获得伺服数据
* @param     :伺服在线的个数
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void getServoAllData(int Id)
{
	int i;
	if(Id<=0 ||Id>7) 
	{
		return;
	}
	for( i=0;i<SERVOXADDR_SAVECOUNT;i++)
	{
		Send_TaskCan(CAN_TX_READ,Id,ServoxAddr[i],0);
		//oprintf("ServoxAddr=%x\r\n",ServoxAddr[i]);
	}
}

/**
* @brief     :从文件中导入伺服数据
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
BOOL LoadServoFile(const char * filepath)
{
	if(!FileExist(filepath))
	{
		return FALSE;
	}
	else
	{
		FILEHD filehd;
		filehd = FileOpen(filepath);
		if(FILEHD_IS_OK(filehd))
		{
			UI16 wVersion;
			UI16 wServoCount;
			UI16 wParamCount;
			UI16 Pos=0;
			UI16 i;
			UI16 Id;
			FileSeek(filehd,Pos);
			FileRead(filehd,(char*)&wVersion,sizeof(UI16),NULL);
			Pos += sizeof(UI16);
			if(wVersion != 0) return false;
			FileSeek(filehd,Pos);
			FileRead(filehd,(char*)&wServoCount,sizeof(UI16),NULL);
			Pos += sizeof(UI16);
			FileSeek(filehd,Pos);
			FileRead(filehd,(char*)&wParamCount,sizeof(UI16),NULL);
			Pos += sizeof(UI16);
			if(wServoCount == 0 || wParamCount ==0) return false;

			for( i=0;i<wServoCount;i++)
			{
				FileSeek(filehd,Pos);
				FileRead(filehd,(char*)&Id,sizeof(UI16),NULL);
				Pos = Pos + sizeof(UI16);
				FileSeek(filehd,Pos);
				FileRead(filehd,(char*)&dbsvox[Id].wData,sizeof(UI16)*wParamCount,NULL);
				dbsvox[Id].fCover = 1;//代表被导入数据过
				Pos = Pos +sizeof(UI16)*wParamCount;
			}
			FileClose(filehd);
			return TRUE;
		}
	}
	return TRUE;
}

/**
* @brief     :dbsvox中的数据保存到文件
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
BOOL SaveServoFile(const char* filepath)
{
	FILEHD filehd;
	if(filepath == NULL)
	{
		return FALSE;
	}
	if(!FileExist(filepath))
	{
		filehd = FileCreate(filepath);
	}
	else
	{
		filehd = FileOpen(filepath);
	}
	if(FILEHD_IS_OK(filehd))
	{
		UI16 wVersion=0;
		UI16 wServoCount = 0;
		UI16 wParamCount = SERVOXADDR_SAVECOUNT;
		UI16 Pos;
		int i=0;
		FileSeek(filehd,0);
        FileWriteUdisk(filehd,(char*)&wVersion,sizeof(UI16),NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync

		for(i=1;i < 8;i++)
		{
			if(dbsvox[i].fCover == 2)
				wServoCount += dbsvox[i].fCover;
		}
		wServoCount = wServoCount/2;
		FileSeek(filehd,sizeof(UI16));
        FileWriteUdisk(filehd,(char*)&wServoCount,sizeof(UI16),NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync

		FileSeek(filehd,sizeof(UI16)*2);
        FileWriteUdisk(filehd,(char*)&wParamCount,sizeof(UI16),NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync

		Pos = sizeof(UI16)*3;
		if(wServoCount == 0 || wParamCount ==0) return false;

		for(i=1;i < 8;i++)
		{
			if(dbsvox[i].fCover == 2)
			{
				dbsvox[i].fCover = 0;
				FileSeek(filehd,Pos);
                FileWriteUdisk(filehd,(char*)&i,sizeof(UI16),NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync
				Pos = Pos + sizeof(UI16);
				FileSeek(filehd,Pos);
                FileWriteUdisk(filehd,(char*)&dbsvox[i].wData[0],sizeof(UI16)*wParamCount,NULL);//20240923 chj FileWrite-》替换为 FileWriteUdisk,调用后，底层过100ms，主动sync
				Pos = Pos +sizeof(UI16)*wParamCount;
			}
		}
		FileClose(filehd);
		return TRUE;
	}
	return FALSE;
}


/**
* @brief     :进行导入导出操作时的监控函数，用于update
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
BOOL DoOPMonitor()
{
    if( ((VarAdrToInt(SYS_FL_MACH_CODE0)&0x0004) == 0) && (VarAdrToInt(SYS_FL_MACH_CODE54) & 0x1020) && (VarAdrToInt(MOTOR_STATE_STATE) ==0) && (getServoIsOnline()!=0))
	{
		if(bServoCanOperate == FALSE)
		{
			bServoCanOperate=TRUE;
		}
	}
	else
	{
		if(bServoCanOperate == TRUE)
		{
			bServoCanOperate=FALSE;
		}
	}

	if(g_opState)
	{
		UI32 cyctime= getBgCYCTime();
		if((cyctime-nCounter)>1000)
		{
			nCounter=cyctime;
			nSecond++;
			if (nSecond>20)
			{
				int i;
	
				g_opState = 0;

				ShowMsg(SERVO_OPFAIL);
				for(i=1; i < 8;i++)
				{
					dbsvox[i].fCover = 0;
				}
				return FALSE;
			}
		}
	}
	if((g_opState&OP_BACKUP2UDISK) && (VarAdrToInt(SETUP_SERVO_EXPORT) == FALSE))
	{
		char spath[64];
		
		g_opState &= ~OP_BACKUP2UDISK;
        snprintf(spath,sizeof(spath)-1,ESTDATA_PATH"/%s/servo.svQt",VarAdrToStr(SYSTEM_ADDR));//USB_PATH
		SaveServoFile(spath);
		ShowMsg(VW_MSG_SAVESUCCESS);
	}
	if((g_opState&OP_BACKUPFROMUDISK) && (VarAdrToInt(SETUP_SERVO_IMPORT) == FALSE))
	{
		
		g_opState &= ~OP_BACKUPFROMUDISK;
		if(bReadPanel)
		{
			VarAdrSetInt(REBOOT_PANEL,TRUE);
			g_question.pOkfunc = PanelMainRestart;//RebootOK;
			g_question.pCancelfunc = NULL;
			ShowQuestion(VW_MSG_RESTART,g_question);
		}
		else
		{
			ShowMsg(VW_MSG_UDISKREADSUCCESS);
		}
	}
	if((g_opState&OP_BACKUPFROMDEFAULT) && (VarAdrToInt(SETUP_SERVO_READDEF) == FALSE))
	{
		g_opState &= ~OP_BACKUPFROMDEFAULT;
		if(bReadPanel)
		{
			VarAdrSetInt(REBOOT_PANEL,TRUE);
			g_question.pOkfunc = PanelMainRestart;//RebootOK;
			g_question.pCancelfunc = NULL;
			ShowQuestion(VW_MSG_RESTART,g_question);
		}
	}
	if((g_opState&OP_BACKUP2DEFAULT) && (VarAdrToInt(SETUP_SERVO_SAVEDEF) == FALSE))
	{
		g_opState &= ~OP_BACKUP2DEFAULT;
		SaveServoFile(PANEL_SERVO_PATH);
		ShowMsg(VW_MSG_SAVESUCCESS);
	}
	return TRUE;
}

/************************************************************************/
/* "save 按钮"（保存）事件   出厂数据保存                        */
/************************************************************************/
BOOL DataOpSave()
{
	char scmd[128];
    snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s %s", VARDB_WORK_FILE,PANEL_FACTORY_FILE);
	osystem(scmd);
	SaveAllMachcfg();
    SavePumpCfg();
	SaveDAData();
    //20200312
    snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s %s", PANEL_MACHCFG,PANEL_FACTORY_MACHCFG);
    osystem(scmd);
    snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s %s", PANEL_DACFG,PANEL_FACTORY_DACFG);
    osystem(scmd);
    snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s %s", PUMPCFG_FILE,PUMPCFG_FACTORY_FILE);
    osystem(scmd);
    snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sopentabcal.dat %sopentabcalbak.dat",SAVE_PATH,SAVE_PATH);//20201111
    osystem(scmd);
	if(bServoCanOperate) 
	{
		int id = getServoIsOnline();
		if(getServoIsOnline()!=0)
		{
			VarAdrSetInt(SETUP_SERVO_SAVEDEF,TRUE);
			g_opState |= OP_BACKUP2DEFAULT;
			getServoAllData(id);
		}
	}
	return TRUE;
}

/**
* @brief     :数据操作画面处的导出数据到U盘
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void ExportToUdisk(int select)
{
	char scmd[128], fname[128], cpath[128];
	//char estpath[64];
	char * pstr = NULL;
	g_curSel = select;
	if(check_usb()){  //!FileExist(USB_PATH)
		ShowMsg(VW_MSG_UDISKNOTEXIST);
		return;
	}
    pstr = VarAdrToStr(SYSTEM_ADDR);
	if((pstr == NULL) || (pstr[0] == ' ') || (strcmp(pstr,"") == 0 ))
	{
		ShowMsg(VM_MSG_PLEASEENTERNAME);
		return;
	}
	if(!FileExist(ESTDATA_PATH))
	{
		//snprintf(estpath,sizeof(estpath)-1,"md %s",ESTDATA_PATH);
		MkDir(ESTDATA_PATH);
		//osystem(estpath);
	}
//20220426
//#ifdef CAPACITIVESCREEN
    if(IsG15Type() || IsTouchType())//20220104 dyl
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
                    return;
                }
            }
        }

        pstr = ConvToGBK_Dup(pstr);

        snprintf(cpath,sizeof(cpath)-1,"%s/%s",ESTDATA_PATH,pstr);

        OBJMemFree(pstr);
    }
    else
//#endif
    {
        snprintf(cpath,sizeof(cpath)-1,"%s/%s",ESTDATA_PATH,pstr);
    }

	if(!FileExist(cpath))
	{
		//snprintf(scmd, sizeof(scmd)-1,"md %s", cpath);
		MkDir(cpath);
		//osystem(scmd);
	}

	if( select&TYPEHOST )
	{
		SaveAllMachcfg();
        SavePumpCfg();
		SaveDAData();
		snprintf(fname,sizeof(fname)-1, "%s/macfg.xcf", cpath);
		if(FileExist(fname))
		{
			OverwriteType = 1;
		}
		snprintf(fname, sizeof(fname)-1,"%s/dacfg.xda", cpath);
		if(FileExist(fname))
		{
			OverwriteType = 1;
		}
        snprintf(fname, sizeof(fname)-1,"%s/pumpcfg.xcf", cpath);
        if(FileExist(fname))
        {
            OverwriteType = 1;
        }
        if(OverwriteType == 0)
		{
            PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_HOST);
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %smachcfg.dat %s/macfg.xcf",SAVE_PATH, cpath);
			if(osystem(scmd) != 0)
				return;
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %spumpcfg.dat %s/pumpcfg.xcf",SAVE_PATH, cpath);
            if(osystem(scmd) != 0)
                return;
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sdacfg.dat %s/dacfg.xda",SAVE_PATH, cpath);
			if(osystem(scmd) != 0)
				return;
		}
	}
	if( select&TYPEHMI )
	{
		snprintf(fname,sizeof(fname)-1, "%s/est_cur.tag", cpath);//20190702dbhms.xwrQt

		if(FileExist(fname))
		{
			OverwriteType = 1;
		}
		if(OverwriteType == 0)
		{
            PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_PANEL);
			if(FileExist(SAVE_PATH"pushtab.dat"))
			{
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %spushtab.dat %s/pushtab.xps",SAVE_PATH, cpath);
				if(osystem(scmd) != 0)
					return;
			}

            //20201111
            if(FileExist(SAVE_PATH"opentabcal.dat"))
            {
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sopentabcal.dat %s/opentabcal.dat",SAVE_PATH, cpath);
                if(osystem(scmd) != 0)
                    return;
            }

			//新平台改动
			if(FileExist(DATA_PATH"est_cur.tag"))
			{
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sest_cur.tag %s/est_cur.tag",DATA_PATH, cpath);
				if(osystem(scmd) != 0)
					return;
			}
			
		}
	}
	if( select&TYPEMOLD )
	{
		if(FileExist(SAVE_PATH"moldsetx.dat"))
		{
			snprintf(fname,sizeof(fname)-1, "%s/mold.xmdQt", cpath);

			if(FileExist(fname))
			{
				OverwriteType = 1;
			}
			if(OverwriteType == 0){
                PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_MOLDSET);
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %smoldsetx.dat %s/mold.xmdQt",SAVE_PATH, cpath);
				if(osystem(scmd) != 0)
					return;
			}
		}

        //20200727.cyx AI记忆开模算法数据 可跟随模具参数导入导出
        if(FileExist(SAVE_PATH"opentabcalx.dat"))
        {
            snprintf(fname,sizeof(fname)-1, "%s/opentabcalx.dat", cpath);

            if(FileExist(fname))
            {
                OverwriteType = 1;
            }
            if(OverwriteType == 0){
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sopentabcalx.dat %s/opentabcalx.dat",SAVE_PATH, cpath);
                if(osystem(scmd) != 0)
                    return;
            }
        }
	}
	if(( select&TYPESERVOR  )&&bServoCanOperate){
		snprintf(fname,sizeof(fname)-1, "%s/servo.svQt", cpath);

		if(FileExist(fname))
		{
			OverwriteType = 1;
		}
		bServoOperateDelay = true;
	}
    if( select&TYPEMACHSET )//20201210
    {
        if(FileExist(SAVE_PATH"MachValue.csv"))
        {
            LoadDataValueExcel(SAVE_PATH"MachValue.csv", FALSE);
            SaveDataValueExcel();

            snprintf(fname,sizeof(fname)-1, "%s/MachValue.csv", cpath);

            if(FileExist(fname))
            {
                OverwriteType = 1;
            }
            if(OverwriteType == 0){
                PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_MACHSET);
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sMachValue.csv %s/MachValue.csv",SAVE_PATH, cpath);
                if(osystem(scmd) != 0)
                    return;
            }
        }
    }
    if(OverwriteType == 1){
		OverwriteType = 0;
		g_question.pOkfunc = Question_OK_ExportToUdisk;
		g_question.pCancelfunc = NULL;
		ShowQuestion(VM_MSG_DATAISALREADY,g_question);
	}
	else{
		if(bServoOperateDelay == false)
		{
			ShowMsg(VW_MSG_SAVESUCCESS);
		}
		else
		{
			int id = getServoIsOnline();
			bServoOperateDelay = false;
			if(id == 0) 
			{
				VarAdrSetInt(SETUP_SERVO_EXPORT,FALSE);
				ShowMsg(VW_MSG_SAVESUCCESS);
                PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_SERVO);
			}
			else
			{
				VarAdrSetInt(SETUP_SERVO_EXPORT,TRUE);
				
				g_opState |= OP_BACKUP2UDISK;
				resetTimer();
				getServoAllData(id);

                //oprintf("id=%d\r\n",id);
			}
		}
	}
}

/**
* @brief     :数据操作画面的从U盘导入数据
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void ImportFromUdisk(int select,const char *pstr)
{
	char cpath[64];
	char scmd[128], fname[128];
	if(pstr == NULL)
		return;
	//char * pstr = NULL;
	g_curSel = select;
	bReadPanel = false;

    //pstr = VarAdrToStr(SYSTEM_ADDR);//待修改，应当从dropbox中获得当前文本
	snprintf(cpath,sizeof(cpath)-1,"%s/%s",ESTDATA_PATH,pstr);

	//oprintf("cpath=%s\r\n",cpath);

	if(select&TYPEHOST )
	{
        PanelRecAdd(data_operation_pg.button43, PNL_INPUT, PNL_HOST);
        snprintf(fname,sizeof(fname)-1, "%s/pumpcfg.xcf", cpath);
        if(FileExist(fname))
        {
            snprintf(scmd, sizeof(scmd)-1,CMD_COPY" %s/pumpcfg.xcf %spumpcfg.dat", cpath,SAVE_PATH);
            osystem(scmd);
            LoadPumpCfg();
        }
        snprintf(fname,sizeof(fname)-1, "%s/macfg.xcf", cpath);
		if(FileExist(fname))
		{
            snprintf(scmd, sizeof(scmd)-1,CMD_COPY" %s/macfg.xcf %smachcfg.dat", cpath,SAVE_PATH);
            osystem(scmd);
			LoadAllMachcfg();
			//SendComm_Config();
			CodeSendSave();//发送code码给下位机，然后发送保存命令
		}


		snprintf(fname,sizeof(fname)-1, "%s/dacfg.xda", cpath);
		if(FileExist(fname))
		{
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s/dacfg.xda %sdacfg.dat", cpath,SAVE_PATH);
			osystem(scmd);
			LoadDACfgData();
		}
	}

	if(select&TYPEMOLD)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_INPUT, PNL_MOLDSET);
		snprintf(fname, sizeof(fname)-1,"%s/mold.xmdQt", cpath);
		if(FileExist(fname)) //文件存在
		{
            if(CheckMoldFile(fname, TRUE))
            {
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s/mold.xmdQt %smoldsetx.dat", cpath,SAVE_PATH);
                osystem(scmd);
            }
            else
            {
                ShowMsg(VM_MSG_FILEFORMATERR);
                return;
            }
		}

        //20200727.cyx AI记忆开模算法数据 可跟随模具参数导入导出
        snprintf(fname, sizeof(fname)-1,"%s/opentabcalx.dat", cpath);
        if(FileExist(fname)) //文件存在
        {
            if(CheckOpenCalFile(fname, TRUE))
            {
                snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s/opentabcalx.dat %sopentabcalx.dat", cpath,SAVE_PATH);
                osystem(scmd);
            }
            else
            {
                ShowMsg(VM_MSG_FILEFORMATERR);
                return;
            }
        }
	}

	if((select&TYPESERVOR) && bServoCanOperate)
	{
		int i;
		int j;
        PanelRecAdd(data_operation_pg.button43, PNL_INPUT, PNL_SERVO);
		snprintf(fname,sizeof(fname)-1, "%s/servo.svQt", cpath);
		if(FileExist(fname)) //文件存在
		{
			if(LoadServoFile(fname))
			{
				for(i=1;i < 8;i++)
				{
					if(dbsvox[i].fCover == 1&&(VarAdrToInt(SERVO_STATE_CANST1+i-1)&0x80)&&(((VarAdrToInt(SERVO_STATE_CANST1+i-1)&0x07) == 3)||((VarAdrToInt(SERVO_STATE_CANST1+i-1)&0x07) == 0)))
					{
						CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[ID_A302],0);
						CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[ID_A303],0);
						for( j=0;j<SERVOXADDR_SAVECOUNT;j++)
						{
							CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[j],dbsvox[i].wData[j]);
						}
						bServoOperateDelay = true;
						g_opState |= OP_BACKUPFROMUDISK;
						VarAdrSetInt(SETUP_SERVO_IMPORT,TRUE);
						resetTimer();
						break;
					}
				}
			}
		}
	}
	if((select&TYPEHMI))  //面板数据
	{
        PanelRecAdd(data_operation_pg.button43, PNL_INPUT, PNL_PANEL);
		snprintf(fname, sizeof(fname)-1,"%s/pushtab.xps", cpath);
		if(FileExist(fname))
		{
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s/pushtab.xps %spushtab.dat", cpath,SAVE_PATH);
			osystem(scmd);
			InitPushTable();
		}
        //20201111
        snprintf(fname, sizeof(fname)-1,"%s/opentabcal.dat", cpath);
        if(FileExist(fname))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s/opentabcal.dat %sopentabcal.dat", cpath,SAVE_PATH);
            osystem(scmd);
        }
		//新平台改动
		snprintf(fname,sizeof(fname)-1, "%s/est_cur.tag", cpath);
		if(FileExist(fname))
		{
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s/est_cur.tag %sest_cur.tag", cpath,DATA_PATH);
			osystem(scmd);
			
			//VarDBInitLoad();
		}
		bReadPanel = true;
	}

    if(select&TYPEMACHSET)//20201210
    {
        PanelRecAdd(data_operation_pg.button43, PNL_INPUT, PNL_MACHSET);
        snprintf(fname, sizeof(fname)-1,"%s/MachValue.csv", cpath);
        if(FileExist(fname)) //文件存在
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %s/MachValue.csv %sMachValue.csv", cpath,SAVE_PATH);
            osystem(scmd);

            //20230518 chj 此处有个bug，当面板数据和结构参数一起导入的时候，如果est_cur.tag文件大小不同，
            //即中间加过参数，再结合结构参数内的dic，可能会导致数据错乱，所以改为如果导入过数据，开机读取结构参数
            char scmdMach[128];
            snprintf(scmdMach,sizeof(scmdMach)-1, "touch %s",PANEL_MACHVALUE_FILE);
            osystem(scmdMach);
            //LoadDataValueExcel(MACHSET_PATH, TRUE);
        }

        bReadPanel = true;//读取数据不下发，需重启
    }

	if(bReadPanel)
	{
		if(bServoOperateDelay == false)
		{
			VarAdrSetInt(REBOOT_PANEL,TRUE);
			g_question.pOkfunc = PanelMainRestart;//RebootOK;
			g_question.pCancelfunc = NULL;
			ShowQuestion(VW_MSG_RESTART,g_question);
		}
		else
		{
			bServoOperateDelay = false;
		}
	}
	else{
		if(bServoOperateDelay == false)
		{
			ShowMsg(VW_MSG_UDISKREADSUCCESS);
		}
		else
		{
			bServoOperateDelay = false;
		}
	}
}

/**
* @brief     :绑定ok为恢复到出厂值
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void RestoreToSave(int select)
{
	g_curSel = select;
	if(select==0)
	{
		ShowMsg(VM_MSG_SELECTTYPE);
		return;
	}
	g_question.pOkfunc = Question_OK_RestoreToSave;
	g_question.pCancelfunc = NULL;
	ShowQuestion(VM_MSG_RESTORETOSAVE,g_question);
}

/**
* @brief     :绑定ok到恢复到默认值
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void RestoreToDefault(int select)
{
	g_curSel = select;
	if(((select&(TYPEHOST|TYPEHMI|TYPEMOLD))==0))
	{
		ShowMsg(VM_MSG_SELECTTYPE);
		return;
	}
	g_question.pOkfunc = Question_OK_RestoreToDefault;
	g_question.pCancelfunc = NULL;
	ShowQuestion(VM_MSG_RESTORETODEF,g_question);
}

/**
* @brief     :根据所选类型导出对应数据到U盘
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void Question_OK_ExportToUdisk()
{
	char scmd[128],  cpath[128];
    char * pstr = VarAdrToStr(SYSTEM_ADDR);
	snprintf(cpath,sizeof(cpath)-1,"%s/%s",ESTDATA_PATH,pstr);
	if( g_curSel&TYPEHOST)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_HOST);
        snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %smachcfg.dat %s/macfg.xcf",SAVE_PATH, cpath);
		if(osystem(scmd) != 0)
			return;
        snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sdacfg.dat %s/dacfg.xda",SAVE_PATH, cpath);
		if(osystem(scmd) != 0)
			return;
        snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %spumpcfg.dat %s/pumpcfg.xcf",SAVE_PATH, cpath);
        if(osystem(scmd) != 0)
            return;
    }

	if(g_curSel&TYPEHMI)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_PANEL);
		if(FileExist(SAVE_PATH"pushtab.dat"))
		{
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %spushtab.dat %s/pushtab.xps",SAVE_PATH, cpath);
			if(osystem(scmd) != 0)
				return;
		}

        //20201111
        if(FileExist(SAVE_PATH"opentabcal.dat"))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sopentabcal.dat %s/opentabcal.dat",SAVE_PATH, cpath);
            if(osystem(scmd) != 0)
                return;
        }

		//新平台改动
		if(FileExist(DATA_PATH"est_cur.tag"))
		{
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sest_cur.tag %s/est_cur.tag",DATA_PATH, cpath);
			if(osystem(scmd) != 0)
				return;
		}
	}
	if(g_curSel&TYPEMOLD)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_MOLDSET);
		if(FileExist(SAVE_PATH"moldsetx.dat"))
		{
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %smoldsetx.dat %s/mold.xmdQt",SAVE_PATH, cpath);
			if(osystem(scmd) != 0)
				return;
		}

        //20200727.cyx AI记忆开模算法数据 可跟随模具参数导入导出
        if(FileExist(SAVE_PATH"opentabcalx.dat"))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sopentabcalx.dat %s/opentabcalx.dat",SAVE_PATH, cpath);
            if(osystem(scmd) != 0)
                return;
        }
	}
	if((g_curSel&TYPESERVOR)&&bServoCanOperate){
		bServoOperateDelay = true;
	}

    if(g_curSel&TYPEMACHSET)//20201210
    {
        PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_MACHSET);
        if(FileExist(SAVE_PATH"MachValue.csv"))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sMachValue.csv %s/MachValue.csv",SAVE_PATH, cpath);
            if(osystem(scmd) != 0)
                return;
        }
    }

	if(bServoOperateDelay == false)
	{
		ShowMsg(VW_MSG_SAVESUCCESS);
	}
	else
	{
		int id = getServoIsOnline();
		bServoOperateDelay = false;
		if(id == 0) 
		{
			VarAdrSetInt(SETUP_SERVO_EXPORT,FALSE);
			ShowMsg(VW_MSG_SAVESUCCESS);
            PanelRecAdd(data_operation_pg.button43, PNL_EXPORT, PNL_SERVO);
		}
		else
		{
			VarAdrSetInt(SETUP_SERVO_EXPORT,TRUE);
			
			g_opState |= OP_BACKUP2UDISK;
			resetTimer();
			getServoAllData(id);
		}
	}
}

/************************************************************************/
/* 出厂数据                                                    */
/************************************************************************/
void Question_OK_RestoreToSave()
{
	char scmd[128];
	if(g_curSel&TYPEMOLD)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_RESTORETODELIVERY, PNL_MOLDSET);
		if(FileExist(MOLD_FILE))
		{
            snprintf(scmd,sizeof(scmd)-1,CMD_DEL" %s",MOLD_FILE);
			osystem(scmd);
		}

        //20200727.cyx AI记忆开模算法数据 可跟随模具参数导入导出
        if(FileExist(SAVE_PATH"opentabcalx.dat"))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_DEL" %sopentabcalx.dat",SAVE_PATH);
            osystem(scmd);
        }
	}

	if(g_curSel&TYPEHMI)//面板数据
	{
        PanelRecAdd(data_operation_pg.button43, PNL_RESTORETODELIVERY, PNL_PANEL);
        if(FileExist(PANEL_FACTORY_FILE))
		{
            snprintf(scmd,sizeof(scmd)-1,CMD_COPY" %s %s", PANEL_FACTORY_FILE, VARDB_WORK_FILE);
			osystem(scmd);
			SavePanelData();
			//VarDBInitLoad();
		}
		else
		{
			//ResetDB();
            snprintf(scmd,sizeof(scmd)-1,CMD_COPY" %s %s", VARDB_INIT_FILE, VARDB_WORK_FILE);
			osystem(scmd);
			SavePanelData();
		}

        //20201111
        if(FileExist(SAVE_PATH"opentabcalbak.dat"))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_COPY" %sopentabcalbak.dat %sopentabcal.dat",SAVE_PATH, SAVE_PATH);
            osystem(scmd);
        }

		bReadPanel =true;
	}
	if(g_curSel&TYPEHOST)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_RESTORETODELIVERY, PNL_HOST);
        //20200312
        if(FileExist(PANEL_FACTORY_MACHCFG))
		{
            snprintf(scmd,sizeof(scmd)-1,CMD_COPY" %s %s", PANEL_FACTORY_MACHCFG, PANEL_MACHCFG);
            osystem(scmd);
			LoadAllMachcfg();
			CodeSendSave();//发送code码给下位机，然后发送保存命令
		}
		else
		{
			MachCfgReset();//?
		}

        if(FileExist(PANEL_FACTORY_DACFG))
		{
            snprintf(scmd,sizeof(scmd)-1,CMD_COPY" %s %s", PANEL_FACTORY_DACFG, PANEL_DACFG);
            osystem(scmd);
			LoadDACfgData();
		}

        if(FileExist(PUMPCFG_FACTORY_FILE))
        {
            snprintf(scmd,sizeof(scmd)-1,CMD_COPY" %s %s", PUMPCFG_FACTORY_FILE, PUMPCFG_FILE);
            osystem(scmd);
            LoadPumpCfg();
        }
	}
	if((g_curSel&TYPESERVOR)&&bServoCanOperate)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_RESTORETODELIVERY, PNL_SERVO);
		bServoOperateDelay = true;
	}

	if(bServoOperateDelay == false)
	{
		if(bReadPanel)
		{
			VarAdrSetInt(REBOOT_PANEL,TRUE);
			g_question.pOkfunc = PanelMainRestart;//RebootOK;
			g_question.pCancelfunc = NULL;
			ShowQuestion(VW_MSG_RESTART,g_question);
		}
	}
	else
	{
		bServoOperateDelay = false;
		if(LoadServoFile(PANEL_SERVO_PATH))
		{
			int i;
			int j;
			for(i=1; i<8; i++)
			{
				if((dbsvox[i].fCover == 1) && ((VarAdrToInt(SERVO_STATE_CANST1+i-1)&0x80) ) && (((VarAdrToInt(SERVO_STATE_CANST1+i-1)&0x07) == 3)||((VarAdrToInt(SERVO_STATE_CANST1+i-1)&0x07) == 0)))
				{
					CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[ID_A302],0);
					CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[ID_A303],0);
					for(j=0;j<SERVOXADDR_SAVECOUNT;j++)
					{
						CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[j],dbsvox[i].wData[j]);
					}
					g_opState |= OP_BACKUPFROMDEFAULT;
					VarAdrSetInt(SETUP_SERVO_READDEF,TRUE);
					resetTimer();
					break;
				}
			}
		}
	}
}

/**
* @brief     :恢复到默认值
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void Question_OK_RestoreToDefault()
{
	char scmd[128];
	BOOL breboot = FALSE;
	if(g_curSel&TYPEMOLD)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_RESTORETODEFAULT, PNL_MOLDSET);
		if(FileExist(MOLD_FILE))
		{
            snprintf(scmd,sizeof(scmd)-1,CMD_DEL" %s",MOLD_FILE);
			osystem(scmd);

		}

        //20200727.cyx AI记忆开模算法数据 可跟随模具参数导入导出
        if(FileExist(SAVE_PATH"opentabcalx.dat"))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_DEL" %sopentabcalx.dat",SAVE_PATH);
            osystem(scmd);
        }
	}
	if(g_curSel&TYPEHMI)//面板数据
	{
        PanelRecAdd(data_operation_pg.button43, PNL_RESTORETODEFAULT, PNL_PANEL);
		//m_pdatadrv->ResetDB();
		if(FileExist(VARDB_INIT_FILE))
		{
            //snprintf(scmd,sizeof(scmd)-1,CMD_COPY" %s %s", VARDB_INIT_FILE, VARDB_WORK_FILE);
            snprintf(scmd,sizeof(scmd)-1,CMD_DEL" %s", VARDB_WORK_FILE);
			osystem(scmd);
			VarDBCreateWorkDB(NULL);
			SavePanelData();
		}
		else
		{
            snprintf(scmd,sizeof(scmd)-1,CMD_DEL" %s",VARDB_WORK_FILE);
			osystem(scmd);
		}

        //20201111
        if(FileExist(SAVE_PATH"opentabcal.dat"))
        {
            snprintf(scmd,sizeof(scmd)-1, CMD_DEL" %sopentabcal.dat",SAVE_PATH);
            if(osystem(scmd) != 0)
                return;
        }

		breboot = TRUE;
	}

	if(g_curSel&TYPEHOST)
	{
        PanelRecAdd(data_operation_pg.button43, PNL_RESTORETODEFAULT, PNL_HOST);
		MachCfgReset();
		breboot = TRUE;
	}

	if(breboot)
	{
		VarAdrSetInt(REBOOT_PANEL,TRUE);
		g_question.pOkfunc = PanelMainRestart;//RebootOK;
		g_question.pCancelfunc = NULL;
		ShowQuestion(VW_MSG_RESTART,g_question);
		breboot = FALSE;
	}
}

/**
* @brief     :用来检测主机、面板、模组、伺服这几类数据是否存在
* @param     :操作方式，文件名
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
int DataFileExist(OP_MODE opmode,const char* ptr)
{
	char  cpath[64];
	char fname[128];
	//char *pstr=NULL;
	int FileIsExist;
	if(check_usb() || ptr==NULL)  //!FileExist(USB_PATH)
	{
		FileIsExist = 0x100;//U盘不存在
		return FileIsExist;
	}
	else
	{
		FileIsExist = 0;
	}

    //pstr = VarAdrToStr(SYSTEM_ADDR);//待修改，应当从dropbox中获得当前文本
	snprintf(cpath,sizeof(cpath)-1,"%s/%s",ESTDATA_PATH,ptr);

	if(opmode == EXPORT)
	{
		FileIsExist = 0x0F;
	}
	else if(opmode == IMPORT)
	{
		snprintf(fname,sizeof(fname)-1,"%s/macfg.xcf",cpath);
		if(FileExist(fname))
		{
			FileIsExist = FileIsExist | 0x02;
		}
		else
		{
			FileIsExist = FileIsExist & 0xFC;
		}

		snprintf(fname,sizeof(fname)-1, "%s/est_cur.tag", cpath);
		if(FileExist(fname))
		{
			FileIsExist = FileIsExist | 0x04;
		}
		else
		{
			FileIsExist = FileIsExist & 0xFA;
		}

		snprintf(fname,sizeof(fname)-1, "%s/mold.xmdQt", cpath);
		if(FileExist(fname))
		{
			FileIsExist = FileIsExist | 0x08;
		}
		else
		{
			FileIsExist = FileIsExist & 0xF6;
		}

		snprintf(fname,sizeof(fname)-1, "%s/servo.svQt", cpath);
		if(FileExist(fname))
		{
			FileIsExist = FileIsExist | 0x10;
		}
		else
		{
			FileIsExist = FileIsExist & 0xEF;
		}

        snprintf(fname,sizeof(fname)-1, "%s/MachValue.csv", cpath);//20201210
        if(FileExist(fname))
        {
            FileIsExist = FileIsExist | 0x20;
        }
        else
        {
            FileIsExist = FileIsExist & 0xDF;
        }

        if(FileIsExist== 0x003e)//20201210
			FileIsExist = FileIsExist | 0x01;
	}
	return FileIsExist;
}

/*********Pump数据保存********************************************************************/
#define MAX_SAVE_PUMPNUM    8 //最大保存泵浦部件数

//写部件号长度
void WriteFilePartHead(FILEHD file, UI16 part_sub_id, UI16 size)
{
    UI32 dwInfo = (part_sub_id<<16) + size;

    FileWrite(file, &dwInfo, sizeof(dwInfo), NULL);
}

//读部件号长度
UI32 LoadFilePartHead(FILEHD file)
{
    UI32 dwInfo;

    FileRead(file, &dwInfo, sizeof(dwInfo), NULL);
    return dwInfo;
}

/**
* @brief     :8个泵浦数据保存
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void SavePumpCfg()
{
    UI16 i;
    UI16 wpos=0;
    UI16 part_sub_id;
    UI16 size;
    int writebytes;
    FILEHD filehd;

    if(!(PanelCfgStyle() == G_TYPE || PanelCfgStyle() == P_TYPE))
        return;
    if(VarAdrToInt(SERVO_CURRENT_NUM) == 0)
        return;

    if(FileExist(PUMPCFG_FILE))//20210120 保存之前先删除原有文件
    {
        FileDelete(PUMPCFG_FILE);
    }

    filehd = FileCreate(PUMPCFG_FILE);
    if(FILEHD_IS_OK(filehd))
    {
        for(i = 1; i <= MAX_SAVE_PUMPNUM; i++)
        {
            part_sub_id = ((((UI8)(PUMP_ID))<<8)|(i<<4) | (UI8)(MACHSET));
            size = GetSubLenByID(part_sub_id);
            FileSeek(filehd, wpos);
            WriteFilePartHead(filehd, part_sub_id, size);//20210120
            wpos += sizeof(UI16)*2;
            FileSeek(filehd, wpos);
            FileWrite(filehd, GetSubByID(part_sub_id, NULL), sizeof(UI16)*size, &writebytes);
            wpos += sizeof(UI16)*size;
        }
    }
    FileClose(filehd);
}

/**
* @brief     :从文件加载泵浦数据
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void LoadPumpCfg()
{
    UI16 i;
    UI16 wpos=0;
    UI16 part_id, part_sub_id;
    UI16 size, data[1];
    UI32 dwInfo;
    UI16 save_size;//20210120 文件保存的部件长度
    int readbytes;
    FILEHD filehd;

    if(!(PanelCfgStyle() == G_TYPE || PanelCfgStyle() == P_TYPE))
        return;
    if(VarAdrToInt(SERVO_CURRENT_NUM) == 0)
        return;

    filehd = FileOpen(PUMPCFG_FILE);
    if(FILEHD_IS_OK(filehd))
    {
        for(i = 1; i <= MAX_SAVE_PUMPNUM; i++)
        {
            FileSeek(filehd, wpos);//20210517
            dwInfo = LoadFilePartHead(filehd);//20210120
            wpos += sizeof(UI16)*2;//20210517
            save_size = dwInfo&0xFFFF;

            part_sub_id = ((((UI8)(PUMP_ID))<<8)|(i<<4) | (UI8)(MACHSET));
            size = GetSubLenByID(part_sub_id);
            if(size > save_size)//20210120 保存时变量数与现有变量数取最小值
            {
                size = save_size;
            }
            FileSeek(filehd, wpos);
            FileRead(filehd, GetSubByID(part_sub_id, NULL), sizeof(UI16)*size, &readbytes);
            wpos += sizeof(UI16)*save_size;//20210120 按照文件保存变量数移位

            part_id = ((((UI8)(PUMP_ID))<<8)|(i<<4));
            ProtSendPartSub(part_id,MACHSET,size,GetSubByID(part_sub_id, NULL));
            data[0] = part_sub_id;
            ProtSysCtrl(COMM_SUBCMD_SERVOPUMP_WRITE,1,data);//save cmd
        }
    }
    FileClose(filehd);
}

/**
* @brief     :泵浦画面保存
* @param     :start: 从第几个开始保存 end: 到第几个结束
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200701
*/
void SavePumpPage(int start, int end)
{
    UI16 i;
    UI16 wpos=0;
    UI16 part_sub_id;
    UI16 size;
    int writebytes;
    FILEHD filehd;

    if(!(PanelCfgStyle() == G_TYPE || PanelCfgStyle() == P_TYPE))
        return;
    if(VarAdrToInt(SERVO_CURRENT_NUM) == 0)
        return;

    filehd = FileOpen(PUMPCFG_FILE);
    if(!FILEHD_IS_OK(filehd))
    {
        filehd = FileCreate(PUMPCFG_FILE);
    }
    if(FILEHD_IS_OK(filehd))
    {
        if(end > MAX_SAVE_PUMPNUM)//20210120
        {
            end = MAX_SAVE_PUMPNUM;
        }

        for(i = 1; i <= end; i++)
        {
            part_sub_id = ((((UI8)(PUMP_ID))<<8)|(i<<4) | (UI8)(MACHSET));
            size = GetSubLenByID(part_sub_id);
            if(i < start)
            {
                wpos += sizeof(UI16)*2;
                wpos += sizeof(UI16)*size;
            }
            else
            {
                FileSeek(filehd, wpos);
                WriteFilePartHead(filehd, part_sub_id, size);//20210120
                wpos += sizeof(UI16)*2;
                FileSeek(filehd, wpos);
                FileWrite(filehd, GetSubByID(part_sub_id, NULL), sizeof(UI16)*size, &writebytes);
                wpos += sizeof(UI16)*size;
            }
        }
    }
    FileClose(filehd);
}
/*********Pump数据保存********************************************************************/


#define MAX_EXCEL_COLUMN 4      //结构参数表单行读取数据个数
#define MAX_EXCEL_ROW    500    //结构参数表最大行数
PMACHSETDATA MACHSET_DATA = NULL;//结构参数表数据储存指针
UI16 MACHSET_DATA_NUM = 0;//结构参数表行数
static char machsetdata_head[1024]; //保存结构参数表第一行信息

/**
 * @brief      : 清除保存的结构参数表
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201210
 */
void ClearMachsetData()
{
    MACHSETDATA* iter = NULL;
    MACHSETDATA* temp = NULL;

    for(iter = MACHSET_DATA; MACHSET_DATA_NUM > 0; MACHSET_DATA_NUM--)
    {
        if(iter != NULL)
        {
            temp = iter;
            iter = iter->pnext;
            OBJMemFree(temp);
            temp = NULL;
        }
    }

    MACHSET_DATA = NULL;
}

/**
 * @brief      : 导入结构参数
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201210
 */
BOOL LoadDataValueExcel(char filename[], BOOL bsend)
{
    char buffer[1024];
    char *line,*celldata;
    int row = 0;
    int column= 0;

    MACHSETDATA* new_node = NULL;
    MACHSETDATA* iter = NULL;

    FILE* stream;
    memset(buffer, 0, sizeof(buffer));

    if(!FileExist(filename))
    {
        oprintf("%s is not exist\n",filename);
        return FALSE;
    }
    else
    {
        stream = fopen(filename,"r");
        oprintf("open %s successful\n",filename);
    }

    ClearMachsetData();//先释放原先申请的内存

    while((line = fgets(buffer, sizeof(buffer), stream)) != NULL)//当没有读取到文件末尾时循环继续
    {
        celldata = strtok(buffer,",");//以逗号分隔

        if(row == 0)//第一行信息读取
        {
            char buff[255];
            snprintf(machsetdata_head,sizeof(machsetdata_head),"%s,",celldata);

            celldata = strtok(NULL, ",");
            if(celldata != NULL)
            {
                strcat(machsetdata_head, celldata);
                strcat(machsetdata_head, ",");
            }

            celldata = strtok(NULL, ",");
            if(celldata != NULL)
            {
                strcat(machsetdata_head, celldata);
                strcat(machsetdata_head, ",");
            }

            celldata = strtok(NULL, ",");
            if(celldata != NULL)
            {
                snprintf(buff,sizeof(buff),"%s",celldata);
                if(buff[strlen(buff)-1] == '\n')//去除末尾换行符
                {
                    buff[strlen(buff)-1] = '\0';
                }
                if(buff[strlen(buff)-1] == '\r')//去除末尾换行符
                {
                    buff[strlen(buff)-1] = '\0';
                }
                strcat(machsetdata_head, buff);
            }
        }

        while(celldata != NULL && row > 0)//读取每一列的数据,第一行数据不读取
        {
            if(column == 0)//NAME
            {
                new_node = OBJMemMalloc(sizeof(MACHSETDATA));
                new_node->pnext = NULL;
                snprintf(new_node->name,sizeof(new_node->name),"%s",celldata);
            }
            else if(column == 1)//ADDRESS
            {
                sscanf(celldata,"%x",&new_node->address);
            }
            else if(column == 2)//VALUE
            {
                new_node->value = StrToWord(celldata, 0);
                if(bsend)
                {
                    //20230518 chj 此处有个bug，当面板数据和结构参数一起导入的时候，如果est_cur.tag文件大小不同，
                    //即中间加过参数，再结合结构参数内的dic，可能会导致数据错乱，所以改为如果导入过数据，开机读取结构参数
                    //注：此处未做任何修改，用于提醒，数据未用VarSendSaveSetIntByAdr发送
                    VarAdrSetInt(new_node->address, new_node->value);
                    VarAdrSaveInt(new_node->address);//不发给下位机
                }
            }
            else if(column == 3)//DESCRIPTION
            {
                snprintf(new_node->desc,sizeof(new_node->desc),"%s",celldata);
                if(new_node->desc[strlen(new_node->desc)-1] == '\n')//去除末尾换行符
                {
                    new_node->desc[strlen(new_node->desc)-1] = '\0';
                }
                if(new_node->desc[strlen(new_node->desc)-1] == '\r')//去除末尾换行符
                {
                    new_node->desc[strlen(new_node->desc)-1] = '\0';
                }
            }

            if(++column >= MAX_EXCEL_COLUMN)//只需要读取到第MAX_EXCEL_COLUMN列
            {
                MACHSET_DATA_NUM++;
                if(MACHSET_DATA == NULL)
                {
                    MACHSET_DATA = new_node;
                    MACHSET_DATA->pnext = NULL;
                }
                else
                {
                    iter = MACHSET_DATA;
                    while (iter != NULL)
                    {
                        if(iter->pnext == NULL)
                        {
                            iter->pnext = new_node;
                            break;
                        }
                        iter = iter->pnext;
                    }
                }

                break;
            }
            //下一列参数值
            celldata = strtok(NULL, ",");
        }
        row++;
        column = 0;
        if(row >= MAX_EXCEL_ROW)
        {
            break;
        }
    }

    fclose(stream);

    return TRUE;
}

/**
 * @brief      : 导出结构参数
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20201210
 */
//20230518 chj 原先的fileprintf方式有问题，每次调用都会写文件
BOOL SaveDataValueExcel()
{
    TSTRING_CREATE(str_tran);

    FILEHD stream;
    int i;

    MACHSETDATA* iter = NULL;

    if(FileExist(MACHSET_PATH))
    {
       stream = FileOpen(MACHSET_PATH);
    }

    if(!FILEHD_IS_OK(stream))
    {
        TSTRING_DESTROY(str_tran);
        return FALSE;
    }

    tstring_appends_fix(&str_tran, machsetdata_head, MIN_ALLOC_SIZE*3);
    tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);

    char tmp[20];

    for(i = 0, iter = MACHSET_DATA; i<MACHSET_DATA_NUM; ++i, iter = iter->pnext)
    {
        if(iter != NULL)
        {
            tstring_appends_fix(&str_tran, iter->name, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            snprintf(tmp, sizeof(tmp)-1, "0x%X", iter->address);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            WordToStr(tmp,VarAdrToInt(iter->address),0);
            tstring_appends_fix(&str_tran, tmp, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

            tstring_appends_fix(&str_tran, iter->desc, MIN_ALLOC_SIZE);
            tstring_append_fix(&str_tran, ',', MIN_ALLOC_SIZE);

        }

        tstring_append_fix(&str_tran, '\n', MIN_ALLOC_SIZE);
    }

    oprintf("str_tran.len:%ld\n",str_tran.len);
    FileWrite(stream, str_tran.str, str_tran.len, NULL);
    TSTRING_DESTROY(str_tran);
    FileClose(stream);
    return TRUE;
}
