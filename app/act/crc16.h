#ifndef _CRC16_H_
#define _CRC16_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

WORD CRC_CCITT16(void* pdata, int len);
WORD CRC_BCB(void* pdata, int len, UI16 cur_crc);

#ifdef __cplusplus
}
#endif

#endif


