#ifndef     __ZERODEBUG_H
#define     __ZERODEBUG_H

#include "common.h"

//zerodata[]元素具体定义
enum DB_ZERODATA
{
	WDA_PRESSZERO = 0,
	WDA_PRESSZERO2,		
 	WDA_FLUZERO,
 	WDA_FLUZERO2,		
 	WDA_PRESSMAX,		
 	WDA_PRESSMAX2,		
 	WDA_FLUMAX,			
 	WDA_FLUMAX2,		
 	WTEMP_CH0ZERO,		
 	WTEMP_CH1ZERO,		
 	WTEMP_CH2ZERO,		
	WTEMP_CH3ZERO,		
  	WTEMP_CH4ZERO,		
	WTEMP_CH5ZERO,		
	WTEMP_CH6ZERO,		
	WTEMP_CH7ZERO,		
	WTEMP_CH0B,			
	WTEMP_CH1B,			
	WTEMP_CH2B,			
	WTEMP_CH3B,			
	WTEMP_CH4B,			
	WTEMP_CH5B,		
	WTEMP_CH6B,		
	WTEMP_CH7B,							//23
	WTIME_AGEB,							//24	老练时间
 	WTEMP_FLPRSUPDATE ,                     /* 25 修正 D/A 压力输出值     */
    WTEMP_FLFLOWUPDATE,                   /* 26 修正 D/A 流量输出值     */
    WTEMPDA_PRSOUT0,                      /* 27 压力DA 0%                  */
    WTEMPDA_PRSOUT1,                      /* 28 压力DA 7%                  */
    WTEMPDA_PRSOUT2,                      /* 29 压力DA 14%                 */
    WTEMPDA_PRSOUT3,                      /* 30 压力DA 21%                 */
    WTEMPDA_PRSOUT4,                      /* 31 压力DA 39%                 */
    WTEMPDA_PRSOUT5,                      /* 32 压力DA 61%                 */
    WTEMPDA_PRSOUT6,                      /* 33 压力DA 86%                 */
    WTEMPDA_PRSOUT7,                      /* 34 压力DA 100%                */
    WTEMPDA_FLOWOUT0,                     /* 35 流量 DA 0%                 */
    WTEMPDA_FLOWOUT1,                     /* 36 流量 DA 7%                 */
    WTEMPDA_FLOWOUT2,                     /* 37 流量 DA 14%                */
    WTEMPDA_FLOWOUT3,                     /* 38 流量 DA 21%                */
    WTEMPDA_FLOWOUT4,                     /* 39 流量 DA 39%                */
    WTEMPDA_FLOWOUT5,                     /* 40 流量 DA 61%                */
    WTEMPDA_FLOWOUT6,                     /* 41 流量 DA 86%                */
    WTEMPDA_FLOWOUT7,                     /* 42 流量 DA 99%                */
                                             
    WTEMP_FLPRSUPDATE2,                    /* 43 修正 D/A 压力输出值     */
    WTEMP_FLFLOWUPDATE2,                   /* 44 修正 D/A 流量输出值     */
    WTEMPDA2_PRSOUT0,                      /* 45 压力DA 0%                  */
    WTEMPDA2_PRSOUT1,                      /* 46 压力DA 7%                  */
    WTEMPDA2_PRSOUT2,                      /* 47 压力DA 14%                 */
    WTEMPDA2_PRSOUT3,                      /* 48 压力DA 21%                 */
    WTEMPDA2_PRSOUT4,                      /* 49 压力DA 39%                 */
    WTEMPDA2_PRSOUT5,                      /* 50 压力DA 61%                 */
    WTEMPDA2_PRSOUT6,                      /* 51 压力DA 86%                 */
    WTEMPDA2_PRSOUT7,                      /* 52 压力DA 100%                */
    WTEMPDA2_FLOWOUT0,                     /* 53 流量 DA 0%                 */
    WTEMPDA2_FLOWOUT1,                     /* 54 流量 DA 7%                 */
    WTEMPDA2_FLOWOUT2,                     /* 55 流量 DA 14%                */
    WTEMPDA2_FLOWOUT3,                     /* 56 流量 DA 21%                */
    WTEMPDA2_FLOWOUT4,                     /* 57 流量 DA 39%                */
    WTEMPDA2_FLOWOUT5,                     /* 58 流量 DA 61%                */
    WTEMPDA2_FLOWOUT6,                     /* 59 流量 DA 86%                */
    WTEMPDA2_FLOWOUT7,                     /* 60 流量 DA 99%                */

