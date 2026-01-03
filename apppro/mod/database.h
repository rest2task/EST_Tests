#ifndef _DATABASE_H
#define _DATABASE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "typedef.h"
#include "taskmoni.h"
#include "panel.h"
#include "oprecord.h"
#include "prodpower.h"
#include "pagename.h"
#include "dbenum.h" //20200323
//温度图标相关
enum enTEMP_PIC
{
		TEMP_GRAY = 1,
		TEMP_BLUE,
		TEMP_LIGHTYELLOW,
		TEMP_YELLOW,
		TEMP_LIGHTRED,
		TEMP_RED
};
// 
enum enVISIBLE
{
	INVISIBLE=0,	//不可见
	VISIBLE			//可见
};

enum enENABLE
{
	DISABLE=0,	//不可用
	ENABLE			//可用
};

//手动保存数据
typedef struct  ty_MANUALSAVEDATA
{
    UI16 flag;
    UI32 prod_shotcnt; //开模数记录
    UI16 autoalarm_state; //自动警报状态
    UI32 prod_pkgcnt; //本包开模数
    UI32 prod_clamplubric; //自动润滑模数
    UI32 prod_clamplubric2; //自动润滑计数2
    UI32 prod_clamplubric3; //自动润滑计数3
    UI32 cyclemold_curcnt; //周期计数当前模数
    UI32 prod_shotcnt_noclear; //开模数记录-不清零 //20211207 dyl
    UI32 prod_goodcnt; //良品数 //20220923 dyl 增加良品数保存
    UI32 rev[8];
}MANUALSAVEDATA, *pMANUALSAVEDATA;

//20211210 dyl 资源分配：IO输入输出点，AD电子尺，DA输出
#define  NUM_MAINRESOURCE_RECD    20//最大对应组数
typedef struct tyDB_MAINRESOURCE_RECD
{
    UI16    nCtrlModelId;//主机型号
    UI16    nIOInNum;    //IO输入点个数
    UI16    nIOInNum_EX; //IO扩展板输入点个数
    UI16    nIOOutNum;   //IO输出点个数
    UI16    nIOOutNum_Ex;//IO扩展板输出点个数
    UI16    nADNum;      //AD电子尺路数
    UI16    nADNum_Ex;   //AD扩展板电子尺路数
    UI16    nDANum;      //DA路数
    UI16    nDANum_Ex;   //DA扩展板路数
}DB_MAINRESOURCE,*PDB_MAINRESOURCE_RECD;
typedef struct tyMAINRESOURCE_RECD
{
    DB_MAINRESOURCE  MainResourceitems[NUM_MAINRESOURCE_RECD];
}MAINRESOURCE_RECD;

extern MAINRESOURCE_RECD m_mainresource_recd;
extern DB_MAINRESOURCE g_mainresource_current;
/********* END 资源分配：IO输入输出点，AD电子尺，DA输出 *********/

#define MANUAL_SAVE_PATH  SAVE_PATH"manualsave.dat"
void LoadManualSaveData();

#define setone(x,y)      (x)|=(1<<(y))       /*设置某一位为1*/
#define setzero(x,y)     (x)&=~(1<<(y))      /*设置某一位为0*/
#define reversebit(x,y)  (x)^=(1<<(y))       /*某一位取反*/
#define getbit(x,y)      ((x)>>(y)&1)        /*获得某一位值*/

//color define
#define LIGHT_GREEN  0x68a800  //浅绿色
#define DARK_GREEN   0x508000  //深绿色 RGB  BGR
#define ORANGE_RED   0x0042ff  //橙红色

