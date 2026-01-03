#include "servo.h"
#include "oprintf.h" 
#include "file.h"
#include "canmod.h"
#include "database.h"
#include "stdio.h"
#include "cancom.h"


DB_SERVOS_EX g_dbsvosx;
DB_SERVOTYPT g_dbsvotype;

//菲仕伺服参数 
UI16 Phase_ServoxFixedValue[][5]={//额定功率，电压，电流，频率，转速
	{ 86,300,166,10000,1500},
	{120,364,243,13333,2000},
	{116,329,239,10000,1500},
	{140,341,282,11333,1700},
	{182,341,367,13333,2000},
	{220,360,410,10000,1500},
	{230,312,440,12000,1800},
	{283,321,605,13333,2000},
	{290,370,610,10000,1500},
	{287,380,554,11333,1700},
	{313,370,575,12000,1800},
	{367,340,737,13333,2000},
	{330,369,620,10000,1500},
	{394,377,926,11333,1700},
	{560,347,1207,13333,2000},
	{668,379,1593,12000,1800}
};

//汇川伺服参数
UI16 Inova_ServoxFixedValue[][5]={//额定功率，电压，电流，频率，转速
	{  89,380,153,11333,1700},     //ESMG1-89C17CD
	{ 105,380,190,13333,2000},     //ESMG1-11D20CD//增加
	{ 134,380,241,11333,1700},     //ESMG1-13D17CD
	{ 157,380,277,13333,2000},     //ESMG1-15D20CD//增加
	{ 164,380,295,11333,1700},     //ESMG1-17D17CD
	{ 193,380,355,13333,2000},     //ESMG1-19D20CD//增加
	{ 205,380,351,11333,1700},     //ESMG1-21D17CD
	{ 241,380,438,13333,2000},     //ESMG1-25D20CD//增加
	{ 267,380,482,11333,1700},     //ESMG1-27D17CD
	{ 314,380,555,13333,2000},     //ESMG1-31D20CD//增加
	{ 302,380,519,11333,1700},     //ESMG2-30D17CD//修改：//ESMG2-30D17CD
	{ 356,380,678,13333,2000},     //ESMG2-36D20CD//增加: //ESMG2-36D20CD
	{ 409,380,739,11333,1700},     //ESMG2-42D17CD
	{ 482,380,891,13333,2000},     //ESMG2-48D20CD//增加
	{ 507,380,915,11333,1700},     //ESMG2-50D17CD
	{ 597,380,1153,13333,2000},    //ESMG2-60D20CD//增加
	{ 605,380,1038,11333,1700},    //ESMG2-62D17CD
	{ 712,380,1355,13333,2000},    //ESMG2-72D20CD//增加
	{ 783,380,1450,11333,1700},    //ESMG2-80D17CD
	{ 921,380,1813,13333,2000},    //ESMG2-92D20CD//增加

	{ 340,380,576,11333,1700},      //ISMQ2-34D17CD
	{ 399,380,688,11333,1700},      //ISMQ2-40D17CD
	{ 510,380,912,11333,1700},      //ISMQ2-51D17CD

	// S4
	{ 70,330,140,10000,1500},    //ISMG1-55C15CD
	{ 80,325,170,11333,1700},    //ISMG1-62C17CD
	{ 95,320,190,13333,2000},    //ISMG1-75C20CD

	{ 95,380,185,10000,1500},      //ISMG1-95C15CD
	{ 110,380,217,11333,1700},     //ISMG1-11D17CD
	{ 126,380,260,13333,2000},     //ISMG1-12D20CD
	{ 141,380,292,10000,1500},     //ISMG1-14D15CD
	{ 160,380,325,11333,1700},     //ISMG1-16D17CD
	{ 188,380,364,13333,2000},     //ISMG1-18D20CD
	{ 173,380,357,10000,1500},     //ISMG1-17D15CD
	{ 196,380,407,11333,1700},     //ISMG1-20D17CD
	{ 230,380,476,13333,2000},     //ISMG1-23D20CD

	{ 220,380,417,10000,1500},     //ISMG1-22D15CD
	{ 240,380,487,11333,1700},     //ISMG1-24D17CD
	{ 283,380,584,13333,2000},     //ISMG1-28D20CD
	{ 306,380,633,10000,1500},     //ISMG1-30D15CD
	{ 347,380,704,11333,1700},     //ISMG1-34D17CD
	{ 410,380,789,13333,2000},     //ISMG1-41D20CD

	{ 314,380,617,10000,1500},     //ISMG2-31D15CD
	{ 356,380,722,11333,1700},     //ISMG2-36D17CD
	{ 419,380,866,13333,2000},     //ISMG2-42D20CD
	{ 424,380,877,10000,1500},     //ISMG2-42D15CD
	{ 481,380,975,11333,1700},     //ISMG2-48D17CD
	{ 565,380,1093,13333,2000},     //ISMG2-57D20CD
	{ 605,380,1188,10000,1500},     //ISMG2-60D15CD
	{ 685,380,1390,11333,1700},     //ISMG2-68D17CD
	{ 806,380,1667,13333,2000},     //ISMG2-80D20CD
	{ 801,380,1656,10000,1500},     //ISMG2-80D15CD
	{ 908,380,1656,11333,1700},     //ISMG2-91D17CD
	{1068,380,2065,13333,2000},     //ISMG2-11E20CD
};

