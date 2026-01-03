#ifndef _MACHCFG_h
#define _MACHCFG_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"

void SaveAllMachcfg();
void LoadAllMachcfg();

/*machine cfg reset*/
BOOL MachCfgReset();

/*machine cfg export*/
void MachCfgExport();


#define MACHINECFG_FILE	SAVE_PATH"machcfg.dat"


#ifdef __cplusplus
}
#endif

#endif