/*************************0x00 机器部件*************************/
#define SYS_FL_MACH_CODE0           0x00120000 // code0
#define SYS_FL_MACH_CODE1           0x00120001 // code1
#define SYS_FL_MACH_CODE2           0x00120002 // code2
#define SYS_FL_MACH_CODE3           0x00120003 // code3
#define SYS_FL_MACH_CODE4           0x00120004 // code4
#define SYS_FL_MACH_CODE5           0x00120005 // code5
#define SYSTEM_MACH_CODE43          0x00120006 // code43
#define SYSTEM_MACH_CODE44          0x00120007 // code44
#define SYS_FL_MACH_CODE48          0x00120008 // code48
#define SYS_FL_MACH_CODE49          0x00120009 // code49  20180912 hz
#define SYS_FL_MACH_CODE50          0x0012000A // code50
#define SYS_FL_MACH_CODE51          0x0012000B // code51  20190505 LET
#define SYS_FL_MACH_CODE52          0x0012000C // code52  20190505 LET
#define SYS_FL_MACH_CODE53          0x0012000D // code53
#define	SYS_FL_MACH_CODE54          0x0012000E // code54
#define SYS_FL_HYDRAUIC             0x0012000F //液压控制
#define SYS_FL_TEMPER               0x00120010 //温度控制选择码
#define SYS_FL_METER                0x00120011 //位置尺控制
#define SYS_MACH_FL_MACHINESAFE     0x00120013 // 机器国家安全标准
#define SYS_MACH_CUSTOMID           0x00120018 //客户及机器代码
#define SYS_MACH_WIO_DATE           0x00120019 // 407  I/O 日期          ;8EH
#define SYS_MACH_WDSR_DATE          0x0012001A // 40b  DSR 日期          ;96H
#define SYS_MACH_FL_PATCHS          0x0012001B // 40c  修改状态          ;98H
#define	SYS_FL_CTRLMODEL            0x0012001C //控制器型号A320/A620/A920/2100/3100
#define	SYS_FL_VERSION              0x0012001D //程序版本
#define	SYS_FL_CTRLDATE             0x0012001E //程序日期
#define SYS_FL_ROTLAC               0x0012001F //滑模使用选择 //20190524 CHZ
#define SYS_MACH_DAPRES_SYSLMT      0x00120021 // 428  系统压力上限      ;D0H
#define SYS_MACH_DAPRES_CLSHILMT    0x00120022 // 429  设定压力上限      ;D2H
#define	SYS_CN_CHGRPMHOLE           0x00120023 //储料RPM孔数
#define	SYS_FL_MACHINEMODEL         0x00120026 // 42F  机器型号（推力座）
#define SYS_DA_TYPE                 0x00120039 // 426  保留 DA线性类型

#define SYS_QC_FUNC                 0x0013000d //QC监控功能

#define	SYS_STATE_FL_AUTOALARM      0x00160004 //自动警报状态
#define POWER_CYCLE                 0x0016000c //本次循环总能耗

/*************************0x04 马达部件*************************/
#define	MOTOR_STATE_STATE           0x04160002 //马达状态

/*************************0x05 温度部件*************************/
#define ADDR_TEMP_RAMP1				0x0512000C //温度加温缓冲区1 1-16
#define ADDR_TEMP_RAMP4				0x0512000F //温度加温缓冲区4 1-16
#define ADDR_TEMP_RAMP5				0x05120010 //温度加温缓冲区5 1-16
#define ADDR_TEMP_RAMP6				0x05120011 //温度加温缓冲区6 1-16
#define ADDR_TEMP_RAMP7				0x05120012 //温度加温缓冲区7 1-16
#define ADDR_TEMP_RAMP8				0x05120013 //温度加温缓冲区8 1-16
#define ADDR_TEMP_RAMP9				0x05120014 //温度加温缓冲区9 1-16
#define ADDR_TEMP_RAMP10			0x05120015 //温度加温缓冲区10 1-16
#define ADDR_TEMP_RAMP11			0x05120016 //温度加温缓冲区11 1-16
#define ADDR_TEMP_RAMP12			0x05120017 //温度加温缓冲区12 1-16
#define ADDR_TEMP_RAMP13			0x05120018 //温度加温缓冲区13 1-16
#define ADDR_TEMP_RAMP14			0x05120019 //温度加温缓冲区14 1-16
#define ADDR_TEMP_RAMP16            0x0512001B //温度加温缓冲区16
#define TEMP_DAPID_KP_1             0x05120030 //温度#1段优化 KP
#define TEMP_DAPID_TC_16            0x0512006F //温度#16段优化 TC

#define ADDR_TEMP_STATUS1			0x05160012 //温度加温状态1
#define ADDR_TEMP_STATUS2			0x05160013 //温度加温状态2
#define ADDR_TEMP_STATUS3			0x05160014 //温度加温状态3
#define ADDR_TEMP_STATUS4			0x05160015 //温度加温状态4
#define ADDR_TEMP_STATUS5			0x05160016 //温度加温状态5
#define ADDR_TEMP_STATUS6			0x05160017 //温度加温状态6
#define ADDR_TEMP_STATUS7			0x05160018 //温度加温状态7
#define ADDR_TEMP_STATUS8			0x05160019 //温度加温状态8
#define ADDR_TEMP_STATUS9			0x0516001a //温度加温状态9
#define ADDR_TEMP_STATUS10			0x0516001b //温度加温状态10
#define ADDR_TEMP_STATUS11			0x0516001c //温度加温状态11
#define ADDR_TEMP_STATUS12			0x0516001d //温度加温状态12
#define ADDR_TEMP_STATUS13			0x0516001e //温度加温状态13
#define ADDR_TEMP_STATUS14			0x0516001f //温度加温状态14
#define TEMP_FL_HEATERSTATUS        0x0516002B //电热状态
#define DATEMP_WORKTM1              0x05160081 //0x05160081 -  0x05160090 60s内电热输出时间
#define DATEMP_YOUHUAOK             0x05160096 //温度优化开启与完成标记 0未优化 1普通优化 2强制优化 3普通优化完成 4强制优化完成

