#ifndef _CFGDEF_H_
#define _CFGDEF_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#ifndef INJ_TM_PRECIS
/*射出精度, 最小精度选择*/
#define INJ_TM_PRECIS		1			/*0 10ms, 1: 1ms*/
#endif

#ifndef FLOW_PRECIS
/*液压，流量小数点选择*/
#define FLOW_PRECIS			1			/*0 无小数点， 1: 1位小数点*/
#endif

#ifdef __cplusplus
}
#endif

#endif



