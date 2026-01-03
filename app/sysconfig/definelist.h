#ifndef     __DEFINELIST_H
#define     __DEFINELIST_H

#define	ALLELE	0x0000			/*全电系统*/
#define	MIXCLP	0x0001			/*混电系统,开合模为油压*/
#define MIXINJ	0x0002			/*混电系统,注射为油压*/
#define	SYSRUN	ALLELE

#define	POWERNULL		0x0000	/*没有输出*/
#define	POWERELEC		0x0001	/*电动输出*/
#define	POWERHYDR		0x0002	/*液压输出*/
#define	PORT0POWER		POWERELEC
#define	PORT1POWER		POWERELEC
#define	PORT2POWER		POWERELEC
#define	PORT3POWER		POWERELEC
#define	PORT0POSLIM		1
#define	PORT1POSLIM		1
#define	PORT2POSLIM		1
#define	PORT3POSLIM		0


#define	MAX_TORQUE		3000	/*电机最大扭矩:300.0%				*/
#define	MAXSERVONUM		1//(4*2)	/*配置从站个数：4*/ //slaves 4 to 8

#define	TENSION			2			/*张力传感器通道*/


// 获取两数的大值或小值
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define GETMAX(a, b) (((a) > (b)) ? (a) : (b))
#define GETMIN(a, b) (((a) < (b)) ? (a) : (b))
// 获取一个正负号
#define SIGN_1(a,b)	(((a) > (b)) ? (1) : (-1))

#endif
