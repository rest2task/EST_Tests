#ifndef _MEMTBL_H_
#define _MEMTBL_H_
	
#ifdef __cplusplus
		extern "C" {  /* define c style exports for c plus plus*/
#endif
	
#include "typedef.h"
#include "actctrl.h"
#include "types.h"

	/*word 计数， 实际内存大小* 2*/
#ifdef PLC_ENABLE
#define PLC_D_TOTAL_SIZE			8000			/*8000, word*/
#define PLC_D_USER_SIZE				2000			/*2k word */
#else
#define PLC_D_TOTAL_SIZE			4500			/*8000, word*/
#define PLC_D_USER_SIZE				0000			/*2k word */
#endif
	
#define DYNC_MEM_SIZE				45000			/*20k bytes*/
#define DYNC_TBL_SIZE				8000			/*12k bytes*/
	
#define TSK_ST_TBL_ACT_SIZE				((ACT_TSK_ST_SIZE)*MAX_TSK_NUM > 400 ? (ACT_TSK_ST_SIZE)*MAX_TSK_NUM : 400)
#define TSK_ST_TBL_REV_SIZE				800
#define TSK_ST_TBL_SIZE					(TSK_ST_TBL_REV_SIZE > TSK_ST_TBL_ACT_SIZE ? TSK_ST_TBL_REV_SIZE : TSK_ST_TBL_ACT_SIZE)
	
#define INNER_MAP_PLC_SIZE			(1000 - TSK_ST_TBL_SIZE)
#define CTRL_PART_MEMTBL_SIZE		1500
#define ACT_PART_MEMTBL_SIZE		2000
	
#define PLC_D_USER_OFFSET			0
#define TSK_ST_TBL_OFFSET			(PLC_D_USER_OFFSET + PLC_D_USER_SIZE)
#define INNER_MAP_PLC_OFFSET		(TSK_ST_TBL_OFFSET + TSK_ST_TBL_SIZE)
#define CTRL_TBL_OFFSET				(INNER_MAP_PLC_OFFSET + INNER_MAP_PLC_SIZE)
#define ACT_TBL_OFFSET				(CTRL_TBL_OFFSET + CTRL_PART_MEMTBL_SIZE)
	
	typedef enum enINNER_PLC_IND
	{
		PLC_RET_IND 	
	}INNER_PLC_IND;
	
	typedef struct tyMEM_TBL
	{
		int 	size;
		int 	free_pos;
		UI8*	pmem;
	}MEM_TBL, *PMEM_TBL;
	
	typedef enum enMEM_TBL_TYPE
	{
		MEMTBL_ACT_TSK,
		MEMTBL_CTRL_PART,
		MEMTBL_ACT_PART,
		MEMTBL_DYNC_MEM,
		MEMTBL_DYNC_TBL = MEMTBL_DYNC_MEM,
		MEMTBL_SDO_TBL,
		MEMTBL_DYNC_MEM_EXT,
		MEMTBL_TYPE_END
	}MEM_TBL_TYPE;
	
	extern UI16 m_plc_d[PLC_D_TOTAL_SIZE];
	void* AllocMemTbl(UI8 type, int size_t, int num);
	BOOL InitMemTbl(void);
	void *Memcpy1(UI16 *dest, UI16 *src,size_t count);	
	void *MemcpyPro(void *dest, void *src, size_t count);
	int UI16Count(int cn);

	
	//#define SetPLCRet(val)	(m_plc_d[INNER_MAP_PLC_OFFSET + PLC_RET_IND] = (val))
#define SetPLCRet(val)	
	
#ifdef PLC_ENABLE
#include "il3.h"
#define SET_PLC_M16(m, val)		{if((m)/16 < sizeof(dwMRegister)/2)  *((UI16*)(dwMRegister) + (m)/16) = (val);}
#endif
	
#ifdef __cplusplus
	}
#endif
	
#endif
	
	
	
