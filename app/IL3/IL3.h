#ifndef _IL3_H_
#define _IL3_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include	"common.h"



typedef struct tag_TIMESTRU
{
	WORD       wBeginTime;
	WORD	   wTimeSpan; 
	WORD	   wCurTime;
	BOOL       bTimeRun;
	BOOL       bTimeEnd;
} TIMESTRU;

typedef struct tag_COUNTSTRU
{
	WORD			wSumCount;
	WORD			wCurCount;
	BOOL			bCountRun;
} COUNTSTRU;

typedef struct tag_FORSTRU
{
	WORD			*pIL;
	WORD			wCount;
} FORSTRU;

typedef struct tag_CALLSTRU
{
	WORD			*pIL;
	WORD			wIL;
} CALLSTRU;

typedef struct tag_ADDRESSTABLESTRU
{
	WORD			wSub;
	WORD			wAddressIn;
	WORD			wAddressOut;
} ADDRESSTABLESTRU;

typedef struct tagCanPlcTestComm
{
	WORD wLength;
	WORD tx_data[300];
	WORD rx_data[300];
	WORD wrxPos;
	BOOL flag_send;
} CanPlcTestComm;

typedef enum enCodeType
{
	CODE_X,
	CODE_Y,
	CODE_M,
	CODE_S,
	CODE_T,
	CODE_C,
	CODE_D
}CodeType;

extern		DWORD				dwInputX,dwInputXB;//X输入
extern		DWORD				dwOutputY,dwOutputYB;//Y输出
extern		DWORD				dwMRegister[20],dwMRegisterB[20];//M继电器
extern		DWORD				dwSRegister[20],dwSRegisterB[20];//S继电器
extern		TIMESTRU            Timer[600];//T
extern		COUNTSTRU           Counter[500];//C
extern		WORD				wPlcUpdate;
extern		WORD				wPLcCrcOK;
extern		BOOL				bReceiveBegin;
extern		DWORD				dwPlcFileLength;
extern		CanPlcTestComm		g_CanPlcTestComm;
extern		WORD				wPlcGetBuffer;
extern		DWORD				PlcIAPToFlashAddress;
extern		WORD				*PlcRecFileAdd;

#define		MAINADDRESS			0x0000
#define		PLCSTARTADDRESS		0x40000
#define		TABLEADDRESS		0x40002+MAINADDRESS
#define		ILLENADDRESS		0x4070A+MAINADDRESS
#define		ILADDRESS			0x4070E+MAINADDRESS
#define		MAX_PLCRECEIVE		260
#define		PLCBUFFERLEN		256
#define		Addr_PlcEntry		0x08040000


void InitAnaIL(void);
WORD AnalysisIL(WORD wSub);
void InitAddressTable(void);
WORD  AnaValue(WORD wType);
void  ILJmp(BOOL bJmp);
void  CloseOut(DWORD dAddFalse,DWORD dAddOut);
void SetOut(WORD wAddress,BOOL bMark);
void CrcCheck(void);

BOOL PLCIsUpdate(void);

void CanPlcSendFile(WORD* pick_rx_buf);
void CanPlcReceiveFile(WORD* pick_rx_buf);
void CanPLCTest(void);
void CanPLCTestTransmit(void);

void  Command8(void);
void  Command9(void);
void  Command10(void);
void  Command11(void);
void  Command12(void);
void  Command13(void);
void  Command14(void);
void  Command15(void);
void  Command16(void);
void  Command17(void);
void  Command18(void);
void  Command19(void);
void  Command20(void);
void  Command21(void);
void  Command22(void);
void  Command23(void);
void  Command24(void);
void  Command25(void);
void  Command30(void);
void  Command31(void);
void  Command32(void);
void  Command33(void);
void  Command34(void);
void  Command35(void);
void  Command36(void);
void  Command37(void);
void  Command38(void);
void  Command39(void);
void  Command40(void);
void  Command41(void);
void  Command42(void);
void  Command43(void);
void  Command44(void);
void  Command45(void);
void  Command46(void);
void  Command47(void);
void  Command48(void);
void  Command49(void);
void  Command50(void);

