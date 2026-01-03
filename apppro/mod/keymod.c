#include "keymod.h"
#include "oprintf.h" 
#include "protcmd.h"
#include "key.h"
#include "mxml.h"
#include "file.h"
#include "database.h"
#include "servo.h"
#include "panel.h"
#include "canmod.h"
#include "keyprot.h"
#include "pay.h"
#include "language.h"
#include "login.h"
#include "information.h"
#include "verinfo.h"
#include "imagehash.h"
#include "panelcfg.h"
#include "keydef.h"
#include "usermod.h"
#include "temp.h"
#include "keyrec.h"
#include "tabpagewgt.h"
#include "keyprot2.h"
#include "udisk.h"
#include "keydefine.h"
#include "ctrlcmd.h"
#include "editwgt.h" //20220104 dyl

//20250319 chj 优化点屏幕功能
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//core and blow group
#define COREGROUP_CNT      6
#define BLOWGROUP_CNT      14 //7-》14 20241210 chj 28组吹气

//20210804 定义10000以上的控件索引作为光标跳进TABPAGE控件的标记
#define JUMPINTABPAGE_FLAG_MIN      10000
#define JUMPINTABPAGE_FLAG_MAX      10020

#define  _KEY_PANEL_ALARMCLEAR      0x6008     //警报清除

#define COREA_FL_CORE_CTRL			0x22130001 //中子A功能选择
#define BLOW1_TM_MOVINGPLATENACT	0x23130003 //吹气1动作时间
#define BLOW2_TM_MOVINGPLATENACT	0x23230003 //吹气2动作时间
#define ADJUST_MACH_FL_JOG_FUN      0x2412000d //调模调试功能
#define EJECTFUNC_SENSOR			0x33120052 //电眼使用
#define ADDR_CORE_KEY_SELECT		0x3312006E //中子切换按键  20190505 LET 立式机地址
#define ADDR_AIRBLOW_KEY_SELECT     0x3312006F //吹气切换按键  20190505 LET 立式机地址
#define SCREEN_SHOT                 0x33120070 //20190430截屏功能（天剑风格用）
#define SERVO_SELFLEARNSWITCH		0x331300d4 //伺服自学习开关
#define PT15_USER_INITRANK          0x33170025 //用户初始等级 //20190523.cyx
#define AUTO_ADJ                    0x33190000 //20190527 CHZ

#define  KEY_MAXITEMS	(100)
static PHASH_TBL g_keyHash = NULL;//物理键与发送键哈希表
static PHASH_TBL g_keyledHash = NULL;//物理灯与发送键哈希表
#define MAP_ADDKEY(KEYID,SOURCEKEY)	HashTblAddPairByInt(g_keyHash,KEYID,(void*)SOURCEKEY)	
#define MAP_ADDLED(KEYID,LED) HashTblAddPairByInt(g_keyledHash,KEYID,(void*)LED)

BOOL m_isInForce = FALSE;//强制输出标记
BOOL g_bButtonPress =FALSE;//上下按键长按标记
BOOL g_bEnterPress =FALSE;//确认按键长按标记
extern UI16 g_tmNoKeyPress;//是否有键按下
UI16 g_tmPanelNoKeyPress=0;//20200221 区分底层和应用层计时
BOOL g_bhelplink=FALSE;//是否进入主画面帮助连接
BOOL g_bhelplinkx=FALSE;//是否进入主画面帮助警报连接

static UI16 g_wKeySel;//key switch
static UI16 g_cKeyState=_KEY_PANEL_KEYLOCK; //data lock

static TESTKEY_CB testkey_cb = NULL; //按键测试页面
static DAADJUST_CB daadjust_cb = NULL;//DA调整

static MACHCFG_CB machcfg_change_cb = NULL;
static MACHCFG5200_CB machcfg5200_change_cb = NULL;
static PUMPFUNC_CB pumpfunc_change_cb = NULL;  //20241223 fqh pump_func画面修改的标志位
static DAADJ_CB   daadj_change_cb = NULL;
static ADEX_CB   adex_change_cb = NULL;
static ADX_CB    adx_change_cb  = NULL;
static TAB_CB tab_cb = NULL;
static ECAT_CB ecat_cb = NULL;
static MAIN_CB main_cb = NULL;
static DATADIAG_CB datadiag_cb = NULL;
static CANCELFOCUS_CB cancelfocuscb = NULL;
static SERVOSTATEX1_CB servostatex1_cb = NULL;
static SPECFUNC_SELECT_CB specfuncselect_change_cb = NULL;

static IO_OUT_CB  ioout_force_cb[MAX_IO_OUT_CB] = {NULL}; //20190507 输出一页面强制输出值归零
static IO_OUT2_CB  ioout2_force_cb[MAX_IO_OUT_CB] = {NULL}; //20190507 输出二页面强制输出值归零

extern UI32 g_warm1,g_warm2,g_warm3,g_warm4,g_warm5,g_warm6,g_warm7,g_warm8,g_warm9,g_warm10;
extern BOOL g_bwarn;
extern UI32 dwPanelErr;
extern int PUMPFUNC_CHANGE_FLAG;

//panel10 core air key
extern BOOL g_corekeylock;//中子按键失效
extern BOOL g_airkeylock; //吹气按键失效
extern UI16 g_airkey_active;
extern UI16 g_corekey_active;
extern UI16 g_keycore_sel;
extern UI16 g_keyair_sel;
extern UI16 g_coretime;
extern UI16 g_airtime;

extern BOOL nFaceIn;//20230518 chj 开机进入FACE页面，显示数据加载中

//language
extern LANG_ID m_currentLangId;

//login 20180823 hz
extern PPAGE_FRAM m_pwdJumpPages[];

static BOOL bLCD_state=TRUE;//20230518 chj 优化关屏调用频率
//keyinfo
KEYINFO m_keyinfo;

//void KeyModInital(PCMOD pmod)
//{
	//1.Register functions
	//InitKeyMap();
//}
static BOOL InitKeyMap();//按键默认表

UI16 SetKeySel(UI16 keysel_bit)
{
	return g_wKeySel |= keysel_bit;
}

UI16 ClrKeySel(UI16 keysel_bit)
{
	return g_wKeySel &= ~keysel_bit;
}

BOOL ChkKeySel(UI16 keysel_bit)
{
	return g_wKeySel & keysel_bit;
}

BOOL SetHelpLink(BOOL b_link)
{
	return g_bhelplink = b_link;
}

BOOL SetHelpLinkx(BOOL b_linkx)
{
	return g_bhelplinkx = b_linkx;
}
/************************************************************************/
/* 解析按键xml数据到keyinfo                                             */
/************************************************************************/
static void ParseKeyXmlFile(mxml_node_t* toptree,KEYINFO* keyinfo)
{
	mxml_node_t * node = NULL;
	const char *pvalue;
	UI32 value;
	UI16 keylimit ;
	UI16 namelimit;
	UI32 * pdata;

	if(keyinfo == NULL)
		return;

	//key 值
	pdata = (UI32*)(&keyinfo->keytbl);
	for(node = mxmlFindElement(toptree,toptree,"KeyType",NULL,NULL,MXML_DESCEND),keylimit=0;
		node != NULL;
		node = mxmlFindElement(node,toptree,"KeyType",NULL,NULL,MXML_DESCEND))
	{
		pvalue = mxmlElementGetAttr(node,"PhyKey");//“物理键”
		if(pvalue!=NULL && (keylimit < PANEL_FUNC_MAX))//最大80个按键值
		{
			sscanf(pvalue,"%X",&value);
			*(pdata+PANEL_TBL_CNT*keylimit) = value;
		}

		pvalue = mxmlElementGetAttr(node,"PhyLight");//“物理灯”
		if(pvalue!=NULL && (keylimit < PANEL_FUNC_MAX))//最大80个按键值
		{
			sscanf(pvalue,"%X",&value);
			*(pdata+PANEL_TBL_CNT*keylimit+1) = value;
		}

		pvalue = mxmlElementGetAttr(node,"SendValue");//“发送值”
		if(pvalue!=NULL && (keylimit < PANEL_FUNC_MAX))//最大80个按键值
		{
			sscanf(pvalue,"%X",&value);
			*(pdata+PANEL_TBL_CNT*keylimit+2) = value;
		}

		pvalue = mxmlElementGetAttr(node,"KeyNum");//“按键内部定义用”
		if(pvalue!=NULL && (keylimit < PANEL_FUNC_MAX))//最大80个按键值
		{
			sscanf(pvalue,"%X",&value);
			*(pdata+PANEL_TBL_CNT*keylimit+3) = value;
		}

		pvalue = mxmlElementGetAttr(node,"NewSendValue");//“发送新值”
		if(pvalue!=NULL && (keylimit < PANEL_FUNC_MAX))//最大80个按键值
		{
			sscanf(pvalue,"%X",&value);
			*(pdata+PANEL_TBL_CNT*keylimit+4) = value;
		}

		pvalue = mxmlElementGetAttr(node,"NewKeyNum");//“重定义后新值”
		if(pvalue!=NULL && (keylimit < PANEL_FUNC_MAX))//最大80个按键值
		{
			sscanf(pvalue,"%X",&value);
			*(pdata+PANEL_TBL_CNT*keylimit+5) = value;
		}

		keylimit++;
	}
	//按键名称
	for(node = mxmlFindElement(toptree,toptree,"KeyType",NULL,NULL,MXML_DESCEND),namelimit=0;
		node != NULL;
		node = mxmlFindElement(node,toptree,"KeyType",NULL,NULL,MXML_DESCEND))
	{
		pvalue = mxmlElementGetAttr(node,"Chinese");//"中文"
		if(pvalue!=NULL && (namelimit < PANEL_FUNC_MAX))
		{
			keyinfo->keyname[namelimit].pcn = pvalue;
		}
		pvalue = mxmlElementGetAttr(node,"English");//"英文"
		if(pvalue!=NULL && (namelimit < PANEL_FUNC_MAX))
		{
			keyinfo->keyname[namelimit].pen = pvalue;
		}
		namelimit++;
	}
}
/************************************************************************/
/* 功能： 读取XML文件按键数据                                           */
/************************************************************************/
static BOOL GetKeyCfgInfo(const char* filename,KEYINFO* keyinfo)
{
	int size;
	char *buf=NULL;
	FILEHD fd;
	mxml_node_t * tree;

	fd = FileOpen(filename);
	if(FILEHD_IS_OK(fd))
	{
		size = FileGetSize(fd,filename);
		buf = (char*)OBJMemMalloc(size + 1);
		if(buf != NULL)
		{
			FileRead(fd,buf,size,NULL);
			FileClose(fd);
			/*pad EOF at tail*/
			buf[size] = '\0';
			tree = mxmlLoadString(NULL,buf,MXML_NO_CALLBACK);
			if(tree != NULL)
			{
				ParseKeyXmlFile(tree,keyinfo);
				mxmlDelete(tree);
			}
			else
			{
				oprintf("mxmlLoadString error\n");
			}
			OBJMemFree(buf);
		}
		return TRUE;
	}

	return FALSE;
}
/************************************************************************/
/* 加载按键xml数据                                                      */
/************************************************************************/
void XmlToMapKey()
{
	int i,j;
	char keydefpath[128]={0};

	snprintf(keydefpath,sizeof(keydefpath)-1,DATA_PATH"keycfg.xml");

	memset(&m_keyinfo,0,sizeof(KEYINFO));

	if(GetKeyCfgInfo(keydefpath,&m_keyinfo))
	{
		//物理值与发送值的哈希表
		if(g_keyHash == NULL)
		{
			if((g_keyHash = HashTblAlloc(KEY_MAXITEMS))!= NULL)
			{
				for(i=0;i<PANEL_FUNC_MAX;i++)
				{
					if(m_keyinfo.keytbl[i].keynum != m_keyinfo.keytbl[i].newkeynum)
					{
						MAP_ADDKEY(m_keyinfo.keytbl[i].phykey,m_keyinfo.keytbl[i].newsendkey);
					}
					else
					{
						MAP_ADDKEY(m_keyinfo.keytbl[i].phykey,m_keyinfo.keytbl[i].sendkey);
					}
				}
			}
		}
		//物理灯与发送值的哈希表
		if(g_keyledHash == NULL)
		{
			if((g_keyledHash = HashTblAlloc(PANEL_FUNC_MAX))!= NULL)
			{
				for(j=0;j<PANEL_FUNC_MAX;j++)
				{
					MAP_ADDLED(m_keyinfo.keytbl[j].newsendkey,m_keyinfo.keytbl[j].phylight);
				}
			}
		}
		m_keyinfo.cfgflag = TRUE;
	}
	else
	{
		InitKeyMap();
		m_keyinfo.cfgflag = FALSE;
	}
}

