#include "mod.h"

//#define NO_MOD

#ifndef NO_MOD
#include "pay.h"
#include "pushtab.h"
#include "kjump.h"
#include "warm.h"
#include "temp.h"
//#include "keymod.h"
#include "login.h"
#include "alarmrecord.h"
#include "moldtemp.h"
#include "servostate.h"
#include "servo.h"
#include "power.h"
#include "texttbl.h"
#include "ioredef.h"
#include "verinfo.h"
#include "panelcfg.h"
#endif

static CMOD_INFO cmod_infos[] = 
{
#ifdef NO_MOD
	{"", NULL}
#else
	//mod_name,		init_func
	{"payset",		PaySetInital},
	{"pushtab",		PushtabInital},
	{"jump",		KJumpInital},
	{"temp",		TempSetInital},
	//{"key",			KeyModInital},
	{"login",		LoginInital},
	{"alarm",		AlarmRecdInital},
	{"moldtemp",	MoldTempInital},
	{"servostate",	ServoStateInital},
	{"servo",		ServoInital},
#endif
};

void CModInital()
{
    CModLoadInfos(cmod_infos, ARRAY_NUM(cmod_infos));
}
