#ifndef _ROBOT_DIC_H_
#define _ROBOT_DIC_H_

#include "typedef.h"

#pragma pack(1)

typedef struct EN_ROBOT_PO
{
    UI16  O_ROBOT   ;                 //0x31100000 机械手       
    UI16  O_R_EJTF  ;                 //0x31100001 顶进 完成
    UI16  O_R_EJTB  ;                 //0x31100002 顶退  完成
    UI16  O_R_SDRC  ;                 //0x31100003 机械手安全门关     
    UI16  O_R_OPNM  ;                 //0x31100004 开模终   
    UI16  O_R_CLSM  ;                 //0x31100005 关模终     
    UI16  O_R_INJE  ;                 //0x31100006 射出中       
    UI16  O_R_AUTO  ;                 //0x31100007 注塑机自动   
    UI16  O_R_CPRF  ;                 //0x31100008 中子进 完成
    UI16  O_R_CPRB  ;                 //0x31100009 中子退 完成
    UI16  O_R_INJERR;                 //0x3110000A 不良品  
    UI16  O_R_EMERG;                  //0x3110000B 注塑机急停   
    UI16  O_R_C2PRF  ;                //0x3110000C 中子2进完成 
    UI16  O_R_C2PRB  ;                //0x3110000D 中子2退完成
    UI16  O_R_SDRC2  ;                //0x3110000E 后安全门关终 
    UI16  O_R_EMERG2 ;                //0x3110000F  注塑机急停2    //20180409 LET 
    UI16  O_R_SFMA1 ;                 //0x31100010  安全装置1     //20180409 LET 
	UI16  O_R_SFMA2 ;                 //0x31100011  安全装置2      //20180409 LET 
	UI16  O_R_OPMD  ;                 //0x31100012  开模中途       //20180409 LET 
	UI16  O_M_OPN;                    //0x31100013 模具开模完成
	UI16  O_M_SFD;                    //0x31100014 模具安全门关
	UI16  O_M_EJEF;                   //0x31100015 模具顶进完成
	UI16  O_M_EJEB;                   //0x31100016 模具顶退完成
	UI16  O_M_MYALARM;                //0x31100017 模压警报解除 
}ROBOT_PO, *PROBOT_PO;

typedef struct EN_ROBOT_PI
{
    UI16  I_ROBOT   ;                 //0x31110000 机械手     
    UI16  I_OPN_ENA ;                 //0x31110001 空气模致能1   
    UI16  I_INJ_ENA ;                 //0x31110002 空气模致能2   
    UI16  I_R_EJTF  ;                 //0x31110003 顶进安全    
    UI16  I_R_CLSM  ;                 //0x31110004 关模安全    
    UI16  I_R_OPNM  ;                 //0x31110005 开模安全     
    UI16  I_R_EJTB  ;                 //0x31110006 顶退指令     
    UI16  I_R_RCYC  ;                 //0x31110007 循环起动    
    UI16  I_R_MAFR  ;                 //0x31110008 模区安全    
    UI16  I_R_EMEG  ;                 //0x31110009 机械手急停   
    UI16  I_R_CPRF  ;                 //0x3111000A 中子进允许   
    UI16  I_R_CPRB  ;                 //0x3111000B 中子退允许   
    UI16  I_R_MANUAL;                 //0x3111000C 机械手手动  
    UI16  I_R_C2PRF  ;                //0x3111000D 中子2进允许   
    UI16  I_R_C2PRB  ;                //0x3111000E 中子2退允许   
    UI16  I_R_EMEG2  ;                //0x3111000F 机械手急停2   
    UI16  I_R_OPMD   ;                //0x31110010 允许完全开模      //20180409 LET 
    UI16  I_M_MYALARM;                //0x31110011 模压警报
	UI16  I_M_CLS;                    //0x31110012 模具关摸安全
	UI16  I_M_EJEF;                   //0x31110013 模具顶进安全
	UI16  I_M_EJEB;                   //0x31110014 模具顶退安全
	UI16  I_M_ROB;                    //0x31110015 模具允许机械手         
	UI16  I_M_ALAR;                   //0x31110016 模具保护警报    
}ROBOT_PI, *PROBOT_PI;


