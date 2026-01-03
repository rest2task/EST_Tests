#ifndef _PRIVDEF_h
#define _PRIVDEF_h

#ifdef __cplusplus
extern "C"
{ 
#endif
#include "typedef.h"
#include "obj.h"
#include "dbenum.h"

#define     GTYPE_OR_PTYPE //画面风格是G或者P系列

//text_table
enum TEXTTBL_INDEX{
	//servo state and main adj
	TEXT_TESTBEGIN = 0,			//Test Begin  开始测试
	TEXT_TESTEND,				//Test End    结束测试
    //servo
	TEXT_STARTLEARN,		    //Start Self-Learn 开始自学习
	TEXT_STOPLEARN,		        //Stop Self-Learn  停止自学习
    //servo curve
	TEXT_START,		            //Start 开始
	TEXT_STOP,		            //Stop  停止
    //da adjust
	TEXT_ADJUST,		        //Adj. 调整
	TEXT_ADJUSTING,		        //Adj.ing  调整中

	//clamp
	TEXT_CLAMP_START = 8,		//Start 启动
	TEXT_CLAMP_1		,		//#1  1段
	TEXT_CLAMP_FAST		,		// Fast 快速
	TEXT_CLAMP_4		,		//#4 4段
	TEXT_CLAMP_END		,		//END	终止
	TEXT_CLAMP_5		,		//#5 5段
	TEXT_CLAMP_CLS2		,		//#2	2段
	TEXT_FAST3SPD		,		//Fast 3 Spd Ramp 快速3速度斜率
	TEXT_FASTSPD		,		//Fast Spd Ramp 快速速度斜率

	//progupdate
	TEXT_UPDATE = 17    ,		//Update 更新
	TEXT_UPDATING       ,		//Updating 更新中

	TEXT_SAVE           ,       //Save   保存
	TEXT_OK             ,       //OK     确定

	//machcfg ctrl
	TEXT_EXPORTCFG=23   ,       //Exporting,Please Waiting 导出配置中，请等待...
	TEXT_ERRCODE        ,       //Code Error 代码错误
	TEXT_EXPORTSUCCESS  ,       //Export Success 配置导出成功
	TEXT_TIMEOUT        ,       //Timeout,replace the configure code 导出超时失败，请先更换配置代码

	//eject/eject func
	TEXT_GROUP1 =30     ,        //Group1 第一组
	TEXT_GROUP2         ,        //Group2 第二组
	TEXT_GROUP3         ,        //Group3 第三组
	TEXT_GROUP4         ,        //Group4 第四组
	TEXT_GROUP5         ,        //Group5 第五组
	TEXT_GROUP6         ,        //Group6 第六组
	TEXT_GROUP7         ,        //Group7 第七组

	//charge
	TEXT_DISTANCE =40   ,        //Distance 距离
	TEXT_ENDPOS         ,        //EndPos 终止位置
	TEXT_POS            ,        //Pos 位置
    TEXT_CLAMP_NOSPC    ,        //#5 5段 开模终止没有空格
    //Temp Optimizing
    TEXT_OPTIMIZE_START ,        //Start Optimize   开启优化
    TEXT_OPTIMIZE_ING   ,        //Optimizing       优化中
    TEXT_FORCE_OPTIMIZE_START,   //Start Force Optimize   开启强制优化
    TEXT_FORCE_OPTIMIZE_ING,     //Force Optimizing        强制优化中
    TEXT_VOLTAGE_ADJ,            //VoltageAdj     电压调整
    TEXT_CURRENT_ADJ,            //CurrentAdj     电流调整
    TEXT_VOLTAGE,                //Voltage   电压
    TEXT_CURRENT,                //Current   电流
    TEXT_USERSUCCESS = 52,       //Login Succeed!登录成功！ //20191118.cyx
    TEXT_USERERROR,              //Password Error!密码错误！
    TEXT_PWDSUCCESS,             //Modify Succeed!已修改完成！
    TEXT_PWDNOTMATCH,            //Password Unmatch!密码不匹配！
    TEXT_PRESS_ADJUST,           //Press Adjust 压力校准
    TEXT_ANTI_LOGIC,             //ANTILg 反逻辑
    TEXT_ANTI_LOGIC2,            //ANTI 反
    TEXT_INPUT,                  //Input 输入
    TEXT_OUTPUT,                 //Output 输出
    TEXT_DRIVER = 61,            //Driver 驱动
    TEXT_HEATOFF,                //Heater Off: 热电偶断线:
    TEXT_HEATREVERSE,            //Heater Reverse: 热电偶接反:
    TEXT_HEATREADFAIL,           //Temp Read Fail: 温度读取失败:
    TEXT_HEATALARM999,           //Off 断开
    TEXT_HEATALARM777,           //Reverse 接反
    TEXT_HEATALARM788,           //Fail 读失败
    TEXT_DOWN,                   //DOWN 按下
    TEXT_UP,                     //UP 弹起
    TEXT_MSG_CALIBRATE,          //s Later,Reboot Panel and Calibrate!  秒后，重启面板并进行触摸屏校准！

    //proportvlv
    TEXT_PROP_START = 71,        //Self-Turning Start 自整定开始
    TEXT_PROP_STOP,              //Self-Turning Cancel 自整定取消
    TEXT_PROP_FINISHED,          //Self-Turning Finish 自整定完成
    TEXT_PROP_UNFINISHED,        //Self-Turning Unfinished 自整定未完成

    TEXT_CODE_TIME = 80,         //Cold Start Left Time 冷态启动剩余时间
    TEXT_LEFT_TIME,              //Left PreHeat Tm 剩余预温计时
    TEXT_AD_NOZZLE,              //Nozzle 座台
    TEXT_AD_ADHYDR1,             //Hydr.1 液压1
    TEXT_AD_ADHYDR2,             //Hydr.2 液压2
    TEXT_AD_EXAD1,               //EXAD1 AD扩展1
    TEXT_AD_EXAD2,               //EXAD2 AD扩展2
    TEXT_AD_EXAD3,               //EXAD3 AD扩展3
    TEXT_AD_EXAD4,               //EXAD4 AD扩展4

    TEXT_CLAMP_3 = 89,           //#3  3段
    TEXT_CLOSE,                  //Close 关模
    TEXT_CLOSE_1,                //Close #1 关模一段
    TEXT_CLOSE_2,                //Close #2 关模二段
    TEXT_CLOSE_3,                //Close #3 关模三段
    TEXT_CLOSE_FAST,             //Cls Fast 关模快速
    TEXT_CLOSE_LOWPRS,           //Close LowPrs 关模低压
    TEXT_OPEN_START,             //Open Start 开模启动
    TEXT_BREAKMOLD,              //BreakMold 破模
    TEXT_OPEN_1,                 //Open #1 开模一段
    TEXT_OPEN_2,                 //Open #2 开模二段
    TEXT_OPEN_3,                 //Open #3 开模三段
    TEXT_OPEN_4,                 //Open #4 开模四段
    TEXT_OPEN_END,               //Open End 开模终止
    TEXT_OPEN_FAST,              //Open Fast 开模快速
    TEXT_OPENMOLD,               //OpenMold 开模
    TEXT_HEATALARM779,           //ComErr 断通讯
    TEXT_DRVIE_NO = 106,         //Drive NO.     驱动器编号
    TEXT_DRVIE_LENGTH,           //Drive Length  驱动器数据长度
    TEXT_DRVIE_FUNC,             //Drive Func    驱动器功能码
    TEXT_DRVIE_ADDR,             //Drive Addr    驱动器地址
    TEXT_DRVIE_VALUE,            //Drive Value   驱动器数据
    TEXT_ALARM_E100101,          //TC1 Cold Start Prevent TC1冷态启动防止时间未到
    TEXT_ALARM_E200101,          //TC2 Cold Start Prevent TC2冷态启动防止时间未到
    TEXT_PRODUCT_DAY,            //day 生成运行时间：日
    TEXT_PRODUCT_HOUR,           //hour 生成运行时间：时
    TEXT_PRODUCT_MIN,            //min 生成运行时间：分
    TEXT_PRODUCT_SEC,            //sec 生成运行时间：秒
    TEXT_AD_SENSOR1,             //AD-传感器1
    TEXT_AD_SENSOR2,             //AD-传感器2
    TEXT_AD_EXAD5,               //EXAD5 AD扩展5
    TEXT_AD_EXAD6,               //EXAD6 AD扩展6
    TEXT_AD_EXAD7,               //EXAD7 AD扩展7
    TEXT_AD_EXAD8,               //EXAD8 AD扩展8
    TEXT_SYSPRS_BAR,             //系统压力(bar)
    TEXT_INJPRS_BAR,             //射出压力(bar)
    TEXT_SYSPRS,                 //系统压力
    TEXT_INJPRS,                 //射出压力
};


//information
enum INFO_INDEX{
    VW_MSG_UDISKNOTEXIST = 1,			//U Disk Not Exist!
    VW_MSG_UDISKNOFILE,					//No File in U Disk!
    VW_MSG_UDISKNOPROG,					//No Program in U Disk!
    PANEL_UPOK,							//Restart
    MAIN_UPOK,							//Please Restart The Main
    UPDATE_FAIL,						//Update Err！
    VW_MSG_RESTART,						//Please Restart！
    VW_MSG_INVALID_PASSWD,				//Invalid Password！提示密码不正确
    VW_MSG_CHAR_NOENOUGH,				//Char is Not Enough！提示机器编码或识别码过短
    VW_MSG_ILLEGEL_DATES,				//Enter dates illegal！提示输入非法日期
    VW_MSG_ERR_ID,						//Inconsistency identifier！提示识别码不一致
    VW_MSG_HOST_RESTAROK,				//Host Reset Success！主机重置成功
    SERVO_OPFAIL,						//Servo Data Operate Fail!
    VW_MSG_SAVESUCCESS,					//Save Successfull!
    VW_MSG_UDISKREADSUCCESS,			//U Disk Import Successfull!
    VM_MSG_SELECTTYPE,					//Please Select Data Type.
    VM_MSG_PLEASEENTERNAME,				//Please Input Name
    VM_MSG_DATAISALREADY,				//Data Is Already In Udisk, Overwrite?
    VM_MSG_RESTORETOSAVE,				//Selected Will Restore To Saved!
    VM_MSG_RESTORETODEF,				//Selected Will Restore To Default!
    VM_MS_DATAIMPORT,					//Data import, please wait！
    VM_MS_IMPORTSUCSS,					//Data import successfully!

    VW_MSG_DRIVERMODEERR =30,           //Driver Mode Err 驱动器控制模式错误
    VW_MSG_MOTORISRUNNING,              //Motor Is Running 电机正在运行中
    VW_MSG_CHANGESERVOMODETODEBUG,      //Please change to debug mode 驱动模式错误，请先转为调试模式
    VW_MSG_MOTORALARM,                  //Drive the alarm, please out 驱动器有报警,请先排除!
    VW_MSG_CLOSEMOTOR,                  //Please Close Motor 请先关闭电机
    VW_SERVO_SELFLEARN_EMERGENCY_STOP,  //Self-learn Emergency Stop 自学习被紧急停止
    VW_SERVO_SELFLEARNFAILURE,          //Motor Self-learn Failure 自学习失败
    VW_MSG_PRESS_MANUAL_KEY_CONFIRM,    //Press Manual Key To Confirm泵浦配置更改，请按手动键确认

    VW_MSG_FIRSTNOTBK =40,               //General,First not backpress!普通机第一路不作背压

    VW_MSG_MANUAL =50,                   //Execute it in Manual Mode!请在手动模式操作此功能!
    VW_MSG_CORESELECTED,                 //Core Has Been Selected!此中子已选择！
    VW_MSG_STOPSELFLEARN,                 //Please stop self-learning 请先停止自学习！

    VW_MSG_SAVEDATA,                      //Data has been modified, please save the data 数据已修改,是否保存！

    VM_MSG_RESETCFG = 54,                //Do you want to delete and reset the host configuration?是否删除主机配置！
    VM_MSG_MANUAL_CUTMOTOR,              //Close motor and switch to manual mode 请关闭马达并切换到手动模式！

    VW_MSG_UDISKSAVESUCCESSX = 60,        //U Disk Save Successful! 写U盘成功！
    VW_MSG_UDISKSAVEFAIL,                //U Disk Save Fail! 写U盘失败！ 61
    VW_MSG_UDISKREADSUCCESSX,            //U Disk Read Successful! 读U盘成功！ 62
    VW_MSG_UDISKREADFAIL,                //U Disk Read Fail! 读U盘失败！ 63

    VW_MSG_MOLDPARSAVESUCCESS,           //MoldPar Save Successful! 模具参数保存成功！64
    VW_MSG_MOLDPARSAVEFAIL,              //MoldPar Save Fail! 模具参数保存失败！65
    VW_MSG_MOLDPARREADSUCCESS,           //MoldPar Read Successful!  模具参数读取成功！ 66
    VW_MSG_MOLDPARREADFAIL,              //MoldPar Read Fail! 模具参数读取失败！ 67

    VW_MSG_WORKPARSAVESUCCESS,           //WorkPar Save Successful!工作参数保存成功！68
    VW_MSG_WORKPARSAVEFAIL,              //WorkPar Save Fail!  工作参数保存失败！ 69
    VW_MSG_WORKPARREADSUCCESS,           //WorkPar Read Successful! 工作参数读取成功！ 70
    VW_MSG_WORKPARREADFAIL,              //WorkPar Read Fail! 工作参数读取失败！ 71

    VW_MSG_PUSHTABLESAVESUCCESS,         //Pushtable Save Successful! 十字头数据保存成功！ 72
    VW_MSG_PUSHTABLESAVEFAIL,            //Pushtable Save Fail! 十字头数据保存失败！ 73
    VW_MSG_PUSHTABLEREADSUCCESS,         //Pushtable Read Successful! 十字头数据读取成功 74
    VW_MSG_PUSHTABLEREADFAIL,            //Pushtable Read Fail! 十字头数据读取失败！ 75

    VW_MSG_CONFIGSAVESUCCESS,            //Config Save Successful! 主机配置数据保存成功！ 76
    VW_MSG_CONFIGSAVEFAIL,               //Config Save Fail!主机配置数据保存失败！ 77
    VW_MSG_CONFIGREADSUCCESS,            //Config Read Successful!主机配置数据读取成功 78
    VW_MSG_CONFIGREADFAIL,               //Config Read Fail! 主机配置数据读取失败！ 79

    VW_MSG_CONFIGFILESAVESUCCESS,        //ConfigFile Save Successful!配置文件保存成功！ 80
    VW_MSG_CONFIGFILESAVEFAIL,           //ConfigFile Save Fail!配置文件保存失败！   81
    VW_MSG_CONFIGFILEREADSUCCESS,        //ConfigFile Read Successful!配置文件读取成功 82
    VW_MSG_CONFIGFILEREADFAIL,           //ConfigFile Read Fail!配置文件读取失败！ 83

    VW_MSG_SCREENSHOTSAVESUCCESS,        //ScreenShot Save Successful!画面截图保存成功！84
    VW_MSG_SCREENSHOTSAVEFAIL,           //ScreenShot Save Fail!画面截图保存失败！ 85
    VW_MSG_MACHINENAMETOOLONG,           //The name of the machine is too long! 机器型号名称过长！ 86 //20181225 hz
    VM_MSG_CLOSEHEATER,                  //Please Close Heater  请关闭电热
    VM_MSG_BARREL_TEMP_HIGH,             //Barrel Temp Too High  料桶温度过高
    VM_MSG_OPTIMIZE_EXIT,                //Temp is Optimizing,Exit?   正在温度优化，是否退出？
    VW_EJECT_SENSER_USE,                 //Electric Eye Check Need to Use  电眼检出需使用
    VW_EJECT_BLOWMODE_NEEDCHANGE,         //Please Change Blow Mode to expect AftSensor Mode  请更改吹气方式到非电眼后模式
    VW_MSG_PANELIMPORTDRIVESUCCESS,      //Panel Import Drive Data Successful! 面板导入驱动器数据成功!
    VW_MSG_PANELOUTPORTDRIVESUCCESS,     //Panel Outport Drive Data Successful! 面板导出驱动器数据成功!
    VW_MSG_PANELIMPORTDRIVEFAIL,         //Panel Import Drive Data Fail! 面板导入驱动器数据失败!
    VW_MSG_PANELOUTPORTDRIVEFAIL,        //Panel Outport Drive Data Fail! 面板导出驱动器数据失败!
    VW_MSG_PLSCLSMOTOR,                    //Do Not Operate When Motor Open, Please Close Motor! 马达开启时不能导入参数,增加提示信息
    VM_MSG_CUROPTNOTFINISH,              //Current Operate Not Finish! 当前操作未完成!
    VM_MSG_SELECTDRIVE,                  //Please Select Drive! 请选择驱动器!
    VW_MSG_OPENLINKERR,                  //Can't Achieve With Charge Open Link! 储料开模功能使用时不能选择此功能!
    VW_MSG_ADJUSTMODE,                   //Attention,Adjust mode! 注意,已进入调机模式!
    VW_MSG_MOTOOPNOTOUTPUT,              //Please do this when the motor is off! 请在马达关状态下进行该操作!
    VM_MSG_TEMP_ABNORMAL,                //Temp Components Abnormity 温度部件异常！
    VM_MSG_PUMP_ABNORMAL,                //Pump Components Abnormity!泵浦部件异常！
    VM_MSG_TEMP_PUMP_ABNORMAL,		     //Temp & Pump Components Abnormity!温度&泵浦部件异常！
    VW_MSG_FILENAMEERROR,                //File name cannot contain any of the following characters: < > / \ | : " * ? 文件名不能包含下列任何字符: < > / \ | : " * ?
    VW_MSG_USERERR,                      //Permission Level Is Too Low,Can Not Set! 权限等级过低，无法设置! 106 //20191224.cyx
    VW_MSG_DATAPROTECT,                  //Data Has Been Protected! 数据已保护! 107 //20191224.cyx
    VM_MSG_PLSINPUTFILENAME,			 //Please Input File Name! 请输入文件名称！ 108
    VW_MSG_SHUTDOWNTEST,                 //Please Close The Test First! 请先关闭测试! 109
    VM_MSG_EMPTY_DATA,                   //One Item Of Data Is Empty,Please Cheak! 数据中存在空数据，请检查！110
    VW_MSG_NOSPEED,                      //Max RPM Unread! 最大转速未读取! 111
    VW_MSG_MOTORTEMPUSED,                //Existing temp range is used as motor temp, please close before selecting!
    VW_MSG_MOUTHTEMPUSED,                //Existing temp range is used as blanking mouth temp, please close before selecting!
    VW_MSG_OILTEMPUSED,                  //Existing temp range is used as oil temp, please close before selecting!
    VW_MSG_USEFIRSTONE,                  //The first block is used first, the second block is not planned for the time being
    VM_MSG_CHANGETOADJMOLD,              //Please Change to AdjMold State! 请切换到调模状态! 116
    VW_MSG_METERCHANNELUSED,             //The Meter Channel Is Used,Please Check It! 电子尺通道已被使用,请检查! 117
    VW_MSG_HOSTSTARTOVER,                //Host Reboot success! 主机重启成功！ 118
    VM_MSG_SELMOLDTMP,                   //Please Tick MoldTemp 请先勾选模温 119
    VM_MSG_UDISKNTFS,                    //U Disk Is NTFS,Please Change to Fat32! U盘格式为NTFS,请更换FAT32格式的U盘 120
    VM_MSG_UDISK_DPT,                    //U Disk exist multiple subarea,May not recognize update file,suggest replacing U Disk! 121
    VM_MSG_CLOSESPARE2,                  //Please Close Spare 2 请关闭备用2按键 122
    VW_MSG_LOCKED,                       //Panel Key Is Locked! 按键锁已锁定! 123
    VW_MSG_WHETHER_SAVE,                 //Whether to save the modified? 确认修改是否保存? 124
    VW_MSG_WHETHER_CLEAR,                //Whether to clear the data? 确认是否清空数据? 125
    VM_MSG_PRESSCORESEL,                 //Please Press Key CORESELECT! 请按中子切换键! 126
    VM_MSG_PRESSAIRSEL,                  //Please Press Key AIRSELECT! 请按吹气切换键! 127
    VM_MSG_COREFUNCUSE,                  //At least use one group core func! 至少一组中子功能选择使用! 128
    VM_MSG_SETAIRTIME,                   //At Least Open One Group Air Act! 至少开启一组吹气动作! 129
    VM_MSG_FILEFORMATERR,                //Incorrect file format! 文件格式不正确！130
    VW_MSG_WHETHER_READ,                 //Whether to read the mold? 确认是否读入模具? 131
    VW_MSG_AUTO_CHECKING,                //Touch Screen Is Checking... 触摸屏检测中... 132 //20210914 dyl touch
    VW_MSG_AUTO_CHECK_OK,                //Touch Screen Check Ok! 触摸屏检测正常! 133
    VW_MSG_AUTO_CHECK_ERR,               //Touch Screen Check Error,Please Contact The Manufacturer! 触摸屏检测异常,请与厂家联系! 134
    VW_MSG_PLANSUCCESS = 150,   //Build supplement success！
    VW_MSG_PLANERR ,   //Build supplement err！
    VW_MSG_REQ ,   //Already req！
    VM_MSG_DATA_PREPARATION, //数据准备中......
    VW_MSG_UDISKABNORMAL,  //U盘更新超时
    VW_MSG_AUTOCLOSE,//在5s后自动关闭弹框
    VW_MSG_PRESSRUNKEY,//在6s内连续按3次运行键触发特殊程序更新
    VW_MSG_AUTOUPDATE,//15s后自动更新程序，如果无需更新，请立即拔出U盘！
    VW_MSG_IAPFAIL,//扩展板更新失败！
    VW_MSG_IAPSUCCESS,//扩展板更新成功！
    VM_MSG_MAIN_CTRL_RESTER,             //主机控制面板：请求重启主机
    VW_MSG_UPDATEPACKFAIL,               //更新包校验失败!
    VW_MSG_CLSHEATERANDMOTOR,            //请关闭电热和马达!
    VW_MSG_RESTORETODEFAULT,            //即将恢复为默认值
    VW_MSG_RESTORE_ELEC_METER_TODEFAULT,//请重置电能表!
    VW_MSG_OPCUA_RESTART,//开关opcua功能，请重启面板
    VW_MSG_DATA_OVERLOAD,//数据超限,请核对!
    VW_MSG_END
};


//usb define
#ifdef USE_QT
    #define USB_PATH		ROOT_PREFIX"ivent/"
    #define USB_MAIN_PATH   ROOT_PREFIX"mainivent/"    //USB中主机相关文件夹
    #define USB_PARAM_PATH  ROOT_PREFIX"iventpar/"     //USB中数据参数相关文件夹
    #define USB_DRIVEIMPORT_PATH   ROOT_PREFIX"DriveImport/"    //导入驱动器参数文件夹
    #define USB_DRIVEOUTPORT_PATH  ROOT_PREFIX"DriveOutport/"   //导出驱动器参数文件夹
    #define ESTDATA_PATH	USB_BOOT_PATH"iventdata"
    #define ESTDATAEXCEL_PATH	USB_BOOT_PATH"iventExcel/"
    #define CURVEEXCEL_PATH ESTDATAEXCEL_PATH"CurveExcel/"
#else
    #define USB_PATH		ROOT_PREFIX"est/"
    #define USB_MAIN_PATH   ROOT_PREFIX"mainest/"    //USB中主机相关文件夹
    #define USB_PARAM_PATH  ROOT_PREFIX"estpar/"     //USB中数据参数相关文件夹
    #define ESTDATA_PATH	USB_BOOT_PATH"estdata"
#endif



#ifdef __cplusplus
}
#endif

#endif



