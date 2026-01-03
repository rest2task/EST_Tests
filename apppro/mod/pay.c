#include "pay.h"
#include "stdio.h"
#include "oprintf.h" 
#include "panel.h" 
#include "vardb.h"  
#include "time.h"
#include "protcmd.h"
#include "database.h"
#include "information.h"
#include "taskmoni.h"
#include "editwgt.h"
#include "ctrlcmd.h"
#include "iotnet.h"//NET_FUNC
#include "file.h"

//information diag
#define DIAG_INSTALL_INFORMATION   "Install_Ask"

//define easy WR const
#define CMOD_WR_HEADSZ sizeof(PAYSET_HEAD)
#define CMOD_WR_ITEMSZ 0
#define CMOD_WR_PATH   SAVE_PATH"pay.dat"
#define PAY_TIME_ID_FILE  HMI_ROOT_PATH"paytimeid.dat"  //新模式的唯一id

struct tm m_dtTimeout = {0};
struct tm m_dtTimeoutAlarm = {0};
struct tm dtNumPay = {0};

OPERATION m_operation;
ONLY_ID onlyid;
BOOL g_bFirstMoni = FALSE; //设置成功后检测一次
BOOL g_bInstallAsk = FALSE;//分期付款设置询问框

static UI32 ori_password, ori_passwordx;

static BOOL bLowVolTage = FALSE; //低电压
static void InitLowVol();

#ifndef WIN32
    time_t m_dttimeout;
	time_t m_dttimeoutalarm;
	time_t m_dttimeoutsecondalarm;
	time_t m_dttimeoutthirdalarm;
	time_t m_dttimeoutfourthalarm;
	time_t m_dttimeoutfifthalarm;
	time_t m_dttimeoutsixthalarm;
	time_t dtnumpay;
	time_t dtnumpayx;
#else
	I64  m_dttimeout,m_dttimeoutalarm,m_dttimeoutsecondalarm,m_dttimeoutthirdalarm,m_dttimeoutfourthalarm,m_dttimeoutfifthalarm,m_dttimeoutsixthalarm;
	I64  dtnumpay;
	I64  dtnumpayx;
#endif

OPERATION   g_dbOperationDefault = {
	0,
	1,
	9,
	0,
	10,
	1,
	1,
	10,
	1,
	1,
	60,
	0,
	0,//wIdentifyConfirm
	1,//cNowNum
	0,//timeout
	0,
	10,
	4,
	1,
	10,
	7,
	1,
	10,
	10,
	1,
	11,
	1,
	1,
	11,
	4,
	1,
	11,
	7,
	1,
	11,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    {49,50,51,52,53,54,55,56,0}
};

static UI32 TransToWord(char* p)
{
	UI32 w = 0;
    while (*p)
	{
		if (*p<='9' && *p>='0')
		{
			w=w*10+(*p-'0');
		}
        p++;
    }
    return w;
} 