/*************************0x07 电子尺部件*************************/
#define ADMETER_PTMSETLEN4          0x07120003 //第四路电子尺行程
#define ADMETER_PTMSETLEN6          0x07120005 //第六路电子尺行程
#define ADMETER_MACHINELEN4         0x0712000B //第四路机械行程
#define ADMETER_MACHINELEN6         0x0712000D //第六路机械行程
#define ADMETER_ZSET1               0x07120010 //位置尺归零位置0
#define ADMETER_ZSET2               0x07120011 //位置尺归零位置1
#define ADMETER_ZSET3               0x07120012 //位置尺归零位置2
#define ADMETER_ZSET4               0x07120013 //位置尺归零位置3
#define ADMETER_ZSET5               0x07120014 //位置尺归零位置4
#define ADMETER_ZSET6               0x07120015 //位置尺归零位置5
#define ADMETER_ZSET7               0x07120016 //位置尺归零位置6
#define ADMETER_ZSET8               0x07120017 //位置尺归零位置7

#define ADDR_ADPOSI_REAL1           0x07160002 //电子尺相对位置 射出
#define ADDR_ADPOSI_REAL2           0x07160003 //电子尺相对位置 开关模
#define ADDR_ADPOSI_REAL3           0x07160004 //托模
#define ADMETER_ADPOSI_AD4          0x07160005 //座台AD4
#define ADMETER_ADPOSI_AD5          0x07160006 //AD5
#define ADMETER_ADPOSI_AD6          0x07160007 //AD6
#define ADMETER_ABSOLUTE1           0x0716000A //位置尺绝对位置0
#define ADMETER_ABSOLUTE2           0x0716000B //位置尺绝对位置1
#define ADMETER_ABSOLUTE3           0x0716000C //位置尺绝对位置2
#define ADMETER_ABSOLUTE4           0x0716000D //位置尺绝对位置3
#define ADMETER_ABSOLUTE5           0x0716000E //位置尺绝对位置4
#define ADMETER_ABSOLUTE6           0x0716000F //位置尺绝对位置5
#define ADMETER_ABSOLUTE7           0x07160010 //位置尺绝对位置6
#define ADMETER_ABSOLUTE8           0x07160011 //位置尺绝对位置7
#define ADDR_MAX_ADINJECT           0x07160012 //射出电子尺最大值
#define ADDR_MAX_ADCLAMP            0x07160013 //开关模电子尺最大值
#define ADDR_MAX_ADEJECT            0x07160014 //托模电子尺最大值

/*************************0x08 DA部件*************************/
#define DA_STATE_TEMPMAX1           0x08160002 //DA校准时使用的第一组压力最大值。
#define DA_STATE_TEMPMAX2           0x08160003 //DA校准时使用的第一组速度最大值。
#define DA_STATE_TEMPMAX3           0x08160004 //DA校准时使用的第二组压力最大值。
#define DA_STATE_TEMPMAX4           0x08160005 //DA校准时使用的第二组速度最大值。
#define DA_STATE_TEMPMAX5           0x08160006 //DA校准时使用的第三组压力最大值。
#define DA_STATE_TEMPMAX6           0x08160007 //DA校准时使用的第三组速度最大值。
#define DA_STATE_TEMPMAX7           0x08160008 //DA校准时使用的第四组压力最大值。
#define DA_STATE_TEMPMAX8           0x08160009 //DA校准时使用的第四组速度最大值。
#define DA_STATE_TEMPMIN1           0x08160012 //DA校准时使用的第一组压力最小值。
#define DA_STATE_TEMPMIN2           0x08160013 //DA校准时使用的第一组速度最小值。
#define DA_STATE_TEMPMIN3           0x08160014 //DA校准时使用的第二组压力最小值。
#define DA_STATE_TEMPMIN4           0x08160015 //DA校准时使用的第二组速度最小值。
#define DA_STATE_TEMPMIN5           0x08160016 //DA校准时使用的第三组压力最小值。
#define DA_STATE_TEMPMIN6           0x08160017 //DA校准时使用的第三组速度最小值。
#define DA_STATE_TEMPMIN7           0x08160018 //DA校准时使用的第四组压力最小值。
#define DA_STATE_TEMPMIN8           0x08160019 //DA校准时使用的第四组速度最小值。

