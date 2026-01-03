#include "canhead.h"
#include "string.h"
 #include "can_e502.h"

CANE502_TXDATA  g_cane502txdata[TEMP_MAX_NUM];

void        TempE502DataPdoWrite(int id,WORD dat0,WORD dat1,WORD dat2)
{
    g_cantempe502comm[id].flag_send = true;

    g_cantempe502comm[id].pdo_tx_data[0] = dat0; 
    g_cantempe502comm[id].pdo_tx_data[1] = dat1; 
    g_cantempe502comm[id].pdo_tx_data[2] = dat2;
  
}


