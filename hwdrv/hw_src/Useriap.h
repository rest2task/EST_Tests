/*===========================================================================+
|  Class    : USER IAP                                                       |
|  Task     : IAP process                                                    |
|----------------------------------------------------------------------------|
|  Compile  : ADS1.2                                                         |
|  Link     : ADS1.2                                                         |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Whaul                                                          |
|  Version  : V1.00                                                          |
|  Creation : 2009-12-10                                                     |
|  Revision : 																 |												 |
+===========================================================================*/
#ifndef    __USERIAP_H
#define    __USERIAP_H


/*******************************************
 define
 ******************************************/
/*
#define		Addr_ProgEntry		0x00020000		// 应用程序入口	    	0x00008000	 
#define 	Addr_UserConfig		0x00008000		// 用户config入口		0x00078000
#define 	Addr_Origconfig		0x00009000		// 出厂config入口		0x00079000
#define 	Addr_Backconfig		0x0000A000		// 备份config入口	
#define 	Addr_UserFlag		0x0000B000	    // 更新标志区地址		0x0007C000
#define 	Addr_Zerodata		0x0000C000		// 用户调试参数区		0x0007A000
#define     Addr_zerodataBack   0x0000D000		// 用户调试参数备份区	0x0007B000
*/
#define		EXT_FLASH_BASE			0x60000000

#define		Addr_ProgEntry		(EXT_FLASH_BASE + 0x00020000)		// 应用程序入口	   
#define 	Addr_Infoexchange	(EXT_FLASH_BASE + 0x00200000)	  // 更新标志区地址		
#define 	Addr_Zerodata			(EXT_FLASH_BASE + 0x00201000)		// 用户调试参数区		
#define 	Addr_Origconfig		(EXT_FLASH_BASE + 0x00202000)		// 出厂config入口		
#define 	Addr_UserConfig		(EXT_FLASH_BASE + 0x00203000)		// 用户config入口	

#define		Addr_Encoder		0x60404000	    // 编码器烧写地址		 
#define   	Encoder_time		0x2000/0x200	//8K扇区写入次数

#define 	offset_AppFlag		0
#define 	offset_Iapok		6				//?6WORD????IAP????,??IAP????,???1???
#define 	offset_BootVersion	8				//BOOT????,?????
#define 	offset_VrInfoFlag	16				//??????????? ????WORD 
#define		offset_LowHDInfo	24
#define 	offset_LowSFInfo	32
#define		offset_HmiInfo		48
//#define		F_Prog_Update		0		 		// 程序更新标志(高8位)
#define 	F_Config_Update		8				// config更新标志(低8位)
#define		F_Config			16				//config更新有效标志,避免更新后重置步骤	
// 程序临时存放区
#define		Addr_ProgBackup		0x20000000		// 接收数据暂存区起始地址

/**************************** GPIO Pins allocation *****************************/
#define A1000_IO_TEST		2, 3	//The Macros will be used as parameters, brackets("()") is not allowed
#define A1000_E_RULER0		1, 30
#define A1000_E_RULER1		1, 31
#define A1000_KEY_MANU		3, 8
#define A1000_KEY_RST		3, 11
#define A1000_LED_RUN		2, 0
#define A1000_LED_UART		2, 4
#define A1000_LED_CANA		2, 5
#define A1000_LED_CANB		2, 6
#define A1000_FORCE_RST		3, 13
#define A1000_C24_TEST		0, 11
/***************************************************************************/

// SDRAM 地址
#define SDRAM_BASE_ADDR		0xA0000000
#define SDRAM_SIZE	        0x10000000	
		
/**********************************************
  EXTERNAL VARIABLE
********************************************/		
extern 	    uint16_t			*RcvData;				//接收数据缓冲区指针
extern 		uint32_t			RcvCount;				// 接收字节数
extern 		BOOL  				RcvOver;				// 接收完成标志 1：表示完成
extern 		uint32_t			*FlagPoint;
extern 		WORD				ErrCode;
extern 		uint32_t			posi_offset; 		
extern 		uint32_t			Pre_package;
extern 		BOOL				F_CONFIGUPDATE;
extern		BOOL				F_PGMUPDATE;

void 		HighToRam(void);
//uint32 		Program_IAP(/*uint32*/BYTE *SrcAddr,uint32 DstAddr,WORD LENG);
BOOL		Config_IAP(uint32_t,uint32_t,uint8_t);
void 		IAP_getcommudata(void);
void 		IAP_senddatatommi(void);
void 		IAP_sendreplytommi(WORD,WORD);
void 		ErrAction(WORD);

#endif