/************************************************************************/
/* 下位机按键转换表(默认表)                                             */
/************************************************************************/
static BOOL InitKeyMap()
{
	if(g_keyHash==NULL)
	{
		if((g_keyHash = HashTblAlloc(KEY_MAXITEMS))!= NULL)
		{
			MAP_ADDKEY(_KEY_PANEL_MOLDOPEN,0x2013);
			MAP_ADDKEY(_KEY_PANEL_MOLDCLOSE,0x2011);
			MAP_ADDKEY(_KEY_PANEL_CHARGE,0x1012);
			MAP_ADDKEY(_KEY_PANEL_AUTOPURGE,0x101B);
			MAP_ADDKEY(_KEY_PANEL_MOLDADJ,0x01);
			MAP_ADDKEY(_KEY_PANEL_INJECT,0x1011);
			MAP_ADDKEY(_KEY_PANEL_SUCKBACK,0x1013);
			MAP_ADDKEY(_KEY_PANEL_COREAIN,0x2211);
			MAP_ADDKEY(_KEY_PANEL_COREAOUT,0x2212);
			MAP_ADDKEY(_KEY_PANEL_MOLDTHICK,0x2412);
			MAP_ADDKEY(_KEY_PANEL_MOLDTHIN,0x2411);
			MAP_ADDKEY(_KEY_PANEL_EJECTRET,0x2113);
			MAP_ADDKEY(_KEY_PANEL_EJECTADV,0x2111);
			MAP_ADDKEY(_KEY_PANEL_COREBIN,0x2221);
			MAP_ADDKEY(_KEY_PANEL_COREBOUT,0x2222);

			MAP_ADDKEY(_KEY_PANEL_AIRBLASTMOV,0x2311);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTSTAT,0x2321);
			MAP_ADDKEY(_KEY_PANEL_NOZZLEADV,0x1111);

			MAP_ADDKEY(_KEY_PANEL_NOZZLERET,0x1112);
			MAP_ADDKEY(_KEY_PANEL_MOTOR,0x0411);
			MAP_ADDKEY(_KEY_PANEL_HEATER,0x0020);
			MAP_ADDKEY(_KEY_PANEL_LUBRICATION,0x3001);
			//MAP_ADDKEY(_KEY_PANEL_CLOSE,0x2013);
			//MAP_ADDKEY(_KEY_PANEL_OPEN,0x2013);
			MAP_ADDKEY(_KEY_PANEL_MANUAL,0x34);
			MAP_ADDKEY(_KEY_PANEL_SEMIAUTO,0x02);
			MAP_ADDKEY(_KEY_PANEL_SENSORAUTO,0x04);
			MAP_ADDKEY(_KEY_PANEL_TIMEAUTO,0x03);
			MAP_ADDKEY(_KEY_PANEL_RUN,0x05);
			//MAP_ADDKEY(_KEY_PANEL_SW1,0x2013);

			MAP_ADDKEY(_KEY_PANEL_AIRBLASTMOV2,0x2331);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTSTAT2,0x2341);
			MAP_ADDKEY(_KEY_PANEL_DOOROPEN,0x2911);
			MAP_ADDKEY(_KEY_PANEL_DOORCLOSE,0x2912);
			MAP_ADDKEY(_KEY_PANEL_CORECIN,0x2231);
			MAP_ADDKEY(_KEY_PANEL_CORECOUT,0x2232);
			MAP_ADDKEY(_KEY_PANEL_COREDIN,0x2241);
			MAP_ADDKEY(_KEY_PANEL_COREDOUT,0x2242);
			//MAP_ADDKEY(_KEY_PANEL_SW2,0x2013);
			//MAP_ADDKEY(_KEY_PANEL_SW3,0x2013);
			MAP_ADDKEY(_KEY_PANEL_COREEIN,0x2251);
			MAP_ADDKEY(_KEY_PANEL_COREEOUT,0x2252);
			MAP_ADDKEY(_KEY_PANEL_COREFIN,0x2261);
			MAP_ADDKEY(_KEY_PANEL_COREFOUT,0x2262);

			//MAP_ADDKEY(_KEY_PANEL_MOLDADJNEW,0x01);
			//MAP_ADDKEY(_KEY_PANEL_AUTOMOLDADJ,0x2415);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTMOV3,0x2351);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTSTAT3,0x2361);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTMOV4,0x2371);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTSTAT4,0x2381);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTMOV5,0x2391);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTSTAT5,0x23A1);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTMOV6,0x23B1);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTSTAT6,0x23C1);

			MAP_ADDKEY(_KEY_PANEL_AIRBLASTMOV7,0x23D1);
			MAP_ADDKEY(_KEY_PANEL_AIRBLASTSTAT7,0x23E1);
			MAP_ADDKEY(_KEY_PANEL_MULTIEJECT,0x2111);
            MAP_ADDKEY(_KEY_PANEL_PRESADJUST,0x0911);   //给下位机用 伺服自学习压力校准 20191119 hz

            MAP_ADDKEY(_KEY_PANEL_PITCHSELFLEARN,0x241A); //齿距自学习 //20211224 dyl

			return TRUE;
		}
	}
	return FALSE;
}
/************************************************************************/
/* 通过物理按键找到发送按键                                             */
/************************************************************************/
UI16 KeyToMap(UI32 oriKey)
{
    return ((UI16)HashTblFindNumberByInt(g_keyHash,oriKey));
}

/************************************************************************/
/* 通过发送按键找到物理灯                                               */
/************************************************************************/
UI32 KeyToLed(UI32 sendkey)
{
    return ((UI32)HashTblFindNumberByInt(g_keyledHash,sendkey));
}

/************************************************************************/
/* 按键XML加载标记                                                      */
/************************************************************************/
UI16 IsKeyByXml()
{
	return m_keyinfo.cfgflag;  
}

/************************************************************************/
/* 面板按键发送及过滤                                                   */
/************************************************************************/
void SendPanelKey(UI32 wKey,UI16 press)    //发送键值
{ 
	static UI32 g_wPanelKey = _KEY_NULL;
    UI16 sendkey;

	if (g_wPanelKey==_KEY_NULL && wKey==_KEY_NULL) return;
	if (g_wPanelKey!=_KEY_NULL && g_wPanelKey!=wKey) wKey=_KEY_NULL;

	if(TRUE){  //运行键使用
		if (wKey==_KEY_PANEL_SENSORAUTO)  wKey=_KEY_PANEL_RUN;
		else if (wKey==_KEY_PANEL_TIMEAUTO && VarAdrToUI16(EJECTFUNC_SENSOR)) wKey=_KEY_PANEL_SENSORAUTO;
	}

	//key filter
    {
        if((wKey!=_KEY_PANEL_RUN) && (wKey!=_KEY_PANEL_DOORCLOSE) && (wKey!=_KEY_PANEL_DOOROPEN) && (wKey!=_KEY_PANEL_MANUAL) && (wKey!=_KEY_NULL))
        {
            if (OperateModeIndex()==4)            //MODE_MACROADJ
            {
                if ((wKey== _KEY_PANEL_SEMIAUTO) || (wKey==_KEY_PANEL_SENSORAUTO) || (wKey==_KEY_PANEL_TIMEAUTO))
                    return;
            }
            else if (OperateModeIndex()==11)     //MODE_AUTOADJMOLD
            {
                if  ((wKey!=_KEY_PANEL_MOLDADJ && (!KeyDefine_Def(wKey, KEY_MOLDADJ))) && (wKey!=_KEY_PANEL_AUTOMOLDADJ) && (wKey!=_KEY_PANEL_MOLDADJNEW))
                    return;
            }
            else if (OperateModeIndex()==7)     //MODE_AUTOPURGE
            {
                if  ((wKey!=_KEY_PANEL_AUTOPURGE && (!KeyDefine_Def(wKey, KEY_AUTOPURGE))) )
                    return;
            }
            else if (OperateModeIndex()>0)
            {
                if((OperateModeIndex()==1)&&((wKey==_KEY_PANEL_SENSORAUTO)||(wKey==_KEY_PANEL_TIMEAUTO)));  //20141225，半自动时，全自动或电眼自动按键有效
                else if(((OperateModeIndex()==2)||(OperateModeIndex()))&&(wKey==_KEY_PANEL_SEMIAUTO));  //20141225，全自动或电眼自动时，半自动按键有效
                else if((OperateModeIndex()==3) && (wKey==_KEY_PANEL_SENSORAUTO));//20150720,全自动时，与电眼按键可以切换；
                else if((OperateModeIndex()==2) && (wKey==_KEY_PANEL_TIMEAUTO)); //20150720,电眼时，与全自动按键可以切换；
                else if((OperateModeIndex()==15)&&(wKey==_KEY_PANEL_PITCHSELFLEARN));//20211230 dyl 齿距自学习
                else if((wKey ==_KEY_PANEL_LUBRICATION || (KeyDefine_Def(wKey, KEY_LUBRICATE))));//20241218 chj 润滑按键不屏蔽 正常润滑模式
                else if((wKey ==_KEY_LUBRICATION1 || wKey ==_KEY_LUBRICATION2)&&(VarAdrToInt(d_autoctrl1_MOLDSET_KEY_LUBR2_INDEPEND) == 1));//20241218 chj 润滑按键不屏蔽 润滑2按键 润滑模式
                else
                    return;
            }

            if (JudgeChargeActStep())  //charge
            {
                //20160728
                if((OperateModeIndex()==1)&&((wKey==_KEY_PANEL_SENSORAUTO)||(wKey==_KEY_PANEL_TIMEAUTO)));  //20141225，半自动时，全自动或电眼自动按键有效
                else if(((OperateModeIndex()==2)||(OperateModeIndex()==3))&&(wKey==_KEY_PANEL_SEMIAUTO));  //20141225，全自动或电眼自动时，半自动按键有效
                else if((OperateModeIndex()==3) && (wKey==_KEY_PANEL_SENSORAUTO));//20150720,全自动时，与电眼按键可以切换；
                else if((OperateModeIndex()==2) && (wKey==_KEY_PANEL_TIMEAUTO)); //20150720,电眼时，与全自动按键可以切换；
                else if (wKey==_KEY_PANEL_CHARGE || KeyDefine_Def(wKey, KEY_CHARGE));
                else
                    return;
                //if (wKey!=_KEY_PANEL_CHARGE)  return;
            }
        } //key filter
    }

    if(VarAdrToUI16(MOTOR_STATE_STATE)== 2){
        if ((wKey!=_KEY_PANEL_MOTOR && (!KeyDefine_Def(wKey, KEY_MOTOR))) && (wKey!=_KEY_PANEL_MANUAL) && (wKey!=_KEY_NULL)) return;
	}

    if (OperateModeIndex()==0 && JudgeChargeActStep())  //charge
	{
        if ((wKey!=_KEY_PANEL_CHARGE && (!KeyDefine_Def(wKey, KEY_CHARGE))) && (wKey!=_KEY_PANEL_MANUAL) && (wKey!=_KEY_NULL)) return;
	}

    if(JudgeActStep(0x11170000, 0x1117FFFF) && wKey!=_KEY_PANEL_OILHEAT)//20210302 油温预热时对所有动作按键做限制
    {
        return;
    }

    if(JudgeSuckBackActStep() && (OperateModeIndex()==0))//20230518 chj增加射退动作判断
    {
        if (((wKey!=_KEY_PANEL_SUCKBACK) && (!KeyDefine_Def(wKey, KEY_SUCKBACK))) && (wKey!=_KEY_PANEL_MANUAL) && (wKey!=_KEY_NULL)) return;
    }

    if(JudgeAdjMoldAdvActStep() && (OperateModeIndex()==4))//20230518 chj增加调模动作判断
    {
        if (((wKey!=_KEY_PANEL_MOLDTHIN) && (!KeyDefine_Def(wKey, KEY_MOLDTHIN))) && (wKey!=_KEY_PANEL_MANUAL) && (wKey!=_KEY_NULL)) return;
    }

    if(JudgeAdjMoldRetActStep() && (OperateModeIndex()==4))
    {
        if (((wKey!=_KEY_PANEL_MOLDTHICK) && (!KeyDefine_Def(wKey, KEY_MOLDTHICK))) && (wKey!=_KEY_PANEL_MANUAL) && (wKey!=_KEY_NULL)) return;
    }

    if(m_isInForce)
    {
        int i;

        m_isInForce = FALSE;
        ProtSysCtrl(EXIT_PO_CMD, 0, NULL);

        //force out to zero
        for(i = 0;i < MAX_IO_OUT_CB;i++)
        {
            if(ioout_force_cb[i] !=NULL)
            {
                (*ioout_force_cb[i])();
            }
        }

        for(i = 0;i < MAX_IO_OUT_CB;i++)
        {
            if(ioout2_force_cb[i] !=NULL)
            {
                (*ioout2_force_cb[i])();
            }
        }
    }

    /*send key*/
	if(wKey !=_KEY_NULL)
    {
        //20220805 dyl
        #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
        if(wKey == _KEY_PANEL_SWX1 || wKey == _KEY_PANEL_SWX2 || wKey == _KEY_PANEL_SWX3
                 || wKey == _KEY_PANEL_SWX6 || wKey == _KEY_PANEL_SWX7 || wKey == _KEY_PANEL_SWX8)//20210118 备用键按键自定义
        #else//G10/G15:KeyBoard
        if(((wKey == _KEY_PANEL_SW2 || wKey == _KEY_PANEL_SW3) && (PanelCfgSize()<DISPLAY_SIZE_15))
            || ((wKey == _KEY_PANEL_SW2 || wKey == _KEY_PANEL_SW3 || wKey == _KEY_PANEL_PRINT || wKey == _KEY_PANEL_CURVE) && (PanelCfgSize()==DISPLAY_SIZE_15)))//20210118 备用键按键自定义
        #endif
        {
            sendkey = KeyDefine_FindSendKey(wKey);
        }
        else
        {
            sendkey = KeyToMap(wKey);
        }
        ProtSendKey(sendkey, press<<15);
        //SetKeyRec(wKey, sendkey, press);//20220822 dyl 去掉按键记录

    }
	else
    {
        //20220805 dyl
        #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
        if(g_wPanelKey == _KEY_PANEL_SWX1 || g_wPanelKey == _KEY_PANEL_SWX2 || g_wPanelKey == _KEY_PANEL_SWX3
                || g_wPanelKey == _KEY_PANEL_SWX6 || g_wPanelKey == _KEY_PANEL_SWX7 || g_wPanelKey == _KEY_PANEL_SWX8)//20210118 备用键按键自定义
        #else//G10/G15:KeyBoard
        if(((g_wPanelKey == _KEY_PANEL_SW2 || g_wPanelKey == _KEY_PANEL_SW3) && (PanelCfgSize()<DISPLAY_SIZE_15))
            || ((g_wPanelKey == _KEY_PANEL_SW2 || g_wPanelKey == _KEY_PANEL_SW3 || wKey == _KEY_PANEL_PRINT || wKey == _KEY_PANEL_CURVE) && (PanelCfgSize()==DISPLAY_SIZE_15)))//20210118 备用键按键自定义
        #endif
        {
            sendkey = KeyDefine_FindSendKey(g_wPanelKey);
        }
        else
        {
            sendkey = KeyToMap(g_wPanelKey);
        }
        ProtSendKey(sendkey,0);
        //SetKeyRec(g_wPanelKey, sendkey, 0);//20220822 dyl 去掉按键记录
    }
    //oprintf("wKey:%x,sendkey:%x\n",wKey,sendkey);
	g_wPanelKey=wKey;
} 
/************************************************************************/
/* 面板手动按键处理                                                     */
/************************************************************************/
void MCUKeyRx(UI16 keycode)
{
	UI8 i;
    UI16 sendkey;

	//manual beep
	KeyCmdSend2(KEY_MANUL_TX_CMD,0,0);

    SetCurrentCheck(TRUE);//20201104

////20220426
//#ifdef CAPACITIVESCREEN
    if(/*IsG15Type() || */IsTouchType())//20220104 dyl
    {
        g_tmNoKeyPress=0;
        g_tmPanelNoKeyPress =0;//20200221
        KeySoundBeep();//串口3按键声音
        /*
        if(VarAdrToInt(PT15_SCREEN_PROTECT))
        {
            SetLCD(TRUE);
            Send_LCDOnOff(TRUE);//屏幕背光亮
            VarAdrSetInt(PT15_SCREEN_PROTECT,0);
        }
        if(VarAdrToInt(PT15_WAKE_SCREEN_SHOW) == 1)//屏幕背光测试
        {
            SetLCD(TRUE);
            Send_LCDOnOff(TRUE);//20190822.cyx
            PanelCloseDialogByName(DIAOGE_WAKE_SCREEN);
            VarAdrSetInt(PT15_WAKE_SCREEN_SHOW,0);
        }*/
        //20230518 chj 优化关屏调用频率 ：去掉判断条件，强制亮屏
        SetLCD(TRUE);
        Send_LCDOnOff(TRUE);//屏幕背光亮
        VarAdrSetInt(PT15_SCREEN_PROTECT,0);
        PanelCloseDialogByName(DIAOGE_WAKE_SCREEN);
        VarAdrSetInt(PT15_WAKE_SCREEN_SHOW,0);
    }
    else  //20200331
//#endif
    {
        SetLCD(TRUE);
        Send_LCDOnOff(TRUE);//屏幕背光亮
        g_tmPanelNoKeyPress=0;
    }

    bLCD_state = TRUE; //20230518 chj 优化关屏调用频率

	//force out to zero
    for(i = 0;i < MAX_IO_OUT_CB;i++)
    {
        if(ioout_force_cb[i] !=NULL)
        {
            (*ioout_force_cb[i])();
        }
    }

    for(i = 0;i < MAX_IO_OUT_CB;i++)
    {
        if(ioout2_force_cb[i] !=NULL)
        {
            (*ioout2_force_cb[i])();
        }
    }

	//servo alarm clean
	for (i=1;i<8;i++) 
	{
		if (dbsvox[i].wData[ID_8000])
		{
			Send_TaskCan(CAN_TX_WRITE,i,0x2000,7);
		}
		else if((VarAdrToUI16(SERVO_STATE_CANST1+i-1)&0x07)==7 || (VarAdrToUI16(SERVO_STATE_ERR1+i-1)))
		{
			Send_TaskCan(CAN_TX_WRITE,i,0x2000,7);
		}
	}

    //20200109.cyx DA画面禁止切自动
    if(PanelCurPage() == PanelFindPage(PAGE_DA_ADJUST))
    {
        ProtSysCtrl(0x0009/*ENTER_DA_CMD*/,0,NULL);//进入DA曲线调试
    }

    //20200220 按手动关闭调模调试模式
    VarSendSaveSetIntByAdr(ADJUST_MACH_FL_JOG_FUN,0);
    //按键记录保存手动-同步qt
    sendkey = KeyToMap(_KEY_PANEL_MANUAL);
    //SetKeyRec(_KEY_PANEL_MANUAL, sendkey, 1);//20220822 dyl 去掉按键记录
    //油温预热功能，在切手动时，油温预热功能选项关闭 20200423 hz
    VarSendSaveSetIntByAdr(FL_OILHEAT, 0);
}

