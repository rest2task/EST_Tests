#ifndef _DA_H_
#define _DA_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"acttype.h"
#include 	"da_dic.h"

#define DA_FORCEOUT_MS	200
#define DA_ZEROBIT	2000  //2048   程序原本用的2048，但根据计算设定2048时电压范围在-0.2V~0.2V之间，因此保护区间改小 2023-11-17 ccz

typedef struct tyDABIOS
{
	PART			part;

	PDA_MACHSET		mh;
	PDA_STATE		st;

	UI8				FlDAAdjust;
	UI8				F2DAMAX;			//进入7474画面中，调节最大最小值时置为1
	UI16			last_out_ms[MAX_DA_NUM];
	UI8             FlCHGSPD;
}DA, *PDA;

#define DA_INIT_INFO		{{MH_ID, CAL_LEN(DA_MACHSET)}, {ST_ID, CAL_LEN(DA_STATE)}}

void DAMaxMinAdj(UI16 adj_type, UI16 ch, UI16 value);
BOOL InitDAPart(void);
void SetDA(int portno, WORD value,WORD Direction);
void SetDACurAdjMode(int mode);
void SetDATempMax(UI16 ch, UI16 temp_max);
void SetDATempMin(UI16 ch, UI16 temp_min);
void DATempMaxMinSave(void);
BOOL InitDAPart(void);
BOOL DAIsAdjust(void);
void DAMaxMinReset(void);
BOOL DACmd(UI32 cmd, UI32 para);
void DAForceOut(void);
void DAMaxMinStCpy(void);

#ifdef __cplusplus
}
#endif

#endif

