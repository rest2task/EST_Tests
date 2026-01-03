#include "appentry.h"
#include "appmodinit.h"
#include "luactl.h"
#include "wgtplugcfg.h"
#include "keyprot.h"
#include "M788panel.h"

int main(int argc, char *argv[])
{
    Register_WGTLib_init();
#ifdef USE_QT
    REGISTER_STATIC_WGT_PLUG(wphistogram)
    REGISTER_STATIC_WGT_PLUG(wptimeline)
#ifdef M788_PANEL
    SetOpenLCDCb(OpenLCD_M788);
    SetSetLCDCb(SetLCD_M788);
    SetKeyBeepCb(KeyBeep_M788);
#endif
#endif
    return AppStart(AppModInit, argc, argv);
}