/*************************0x09 液压部件*************************/
#define DA_PRSOUT0_1                0x09140000 //DA压力曲线第一组输出值
#define DA_FLOWOUT0_1               0x09140019 //DA速度曲线第一组输出值
#define DA_PRSOUT0_2                0x09140024 //DA压力曲线第二组输出值
#define DA_FLOWOUT0_2               0x0914003d //DA速度曲线第二组输出值
#define DA_PRSOUT0_3                0x09140048 //DA压力曲线第三组输出值
#define DA_FLOWOUT0_3               0x09140061 //DA速度曲线第三组输出值
#define DA_PRSOUT0_4                0x0914006c //DA压力曲线第四组输出值
#define DA_FLOWOUT0_4               0x09140085 //DA速度曲线第四组输出值

/*************************0x0A 伺服部件*************************/
#define SERVO_STATE_SPEED1          0x0A160002 //伺服驱动实际转速1
#define	SERVO_STATE_CANST1          0x0A160012 //伺服CAN通信状态1
#define SERVO_STATE_ERR1            0x0A16001A //伺服警报1
#define	SERVO_STATE_CURRENT1        0x0A160022 //伺服电流1
#define SERVO_STATE_POWER1          0x0A16002A //伺服功率1
#define	SERVO_STATE_TEMP1           0x0A16003A //驱动器模块温度1
#define SERVO_STATE_PRESS1          0x0A160042 //驱动器反馈压力1

/*************************0x10 射出部件*************************/
#define INJECT_TMDLY_VLV_OFFINJEND	0x10120024 //射出结束阀延迟关计时
#define INJECT_TMDLY_INJEND         0x10120029 //射出完延迟计时

#define INJECT_FL_TURNTOHOLD        0x10130009 //保压转换方式 0:压力/时间 1:压力 2:速度 3:时间 4:位置
#define INJECT_SEC_INJ              0x10130017 //射出使用段数
#define INJECT_SEC_HOLD             0x10130019 //保压使用段数
#define INJECT_MOLD_FL_SUCK_SOOMTH  0x10130047 //射退平稳

#define INJECT_TM_INJ1              0x10140028 //射出一段计时
#define INJECT_TM_INJ2              0x10140029 //射出二段计时
#define INJECT_TM_INJ3              0x1014002A //射出三段计时
#define INJECT_TM_INJ4              0x1014002B //射出四段计时
#define INJECT_TM_INJ5              0x1014002C //射出五段计时
#define INJECT_TM_INJ6              0x1014002D //射出六段计时
#define INJECT_TM_HOLD1             0x10140032 //保压一段计时
#define INJECT_TM_HOLD2             0x10140033 //保压二段计时
#define INJECT_TM_HOLD3             0x10140034 //保压三段计时
#define INJECT_TM_HOLD4             0x10140035 //保压四段计时
#define INJECT_TM_HOLD5             0x10140036 //保压五段计时
#define INJECT_TM_HOLD6             0x10140037 //保压六段计时
#define INJECT_DAPRES_CHG1          0x10140046 //储料一段压力
#define INJECT_DAFLOW_CHG1          0x10140050 //储料一段流量

#define INJECT_PUMPS_CHARGE         0x1015000C //储料动作的比例流量限制:位操作-->P1……P7
#define INJECT_PORT_CHRGBACK        0x10150015 //储料背压的DA输出通道：位操作-->port1……port4

#define INJECT_CN_CHRGSPD           0x1016000C //储料转速

/*************************0x11 座台部件*************************/
#define	NOZZLE_PRES_NOZRET          0x11140004 //座台退压力
#define	NOZZLE_FLOW_NOZRET          0x11140005 //座台退流量
#define FL_OILHEAT                  0x1114000f //油温预热功能

/*************************0x20 开关模部件*************************/
#define CLAMP_MACHSET_EFFCPR        0x20120028 //开模位置有效区
#define CLAMP_MACHSET_FL_ADPOSROBOT 0x20120030 //开模途中机械手选择
#define CLAMP_MACHSET_TM_MANUALOUT  0x20120035 //手动输出时间

