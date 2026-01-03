#ifndef _ZHONGWEI_H_
#define _ZHONGWEI_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

/*中威机型列表*/
#define ZW_MODEL_0731		((CUS_ZW << 16) + 1)

/*中威机型选择*/
#define CUS_MODEL 			ZW_MODEL_0731

/*射出时间精度选择*/
#undef INJ_TM_PRECIS
#define INJ_TM_PRECIS		1			/*0 10ms, 1: 1ms*/

#ifdef __cplusplus
}
#endif

#endif


