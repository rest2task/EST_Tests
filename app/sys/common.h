/*===========================================================================+
|  System   : Common                                                         |
|  Task     : Common header file                                             |
|----------------------------------------------------------------------------|
|  Compile  : CC' C2000 -                                                    |
|  Link     : CC' C2000 -                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Xiaqinghua                                                     |
|  Version  : V1.00                                                          |
|  Creation : 03/11/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     COMMON_H
#define     COMMON_H

#include 	"typedef.h"

#define     LOBYTE(w)           ((BYTE)((WORD)(w) & 0x0FF))
#define     HIBYTE(w)           ((BYTE)((WORD)(w) >> 8))

#if 0
#define     MASK_BIT0           0x01
#define     MASK_BIT1           0x02
#define     MASK_BIT2           0x04
#define     MASK_BIT3           0x08
#define     MASK_BIT4           0x10
#define     MASK_BIT5           0x20
#define     MASK_BIT6           0x40
#define     MASK_BIT7           0x80
#define     MASK_BIT8           0x0100
#define     MASK_BIT9           0x0200
#define     MASK_BIT10          0x0400
#define     MASK_BIT11          0x0800
#define     MASK_BIT12          0x1000
#define     MASK_BIT13          0x2000
#define     MASK_BIT14          0x4000
#define     MASK_BIT15          0x8000

// assemble program data exchange offset define

#define     PDBMACHSTART        0x0000
#define     PDBMOLDSTART        0x01A0   
#define     PDBCFGOFFSET        0x0400
#define     PDBPBASSIGNOFS      0x0440
#define     PDBPCASSIGNOFS      0x0450
#define     PDBLMTOFFSET        0x04C0
#define     PDBSTATOFFSET       0x0510   
#define     PDBSTATDAADOFFSET   0x0540 
#define     PDBPBOFFSET         0x0580
#define     PDBPCOFFSET         0x05B0
#define     PDBMONIOFFSET       0x0600
#define     PDBPBOFFSETX        0x06B0    //06-10-8
#define     PDBPCOFFSETX        0x06D0    //06-10-8
#define     PDBKEYOFFSET        0x0700    //05-5-12
#define     PDBCFGTEMP          0x0800
#define     PDBPBDEFINE         0x0900
#define     PDBPCDEFINE         0x0A00
#define     PDBTEMPEXT          0x0D00        // 扩展温度缓冲地址 08-2-28 
#define     PDBDALINESET        0x0E00    //08-3-25

#define     CFGLENGTH           0x80          // EEPROM写入部分
#define     CFGLENGTHUP         0x60          // CONFIG发送部分
#define  	Zdataoffset    (CFGLENGTH+ 0x80)
//#define     AD_STRU_MAX         11 
#endif

extern		WORD delt_time(WORD,WORD);
//extern      WORD   Ar_psrcaddr;  	//始终从0开始读写
//extern	WORD   Aw_pdestaddr;

extern volatile DWORD A_10usclock;
extern volatile DWORD A_100usclock;
extern volatile DWORD A_1msclock;
extern volatile WORD A_10msclock;
extern volatile WORD A_100msclock;
extern volatile WORD A_1sclock ; 

#define GetSecTick()	(A_1sclock)
#define Get100MsTick()	(A_100msclock)
#define Get10MsTick()	(A_10msclock)
#define GetMsTick()		(A_1msclock)
#define GetMsTick32()	(A_1msclock);
#define Get100UsTick()	(A_100usclock)

#define Get10UsTick()	(A_10usclock)

WORD delt_time(WORD Cur_T,WORD Del_T);
void delay_ms(WORD CN);


#endif
