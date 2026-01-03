#ifndef _CFGDB_H_
#define _CFGDB_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#include "typedef.h"
#include "tbl.h"
#include "protcmd.h"
#include "hwcfg.h"

#define PART_DB_VER	0x02
#define OFFSET_backup	0x020000		//备份区数据与原始区数据的偏移量，为FLASH_F_P_ADR_backup 减去FLASH_F_P_ADR

#define FLASH_F_P_ADR		0x60204000
#define FLASH_U_P_ADR		0x60208000
#define FLASH_F_T_ADR		0x6020C000
#define FLASH_U_T_ADR		0x60210000
#define FLASH_4K_SIZE		0x1000

#define FLASH_F_P_ADR_backup		0x60224000		//出厂配置的备份区		20211110
#define FLASH_U_P_ADR_backup		0x60228000		//用户配置的备份区
#define FLASH_F_T_ADR_backup		0x6022C000		//出厂表的备份区
#define FLASH_U_T_ADR_backup		0x60230000		//用户表的备份区
#define FLASH_4K_SIZE_backup		0x1000			//备份区的数据大小


/*用于存储面板参数文件的最后一块FLASH大小*/
#define FALSH_ENDBLK_SIZE		0x8000			/*32k byte*/
#define FLASH_ENDBLK_ADR		0x78000
#define MMI_PARA_FILE_OFFSET	0x200
#define MMI_PARA_FILE_HD_ID		0x4D495041

/*存储区物理类型*/
#define HW_MEM_AREA	0
#define HW_FLASH_AREA	1
#define HW_EEPROM_AREA 2

/*存储区使用类型: 用户和出厂*/
#define DYNC_MEM_AREA	0
#define FACTORY_CFG_AREA	1
#define USER_CFG_AREA		2

//用户和出厂区的备份区		20211110
#define DYNC_MEM_AREA_backup	0
#define FACTORY_CFG_AREA_backup	1
#define USER_CFG_AREA_backup		2

#define CFG_CACHE_NUM	2
#define CFG_CACHE_SIZE	4000
#define MAX_CFG_HD 2
#define DEF_CFG_ITEM_LEN	16

#define HW_STOR_AREA_NUM	5

typedef enum enHWSTOR_STATE
{
	HWSTOR_ERASED = -2,
	HWSTOR_UNKNOW = -1,
	HWSTOR_CHKFAIL = 0,
	HWSTOR_OK,
}HWSTOR_STATE, PHWSTOR_STAT;

typedef BOOL (*HW_STOR_RW)(UI32 adr, void*pmem, int len);
typedef BOOL (*HW_STOR_DEL)(UI32 adr);


typedef struct tyCFG_LOC
{
	UI16	cfg_id;
	UI16	stor_id;
}CFG_LOC, *PCFG_LOC;

typedef struct tyCFG_ST
{
	UI16	stor_ind;
	UI8		berase;
}CFG_ST, *PCFG_ST;

//配置存储区
typedef struct tyHW_STOR_AREA
{
	UI8		id;							//存储区ID
	UI8		use_type;					//用户区或出厂区
	UI8		phy_type;					//存储区类型
	UI32	area_size;					//存储区大小
	UI32	area_adr;					//存储区地址
	void*	pbuffer;					//存储区内存缓存区，存储区EEPROM时使用
}HW_STOR_AREA, *PHW_STOR_AREA;
#define MAX_CFG_ITEM_NUM		30

/*出厂配置表存储在FLASH, 在A310, 为第4个16K块*/
#define FLASH_MEMOY_ADR				0x0800000
#define SYS_CFG_FLASH_ADR			(FLASH_MEMOY_ADR + 0xC000)
/*用户配置表存储在EEPROM, 在A310, EEPROM的偏移地址为0*/
#define USER_CFG_EEPROM_ADR			0x0000

//0x4555	3200 5200卓越电脑数据ID
//0x4554	其他电脑数据ID
#define CFG_HD_ID					0x4555	
#define CFG_HD_VER					0x0001		

typedef enum enHWSTOR_RD_TYPE
{
	HWSTOR_RD_ALL = 0,			//全部
	HWSTOR_RD_HD,				//表头
	HWSTOR_RD_ITEM				//表头加目录索引
}HWSTOR_RD_TYPE;

typedef enum enCFG_DB_ID
{
	CFG_ID_START = 1,
	FACTORY_PART_DB = CFG_ID_START,
	USER_PART_DB = 2,
	FACTORY_TBL_DB = 3,
	USER_TBL_DB = 4,
	CFG_ID_END
}CFG_DB_ID;

#define PART_ITEM 1
#define TBL_ITEM 2

#pragma pack(1)

//部件配置数据目录项
typedef struct tyPART_CFG_IND
{
	void*	psave;						/*如存储区是FLASH类型时，为FLASH读的内存地址 ; 如为EEPROM，为偏移地址，读到内存后重新计算为内存地址*/
	UI16	part_id;					/*部件ID*/
	UI8		ver;						/*版本*/
	UI8		sub_id;						/*子类ID*/
	UI16	offset;						/*数据在子类内的偏移*/	
	UI16	len;						/*数据长度，以WORD计数*/
}PART_CFG_IND, *PPART_CFG_IND;

//表配置目录项
typedef struct tyTBL_CFG_IND
{
	void*	psave;						/*如存储区是FLASH类型时，为FLASH读的内存地址; 如为EEPROM，为偏移地址，读到内存后重新计算为内存地址*/
	TBL_HD	hd;							/*表头*/
}TBL_CFG_IND, *PTBL_CFG_IND;

