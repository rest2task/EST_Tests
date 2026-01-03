#ifndef _LOCK_DIC_H_
#define _LOCK_DIC_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

#pragma pack(1)
typedef struct EN_LOCK_PO
{
  UI16  O_LOC_FWD;                 //0x28100000 定位1进      //定位2进
  UI16  O_LOC_BWD ;                //0x28100001 定位1退      //定位2退
  UI16  O_LOC_SRV;                 //0x28100002 定位1备用     //定位2备用
}LOCK_PO, *PLOCK_PO;

typedef struct EN_LOCK_PI
{
  UI16  I_LOC_FWD ;                 //0x28110000 定位进终 //定位2进终
  UI16  I_LOC_BWD ;                 //0x28110001 定位退终 //定位2退终
  UI16  I_LOC_SRV;                  //0x28110002 定位备用 //定位2备用
}LOCK_PI, *PLOCK_PI;

typedef struct EN_LOCK_MACHSET
{
  UI16  DAPRES_LOCRAMP;		         /* 0x28120000 定位启动压力斜率   .01S    默认0.50*/
  UI16  DAFLOW_LOCRAMP;		         /* 0x28120001 定位启动速度斜率   .01S    默认0.50*/
  UI16  DAPRES_LOCOFFRAMP;		     /* 0x28120002 定位结束压力斜率   .01S    默认0.00*/
  UI16  DAFLOW_LOCOFFRAMP;		     /* 0x28120003 定位结束速度斜率   .01S    默认0.00*/
  UI16  DAFLOW_LOCFWD_SETUP;         /* 0x28120004 定位慢速调试速度           默认30*/
  UI16  DAPRES_LOCBWD_SETUP;         /* 0x28120005 定位慢速调试压力           默认30*/

   UI16  LOCKF_HYDELAY;				 /* 0x28120006 定位进液压延迟*/
   UI16	 LOCKB_HYDELAY;			     /* 0x28120007 定位退液压延迟*/
  
}LOCK_MACHSET, *PLOCK_MACHSET;

typedef struct EN_LOCK_MOLDSET
{
	  /*功能+保护参数*/
  UI16  FL_LOC;                      /* 0x28130000 定位使用选择  0：不使用 1：使用 默认0*/
  UI16  TM_LOCFWDDLY;                /* 0x28130001 定位进延迟  .01S    默认0.50*/
  UI16  TM_LOCBWDDLY;                /* 0x28130002 定位退延迟  .01S    默认0.50*/
}LOCK_MOLDSET, *PLOCK_MOLDSET;

typedef struct EN_LOCK_ACTPARA
{
  UI16  DAPRES_LCKFWD;               /* 0x28140000 定位进压力                 */
  UI16  DAPRES_LCKBWD;               /* 0x28140001 定位退压力                 */
  UI16  DAFLOW_LCKFWD;               /* 0x28140002 定位进速度                 */
  UI16  DAFLOW_LCKBWD;               /* 0x28140003 定位退速度                 */

}LOCK_ACTPARA, *PLOCK_ACTPARA;

typedef struct EN_LOCK_HYDR
{
	UI16  HYDR_PORT_LOCK;			// 0x28150000 定位动作的DA输出通道：位操作-->port1……port4
    UI16  HYDR_PUMPS_LOCK;			// 0x28150001 定位动作的比例速度限制:位操作-->P1……P7
	UI16  SERVO_KP_LOCK;			// 0x28150002 定位动作的PID_KP
	UI16  SERVO_KI_LOCK;			// 0x28150003 定位动作的PID_KI
	UI16  SERVO_KD_LOCK;			// 0x28150004 定位动作的PID_KD
 }LOCK_HYDR, *PLOCK_HYDR;



typedef struct EN_LOCK_STATE
{
	UI16  FL_ERROR1;				//0x28160000 警报2
	UI16  FL_ERROR2;				//0x28160001 警报2
    UI16  Step_LockFwd;             //0x28160002 定位进实时动作步
    UI16  Step_LockBwd;             //0x28160003 定位退实时动作步
}LOCK_STATE, *PLOCK_STATE;
#pragma pack()


#ifdef DIC_ENUM_NEED

/*ACT SN start from 1*/
enum  EN_LOCK_ACT_SN
{
  OS_LOCF=1,            // 10  '定位进  '      //A310
  OS_LOCB,              // 11  '定位退  '      //A310
  
  LOCK_ACT_SN_END
};

enum EN_LOCK_ACTNAME
{
    LOCKFWD1 = STAGE_1_STEP,    
    LOCKBWD1 = STAGE_1_STEP,  
    
};

#define     ER0_LIMT    0x0001     //#28100 '未检测到定位进退终' 
 
#endif

#ifdef __cplusplus
}
#endif

#endif