#define	CLAMP_MOLD_MOLDREACHCHK     0x2013000A //计数方式
#define CLAMP_MOLD_PRODUCTSETTING1  0x2013000B //开模数设定一
#define CLAMP_MOLD_PRODUCTSETTING2  0x2013000C //开模数设定二
#define CLAMP_MOLD_FL_CLSE_SMOOTH   0x2013004A //关模平稳
#define FL_CLSOPEN_MODE             0x20130060 //开关模模式  0标准 1平顺 2比例阀

#define ADPOSI_CLOSE1               0x2014000A //关模一段终止位置
#define ADPOSI_CLOSE3               0x2014000C //关模三段终止位置
#define ADPOSI_CLOSELOW             0x2014000D //关模低压终止位置
#define ADPOSI_OPEN1                0x20140018 //开模一段终止位置
#define ADPOSI_OPEN2                0x20140019 //开模二段终止位置
#define ADPOSI_OPEN5                0x2014001C //开模五段终止位置
#define ADPOSI_CLOSEHIGH            0x2014001F //关模高压锁模位置
#define ADPOS_OPENSPD1              0x20140025 //快速开模启动位置
#define ADPOS_OPENSPD3              0x20140029 //快速开模结束位置

#define CLAMP_STATE_MOLDOPNNUM0     0x20160002 //开模数一
#define CLAMP_STATE_MOLDOPNNUM1     0x20160003 //开模数二
#define CLAMP_STATE_CURPACKMOLD     0x20160004 //本包装良品模数
#define CLAMP_STATE_VALIDMOLD       0x20160005 //良品模数
#define CLAMP_STATE_CLAMPLUBRIC     0x20160006 //自动润滑计数

/*************************0x21 托模部件*************************/
#define EJECT1_MOLD_FL_EJEF_SMOOTH  0x21130021 //托进平稳

/*************************0x23 吹气部件*************************/
#define	FL_BLAST1                   0x23130000 //吹气1方式
#define ADPOSI_BLASTSTART1          0x23130001 //吹气1动作位置

#define	FL_BLAST2                   0x23230000 //吹气2方式
#define ADPOSI_BLASTSTART2          0x23230001 //吹气2动作位置

#define	FL_BLAST3                   0x23330000
#define ADPOSI_BLASTSTART3          0x23330001

#define	FL_BLAST4                   0x23430000
#define ADPOSI_BLASTSTART4          0x23430001

#define	FL_BLAST5                   0x23530000
#define ADPOSI_BLASTSTART5          0x23530001

#define	FL_BLAST6                   0x23630000
#define ADPOSI_BLASTSTART6          0x23630001

#define	FL_BLAST7                   0x23730000
#define ADPOSI_BLASTSTART7          0x23730001

#define	FL_BLAST8                   0x23830000
#define ADPOSI_BLASTSTART8          0x23830001

#define	FL_BLAST9                   0x23930000
#define ADPOSI_BLASTSTART9          0x23930001

#define	FL_BLAST10                  0x23a30000
#define ADPOSI_BLASTSTART10         0x23a30001

#define	FL_BLAST11                  0x23b30000
#define ADPOSI_BLASTSTART11         0x23b30001

#define	FL_BLAST12                  0x23c30000
#define ADPOSI_BLASTSTART12         0x23c30001

#define	FL_BLAST13                  0x23d30000
#define ADPOSI_BLASTSTART13         0x23d30001

#define	FL_BLAST14                  0x23e30000
#define ADPOSI_BLASTSTART14         0x23e30001

/*************************0x24 调模部件*************************/
#define ADPOSI_ADJCUR               0x24120006 //调模当前位置
#define ADJUST_MACH_ADPOSI_ADJTARG  0x2412000b //调模更换位置

/*************************0x30 润滑部件*************************/
#define LUB1_STATE_CN_CLAMPLUBRIC   0x30160006 //自动润滑模数

#define LUB2_STATE_CN_CLAMPLUBRIC   0x30260006 //自动润滑模数

#define LUB3_STATE_CN_CLAMPLUBRIC   0x30360006 //自动润滑模数

/*************************0x31 机械手部件*************************/
#define ROBOT_FL_ROBOTCLPMID        0x31130004 //开模途中机械手 预留给面板使用，下位机不可占用该地址
#define ROBOT_TM_ROBOTMID           0x31130005 //开模途中机械手时间 预留给面板使用，下位机不可占用该地址