/************************************************************************/
/* 面板按键获取的回调函数                                               */
/************************************************************************/
#include "plansheet.h"  //NET_FUNC
int KeyGetCallBack(int cur_key, int last_key)
{
    static UI32 time_3s = 0;
#ifndef WIN32
    UserLoginShow( cur_key);//NET_FUNC
#endif
	/*data lock */
	if (cur_key & 0x8000)
	{
		cur_key&=0x7FFF;
        g_cKeyState=_KEY_PANEL_KEYLOCK;
        VarAdrSetInt(HARDWARE_LOCK,1);//20191224.cyx 硬件锁，第一位置1
	}
	else 
    {
////20220426
//#ifdef CAPACITIVESCREEN
        if(/*IsG15Type()*/IsTouchType())
        {
            if((cur_key>=KEY_FUN1 && cur_key<=KEY_FUN10) || (cur_key == _KEY_GRAB_PNGIMAGE) || (cur_key == _KEY_VIEW_USER)
                    || (cur_key == KEY_FUN7) || (cur_key == KEY_HELP) || (cur_key == KEY_NEXTPAGE) || (cur_key == _KEY_SEARCH_VIEW))
            {
                if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400))//20241101 金鹰10寸快速机特殊面膜按键修改
                {
                    cur_key = JYKeyChange(cur_key);//20241101 金鹰10寸快速机：帮助按键特殊处理
                }
                return cur_key;//触摸屏排除由程序发送的按键 在Mainframe发送
            }
        }
