#include "hydr.h"
#include "OilPortTb.h"
#include "machine.h"

//该obj存放在SRAM中，所以全局变量不能够做有数据的初始化。会在SRAM初始化的时候被清空。

typedef struct tyCUS_PORT_LIST CUS_PORT_LIST;
extern CUS_PORT_LIST cus_port_menu[OILPORTTB_NUM];
UI16 g_OilPortSelect = 0;	// 1表示内置表成立

void OilPortTbCopy()
{
	//威力仕标准油路
	CUS_PORT_LIST cus_port_weilishi[] = 
	{
		{INJECT_ID,	OS_INJE,	0x02, 0xF,	0x00},	//注射
		{INJECT_ID,	OS_INJE,	0x02, 0,	0x01},	//保压
		{INJECT_ID,	OS_CHRG,	0x02},
		{INJECT_ID,	OS_ACHRG,	0x02},
		{INJECT_ID,	OS_PCHRG,	0x02},
		{INJECT_ID,	OS_CHRGBFINJ,	0x02},
		{INJECT_ID,	OS_SUCK,	0x02},
		{INJECT_ID,	OS_SUCKCHG,	0x02},
		{INJECT_ID,	OS_ASUCK,	0x02},
		{NOZZLE_ID,	OS_NOZF,	0x02},
		{NOZZLE_ID,	OS_ANOZF,	0x02},
		{NOZZLE_ID,	OS_NOZB,	0x02},
		{NOZZLE_ID,	OS_ANOZB,	0x02},
		{CLAMP_ID, OS_OPNM,	0x01},
	};

	//经典开储联动油路
	CUS_PORT_LIST cus_port_classic1[] = 
	{
		{INJECT_ID, OS_CHRG,	0x02},
		{INJECT_ID, OS_ACHRG,	0x02},
		{INJECT_ID, OS_PCHRG,	0x02},
		{INJECT_ID, OS_CHRGBFINJ,	0x02},
		{INJECT_ID, OS_SUCK,	0x02},
		{INJECT_ID, OS_SUCKCHG, 0x02},
		{INJECT_ID, OS_ASUCK,	0x02},
		{NOZZLE_ID, OS_NOZF,	0x02},
		{NOZZLE_ID, OS_ANOZF,	0x02},
		{NOZZLE_ID, OS_NOZB,	0x02},
		{NOZZLE_ID, OS_ANOZB,	0x02},
		{CLAMP_ID, OS_OPNM,	0x01},
	};

	//经典开托联动油路
	CUS_PORT_LIST cus_port_classic2[] = 
	{
		{EJECT_ID, OS_EJTF,	0x02},
		{EJECT_ID, OS_EJTB,	0x02},
		{EJECT_ID, OS_VEJTF,	0x02},
		{EJECT_ID, OS_VEJTB,	0x02},		
		{EJECT_ID, OS_AEJTF,	0x02},
		{EJECT_ID, OS_AEJTB,	0x02},		
		{CLAMP_ID, OS_OPNM,	0x01},
	};

	//经典开模中子联动油路
	CUS_PORT_LIST cus_port_classic3[] = 
	{		
		{CORE_ID, OS_CPRF,	0x02},
		{CORE_ID, OS_CPRB,	0x02},
		
		{CORE_ID, OS_CPRAF,	0x02},
		{CORE_ID, OS_CPRAB,	0x02},
		
		{CLAMP_ID, OS_OPNM,	0x01},
	};

	//厂家列表
	CUS_PORT_LIST_PRO cus_port_list[] = 
	{
		{0,				cus_port_classic1, ARRAY_NUM(cus_port_classic1)},
		{CS_WEILISHI,	cus_port_weilishi, ARRAY_NUM(cus_port_weilishi)},
	};
	
	UI16 cs_num = 0,i = 0;
	
	if (GetSys()->md->FL_HYDY_WAY_SET == 1)
	{
		//有厂家默认油路图的
		for (cs_num = 0; cs_num < ARRAY_NUM(cus_port_list); cs_num++)
		{
			if (cus_port_list[cs_num].cus_id == CUS_ID)
			{
				for (i = 0;i < ARRAY_NUM(cus_port_menu); i++)
				{
					if (i < cus_port_list[cs_num].len)
					{
						cus_port_menu[i].part_id = cus_port_list[cs_num].cus_port[i].part_id;
						cus_port_menu[i].act_id = cus_port_list[cs_num].cus_port[i].act_id;
						cus_port_menu[i].port = cus_port_list[cs_num].cus_port[i].port;
						cus_port_menu[i].pump = cus_port_list[cs_num].cus_port[i].pump;
						cus_port_menu[i].act_flag = cus_port_list[cs_num].cus_port[i].act_flag;
					}
					else
					{
						cus_port_menu[i].part_id = 0xFFFF;
						g_OilPortSelect = 1;
						return;
					}
				}
			}
		}
	}
	else if (GetSys()->md->FL_HYDY_WAY_SET == 4)
	{//开托联动
		for (i = 0;i < ARRAY_NUM(cus_port_menu); i++)
		{
			if (i < ARRAY_NUM(cus_port_classic2))
			{
				cus_port_menu[i].part_id = cus_port_classic2[i].part_id;
				cus_port_menu[i].act_id = cus_port_classic2[i].act_id;
				cus_port_menu[i].port = cus_port_classic2[i].port;
				cus_port_menu[i].pump = cus_port_classic2[i].pump;
				cus_port_menu[i].act_flag = cus_port_classic2[i].act_flag;
			}
			else
			{
				cus_port_menu[i].part_id = 0xFFFF;
				g_OilPortSelect = 1;
				return;
			}
		}
	}
	else if (GetSys()->md->FL_HYDY_WAY_SET == 5)
	{//开中联动
		for (i = 0;i < ARRAY_NUM(cus_port_menu); i++)
		{
			if (i < ARRAY_NUM(cus_port_classic3))
			{
				cus_port_menu[i].part_id = cus_port_classic3[i].part_id;
				cus_port_menu[i].act_id = cus_port_classic3[i].act_id;
				cus_port_menu[i].port = cus_port_classic3[i].port;
				cus_port_menu[i].pump = cus_port_classic3[i].pump;
				cus_port_menu[i].act_flag = cus_port_classic3[i].act_flag;
			}
			else
			{
				cus_port_menu[i].part_id = 0xFFFF;
				g_OilPortSelect = 1;
				return;
			}
		}
	}
	else
	{//经典开储联动
		for (i = 0;i < ARRAY_NUM(cus_port_menu); i++)
		{
			if (i < ARRAY_NUM(cus_port_classic1))
			{
				cus_port_menu[i].part_id = cus_port_classic1[i].part_id;
				cus_port_menu[i].act_id = cus_port_classic1[i].act_id;
				cus_port_menu[i].port = cus_port_classic1[i].port;
				cus_port_menu[i].pump = cus_port_classic1[i].pump;
				cus_port_menu[i].act_flag = cus_port_classic1[i].act_flag;
			}
			else
			{
				cus_port_menu[i].part_id = 0xFFFF;
				g_OilPortSelect = 1;
				return;
			}
		}
	}
	
	g_OilPortSelect = 0;
}
