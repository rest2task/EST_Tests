#ifndef __MD_MEMTBL_H__
#define __MD_MEMTBL_H__

#include "typedef.h"

#define MD_MEMPOLL_LEN	1024*12	//缓存大小单位字节	可根据需求调整

typedef struct tyMD_MEM_LINK
{
    struct tyMD_MEM_LINK* 		next;
    struct tyMD_MEM_LINK* 		prev;
    UI16             	len;           	//缓冲区大小 单位Byte
    UI16				state;			//状态 :0 未占用  1 占用  2 固定锁	3 运行锁	
}MD_MEM_LINK, *PMD_MEM_LINK ;


typedef struct tyMD_MEM_STATE
{
	UI8		app_num;		//当前申请缓存数量
	UI8		idle_rate;		//空闲率
	UI8 	min_idle_rate;	//最小空闲率
	UI8 	fail_num;		//请求失败累计
	UI16	max_mem;		//最大缓存空间
	UI16	idle_mem;		//空闲空间
}MD_MEM_STATE, *PMD_MEM_STATE;

//申请缓存
UI8* m_malloc(UI16 len);
//释放缓存
void m_free(UI8* pp);

//查看空闲率
UI16 GetMdMemIdleRateNow(void);
//查看当前申请缓存数量
UI16 GetMdMemAppNumNow(void);
//查看设定的最大缓存空间大小
UI16 GetMdMemMaxMem(void);
//查看上电后缓存的最小空闲率
UI16 GetMdMemMinIdleRateNow(void);
//查看申请缓存失败次数
UI16 GetMdMemFailNum(void);

//返回缓存池状态结构体
MD_MEM_STATE GetMdMemPollState(void);

#define M_free(pp)	m_free((UI8*) pp);



#endif