//存储区目录项，联合体
typedef union unCFG_IND
{
	PART_CFG_IND	part_ind;
	TBL_CFG_IND		tbl_ind;
	UI8				def[DEF_CFG_ITEM_LEN];
}CFG_IND,  *PCFG_IND;

//配置项
typedef struct tyCFG_ITEM
{
	UI16		item_type;		//类型 PART_ITEM(部件数据) TBL_ITEM(表)
	UI16		rev;			//对齐，保证psave是4字节对齐。
	CFG_IND		item;			//目录项
}CFG_ITEM, *PCFG_ITEM;

//存储区头结构体，2字节Crc16校验值放在整个存储区后。
typedef struct tyCFG_HD
{
	UI16	id;					//一个固定数字 0x4554
	UI8 	ver;				//0x01
	UI8		type;				//存储区域类型。
	UI32	cus_ver;			//用户版本
	UI32	len;				//以字节为单位, 配置数据长度， 包括CFG_HD。数据后面为2字节CRC16校验值。
	UI16	item_num;			//目录项数
	UI16	item_len;			//目录项的长度
}CFG_HD, *PCFG_HD;

typedef struct tyHW_STOR_ST
{
	PCFG_HD phd;
	I8		chk_st;			//HWSTOR_STATE
}HW_STOR_ST, *PHW_STOR_ST;

#pragma pack()

typedef struct tyCFGDB_UPDATE_ST
{
	UI16 	cfgdb_id;
	UI16 	tail_crc16;
	UI16 	cur_crc16;
	UI16 	org_crc16;
	UI32 	need_len;
}CFGDB_UPDATE_ST, *PCFGDB_UPDATE_ST;

typedef struct tyMMI_PARA_FILE_HD
{
	UI32	file_id;
	UI32	file_len;
	UI16	rev;
	UI16	crc_16;
}MMI_PARA_FILE_HD, *PMMI_PARA_FILE_HD;

typedef struct tyCFG_SAVE_LIST
{
	PPART pp;
	UI16 sub_id;
	UI16 len;
	UI16 offset;
	CFG_DB_ID db_id;
	UI16	state;		//0:表示无数据	1:数据需要写入
}CFG_SAVE_LIST, *PCFG_SAVE_LIST;

#define SAVE_CFGLIST_NUM	8	//代写FLASH最大个数
typedef struct tyCFG_SAVE_LIST_MENU
{
	UI32 lastsavetime;		//上一次写FLASH时间
	UI16 MoveCacheLocation;	//运行位置
	UI16 LoadCacheLocation;	//装载位置
	CFG_SAVE_LIST savelist[SAVE_CFGLIST_NUM];	//待写FLASH列表
}CFG_SAVE_LIST_MENU, *PCFG_SAVE_LIST_MENU;


void FreeCache(void* pcache);
UI8* GetCache(UI16* psize);
BOOL CfgDbUpdate(PPACK_PKT_RX_ST pst);
void CfgDbUpdateFail(CFG_DB_ID cfgdb_id, PPACK_PKT_RX_ST pst);
BOOL BootInitCfgDb(void);
BOOL LoadCfgDb(CFG_DB_ID cfgdb_id);
UI8* SetPartCfgDefault(PCFG_HD phd, UI8*psave, int max_item_num);
UI8* SetTblCfgDefault(PCFG_HD phd, UI8*psave,  int max_item_num);
void UserPartCfgReset(void);
BOOL CfgDbTblSave(UI16 type, UI8 sn, UI16 row, UI16 row_len, void*prow, CFG_DB_ID db_id);
BOOL CfgDbPartSave(PPART pp, UI16 sub_id, UI16 len, UI16 offset, BOOL bfactory);
const PART_CFG_IND* CfgDbPartRead(PPART pp, UI16 sub_id, UI16 len, UI16 offset, BOOL bfactory);
void UserTblCfgReset(void);
BOOL CfgDbUpload(CFG_DB_ID cfgdb_id, PPACK_PKT_TX_ST pst);
void ChkHWStor(void);
const TBL_CFG_IND* CfgDbTblRead(UI16 type, UI8 sn, CFG_DB_ID db_id);
void SetHyPriTblDefault(void);

void FactoryPartCfgReset(void);
void FactoryTblCfgReset(void);
void CfgDbSetDefault(void);

/*存储面板主机参数到FLASH的最后一块*/
BOOL MMIParaFileSave(PPACK_PKT_RX_ST pst);
BOOL MMIParaFileRead(PPACK_PKT_TX_ST pst);
UI16 ListSaveCfgDb(void);
UI16 SetListSaveCfgDb(PPART pp, UI16 sub_id, UI16 len, UI16 offset, CFG_DB_ID db_id);
UI16 InitListSaveCfgDb(void);


#define PartCfgSaveInList(pp, sub_id)	SetListSaveCfgDb((PPART)(pp), (sub_id), 0, 0, USER_PART_DB)
#define PartCfgSave(pp, sub_id)	CfgDbPartSave((PPART)(pp), (sub_id), 0, 0, USER_PART_DB)
#define PartCfgSaveFa(pp, sub_id)	CfgDbPartSave((PPART)(pp), (sub_id), 0, 0, FACTORY_PART_DB)
#define PartCfgRead(pp, sub_id)	CfgDbPartRead((PPART)(pp), (sub_id), 0, 0, USER_PART_DB)

#ifdef __cplusplus
}
#endif

#endif

