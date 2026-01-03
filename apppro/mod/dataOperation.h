#ifndef _DATAD_OPERATION_h
#define _DATAD_OPERATION_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

#define TYPEALL		(1<<0)
#define TYPEHOST	(1<<1)
#define TYPEHMI		(1<<2)
#define TYPEMOLD	(1<<3)
#define TYPESERVOR	(1<<4)
#define TYPEMACHSET	(1<<5)//20201210

#define OP_BACKUP2UDISK			(1<<0)	//导出到U盘标志，表示开始这个步骤
#define OP_BACKUPFROMUDISK		(1<<1)	//从U盘导入标志，表示开始这个步骤
#define OP_BACKUP2DEFAULT 		(1<<2)	//伺服数据保存为默认数据
#define	OP_BACKUPFROMDEFAULT	(1<<3)	//伺服数据保存为默认数据

#define ADDR_PASSWD_USER        0x33120047      //用户等级

typedef enum enOP_MODE
{
	EXPORT=0,			//导出
	IMPORT,				//导入
	RESTORTOSAVE,		//恢复到出厂值
	RESTORTODEF			//恢复到默认值
}OP_MODE;
#define	MOLD_PATH       SAVE_PATH"moldsetx.dat"
#define	MACHSET_PATH	SAVE_PATH"MachValue.csv"//20201210

//#define ESTDATA_PATH	USB_BOOT_PATH"estdata"
#define PANEL_SERVO_PATH        SAVE_PATH"servo.dat"
#define MOLD_FILE               SAVE_PATH"moldsetx.dat"
#define PANEL_FACTORY_FILE      DATA_PATH"est_fact.tag"	//出厂数据
#define PANEL_MACHCFG           SAVE_PATH"machcfg.dat"  //出厂数据
#define PANEL_DACFG             SAVE_PATH"dacfg.dat"    //出厂数据
#define PUMPCFG_FILE	        SAVE_PATH"pumpcfg.dat"  //泵浦数据
//20200312
//数据操作页面的出厂数据保存时，将主机的数据保存的同时，重新拷贝命名 防止U盘读入后，再去恢复出厂时数据已经被覆盖
#define PANEL_FACTORY_MACHCFG   SAVE_PATH"machcfg_fact.dat"//出厂数据
#define PANEL_FACTORY_DACFG		SAVE_PATH"dacfg_fact.dat"//出厂数据
#define PUMPCFG_FACTORY_FILE    SAVE_PATH"pumpcfg_fact.dat"  //泵浦数据

//20230518 chj 此处有个bug，当面板数据和结构参数一起导入的时候，如果est_cur.tag文件大小不同，
//即中间加过参数，再结合结构参数内的dic，可能会导致数据错乱，所以改为如果导入过数据，开机读取结构参数
#define PANEL_MACHVALUE_FILE    SAVE_PATH"MachValueFlag.dat"

//typedef struct tyCB_PARA {
//	int type;            
//	union {
//		const void * p;
//		int v;
//	} Data;
//}CB_PARA;
//typedef int (*OP_RESULT_CB)(CB_PARA para);

#define MACHSETDATA_NAME_LEN    255//地址描述最大长度
#define MACHSETDATA_DES_LEN     255//其它描述最大长度
typedef struct tyMACHSETDATA * PMACHSETDATA;//20201210
typedef struct tyMACHSETDATA{
    char name[MACHSETDATA_NAME_LEN];        //地址描述
    UI32 address;                           //下位机地址
    UI32 value;                             //设定值
    char desc[MACHSETDATA_DES_LEN];         //其它描述
    PMACHSETDATA pnext;
}MACHSETDATA;

BOOL LoadDataValueExcel(char filename[], BOOL bsend);
BOOL SaveDataValueExcel();

void RebootOK(PCPAR para);
void getServoAllData(int Id);
BOOL SaveServoFile(const char* filepath);
BOOL LoadServoFile(const char * filepath);
void LoadDACfgData();
void SavePanelData();
void SaveDAData();
int getServoIsOnline();//伺服CAN通信状态
UI32 getBgCYCTime();

void ExportToUdisk(int select);
void ImportFromUdisk(int select,const char *pstr);
void RestoreToSave(int select);
void RestoreToDefault(int select);

void ModInit();
BOOL DoOPMonitor();	//进行导入导出操作时的监控函数，用于update
BOOL DataOpSave();//保存操作
BOOL getServoOperateEnable();//返回伺服操作是否可用
int DataFileExist(OP_MODE opmode,const char* ptr);

void SavePumpCfg();
void LoadPumpCfg();
void SavePumpPage(int start, int end);
#ifdef __cplusplus
}
#endif

#endif



