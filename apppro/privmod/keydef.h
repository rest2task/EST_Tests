#ifndef _KEYDEF_h
#define _KEYDEF_h

#ifdef __cplusplus
extern "C"
{ 
#endif
#include "typedef.h"
#include "key.h"


#ifndef PANEL_NOKEYBOARD//G15:KeyBoard
/*********** 
|     func key 
|     def range:0x401C--0x40FF
***********/ 

#define     _KEY_PANEL_MOLDOPEN         0x402F      //  开模
#define     _KEY_PANEL_MOLDCLOSE        0x4030      //  关模
#define     _KEY_PANEL_CHARGE           0x4031      //  储料
#define     _KEY_PANEL_AUTOPURGE        0x4032      //  自动清料
#define     _KEY_PANEL_MOLDADJ          0x4033      //  调模 使用

#define     _KEY_PANEL_INJECT           0x4029      //  射出
#define     _KEY_PANEL_SUCKBACK         0x402A      //  射退
#define     _KEY_PANEL_COREAIN          0x402B      //  中子A 进
#define     _KEY_PANEL_COREAOUT         0x402C      //  中子A 退
#define     _KEY_PANEL_MOLDTHICK        0x402D      //  调模退(后)  
#define     _KEY_PANEL_MOLDTHIN         0x402E      //  调模进（前）

#define     _KEY_PANEL_EJECTRET         0x4023      //  托模退
#define     _KEY_PANEL_EJECTADV         0x4024      //  托模进
#define     _KEY_PANEL_COREBIN          0x4025      //  中子B 进
#define     _KEY_PANEL_COREBOUT         0x4026      //  中子B 退
#define     _KEY_PANEL_AIRBLASTMOV      0x4027      //  (动)公模吹气1
#define     _KEY_PANEL_AIRBLASTSTAT     0x4028      //  （定)母模吹气1

#define     _KEY_PANEL_NOZZLEADV        0x401C      //  座进
#define     _KEY_PANEL_NOZZLERET        0x401D      //  座退
#define     _KEY_PANEL_MOTOR            0x401E      //  马达开关
#define     _KEY_PANEL_HEATER           0x401F      //  电热开关
#define     _KEY_PANEL_LUBRICATION      0x4020      //  润滑
#define     _KEY_PANEL_CLOSE            0x4021      //  关机(备用)
#define     _KEY_PANEL_OPEN             0x4022      //  开机(备用)

#define     _KEY_PANEL_MANUAL           0x4034      //  手动
#define     _KEY_PANEL_SEMIAUTO         0x4035      //  半自动
#define     _KEY_PANEL_SENSORAUTO       0x4036      //  电眼自动
#define     _KEY_PANEL_TIMEAUTO         0x4037      //  全程自动
#define     _KEY_PANEL_RUN              0x4038      //  运行
#define     _KEY_PANEL_SW1              0x403B      //  SW1

#define     _KEY_PANEL_AIRBLASTMOV2     0x4040      //  (动)公模吹气2
#define     _KEY_PANEL_AIRBLASTSTAT2    0x4041      //  （定)母模吹气2
#define     _KEY_PANEL_DOOROPEN         0x4042      //  安全门开
#define     _KEY_PANEL_DOORCLOSE        0x4043      //  安全门关
#define     _KEY_PANEL_CORECIN          0x4044      //  中子C 进
#define     _KEY_PANEL_CORECOUT         0x4045      //  中子C 退
#define     _KEY_PANEL_COREDIN          0x4046      //  中子D 进
#define     _KEY_PANEL_COREDOUT         0x4047      //  中子D 退
#define     _KEY_PANEL_SW2              0x4048      //  SW2
#define     _KEY_PANEL_SW3              0x4049      //  SW3
#define     _KEY_PANEL_COREEIN          0x4051      //  中子E 进
#define     _KEY_PANEL_COREEOUT         0x4052      //  中子E 退
#define     _KEY_PANEL_COREFIN          0x4053      //  中子F 进
#define     _KEY_PANEL_COREFOUT         0x4054      //  中子F 退

#define     _KEY_PANEL_MOLDADJNEW          0x404A      //  调模(新)
#define     _KEY_PANEL_AUTOMOLDADJ      0x404B      //  自动调模

#define     _KEY_PANEL_AIRBLASTMOV3     0x404C      //  (动)公模吹气3
#define     _KEY_PANEL_AIRBLASTSTAT3     0x404D      // （定)母模吹气3

#define     _KEY_PANEL_AIRBLASTMOV4     0x4055      //  (动)公模吹气4
#define     _KEY_PANEL_AIRBLASTSTAT4     0x4056      // （定)母模吹气4

#define     _KEY_PANEL_AIRBLASTMOV5     0x4057      //  (动)公模吹气5
#define     _KEY_PANEL_AIRBLASTSTAT5     0x4058      // （定)母模吹气5

#define     _KEY_PANEL_AIRBLASTMOV6     0x4059      //  (动)公模吹气6
#define     _KEY_PANEL_AIRBLASTSTAT6     0x405A      // （定)母模吹气6

#define     _KEY_PANEL_AIRBLASTMOV7     0x405B      //  (动)公模吹气7
#define     _KEY_PANEL_AIRBLASTSTAT7     0x405C      // （定)母模吹气7
//20241210 chj 28组吹气,增加相应的按键表
#define     _KEY_PANEL_AIRBLASTMOV8      0x4080      //  (动)公模吹气8
#define     _KEY_PANEL_AIRBLASTSTAT8     0x4081      //  (定)母模吹气8

#define     _KEY_PANEL_AIRBLASTMOV9      0x4082      //  (动)公模吹气9
#define     _KEY_PANEL_AIRBLASTSTAT9     0x4083      //  (定)母模吹气9

#define     _KEY_PANEL_AIRBLASTMOV10      0x4084      //  (动)公模吹气10
#define     _KEY_PANEL_AIRBLASTSTAT10     0x4085      //  (定)母模吹气10

#define     _KEY_PANEL_AIRBLASTMOV11      0x4086      //  (动)公模吹气11
#define     _KEY_PANEL_AIRBLASTSTAT11     0x4087      //  (定)母模吹气11

#define     _KEY_PANEL_AIRBLASTMOV12      0x4088      //  (动)公模吹气12
#define     _KEY_PANEL_AIRBLASTSTAT12     0x4089      //  (定)母模吹气12

#define     _KEY_PANEL_AIRBLASTMOV13      0x408A      //  (动)公模吹气13
#define     _KEY_PANEL_AIRBLASTSTAT13     0x408B      //  (定)母模吹气13

#define     _KEY_PANEL_AIRBLASTMOV14      0x408C      //  (动)公模吹气14
#define     _KEY_PANEL_AIRBLASTSTAT14     0x408D      //  (定)母模吹气14

#define     _KEY_PANEL_MULTIEJECT       0x405D      // 多次托模
#define     _KEY_PANEL_PRESADJUST       0x405E      //压力校准 0x0911

#define     _KEY_PANEL_OILHEAT          0x4062      //油温预热机制改为按键机制
#define     _KEY_LUBRICATION1           0x4063      //润滑
#define     _KEY_LUBRICATION2           0x4064      //润滑2
#define     _KEY_MDTPHEATER             0x4065      //模温电热
#define     _KEY_PANEL_PITCHSELFLEARN   0x4066      //齿距自学习 //20211224 dyl
/*********** 
|     new func key 
|     def range:0x6000--0x6FFF
***********/ 
#define     _KEY_PANEL_CORESELECT       0x6001        //中子切换
#define     _KEY_PANEL_AIRSELECT        0x6002        //吹气切换
#define     _KEY_PANEL_ALARMCLEAR       0x6008        //警报清除
#define     _KEY_PANEL_PRINT            0x6003        //打印   20220426
#define     _KEY_PANEL_CURVE            0x6004        //曲线   20220426

#define     _KEY_PANEL_KEYLOCK          0x40      
#define     _KEY_PANEL_RESET            0x20 


/*********** 
|           LED 
***********/ 

#define     _LED_MANUAL         0x000001 //手动
#define     _LED_SEMIAUTO       0x000002 //半自动
#define     _LED_TIMEAUTO       0x000004 //全自动
#define     _LED_SENSORAUTO     0x000008 //电眼
#define     _LED_RUN            0x000010 //运行
#define     _LED_HEATER         0x000020 //电热
#define     _LED_ADJMOLD        0x000040 //调模
#define     _LED_ADJMOLDAUTO    0x000080 //自动调模

#define     _LED_CHARGE         0x000100 //储料
#define     _LED_AUTOPURGE      0x000200 //清料
#define     _LED_CHARGEB        0x000400 //储料B
#define     _LED_AUTOPURGEB     0x000800 //清料B
#define     _LED_AIRBLASTMOV    0x000400 //动模吹1
#define     _LED_AIRBLASTSTAT   0x80000000//定模吹1
#define     _LED_AIRBLASTMOV2   0x001000 //动模吹2
#define     _LED_AIRBLASTSTAT2  0x002000 //定模吹2
#define     _LED_LUBRICATION    0x004000 //润滑

#define     _LED_CORESELECT  0x008000 //中子切换
#define     _LED_AIRSELECT   0x000800 //吹气切换

#define     _LED_MOTOR          0x010000 //马达
#define     _LED_MOTORFLASH     0x020000 //马达闪
//20190626.cyx
#define     _LED_MOLDTHIN       0x00040000 //调模进
#define     _LED_MOLDTHICK      0x00080000 //调模退
#define     _LED_MOLDOPEN       0x00100000 //开模
#define     _LED_MOLDCLOSE      0x00200000 //关模
#define     _LED_EJECTRET       0x00400000 //托模退
#define     _LED_EJECTADV       0x00800000 //托模进
#define     _LED_NOZZLERET      0x01000000 //座退
#define     _LED_NOZZLEADV      0x02000000 //座进
#define     _LED_SUCKBACK       0x04000000 //射退
#define     _LED_INJECT         0x08000000 //射出
#define     _LED_COREOUT        0x10000000 //中子出
#define     _LED_COREIN         0x20000000 //中子入
#define     _LED_SW1            0x00001000 //备用1
#define     _LED_SW2            0x00002000 //备用2
#define     _LED_ALARMCLEAR     0x40000000 //警报清除
#define     _LED_PANEL_CURVE    0x00000080 //曲线 //20220426
#define     _LED_PANEL_PRINT    0x00001000 //打印 //20220426
//G15P15非触摸
#define     _LED_15_SW1         0x00002000 //15寸备用1 20220426
#define     _LED_15_SW2         0x40000000 //15寸备用2 20220426

#else //20210914 dyl touch/No_KeyBoard

//电容屏P15-5排新按键
/***********************标准P15-5排按键排布*************************
 *  手动          半自动         全自动         运行
key:0x4034      0x4035         0x4037       0x4038
led:0x00000001  0x000002       0x000004     0x000010
 *  开模          关模           中子出        中子入     中子切换     射出        加料        射退
key:0x402F      0x4030         0x402C       0x402B     0x6001     0x402D      0x402E     0x4033
led:0x00100000  0x00200000     0x10000000   0x20000000 0x008000   0x00080000  0x00040000 0x00000040
 *  托模退        托模进          吹气1         吹气2     吹气切换     清料        座进        座退
key:0x4023      0x4024         0x4045       0x4044     0x6015     0x4029      0x4031     0x402A
led:0x00400000  0x00800000     1x00001000   1x00002000 1x00004000 0x08000000  0x00000100 0x04000000
 *  润滑         警报清除         调模退        调模进     调模使用    备用1       备用2       备用3
key:0x4020      0x6008          0x4052      0x4051     0x6016     0x4032     0x401C      0x401D
led:0x004000    0x40000000      1x00008000  1x00010000 1x020000   0x000200   0x02000000  0x01000000
 *  备用4        备用5           备用6         备用7      备用8       马达       空白         电热
key:0x4048      0x4049          0x4027      0x4028     0x6002     0x401E     0x6005      0x401F
led:0x001000    0x00002000      0x00000400  0x80000000 0x000800   0x010000   0x000080    0x000020
 ***********************************************************/

#define     _KEY_PANEL_MOLDOPEN         0x402F      //  开模
#define     _KEY_PANEL_MOLDCLOSE        0x4030      //  关模
#define     _KEY_PANEL_CHARGE           0x402E      //  储料
#define     _KEY_PANEL_AUTOPURGE        0x4029      //  自动清料
#define     _KEY_PANEL_MOLDADJ          0x6016      //  调模 使用

#define     _KEY_PANEL_INJECT           0x402D      //  射出
#define     _KEY_PANEL_SUCKBACK         0x4033      //  射退
#define     _KEY_PANEL_COREAIN          0x402B      //  中子A 进
#define     _KEY_PANEL_COREAOUT         0x402C      //  中子A 退
#define     _KEY_PANEL_MOLDTHICK        0x4052      //  调模退(后)
#define     _KEY_PANEL_MOLDTHIN         0x4051      //  调模进（前）

#define     _KEY_PANEL_EJECTRET         0x4023      //  托模退
#define     _KEY_PANEL_EJECTADV         0x4024      //  托模进
#define     _KEY_PANEL_COREBIN          0x4025      //  中子B 进
#define     _KEY_PANEL_COREBOUT         0x4026      //  中子B 退
#define     _KEY_PANEL_AIRBLASTMOV      0x4045      //  (动)公模吹气1
#define     _KEY_PANEL_AIRBLASTSTAT     0x4044      //  （定)母模吹气1

#define     _KEY_PANEL_NOZZLEADV        0x4031      //  座进
#define     _KEY_PANEL_NOZZLERET        0x402A      //  座退
#define     _KEY_PANEL_MOTOR            0x401E      //  马达开关
#define     _KEY_PANEL_HEATER           0x401F      //  电热开关
#define     _KEY_PANEL_LUBRICATION      0x4020      //  润滑
#define     _KEY_PANEL_CLOSE            0x4021      //  关机(备用)
#define     _KEY_PANEL_OPEN             0x4022      //  开机(备用)

#define     _KEY_PANEL_MANUAL           0x4034      //  手动
#define     _KEY_PANEL_SEMIAUTO         0x4035      //  半自动
#define     _KEY_PANEL_SENSORAUTO       0x4036      //  电眼自动
#define     _KEY_PANEL_TIMEAUTO         0x4037      //  全程自动
#define     _KEY_PANEL_RUN              0x4038      //  运行
#define     _KEY_PANEL_SW1              0x403B      //  SW1

#define     _KEY_PANEL_AIRBLASTMOV2     0x4040      //  (动)公模吹气2
#define     _KEY_PANEL_AIRBLASTSTAT2    0x4041      //  （定)母模吹气2
#define     _KEY_PANEL_DOOROPEN         0x4042      //  安全门开
#define     _KEY_PANEL_DOORCLOSE        0x4043      //  安全门关
#define     _KEY_PANEL_CORECIN          0x405E      //  中子C 进
#define     _KEY_PANEL_CORECOUT         0x405F      //  中子C 退
#define     _KEY_PANEL_COREDIN          0x4046      //  中子D 进
#define     _KEY_PANEL_COREDOUT         0x4047      //  中子D 退
#define     _KEY_PANEL_SW2              0x4048      //  SW2
#define     _KEY_PANEL_SW3              0x4049      //  SW3
#define     _KEY_PANEL_COREEIN          0x4060      //  中子E 进
#define     _KEY_PANEL_COREEOUT         0x4061      //  中子E 退
#define     _KEY_PANEL_COREFIN          0x4053      //  中子F 进
#define     _KEY_PANEL_COREFOUT         0x4054      //  中子F 退

#define     _KEY_PANEL_MOLDADJNEW          0x404A      //  调模(新)
#define     _KEY_PANEL_AUTOMOLDADJ      0x404B      //  自动调模

#define     _KEY_PANEL_AIRBLASTMOV3     0x404C      //  (动)公模吹气3
#define     _KEY_PANEL_AIRBLASTSTAT3     0x404D      // （定)母模吹气3

#define     _KEY_PANEL_AIRBLASTMOV4     0x4055      //  (动)公模吹气4
#define     _KEY_PANEL_AIRBLASTSTAT4     0x4056      // （定)母模吹气4

#define     _KEY_PANEL_AIRBLASTMOV5     0x4057      //  (动)公模吹气5
#define     _KEY_PANEL_AIRBLASTSTAT5     0x4058      // （定)母模吹气5

#define     _KEY_PANEL_AIRBLASTMOV6     0x4059      //  (动)公模吹气6
#define     _KEY_PANEL_AIRBLASTSTAT6     0x405A      // （定)母模吹气6

#define     _KEY_PANEL_AIRBLASTMOV7     0x405B      //  (动)公模吹气7
#define     _KEY_PANEL_AIRBLASTSTAT7     0x405C      // （定)母模吹气7

#define     _KEY_PANEL_MULTIEJECT       0x40FE      //多次托模(未定义)
#define     _KEY_PANEL_PRESADJUST       0x405D      //压力校准

#define     _KEY_PANEL_OILHEAT          0x4062      //油温预热机制改为按键机制
#define     _KEY_LUBRICATION1           0x4063      //润滑
#define     _KEY_LUBRICATION2           0x4064      //润滑2
#define     _KEY_MDTPHEATER             0x4065      //模温电热
#define     _KEY_PANEL_PITCHSELFLEARN   0x4066      //齿距自学习 //20211224 dyl

#define     _KEY_PANEL_SWX1              0x4032      //  SWX1
#define     _LED_SWX1                    0x000200    //  SWX1
#define     _KEY_PANEL_SWX2              0x401C      //  SWX2
#define     _LED_SWX2                    0x02000000  //  SWX2
#define     _KEY_PANEL_SWX3              0x401D      //  SWX3
#define     _LED_SWX3                    0x01000000  //  SWX3
#define     _KEY_PANEL_SWX4              0x4048      //  SWX4
#define     _LED_SWX4                    0x001000    //  SWX4
#define     _KEY_PANEL_SWX5              0x4049      //  SWX5
#define     _LED_SWX5                    0x00002000  //  SWX5
#define     _KEY_PANEL_SWX6              0x4027      //  SWX6
#define     _LED_SWX6                    0x00000400  //  SWX6
#define     _KEY_PANEL_SWX7              0x4028      //  SWX7
#define     _LED_SWX7                    0x80000000  //  SWX7
#define     _KEY_PANEL_SWX8              0x6002      //  SWX8
#define     _LED_SWX8                    0x000800    //  SWX8

/***********
|     new func key
|     def range:0x6000--0x6FFF
***********/
#define     _KEY_PANEL_CORESELECT       0x6001        //中子切换
#define     _KEY_PANEL_AIRSELECT        0x6015        //吹气切换
#define     _KEY_PANEL_ALARMCLEAR       0x6008        //警报清除
#define     _KEY_PANEL_PRINT            0x6003        //打印   20220426
#define     _KEY_PANEL_CURVE            0x6004        //曲线   20220426

#define     _KEY_PANEL_KEYLOCK          0x40
#define     _KEY_PANEL_RESET            0x20


/***********
|           LED
***********/

#define     _LED_MANUAL         0x000001 //手动
#define     _LED_SEMIAUTO       0x000002 //半自动
#define     _LED_TIMEAUTO       0x000004 //全自动
#define     _LED_SENSORAUTO     0x000008 //电眼
#define     _LED_RUN            0x000010 //运行
#define     _LED_HEATER         0x000020 //电热
#define     _LED_ADJMOLD        0x020000 //调模
#define     _LED_ADJMOLDAUTO    0x000080 //自动调模

#define     _LED_CHARGE         0x00040000 //储料
#define     _LED_AUTOPURGE      0x08000000 //清料
#define     _LED_CHARGEB        0x000400 //储料B
#define     _LED_AUTOPURGEB     0x000800 //清料B
#define     _LED_AIRBLASTMOV    0x00001000 //动模吹1
#define     _LED_AIRBLASTSTAT   0x00002000 //定模吹1
#define     _LED_AIRBLASTMOV2   0x001000 //动模吹2
#define     _LED_AIRBLASTSTAT2  0x002000 //定模吹2
#define     _LED_LUBRICATION    0x004000 //润滑

#define     _LED_CORESELECT  0x008000 //中子切换
#define     _LED_AIRSELECT   0x00004000 //吹气切换

#define     _LED_MOTOR          0x010000 //马达
#define     _LED_MOTORFLASH     0x020000 //马达闪
//20190626.cyx
#define     _LED_MOLDTHIN       0x00010000 //调模进
#define     _LED_MOLDTHICK      0x00008000 //调模退
#define     _LED_MOLDOPEN       0x00100000 //开模
#define     _LED_MOLDCLOSE      0x00200000 //关模
#define     _LED_EJECTRET       0x00400000 //托模退
#define     _LED_EJECTADV       0x00800000 //托模进
#define     _LED_NOZZLERET      0x04000000 //座退
#define     _LED_NOZZLEADV      0x00000100 //座进
#define     _LED_SUCKBACK       0x00000040 //射退
#define     _LED_INJECT         0x00080000 //射出
#define     _LED_COREOUT        0x10000000 //中子出
#define     _LED_COREIN         0x20000000 //中子入
#define     _LED_SW1            0x00001000 //备用1
#define     _LED_ALARMCLEAR     0x40000000 //警报清除

#endif

#define     _LCD_RESET_STATE  	0x03000000 // 按键板复位
#define     _KEYBOARD_VERSION 	0x04000000 // 按键板版本

//20190508 VCOM
#define     _LCD_VCOM_INQUIRE   0x07030000 //VCOM对比度查询
#define     _LCD_VCOM_ADJUST    0x08000000 //VCOM电压手动调整 高16位的低8位表示调整%,如0x0832,0x32表示50%

/*********** 
|          self key 
***********/ 
#define    _KEY_GRAB_PNGIMAGE      (_F10+1)
#define    _KEY_GRAB_BMPIMAGE      (_F10+2)

#ifdef __cplusplus
}
#endif

#endif