/*************************0x32 电眼部件与面板自用地址*************************/
#define SYSTEM_DAPRES_BACKLIMIT		0x32130009 //储料背压压力上限
#define DATEMP_ADJUST1              0x3213000e //温度校正系数
#define DATEMP_ADJUST2              0x3213000f //温度校正系数
#define DATEMP_ADJUST3              0x32130010 //温度校正系数
#define DATEMP_ADJUST4              0x32130011 //温度校正系数
#define DATEMP_ADJUST5              0x32130012 //温度校正系数
#define DATEMP_ADJUST6              0x32130013 //温度校正系数
#define DATEMP_ADJUST7              0x32130014 //温度校正系数
#define DATEMP_ADJUST8              0x32130015 //温度校正系数
#define DATEMP_ADJUST9              0x32130016 //温度校正系数
#define DATEMP_ADJUST10             0x32130017 //温度校正系数
#define DATEMP_ADJUST11             0x32130018 //温度校正系数
#define DATEMP_ADJUST12             0x32130019 //温度校正系数
#define DATEMP_ADJUST13             0x3213001a //温度校正系数
#define DATEMP_ADJUST14             0x3213001b //温度校正系数
#define SYSTEM_MACH_ADPOSI_ADJTARG  0x32130037 //调模更换位置

/*************************0x33 面板地址 不发送给下位机*************************/
#define PROD_PERCNT                 0x33120017 //每模产品数
#define PROD_SHOTCNT                0x33120018 //开模记录数
#define	PROD_TARGETCNT              0x33120019 //开模设定值
#define PROD_PKGCNT                 0x3312001a //本包开模数
#define INJECT_CUREMODE             0x3312001e //射出曲线模式
#define LOGIN_PWD                   0x3312001F //登录框密码
#define LANGE_SELECT                0x33120022 //语言选择保存
#define AUTOALARM_STATE             0x33120023 //自动警报状态
#define PROD_GOODCNT                0x33120024 //良品数
#define	PROD_CLAMPLUBRIC            0x33120025 //自动润滑模数
#define MDTEMP_STATE1               0x33120026 //模温加热状态1
#define MDTEMP_STATE2               0x33120027 //模温加热状态2
#define MDTEMP_VERSION1             0x3312002d //模温版本号1
#define MDTEMP_VERSION2             0x3312002e //模温版本号2
#define MDTEMP_STATEX1              0x33120034 //模温在线状态1
#define MDTEMP_STATEX2              0x33120035 //模温在线状态2
#define SETUP_SERVO_EXPORT          0x3312003b //伺服数据导出
#define SETUP_SERVO_IMPORT          0x3312003c //伺服数据导入
#define SETUP_SERVO_SAVEDEF         0x3312003d //保存位出厂设置标志
#define SETUP_SERVO_READDEF         0x3312003e //读取位出厂设置标志
#define	SETUP_SERVO_MSG             0x3312003f //伺服地址信息
#define	SETUP_SERVO_MSGDEF          0x33120040 //伺服地址默认信息
#define SAFETY_DATAPROTECT          0x33120048 //数据保护
#define SAFETY_VIEWPROTECT          0x33120049 //画面显示保护
#define SAFETY_PARAMHIDE            0x3312004a //参数画面隐藏
#define SAFETY_PWDPROMPT            0x3312004b //密码提示框功能
#define	SYSTEMINDEX_ADDR            0x3312004C //系统类型名称索引
#define	SYSTEM_ADDR                 0x3312004d //系统类型名称
#define DAADJUST_ADJMOD             0x33120050 //DA调整模式：0:调整 1-30:调整中
#define SYSCFG_ACTIVETIME           0x33120053 //屏保时间
#define SYSCFG_DISMODE              0x33120054 //显示方式 20190326 hz
#define DATA_SOFTWARE_LOCK          0x33120059 //资料锁软件锁
#define USB_VISIBLE                 0x3312005a //USB是否使用
#define AD_CLAMP_ABSOUTE            0x33120061 //开关模绝对位置
#define PANEL_TYPE_FLAG             0x33120062 //面板类型标记
#define UDISK_WR_PRIVILEGE          0x33120063 //U盘导入导出权限等级
#define UDIKS_READ_FLAG             0x33120064 //U盘读写成功标记
#define DAADJ_CHANGE_FLAG           0x33120065 //DA调整页面数据修改标记
#define MACHCFG_CHANGE_FLAG         0x33120066 //机器配置页面数据修改标记
#define MACHCFGX_CHANGE_FLAG        0x33120067 //机型选择页面数据修改标记
#define SYSPAR_BLOW14_FUNC          0x33120068 //是否使用7组吹气功能
#define DATA_PROTECT_PASSWORD       0x33120069 //数据保护页面密码
#define FLCOOLTIMEPROT              0x3312006A //模具冷却是否保护
#define FLCLAMPSPDPROT              0x3312006B //开关模速度上限
#define FLHIPRSPRSPROT              0x3312006C //关模高压压力上限
#define FLHIPRSSPDPROT              0x3312006D //关模高压速度上限
#define TEMP_CURVE                  0x33120071 //温度曲线
#define SWCYCLEMOLDCNT              0x33120072 //周期计数模数
#define SWCYCLEMOLDCURRENTCNT       0x33120073 //周期计数当前模数
#define POWER_ON_MOLD               0x33120074 //每次上电开始记录开模数
#define ADEX_CHANGE_FLAG            0x33120085 //AD扩展页面数据修改标记
#define ADX_CHANGE_FLAG             0x33120086 //ADX页面数据修改标记
#define MACHCFG5200_CHANGE_FLAG     0x33120087 //5200机器配置页面数据修改标记
#define KEY_LAST_FOCUS              0x33120088 //换页画面聚焦控件索引值
#define SYSPARA_BACK                0x3312008A //系统参数返回上一层标记
#define MT_BOARD_COUNT              0x33120092 //用户自定义使用模温板的数量 //20200407HJM 模温功能
#define MT_OPT_SELECT               0x33120093 //模温优化选择            //20200407HJM 模温功能
#define	PROD_CLAMPLUBRIC2           0x33120094 //自动润滑模数2 //20200410
#define	PROD_CLAMPLUBRIC3           0x33120095 //自动润滑模数3 //20200410

