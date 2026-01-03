#include "verinfo.h"
#include "file.h"
#include "stdio.h"
#include "oprintf.h" 
#include "database.h"
#include "panelcfg.h"

VERSIONREAD m_verinfo;
OUTVERSION  m_panelcode;

static void PanelTypeFlag();

//20200326
PHASH_TBL pmap_powertype = NULL;
PHASH_TBL pmap_machtype = NULL;
PHASH_TBL pmap_usetype = NULL;

#define MONTH_NUM 12
#define MAX_CHAR_NUM 5
static UI32 KerVer[3] = {0};
char KerVerMonth[MONTH_NUM][MAX_CHAR_NUM]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};//20190708.cyx

/************************************************************************/
/* 面板配置的读取                                                       */
/************************************************************************/
BOOL PanelCfgLoad()
{
	FILEHD filehd;
	int readbytes;

	filehd = FileOpen(PANELCODE_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
		FileRead(filehd,&m_panelcode,sizeof(OUTVERSION),&readbytes);
		FileClose(filehd);
	}
    else
    {
        ODEBUG("There is NO PANELCODE_INIT_FILE(save/panelcfg.dat)!");
    }

    PanelTypeFlag();

    //20200326
    ParseCSVFile(POWERTYPE_FILE,&pmap_powertype);
    ParseCSVFile(MACHTYPE_FILE,&pmap_machtype);
    ParseCSVFile(USETYPE_FILE,&pmap_usetype);

	return TRUE;
}
/************************************************************************/
/* 面板类型7/8/10/12/15                                                 */
/************************************************************************/
UI16 PanelType()
{
    UI16 panel_size;
    panel_size = PanelCfgSize();
    return panel_size;
}

/************************************************************************/
/* 面板类型标志 type:>=10表示10寸以上面板                               */
/************************************************************************/
static void PanelTypeFlag()
{
    //面板大小从paneldef.json开始读  20190614 hz
    int panel_size;
    panel_size = PanelCfgSize();

    if(panel_size>=10)
	{
		VarAdrSetInt(PANEL_TYPE_FLAG,1);
	}
	else
	{
		VarAdrSetInt(PANEL_TYPE_FLAG,0);
	}
}

/************************************************************************/
/* 面板类型判断函数     20190424 hz                                  */
/************************************************************************/
UI16 GetPanelType()
{
    int panel_size;
    int x_size;

    panel_size = PanelCfgSize();
    x_size = PanelCfgXsize();

    if(panel_size == 15)
    {
        return PANEL_VT15;
    }

    if(panel_size == 10)
    {
        if(x_size == 600) //10寸竖屏x轴600
        {
            return PANEL_V10;
        }
        else
        {
            return PANEL_H10;
        }
    }

    if(panel_size == 8)
    {
        return PANEL_H8;
    }

    if(panel_size == 7)
    {
        return PANEL_H7;
    }
    return 0;
}

/************************************************************************/
/* 内核版本的读取                                                       */
/************************************************************************/
void KernelVersionRead()
{
    char scmd[128];
    FILEHD fd;
    int  size,i;
    char *buf = NULL;
    char year[MAX_CHAR_NUM];
    char month[MAX_CHAR_NUM];
    char day[MAX_CHAR_NUM];

    if(FileExist(KERNEL_VERSION_FILE))
    {
        FileDelete(KERNEL_VERSION_FILE);
    }

#ifndef WIN32
    snprintf(scmd,sizeof(scmd)-1, "cat /proc/version>%s", KERNEL_VERSION_FILE);
    osystem(scmd);
#endif

    fd = FileOpen(KERNEL_VERSION_FILE);
    if(FILEHD_IS_OK(fd))
    {
        size = FileGetSize(fd,KERNEL_VERSION_FILE);
        buf = (char*)OBJMemMalloc(size + 1);
        if(buf != NULL)
        {
            FileRead(fd,buf,size,NULL);
            FileClose(fd);
            buf[size] = '\0';
            for(i = 0;i < 5;i++)
            {
                while (buf[size] != ' ')
                {
                    size--;
                }
                if(i == 0)
                {
                    year[0] = buf[size+1];
                    year[1] = buf[size+2];
                    year[2] = buf[size+3];
                    year[3] = buf[size+4];
                    year[4] = '\0';
                }
                else if(i == 3)
                {
                    if(buf[size+2] != ' ')
                    {
                        day[0] = buf[size+1];
                        day[1] = buf[size+2];
                        day[2] = '\0';
                    }
                    else
                    {
                        day[0] = buf[size+1];
                        day[1] = '\0';
                    }
                }
                else if(i == 4)
                {
                    month[0] = buf[size+1];
                    month[1] = buf[size+2];
                    month[2] = buf[size+3];
                    month[3] = '\0';
                }
                size--;
            }

            KerVer[0] = StrToWord(year,0);
            for(i = 0;i < MONTH_NUM;i++)
            {
                if((month[0] == KerVerMonth[i][0]) && (month[1] == KerVerMonth[i][1]) && (month[2] == KerVerMonth[i][2]))
                {
                    KerVer[1] = i + 1;
                    break;
                }
            }
            KerVer[2] = StrToWord(day,0);

            OBJMemFree(buf);
        }
    }
}

UI32 GetKernelVersion(int index)
{
    if(index < 3)
    {
        return KerVer[index];
    }
    else
    {
        return 0;
    }
}

/************************************************************************/
/* 面板版本数据的读取                                                      */
/************************************************************************/
extern VERSIONREAD g_dbversiondefault;
BOOL PanelVerLoad()
{
    memset(&m_verinfo, 0, sizeof(VERSIONREAD));
    memcpy(&m_verinfo,&g_dbversiondefault,sizeof(VERSIONREAD));

    return TRUE;
}
