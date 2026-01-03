#ifndef _OILPORTTB_H_
#define _OILPORTTB_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

typedef struct tyCUS_PORT_LIST_PRO
{
   UI16    			cus_id;
   PCUS_PORT_LIST   cus_port;
   UI16				len;
}CUS_PORT_LIST_PRO, *PCUS_PORT_LIST_PRO;

 

#endif
