#ifndef UDISK_H
#define UDISK_H
#include "typedef.h"
#include "obj.h"
#ifdef __cplusplus
extern "C"
{
#endif


//int system_deadline(const char* cmd, int sec);
int system_deadline(const char* command, int deadline, int* exit_code,BOOL bCompare);//20230518 chj 面板更新比较
void AutoUdiskRunKeyCnt(UI32 wkey, int press);
void AutoUdiskUpdate();
BOOL isExist_helpfx9999();
BOOL isExist_helpfxv7777();
UI16 RunKeyCnt();
BOOL isOkRunKey();
UI32 Countdown6s();
BOOL DetectCheckFile_VIPCmd();//检测U盘中是否存在特殊更新命令checkfxv6666 //20210914 dyl touch
BOOL CheckTouchScreen();//20210914 dyl touch
void DecectTouchTest();//20210914 dyl touch
BOOL CheckTouchScreenHeLiTai();//20220927 CHJ + //20221008 dyl
BOOL CheckTouchScreenXiChu();//西楚 //20221008 dyl

BOOL Compare_filename();//20230518 chj 面板更新比较
void CompareToPanel();
void BakToPanel();
void ReplaceChar(char *str, const char *oldStr, const char *newStr);
void delchar(char *str, char ch);
void strtokchar(char *oldStr, char *newStr);
int GetCharPos(const char *oldStr, char *newStr);
void ClearCompareFile();
BOOL bneedCompare;

#define HELPFX9999_PATH "/udisk/helpupdate/helpfxn9999"
#define HELPFXV7777_PATH "/udisk/helpupdate/helpfxnv7777"
#define CHECKFXV6666_PATH "/udisk/helpcheck/checkfxv6666"  //触摸屏检测文件 //20210914 dyl touch

#ifdef __cplusplus
}
#endif
#endif // CMDREC_H
