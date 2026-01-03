#include "moldset.h"
#include "file.h"
#include "oprintf.h"
#include "time.h"
#include "part.h"
#include "command.h"
#include "common.h"
#include "stdio.h"
#include "information.h"

//define easy WR const
#define CMOD_WR_HEADSZ sizeof(MOLDSET_HEAD)
#define CMOD_WR_ITEMSZ sizeof(MOLDSET_ITEM)
#define CMOD_WR_PATH   SAVE_PATH"moldsetx.dat"
#define CMOD_TEMP_PATH	SAVE_PATH"moldsetxtemp.dat"



MOLDSET_CONTEXT g_moldset;
MOLDSET_HEAD g_moldset_head;

/************************************************************************/
/* public methods                                                       */
/************************************************************************/
void MoldSet_GetCur(char* cur_name, int* cur_id)
{
	MOLDSET_ITEM item;
	if(cur_id)
	{
		*cur_id = g_moldset.cur_no+1;
	}

	if(cur_name)
	{
		if(CMOD_RITEM_EX(g_moldset.cur_no, &item))
		{
			strcpy(cur_name, item.sName);
		}
		else
		{
			cur_name[0] = '\0';
		}
	}
}

void MoldSetInital()
{
	MOLDSET_HEAD head;

	if(CMOD_RHEAD_EX(&head))
	{
		g_moldset.cur_no = head.cur_no;
	}
}

BOOL SaveMoldSet(UI16 cMoldSet)
{
    DB_MOLDSET_EX moldsetx;
	UI32 dataPos;
	int sub_id;
	PPART ppart;
	PART_ITER iter;

	memset(&moldsetx, 0, sizeof(DB_MOLDSET_EX));
      
	PartIterHead(&iter);
	 while(ppart = PartIterData(&iter))
	{
		for(sub_id = MOLDSET; sub_id <= HYDR; ++sub_id) 
		{
			if(ppart->psub[sub_id] != NULL && (ppart->pvar[sub_id]->bit_attr & VAR_IS_MOLD_BIT))
			{
				if(moldsetx.wIndCount > MAX_MOLDSUBCLASSCOUNT)
				{
                    ShowMsg(VW_MSG_DATA_OVERLOAD);//20250114 chj 数据超限,请核对!
					return FALSE;
				}

				moldsetx.wIndAddrArray[moldsetx.wIndCount] = (((ppart->id&0xFFF0)|(sub_id&0xF))<<16);
				moldsetx.wIndCntArray[moldsetx.wIndCount] = ppart->sub_len[sub_id];
				moldsetx.wIndCount++;

				if(moldsetx.wCount + ppart->sub_len[sub_id] > MAX_MOLDPARAMCOUNT)
				{
                    ShowMsg(VW_MSG_DATA_OVERLOAD);//20250114 chj 数据超限,请核对!
					return FALSE;
				}

                //ODEBUG("save 0x%04X0000 - 0x%04X%04X", ppart->id | sub_id, ppart->id | sub_id, ppart->sub_len[sub_id]);
				memcpy(	moldsetx.wDataArray+moldsetx.wCount,
						ppart->psub[sub_id],
						ppart->sub_len[sub_id]*2);
				moldsetx.wCount += ppart->sub_len[sub_id];
			}
		}
        //oprintf("mold:%d,%d\n",moldsetx.wCount,moldsetx.wIndCount);
		PartIterNext(&iter);
	}

	dataPos = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + sizeof(DB_MOLDSET_EX)*cMoldSet;
    return CModWriteRaw(dataPos, &moldsetx, sizeof(DB_MOLDSET_EX), CMOD_WR_PATH);
}