typedef struct EN_ROBOT_MOLDSET
{
    UI16  FL_ROBOTCTRL;                  /* 0x31130000 机械手功能 0不使用 1使用 默认0*/
    UI16  FL_MANUALOPN;					 /* 0x31130001 手动状态使用机械手  0不使用1使用  默认0*/
    UI16  FL_CORESEC1;                   /* 0x31130002 中子信号1连接:0:不使用 1:A 2:B 3:C 4:D 5:E 6:F    默认0*/
    UI16  FL_CORESEC2;	                 /* 0x31130003 中子信号2连接:0:不使用 1:A 2:B 3:C 4:D 5:E 6:F    默认0*/
    UI16  FL_ROBOTCLPMID;                /* 0x31130004 开模途中机械手     预留给面板使用，下位机不可占用该地址*/
    UI16  TM_ROBOTMID;                   /* 0x31130005 开模途中机械手时间 预留给面板使用，下位机不可占用该地址*/
    //20190403 CHJ 补齐
    UI16  FL_ROBOTWAIT;					 /* 0x31130006 等待机械手警报   0   警报   1  不警报  默认0   20190401*/
	UI16  FL_MOJUPOTE;                   // 0x31130007 模具保护使用   0  不使用   1   使用
	UI16  ROBOTACT_POSITION;             /* 0x31130008 途中机械手动作位置下限  范围 关模低压终止位置 - 开模五段终止位置 默认150.0mm*/
}ROBOT_MOLDSET, *PROBOT_MOLDSET;

typedef struct EN_ROBOT_STATE
{
    UI16  FL_ERROR0;					//0x31160000 警报1
    UI16  FL_ERROR1;					//0x31160001 警报2
}ROBOT_STATE, *PROBOT_STATE;

#pragma pack()

#ifdef DIC_ENUM_NEED

enum EN_ROBOT_ACT_SN
{
    //OS_ROBOT = 1,   				// 0   ; '机械手 '     'EJECTOR      '

    ROBOT_ACT_SN_END
};



/*错误位定义*/
#define     ER0_ROBO      0x0001      //#31100；机械手异常
#define     ER0_ROBE      0x0002      //#31101；机械手紧急停止  
#define     ER0_ROB2      0x0004      //#31102；等待机械手  
#define     ER0_ROBF      0x0008      //#31103；模具监控失败--------------下位机补
//20180409 LET  机械手警报
#define     ER0_ROB2_EJTF    0x0010  //#31104;    '等待机械手 -ROB顶进安全'      31100105
#define     ER0_ROB2_EJTB    0x0020  //#31105;    '等待机械手 -ROB顶退安全'      31100106
#define     ER0_ROB2_CLSM    0x0040  //#31106;    '等待机械手 -ROB允许关模'      31100107
#define     ER0_ROB2_OPNM    0x0080  //#31107;    '等待机械手 -ROB允许开模'      31100108
#define     ER0_ROB2_RCYC    0x0100  //#31108;    '等待机械手 -ROB循环启动'      31100109
#define     ER0_ROB2_MAFR    0x0200  //#31109;    '等待机械手-ROB模区安全'      3110010a
#define     ER0_ROB2_IMML    0x0400  //#3110A;    '等待机械手-ROB机械手联锁' 3110010b 

#define     ER0_ROBO_MAFR    0x10000  //#31110;   '机械手异常 - ROB模区安全 '    31100201
#define     ER0_ROBO_OPNM    0x20000  //#31111;   '机械手异常 - ROB允许开模 '    31100202
#define     ER0_ROBO_CLSM    0x40000  //#31112;   '机械手异常 - ROB允许关模 '    31100203
#define     ER0_ROBO_EJTF    0x80000  //#31113;   '机械手异常 - ROB顶进安全'     31100204
#define     ER0_ROBO_EJTB    0x100000 //#31114;   '机械手异常- ROB顶退安全'      31100205
#define		ER0_ROB_TURN	 0x200000 //#31115;	   机械手异常-输入点未变化   //20190403 CHJ 补齐

#define     ER0_MoldO        0x400000     //#31116 模具保护失败   
#define     ER0_MoldE        0x800000     //#31117 模具保护急停
#define     ER0_Mold2        0x1000000     //#31118 等待模具保护
#define     ER0_NOROBOT      0x2000000     //#31119 机械手未使用
#define     ER1_ROB_CFG     0x4000000   // #31119 - #3110020A 输入点未定义-ROB机械手
#define     ER1_ROBCLSM_CFG 0x8000000   // #3111A - #3110020B 输入点未定义-ROB允许关模

#endif


#endif

