#include "usermod.h"
#include "oprintf.h" 
#include "file.h"
#include "panel.h"
#include "database.h"
#include "panelcfg.h"
#include "texttbl.h"
#include "time.h"
#include "stdio.h"
#include "login.h"
#include "verinfo.h"

USERPWD dbuserpwd;
UI16 m_currentUserId;

USERPWD userpwd_default = {
	{
		{0,0},
		{5858,1},
		{14321,2},
		{19110,3},
		{12995,4},
		{7474,6},
		{5656,7},
		{19119,8}
	}
};

#define PASSWORD_PG_NAME   DIALOG_PASSWORD
#define THIS_PF PanelFindPage(PASSWORD_PG_NAME)

static PPAGE_FRAM password = NULL;
static PPAGE_FRAM last_pf = NULL; //20190430

/************************************************************************/
/* 初始化用户密码       20181226 hz                                     */
/************************************************************************/
void InitUserPwd()
{
	FILEHD filehd;

	filehd = FileOpen(USERPWD_INIT_FILE);
	if(!FILEHD_IS_OK(filehd))
	{
		filehd = FileCreate(USERPWD_INIT_FILE);
		memcpy(&dbuserpwd, &userpwd_default, sizeof(USERPWD));
		FileSeek(filehd, 0);
		FileWrite(filehd, &dbuserpwd, sizeof(USERPWD), NULL);
	}
	if(FILEHD_IS_OK(filehd))
	{
		FileRead(filehd, &dbuserpwd, sizeof(USERPWD), NULL);
        //login、passward弹窗密码对应 临时解决方案 下一步应该把boot.event文件去除
        m_pwdArray[1] = dbuserpwd.userpwd[1].wpassword;
        m_pwdArray[3] = dbuserpwd.userpwd[2].wpassword;
        m_pwdArray[5] = dbuserpwd.userpwd[3].wpassword;
	}
	FileClose(filehd);
}

/************************************************************************/
/* 保存用户修改密码                                                     */
/************************************************************************/
void SaveUserPwd(UI32 dst, void* src, UI16 wCount)
{
	FILEHD filehd;
	int writebytes;

	filehd = FileOpen(USERPWD_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
		FileSeek(filehd, dst);
		FileWrite(filehd, src, wCount, &writebytes);
	}
	FileClose(filehd);
}
/************************************************************************/
/* 用户权限密码框显示关闭                                               */
/************************************************************************/
void NaviPasswordUser()
{
	PPAGE_FRAM pcur = PanelCurPage();


	if(pcur && PageFramGetPriv() > 0 && last_pf !=pcur) //20190430
	{
		last_pf = pcur;

		if(password == NULL)
			password = THIS_PF;

		PanelCloseDialog(password);
		PageFramSetProtect(FALSE);

		if(IsPwdPrompt())
		{

			PanelShowDialog(password);
		}
		else
		{	

			if(PanelCfgStyle() == TIANJIAN_TYPE) //20140424
			{

				PageFramShow(pcur);
			}
		}
	}
	else
	{
		if(last_pf !=pcur)  last_pf = NULL; //20190430

		PanelCloseDialog(password);
	}
	
}

//20190218.cyx
void SetCurrentUserId(UI16 userid)
{
    if(userid <= USER_SUPERVISOR)
        m_currentUserId = userid;
}
UI16 GetCurrentUserId()
{
    return m_currentUserId;
}

/**
 * @brief      : 设置画面锁模式
 *
 * @param      : 0表示无锁,1表示生产人员锁,2表示操作模式锁,3表示生产人员和操作模式锁
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20191224
 */
void SetPageLockMode(UI16 index)
{
    if(MODE_NOLOCK == index)
    {
        VarAdrSetInt(LOCK_MSG_SHOW,0);
    }
    else if(MODE_USERLOCK == index)
    {
        VarAdrSetInt(LOCK_MSG_SHOW,1);
        PAGE_SHOW_LOCK
    }
    else if(MODE_MODELOCK  == index)
    {
        VarAdrSetInt(LOCK_MSG_SHOW,2);
    }
    else if(MODE_USER_MODELOCK  == index)
    {
        VarAdrSetInt(LOCK_MSG_SHOW,3);
        PAGE_SHOW_LOCK
    }
}

/**
 * @brief      : 获取画面锁模式
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20191224
 */
UI16 GetPageLockMode()
{
    if(3 == VarAdrToInt(LOCK_MSG_SHOW))
    {
        return MODE_USER_MODELOCK;
    }
    else if(2 == VarAdrToInt(LOCK_MSG_SHOW))
    {
        return MODE_MODELOCK;
    }
    else if(1 == VarAdrToInt(LOCK_MSG_SHOW))
    {
        return MODE_USERLOCK;
    }
    else
    {
        return MODE_NOLOCK;
    }
}

/**
 * @brief      : 设置画面操作模式保护
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20191224
 */
void SetPageProtect()
{
    if(MODE_MODELOCK == VarAdrToInt(LOCK_MSG_SHOW))
    {
        if(!VarAdrToInt(HARDWARE_LOCK))
        {
            PAGE_PROTECT
        }
    }
    else
    {
        if(!(VarAdrToInt(USER_KEYLOCK) || VarAdrToInt(HARDWARE_LOCK)))
        {
            PAGE_PROTECT
        }
    }
}