//新伺服地址
UI16 ServoxAddr[]={
	0xA300,   //  0 - 驱动器控制模式 0:调试,3:EST控制,5:伺服5 新增
	0x1F02,   // 1 - 电机打包代码  新增
	0xF101,   //  2 - 0额定功率  0.4---1000.0 kW
	0xF102,   //  3 - 1额定电压  0---440 V
	0xF103,   //  4 - 2额定电流  0.01---655.35 A
	0xF104,   //  5 - 3额定频率  0.00---最大频率 Hz
	0xF105,   //  6 - 4额定转速  0---30000 rmp

	0xF00A,   //  7 - 5最大频率  50.00---300.00 Hz
	0xF012,   //  8 - 8减速时间  0.0---6500.0 s
	0xA301,   //  9 - 最大转速  0---30000rmp 新增

	0xF00F,   //  10 - 6载波频率  0.5---16.0 Hz
	0xF011,   // 11 - 7加速时间  0.0---6500.0 s
	0xF200,   // 12 - 9速度环比例增益  0---100
	0xF201,   // 13 - 10速度环积分时间  0.01---10.00 s
	0xF20A,   // 14 - 扭矩上限  0.0---250.0 %  新增

	0xF10B,   // 15 - 33D轴电感
	0xF10C,   // 16 - 34Q轴电感
	0xF10D,   // 17 - 35定子电阻
	0xF10E,   // 18 - 36单位
	0xF10F,   // 19 - 37反电动势

	0xF20D,   // 20 - 39DP
	0xF20E,   // 21 - 40DI
	0xF20F,   // 22 - 41QP
	0xF210,   // 23 - 42QI

	0xA102,   // 24 - 44初始角
	0xA103,   // 25 - 45方向



	0xF21B,   // 26 - 自动相序调整  新增

	0xF009,   // 27 - 31旋转方向
	0xF00C,   // 28 - 32频率上限

	0xF202,   // 29 - 切换频率1 0.00 ～F2-05 新增
	0xF203,   // 30 - 38速度环比例增益2  0 ～ 400
	0xF204,   // 31 - 速度环积分时间2  0.01s ～10.00s 新增
	0xF205,   // 32 - 切换频率2  F2-02 ～最大频率 新增

	0xF819,   // 33 - 制动单元允许开启时间 0.1s ～ 3600.0s 新增
	0xF911,   // 34 - 电机超温保护方式  新增

	0xA308,   // 35 - 最大反向速 0.0% ～ 100.0% 新增

	0xA303,   // 36 - 22最大油压  0.0---500.0 kg/cm2
	0xA30A,   // 37 - 23底压  0.0---50.0 kg/cm2

	0xA322,   // 38 - 从机中间点输入 A3-34 ～ A3-36   新增
	0xA323,   // 39 - 从机中间点输入对应 -100.0% ～ 100.0% 新增
	0xA320,   // 40 - 从机最小输入 0.0% ～ A3-34       新增
	0xA321,   // 41 - 从机最小输入对应 -100.0% ～ 100.0%   新增
	0xA324,   // 42 - 从机最大输入 A3-34 ～ 100.0%    新增
	0xA325,   // 43 - 从机最大输入对应 -100.0% ～ 100.0%   新增

	0xA304,   // 44 - 油压指令斜坡时间 0.000s ～ 2.000s 新增
	0xA305,   // 45 - 油压控制KP1   0.0 ～ 800.0 新增
	0xA306,   // 46 - 油压控制ti1   0.001s ～ 10.000s 新增
	0xA307,   // 47 - 油压控制td1   0.000s ～ 1.000s 新增
	0xA309,   // 48 - 底流   0.0% ～ 50.0% 新增
	0xA315,   // 49 - 油压传感器故障检测时间    0.000s：检测无效 0.001s ～ 60.000 新增
	0xA319,   // 50 - 给定油压上升S 滤波时间 0.000s ～ 1.000s 新增
	0xA31A,   // 51 - 给定油压下降S 滤波时间 0.000s ～ 1.000s 新增
	0xA31B,   // 52 - 超调抑制检测等级 0~2000 新增
	0xA31E,   // 53 - 压力模式切换速度模式转矩上限 50.0%~250.0% 新增
	0xA326,   // 54 - 多泵主机判断是否发送从机转速使能 0：禁止从机速度使能 1：允许从机速度使能 新增
	0xA327,   // 55 - 多泵合流保压控制增益 20~800 新增

	0xA403,   // 56 - 流量上升滤波时间 0~1.000s   新增
	0xA404,   // 57 - 流量下降滤波时间 0~1.000s   新增
	0xA40A,   // 58 - 注射动作曲线S曲线上升时间 0.001s ～ 1.000s 新增
	0xA40B,   // 59 - 注射动作曲线S曲线下降时间 0.001s ～ 1.000s 新增
	0xA40C,   // 60 - 注射动作流量上升斜率 0~5.000s 新增
	0xA40D,   // 61 - 注射动作流量下降斜率 0~5.000s 新增
	0xA40E,   // 62 - 注射给定油压上升时间 0~2.000s 新增
	0xA40F,   // 63 - 注射给定油压下降时间 0~2.000s 新增
	0xA41A,   // 64 - 油压 PID 算法选择   0~2     新增
	0xA41B,   // 65 - 保留  新增

	0xF400,   // 66 - DI1端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
	0xF401,   // 67 - DI2端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
	0xF402,   // 68 - DI3端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
	0xF403,   // 69 - DI4端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
	0xF404,   // 70 - DI5端子功能选择  0：无功能 1：正转使能 2：反转使能 3：三线式运行控制 4：正向点动使能 5：反向点动使能 6 ～ 7：保留 8：自由停车 9：故障复位 10：保留 11：外部故障常开输入 12 ～ 32：保留 33：外部故障常闭输入 34 ～ 47：保留 48：伺服油泵PID选择端子1 49：伺服油泵PID选择端子2 50：CAN 通信使能 51：从机作主机使能 52：压力切换到速度模式端子 53：从泵地址选择端子1 54：从泵地址选择端子2 55：射胶转保压端子 56：故障复位端子2过流故障无法复位） 新增
	0xF412,   // 71 - AI1最小输入  -11.00V ～ 11.00V    新增
	0xF413,   // 72 - AI1最小输入对应设定 -100.0% ～ 100.0% 新增
	0xF414,   // 73 - AI1最大输入  -11.00V ～ 11.00V    新增
	0xF415,   // 74 - AI1最大输入对应设定 -100.0% ～ 100.0% 新增
	0xF416,   // 75 - AI1输入滤波时间  0.000s ～ 10.000s 新增
	0xF417,   // 76 - AI2最小输入  -11.00V ～ 11.00V    新增
	0xF418,   // 77 - AI2最小输入对应设定 -100.0% ～ 100.0% 新增
	0xF419,   // 78 - AI2最大输入  -11.00V ～ 11.00V    新增
	0xF41A,   // 79 - AI2最大输入对应设定 -100.0% ～ 100.0% 新增
	0xF41B,   // 80 - AI2输入滤波时间  0.000s ～ 10.000s 新增
	0xF41D,   // 81 - AI3最小输入对应设定 -100.0% ～ 100.0% 新增
	0xF41E,   // 82 - AI3最大输入  -11.00V ～ 11.00V    新增
	0xF41F,   // 83 - AI3最大输入对应设定 -100.0% ～ 100.0% 新增
	0xF420,   // 84 - AI3输入滤波时间  0.000s ～ 10.000s 新增

	0xF501,   // 85 - 控制板继电器（T/A1-T/B1-T/C1）输出选择 0：无输出 1：伺服驱动器运行中 2：故障输出 3 ～ 5：保留 6：电机过载预报警 7：驱动器过载预报警 8 ～ 11：保留 12：运行时间到达 13 ～ 14：保留 15：运行准备就绪 16 ～ 19：保留 20: 通讯设定 21 ～ 22:保留 23：双排量柱塞泵斜盘切换1 24：压力控制状态输出 25：从泵报警输出 26：双排量柱塞泵斜盘切换2 27：母线电压建立 28：商务运行时间到达 29：商务运行时间不足24小时 30：最大反向转速DO输出 新增
	0xF502,   // 86 - 控制板继电器（T/A2-T/C2）输出选择 0：无输出 1：伺服驱动器运行中 2：故障输出 3 ～ 5：保留 6：电机过载预报警 7：驱动器过载预报警 8 ～ 11：保留 12：运行时间到达 13 ～ 14：保留 15：运行准备就绪 16 ～ 19：保留 20: 通讯设定 21 ～ 22:保留 23：双排量柱塞泵斜盘切换1 24：压力控制状态输出 25：从泵报警输出 26：双排量柱塞泵斜盘切换2 27：母线电压建立 28：商务运行时间到达 29：商务运行时间不足24小时 30：最大反向转速DO输出 新增
	0xF503,   // 87 - 控制板继电器（T/A3-T/C3）输出选择 0：无输出 1：伺服驱动器运行中 2：故障输出 3 ～ 5：保留 6：电机过载预报警 7：驱动器过载预报警 8 ～ 11：保留 12：运行时间到达 13 ～ 14：保留 15：运行准备就绪 16 ～ 19：保留 20: 通讯设定 21 ～ 22:保留 23：双排量柱塞泵斜盘切换1 24：压力控制状态输出 25：从泵报警输出 26：双排量柱塞泵斜盘切换2 27：母线电压建立 28：商务运行时间到达 29：商务运行时间不足24小时 30：最大反向转速DO输出 新增
	0xF50A,   // 88 - AO1输出选择  0：运行频率 1：设定频率 2：输出电流 3：输出转矩 4：输出功率 5：输出电压 6：保留 7：AI1 8：AI2 9：AI3 10：反馈转速（油压模式） 11：反馈压力（油压模式） 12 ～ 16：保留  新增
	0xF50B,   // 89 - AO2输出选择  0：运行频率 1：设定频率 2：输出电流 3：输出转矩 4：输出功率 5：输出电压 6：保留 7：AI1 8：AI2 9：AI3 10：反馈转速（油压模式） 11：反馈压力（油压模式） 12 ～ 16：保留  新增
	0XF50E,   // 90 - AO1零偏系数  -100.0% ～ 100.0%  新增
	0xF50F,   // 91 - AO1增益      -10.00 ～ 10.00    新增
	0xF510,   // 92 - AO2零偏系数  -100.0% ～ 100.0%  新增
	0xF511,   // 93 - AO2增益      -10.00 ～ 10.00    新增

	0xF100,   // 94 - 电机类型选择  0：普通异步电机 1：变频异步电机 2：永磁同步伺服电机  新增
	0xF206,   // 95 - 转差补偿系数  50% ～200%   新增
	0xF207,   // 96 - 速度反馈滤波时间 0.5ms ～ 10.0ms 新增
	0xF208,   // 97 - 转矩控制  0：无效 1：有效  新增
	0xF209,   // 98 - 转矩上限源  0：F2-10 1：AI1 2：AI2 3：AI3 4：保留 5：通信给定模拟输入量程对应F2-10  新增
	0xF20B,   //99 - 转矩滤波带宽  0Hz~1500Hz 新增
	0xF20C,   //100 - 保留   新增
	0xF211,   //101 -
	0xF212,   //102 -
	0xF213,   //103 -
	0xF214,   //104 -
	0xF215,   //105 -
	0xF216,   //106 -
	0xF217,   //107 -
	0xF218,   //108 -
	0xF219,   //109 - 过电压调制系数  100%~120%  新增
	0xF21A,   //110 - 母线电压滤波   0.000~0.100  新增
	0xF21C,   //111 -

	0xF90C,   //112 - 输入缺相保护选择  0：禁止 1：允许  新增
	0xF90D,   //113 - 输出缺相保护选择  0：禁止 1：允许  新增
	0xF90E,   //114 - 飞车速度偏差设定  0.50Hz~50.00Hz 新增
	0xF90F,   //115 - 飞车故障判断时间  0.1s~20.0S     新增
	0xF910,   //116 - 电机温度保护使能  0：禁止 1：允许  新增

	0xA000,   //117 - 弱磁控制方式  0：直接计算 1：自动调整 2：自动调整 + 计算  新增
	0xA001,   //118 - 弱磁电流系数  0~500    新增
	0xA002,   //119 - 同步机弱磁深度  0~50%  新增
	0xA003,   //120 - 同步机最大出力调整增益     20%~300%  新增
	0xA004,   //121 - 同步机计算励磁电流调整增益 40%~200%  新增

	0xA100,   //122 - PG卡类型  0：旋转变压器 1：保留 2：普通 ABZ 编码器  新增
	0xA104,   //123 - 旋转变压器极对数     1 ～ 50  新增
	0xA105,   //124 - 旋变断线故障检测时间  0.000：检测无效 0.001s ～ 60.000s 新增
	0xA106,   //125 - 编码器线数  0 ～ 65535  新增

	0xA200,   //126 - 波特率选择  0：20k 1：50k 2：125k 3：250k 4：500k 5：1M  新增
	0xA202,   //127 - CAN 连续通讯时间  0.0S（无效）0.1S～ 600.0s  新增
	0xA203,   //128 - CAN 多泵模式  0（广播模式）1（多主模式） 新增
	0xA204,   //129 - CAN 从机地址1  0~65535  新增
	0xA205,   //130 - CAN 从机地址2  0~65535  新增
	0xA206,   //131 - CAN 从机地址3  0~65535  新增
	0xA207,   //132 - CAN 从机地址4  0~65535  新增
	0xA208,   //133 -
	0xA209,   //134 -

	0xA302,   //135 - 系统油压  0.0kg/cm2 ～最大油压（A3-03）  新增
	0xA30B,   //136 - 油压控制 Kp2  0.0 ～ 800.0      新增
	0xA30C,   //137 - 油压控制 Ti2  0.001s ～ 10.000s 新增
	0xA30D,   //138 - 油压控制 Td2  0.000s ～ 1.000s  新增
	0xA30E,   //139 - 油压控制 Kp3  0.0 ～ 800.0      新增
	0xA30F,   //140 - 油压控制 Ti3  0.001s ～ 10.000s 新增
	0xA310,   //141 - 油压控制 Td3  0.000s ～ 1.000s  新增
	0xA311,   //142 - 油压控制 Kp4  0.0 ～ 800.0      新增
	0xA312,   //143 - 油压控制 Ti4  0.001s ～ 10.000s 新增
	0xA313,   //144 - 油压控制 Td4  0.000s ～ 1.000s  新增
	0xA316,   //145 - 压力控制状态输出最高转速设定  0.0% ～ 100.0%  新增
	0xA317,   //146 - 压力控制状态输出最低油压设定  0.0% ～ 100.0%  新增
	0xA318,   //147 - 压力控制状态输出延迟时间 0.000s ～ 10.000s 新增
	0xA31C,   //148 - 超调抵制系数   0~3.000  新增
	0xA31D,   //149 - 油压环增益系数  0.20~5.00  新增
	0xA31F,   //150 - 注射阀门开通延迟时间  0.020s ～ 0.500s  新增
	0xA328,   //151 - 多泵注射降 PI去抖压力偏差 0.0~50.0kg  新增
	0xA329,   //152 - 多泵注射降 PI去抖流量下限 0~30000rpm 新增
	0xA32A,   //153 - 多泵注射降 PI去抖流量检测时间  0.200~2.000s 新增
	0xA32B,   //154 - 多泵 CAN 通讯模式下从泵不工作的压力偏差  0~50.0kg 新增
	0xA32C,   //155 - 多泵 CAN 通讯模式下从泵不工作的流量下限 -100.0%~100.0% 新增
	0xA32D,   //156 - 从泵无转速指令停机判断时间 0.100~5.000s 新增
	0xA32E,   //157 - 从泵无转速指令停机减速时间 0.001~5.000s 新增
	0xA32F,   //158 - 启动阀门卸压延迟  0.001~5.000s  新增
	0xA330,   //159 - 退出阀门卸压延迟  0.001~5.000s  新增
	0xA331,   //160 - 启动阀门卸压压力偏差下限  0.0~A3-02( 系统油压） 新增
	0xA332,   //161 - 启动阀门卸压压力设定下限  0.0~A3-02( 系统油压） 新增
	0xA333,   //162 - 压力传感器故障检测电流下限 0%~300%（电机额定电流F1-03） 新增
	0xA334,   //163 - 压力传感器故障检测速度上限 0%~100%（最大转速 A3-01） 新增

	0xA400,   //164 - 转速滤波时间  0~5.000S 新增
	0xA401,   //165 - 电流滤波时间  0~5.000S 新增
	0xA402,   //165 - 保留 新增
	0xA405,   //167 - 保留 新增
	0xA406,   //168 - 流量泄漏补偿值 0.0%~50.0% 新增
	0xA407,   //169 - 保留 新增
	0xA408,   //170 - 反转泄压最小压力 0.0kg/cm2 ～ A3-02  新增
	0xA409,   //171 - 反转泄压长时间运行保护时间 0.001s ～ 5.000s 新增
	0xA410,   //172 - 注射超调抑制检测等级  0~2000 新增
	0xA411,   //173 - 注射超调抑制系数  0~3.000s  新增
	0xA412,   //174 - 保留 新增
	0xA413,   //175 - 保留 新增
	0xA414,   //176 - 保留 新增
	0xA415,   //177 - 保留 新增
	0xA416,   //178 - 油压抑制取消油压偏差  0.0kg/cm2 ～ A3-02  新增
	0xA417,   //179 - 积分限制偏差最大值   0.0kg/cm2 ～ A3-02  新增
	0xA418,   //180 - 积分限幅模式选择  0~1  新增
	0xA419,   //181 - 压力环输出上限增幅 0~50.0 新增
	0xA41C,   //182 -
	0xA41D,   //183 -
	0xA41E,   //184 -
	0xA41F,   //185 -
	0xA420,   //186 -

	0xE000,   // 187 - 驱动器控制模式 0:调试
	0xE002,   // 188 - 11转矩上限  0.0---250.0 %

	0x1001,   // 189 - 12运行频率  0.00---最大频率 Hz  状态量
	0x1003,   // 190 - 13输出电压  0.00---电机额定电压 V  状态量
	0x1004,   // 191 - 14输出电流  0.01---655.35 A   状态量
	0x1006,   // 192 - 15输出转矩  0.0%---扭矩上限 %  状态量
	0x100F,   // 193 - 16电机转速  -9999---30000 rmp  状态量

	0x3000,   // 194 - 17驱动器运行状态   状态量
	0x8000,   // 195 - 18驱动器状态   状态量
	0x8001,   // 196 - 19通讯状态  状态量

	0xF70A,   // 197 - 20版本号1  状态量
	0xF70B,   // 198 - 21版本号2  状态量
	0x100B,   // 199 - 检测前零漂电压  0.00---5.00 V
	0xF41C,   // 200 - 检测后零漂电压  0.00---5.00 V

	//新伺服功能增加
	0x1002,   // 201 - 母线电压
	0x1005,   // 202 - 功率
	0x1013,   // 203 - 实时角度
	0x1707,   // 204 - 模块温度

	0xA005,   // 205 - 缺相检测T
	0x1019,   // 206 - 过载累计
	0x1015,    // 207 - 反馈油压
	0x1017   // 208 - 给定流量
};