void  Command51(void);
void  Command52(void);
void  Command53(void);
void  Command54(void);
void  Command55(void);
void  Command56(void);
void  Command57(void);
void  Command58(void);
void  Command59(void);
void  Command60(void);
void  Command61(void);
void  Command62(void);
void  Command63(void);
void  Command64(void);
void  Command65(void);
void  Command66(void);
void  Command67(void);
void  Command68(void);
void  Command69(void);
void  Command70(void);
void  Command71(void);
void  Command72(void);
void  Command73(void);

void  Command76(void);
void  Command77(void);
void  Command78(void);
void  Command79(void);
void  Command80(void);
void  Command81(void);
void  Command82(void);
void  Command83(void);
void  Command84(void);
void  Command85(void);
void  Command86(void);
void  Command87(void);
void  Command88(void);
void  Command89(void);
void  Command90(void);
void  Command91(void);
void  Command92(void);
void  Command93(void);
void  Command94(void);
void  Command95(void);
void  Command96(void);
void  Command97(void);
void  Command98(void);
void  Command99(void);
void  Command100(void);
void  Command101(void);
void  Command102(void);
void  Command103(void);
void  Command104(void);
void  Command105(void);
void  Command106(void);
void  Command107(void);
void  Command108(void);
void  Command109(void);
void  Command110(void);
void  Command111(void);
void  Command112(void);
void  Command113(void);
void  Command114(void);
void  Command115(void);
void  Command116(void);
void  Command117(void);
void  Command118(void);

void  Command119(void);
void  Command120(void);
void  Command121(void);
void  Command122(void);
void  Command123(void);
void  Command124(void);
void  Command125(void);
void  Command126(void);
void  Command127(void);
void  Command128(void);
void  Command129(void);
void  Command130(void);
void  Command131(void);
void  Command132(void);
void  Command133(void);
void  Command134(void);
void  Command135(void);
void  Command136(void);
void  Command137(void);
void  Command138(void);
void  Command139(void);
void  Command140(void);
void  Command141(void);
void  Command142(void);
void  Command143(void);
void  Command144(void);
void  Command145(void);
void  Command146(void);
void  Command147(void);
void  Command148(void);
void  Command149(void);
void  Command150(void);
void  Command151(void);
void  Command152(void);
void  Command153(void);

void  Command154(void);
void  Command155(void);
void  Command156(void);
void  Command157(void);
void  Command158(void);
void  Command159(void);

void  Command160(void);
void  Command161(void);
void  Command162(void);
void  Command163(void);
void  Command164(void);

void  Command165(void);
void  Command166(void);
void  Command167(void);
void  Command168(void);
void  Command169(void);
void  Command170(void);

void  Command171(void);
void  Command172(void);
void  Command173(void);
void  Command174(void);
void  Command175(void);
void  Command176(void);

void  Command177(void);
void  Command178(void);
void  Command179(void);
void  Command180(void);
void  Command181(void);
void  Command182(void);

void  Command183(void);
void  Command184(void);
void  Command185(void);
void  Command186(void);
void  Command187(void);
void  Command188(void);

void  Command189(void);
void  Command190(void);
void  Command191(void);
void  Command192(void);
void  Command193(void);
void  Command194(void);

void  Command195(void);
void  Command196(void);
void  Command197(void);
void  Command198(void);
void  Command199(void);
void  Command200(void);

void  Command201(void);
void  Command202(void);
void  Command203(void);

void  Command204(void);
void  Command205(void);
void  Command206(void);
void  Command207(void);
void  Command208(void);
void  Command209(void);
void  Command210(void);
void  Command211(void);
void  Command212(void);
void  Command213(void);
void  Command214(void);
void  Command215(void);
void  Command216(void);
void  Command217(void);

void  Cmd1(WORD *pILB);
void  Cmd2(WORD *pILB);
void  Cmd3(WORD *pILB);
void  Cmd4(WORD *pILB);
void  Cmd5(WORD *pILB);

#ifdef __cplusplus
}
#endif

#endif