BOOL ReadMoldSet(UI16 cMoldSet)
{
	DB_MOLDSET_EX moldsetx;
    UI32 dataPos,addr,str_addr,end_addr;
    int sub_id, i, offset;
	PPART ppart;
	UI16* pdata;
    FILEHD fhd;
    PVAR pdat,str_pdat,end_pdat;
    UI16 value;

	dataPos = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + sizeof(DB_MOLDSET_EX)*cMoldSet;
	if(!CModReadRaw(&moldsetx, dataPos, sizeof(DB_MOLDSET_EX), CMOD_WR_PATH))
	{
		return FALSE;
	}
	
    fhd = OpenWorkFile();
    if(!FILEHD_IS_OK(fhd))
    {
        FileClose(fhd);
        oprintf("open work file err---\n");
        return FALSE;
    }

	pdata = moldsetx.wDataArray;
	for(i = 0; i < moldsetx.wIndCount; ++i)
	{
		moldsetx.wIndAddrArray[i] = moldsetx.wIndAddrArray[i]>>16;
		ppart = GetPartByID((UI16)moldsetx.wIndAddrArray[i]);
		sub_id = GET_PART_SUB(moldsetx.wIndAddrArray[i]);
		if(ppart)
		{
			memcpy(	ppart->psub[sub_id],
					pdata,
					CMIN(ppart->sub_len[sub_id], moldsetx.wIndCntArray[i])*2);
            //20250303 chj 卓越程序温度部件为新的0xOE部件，老温度部件0x05部件数据不发送
            if((moldsetx.wIndAddrArray[i] == 0x514) || (moldsetx.wIndAddrArray[i] == 0x524))
            {
                ;
            }
            else
            {
                PartSubTx(ppart, 1<<sub_id, FALSE);
            }

            //ODEBUG("load 0x%04X0000 - 0x%04X%04X,%d,%d", moldsetx.wIndAddrArray[i], moldsetx.wIndAddrArray[i], CMIN(ppart->sub_len[sub_id], moldsetx.wIndCntArray[i])-1,ppart->sub_len[sub_id],moldsetx.wIndCntArray[i]);

            //20210715.cyx
            {
                str_addr = moldsetx.wIndAddrArray[i]<<16;
                end_addr = moldsetx.wIndAddrArray[i]<<16 | (CMIN(ppart->sub_len[sub_id], moldsetx.wIndCntArray[i])-1);

                for(addr = str_addr; addr <= end_addr; addr++)
                {
                    if(pdat = VarAdrToVar(addr))
                    {
                        if(!VarChkAttrBit(pdat, VAR_IS_MOLD_BIT))//部件中有些没有勾选模具数据
                        {
                            offset = GetVarOffset(pdat);
                            FileSeek(fhd, offset);
                            FileRead(fhd, &value, 2, NULL);
                            VarAdrSetInt(addr, value);
                        }
                    }
                }

                str_pdat = VarAdrToVar(str_addr);
                end_pdat = VarAdrToVar(end_addr);
                if(str_pdat != NULL && end_pdat != NULL)
                {
                    SaveMoldValue(fhd, str_pdat, end_pdat);
                }
            }
        }
		pdata += moldsetx.wIndCntArray[i];
	}

    FileClose(fhd);
    //SaveAllValue();//20190529
	return TRUE;
}

//csj 2018.5.21
static void	ReadBKRAM_MoldTemp(void*item,UI32 index,UI16 size)
{
	UI32 dataPos =  CMOD_WR_ITEMSZ*index;
	CModReadRaw(item, dataPos, size, CMOD_TEMP_PATH);
}

static void	ReadBKRAM_MoldTempEX(void*item,UI32 index,UI16 size)
{
	UI32 dataPos =  MAX_MOLDSET*CMOD_WR_ITEMSZ+sizeof(DB_MOLDSET_EX)*index;
	CModReadRaw(item, dataPos, size, CMOD_TEMP_PATH);
}

