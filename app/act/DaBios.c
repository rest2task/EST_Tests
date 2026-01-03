/*===========================================================================+
|  Class    : AD Driver utility process                                         |
|  Task     : AD Driver utility process                                         |
|----------------------------------------------------------------------------|
|  Compile  : CC'C2000 -                                                     |
|  Link     : CC'C2000 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Zhanghaidong                                                     |
|  Version  : V1.00                                                          |
|  Creation : 11/21/2007                                                     |
|  Revision :                                                                |
+===========================================================================*/


#include "common.h"
#include "dabios.h"
#include "hwcfg.h"
#include "hwmid.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

/* Driver for DAC LTC1661
==========================================================*/

void DaOutPortEnable(BOOL benable)
{
	if(benable)
	{
		m_enable_daccurrent_out();
	}
	else
	{
        m_diable_daccurrent_out();
	}
}

