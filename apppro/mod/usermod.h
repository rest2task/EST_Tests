#ifndef _USERMOD_h
#define _USERMOD_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

#define MAX_USERPWD  8

typedef enum enUSER_ID {
    USER_EVERYONE,
    USER_PROFESSIONAL,
    USER_DEALER,
    USER_MANUFACTURER,
    USER_SUPERVISOR
}USER_ID;//20190214.cyx

typedef struct tyUSER
{
	UI32 wpassword;
	UI32 cprivilege;
}USER,*PUSER;

typedef struct tyUSERPWD
{
	USER userpwd[MAX_USERPWD];
}USERPWD;

extern USERPWD dbuserpwd;

void InitUserPwd();
void SaveUserPwd(UI32 dst, void* src, UI16 wCount);
void NaviPasswordUser();

/*************************************************
Function: SetCurrentUserId
Description: 设置当前用户等级
Input: userid:设置的用户等级id
*************************************************/
void SetCurrentUserId(UI16 userid);//20190218.cyx
/*************************************************
Function: GetCurrentUserId
Description: 得到当前用户等级
Return: 当前用户等级id
*************************************************/
UI16 GetCurrentUserId();//20190218.cyx

#define USERPWD_INIT_FILE	SAVE_PATH"userpwd.dat"
#define USERMANAGE_INIT_FILE	SAVE_PATH"UserManage.dat" //震雄用户管理权限 //20190726.cyx

//画面锁住不可设模式 此为软件锁模式,均包含硬件锁
typedef enum enLOCK_MODE {
    MODE_NOLOCK,//无锁
    MODE_USERLOCK,//生产人员锁 PAGE_SHOW_LOCK
    MODE_MODELOCK,//模式锁 PAGE_PROTECT
    MODE_USER_MODELOCK,//生产人员与模式锁
    MODE_END
}LOCK_MODE;//20191224.cyx

void SetPageLockMode(UI16 index);
void SetPageProtect();
UI16 GetPageLockMode();

#ifdef __cplusplus
}
#endif

#endif