#define SERVO_MOTORBRAND            0x331300d1 //电机品牌
#define SERVO_CURRENT_NUM           0x331300E8 //3200判断伺服个数

#define ACT_ACTNAME1                0x33160000 //动作名称1
#define ACT_SETTIME1                0x33160001 //设定时间1
#define ACT_ACTTIME1                0x33160002 //动作时间1
#define ACT_ACTNAME2                0x33160003 //动作名称2
#define ACT_SETTIME2                0x33160004 //设定时间2
#define ACT_ACTTIME2                0x33160005 //动作时间2
#define ACT_ACTNAME3                0x33160006 //动作名称3
#define ACT_SETTIME3                0x33160007 //设定时间3
#define ACT_ACTTIME3                0x33160008 //动作时间3
#define ALARM_NO1                   0x33160009 //警报1编号
#define PROD_TOTAL_SHOTCNT          0x3316000C //开模实际总数
#define PROD_TOTALPRODCNT           0x3316000d //产品总数
#define OPMODE_INDEX                0x33160013 //Operation mode index
#define KEYTEST_ADDR                0x33160018 //按键测试标志
#define PROG_CYCTIME                0x33160019 //程序运行起始时间
#define REBOOT_PANEL                0x3316001A //面板重启标志
#define PROGUPDATE_FLAG             0x3316001b //程序更新标记
#define ACT_STATE                   0x3316001c //全自动过程中的动作状态，用于开关模部分的动画显示
#define TEMP_CTRL_PART              0x33160033 //温度控制选择部件
#define ACT_ACTNAME4                0x33160034 //动作名称4
#define ACT_SETTIME4                0x33160035 //设定时间4
#define ACT_ACTTIME4                0x33160036 //动作时间4
#define MT_CTRL_PART                0x33160037 //模具温度控制选择         //20200407HJM 模温功能
#define PART_REQ_STATE              0x33160039 //部件（温度、伺服、模温...）请求是否存在，按位判断

#define USER_KEYLOCK                0x3317004c //生产人员等级锁  //20190524.cyx
#define USER_STATE                  0x3317004d //自动登出刷新标志位  //20190524.cyx
#define CURVE_CHECKTIME             0x3317005e //曲线采样时间       //20190612.cyx
#define LOCK_MSG_SHOW               0x331700AC //锁住后弹出框显示    //20190628.cyx
#define PT15_SCREEN_PROTECT         0x331700AD //是否处于屏保状态    //20190629.cyx
#define PT15_WAKE_SCREEN_SHOW       0x331700AE //唤醒屏保显示       //20190629.cyx
#define HARDWARE_LOCK               0x331700B2 //硬件锁            //20190705.cyx