DB_SERVOTYPT g_dbServotypeDef = {
		0xEB90,       //已使用标志 EB90 wMarkUsed

		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}
};

//驱动器参数重置确认条件
BOOL bDriveParResetConfirm= FALSE;
//界面切换确认条件
BOOL bSwitchPageConfirm= FALSE;

void ServoRecvData(UI8 cCanType, UI8 cType,UI8 kind,UI8 cCanTypex,UI8 nServoId,UI8 htask);

/************************************************************************/
/* private methods   读取伺服类型                                       */
/************************************************************************/
static BOOL LoadServoType()
{
	FILEHD filehd;
	int readbytes;

	filehd = FileOpen(SERVOTYPE_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
		FileRead(filehd,&g_dbsvotype,sizeof(DB_SERVOTYPT),&readbytes);
		FileClose(filehd);
	}
	else {
		filehd = FileCreate(SERVOTYPE_INIT_FILE);
		memcpy(&g_dbsvotype,&g_dbServotypeDef,sizeof(DB_SERVOTYPT));
		FileWrite(filehd,&g_dbsvotype,sizeof(DB_SERVOTYPT),NULL);
		FileClose(filehd);
	}
	return TRUE;
}
/************************************************************************/
/* private methods  保存伺服类型                                        */
/************************************************************************/
void SaveServoType(UI32 dst, void* src, UI16 wCount)
{
	FILEHD filehd;
	int writebytes;

	filehd = FileOpen(SERVOTYPE_INIT_FILE);
	if(FILEHD_IS_OK(filehd))
	{
		FileSeek(filehd, dst);
		FileWrite(filehd, src, wCount, &writebytes);
	}
	FileClose(filehd);
}

