/*===========================================================================+
|  Class    : system initial and reset                                       |
|  Task     : system proc header file                                        |
|----------------------------------------------------------------------------|
|  Compile  : CC'C2000-                                                      |
|  Link     : CC'C2000-                                                      |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Xiaqinghua                                                     |
|  Version  : V1.00                                                          |
|  Creation : 03/26/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     __SYSTEMPRO_H
#define     __SYSTEMPRO_H

#include    "common.h"
#include	"part.h"
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Statistics                                                       |
+---------------------------------------------------------------------------*/
/*===========================================================================+
|           External                                                         |
+===========================================================================*/
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
typedef struct tag_LED
  {
    WORD       cputime;
    WORD       cpusts;
    WORD	   upgrade;    	
  }  LED_STRU;	  

typedef enum enCOM_SEQ_STEP
{
	COM_INIT_START,
	COM_BOOTUP_OK,
	COM_PART_PARA,
	COM_SYS_PARA,
	COM_INIT_OK
}COM_SEQ_STEP;


typedef struct tySYS_ST
{
	UI32		system_flag;			//系统标记初始化 此标记用于表示系统处于什么状态(首模自动，自动，在循环，手动放开，系统重启，马达开启等)
	
	/*COM MMI state data*/
	COM_SEQ_STEP	com_step;
	BOOL			com_fail;
	BOOL			com_fail_post;		//在自动模式下，在再循环时，置通信失败
	UI32			last_req_tx_ms;
	UI8				req_retry;
	int				load_part_ind;
	PPART			cur_part;
	UI16			last_sub_bits;
	
	UI8				rx_mmi_part_cnt;	//启动时，从面板成功获取部件数据的部件数。
	/***********/
	UI16			Udp_init_step;		//通讯断开时，启动udp与232重练时的动作步
	UI16			Udp_init_cnt;		//通讯断开时，启动udp与232重练时的尝试次数
	
	BOOL			err_cnt;	
}SYS_ST, *PSYS_ST;

//N_INIFLG  system_flag
#define     INIF_COLD		0x0001      // 0   ; AFTER INIT ONLY KEEP THIS BIT 0 OR 1
#define     INTF_OFF		0x0002      // 4   ; OFF PROCESS
#define     INIF_MOTOR		0x0004      // 8   ; DURING MOTOR ON 
#define     INIF_AUTO		0x0008      // 9   ; TO AUTOMODE FIRST for recycle
#define     INIF_CYCLE		0x0010      // 10  ; CYCLE TIME SHOW
#define     INIF_AUTO1		0x0020      // 11  ; TO AUTOMODE FIRST for other use
#define     INIF_CHKPC		0x0040      // 12  ; DURING CHECK PC OUT
#define     INIF_RESET		0x0080      // 15  ; RESET  KEY     
#define     INIF_DEBUG		0x0100      // 15  ; 调机状态    
#define     INIF_CFGDBCHG	0x0200      // 15  ; 配置数据改变    
#define     INIF_DASCREEN	0x0400      // 15  ; DA页面

#define		INIF_INITPART		0x0800
#define		INIF_ETHERCAT		0x1000	//	EtherCat初始化状态上传。
#define		INIF_CLSOFF			0x2000	//关模手动放开标记
#define		INTF_LUB_OFF		0x4000	//润滑结束标记
#define		INIF_SDO_NULL		0x8000	//	EtherCat初始化状态上传。

#define		INIF_USEECAT		0x10000	//	使用Ecat
#define		INIF_SAFE_RESET		0x20000	//	安全门急停


#define MMI_ACK_TM				200
#define MAX_SUB_REQ_RETRY		3

#define BOOT_VER_OFFSET		8
#define BOOT_VER_TRY		1		//发送udp与232的握手命令的次数

void SystemInit(void);
void ResetSystem(void);
void SystemReset(void);
void RobotReset(void);
void RestartSystem(void);
void SystemRestart(void);
void ManualReset(void);
void systemmodiinit(void);
void systemSafemodiinit(void);
void LedFlash(void);
BOOL InitBaseCtrl(void);
void WritEepromData(WORD *srcaddr,uint32 dstaddr,/*WORD *dstaddr*/WORD length);
void ReadEepromData(WORD *srcaddr,uint32 dstaddr,/*WORD *dstaddr*/WORD length);
BOOL InitBaseCtrl(void);
void LoadPartConfig(void);
void LoadTblConfig(void);
UI32 GetRunFlag(void);
UI32 ChkRunFlag(UI32 mask);
UI32 SetRunFlag(UI32 mask);
UI32 ClrRunFlag(UI32 mask);
BOOL ComIsOk(void);
void SetComState(BOOL bok);
void ManualInitProc(BOOL berr_init);
void SendKeyToMMI(WORD key);
void SendBootUpMsgToMMI(void);
void SendRequestResetToPanel(void);
void ComStateProc(void);
UI32 CalMsTo100MsUI16(UI32 ms_t);
BOOL ComIsFail(void);
void ComStateReset(void);
void SysAdjCfgCmdRx(UI16 ctrl_cmd, UI16 data_len, UI16* pdat);
void UpgradeCmdRx(UI16 type);
UI32 CalMsTo10MsUI16(UI32 ms_t);
UI16* GetBootVer(void);
UI32 Cal100UsTo10MsUI16(UI32 us100_t);
void ComFailIsPost(void);
void SetPktAckOK(UI16 id, UI16 cmd);
void ExtExitDebug(void);
void delay_ms(WORD CN);
void CtrlModelInit(UI16 ctrl_model);

#define ManualInit()		ManualInitProc(FALSE)
#define ManualInitErr()		ManualInitProc(TRUE)

//void        interrupt nothing(void);

#endif

