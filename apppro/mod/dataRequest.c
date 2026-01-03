#include "dataRequest.h"
#include "typedef.h"
#include "vardb.h"
#define READ_RAM_ID		(100+0x100)

/**
* @brief     :请求某个地址的数据
* @param     :地址 收到数据后处理的函数
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20200306
*/
BOOL DataRequest(UI32 addr,DATA_SET_RX_CB cb)
{
	VAR_ADR_ARRAY  ppaddr[4];
	if(addr > 0x100000)
	{
		int i = 0;
		for(i = 0;i<ARRAY_NUM(ppaddr);i++)
		{
			ppaddr[i].address = addr+i;
			ppaddr[i].len = 1;
		}
		DataSetReqAdd(READ_RAM_ID,0,ARRAY_NUM(ppaddr),ppaddr,FALSE,cb);//设置回调函数
	}
	return TRUE;
}