    WTEMP_FLPRSUPDATE3,                    /* 61 修正 D/A 压力输出值     */
    WTEMP_FLFLOWUPDATE3,                   /* 62 修正 D/A 流量输出值     */
    WTEMPDA3_PRSOUT0,                      /* 63 压力DA 0%                  */
    WTEMPDA3_PRSOUT1,                      /* 64 压力DA 7%                  */
    WTEMPDA3_PRSOUT2,                      /* 65 压力DA 14%                 */
    WTEMPDA3_PRSOUT3,                      /* 66 压力DA 21%                 */
    WTEMPDA3_PRSOUT4,                      /* 67 压力DA 39%                 */
    WTEMPDA3_PRSOUT5,                      /* 68 压力DA 61%                 */
    WTEMPDA3_PRSOUT6,                      /* 69 压力DA 86%                 */
    WTEMPDA3_PRSOUT7,                      /* 70 压力DA 100%                */
    WTEMPDA3_FLOWOUT0,                     /* 71 流量 DA 0%                 */
    WTEMPDA3_FLOWOUT1,                     /* 72 流量 DA 7%                 */
    WTEMPDA3_FLOWOUT2,                     /* 73 流量 DA 14%                */
    WTEMPDA3_FLOWOUT3,                     /* 74 流量 DA 21%                */
    WTEMPDA3_FLOWOUT4,                     /* 75 流量 DA 39%                */
    WTEMPDA3_FLOWOUT5,                     /* 76 流量 DA 61%                */
    WTEMPDA3_FLOWOUT6,                     /* 77 流量 DA 86%                */
    WTEMPDA3_FLOWOUT7,                     /* 78 流量 DA 99%                */

    WTEMP_FLPRSUPDATE4,                    /* 79 修正 D/A 压力输出值     */
    WTEMP_FLFLOWUPDATE4,                   /* 80 修正 D/A 流量输出值     */
    WTEMPDA4_PRSOUT0,                      /* 81 压力DA 0%                  */
    WTEMPDA4_PRSOUT1,                      /* 82 压力DA 7%                  */
    WTEMPDA4_PRSOUT2,                      /* 83 压力DA 14%                 */
    WTEMPDA4_PRSOUT3,                      /* 84 压力DA 21%                 */
    WTEMPDA4_PRSOUT4,                      /* 85 压力DA 39%                 */
    WTEMPDA4_PRSOUT5,                      /* 86 压力DA 61%                 */
    WTEMPDA4_PRSOUT6,                      /* 87 压力DA 86%                 */
    WTEMPDA4_PRSOUT7,                      /* 88 压力DA 100%                */
    WTEMPDA4_FLOWOUT0,                     /* 89 流量 DA 0%                 */
    WTEMPDA4_FLOWOUT1,                     /* 90 流量 DA 7%                 */
    WTEMPDA4_FLOWOUT2,                     /* 91 流量 DA 14%                */
    WTEMPDA4_FLOWOUT3,                     /* 92 流量 DA 21%                */
    WTEMPDA4_FLOWOUT4,                     /* 93 流量 DA 39%                */
    WTEMPDA4_FLOWOUT5,                     /* 94 流量 DA 61%                */
    WTEMPDA4_FLOWOUT6,                     /* 95 流量 DA 86%                */
    WTEMPDA4_FLOWOUT7,                      /* 96 流量 DA 99%                */
	WDATA_CORRECT,						   /* 97 参数区数据正常             */
	WTEMP_OFFSET,						   /* 98 室温偏移量                 */	
	WRESERVE99,							   /* 99 保留						*/
	WRESERVE100,						   /* 100 保留						*/
	WRESERVE101,						   /* 101 保留						*/
	WRESERVE102,						   /* 102 保留						*/
	WRESERVE103,						   /* 103 保留						*/	
	WRESERVE104,						   /* 104 保留						*/
	WRESERVE105,						   /* 105 保留						*/
	WRESERVE106,						   /* 106 保留						*/
	WRESERVE107,						   /* 107 保留						*/	
	WRESERVE108,						   /* 108 保留						*/	
	WRESERVE109,						   /* 109 保留						*/
	WRESERVE110,						   /* 110 保留						*/
	WRESERVE111,						   /* 111 保留						*/
	WRESERVE112,						   /* 112 保留						*/	
	WRESERVE113,						   /* 113 保留						*/	
	WRESERVE114,						   /* 114 保留						*/
	WRESERVE115,						   /* 115 保留						*/
	WRESERVE116,						   /* 116 保留						*/
	WRESERVE117,						   /* 117 保留						*/	
	WRESERVE118,						   /* 118 保留						*/	
	WRESERVE119,						   /* 119 保留						*/
	WRESERVE120,						   /* 120 保留						*/
	WRESERVE121,						   /* 121 保留						*/
	WRESERVE122,						   /* 122 保留						*/	
	WRESERVE123,						   /* 123 保留						*/	
	WRESERVE124,						   /* 124 保留						*/
	WRESERVE125,						   /* 125 保留						*/
	WRESERVE126,						   /* 126 保留						*/
	WDATA_XOR							   /* 127 前127个WORD的校验码		 */		
};


#define 	WCMD_DAUPDATE		800
#define		WCMD_ZERO			600
#define     WTEMP_CLIB			300
#define		RELUV300 			12209			//12.209mV
#define 	RELUV400   			16397			//16.397mV

extern WORD	wtempZero;
extern  volatile WORD zerodata[128];
extern WORD pre_debug_state;
extern void ZeroDebug(void);
void Zerodata_Reset(void);
void Age_record(void);
void Zerodata_Load(void);
WORD* ZeroData_Get(void);
UI16 Zerodata_AgeTM(void);

#endif