static int PeriodPassx(UI32 password)
{
	UI16 j,k,h,m,n;
	UI32 dwOutNo,dwDate,dwCode;
	UI32 dwValue,dwValuex;
	UI32 x,x1,x2,x3,x4,x5,x6,x7;
	UI32 y,y1,y2,y3,y4,y5,y6,y7;
	UI32 jmdwValue,jmdwValuex;
	char cOutNo[9],cOutDate[9],cIdentifyCode[9];
	char cValue[12],cValuex[12];

	UI32 Periods[50]={78439121,23680729,57294760,56722311,29862580,
						69810549,15876621,42198890,23874001,41298300,
						44619805,18710659,76701648,36890468,89164801,
						77809168,35591975,76917559,97116554,66154075,
						93847512,54823165,69121265,67813556,57379834,
						15769880,97613468,97633310,16687146,80362582,
						98977755,24566713,47623763,13267125,46583256,
						32876550,63165237,65223543,83736523,49876871,
						32476598,73645287,48645876,16332538,96542587,
						27645467,23765905,64556253,87638765,41854342};
	int k0=23;
	int k1=19;
	int k2=17;
	int k3=13;
	int k4=11;
	int k5=7;
	int k6=3;
	int k7=1;
	int i=m_operation.cNowNum;

	dwOutNo=TransToWord(m_operation.machine2);
	sprintf(cOutNo,"%u",dwOutNo);
	for (j=0;j<8-strlen(cOutNo);j++)
	{
        dwOutNo=dwOutNo*10;
	}

	dwDate=m_operation.doYear*10000+m_operation.doMonth*100+m_operation.doDay;
	sprintf(cOutDate,"%u",dwDate);
	for (k=0;k<8-strlen(cOutDate);k++)
	{
        dwDate=dwDate*10;
	}

	dwCode=m_operation.wIdentifyCode;
	sprintf(cIdentifyCode,"%u",dwCode);
	for(h=0;h<8-strlen(cIdentifyCode);h++)
	{
        dwCode=dwCode*10;
	}
	while (i<=m_operation.cPeriods)
	{
        dwValue=(dwOutNo ^ dwDate ^Periods[i-1] ^ dwCode);
        dwValuex=(12345678 ^ dwDate ^Periods[i-1]);

		sprintf(cValue,"%u",dwValue);
        if(strlen(cValue)<8)
			for(m=0;m<8-strlen(cValue);m++)
			{
                dwValue=dwValue*10;
			}
		sprintf(cValuex,"%u",dwValuex);
        if(strlen(cValuex)<8)
			for(n=0;n<8-strlen(cValuex);n++)
			{
                dwValuex=dwValuex*10;
			}
			sprintf(cValue,"%u",dwValue);
			sprintf(cValuex,"%u",dwValuex);

		x= ((cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*k7
                          +(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*k6
                          +(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*k5
                          +(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*k4
                          +(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*k3
                          +(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*k2
                          +(cValue[6]-'0')*(cValue[6]-'0')*k1
                          +(cValue[7]-'0')*k0)%9+1;

		x1=((cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*k6
                         +(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*k5
                         +(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*k4
                         +(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*k3
                         +(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*k2
                         +(cValue[5]-'0')*(cValue[5]-'0')*k1
                         +(cValue[6]-'0')*k0
                         +(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*k7)%10;

		x2=((cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*k5
                         +(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*k4
                         +(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*k3
                         +(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*k2
                         +(cValue[4]-'0')*(cValue[4]-'0')*k1
                         +(cValue[5]-'0')*k0
                         +(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*k7
                         +(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*k6)%10;

		x3=((cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*k4
                         +(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*k3
                         +(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*k2
                         +(cValue[3]-'0')*(cValue[3]-'0')*k1
                         +(cValue[4]-'0')*k0
                         +(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*k7
                         +(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*k6
                         +(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*k5)%10;

		x4=((cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*k3
                         +(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*k2
                         +(cValue[2]-'0')*(cValue[2]-'0')*k1
                         +(cValue[3]-'0')*k0
                         +(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*k7
                         +(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*k6
                         +(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*k5
                         +(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*k4)%10;

		x5=((cValue[0]-'0')*(cValue[0]-'0')*(cValue[0]-'0')*k2
                         +(cValue[1]-'0')*(cValue[1]-'0')*k1
                         +(cValue[2]-'0')*k0
                         +(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*k7
                         +(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*k6
                         +(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*k5
                         +(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*k4
                         +(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*k3)%10;

		x6=((cValue[0]-'0')*(cValue[0]-'0')*k1
                         +(cValue[1]-'0')*k0
                         +(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*k7
                         +(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*k6
                         +(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*k5
                         +(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*k4
                         +(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*k3
                         +(cValue[7]-'0')*(cValue[7]-'0')*(cValue[7]-'0')*k2)%10;

		x7=((cValue[0]-'0')*k0
                         +(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*(cValue[1]-'0')*k7
                         +(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*(cValue[2]-'0')*k6
                         +(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*(cValue[3]-'0')*k5
                         +(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*(cValue[4]-'0')*k4
                         +(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*(cValue[5]-'0')*k3
                         +(cValue[6]-'0')*(cValue[6]-'0')*(cValue[6]-'0')*k2
                         +(cValue[7]-'0')*(cValue[7]-'0')*k1)%10;

		//no identify
		y=((cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*k7
                          +(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*k6
                          +(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*k5
                          +(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*k4
                          +(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*k3
                          +(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*k2
                          +(cValuex[6]-'0')*(cValuex[6]-'0')*k1
                          +(cValuex[7]-'0')*k0)%9+1;

		y1=((cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*k6
                         +(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*k5
                         +(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*k4
                         +(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*k3
                         +(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*k2
                         +(cValuex[5]-'0')*(cValuex[5]-'0')*k1
                         +(cValuex[6]-'0')*k0
                         +(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*k7)%10;

		y2=((cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*k5
                         +(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*k4
                         +(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*k3
                         +(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*k2
                         +(cValuex[4]-'0')*(cValuex[4]-'0')*k1
                         +(cValuex[5]-'0')*k0
                         +(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*k7
                         +(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*k6)%10;

		y3=((cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*k4
                         +(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*k3
                         +(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*k2
                         +(cValuex[3]-'0')*(cValuex[3]-'0')*k1
                         +(cValuex[4]-'0')*k0
                         +(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*k7
                         +(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*k6
                         +(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*k5)%10;

		y4=((cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*k3
                         +(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*k2
                         +(cValuex[2]-'0')*(cValuex[2]-'0')*k1
                         +(cValuex[3]-'0')*k0
                         +(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*k7
                         +(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*k6
                         +(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*k5
                         +(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*k4)%10;

		y5=((cValuex[0]-'0')*(cValuex[0]-'0')*(cValuex[0]-'0')*k2
                         +(cValuex[1]-'0')*(cValuex[1]-'0')*k1
                         +(cValuex[2]-'0')*k0
                         +(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*k7
                         +(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*k6
                         +(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*k5
                         +(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*k4
                         +(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*k3)%10;

		y6=((cValuex[0]-'0')*(cValuex[0]-'0')*k1
                         +(cValuex[1]-'0')*k0
                         +(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*k7
                         +(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*k6
                         +(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*k5
                         +(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*k4
                         +(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*k3
                         +(cValuex[7]-'0')*(cValuex[7]-'0')*(cValuex[7]-'0')*k2)%10;

		y7=((cValuex[0]-'0')*k0
                         +(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*(cValuex[1]-'0')*k7
                         +(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*(cValuex[2]-'0')*k6
                         +(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*(cValuex[3]-'0')*k5
                         +(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*(cValuex[4]-'0')*k4
                         +(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*(cValuex[5]-'0')*k3
                         +(cValuex[6]-'0')*(cValuex[6]-'0')*(cValuex[6]-'0')*k2
                         +(cValuex[7]-'0')*(cValuex[7]-'0')*k1)%10;

		jmdwValue= x*10000000+x1*1000000+x2*100000+x3*10000+x4*1000+x5*100+x6*10+x7;
		jmdwValuex=y*10000000+y1*1000000+y2*100000+y3*10000+y4*1000+y5*100+y6*10+y7;
            //oprintf("PeriodPassx %d\n", jmdwValue);
        ori_password = jmdwValue;
        ori_passwordx = jmdwValuex;
		if ((jmdwValue==password)||(jmdwValuex==password)) break;
			i++;
	}
	 return i;
}
/************************************************************************/
/* 分期停机功能使                                                       */
/************************************************************************/
BOOL IsPayChecking()
{
	return m_operation.cState != 0;
}
/************************************************************************/
/* 分期停机警报或停机                                                   */
/************************************************************************/
UI16 IsPayTimeOut()
{
	return m_operation.bTimeOut==1;
}
/************************************************************************/
/* 分期停机识别码                                                       */
/************************************************************************/
UI16 PayIdentifyCode()
{
	return m_operation.wIdentifyCode;
}
/************************************************************************/
/* 设置停机时间状态                                                     */
/************************************************************************/
static void SetTimeout(UI16 bTimeout )
{
	m_operation.bTimeOut = bTimeout;
}
/************************************************************************/
/* 分期停机警报或停机设定                                               */
/************************************************************************/
static void OnTimeOut(UI16 bTimeout)
{
	SetTimeout(bTimeout);

	if(bTimeout !=0)
	{
		PanelShowPageByName(PAGE_INSTALLPAY);
	}
}
/************************************************************************/
/* 分期停机密码判断                                                     */
/************************************************************************/
int Pay(UI32 dwpassword)
{
	int i,nret;
	PAYSET_HEAD head;

	i = PeriodPassx(dwpassword);
	if (i > m_operation.cPeriods) 
	{
		return -1;
	}
	OnTimeOut(0);
	nret = FALSE;
	m_operation.cNowNum = i+1;
	if(m_operation.cNowNum > m_operation.cPeriods)
	{
		m_operation.cState = 0;
		m_operation.wIdentifyCode = 0;
		nret = TRUE;
	}
	memcpy(&head.m_operation,&m_operation,sizeof(OPERATION));
	CMOD_WHEAD_EX(&head);
	return nret ? 0 : i+1;
}
int Payx(UI64 dwpassword)
{
    int i;
    struct tm* t;
    time_t time_now;
    time(&time_now);
    t = localtime(&time_now);

    UI32 dwPassOld, dwPassOldx;
    UI32 dwPassCalc = 0;	//计算后的实际密码，应与dwPassOld相同
    UI32 dwPassCalcx = 0;	//计算后的实际密码，应与dwPassOld相同
    UI32 dwDayCalc = 0;	//计算后的天数

    int nDayBit = 0;	//天数值当前位
    //20221026 dyl 扣电池或低电压导致时间错误
    if ((t->tm_year+1900)<=2000)//如果日期错乱，默认选择为2070年1月1日
    {
        m_operation.doYear = 70;
        m_operation.doMonth = 1;
        m_operation.doDay = 1;
    }
    else
    {
        m_operation.doYear = t->tm_year+1900-2000;
        m_operation.doMonth = t->tm_mon+1;
        m_operation.doDay = t->tm_mday;
    }
    PeriodPassx(0);
    dwPassOld = ori_password; //原密码
    dwPassOldx = ori_passwordx;  //原超级密码

#if 0
    for(i=0; i<4; i++){
        UI32 v = dwPassOld % 10;
        UI32 vx = dwPassOldx % 10;
        UI32 sft = v % 8;
        UI32 sftx = vx % 8;

        if(i == 3){
            UI32 d = dwpassword & 2047;	//最后11bit
            UI32 dx = dwpassword & 2047;	//最后11bit
            v = ((d >> (sft+3)) << sft) | (d & ((1<<sft)-1));
            vx = ((dx >> (sftx+3)) << sftx) | (dx & ((1<<sftx)-1));
            dwDayCalc |= ((d >> sft) & 7) << nDayBit; nDayBit += 3;
        }
        else{
            UI32 d = dwpassword & 1023;	//每次10bit
            UI32 dx = dwpassword & 1023;	//每次10bit
            v = ((d >> (sft+2)) << sft) | (d & ((1<<sft)-1));
            vx = ((dx >> (sftx+2)) << sftx) | (dx & ((1<<sftx)-1));
            dwDayCalc |= ((d >> sft) & 3) << nDayBit; nDayBit += 2;
        }

        dwPassCalc |= v << (i<<3);
        dwPassCalcx |= vx << (i<<3);
        dwpassword >>= 10;
        dwPassOld /= 10;
        dwPassOldx /= 10;
    }
#endif
    for(i=0; i<7; i++){
        UI32 v = dwPassOld % 10;
        UI32 vx = dwPassOldx % 10;
        UI32 sft = v % 4;
        UI32 sftx = vx % 4;

        if(i == 6){
            UI32 d = dwpassword & 127;	//最后7bit
            UI32 dx = dwpassword & 127;	//最后7bit

            v = ((d >> (sft+3)) << sft) | (d & ((1<<sft)-1));
            vx = ((dx >> (sftx+3)) << sftx) | (dx & ((1<<sftx)-1));
            dwDayCalc |= ((d >> sft) & 7) << nDayBit; nDayBit += 3;
        }
        else{
            UI32 d = dwpassword & 31;	//每次5bit
            UI32 dx = dwpassword & 31;	//每次5bit

            v = ((d >> (sft+1)) << sft) | (d & ((1<<sft)-1));
            vx = ((dx >> (sftx+1)) << sftx) | (dx & ((1<<sftx)-1));
            dwDayCalc |= ((d >> sft) & 1) << nDayBit; nDayBit ++;
        }

        dwPassCalc |= v << (i<<2);
        dwPassCalcx |= vx << (i<<2);
        dwpassword >>= 5;
        dwPassOld /= 10;
        dwPassOldx /= 10;
    }

    if (ori_password == dwPassCalc)
    {
        if(dwDayCalc >= 367){
            OnTimeOut(0);
            m_operation.cState = 0;
            m_operation.wIdentifyCode = 0;
            m_operation.ds9Year = 0;
            m_operation.ds9Month = 0;
            m_operation.ds9Day = 0;
            m_operation.ds10Year = 0;
            m_operation.ds10Month = 0;
            m_operation.ds10Day = 0;
            m_operation.ds8Month = 0;
            m_operation.ds8Day = 0;
            SavePayData();
            return dwDayCalc;  //超出一年，为无限
        }
        else{
            OnTimeOut(0);
            return dwDayCalc; //总天数
        }
    }
    if(ori_passwordx == dwPassCalcx){
        OnTimeOut(0);
        m_operation.cState = 0;
        m_operation.wIdentifyCode = 0;
        m_operation.ds9Year = 0;
        m_operation.ds9Month = 0;
        m_operation.ds9Day = 0;
        m_operation.ds10Year = 0;
        m_operation.ds10Month = 0;
        m_operation.ds10Day = 0;
        m_operation.ds8Month = 0;
        m_operation.ds8Day = 0;
        SavePayData();
        return -2; //超级密码解密
    }

    return -1; //解密失败
}
/************************************************************************/
/* 分期停机日期的设定                                                   */
/************************************************************************/
I64 PayDateTime(UI8 numPay,BOOL bStopTime)
{
	UI16 dsYear,dsMonth,dsDay;
#ifndef WIN32
	struct tm *p_tm;
#endif

	if(numPay == 1)
	{
		dsYear = m_operation.dsYear;
		dsMonth = m_operation.dsMonth;
		dsDay = m_operation.dsDay;
	}
	else if(numPay == 2)
	{
		dsYear = m_operation.ds2Year;
		dsMonth = m_operation.ds2Month;
		dsDay = m_operation.ds2Day;
	}
	else if(numPay == 3)
	{
		dsYear = m_operation.ds3Year;
		dsMonth = m_operation.ds3Month;
		dsDay = m_operation.ds3Day;
	}
	else if(numPay == 4)
	{
		dsYear = m_operation.ds4Year;
		dsMonth = m_operation.ds4Month;
		dsDay = m_operation.ds4Day;
	}
	else if(numPay == 5)
	{
		dsYear = m_operation.ds5Year;
		dsMonth = m_operation.ds5Month;
		dsDay = m_operation.ds5Day;
	}
	else if(numPay == 6)
	{
		dsYear = m_operation.ds6Year;
		dsMonth = m_operation.ds6Month;
		dsDay = m_operation.ds6Day;
	}

#ifndef WIN32
	dtnumpay = time(NULL);
	p_tm = localtime(&dtnumpay);
	memcpy(&dtNumPay, p_tm, sizeof(struct tm));

	dtNumPay.tm_hour = 0;
	dtNumPay.tm_min  = 0;
	dtNumPay.tm_sec  = 0;	
#endif

	dtNumPay.tm_year = 2000+dsYear-1900;
	dtNumPay.tm_mon = dsMonth-1;
	dtNumPay.tm_mday = dsDay;

	dtnumpay = mktime(&dtNumPay);
	dtnumpayx = dtnumpay + m_operation.tHour*3600+m_operation.tMinute*60;
	//OWARN("dtnumpay=%x",dtnumpay);
	if(bStopTime == TRUE)
	{
		return  dtnumpayx;
	}
	else 
	{
		return  dtnumpay;
	}

}
static int convert_days(int year, int mon, int day){
    int i;
    int monthlyd[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int leap_monthlyd[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    UI32 value = 0;
    for (i=0; i < mon; i++){
        if(year % 4 == 0){
            value += leap_monthlyd[i];
        } else {
            value += monthlyd[i];
        }
    }
    value += ((365*(year - 1)) + day + (year / 4));
    return value;
}
/************************************************************************/
/* 停机日期的监控判断                                                   */
/************************************************************************/
static void TimeOutMonitor()
{
	struct tm* t;
	time_t m_tmNow;
#ifndef WIN32
	struct tm *p_tm;
#endif

	if(m_operation.cState == 0) return;

    if(m_operation.cState == 1){
        time(&m_tmNow);
        t = localtime(&m_tmNow);

        if(m_operation.cMode == 0)
        {
            m_dttimeout = PayDateTime(PAYPERIODS_FIRST,TRUE) + m_operation.wInterval*(m_operation.cNowNum-1)*24*3600;
            m_dttimeoutalarm = PayDateTime(PAYPERIODS_FIRST,TRUE) + (m_operation.wInterval*(m_operation.cNowNum-1)-5)*24*3600;
        }
        else if(m_operation.cMode == 1)
        {
#ifndef WIN32
            m_dttimeout = time(NULL);
            p_tm = localtime(&m_dttimeout);
            memcpy(&m_dtTimeout, p_tm, sizeof(struct tm));

            m_dtTimeout.tm_hour = 0;
            m_dtTimeout.tm_min  = 0;
            m_dtTimeout.tm_sec  = 0;
#endif
            m_dtTimeout.tm_year = 2000+m_operation.dsYear-1900;
            m_dtTimeout.tm_mon = m_operation.dsMonth+m_operation.cNowNum-1-1;
            m_dtTimeout.tm_mday = m_operation.dsDay;

            m_dttimeout =mktime(&m_dtTimeout) + m_operation.tHour*3600+m_operation.tMinute*60;
            m_dttimeoutalarm = m_dttimeout - WARNING_DAYS;
        }
        else if(m_operation.cMode == 2)
        {
            m_dttimeoutalarm = PayDateTime(PAYPERIODS_FIRST,TRUE) -WARNING_DAYS;
            m_dttimeoutsecondalarm = PayDateTime(PAYPERIODS_SECOND,TRUE) - WARNING_DAYS;
            m_dttimeoutthirdalarm = PayDateTime(PAYPERIODS_THIRD,TRUE) - WARNING_DAYS;
            m_dttimeoutfourthalarm = PayDateTime(PAYPERIODS_FOURTH,TRUE) - WARNING_DAYS;
            m_dttimeoutfifthalarm = PayDateTime(PAYPERIODS_FIFTH,TRUE) - WARNING_DAYS;
            m_dttimeoutsixthalarm = PayDateTime(PAYPERIODS_SIXTH,TRUE) - WARNING_DAYS;
        }
        //OWARN("m_tmNow=%x",m_tmNow);
        //OWARN("m_dttimeout=%x",m_dttimeout);
        //OWARN("t->tm_year=%x",t->tm_year);
        if((m_operation.cMode == 1)||(m_operation.cMode == 0))
        {
            if (((m_tmNow>=m_dttimeout) || ((t->tm_year + 1900)<=2000) || bLowVolTage) && (m_operation.cPeriods>0))
            {
                OnTimeOut(1);
            }
            else if ((m_tmNow>=m_dttimeoutalarm) && (m_operation.cPeriods>0))
            {
                OnTimeOut(2);
            }
        }
        else if (m_operation.cMode == 2)
        {
            if ((m_operation.cNowNum==1) && (m_tmNow>=m_dttimeoutalarm)  && (m_operation.cPeriods>0)) OnTimeOut(2);
            if ((m_operation.cNowNum==2) && (m_tmNow>=m_dttimeoutsecondalarm) && (m_operation.cPeriods>0)) OnTimeOut(2);
            if ((m_operation.cNowNum==3) && (m_tmNow>=m_dttimeoutthirdalarm)  && (m_operation.cPeriods>0)) OnTimeOut(2);
            if ((m_operation.cNowNum==4) && (m_tmNow>=m_dttimeoutfourthalarm) && (m_operation.cPeriods>0)) OnTimeOut(2);
            if ((m_operation.cNowNum==5) && (m_tmNow>=m_dttimeoutfifthalarm)  && (m_operation.cPeriods>0)) OnTimeOut(2);
            if ((m_operation.cNowNum==6) && (m_tmNow>=m_dttimeoutsixthalarm)  && (m_operation.cPeriods>0)) OnTimeOut(2);

            if ((m_operation.cNowNum==1 && m_tmNow>=PayDateTime(PAYPERIODS_FIRST,TRUE)  || (t->tm_year+1900)<=2000 || bLowVolTage) && (m_operation.cPeriods>0)) OnTimeOut(1);
            if ((m_operation.cNowNum==2 && m_tmNow>=PayDateTime(PAYPERIODS_SECOND,TRUE) || (t->tm_year+1900)<=2000 || bLowVolTage) && (m_operation.cPeriods>0)) OnTimeOut(1);
            if ((m_operation.cNowNum==3 && m_tmNow>=PayDateTime(PAYPERIODS_THIRD,TRUE)  || (t->tm_year+1900)<=2000 || bLowVolTage) && (m_operation.cPeriods>0)) OnTimeOut(1);
            if ((m_operation.cNowNum==4 && m_tmNow>=PayDateTime(PAYPERIODS_FOURTH,TRUE) || (t->tm_year+1900)<=2000 || bLowVolTage) && (m_operation.cPeriods>0)) OnTimeOut(1);
            if ((m_operation.cNowNum==5 && m_tmNow>=PayDateTime(PAYPERIODS_FIFTH,TRUE)  || (t->tm_year+1900)<=2000 || bLowVolTage) && (m_operation.cPeriods>0)) OnTimeOut(1);
            if ((m_operation.cNowNum==6 && m_tmNow>=PayDateTime(PAYPERIODS_SIXTH,TRUE)  || (t->tm_year+1900)<=2000 || bLowVolTage) && (m_operation.cPeriods>0)) OnTimeOut(1);
        }
    }
    else if(m_operation.cState == 2){
        UI16 cur_y, cur_m, cur_d;
        UI16 autho_y, autho_m, autho_d;
        int diff_days;
        int left_days;
        time(&m_tmNow);
        t = localtime(&m_tmNow);
        cur_y = t->tm_year+1900;
        cur_m = t->tm_mon;
        cur_d = t->tm_mday;
        autho_y = m_operation.ds9Year+2000;
        autho_m = m_operation.ds9Month-1;
        autho_d = m_operation.ds9Day;
        diff_days = convert_days(cur_y, cur_m, cur_d) - convert_days(autho_y, autho_m, autho_d);
        left_days = m_operation.ds8Day - diff_days;

        if ((t->tm_year+1900)<=2000 || bLowVolTage)//20221026 dyl 扣电池或低电压导致时间错误
        {
            if(m_operation.ds8Month != 0){
                m_operation.ds8Month = 0;
                SavePayData();
            }
            OnTimeOut(1);
        }
        else
        {
        if(left_days >= 0){
            if(left_days != m_operation.ds8Month){
                m_operation.ds8Month = left_days;
                SavePayData();
            }
        }

        if(left_days <= -3){
            //剩余天数-3，过期
            if(m_operation.ds8Month != 0){
                m_operation.ds8Month = 0;
                SavePayData();
            }
            OnTimeOut(1);
        }
        else if(left_days <= 0){
            //剩余天数为0，快过期
            if(m_operation.ds8Month != 0){
                m_operation.ds8Month = 0;
                SavePayData();
            }
            OnTimeOut(2);
        }
        }
    }
}
/************************************************************************/
/* 分期停机功能的监控判断                                               */
/************************************************************************/
void PayMonitor()
{
	static UI16 second =0;
	static UI16 secondx =0;
	static UI32 time_1s =0;
    static UI16 time_5m = 0;

	if(m_operation.cState == 0) return;

	if (GetTick()-time_1s>=1000) //一小时检测一次
	{
		time_1s = GetTick();
		second++;
		secondx++;
		/*停机主机切手动 */
		if(secondx>=60)
		{
			secondx =0;
            time_5m++;
			if((m_operation.bTimeOut==1) && ((OperateModeIndex()==1) || (OperateModeIndex()==2) || (OperateModeIndex()==3)))
				ProtSysCtrl(PAYMENT_TIMEOUT_CMD,0,NULL);	
		}

        if(time_5m >= 5)
        {
            if(m_operation.cState && (bLowVolTage == FALSE))
            {
                if(!FileExist(LOWVOL_FILE)){
                    oprintf("not find lowvol-file ------5min\n");
                }
                else
                {
                    SaveCurTmLowVol();
                    oprintf("find lowvol-file , save date ---5min\n");
                }
            }
            time_5m = 0;
        }
		if(second>3600)
		{	
			second =0;
			TimeOutMonitor();
		}
	}

	if(g_bFirstMoni) //首次设定检测一次
	{
		g_bFirstMoni = FALSE;
		TimeOutMonitor();
	}
}

/************************************************************************/
/* iot MOD对HMI 分期付款 NET_FUNC
*/
/************************************************************************/
#ifndef WIN32
#define PAY_SIZE    (sizeof(IOT_PAY)/sizeof(UI16))  //按UI16计数
static BOOL IOT_GetPayState(PIOT_PAY pay)
{
    int i = 0;
    if(pay != NULL)
    {
        pay->wState=m_operation.cState;
        pay->wMode=m_operation.cMode;
        pay->wCnt=m_operation.cPeriods;
        pay->wStopTime=m_operation.tHour<<8|m_operation.tMinute;
        pay->wIntervalDay=m_operation.wInterval;

        for(i=0;i<8;i++)
        {
           pay->cMachineCode[i]=m_operation.machine2[i];
        }
        pay->wOutDateH=m_operation.doYear;
        pay->wOutDateL=m_operation.doMonth<<8|m_operation.doDay;
        pay->wOneDateH=m_operation.dsYear;
        pay->wOneDateL=m_operation.dsMonth<<8|m_operation.dsDay;
        pay->wTwoDateH=m_operation.ds2Year;
        pay->wTwoDateL=m_operation.ds2Month<<8|m_operation.ds2Day;
        pay->wThreeDateH=m_operation.ds3Year;
        pay->wThreeDateL=m_operation.ds3Month<<8|m_operation.ds3Day;
        pay->wFourDateH=m_operation.ds4Year;
        pay->wFourDateL=m_operation.ds4Month<<8|m_operation.ds4Day;
        pay->wFiveDateH=m_operation.ds5Year;
        pay->wFiveDateL=m_operation.ds5Month<<8|m_operation.ds5Day;
        pay->wSixDateH=m_operation.ds6Year;
        pay->wSixDateL=m_operation.ds6Month<<8|m_operation.ds6Day;
        pay->wIdentifyCode=m_operation.wIdentifyCode;
        pay->wNowCnt=m_operation.cNowNum;
        return TRUE;
    }
    return FALSE;
}

static BOOL IOT_SetPayState(PIOT_PAY pay)
{
    int i = 0;
    if(pay != NULL)
    {
        m_operation.cState = pay->wState;
        m_operation.cMode = pay->wMode;
        m_operation.cPeriods = pay->wCnt;
        m_operation.tHour = pay->wStopTime>>8;
        m_operation.tMinute = pay->wStopTime & 0xff;
        m_operation.wInterval = pay->wIntervalDay;
        for(i=0;i<8;i++)
        {
           m_operation.machine2[i] = pay->cMachineCode[i];
        }
        m_operation.doYear = pay->wOutDateH;
        m_operation.doMonth = pay->wOutDateL>>8;
        m_operation.doDay = pay->wOutDateL & 0xff;
        m_operation.dsYear = pay->wOneDateH;
        m_operation.dsMonth = pay->wOneDateL>>8;
        m_operation.dsDay = pay->wOneDateL & 0xff;
        m_operation.ds2Year = pay->wOneDateH;
        m_operation.ds2Month = pay->wOneDateL>>8;
        m_operation.ds2Day = pay->wOneDateL & 0xff;
        m_operation.ds3Year = pay->wThreeDateH;
        m_operation.ds3Month = pay->wThreeDateL>>8;
        m_operation.ds3Day = pay->wThreeDateL & 0xff;
        m_operation.ds4Year = pay->wFourDateH;
        m_operation.ds4Month = pay->wFourDateL>>8;
        m_operation.ds4Day = pay->wFourDateL & 0xff;
        m_operation.ds5Year = pay->wFiveDateH;
        m_operation.ds5Month = pay->wFiveDateL>>8;
        m_operation.ds5Day = pay->wFiveDateL & 0xff;
        m_operation.ds6Year = pay->wSixDateH;
        m_operation.ds6Month = pay->wSixDateL>>8;
        m_operation.ds6Day = pay->wSixDateL & 0xff;

        m_operation.wIdentifyCode = pay->wIdentifyCode;
        m_operation.cNowNum = pay->wNowCnt;
        return TRUE;
    }
    return FALSE;
}

static void IOT_PayCallback(void* pdat, int para1, int para2)
{

    PCMD150 ppkt = (PCMD150)pdat;
    if(ppkt != NULL)
    {
        switch(ppkt->ctrl_cmd)
        {
        case RX_PAYDATA:
            {
                IOT_PAY pay;
                if(IOT_GetPayState(&pay))
                {
                    IOT_Send150SubCmd(TX_PAYDATA,(UI16*)&pay,PAY_SIZE);
                }
            }
            break;
        case RX_PAYSET:
            {
                UI16 ok = FALSE;
                if((ppkt->data_len == PAY_SIZE) && (m_operation.cState == 0))
                {
                    PIOT_PAY ppay = (PIOT_PAY)ppkt->data;
                    if(IOT_SetPayState(ppay))
                    {
                        if(m_operation.cState == 0)
                        {
                            ok = FALSE;
                        }
                        else if(m_operation.machine2[0]==0
                                || m_operation.machine2[0]==' '
                                || strlen(m_operation.machine2)<6
                                || m_operation.wIdentifyCode < 10000
                                || m_operation.wIdentifyCode > 60000)
                        {
                            m_operation.cState = 0;
                            ok = FALSE;
                        }
                        else
                        {
                            ok = TRUE;
                            SavePayData();
                        }
                    }
                }

                IOT_Send150SubCmd(TX_PAYSET_REPLAY,&ok,1);
            }
            break;
        case RX_PAYRELEASE:
        {
            UI32 passwd = ppkt->data[0]<<16 | ppkt->data[1];
            UI16 ok = Pay(passwd) >= 0;
            if(ok)//解密成功显示主画面 回调函数？
            {
                PanelShowPage(PanelMainPage());
            }
            IOT_Send150SubCmd(TX_PAYRELEASE_REPLAY,&ok,1);
        }
            break;
        default:
            break;
        }
    }

}
#endif

/************************************************************************/
/* 分期停机功能的数据初始化                                             */
/************************************************************************/
void PaySetInital(PCMOD pmod)
{
	PAYSET_HEAD head;
    struct tm* t;
    time_t time_now;
    FILEHD filehd;

    time(&time_now);
    t = localtime(&time_now);

	if(CMOD_RHEAD_EX(&head))
	{
		memcpy(&m_operation,&head.m_operation,sizeof(OPERATION));
	}
	else
	{
		memcpy(&m_operation,&g_dbOperationDefault,sizeof(OPERATION));
	}

    if(!FileExist(PAY_TIME_ID_FILE)){
        filehd = FileCreate(PAY_TIME_ID_FILE);
        if(FILEHD_IS_OK(filehd)){
            snprintf(onlyid.only_id, sizeof(onlyid.only_id), "%02d%02d%02d%02d",t->tm_min,t->tm_hour,t->tm_sec,t->tm_mday);
            FileWrite(filehd, &onlyid, sizeof(onlyid), NULL);
            FileClose(filehd);
        }
    }
    else{
        filehd = FileOpen(PAY_TIME_ID_FILE);
        FileRead(filehd, onlyid.only_id, sizeof(onlyid.only_id), NULL);
        FileClose(filehd);
    }

    InitLowVol();

	TimeOutMonitor();

#ifndef WIN32 //NET_FUNC
    IOT_SetProtCb(IOT_RX_COMMAND_IOT,IOT_PayCallback);
#endif
}

/************************************************************************/
/* 分期停机功能的数据保存                                               */
/************************************************************************/
void SavePayData()
{
	PAYSET_HEAD head;

	memcpy(&head.m_operation,&m_operation,sizeof(OPERATION));
	CMOD_WHEAD_EX(&head);
}
/************************************************************************/
/* 分期设定数据核对                                                     */
/************************************************************************/
void CheckPaySet()
{
	I64 dtsecondx,dtthirdx,dtfourthx,dtfifthx,dtsixthx;
	int nerr = 0;

	if(m_operation.cState == 0)
	{
		SavePayData();
	}
    else if(m_operation.cState == 1)
	{
		if(strlen(m_operation.machine2) < 6)
		{
			nerr = 1;
		}
		else if(strlen(m_operation.machine2) > 8)
		{
			nerr = 2;
		}
		else if(m_operation.machine2[0] == 32) //空字符
		{
			nerr = 3;
		}
		else if((m_operation.wIdentifyCode < 10000)|| (m_operation.wIdentifyConfirm < 10000))
		{
			nerr = 4;
		}
		else if(m_operation.wIdentifyCode != m_operation.wIdentifyConfirm)
		{
			nerr = 6;
		}
		else if(m_operation.cMode == 2)
		{
			dtsecondx = PayDateTime(PAYPERIODS_SECOND,FALSE) - INTERVAL_DAYS;
			dtthirdx = PayDateTime(PAYPERIODS_THIRD,FALSE) - INTERVAL_DAYS;
			dtfourthx = PayDateTime(PAYPERIODS_FOURTH,FALSE) - INTERVAL_DAYS;
			dtfifthx = PayDateTime(PAYPERIODS_FIFTH,FALSE) - INTERVAL_DAYS;
			dtsixthx = PayDateTime(PAYPERIODS_SIXTH,FALSE) - INTERVAL_DAYS;

			if(m_operation.cPeriods == 2)
			{
				if((PayDateTime(PAYPERIODS_SECOND,FALSE)<PayDateTime(PAYPERIODS_FIRST,FALSE))||(dtsecondx<=PayDateTime(PAYPERIODS_FIRST,FALSE)))
				{
					nerr = 5;
				}
			}
			if(m_operation.cPeriods == 3)
			{
				if((PayDateTime(PAYPERIODS_SECOND,FALSE)<PayDateTime(PAYPERIODS_FIRST,FALSE))||(dtsecondx<=PayDateTime(PAYPERIODS_FIRST,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_THIRD,FALSE)<PayDateTime(PAYPERIODS_SECOND,FALSE))||(dtthirdx<=PayDateTime(PAYPERIODS_SECOND,FALSE)))
				{
					nerr = 5;
				}

			}
			if(m_operation.cPeriods == 4)
			{
				if((PayDateTime(PAYPERIODS_SECOND,FALSE)<=PayDateTime(PAYPERIODS_FIRST,FALSE))||(dtsecondx<=PayDateTime(PAYPERIODS_FIRST,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_THIRD,FALSE)<PayDateTime(PAYPERIODS_SECOND,FALSE))||(dtthirdx<=PayDateTime(PAYPERIODS_SECOND,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_FOURTH,FALSE)<PayDateTime(PAYPERIODS_THIRD,FALSE))||(dtfourthx<=PayDateTime(PAYPERIODS_THIRD,FALSE)))
				{
					nerr = 5;
				}
			}
			if(m_operation.cPeriods == 5)
			{
				if((PayDateTime(PAYPERIODS_SECOND,FALSE)<PayDateTime(PAYPERIODS_FIRST,FALSE))||(dtsecondx<=PayDateTime(PAYPERIODS_FIRST,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_THIRD,FALSE)<PayDateTime(PAYPERIODS_SECOND,FALSE))||(dtthirdx<=PayDateTime(PAYPERIODS_SECOND,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_FOURTH,FALSE)<PayDateTime(PAYPERIODS_THIRD,FALSE))||(dtfourthx<=PayDateTime(PAYPERIODS_THIRD,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_FIFTH,FALSE)<PayDateTime(PAYPERIODS_FOURTH,FALSE))||(dtfifthx<=PayDateTime(PAYPERIODS_FOURTH,FALSE)))
				{
					nerr = 5;
				}
			}
			if(m_operation.cPeriods == 6)
			{
				if((PayDateTime(PAYPERIODS_SECOND,FALSE)<PayDateTime(PAYPERIODS_FIRST,FALSE))||(dtsecondx<=PayDateTime(PAYPERIODS_FIRST,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_THIRD,FALSE)<PayDateTime(PAYPERIODS_SECOND,FALSE))||(dtthirdx<=PayDateTime(PAYPERIODS_SECOND,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_FOURTH,FALSE)<PayDateTime(PAYPERIODS_THIRD,FALSE))||(dtfourthx<=PayDateTime(PAYPERIODS_THIRD,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_FIFTH,FALSE)<PayDateTime(PAYPERIODS_FOURTH,FALSE))||(dtfifthx<=PayDateTime(PAYPERIODS_FOURTH,FALSE)))
				{
					nerr = 5;
				}
				if((PayDateTime(PAYPERIODS_SIXTH,FALSE)<PayDateTime(PAYPERIODS_FIFTH,FALSE))||(dtsixthx<=PayDateTime(PAYPERIODS_FIFTH,FALSE)))
				{
					nerr = 5;
				}
			}
		}
		//数据设置有问题，分期功能不启用
		if(nerr != 0)
		{
			m_operation.cState = 0;
			SavePayData();
		}
	}
    else if(m_operation.cState == 2)
    {
        if(m_operation.wIdentifyCode != m_operation.wIdentifyConfirm)
        {
            nerr = 6;
        }
        else if((m_operation.wIdentifyCode < 10000)|| (m_operation.wIdentifyConfirm < 10000))
        {
            nerr = 4;
        }
        if(nerr != 0)
        {
            m_operation.cState = 0;
            SavePayData();
        }
    }
	//设置非法或错误提示
	if((nerr > 0)&&(nerr < 5))
	{
		/*提示机器编码或识别码过短*/
		ShowMsg(VW_MSG_CHAR_NOENOUGH)
	}
	else if (nerr == 5)
	{
		/*提示输入非法日期*/
		ShowMsg(VW_MSG_ILLEGEL_DATES);
	}
	else if (nerr == 6)
	{
		/*提示识别码不一致*/
		ShowMsg(VW_MSG_ERR_ID);
	}
	else
	{
		g_bInstallAsk = TRUE;
		PanelShowDialogByName(DIAG_INSTALL_INFORMATION);

	}
}
/************************************************************************/
/* 分期停机询问框是否弹出                                              */
/************************************************************************/
BOOL IsInstallAsk()
{
	return g_bInstallAsk;
}
/************************************************************************/
/* 设定日期范围的最大值                                                 */
/************************************************************************/
void SetDayMax(PWGT pwgt,UI16 year ,UI16 month)
{
	BOOL  bLeapYear;

	if (((year+2000)%4==0)&&((year+2000)%100!=0)||((year+2000)%400==0))
	{
		bLeapYear=TRUE;
	}
	else
	{
		bLeapYear=FALSE;
	}
	if(month ==2)
	{
		if(bLeapYear)
		{
			EditSetMax(pwgt,29);
		}
		else
		{
			EditSetMax(pwgt,28);
		}
	}
	else if ((month==4)||(month==6)||(month==9)||(month==11))
	{
		EditSetMax(pwgt,30);
	}
	else
	{
		EditSetMax(pwgt,31);
	}
}
/************************************************************************/
/* 日期修改的控制码                                                     */
/************************************************************************/
UI16 ControlPass(UI8 year,UI8 month,UI8 day,UI16 random)
{
	UI16 value;
	UI16 Periods[20]={39121,23629,57760,56722,29862,
		10549,27621,32189,23874,51298,
		41905,10659,30168,60468,61480,
		29168,41975,21759,16554,15407};

	value=(year*100+month*10+day) ^
		Periods[random%20] ^
		random^
		PayIdentifyCode();

	return value;
}

/*******************分期停机功能--低电压功能**********************************/
typedef struct LOWVOLTAGEDATE
{
    UI16 year;
    UI16 month;
    UI16 day;
}LowVoltageDate;

LowVoltageDate g_lowvol;

//保存本次收到低电压的时间
void SaveCurTmLowVol()
{
    struct tm* t;
    time_t time_now;
    FILEHD filehd;

    time(&time_now);
    t = localtime(&time_now);
    g_lowvol.day = t->tm_mday;
    g_lowvol.month = t->tm_mon;
    g_lowvol.year = t->tm_year;

    filehd = FileOpen(LOWVOL_FILE);
    if(FILEHD_IS_OK(filehd))
    {
        FileWrite(filehd, &g_lowvol, sizeof(LowVoltageDate), NULL);
    }
    else
    {
        FileCreate(LOWVOL_FILE);
        FileWrite(filehd, &g_lowvol, sizeof(LowVoltageDate), NULL);
    }
        FileClose(filehd);
}

//读取最近一次收到低电压的时间
void ReadLastTmLowVol()
{
    FILEHD filehd;
    filehd = FileOpen(LOWVOL_FILE);
    FileRead(filehd, &g_lowvol, sizeof(LowVoltageDate), NULL);
    FileClose(filehd);
}

//比较当前时间和低电压时间 0：小于180天 1：大于180天 2：当前时间在保存时间前面
static int CheckLowVolTm()
{
    struct tm* t;
    time_t time_now;
    LowVoltageDate cur_tm;
    UI32 cur, last;

    time(&time_now);
    t = localtime(&time_now);
    cur_tm.day = t->tm_mday;
    cur_tm.year = t->tm_year;
    cur_tm.month = t->tm_mon;

    last = g_lowvol.day + g_lowvol.month*30 + g_lowvol.year*365;
    cur = cur_tm.day + cur_tm.month*30 + cur_tm.year*365;
    if(cur < last){
        oprintf("cur_time < lowvol_time\n");
        return 2;
    }
    else if(cur - last >= 180){
        oprintf("cur_time - lowvol_time >= 180 days\n");
        return 1;
    }
    else{
        oprintf("cur_time - lowvol_time < 180 days\n");
        return 0;
    }
    //oprintf("cur_year: %d, cur_month: %d, cur_day: %d\n", cur_tm.year+1900, cur_tm.month+1, cur_tm.day);
    //oprintf("lowvol_year: %d, lowvol_month: %d, lowvol_day: %d\n", g_lowvol.year+1900, g_lowvol.month+1, g_lowvol.day);
}

#ifndef WIN32
#ifdef IMX6
#include "unistd.h"
#else
extern rt_uint8_t rt_rtc_lowvoltage(void);
#endif
#endif
//检测是否有低电压
static void InitLowVol()
{
#ifndef WIN32
#ifdef IMX6
    if(m_operation.cState)
    {
        FILEHD filehd;
        char* findok;
        char dmesg[100];
        const char* lowvol = "low voltage detected";
        osystem("dmesg | grep 'low voltage detected' > /pages/save/openinfo.txt");
        filehd = FileOpen(OPENINFO_FILE);
        FileRead(filehd, dmesg, sizeof(dmesg), NULL);
        if(strstr(dmesg, lowvol))
        {
            oprintf("find lowvoltage--------\n");
            if(access(LOWVOL_FILE, F_OK) != 0)
            {
                oprintf("find lowvoltage----file not exist\n");
                SaveCurTmLowVol();
                bLowVolTage = FALSE;
            }
            else
            {
                ReadLastTmLowVol();
                oprintf("find lowvoltage---existed year:%d month:%d day:%d\n", g_lowvol.year+1900, g_lowvol.month+1, g_lowvol.day);
                switch (CheckLowVolTm()) {
                case 0:
                    SaveCurTmLowVol();
                    osystem("hwclock -w");
                    bLowVolTage = FALSE;
                    break;
                case 1:
                    bLowVolTage = TRUE;
                    break;
                case 2:
                    bLowVolTage = TRUE;
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            oprintf("no lowvoltage--------\n");
            SaveCurTmLowVol();
            bLowVolTage = FALSE;
        }
        FileClose(filehd);
        TimeOutMonitor();
    }
    else
    {
        oprintf("no pay----------------\n");
        if(access(OPENINFO_FILE, F_OK) == 0)
            osystem("rm -f /pages/save/openinfo.txt");
        if(access(LOWVOL_FILE, F_OK) == 0)
            osystem("rm -f /pages/save/lowvol.dat");
    }
#else
    if(m_operation.cState)
    {
        if(rt_rtc_lowvoltage() == 1)  //rtt---  1：有低电压 0：正常
        {
            oprintf("find lowvoltage--------\n");
            if(!FileExist(LOWVOL_FILE))
            {
                oprintf("find lowvoltage----file not exist\n");
                SaveCurTmLowVol();
                bLowVolTage = FALSE;
            }
            else
            {
                ReadLastTmLowVol();
                oprintf("find lowvoltage---existed year:%d month:%d day:%d\n", g_lowvol.year+1900, g_lowvol.month+1, g_lowvol.day);
                switch (CheckLowVolTm()) {
                case 0:
                    SaveCurTmLowVol();
                    bLowVolTage = FALSE;
                    break;
                case 1:
                    bLowVolTage = TRUE;
                    break;
                case 2:
                    bLowVolTage = TRUE;
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            oprintf("no lowvoltage--------\n");
            SaveCurTmLowVol();
            bLowVolTage = FALSE;
        }
        TimeOutMonitor();
    }
    else
    {
        oprintf("no pay----------------\n");
        if(FileExist(LOWVOL_FILE))
            FileDelete(LOWVOL_FILE);
    }

#endif
#endif
}
