#include "useract.h"
#include "act.h"
#include "actctrl.h"
#include "alarm.h"

const USER_ACT_MAP user_act_map[ ] = USER_ACT_MAP_INFO;


static __inline const USER_ACT_MAP* GetActMapByName(int act_name)
{
	if(act_name >= 0 && act_name < sizeof(user_act_map)/sizeof(USER_ACT_MAP))
		return user_act_map + act_name;
	else
		return NULL;
}

BOOL ChkUserActIsRun(int act_name)
{
	const USER_ACT_MAP* pm;

	if((pm = GetActMapByName(act_name)) != NULL)
		return ChkActRun(GetPartByID(MK_PART(pm->part_type, pm->part_sn)), ACT_SN_BIT(pm->act_sn));
	else
		return FALSE;
}

BOOL RunUserAct(int act_name, UI32 para, UI16 dly_ms)
{
	const USER_ACT_MAP* pm;

	if((pm = GetActMapByName(act_name)) != NULL)
	{
		AlarmManualInit();
		ClearAllPartError();
		return ForkAct(GetActByID(MK_ACT(pm->part_type, pm->part_sn, pm->act_sn)), FIRST_FORK, NULL, para, dly_ms);
	}
	else
		return FALSE;
}

void StopUserAct(int act_name)
{
	const USER_ACT_MAP* pm;
	PACT pact;
	
	if((pm = GetActMapByName(act_name)) != NULL)
	{
		pact = GetActByID(MK_ACT(pm->part_type, pm->part_sn, pm->act_sn));
		EndActTsk(pact->ptsk, TRUE);
	}
}

PPART GetUserActPart(int act_name)
{
	const USER_ACT_MAP* pm;

	if((pm = GetActMapByName(act_name)) != NULL)
		return GetPartByID(MK_PART(pm->part_type, pm->part_sn));
	else
		return NULL;
}

