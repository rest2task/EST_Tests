#ifndef _can502_H_
#define _can502_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif
			
#include "common.h"	
 
#define E502_ENTER_DEBUG		1			/*主机进入调机状态*/
#define E502_EXIT_DEBUG		2			/*主机退出调机状态*/
#define E502_TEMP_ZERO_CMD	3			/*主机发送温度归零调机命令*/
#define E502_TEMP_CAB_CMD		4			/*主机发送温度调整命令*/
#define E502_TEMP_AMB_ADJ_CMD	5			/*主机发送室温校正命令*/
#define	E502_TEMP_DATA			6           /*主机发送单独数据命令*/
#define	E502_TEMP_VALUE		7           /*主机主机发送读E502温度命令*/

typedef struct tyCANE502_TXDATA
{
	UI16 cmd;			  	/*命令方式命令*/
	UI16 wtemp_channel_adjall;    /*高8位是温度调试界面的温度通道值；低8位是温度多通道联调标志*/
	UI16 wtemp_adjamb;    /*室温校准值*/

}CANE502_TXDATA, *PCANE502_TXDATA;

extern 	CANE502_TXDATA	g_cane502txdata[TEMP_MAX_NUM];

void    TempE502DataPdoWrite(int id,WORD dat0,WORD dat1,WORD dat2);
			
#endif
