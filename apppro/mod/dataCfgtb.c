#include "dataCfgtb.h"
#include "language.h"
#include "panel.h"
#include "database.h"
#include "protcmd.h"
#include "monmcu.h"
#include "oprintf.h"
#include "file.h"
#include "string.h"
#include "stdio.h"
#include "common.h"
#include "typedef.h"


#define		CMOD_WR_PATH			SAVE_PATH"version.dat"		//保存的文件名

VERSION g_version;

//20181225 hz
VERSION   g_dbversionDefault = {
	828, //未使用
	0,    //未使用
	{0,0,0,0,0,0,0, 
	 0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,'\0'},//system1
	{0,0,0,0,0,0,0,0,'\0'},//machine1  //未使用
	{0,0,0,0,0,0,0,0,'\0'},//machine2  //未使用
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} //reserve
};

static BOOL CheckFileValid(FILEHD filehd)
{
	if(FILEHD_IS_OK(filehd))
	{
		return true;
	}
	else
	{
		FileClose(filehd);
		return false;
	}
}

/**
* @brief     :文件写入版本信息
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
void WriteVersion(VERSION version)
{
	FILEHD filehd;
	filehd = FileOpen(CMOD_WR_PATH);
	if(CheckFileValid(filehd))
	{
		FileSeek(filehd,0);
		FileWrite(filehd,&version,sizeof(VERSION),NULL);
		FileClose(filehd);
	}
}

/**
* @brief     :读取版本文件信息
* @param     :
* @return    :版本信息
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
VERSION	ReadVersion()
{
	FILEHD filehd;
    VERSION version;
	filehd = FileOpen(CMOD_WR_PATH);
    

	if(CheckFileValid(filehd))
	{
		FileRead(filehd,&version,sizeof(VERSION),NULL);
		FileClose(filehd);
	}
	else 
	{
		filehd = FileCreate(CMOD_WR_PATH);
		memcpy(&version,&g_dbversionDefault,sizeof(VERSION));
		FileWrite(filehd,&version,sizeof(VERSION),NULL);
		FileClose(filehd);
	}
	VarAdrSetStr(SYSTEM_ADDR,version.system1);
	return version;
}
/**
* @brief     :将文件名加上;进行重新组合，这里会申请内存，外部调用需要释放
* @param     : 转换后字符串内存所在指针
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
char * ConvertDirInfoToStr(FILEDIR * dirinfo)
{
	char* ret = NULL;
	if(dirinfo!=NULL)
	{
		int i=0;
		int dev=0;//偏移
		PFILENODE pnode=NULL;
		char*buf=NULL;
		char*pend;
		
		buf= (char*)OBJMemMalloc(FILE_NAME_LENTH*dirinfo->subcount);
		if(buf != NULL)
		{
			pend = buf + FILE_NAME_LENTH*dirinfo->subcount - 1;
			ret = buf;
			pnode = &(dirinfo->filenode);
			for(i=0;(i < dirinfo->subcount)&&(pnode != NULL);i++)
			{
				if(buf + dev + strlen(pnode->name) >= pend)
					break;
					
				sprintf(buf+dev,"%s;",pnode->name);
				buf += dev;
				dev = strlen(pnode->name)+1;
				pnode = pnode->psibling;
			}
		}
	}
	return ret;
}

