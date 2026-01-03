#include "ApiCommon.h"
#include "printf.h"
#include "MD_memtbl.h"

BOOL ApiFreeBuf(void* pbuf)
{
  M_free(pbuf);
   
   return TRUE;
}

__inline PPARA_FORMAT_HD ApiPackHd(PPARA_FORMAT_HD phd, UI16 cmd, UI16 id, UI16 ver, UI16 len)
{
	if((phd == NULL) || (len == 0))
		return NULL;

	phd->Cmd = cmd;
	
	phd->Id = id;
	phd->ParaLen = len;

	if(ver == 0)
		phd->FormatVer = API_DEF_VER;
	else
		phd->FormatVer = ver;
	
	return phd;
}

void* ApiNewPkt(WORD id, WORD cmd, WORD ver, WORD data_len)
{
	PPARA_FORMAT_HD phd;

	if(data_len == 0)
		return NULL;

	if((phd = (PPARA_FORMAT_HD)m_malloc(data_len + API_PKT_LEN)) != NULL)
	{
		ApiPackHd(phd, cmd, id, ver, data_len + API_PKT_LEN);
		return (phd + 1);
	}
	else
	{
		//mprintf("NULL free buf\r\n");
		return NULL;
	}
}