BOOL ReadMoldsetInfoEX(UI16 cMoldSet,DB_MOLDSET_EX*   moldSetx)
{
	UI32 dataPos;
	if(moldSetx != NULL)
	{
		dataPos = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + sizeof(DB_MOLDSET_EX)*cMoldSet;
		if(!CModReadRaw(moldSetx, dataPos, sizeof(DB_MOLDSET_EX), CMOD_WR_PATH))
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

//读取临时模具数据 
BOOL    ReadMoldsetInfoTemp(UI16 cMoldSet,MOLDSET_ITEM* info)
{
	ReadBKRAM_MoldTemp(info,cMoldSet-1,sizeof(MOLDSET_ITEM));

	if (info->flag== MARK_USED)  return TRUE;
	else 	return FALSE;
}

BOOL        ReadMoldSetTempEx(UI16 cMoldSet,DB_MOLDSET_EX*   moldSetx)
{
	MOLDSET_ITEM  info;

	if (ReadMoldsetInfoTemp(cMoldSet, &info)){
		
		ReadBKRAM_MoldTempEX(moldSetx,cMoldSet-1,sizeof(DB_MOLDSET_EX));
		if(moldSetx->wCount > MAX_MOLDPARAMCOUNT)
        {
            ShowMsg(VW_MSG_DATA_OVERLOAD);//20250114 chj 数据超限,请核对!
			return FALSE;
        }

		if(moldSetx->wIndCount > MAX_MOLDSUBCLASSCOUNT)
        {
            ShowMsg(VW_MSG_DATA_OVERLOAD);//20250114 chj 数据超限,请核对!
			return FALSE;
        }
	}
	else{
		return FALSE;
	}

	return TRUE;

}
static void        ReadBKRAM_Mold(void* dst,UI16 cMoldSet)
{
	if(dst != NULL && cMoldSet<MAX_MOLDSET)
	{		
		CMOD_RITEM_EX(cMoldSet,dst);
	}
}

static BOOL    ReadMoldsetInfo(UI16 cMoldSet,MOLDSET_ITEM* info)
{
	info->flag=0;
	ReadBKRAM_Mold(info,cMoldSet-1);

	if (info->flag== MARK_USED)  return TRUE;
	else 	return FALSE;
}

void        WriteBKRAM_Mold(UI32 dst,void* src,UI16 wCount)
{
	CModWriteRaw(dst, src, wCount, CMOD_WR_PATH);

}

void    SaveMoldsetInfo(UI16 cMoldSet,MOLDSET_ITEM* info)
{
	UI32    dwAddr;
	dwAddr=CMOD_WR_HEADSZ+(UI16)(cMoldSet-1)*sizeof(MOLDSET_ITEM);

	WriteBKRAM_Mold(dwAddr,info,sizeof(MOLDSET_ITEM));

	
}
//保存模具数据 
BOOL        SaveMoldSetEx(UI16 cMoldSet,DB_MOLDSET_EX* moldSetx)
{
	UI32 dataPos = CMOD_WR_HEADSZ + MAX_MOLDSET*CMOD_WR_ITEMSZ + sizeof(DB_MOLDSET_EX)*(cMoldSet-1);
	return CModWriteRaw(dataPos, moldSetx, sizeof(DB_MOLDSET_EX), CMOD_WR_PATH);
}
BOOL MoldInput()
{
	//插入模组
	MOLDSET_ITEM  info;
	DB_MOLDSET_EX   moldSetx;
	int k=0;
	int i=1,j=0;
    UI16 nIdAddMoldset[MAX_MOLDSET+1]={0};
	for(i=1;i<=MAX_MOLDSET;i++){
		if(ReadMoldsetInfoTemp(i, &info)){
			nIdAddMoldset[k++]=i;
		}
	}

	i=1;
	j=0;
	while(j<k && i<=MAX_MOLDSET){
		if(!ReadMoldsetInfo(i, &info)){

			ReadMoldsetInfoTemp(nIdAddMoldset[j],&info);
			ReadMoldSetTempEx(nIdAddMoldset[j],&moldSetx);

			SaveMoldsetInfo(i,&info);
			SaveMoldSetEx(i,&moldSetx);
			j++;
		}

		i++;
	}
    osystem(CMD_DEL CMOD_TEMP_PATH);
	return true;
}

static void        WriteBKRAM_MoldTemp(UI32 dst,void* src,UI16 wCount)
{
	CModWriteRaw(dst, src, wCount, CMOD_TEMP_PATH);
}

//删除临时模具数据
static void    DeleteMoldsetTemp(UI16 cMoldSet)
{
	UI32    dwAddr;
	MOLDSET_ITEM  info;
        memset(&info,0,CMOD_WR_ITEMSZ);
	dwAddr=(UI16)(cMoldSet-1)*CMOD_WR_ITEMSZ;

	info.flag = 0;

	WriteBKRAM_MoldTemp(dwAddr,&info,CMOD_WR_ITEMSZ);

}

BOOL MoldExportTo(const char* cmd,const char * filepath,UI16 maxsize)
{
	if(cmd != NULL && filepath!=NULL)
	{
		int i=0;
		int j=0;
		UI32    dwAddr;
		MOLDSET_ITEM   info;
		DB_MOLDSET_EX   moldSetx;

		for( i=1;i<=MAX_MOLDSET;i++)
		{
			DeleteMoldsetTemp(i);
			//读取选中的模组到临时文件中
			ReadBKRAM_Mold(&info,i-1);
			if(g_moldset.sel_state[i-1]  && info.flag==MARK_USED)
			{
				dwAddr=(UI16)(j)*CMOD_WR_ITEMSZ;
				WriteBKRAM_MoldTemp(dwAddr,&info,CMOD_WR_ITEMSZ);

				if(!ReadMoldsetInfoEX(i-1,&moldSetx))
                                  return false;
                                
				dwAddr = MAX_MOLDSET*CMOD_WR_ITEMSZ + sizeof(DB_MOLDSET_EX)*j;
				WriteBKRAM_MoldTemp(dwAddr,&moldSetx,sizeof(DB_MOLDSET_EX));
				j++;
				
			}
		}
        snprintf((char*)cmd,maxsize-1,CMD_COPY" %s %s",CMOD_TEMP_PATH,filepath);
		
		return true;
	}
	
	return FALSE;
}

//校验文件长度是否符合HMI数据
BOOL CheckMoldFile(const char* filename, BOOL includehead)
{
    FILEHD filehd;
    int file_size = FileGetSize(filehd, filename);
    //oprintf("--CheckMoldFile--%s--file_size = %d\n",filename,file_size);

    if(includehead)
    {
        file_size = file_size - sizeof(MOLDSET_HEAD);
    }

    if(file_size > MAX_MOLDSET*sizeof(MOLDSET_ITEM))
    {
        int data_num = (file_size-MAX_MOLDSET*sizeof(MOLDSET_ITEM))%sizeof(DB_MOLDSET_EX);

        if(data_num == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}