/************************************************************************/
/* 特殊面板键值定义                                                        */
/************************************************************************/
#define _KEY_VIEW_USER        0x1311 //主画面用户按键的键值，用于跳转到用户管理界面
#define _KEY_SEARCH_VIEW      0x1312 //主画面页面跳转的键值，用于跳转到索引到的页面值 //20191022.cyx

/************************************************************************/
/* 控件属性定义                                                           */
/************************************************************************/
#define  EDIT_TEXT_NAME  "text"  //属性名称text
#define  DROP_VALUE_NAME "value"  //属性名称value

/************************************************************************/
/* charge act step                                                      */
/************************************************************************/
//#define  CHARGE_ACTSETP      (((VarAdrToInt(ACT_ACTNAME1)>=0x10120000 && VarAdrToInt(ACT_ACTNAME1)<=0x1012FE01) || (VarAdrToInt(ACT_ACTNAME1)>=0x10160000 && VarAdrToInt(ACT_ACTNAME1)<=0x10181000))|| ((VarAdrToInt(ACT_ACTNAME2)>=0x10120000 && VarAdrToInt(ACT_ACTNAME2)<=0x1012FE01) || (VarAdrToInt(ACT_ACTNAME2)>=0x10160000 && VarAdrToInt(ACT_ACTNAME2)<=0x10181000)))

/************************************************************************/
/* 控制器类型     20200323                                                */
/************************************************************************/
#define  A320                 (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA320)   /*A320*/
#define  A620                 (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA620)   /*A620*/
#define  A920                 (VarAdrToInt(SYS_FL_CTRLMODEL) == 0xA920)   /*A920*/
#define  IV3100               (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3100)   /*3100*/
#define  IV5000               (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5000)   /*5000*/
#define  IV3200               (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3200)   /*3200*/
#define  IV3300               (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x3300)   /*3300*/
#define  IV5200               (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5200)   /*5200*/
#define  IV5300               (VarAdrToInt(SYS_FL_CTRLMODEL) == 0x5300)   /*5300*/

/************************************************************************/
/* 页面保护                                                              */
/************************************************************************/
//20230518 chj增加射退动作判断
static PPAGE_FRAM pcur;
#define  PAGE_PROTECT  \
	pcur = PanelCurPage();\
    if((OperateModeIndex() !=MODE_MANUAL) || JudgeChargeActStep() || JudgeSuckBackActStep()) { \
        pcur->bprotect=TRUE;    \
	}\
	else \
   {\
		pcur->bprotect=FALSE; \
	}\

//20190524.cyx 用户权限管理:生产人员lock
#define  PAGE_SHOW_LOCK \
    if(VarAdrToInt(USER_KEYLOCK))\
    {\
        PageFramSetProtect(TRUE);\
    }\
    else\
    {\
        PageFramSetProtect(FALSE);\
    }\

/*send state data of mcu to init*/
void DataBaseInit();

/*send part state data to mcu*/
void SendPartSateData();

/*request cfg data  eg.machine cfg */ 
void RequestCfgData();

/*language set init */
void LanguageSetInit();

/* view data protect */
UI16 IsDataProtect();

/* page protect */
UI16 IsViewProtect();

/* param page hide */
UI16 IsParamViewHide();

/* password prompt function */
UI16 IsPwdPrompt();

/*open mold cnt save */
void ShotCntSave();

/* is G15 Type*/
BOOL IsG15Type();

/******
 * 是否是触摸屏
 * //20220104 dyl
 * *****/
BOOL IsTouchType();

/******
 * 是否是带数字按键板面板
 * //20220104 dyl
 * *****/
BOOL IsHaveKeyBoard();

/* is Use Mold Func*/
BOOL isUseMoldFunc();

BOOL JudgeActStep(UI32 actstep1, UI32 actstep2);
BOOL JudgeChargeActStep();
BOOL JudgeSuckBackActStep();//20230518 chj
BOOL JudgeAdjMoldAdvActStep();//20230518 chj增加调模动作判断
BOOL JudgeAdjMoldRetActStep();

void ResourceUse();//20211210 dyl 资源分配：IO输入输出点，AD电子尺，DA输出
BOOL ResourceLoad();//20211210 dyl 资源分配：IO输入输出点，AD电子尺，DA输出
int GetADMaxNumber();//20220718 dyl 当3101/3202等使用6路电子尺时的电子尺通道选择

//20210914 dyl touch
BOOL IsScreenChecking();
void SetValOfScreenCheck(BOOL bCheck);

#ifdef __cplusplus
}
#endif

#endif