void ServoInital(PCMOD pmod)
{
	LoadServoType();
	CanDataRxCbSet(ServoRecvData);
}
/************************************************************************/
/* 伺服数据接收回调函数                                                 */
/************************************************************************/
void ServoRecvData(UI8 cCanType, UI8 cType,UI8 kind,UI8 cCanTypex,UI8 nServoId,UI8 htask)
{
	//servo data operator param
	char sServoMsg[10];
	char sServoMsgDefault[10];
	UI16  wData,wAddr,i;  

	if(cCanType == CAN_PDO_RX2_CODE)
	{
		UI16 nServo,wData1,wData2,wData3;

		nServo= g_TaskCan.cRxBuf[3]&0x07;
		wData1 = (g_TaskCan.cRxBuf[5] <<8 ) | g_TaskCan.cRxBuf[6];
		wData2 =(g_TaskCan.cRxBuf[7] <<8 ) | g_TaskCan.cRxBuf[8];
		wData3 =(g_TaskCan.cRxBuf[9] <<8 ) | g_TaskCan.cRxBuf[10];
		dbsvox[nServo].wData[ID_100F]= wData1;
		dbsvox[nServo].wData[ID_1004]= wData2;
		dbsvox[nServo].wData[ID_1002]= wData3;
		return;
	}

	/*main can test*/
	if(cCanTypex==((CAN_CPU_KIND << 3) | (CPU_ID & 0x07)))
	{
		g_dbsvosx.wFromControl++;
		return;
	}

	if(cCanType == CAN_NMT_CTRL_CODE)
	{
		CreateTaskCan(CAN_TX_IDRESPONSE,(UI8)nServoId,0,0x02);
		return;
	}

	//htask=g_TaskCan.hTask; 
	switch(cType)
	{ 
	case CAN_RX_STATE: 
	case CAN_RX_READ: 
		dbsvox[nServoId].nCount ++;//can test from drive
		wAddr=(g_TaskCan.cRxBuf[7] <<8 ) | g_TaskCan.cRxBuf[8];
		wData=(g_TaskCan.cRxBuf[9] <<8 ) | g_TaskCan.cRxBuf[10];
		//OWARN("wAddr=%X   wData=%d",wAddr,wData);
		//servo data receive
		for(i=0;i<SERVOXADDR_COUNT;i++){
			if (wAddr == ServoxAddr[i])
			{
				dbsvox[nServoId].wData[i]= wData;
				break;
			}
		}
		/* servo import  or save function*/
		if(VarAdrToUI16(SETUP_SERVO_EXPORT))
		{
			snprintf(sServoMsg,sizeof(sServoMsg)-1,"%d-%04X",nServoId,wAddr);
			VarAdrSetStr(SETUP_SERVO_MSG,sServoMsg);
			//oprintf("wAddr=%x\r\n",wAddr);
		}
		else if(VarAdrToUI16(SETUP_SERVO_SAVEDEF))
		{
			snprintf(sServoMsgDefault,sizeof(sServoMsgDefault)-1,"%d-%04X",nServoId,wAddr);
			VarAdrSetStr(SETUP_SERVO_MSGDEF,sServoMsgDefault);
		}

		if(wAddr == 0xA420 && (VarAdrToUI16(SETUP_SERVO_EXPORT)||VarAdrToUI16(SETUP_SERVO_SAVEDEF))) //0xA420是保存数据最后一个，然后判断是否需要获取下一个驱动器点数据，如果不需要就将获取数据结束
		{  
			BOOL bfinish = TRUE;
			int i,j;
			dbsvox[nServoId].fCover = 2;
			for(i = nServoId+1;i<8;i++)
			{
				if(((VarAdrToUI16(SERVO_STATE_CANST1+i-1)&0x80)) && (((VarAdrToUI16(SERVO_STATE_CANST1+i-1)&0x07)  == 3)||((VarAdrToUI16(SERVO_STATE_CANST1+i-1)&0x07)  == 0)))
				{
					bfinish = FALSE;
					for(j=0;j<SERVOXADDR_SAVECOUNT;j++)
					{
						CreateTaskCan(CAN_TX_READ,i,ServoxAddr[j],0);
					}
					break;
				}
			}
			if(bfinish){
				sServoMsg[0] ='\0';
				sServoMsgDefault[0] ='\0';
				VarAdrSetStr(SETUP_SERVO_MSG,"");
				VarAdrSetStr(SETUP_SERVO_MSGDEF,"");
				VarAdrSetInt(SETUP_SERVO_EXPORT,0);
				VarAdrSetInt(SETUP_SERVO_SAVEDEF,0);
			}
		}

		if (cType==CAN_RX_READ)
		{
			//OWARN("wAddr=%X cType=%d  sendaddr=%X  state=%d",wAddr,cType,g_TaskCan.Task[htask].wAddr,g_TaskCan.TxState);
			g_dbsvosx.wFromDriver++;
			if (g_TaskCan.TxState==CAN_TX_SEND)
			{    
				if(wAddr == g_TaskCan.Task[htask].wAddr)
				{
					g_TaskCan.TxState=CAN_TX_SUCCESS; 
				}
			} 
		}
		break;
	case CAN_RX_WRITE: 
		if (g_TaskCan.TxState==CAN_TX_SEND)
		{ 
			if(((g_TaskCan.cRxBuf[7]<<8) | g_TaskCan.cRxBuf[8])== 0x8001 && ((g_TaskCan.cRxBuf[9]<<8) | g_TaskCan.cRxBuf[10])==0x0008)
			{
				htask=g_TaskCan.hTask; 
				g_TaskCan.cTxBuf[0]=8; 
				g_TaskCan.cTxBuf[1]=(CAN_PDO_TX_CODE << 4) | (CAN_SERVO_KIND);//1.mimacuow2,duxieminglingcuowu ,3,jiaoyan4,gongnengmawuxiao,5,wuxiaocanshu ,6,canshuzhidu,7,canshusuoding
				g_TaskCan.cTxBuf[2]=g_TaskCan.Task[htask].nId << 5;
				g_TaskCan.cTxBuf[3]=(CAN_PANEL_KIND << 3) | (PANEL_ID & 0x07);
				g_TaskCan.cTxBuf[4]=0;
				g_TaskCan.cTxBuf[5]=0;
				g_TaskCan.cTxBuf[6]=(UI8)(g_TaskCan.Task[htask].cType);
				g_TaskCan.cTxBuf[7]=(UI8)(g_TaskCan.Task[htask].wAddr>>8);
				g_TaskCan.cTxBuf[8]=(UI8)(g_TaskCan.Task[htask].wAddr);
				g_TaskCan.cTxBuf[9]=(UI8)(g_TaskCan.Task[htask].dwData>>8);
				g_TaskCan.cTxBuf[10]=(UI8)(g_TaskCan.Task[htask].dwData);

				/*sent data */
				CanWrite((PCAN_FRAM)g_TaskCan.cTxBuf);
				//write(fdcan,cTxBuf,11);
			}
			else if (((g_TaskCan.cRxBuf[7]<<8) | g_TaskCan.cRxBuf[8])==g_TaskCan.Task[htask].wAddr) 
			{
				g_TaskCan.TxState=CAN_TX_SUCCESS; 

				if(VarAdrToUI16(SETUP_SERVO_IMPORT) || VarAdrToUI16(SETUP_SERVO_READDEF))
				{
					snprintf(sServoMsg,sizeof(sServoMsg)-1,"%d--%04X",nServoId,g_TaskCan.Task[htask].wAddr);
					VarAdrSetStr(SETUP_SERVO_MSG,sServoMsg);
				}
				if(g_TaskCan.Task[htask].wAddr == 0xA420 && (VarAdrToUI16(SETUP_SERVO_IMPORT)||VarAdrToUI16(SETUP_SERVO_READDEF)))
				{
					BOOL bfinish = TRUE;
					int i,j;
					dbsvox[nServoId].fCover = 0;
					for(i = nServoId+1;i<8;i++)
					{
						if(dbsvox[i].fCover == 1)
						{
							if(((VarAdrToUI16(SERVO_STATE_CANST1+i-1)&0x80)) && (((VarAdrToUI16(SERVO_STATE_CANST1+i-1)&0x07) ==3)||((VarAdrToUI16(SERVO_STATE_CANST1+i-1)&0x07) ==0)))
							{
								bfinish = FALSE;
								CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[ID_A302],0);
								CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[ID_A303],0);
								for(j=0;j<SERVOXADDR_SAVECOUNT;j++)
								{
									CreateTaskCan(CAN_TX_WRITE,i,ServoxAddr[j],dbsvox[i].wData[j]);
								}
								break;
							}
							else{
								dbsvox[i].fCover = 0;
							}
						}
					}
					if(bfinish){
						sServoMsg[0] = '\0';
						sServoMsgDefault[0] = '\0';
						VarAdrSetStr(SETUP_SERVO_MSG,"");
						VarAdrSetStr(SETUP_SERVO_MSGDEF,"");
						VarAdrSetInt(SETUP_SERVO_IMPORT,0);
						VarAdrSetInt(SETUP_SERVO_READDEF,0);
					}
				}
			}
		} 
		break; 
	}   
}


