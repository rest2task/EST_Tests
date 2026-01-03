#ifndef _MONMCU_H
#define _MONMCU_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "typedef.h"
#include "part.h"
#include "vardb.h"

typedef struct tyDATA_SET_ADR
{
	PVAR  		pvar;
	UI16		len;	
}DATA_SET_ADR, *PDATA_SET_ADR;

typedef void (*DATA_SET_RX_CB)(UI16 set_id, int dat_num, UI16* pdata, int adr_num, PDATA_SET_ADR pvar_adr);

typedef struct tyDATA_SET
{
	UI16			period_ms;
	UI16			set_id;
	UI16 			num;
	PVAR_ADR_ARRAY	pset_adr;
	PDATA_SET_ADR	pvar_adr;
	UI16			mem_num;		/*pvar_adr allocate memory number*/
	int				total_len;
	DATA_SET_RX_CB 	rx_cb;
	BOOL			bneedtran;
	BOOL 			bused;
}DATA_SET, *PDATA_SET;

BOOL IsBootUpMsgRx();
void BootUpMsgRx(UI16 type, UI16 error, UI16 hw_ver, UI16 sw_ver);
void DataSetRx(UI16 set_id, int num, UI16* pdata);
DATA_SET_RX_CB DataSetCbSet(UI16 set_id, DATA_SET_RX_CB rx_cb);

/*add DataSet require and send packet to mcu
set_id: data set id;
period_ms: require mcu period send time, 0 just send once; 
ind_num: data set variant address number;
padr:variant address, if need tranlate (btranlate == true) receive data to variand and set value, padr memory must be 
valid and not be freed all the time;
btranlate: when true, need tranlate receive data to vaiant and set value;
rx_cb: call back function when data receive;*/
BOOL DataSetReqAdd(UI16 set_id, UI16 period_ms, UI16 ind_num, PVAR_ADR_ARRAY padr, BOOL btranlate, DATA_SET_RX_CB rx_cb);

#ifdef __cplusplus
}
#endif

#endif