//#endif
        g_cKeyState&=~_KEY_PANEL_KEYLOCK;
        VarAdrSetInt(HARDWARE_LOCK,VarAdrToInt(HARDWARE_LOCK)&0xE);//20191224.cyx
    }

	//长按上下键处理   //左右2015.8.31
	if ((cur_key!=_KEY_NULL)){
        if((GetTick() - time_3s) >= 3000)
        {
            if((cur_key==_UARROW)||(cur_key==_DARROW)) //_UARROW   GUI_KEY_UP  GUI_KEY_DOWN
            {
                g_bButtonPress=TRUE;
            }
            if(cur_key == _ENTER)
            {
                g_bEnterPress = TRUE;
            }
        }
	}
	
	if(cur_key==_KEY_NULL) {
		time_3s = GetTick();
		g_bButtonPress=FALSE;
        g_bEnterPress = FALSE;
	}

	if((cur_key != KEY_INVALID) && (cur_key != KEY_RELEASE))
	{
        if(VarAdrToUI16(PROGUPDATE_FLAG)) return 0;//20221024 dyl


		if(PanelCfgSort()==3)//如果是立式机 //20190505 LET
		{	
			if((cur_key >= _KEY_PANEL_ && cur_key<= _KEY_PANEL_END))

			{
				if ((cur_key ==_KEY_PANEL_COREAOUT || cur_key == _KEY_PANEL_COREAIN) && (VarAdrToInt(KEYTEST_ADDR) == 0))
				{	
					if(VarAdrToInt(ADDR_CORE_KEY_SELECT)==0 && cur_key==_KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_COREAIN;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==0 && cur_key==_KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_COREAOUT;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==1 && cur_key==_KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_COREBIN;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==1 && cur_key==_KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_COREBOUT;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==2 && cur_key==_KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_CORECIN;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==2 && cur_key==_KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_CORECOUT;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==3 && cur_key==_KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_COREDIN;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==3 && cur_key==_KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_COREDOUT;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==4 && cur_key==_KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_COREEIN;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==4 && cur_key==_KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_COREEOUT;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==5 && cur_key==_KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_COREFIN;
					else if (VarAdrToInt(ADDR_CORE_KEY_SELECT)==5 && cur_key==_KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_COREFOUT;
				}
				else if ((cur_key==_KEY_PANEL_AIRBLASTMOV || cur_key==_KEY_PANEL_AIRBLASTSTAT) && (VarAdrToInt(KEYTEST_ADDR) == 0))
				{
					
					if (VarAdrToInt(ADDR_AIRBLOW_KEY_SELECT)==0 && cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV;
					else if (VarAdrToInt(ADDR_AIRBLOW_KEY_SELECT)==0 && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT;
					else if (VarAdrToInt(ADDR_AIRBLOW_KEY_SELECT)==1&& cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV2;
					else if (VarAdrToInt(ADDR_AIRBLOW_KEY_SELECT)==1 && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT2;
					else if (VarAdrToInt(ADDR_AIRBLOW_KEY_SELECT)==2 && cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV3;
					else if (VarAdrToInt(ADDR_AIRBLOW_KEY_SELECT)==2 && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT3;
				}
			}	
		}
        else if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400))//20241101 金鹰10寸快速机特殊面膜按键修改
        {
            cur_key = JYKeyChange(cur_key);//20241101 金鹰10寸快速机特殊面膜按键修改
        }
        else
		{
            //20220805 dyl
            #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
            if((cur_key >= _KEY_PANEL_ && cur_key<= _KEY_PANEL_END) || cur_key== _KEY_PANEL_CORESELECT || cur_key==_KEY_PANEL_AIRSELECT || cur_key==_KEY_PANEL_SWX8)
            #else//G10/G15:KeyBoard
            if((cur_key >= _KEY_PANEL_ && cur_key<= _KEY_PANEL_END) || cur_key== _KEY_PANEL_CORESELECT || cur_key==_KEY_PANEL_AIRSELECT || cur_key==_KEY_PANEL_PRINT || cur_key==_KEY_PANEL_CURVE) //20220426
            #endif
			{
	//#ifdef PANEL_10
                if(GetPanelType() == PANEL_H10 || GetPanelType() == PANEL_V10 || GetPanelType() == PANEL_VT15)
				{
					UI32 addr1=0,addr2=0;
                    //中子切换 //20210118
                    if((cur_key==_KEY_PANEL_CORESELECT || KeyDefine_Def(cur_key, KEY_CORESELECT)) && (OperateModeIndex()==MODE_MANUAL||OperateModeIndex()==4) && (VarAdrToInt(KEYTEST_ADDR) == 0))
					{
						UI16 index;
						int cnt=0;

						if(g_corekeylock)
						{
							g_corekeylock=FALSE;
							if(g_keycore_sel!=0)
								index=g_keycore_sel-1;//g_wCoreKeyActive;
						}
						else
							index=g_keycore_sel;//g_wCoreKeyActive;
						
                        addr1 = COREA_FL_CORE_CTRL;
						do{
							cnt++;
							index++;
							if(index>COREGROUP_CNT)
								index=1;
                            addr1 = (UI32)COREA_FL_CORE_CTRL+(UI32)((index-1)<<20);
						}while(cnt<COREGROUP_CNT && VarAdrToUI16(addr1)==0);//中子功能选择使用
						if(VarAdrToUI16(addr1) != 0)
						{
							g_corekey_active=index;
							g_keycore_sel=index;
						}
						else
						{
							g_corekey_active=0;
							g_keycore_sel=0;
                            ShowMsg(VM_MSG_COREFUNCUSE);//20210301
						}
					}
                    if(g_corekey_active && (cur_key==_KEY_PANEL_CORESELECT || cur_key==_KEY_PANEL_COREAIN || cur_key==_KEY_PANEL_COREAOUT
                            || KeyDefine_Def(cur_key, KEY_CORESELECT) || KeyDefine_Def(cur_key, KEY_COREIN) || KeyDefine_Def(cur_key, KEY_COREOUT)))
						g_coretime=0;

					//中子切换
                    if((cur_key==_KEY_PANEL_COREAOUT || cur_key==_KEY_PANEL_COREAIN || KeyDefine_Def(cur_key, KEY_COREIN) || KeyDefine_Def(cur_key, KEY_COREOUT))&& (VarAdrToInt(KEYTEST_ADDR) == 0))
					{
                        //20241225 chj 中子如果单组开了，无需按中子切换,已经金鹰快速机模式下，中子按键为下拉框对应的选择项
                        //CoreSpcPorcess();

                        if (g_corekey_active==0)
                        {
                            cur_key =_KEY_NULL;
                            ShowMsg(VM_MSG_PRESSCORESEL);//20210301
                        }
                        else if ((g_corekey_active==1) && (cur_key==_KEY_PANEL_COREAIN || KeyDefine_Def(cur_key, KEY_COREIN))) cur_key =_KEY_PANEL_COREAIN;
                        else if ((g_corekey_active==1) && (cur_key==_KEY_PANEL_COREAOUT || KeyDefine_Def(cur_key, KEY_COREOUT))) cur_key =_KEY_PANEL_COREAOUT;
                        else if ((g_corekey_active==2) && (cur_key==_KEY_PANEL_COREAIN || KeyDefine_Def(cur_key, KEY_COREIN))) cur_key =_KEY_PANEL_COREBIN;
                        else if ((g_corekey_active==2) && (cur_key==_KEY_PANEL_COREAOUT || KeyDefine_Def(cur_key, KEY_COREOUT))) cur_key =_KEY_PANEL_COREBOUT;
                        else if ((g_corekey_active==3) && (cur_key==_KEY_PANEL_COREAIN || KeyDefine_Def(cur_key, KEY_COREIN))) cur_key =_KEY_PANEL_CORECIN;
                        else if ((g_corekey_active==3) && (cur_key==_KEY_PANEL_COREAOUT || KeyDefine_Def(cur_key, KEY_COREOUT))) cur_key =_KEY_PANEL_CORECOUT;
                        else if ((g_corekey_active==4) && (cur_key==_KEY_PANEL_COREAIN || KeyDefine_Def(cur_key, KEY_COREIN))) cur_key =_KEY_PANEL_COREDIN;
                        else if ((g_corekey_active==4) && (cur_key==_KEY_PANEL_COREAOUT || KeyDefine_Def(cur_key, KEY_COREOUT))) cur_key =_KEY_PANEL_COREDOUT;
                        else if ((g_corekey_active==5) && (cur_key==_KEY_PANEL_COREAIN || KeyDefine_Def(cur_key, KEY_COREIN))) cur_key =_KEY_PANEL_COREEIN;
                        else if ((g_corekey_active==5) && (cur_key==_KEY_PANEL_COREAOUT || KeyDefine_Def(cur_key, KEY_COREOUT))) cur_key =_KEY_PANEL_COREEOUT;
                        else if ((g_corekey_active==6) && (cur_key==_KEY_PANEL_COREAIN || KeyDefine_Def(cur_key, KEY_COREIN))) cur_key =_KEY_PANEL_COREFIN;
                        else if ((g_corekey_active==6) && (cur_key==_KEY_PANEL_COREAOUT || KeyDefine_Def(cur_key, KEY_COREOUT))) cur_key =_KEY_PANEL_COREFOUT;
					}
					//吹气切换
                    if((cur_key==_KEY_PANEL_AIRSELECT || KeyDefine_Def(cur_key, KEY_AIRSELECT)) && (OperateModeIndex()==MODE_MANUAL||OperateModeIndex()==4) && (VarAdrToInt(KEYTEST_ADDR) == 0))
					{
						UI16 index;
						int cnt=0;

						if(g_airkeylock)
						{
							g_airkeylock=FALSE;
							if(g_keyair_sel!=0)
								index=g_keyair_sel-1;//g_wAirKeyActive;
						}
						else
							index=g_keyair_sel;//g_wAirKeyActive;

                    addr1 = BLOW1_TM_MOVINGPLATENACT;
                    addr2 = BLOW2_TM_MOVINGPLATENACT;
                    UI32 addr_AirMode1=d_airblow1_MOLDSET_FL_BLAST;//20211214 dyl 吹气方式！=不使用
                    UI32 addr_AirMode2=d_airblow2_MOLDSET_FL_BLAST;
                    UI16 maxCnt = BLOWGROUP_CNT/2;//20241210 chj 28组吹气,当吹气大于7组的时候，最大组数要切换下
                    if(VarAdrToUI16(d_machine1_MOLDSET_FL_AIR_EXTEND28))
                    {
                        maxCnt = BLOWGROUP_CNT;
                    }
						do{
							cnt++;
							index++;
                        if(index>maxCnt)//20241210 chj 28组吹气,当吹气大于7组的时候，最大组数要切换下
								index=1;
                            if(index > (BLOWGROUP_CNT/2)) //20241210 chj 28组吹气,当吹气大于7组的时候，dic地址需要切换
                            {
                                addr1 = (UI32)d_airblow1_MOLDSET_TM_MOVINGPLATENACT2+(UI32)((index-1-(BLOWGROUP_CNT/2))<<21);
                                addr2 = (UI32)d_airblow2_MOLDSET_TM_MOVINGPLATENACT2+(UI32)((index-1-(BLOWGROUP_CNT/2))<<21);
                                addr_AirMode1 = (UI32)d_airblow1_MOLDSET_FL_BLAST2+(UI32)((index-1-(BLOWGROUP_CNT/2))<<21);//20211214 dyl 吹气方式！=不使用
                                addr_AirMode2 = (UI32)d_airblow2_MOLDSET_FL_BLAST2+(UI32)((index-1-(BLOWGROUP_CNT/2))<<21);
                            }
                            else
                            {
                                addr1 = (UI32)BLOW1_TM_MOVINGPLATENACT+(UI32)((index-1)<<21);
                                addr2 = (UI32)BLOW2_TM_MOVINGPLATENACT+(UI32)((index-1)<<21);
                                addr_AirMode1 = (UI32)d_airblow1_MOLDSET_FL_BLAST+(UI32)((index-1)<<21);//20211214 dyl 吹气方式！=不使用
                                addr_AirMode2 = (UI32)d_airblow2_MOLDSET_FL_BLAST+(UI32)((index-1)<<21);
                            }
                            //oprintf("index:%d,%x,%x,%x,%x\n",index,addr1,addr2,addr_AirMode1,addr_AirMode2);

                        }while(cnt<maxCnt && ((VarAdrToUI16(addr1)==0  || VarAdrToUI16(addr_AirMode1)==256) && (VarAdrToUI16(addr2)==0 || VarAdrToUI16(addr_AirMode2)==256)));//吹气动作时间 //20211214 dyl 吹气方式！=不使用 //20241210 chj 28组吹气,当吹气大于7组的时候，最大组数要切换下

                        if((VarAdrToUI16(addr1) != 0  && (VarAdrToUI16(addr_AirMode1)!=256)) || (VarAdrToUI16(addr2) != 0  && (VarAdrToUI16(addr_AirMode2)!=256)))//20211214 dyl 吹气方式！=不使用
						{
							g_airkey_active=index;
							g_keyair_sel=index;
						}
						else
						{
							g_airkey_active=0;
							g_keyair_sel=0;
                            ShowMsg(VM_MSG_SETAIRTIME);//20210301
						}
					}
                    if(g_airkey_active && (cur_key==_KEY_PANEL_AIRSELECT || cur_key==_KEY_PANEL_AIRBLASTMOV || cur_key==_KEY_PANEL_AIRBLASTSTAT
                            || KeyDefine_Def(cur_key, KEY_AIRSELECT) || KeyDefine_Def(cur_key, KEY_AIRBLOW135) || KeyDefine_Def(cur_key, KEY_AIRBLOW246)))
						g_airtime=0;

					//吹气切换
                    if((cur_key==_KEY_PANEL_AIRBLASTMOV || cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW135) || KeyDefine_Def(cur_key, KEY_AIRBLOW246))&& (VarAdrToInt(KEYTEST_ADDR) == 0))
					{
                        if (g_airkey_active==0)
                        {
                            cur_key=_KEY_NULL;
                            ShowMsg(VM_MSG_PRESSAIRSEL);//20210301
                        }
                        else if ((g_airkey_active==1) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV;
                        else if ((g_airkey_active==1) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT;
                        else if ((g_airkey_active==2) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV2;
                        else if ((g_airkey_active==2) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT2;
                        else if ((g_airkey_active==3) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV3;
                        else if ((g_airkey_active==3) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT3;
                        else if ((g_airkey_active==4) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV4;
                        else if ((g_airkey_active==4) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT4;
                        else if ((g_airkey_active==5) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV5;
                        else if ((g_airkey_active==5) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT5;
                        else if ((g_airkey_active==6) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV6;
                        else if ((g_airkey_active==6) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT6;
                        else if ((g_airkey_active==7) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV7;
                        else if ((g_airkey_active==7) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT7;
                        else if ((g_airkey_active==8) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV8;//20241210 chj 28组吹气,增加相应的按键表
                        else if ((g_airkey_active==8) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT8;
                        else if ((g_airkey_active==9) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV9;
                        else if ((g_airkey_active==9) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT9;
                        else if ((g_airkey_active==10) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV10;
                        else if ((g_airkey_active==10) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT10;
                        else if ((g_airkey_active==11) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV11;
                        else if ((g_airkey_active==11) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT11;
                        else if ((g_airkey_active==12) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV12;
                        else if ((g_airkey_active==12) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT12;
                        else if ((g_airkey_active==13) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV13;
                        else if ((g_airkey_active==13) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT13;
                        else if ((g_airkey_active==14) && (cur_key==_KEY_PANEL_AIRBLASTMOV || KeyDefine_Def(cur_key, KEY_AIRBLOW135))) cur_key=_KEY_PANEL_AIRBLASTMOV14;
                        else if ((g_airkey_active==14) && (cur_key==_KEY_PANEL_AIRBLASTSTAT || KeyDefine_Def(cur_key, KEY_AIRBLOW246))) cur_key=_KEY_PANEL_AIRBLASTSTAT14;

					}
				}
	//#else
				else if((GetPanelType() == PANEL_H8) || (GetPanelType() == PANEL_H7))
				{
			//blow
					if ((ChkKeySel(KSEL_BLAST2)) && cur_key==_KEY_PANEL_AIRBLASTMOV ) cur_key=_KEY_PANEL_AIRBLASTMOV2;
					else if ((ChkKeySel(KSEL_BLAST2)) && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT2;
					else if ((ChkKeySel(KSEL_BLAST3)) && cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV3;
					else if ((ChkKeySel(KSEL_BLAST3)) && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT3;
					else if ((ChkKeySel(KSEL_BLAST4)) && cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV4;
					else if ((ChkKeySel(KSEL_BLAST4)) && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT4;
					else if ((ChkKeySel(KSEL_BLAST5)) && cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV5;
					else if ((ChkKeySel(KSEL_BLAST5)) && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT5;
					else if ((ChkKeySel(KSEL_BLAST6)) && cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV6;
					else if ((ChkKeySel(KSEL_BLAST6)) && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT6;
					else if ((ChkKeySel(KSEL_BLAST7)) && cur_key==_KEY_PANEL_AIRBLASTMOV) cur_key=_KEY_PANEL_AIRBLASTMOV7;
					else if ((ChkKeySel(KSEL_BLAST7)) && cur_key==_KEY_PANEL_AIRBLASTSTAT) cur_key=_KEY_PANEL_AIRBLASTSTAT7;
					//core
					else if ((ChkKeySel(KSEL_COREA)) && cur_key==_KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_COREDIN;
					else if ((ChkKeySel(KSEL_COREA)) && cur_key==_KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_COREDOUT;
					else if ((ChkKeySel(KSEL_COREB)) && cur_key==_KEY_PANEL_COREBIN) cur_key=_KEY_PANEL_COREEIN;
					else if ((ChkKeySel(KSEL_COREB)) && cur_key==_KEY_PANEL_COREBOUT) cur_key=_KEY_PANEL_COREEOUT;
					else if ((ChkKeySel(KSEL_COREC)) && cur_key==_KEY_PANEL_CORECIN) cur_key=_KEY_PANEL_COREFIN;
					else if ((ChkKeySel(KSEL_COREC)) && cur_key==_KEY_PANEL_CORECOUT) cur_key=_KEY_PANEL_COREFOUT;
				}
	//#endif
			}
	
		}
	}


	return cur_key;
}
/************************************************************************/
/* 返回资料锁状态                                                       */
/************************************************************************/
static UI16 IsDataLock()
{
    if(PanelCfgStyle() == G_TYPE)//20191121.cyx 用户权限管理:生产人员lock
    {
        if(PanelCurPage() == PanelFindPage(PAGE_NEXTPAGE))
        {
            return FALSE;
        }
        else if(VarAdrToInt(USER_KEYLOCK))
        {
            return TRUE;
        }
        else
        {
            return g_cKeyState & _KEY_PANEL_KEYLOCK;
        }
    }
    else
    {
        return g_cKeyState & _KEY_PANEL_KEYLOCK;
    }
}

//#include "crsa.h"
//2018.3.14 csj 模数n
char pub_n[] =
{-93, 42, -55, -76, -51, 90, -25, 72, -14, 63,
103, 8, 108, 41, -120, 28, -28, 63, 106, -62,
-90, 113, -22, 57, 6, 37, -87, 18, 7, -110,
-90, -11, -36, -81, 44, 124, 38, -41, 72, -32,
11, 0, 41, 93, -25, -95, 45, -61, -104, 119,
-53, -102, -84, -1, 63, 3, 38, 105, 17, 5,
121, 127, 108, 57
};
int pub_nx[]=
{42, 90, 72, 63,
103, 8, 108, 41, 28, 63, 106,
113, 57, 6, 37,18, 7,
44, 124, 38, 72,
11, 41, 93,45, 119,
63, 3, 38, 105, 17, 5,
121, 127, 108, 57
};
//int pub_n[] =
//{93, 42, 55, 76, 51, 90, 25, 72, 14, 63,
//103, 8, 108, 41, 120, 28, 28, 63, 106, 62,
//90, 113, 22, 57, 6, 37,87, 18, 7, 110,
//90, 11, 36, 81, 44, 124, 38, 41, 72, 32,
//11, 0, 41, 93, 25, 95, 45, 61, 104, 119,
//53, 102, 84, 1, 63, 3, 38, 105, 17, 5,
//121, 127, 108, 57
//};
// 公钥指数
int pub_exp[] =
{0x01, 0x00, 0x01};

char pri_d[]={41, -115, -110, 4, -69, 111, 117, 11, 60,
-74, 44, -43, 22, 70, -107, 101, 86, 51, 88, 74, -19,
-4, -128, -69, -72, -9, 10, 81, -113, 32, 39, -24, 119,
-21, 126, -71, 29, -107, 67, 79, 97, -113, -55, -53, -105,
-111, 19, -103, 126, -44, 101, -34, 6, -102, -107, -72, 101,
-104, 16, -27, -87, -79, 25, -87};//64
char pri_dx[]={41, 4,  111, 117, 11, 60,
 44, 22, 70, 101, 86, 51, 88, 74,
 10, 81, 32, 39,119,
 126, 29, 67, 79, 97,
 19,  126, 101, 6,  101,
 16, 25};//64
//int pri_d[]={41, 115, 110, 4, 69, 111, 117, 11, 60,
//74, 44, 43, 22, 70, 107, 101, 86, 51, 88, 74, 19,
//4, 128, 69, 72, 9, 10, 81, 113, 32, 39, 24, 119,
//21, 126, 71, 29, 107, 67, 79, 97, 113, 55, 53, 105,
//111, 19, 103, 126, 44, 101, 34, 6, 102, 107, 72, 101,
//104, 16, 27, -87, 79, 25, 87};//64



//#define  WECHAT_HEAD   "http://yun.esto.cn/netRegist/index.html"
#define  WECHAT_HEAD   "12345"
extern int n[];
extern int d[];
extern int e[];
int expx[100]={0};
int expn[100]={0};
int pri[100]={0};


//20241101 金鹰10寸快速机特殊面膜按键修改
UI32 JYKeyChange(UI32 cur_key)
{
    if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400) && (VarAdrToInt(KEYTEST_ADDR) == 0))
    {
        if((cur_key >= _KEY_PANEL_ && cur_key<= _KEY_PANEL_END) || (cur_key == KEY_HELP) || (cur_key== _KEY_PANEL_CORESELECT) || (cur_key==_KEY_PANEL_AIRSELECT))
        {
            //oprintf("jy key change--in---%x\n",cur_key);
            if(cur_key == _KEY_PANEL_SEMIAUTO) cur_key=_KEY_PANEL_MOLDADJ;                  //半自动->调模使用
            else if(cur_key == _KEY_PANEL_TIMEAUTO) cur_key=_KEY_PANEL_SEMIAUTO;            //全自动->半自动
            else if(cur_key == _KEY_PANEL_RUN) cur_key=_KEY_PANEL_TIMEAUTO;                 //运行->全自动

            else if(cur_key == _KEY_PANEL_COREAOUT) cur_key=_KEY_PANEL_MOLDTHICK;           //中子出->调模退
            else if(cur_key == _KEY_PANEL_COREAIN) cur_key=_KEY_PANEL_MOLDTHIN;             //中子入->调模进

            else if(cur_key == _KEY_PANEL_AIRBLASTMOV )
            {
                if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 0) cur_key=_KEY_PANEL_COREAOUT;         //吹气135->中子出
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 1) cur_key=_KEY_PANEL_COREBOUT;    //吹气135->中子出
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 2) cur_key=_KEY_PANEL_CORECOUT;    //吹气135->中子出
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 3) cur_key=_KEY_PANEL_COREDOUT;    //吹气135->中子出
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 4) cur_key=_KEY_PANEL_COREEOUT;    //吹气135->中子出
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 5) cur_key=_KEY_PANEL_COREFOUT;    //吹气135->中子出

            }
            else if(cur_key == _KEY_PANEL_AIRBLASTSTAT)
            {
                if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 0) cur_key=_KEY_PANEL_COREAIN;         //吹气246->中子入
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 1) cur_key=_KEY_PANEL_COREBIN;    //吹气246->中子入
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 2) cur_key=_KEY_PANEL_CORECIN;    //吹气246->中子入
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 3) cur_key=_KEY_PANEL_COREDIN;    //吹气246->中子入
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 4) cur_key=_KEY_PANEL_COREEIN;    //吹气246->中子入
                else if(VarAdrToInt(p_PP_MACHSET_CORE_KEY_SELECT) == 5) cur_key=_KEY_PANEL_COREFIN;    //吹气246->中子入
            }

            else if(cur_key == _KEY_PANEL_INJECT) cur_key=_KEY_PANEL_AIRBLASTMOV;           //射出->吹气1
            else if(cur_key == _KEY_PANEL_SUCKBACK) cur_key=_KEY_PANEL_AIRBLASTSTAT;        //射退->吹气2

            else if(cur_key == _KEY_PANEL_CHARGE) cur_key=_KEY_PANEL_AIRBLASTMOV2;          //加料->吹气3
            else if(cur_key == _KEY_PANEL_AUTOPURGE) cur_key=_KEY_PANEL_AIRBLASTSTAT2;      //清料->吹气4

            else if(cur_key == _KEY_PANEL_CORESELECT) cur_key=_KEY_PANEL_AIRBLASTMOV3;      //中子切换->吹气5
            else if(cur_key == _KEY_PANEL_LUBRICATION) cur_key=_KEY_PANEL_AIRBLASTSTAT3;    //润滑->吹气6

            else if(cur_key == _KEY_PANEL_AIRSELECT) cur_key=_KEY_PANEL_AIRBLASTMOV4;       //吹气切换->吹气5
            else if(cur_key == _KEY_PANEL_MOLDADJ) cur_key=_KEY_PANEL_AIRBLASTSTAT4;        //调模使用->吹气6

            else if(cur_key == _KEY_PANEL_NOZZLEADV) cur_key=_KEY_PANEL_INJECT;             //座进->射出
            else if(cur_key == _KEY_PANEL_NOZZLERET) cur_key=_KEY_PANEL_CHARGE;             //座退->加料

            else if(cur_key == _KEY_PANEL_MOLDTHICK) cur_key=_KEY_PANEL_NOZZLEADV;          //调模退->座进
            else if(cur_key == _KEY_PANEL_MOLDTHIN) cur_key=_KEY_PANEL_NOZZLERET;           //调模进->座退

            else if(cur_key == _KEY_PANEL_SW2) cur_key=_KEY_PANEL_SUCKBACK;                 //调模退->座进
            else if(cur_key == _KEY_PANEL_SW3) cur_key=_KEY_PANEL_AUTOPURGE;                //调模进->座退

            else if(cur_key == KEY_HELP) cur_key=_KEY_PANEL_LUBRICATION;                    //帮助->润滑

            else {
                ;
            }
        }
    }
    //oprintf("jy key change--return---%x\n",cur_key);
    return cur_key;

}
//20241101 金鹰10寸快速机特殊面膜按键修改
UI32 JYLubKeyChange(UI32 cur_key)
{
    if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)&& (VarAdrToInt(KEYTEST_ADDR) == 0))
    {
        if(cur_key == _KEY_PANEL_LUBRICATION)
        {
            if(VarAdrToInt(p_PP_MACHSET_LUBR_BTN_FUNC) == 0)
                cur_key = _KEY_PANEL_LUBRICATION;
            else if(VarAdrToInt(p_PP_MACHSET_LUBR_BTN_FUNC) == 1)
                cur_key = _KEY_LUBRICATION1;
            else if(VarAdrToInt(p_PP_MACHSET_LUBR_BTN_FUNC) == 2)
                cur_key = _KEY_LUBRICATION2;
            else
                cur_key = _KEY_PANEL_LUBRICATION;
        }
    }
    return cur_key;
}
//20241225 chj 中子如果单组开了，无需按中子切换,已经金鹰快速机模式下，中子按键为下拉框对应的选择项
void CoreSpcPorcess()
{
    int i = 0;
    UI16 usecorecnt = 0;
    UI16 bOneWhere = 0;
    UI32 addr1 = COREA_FL_CORE_CTRL;

    if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)&& (VarAdrToInt(KEYTEST_ADDR) == 0))
    {
        ;
    }
    else
    {
        for(i = 0; i < COREGROUP_CNT;i++)
        {
            addr1 = (UI32)COREA_FL_CORE_CTRL+(UI32)(i<<20);
            if(VarAdrToInt(addr1))
            {
                usecorecnt++;
                bOneWhere = i+1;
            }
        }
        if(usecorecnt == 1)
        {
            g_corekey_active = bOneWhere;
            g_keycore_sel = bOneWhere;
        }
    }
}
/************************************************************************/
/* 画面按键处理的回调函数                                               */
/************************************************************************/
void PanelKeyProcCallBack(int cur_key, int press)
{
    //static BOOL bLCD_state=TRUE;//20230518 chj 优化关屏调用频率
	static UI32 time_1s = 0;
	static BOOL bmainhelp = FALSE; //主画面帮助
	BOOL istestkey = FALSE; //20190426
    UI16 machcfgsave = 0;//VarAdrToUI16(MACHCFG_CHANGE_FLAG); //20250103 chj 卓越II代版本，未使用次变量，注释掉，某些数据导入的时候，这个变量为1，导致按F1无法切换
    UI16 machcfg5200save = VarAdrToUI16(MACHCFG5200_CHANGE_FLAG);
	UI16 daadjsave = VarAdrToUI16(DAADJ_CHANGE_FLAG);
    UI16 machcfgxsave = VarAdrToUI16(MACHCFGX_CHANGE_FLAG);
    UI16 adexsave = VarAdrToUI16(ADEX_CHANGE_FLAG);//20191126.cyx
    UI16 adxsave = VarAdrToUI16(ADX_CHANGE_FLAG);
    UI16 ecatsave = VarAdrToUI16(p_PP_MACHSET_ECAT_CHANGE_FLAG);
    UI16 specfuncselectsave = VarAdrToUI16(p_PP_MACHSET_SPECFUNC_SELECT_CHANGE_FLAG);
    UI32 heater_state;



	if(cur_key!=0)
	{
        //oprintf("PanelKey: %x  press:%x\r\n", cur_key, press);
		//progr update protect 程序更新时其他按键无效
		if(VarAdrToUI16(PROGUPDATE_FLAG)) return;

        if(nFaceIn) return;//20230518 chj 开机进入FACE页面，显示数据加载中

        if(FrmInputIsShow() && IsTouchType())//20220104 dyl
        {
            if ((cur_key>=_N0 && cur_key<=_N9) ||(cur_key>=GUI_KEY_LEFT && cur_key<=GUI_KEY_DOWN)|| cur_key == _DEL||cur_key == _DOT||cur_key == _ENTER||cur_key == _SPC)
            {
                if(press != 0)//20241225 chj 点击物理按键，发送信号给数字按键盘，从而出发数字按键盘按下数字按钮信号
                {
                    PhyBtnSignalToFrmInput(cur_key);
                }
                return;
            }
        }

        //U盘自动更新获取运行键次数
        AutoUdiskRunKeyCnt(cur_key, press);

        //20200323 温度状态地址切换
        if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
        {
            heater_state = VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS);//0E160038 E502  E506
        }
        else
        {
            heater_state = VarAdrToInt(d_tempctrl1_STATE_FL_HEATERSTATUS); //0516002B
        }
        //温度优化过程中 电热键按下需确认 20190522  //20191212 e502温度优化
        if(press && (isTempOptimize()||isTempOptimizex()) && heater_state && (cur_key == _KEY_PANEL_HEATER || KeyDefine_Def(cur_key, KEY_HEATER)))
        {
            if(TempOptimizeJudege())
            {
               return;
            }
        }

        //20200221 safety door 屏幕点亮处理
        if (cur_key!=_KEY_PANEL_DOORCLOSE && cur_key!=_KEY_PANEL_DOOROPEN && press!=0)  //20190129
        {
            //g_tmNoKeyPress=0; 20200221在画面按键未处理前，不能置0
            g_tmPanelNoKeyPress=0;

            //20241230 chj 主画面，换页后进行强制亮屏
            if((cur_key ==GUI_KEY_MAINPAGE) || (cur_key == GUI_KEY_NEXTPAGE))
            {
                SetLCD(TRUE);
                Send_LCDOnOff(TRUE);
                VarAdrSetInt(PT15_SCREEN_PROTECT,0);
                bLCD_state=TRUE;
                PanelCloseDialogByName(DIAOGE_WAKE_SCREEN);
                VarAdrSetInt(PT15_WAKE_SCREEN_SHOW,0);
                //oprintf("light:F1-F7\n");
            }
            else
            {
                // //20220426
                // #ifdef CAPACITIVESCREEN
                if(/*IsG15Type() || */IsTouchType())//20190629.cyx + //20220104 dyl
                {
#ifdef A40I //20250319 chj 优化点屏幕功能，每次亮屏，需要获取驱动信号，来确定状态
                    if(GetScreenState() == FALSE)
#else
                    if (VarAdrToInt(PT15_SCREEN_PROTECT))
#endif
                    {
                        SetLCD(TRUE);//20190417
                        Send_LCDOnOff(TRUE);//20190822.cyx
                        VarAdrSetInt(PT15_SCREEN_PROTECT,0);
                        //oprintf("light:PT15_SCREEN_PROTECT\n");
                    }
                }
                else
                // #endif
                {
#ifdef A40I//20250319 chj 优化点屏幕功能，每次亮屏，需要获取驱动信号，来确定状态
                    if(GetScreenState() == FALSE)
#else
                    if (!bLCD_state)
#endif
                    {
                        SetLCD(TRUE);//20190417
                        Send_LCDOnOff(TRUE);//20190822.cyx
                        bLCD_state=TRUE;
                        //oprintf("light:bLCD_state\n");
                    }
                }
            }

        }

        if(press && PanelCurPage() == PanelFindPage(PAGE_DATA_DIAG)) //数据检测画面按键特殊处理
        {
            if(datadiag_cb != NULL)
            {
                if((*datadiag_cb)(cur_key, press))
                {
                    return;
                }
            }
        }

		//servo protect 自学习中，除输入键其他都无效
		if(VarAdrToUI16(SERVO_SELFLEARNSWITCH) && (cur_key != _ENTER && cur_key != _KEY_PANEL_MANUAL
			&& cur_key != KEY_INVALID  && cur_key != _KEY_NULL))
		{
			ShowMsg(VW_MSG_STOPSELFLEARN);
			return;
        }

        //自学习中，除输入键与方向键其他都无效 //20200709.cyx
        if(VarAdrToInt(p_PP_MACHSET_SELF_STUDY_FLAG) && (cur_key != _ENTER && cur_key != GUI_KEY_DOWN
            && cur_key != GUI_KEY_UP && cur_key != GUI_KEY_LEFT && cur_key != GUI_KEY_RIGHT))
        {
            return;
        }

        //20191113 hz tab画面按键特殊处理   除ecat外只支持G10
        if(PanelCfgStyle() == G_TYPE)
        {
            //20220426
            if(press && (GetPanelType()==PANEL_V10 || GetPanelType()== PANEL_VT15) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN8) || (cur_key ==GUI_KEY_MAINPAGE)
                          || (cur_key == _KEY_VIEW_USER) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP) || (cur_key == GUI_KEY_NEXTPAGE)))
            {
                if(tab_cb != NULL)
                {
                    (*tab_cb)(cur_key, press);
                }
            }
            if(ecatsave && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN8) || (cur_key ==GUI_KEY_MAINPAGE)
                          || (cur_key == _KEY_VIEW_USER) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP) || (cur_key == GUI_KEY_NEXTPAGE)))
            {
                if(ecat_cb != NULL)
                {
                    (*ecat_cb)(cur_key,press);
                    return;
                }
            }
            if(press && (GetPanelType()==PANEL_V10 || GetPanelType()== PANEL_VT15 ) && (PanelCurPage() ==  PanelMainPage()) && (cur_key == GUI_KEY_MAINPAGE || cur_key ==GUI_KEY_LEFT || cur_key == GUI_KEY_RIGHT || cur_key == GUI_KEY_DOWN || cur_key == GUI_KEY_UP)
                    /*&& (PanelDialogCnt() < 1)*/)//20200415 有弹出框时不捕获按键 20220426
            {
                //20211125 dyl 在主画面弹出帮主页面后，按下主画面需要关闭帮助
                if (PanelDialogCnt() > 0)
                {
                    if((PanelTopDialog() == PanelFindPage(PAGE_PAGEHELP)) &&  (cur_key == GUI_KEY_MAINPAGE))
                    {
                        PanelCloseDialogByName(PAGE_PAGEHELP);
                    }
                }
                else
                {
                    if(main_cb != NULL)
                    {
                        (*main_cb)(cur_key,press);
                        return;
                    }
                }
            }
        }
        else if(PanelCfgStyle() == P_TYPE)
        {
            if(press && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN8) || (cur_key ==GUI_KEY_MAINPAGE)
                          || (cur_key == _KEY_VIEW_USER) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP) || (cur_key == GUI_KEY_NEXTPAGE)))
            {
                if(tab_cb != NULL)
                {
                    (*tab_cb)(cur_key, press);
                }
            }
            if(ecatsave && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN8) || (cur_key ==GUI_KEY_MAINPAGE)
                          || (cur_key == _KEY_VIEW_USER) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP) || (cur_key == GUI_KEY_NEXTPAGE)))
            {
                if(ecat_cb != NULL)
                {
                    (*ecat_cb)(cur_key,press);
                    return;
                }
            }
        }

		//save pop-up 保存弹出框显示时，按以下按键无效
        if(PanelCfgStyle() == G_TYPE)//20191126.cyx
        {
            if((machcfgsave && cur_key==GUI_KEY_FUN1) || (machcfg5200save && cur_key==GUI_KEY_FUN1) || (daadjsave && cur_key == GUI_KEY_FUN1) || (adexsave && cur_key == GUI_KEY_FUN2) || (adxsave && cur_key == GUI_KEY_FUN1)
                     || (specfuncselectsave && cur_key == GUI_KEY_FUN3) || (PUMPFUNC_CHANGE_FLAG && cur_key==GUI_KEY_FUN2)) //20241223 fqh Pump表修改
                return;
            if((machcfgsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
                || (cur_key == GUI_KEY_NEXTPAGE) || (cur_key == _KEY_VIEW_USER)))
            {
                oprintf("machcfgsave\r\n");
                if(machcfg_change_cb!=NULL)
                {
                    (*machcfg_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((machcfg5200save) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
                                      || (cur_key == GUI_KEY_NEXTPAGE) || (cur_key == _KEY_VIEW_USER)))
            {
                oprintf("machcfg5200save\r\n");
                if(machcfg5200_change_cb!=NULL)
                {
                    (*machcfg5200_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((PUMPFUNC_CHANGE_FLAG) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
                                      || (cur_key == GUI_KEY_NEXTPAGE) || (cur_key == _KEY_VIEW_USER))) //20241223 fqh Pump表修改后未保存时就切换画面的操作
            {
                oprintf("pumpfunc_change_cb\r\n");
                if(pumpfunc_change_cb!=NULL)
                {
                    (*pumpfunc_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((daadjsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
                || (cur_key == GUI_KEY_NEXTPAGE) || (cur_key == _KEY_VIEW_USER)))
            {
                oprintf("daadjsave\r\n");
                if(daadj_change_cb!=NULL)
                {
                    (*daadj_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((adexsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
                || (cur_key == GUI_KEY_NEXTPAGE) || (cur_key == _KEY_VIEW_USER)))
            {
                oprintf("adexsave\r\n");
                if(adex_change_cb!=NULL)
                {
                    (*adex_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((adxsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
                || (cur_key == GUI_KEY_NEXTPAGE) || (cur_key == _KEY_VIEW_USER)))
            {
                oprintf("adxsave\r\n");
                if(adx_change_cb!=NULL)
                {
                    (*adx_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((specfuncselectsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
                || (cur_key == GUI_KEY_NEXTPAGE) || (cur_key == _KEY_VIEW_USER)))
            {
                oprintf("specfuncselect save\r\n");
                if(specfuncselect_change_cb!=NULL)
                {
                    (*specfuncselect_change_cb)(cur_key,press);
                    return;
                }
            }
        }
        else if(PanelCfgStyle() == P_TYPE)//20200220.cyx
        {
            if((machcfgsave && cur_key==GUI_KEY_FUN3) || (machcfg5200save && cur_key==GUI_KEY_FUN3) || (PUMPFUNC_CHANGE_FLAG && cur_key==GUI_KEY_FUN2) || (daadjsave && cur_key == GUI_KEY_FUN2) || (adxsave && cur_key == GUI_KEY_FUN1) || (adexsave && (cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN7)))
                return;

            if((machcfgsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)))
            {
                oprintf("machcfgsave\r\n");
                if(machcfg_change_cb!=NULL)
                {
                    (*machcfg_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((machcfg5200save) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)))
            {
                oprintf("machcfg5200save\r\n");
                if(machcfg5200_change_cb!=NULL)
                {
                    (*machcfg5200_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((PUMPFUNC_CHANGE_FLAG) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)))
            {
                //20241223 fqh Pump表修改后未保存时就切换画面的操作
                oprintf("PUMPFUNCsave\r\n");
                if(pumpfunc_change_cb!=NULL)
                {
                    (*pumpfunc_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((daadjsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)))
            {
                oprintf("daadjsave\r\n");
                if(daadj_change_cb!=NULL)
                {
                    (*daadj_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((adxsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)))
            {
                oprintf("adxsave\r\n");
                if(adx_change_cb!=NULL)
                {
                    (*adx_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((adexsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key == GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)))
            {
                oprintf("adexsave\r\n");
                if(adex_change_cb!=NULL)
                {
                    (*adex_change_cb)(cur_key,press);
                    return;
                }
            }
        }
        else
        {
            if((machcfgsave && cur_key==GUI_KEY_FUN3) || (daadjsave && cur_key == GUI_KEY_FUN2) || (machcfgxsave && cur_key == GUI_KEY_FUN5))
                return;
            if((machcfgsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key ==GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8) || (cur_key == KEY_HELP)
				|| (PanelCfgStyle() == TIANJIAN_TYPE && cur_key == GUI_KEY_NEXTPAGE))) //20190521
            {
                oprintf("machcfgsave\r\n");
                if(machcfg_change_cb!=NULL)
                {
                    (*machcfg_change_cb)(cur_key,press);
                    return;
                }
            }
            else if((daadjsave) && ((cur_key>=GUI_KEY_FUN1 && cur_key<=GUI_KEY_FUN9) || (cur_key ==GUI_KEY_MAINPAGE) || (cur_key == KEY_FUN8)
				|| (PanelCfgStyle() == TIANJIAN_TYPE && cur_key == GUI_KEY_NEXTPAGE))) //20190521
            {
                oprintf("daadjsave\r\n");
                if(daadj_change_cb!=NULL)
                {
                    (*daadj_change_cb)(cur_key,press);
                    return;
                }
            }
        }


		//software_lock or data lock
		if ((IsDataProtect() || IsViewProtect()) ||(IsDataLock()))
		{
			VarAdrSetInt(DATA_SOFTWARE_LOCK,1);
		}
		else 
		{
			VarAdrSetInt(DATA_SOFTWARE_LOCK,0);
		}

		/*panel 7/8 system key*/
        if((GetPanelType() == PANEL_H8) || (GetPanelType() == PANEL_H7))
		{
			if(cur_key == KEY_FUN8)
			{
				cur_key = GUI_KEY_FUN8;
			}
		}

////20220426
//#ifdef CAPACITIVESCREEN
        if((/*IsG15Type()*/!IsHaveKeyBoard()))//20220805 dyl
        {
            istestkey = VarAdrToInt(KEYTEST_ADDR) != 0 && (testkey_cb != NULL) && (cur_key != GUI_KEY_MAINPAGE) && (cur_key != GUI_KEY_NEXTPAGE);
        }
        else
//#endif
        {
            istestkey = VarAdrToInt(KEYTEST_ADDR) != 0 && (testkey_cb != NULL);
        }

        if(istestkey)
		{
			(*testkey_cb)(cur_key,press);
		}
		else if(VarAdrToUI16(DAADJUST_ADJMOD)!=0 &&(cur_key == GUI_KEY_DOWN || cur_key == GUI_KEY_UP || cur_key == GUI_KEY_LEFT || cur_key == GUI_KEY_RIGHT ) && daadjust_cb !=NULL
			&& (PanelFocusPage() == PanelCurPage()))
		{
			(*daadjust_cb)(cur_key,press);
		}
		else if((IsDataProtect() || IsDataLock()) && ((cur_key>=_N0 && cur_key <=_N9) || (cur_key == _ENTER) || (cur_key == _DOT) || (cur_key == _SPC)))
		{
			return;
		}
        else if (((PanelCfgStyle() == G_TYPE)) && (cur_key == GUI_KEY_NEXTPAGE)) //20190426 进入导航页面
		{
            if(!IsPayTimeOut() && press)//20191219
            { 
                if(servostatex1_cb != NULL)
                {
                    (*servostatex1_cb)(cur_key,press);
                }
                else
                {
                    PanelShowPageByName(PAGE_NEXTPAGE);
                }
            }
		}
        else if (((PanelCfgStyle() == G_TYPE)) && (cur_key == _KEY_VIEW_USER)) //20190912.cyx 进入用户管理页面
        {
            if(!IsPayTimeOut() && press)//20191219
            {
                PanelShowPageByName(PAGE_USER_MANAGE);
            }
        }
        //20220805 dyl
        #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard  //#ifdef CAPACITIVESCREEN  //电容屏P15-5排新按键
        else if((PanelCfgStyle() == G_TYPE) && ((cur_key == KEY_FUN8) || (cur_key == _KEY_PANEL_SWX4)))
        #else
        else if (((PanelCfgStyle() == G_TYPE)) && (cur_key == KEY_FUN8)) //20191121.cyx 进入语言页面
        #endif
        {
            if(!IsPayTimeOut() && press)//20191219
            {
                PanelShowPageByName(PAGE_LANGUAGE);
            }
        }
		else 
		{
			PageFramPanelKey(PanelFocusPage(), cur_key, press);

            //20200221 safety door  PageFramPanelKey函数处理后将 g_tmNoKeyPress置0
            if (cur_key!=_KEY_PANEL_DOORCLOSE && cur_key!=_KEY_PANEL_DOOROPEN && press!=0)
            {
                g_tmNoKeyPress=0;
            }

            //20220805 dyl
            #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard  //#ifdef CAPACITIVESCREEN  //电容屏P15-5排新按键
            if((cur_key == _KEY_PANEL_MOLDADJ) || (cur_key == _KEY_PANEL_ALARMCLEAR) || (cur_key == _KEY_PANEL_SWX8))
            {
                if(press == 0)
                {
                    cur_key =_KEY_NULL;
                }

                if(!(KeyDefine_Def(cur_key, KEY_CORESELECT) || KeyDefine_Def(cur_key, KEY_AIRSELECT)))//20210118
                {
                    SendPanelKey(cur_key,press);
                }
            }
            #endif

//#ifndef WIN32
            if ((cur_key>=_KEY_PANEL_ && cur_key<=_KEY_PANEL_END) || cur_key == _KEY_NULL || cur_key == _KEY_PANEL_PRINT ||cur_key == _KEY_PANEL_CURVE) //20220426
			{  
				/*make up the key*/
				if(press ==0)
				{
					cur_key =_KEY_NULL;
				}

				if(IsPayTimeOut() && (cur_key==_KEY_PANEL_SEMIAUTO || cur_key==_KEY_PANEL_SENSORAUTO
					|| cur_key==_KEY_PANEL_TIMEAUTO || cur_key==_KEY_PANEL_RUN)) return;

				//20190527 CHZ
				if(PanelCfgSort()==VERTICAL_MACH)
				{
					if( VarAdrToInt(AUTO_ADJ)==0 && cur_key==_KEY_PANEL_TIMEAUTO)
					{
						return;
					}
				}

                //20200407HJM 模温功能
                //20220805 dyl
                #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard //#ifdef CAPACITIVESCREEN  //电容屏P15-5排新按键
                if(isUseMoldFunc() && (cur_key == _KEY_PANEL_SWX2))
                #else//G10/G15:KeyBoard
                if((isUseMoldFunc() && (cur_key == _KEY_PANEL_SW3) && (PanelCfgSize()<DISPLAY_SIZE_15))
                   || (isUseMoldFunc() && (cur_key == _KEY_PANEL_CURVE) && (PanelCfgSize()==DISPLAY_SIZE_15)))
                #endif
                {
                    int i;
                    BOOL key_use = FALSE;

                    for(i = 0; i < VarAdrToInt(MT_CTRL_PART); i++)
                    {
                        if(VarAdrToInt(d_TempCtrlMW1_MOLDSET_FL_USE_MW + i*0x100000))
                        {
                            key_use = TRUE;
                            break;
                        }
                    }

                    if(key_use)//20210118
                    {
                        cur_key = _KEY_MDTPHEATER;
                    }

                    //温度优化过程中 电热键按下需确认 //20200806
                    if(press && isMTOptimizex() && GetMtpTempHeatState())
                    {
                        if(MoldTempOptimizeJudege())
                        {
                            return;
                        }
                    }
                }

                //20200303
                if((OperateModeIndex()==MODE_MANUAL) && (VarAdrToInt(p_PP_MACHSET_FLOPMODE_USEPSW)==1) &&
                        (cur_key==_KEY_PANEL_SEMIAUTO || cur_key==_KEY_PANEL_SENSORAUTO
                        || cur_key==_KEY_PANEL_TIMEAUTO) && (MainProtComIsOK())
                        && ((VarAdrToInt(SYS_MACH_CUSTOMID)&0xFF00) == 0x8B00))//20200415 通用厂商码才使用
                {

                    VarAdrSetInt(p_PP_MACHSET_G_KEYVALUE,cur_key);
                    PageFramPanelKey(PanelFocusPage(), GUI_KEY_FUN8, press);
                    return;
                }

                if((VarAdrToInt(p_PP_MACHSET_SPC_KEY_USE_JY) == 1) && (PanelCfgSize()==DISPLAY_SIZE_10) && ((VarAdrToInt(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x6400)&& (VarAdrToInt(KEYTEST_ADDR) == 0))
                {
                    cur_key = JYLubKeyChange(cur_key);//20241101 金鹰10寸快速机特殊面膜按键修改-金鹰润滑特殊修改
                }
                else
                {
                    //20200930 特殊按键切换
                    if(VarAdrToInt(d_autoctrl1_MOLDSET_KEY_LUBR2_INDEPEND) == 1)
                    {
                        if(cur_key == _KEY_PANEL_LUBRICATION)
                        {
                            cur_key = _KEY_LUBRICATION1;
                        }
                        //20220805 dyl
                        #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard
                        else if (cur_key == _KEY_PANEL_SWX1)
                        #else//G10/G15:KeyBoard
                        else if (((cur_key == _KEY_PANEL_SW2) && (PanelCfgSize()<DISPLAY_SIZE_15))
                            || ((cur_key == _KEY_PANEL_PRINT) && (PanelCfgSize()==DISPLAY_SIZE_15)))
                        #endif
                        {
                            cur_key = _KEY_LUBRICATION2;
                        }
                    }
                }


                if(!(KeyDefine_Def(cur_key, KEY_CORESELECT) || KeyDefine_Def(cur_key, KEY_AIRSELECT)))//20210118
                {
                    SendPanelKey(cur_key,press);
                }
			}

			//rsa test
#if 0		
			if(cur_key == _F10)
			{
				if(press)
				{
					int k,j,g,s,l;
					RSAKey();
					memset(buf,0,sizeof(buf));
					memset(bufx,0,sizeof(bufx));
					expx[0]=1;
					expx[1]=0;
					expx[2]=0;
					expx[3]=0;
					expx[4]=1;
					expx[99]=5;

					for(g=0;g<36;g++)
					{
	               /*	int pub_nx[]=
						{42, 90, 72, 63,
						103, 8, 108, 41, 28, 63, 106,
						113, 57, 6, 37,18, 7,
						44, 124, 38, 72,
						11, 41, 93,45, 119,
						63, 3, 38, 105, 17, 5,
						121, 127, 108, 57
						};*/
						//expn[g]=pub_nx[g];
						expn[0]=4;expn[1]=2;expn[2]=9;expn[3]=0;expn[4]=7;expn[5]=2;expn[6]=6;expn[7]=3;expn[8]=1;
						expn[9]=0;expn[10]=3;expn[11]=8;expn[12]=1;expn[13]=0;expn[14]=8;expn[15]=4;expn[16]=1;expn[17]=2;
						expn[18]=8;expn[19]=6;expn[20]=3;expn[21]=1;expn[22]=0;expn[23]=6;expn[24]=1;expn[25]=1;expn[26]=3;
						expn[27]=5;expn[28]=7;expn[29]=6;expn[30]=3;expn[31]=7;expn[32]=1;expn[33]=8;expn[34]=7;expn[35]=4;
						expn[36]=4;expn[37]=1;expn[38]=2;expn[39]=4;expn[40]=3;expn[41]=8;expn[42]=7;expn[43]=2;expn[44]=1;
						expn[45]=1;expn[46]=4;expn[47]=1;expn[48]=9;expn[49]=3;expn[50]=4;expn[51]=5;expn[52]=1;expn[53]=1;
						expn[54]=9;expn[55]=6;expn[56]=3;expn[57]=3;expn[58]=3;expn[59]=8;expn[60]=1;expn[61]=0;expn[62]=5;
						expn[63]=1;expn[64]=7;expn[65]=5;expn[66]=1;expn[67]=2;expn[68]=1;expn[69]=1;expn[70]=2;expn[71]=7;
						expn[72]=1;expn[73]=0;expn[74]=8;expn[75]=5;expn[76]=7;expn[99]=77;
						
					}

					pri[99]=31;

					for(k=0;k<100;k++)
					{
						oprintf("----x%d-----\n",e[k]);
					}
					for(j=0;j<100;j++)
					{
						oprintf("xxxxxx%d-----\n",n[j]);
					}
					for(s=0;s<100;s++)
					{
						oprintf("*************%d-----\n",d[s]);
					}
					
					tencrypto(expx, n, WECHAT_HEAD,buf,512); //e,n
					//oprintf("--------------%s\n",buf);

					tdecrypto(d,n, buf,bufx,512); //d n
					//oprintf("--------------%s\n",bufx);
				}

			}
#endif

			//language key
			if(cur_key == KEY_FUN8 && !IsViewProtect())//20190109
			{
				//p10 language jump 20180823 hz
				if (press)
				{
					PageFramHide(PanelCurPage());
					SetCurPage(m_pwdJumpPages[1]);//5858页面
					NaviUpdate();                 //刷新导航条
					SendKey(KEY_FUN7);
				}

#if 0  //the old way two language shift 20180823 hz
				if(press)
				{
					UI16 id;

					id = VarAdrToUI16(LANGE_SELECT);
					if(id==1) id =0;
					else if(id==0) id=1;
					VarSendSaveSetIntByAdr(LANGE_SELECT,id);

					m_currentLangId = (LANG_ID)id;

					PageModMsgCall(PanelCurPage(), NULL, PAGE_SHOW_MSG, 0, 0);//当前页面重新刷新

					if(PageFramGetPriv()>0)//当前页面权限
					{
						PanelSetJump(PanelCurPage());
					}

					if(LoginPage())//登录框
					{
						PanelCloseDialog(LoginPage());
					}
				}
#endif
			}

			/*grab screen image*/
			if(cur_key == _KEY_GRAB_PNGIMAGE)
			{
				if(press)
				{
					/*need U disk exist*/
					if(check_usb())
					{
						ShowMsg(VW_MSG_UDISKNOTEXIST);
						return;
					}

                    if(GrabScreenImage(IMAGE_PNG))//20200804 截图增加返回值判断
                    {
                        ShowMsg(VW_MSG_SCREENSHOTSAVESUCCESS);
                    }
                    else
                    {
                        ShowMsg(VW_MSG_SCREENSHOTSAVEFAIL);
                    }
				}
				
			}
			else if ((cur_key == _KEY_GRAB_BMPIMAGE) || ((cur_key == _SPC) && VarAdrToUI16(SCREEN_SHOT)))
			{
				if(press)
				{
					/*need U disk exist*/
					if(check_usb())
					{
						ShowMsg(VW_MSG_UDISKNOTEXIST);
						return;
					}

                    if(GrabScreenImage(IMAGE_BMP))//20200804 截图增加返回值判断
                    {
                        ShowMsg(VW_MSG_SCREENSHOTSAVESUCCESS);
                    }
                    else
                    {
                        ShowMsg(VW_MSG_SCREENSHOTSAVEFAIL);
                    }
				}
			}

			/*help key*/
			if((cur_key == GUI_KEY_FUN7) && g_bhelplinkx)
			{
				if(press)
				{
					g_bhelplinkx = FALSE;
				}
			}
			else if((cur_key == GUI_KEY_FUN7) && g_bhelplink && (g_bhelplinkx == FALSE)) //F7返回
			{
				if(press)
				{
					g_bhelplink = FALSE;
				}
			}

			if(bmainhelp && (PanelCurPage() == PanelMainPage()))
			{
				bmainhelp = FALSE;
			}

            //20220805 dyl
            #ifdef PANEL_NOKEYBOARD//G15:No_KeyBoard //#ifdef CAPACITIVESCREEN  //电容屏P15-5排新按键
            if((cur_key == KEY_HELP) || (cur_key == _KEY_PANEL_SWX5))
        #else
            if(cur_key == KEY_HELP)
        #endif
			{
				static PPAGE_FRAM pmain = NULL;
				static PPAGE_FRAM pcur = NULL;
				static PPAGE_FRAM plast =NULL;

				//alarmhelp
				static BOOL balarmhelp =FALSE;
				static PPAGE_FRAM palarm = NULL;

				//pagehelp
				static BOOL bpagehelp =FALSE;
				static PPAGE_FRAM ppage = NULL;


				if(press)
				{
                    if((PanelCfgStyle() == G_TYPE)) //20190426
                    {
                        if(PanelTopDialog() == PanelFindPage(PAGE_PAGEHELP))
                        {
                            PanelCloseDialogByName(PAGE_PAGEHELP);
                        }
                        else
                        {
                            if(g_warm1!=0 || g_warm2!=0 || g_warm3!=0 || g_warm4!=0
                                     || g_warm5!=0 || g_warm6!=0 || g_warm7!=0 || g_warm8!=0
                                     || g_warm9!=0 || g_warm10!=0 || dwPanelErr==0x34100101)//20200629 有警报才显示帮助画面
                            {
                                PanelShowDialogByName(PAGE_PAGEHELP);
                            }
                        }
                        return;
                    }
                    else if(PanelCfgStyle() == P_TYPE)
                    {
                        if(g_warm1!=0 || g_warm2!=0 || g_warm3!=0 || g_warm4!=0
                                || g_warm5!=0 || g_warm6!=0 || g_warm7!=0 || g_warm8!=0
                                || g_warm9!=0 || g_warm10!=0 || dwPanelErr==0x34100101)//20200629 有警报才显示帮助画面
                        {
                            PanelShowPageByName(PAGE_PAGEHELP);
                        }
                        return;
                    }

					pmain = PanelMainPage(); //
					pcur = PanelCurPage();

					//F7Back
					if(balarmhelp && pcur!=palarm)
					{
						balarmhelp =FALSE;
					}
					else if(bmainhelp && pcur == pmain)  
					{   
						bmainhelp = FALSE;
					}
					else if(bpagehelp && pcur!= ppage)
					{
						bpagehelp = FALSE;
					}

					if (bpagehelp)
					{
						bpagehelp = FALSE;
						PanelShowPage(plast);
					}
					else if(balarmhelp)
					{
						balarmhelp = FALSE;
						PanelShowPage(plast);
					}
					else if(bmainhelp)
					{ 
						if(g_bhelplinkx)   //alarm help link(back alarm help link)
						{
							g_bhelplinkx = FALSE;
							PanelShowPage(PanelLastPage());
						}
						else 
						{
							if(g_bhelplink)  //page help link(back main help)
							{
								g_bhelplink =FALSE;
								PanelShowPageByName(PAGE_MIANHELP);
							}
							else         
							{
								bmainhelp = FALSE;
								PanelShowPage(pmain);
							}
						}
					}
					else
					{
						if((g_warm1 != 0) && !balarmhelp)  //alarm help priority
						{ 
							palarm = PanelFindPage(PAGE_ALARMHELP);
							balarmhelp = TRUE;
							plast = PanelCurPage();
							PanelShowPage(palarm); 
						}
						else if(pcur == pmain && !bmainhelp) //main help
						{ 
							bmainhelp = TRUE;
							PanelShowPageByName(PAGE_MIANHELP);
						}
						else if(!bpagehelp)                 //page help
						{
							ppage = PanelFindPage(PAGE_PAGEHELP);
							bpagehelp = TRUE;
							plast = PanelCurPage();
							PanelShowPage(ppage);
						}
					}
				}
			}//help key end
//#endif
		}
	}
	else
	{
		if((g_warm1 == 0) && g_bwarn)
		{
			g_bwarn =FALSE;
		}

		if((GetTick() - time_1s) > 1000)
		{
			time_1s = GetTick();
			g_tmNoKeyPress++;
            g_tmPanelNoKeyPress++;//20200221

			//screen saver
            if((g_tmPanelNoKeyPress > VarAdrToUI16(SYSCFG_ACTIVETIME) * 60)&&(!g_bwarn) && VarAdrToUI16(SYSCFG_ACTIVETIME)!=0)
			{
////20220426
//#ifdef CAPACITIVESCREEN
                if(/*IsG15Type()||*/ IsTouchType())//20190629.cyx + //20220104 dyl
                {
                    if (VarAdrToInt(PT15_SCREEN_PROTECT) == 0)//20230518 chj 优化关屏调用频率
                    {
                        SetLCD(FALSE);//20190417
                        Send_LCDOnOff(FALSE);//20190822.cyx
                        VarAdrSetInt(PT15_SCREEN_PROTECT,1);
                    }
                }
                else
//#endif
                {
                    if (bLCD_state)//20230518 chj 优化关屏调用频率
                    {
                        SetLCD(FALSE);//20190417
                        Send_LCDOnOff(FALSE);//20190822.cyx
                        bLCD_state=FALSE;
                    }
                }
			}
            //G10 20分钟自动登出 //20191121.cyx
            if(PanelCfgStyle() == G_TYPE)
            {
                if(VarAdrToInt(PT15_USER_INITRANK) == 0)
                {
                    //自学习中不登出
                    if((g_tmPanelNoKeyPress > VarAdrToUI16(SYSCFG_ACTIVETIME) * 60) && VarAdrToUI16(SYSCFG_ACTIVETIME)!=0 && (GetCurrentUserId() > USER_EVERYONE) && (VarAdrToInt(p_PP_MACHSET_SELF_STUDY_FLAG)==0))
                    {
                        VarAdrSetInt(USER_STATE,1);
                        SetCurrentUserId(USER_EVERYONE);
                        VarAdrSetInt(USER_KEYLOCK,1);//用户权限管理:生产人员lock
                        if(!IsPayTimeOut() && (PanelCurPage() != PanelFindPage(PAGE_PROGUPDATE)))//20191219 停机不跳转 更新页面不跳转
                        {
                            PanelShowPage(PanelMainPage());//自动登出时跳转到主画面
                            HideFrmInput();//20220104 dyl
                        }
                    }
                }
                else if(VarAdrToInt(PT15_USER_INITRANK) >= 1)
                {
                    //自学习中不登出
                    if((g_tmPanelNoKeyPress > VarAdrToUI16(SYSCFG_ACTIVETIME) * 60) && VarAdrToUI16(SYSCFG_ACTIVETIME)!=0 && (GetCurrentUserId() > USER_PROFESSIONAL) && (VarAdrToInt(p_PP_MACHSET_SELF_STUDY_FLAG)==0))
                    {
                        VarAdrSetInt(USER_STATE,1);
                        SetCurrentUserId(USER_PROFESSIONAL);
                        if(!IsPayTimeOut() && (PanelCurPage() != PanelFindPage(PAGE_PROGUPDATE)))//20191219 停机不跳转 更新页面不跳转
                        {
                            PanelShowPage(PanelMainPage());//自动登出时跳转到主画面
                            HideFrmInput();//20220104 dyl
                        }
                    }
                }
            }
		}

		if(g_bwarn){
////20220426
//#ifdef CAPACITIVESCREEN
            if(/*IsG15Type()||*/ IsTouchType())//20190629.cyx + //20220104 dyl
            {
                if (VarAdrToInt(PT15_SCREEN_PROTECT))
                {
                    SetLCD(TRUE);//20190417
                    Send_LCDOnOff(TRUE);//20190822.cyx
                    VarAdrSetInt(PT15_SCREEN_PROTECT,0);
                }
            }
            else
//#endif
            {
                if(!bLCD_state)
                {
                    SetLCD(TRUE);//20190417
                    Send_LCDOnOff(TRUE);//20190822.cyx
                    bLCD_state=TRUE;
                }
            }
        }
	}
}

TESTKEY_CB SetTestKeyCB(TESTKEY_CB cb)
{
	TESTKEY_CB ret = testkey_cb;
	testkey_cb = cb;
	return ret;
}

void TestKeyStart()
{
	VarAdrSetInt(KEYTEST_ADDR,1);
}

void TestKeyEnd()
{
	VarAdrSetInt(KEYTEST_ADDR,0);
}

DAADJUST_CB SetDaAdjustCB(DAADJUST_CB da_linear)
{
	DAADJUST_CB ret;

	ret = daadjust_cb;
	daadjust_cb = da_linear;
	return ret;
}

ADEX_CB SetADExCB(ADEX_CB data_change)
{
    ADEX_CB ret;

    ret = adex_change_cb;
    adex_change_cb = data_change;
    return ret;
}

ADX_CB SetADxCB(ADX_CB data_change)
{
    ADX_CB ret;

    ret = adx_change_cb;
    adx_change_cb = data_change;
    return ret;
}

MACHCFG_CB SetMachcfgCB(MACHCFG_CB data_change)
{
	MACHCFG_CB ret;

	ret = machcfg_change_cb;
	machcfg_change_cb = data_change;
	return ret;
}

MACHCFG5200_CB SetMachcfg5200CB(MACHCFG5200_CB data_change)
{
    MACHCFG5200_CB ret;

    ret = machcfg5200_change_cb;
    machcfg5200_change_cb = data_change;
    return ret;
}

//20241223 fqh pump修改的回调函数
PUMPFUNC_CB SetPumpFuncCB(PUMPFUNC_CB data_change)
{
    PUMPFUNC_CB ret;

    ret = pumpfunc_change_cb;
    pumpfunc_change_cb = data_change;
    return ret;
}

DAADJ_CB SetDaAdjCB(DAADJ_CB data_change)
{
	DAADJ_CB ret;

	ret = daadj_change_cb;
	daadj_change_cb = data_change;
	return ret;
}

SPECFUNC_SELECT_CB SetSpecFuncSelectCB(SPECFUNC_SELECT_CB data_change)
{
    SPECFUNC_SELECT_CB ret;

    ret = specfuncselect_change_cb;
    specfuncselect_change_cb = data_change;
    return ret;
}

//20190507 ybc
IO_OUT_CB SetIOOutZeroCB(IO_OUT_CB force_value_zero)
{
    IO_OUT_CB ret = NULL;
    UI16 i = 0;

    for(i = 0;i < MAX_IO_OUT_CB;i++)
    {
        if((ioout_force_cb[i] == NULL) || (ioout_force_cb[i] == force_value_zero))
        {
            ioout_force_cb[i] = force_value_zero;
            ret = ioout_force_cb[i];

            break;
        }
    }

	return ret;
}

//20190507 ybc
IO_OUT2_CB SetIOOut2ZeroCB(IO_OUT2_CB force_value_zero)
{
    IO_OUT_CB ret = NULL;
    UI16 i = 0;

    for(i = 0;i < MAX_IO_OUT_CB;i++)
    {
        if((ioout2_force_cb[i] == NULL) || (ioout2_force_cb[i] == force_value_zero))
        {
            ioout2_force_cb[i] = force_value_zero;
            ret = ioout2_force_cb[i];

            break;
        }
    }

	return ret;
}

#ifdef USE_QT
/**
* @brief     :tab控件换页
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191202
*/

static UI32 g_fkey;
static PWGT g_tabpage;
static PPAGE_FRAM tab_cur_page;
TAB_CB SetTabCB(TAB_CB cb)
{
    tab_cb = cb;
    return NULL;
}

void TabKey(UI32 key,UI32 press)
{
    if(key == g_fkey && tab_cur_page == PanelCurPage() && (PanelDialogCnt() < 1))//有弹出框时不切换
    {
        UI16 tab_id = TabPageGetCurSel(g_tabpage);
        if(tab_id != TabPageSelNext(g_tabpage, TRUE))
        {
            SendKey(_DARROW);
        }
        PageWgtSetFocus(g_tabpage);
    }
}

/**
* @brief     :tab键画面切换 画面show的时候调用
* @param     :tab控件名称  画面对应的F键
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191202
*/
void FkeyTab(PWGT tabpage, UI32 fkey)
{
    SetTabCB(TabKey);
    g_fkey = fkey;
    g_tabpage = tabpage;
    tab_cur_page = PanelCurPage();
}
#endif


/**
* @brief     :ecat泵浦画面数据修改
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191202
*/
ECAT_CB SetEcatCB(ECAT_CB cb)
{
    ecat_cb = cb;
    return NULL;
}

/**
* @brief     :G10主画面按键特殊处理
* @param     :
* @return    :
* @retval    :
* @note      :
* @attention :
* @author    :hz
* @date      :20191204
*/
MAIN_CB SetMainCB(MAIN_CB cb)
{
    main_cb = cb;
    return NULL;
}

/**
 * @brief      : 数据检测画面按键特殊处理
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200117
 */
DATADIAG_CB SetDataDiagCB(DATADIAG_CB cb)
{
    datadiag_cb = cb;
    return NULL;
}

/**
 * @brief      : 设置取消对应控件类别(用于IO输入页面临时取消复选框光标)
 * @param      : 控件类别
 * @return     : TRUE:指定控件类别不聚焦
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200313
 */
CANCELFOCUS_CB SetCancelFocusCB(CANCELFOCUS_CB cancelfocus_cb)
{
    CANCELFOCUS_CB old_cb;

    old_cb = cancelfocus_cb;
    cancelfocuscb = cancelfocus_cb;

    return old_cb;
}

/**
* @brief     :设置取消对应控件类别
* @param     :控件类别
* @return    :TRUE:指定控件类别不聚焦
* @retval    :
* @note      :
* @attention :
* @author    :ybc
* @date      :20191214
*/
BOOL SetCancelFocus(int obj_type)
{
    BOOL bcancel = FALSE;

    if(obj_type == OBJ_TABPAGE) //G10 多页控件光标不聚焦
    {
        bcancel = TRUE;
    }

    if(cancelfocuscb !=NULL)
    {
        if((*cancelfocuscb)(obj_type))
        {
            bcancel = TRUE;
        }
    }

    return bcancel;
}

/**
 * @brief      : 伺服页面换页键特殊处理
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200723
 */
SERVOSTATEX1_CB SetServoStatex1CB(SERVOSTATEX1_CB cb)
{
    servostatex1_cb = cb;
    return NULL;
}

/**
 * @brief      : 重置屏保时间计数
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       : 点击屏幕即重置时间
 * @attention  :
 * @author     : cyx
 * @date       : 20191224
 */
void Reset_ActiveTime_Count()
{
    g_tmNoKeyPress=0;
    g_tmPanelNoKeyPress =0;//20200221

#ifdef A40I//20250319 chj 优化点屏幕功能，每次亮屏，需要获取驱动信号，来确定状态
    if(GetScreenState() == FALSE)
#else
    if (VarAdrToInt(PT15_SCREEN_PROTECT))
#endif
    {
        SetLCD(TRUE);//20190417
        Send_LCDOnOff(TRUE);//20190822.cyx
        VarAdrSetInt(PT15_SCREEN_PROTECT,0);
        bLCD_state = TRUE;//20230518 chj 优化关屏调用频率
    }
}

//20250319 jhh 更新画面主机更新过程中不进入屏保
void Reset_PanelNoKeyPress()
{
    g_tmNoKeyPress=0;
    g_tmPanelNoKeyPress =0;
}

/**
 * @brief      : 获取面板锁的状态
 *
 * @param      :
 * @return     :
 * @retval     :
 * @note       : 主画面调用 刷新锁图标
 * @attention  :
 * @author     : cyx
 * @date       : 20210708
 */
BOOL GetLockState()
{
    if ((IsDataProtect() || IsViewProtect()) ||(IsDataLock()))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
* @brief     :按向下键设置多页控件的聚焦
* @param     :无
* @return    :无
* @retval    :
* @note      :
* @attention :
* @author    :ybc
* @date      :20210804
*/
BOOL DownSetTabPageFocus()
{
    PPAGE_FRAM pcur = NULL;
    int goffset =0;
    int index;
    PWGT pwgt =NULL;
    static BOOL bjump =TRUE;

    pcur = PanelCurPage();
    if(pcur)
    {
        goffset = WGTGetGOffset(pcur->plast_focus);
        //解决同列中多个设置跳转标记的情况
        for(index=JUMPINTABPAGE_FLAG_MIN;index<=JUMPINTABPAGE_FLAG_MAX;index++)
        {
            pwgt = PageFramCurWgtByIndex(index);
            if(pwgt && WGTCanFocus(pwgt)
                    && (WGTGetYPos(pwgt)-WGTGetYPos(pcur->plast_focus)>=20) //上下控件间隔大于20
                    && (WGTGetXPos(pwgt)-WGTGetXPos(pcur->plast_focus)==0))
            {
                bjump = FALSE;
                break;
            }
            else
            {
                bjump = TRUE;
            }
        }

        if( bjump && tab_cur_page == pcur && (goffset>=JUMPINTABPAGE_FLAG_MIN && goffset<=JUMPINTABPAGE_FLAG_MAX)) //控件组内索引的10000~10020用作多页控件的向下按键的聚焦的跳转
        {
            PageWgtSetFocus(g_tabpage);
            SendKey(_DARROW);
            return TRUE;
         }
        else
        {
            //PageFramSetOrderFocus(pcur,pcur->pyo_tbl,TRUE,WGTGetYOrder(pcur->plast_focus));
            return FALSE;
        }
    }
}

/**
* @brief     :画面光标xo重新排序
* @param     :无
* @return    :无
* @retval    :
* @note      :
* @attention :
* @author    :dyl
* @date      :20211229
*/
BOOL PageFramXSort(PPAGE_FRAM pf)
{
    int i;
    POBJ_INFO pinfo;
    int xo;

    for(i = 0; i < pf->obj.sub_obj_num; i++)
    {
        if((pinfo = OBJFindInfoByType(pf->obj.psub[i]->type)) != NULL)
        {
            if(pinfo->attribbits & WGT_OBJ)
            {
                xo = WGTGetXOrder((PWGT)(pf->obj.psub[i]));
                if(xo>0 && (pf->pxo_tbl!=NULL))
                {
                    QSTFind(pf->pxo_tbl, xo);
                    POBJ pobj = (POBJ)QSTIter(pf->pxo_tbl)->value;
                    if(pobj != NULL)
                    {
                        if(pobj != pf->obj.psub[i])
                        {
                            QSTIter(pf->pxo_tbl)->value = pf->obj.psub[i];
                        }
                    }
                }
            }
        }
    }

    QSTSort(pf->pxo_tbl);
}

/**
* @brief     :子窗体光标重新排序
* @param     :无
* @return    :无
* @retval    :
* @note      :
* @attention :
* @author    :dyl
* @date      :20211229
*/
BOOL PageSubXExChange(PPAGE_FRAM pf,const char *wgtname, int srcxo,int dstxo)
{
    POBJ psrcobj = NULL;
    POBJ pdstobj = NULL;

    if(pf->pxo_tbl!=NULL)
    {
        QSTFind(pf->pxo_tbl, dstxo);
        pdstobj = (POBJ)QSTIter(pf->pxo_tbl)->value;
        if(strcmp(wgtname, RSTR(((PWGT)pdstobj)->name)) == 0)
        {
            return 0;
        }
        QSTFind(pf->pxo_tbl, srcxo);
        psrcobj = (POBJ)QSTIter(pf->pxo_tbl)->value;
        QSTIter(pf->pxo_tbl)->value = pdstobj;
        QSTFind(pf->pxo_tbl, dstxo);
        QSTIter(pf->pxo_tbl)->value = psrcobj;

        QSTSort(pf->pxo_tbl);
    }
}

//20250319 chj 优化点屏幕功能，获取命令的反馈数值
int find_number(const char* filename, const char* search_str)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    int ch;
    int match_pos = 0;
    int found = (search_str == NULL); // 无搜索字符串时直接找数字

    while ((ch = fgetc(fp)) != EOF)
    {
        // 阶段1：匹配目标字符串
        if (!found && search_str)
        {
            if (ch == search_str[match_pos])
            {
                if (search_str[++match_pos] == '\0') found = 1;
            }
            else
            {
                match_pos = 0;
            }
            continue;
        }

        // 阶段2：查找数字
        if (isdigit(ch) || ch == '+' || ch == '-')
        {
            ungetc(ch, fp); // 回退字符用于完整读取数字
            int num;
            if (fscanf(fp, "%d", &num) == 1)
            {
                fclose(fp);
                return num;
            }
        }
    }

    fclose(fp);
    return -1; // 未找到
}

//20250319 chj 优化点屏幕功能，获取PWM数值
int GetPWMDATA()
{
    int pwmdata = find_number("/sys/class/backlight/backlight_lvds/brightness",NULL);
    //oprintf("pwmdata----------:%d\n",pwmdata);
    return pwmdata;
}

//20250319 chj 优化点屏幕功能，获取背光IO数值
static BOOL B_Mount_Debugfs = FALSE;
int GetGPIODATA_273()
{
    if(B_Mount_Debugfs == FALSE)
    {
        //这里调用system，不调用总的osystem
        system("mount -t debugfs none /sys/kernel/debug");
        system("echo PI17 > /sys/kernel/debug/sunxi_pinctrl/sunxi_pin");
        B_Mount_Debugfs = TRUE;
    }

    int IOdata = find_number("/sys/kernel/debug/sunxi_pinctrl/data","data:");
    //oprintf("IOdata----------:%d\n",IOdata);
    return IOdata;
}

//20250319 chj 优化点屏幕功能，每次亮屏，需要获取驱动信号，来确定状态
BOOL GetScreenState()
{
    int pwmdata = GetPWMDATA();
    if(pwmdata == -1)
    {
        return FALSE;
    }

    int iodata = GetGPIODATA_273();
    if(iodata == -1)
    {
        return FALSE;
    }

    if((pwmdata != 9) && (iodata == 0))//亮
    {
        return TRUE;
    }
    else//灭
    {
        return FALSE;
    }
}

int set_brightness(int value)
{
    char buf[128];
    int fd, len;

    if ((fd = open("/sys/class/backlight/backlight_lvds/brightness", O_WRONLY)) == -1) {
        oprintf("open brightness error\n");
        return -1;
    }

    len = snprintf(buf, sizeof(buf)-1, "%d\n", value);
    if (write(fd, buf, len) != len) {
        oprintf("write brightness error\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

