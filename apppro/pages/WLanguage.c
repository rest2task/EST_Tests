#include "WLanguage.h"
#include "typedef.h"
#include "pagefram.h"
#include "database.h"
#include "mod.h"
#include "usermod.h"//20210914 dyl touch

DEFINE_LANGUAGE_MAP

CONNECT_LANGUAGE_EVENT

//language addr
#define TMP_ENGLIASH	    TMPVAL(0)//英语
#define TMP_CHINESE	        TMPVAL(1)//中文
#define TMP_PERSIAN 	    TMPVAL(2)//波斯语
#define TMP_RUSSIAN 	    TMPVAL(3)//俄语
#define TMP_SPANISH 	    TMPVAL(4)//西班牙语

#define TMP_TURKEY  	    TMPVAL(5)//土耳其语
#define TMP_PORTUGAL 	    TMPVAL(6)//葡萄牙语
#define TMP_ARABIC  	    TMPVAL(7)//阿拉伯语
#define TMP_FRENCH  	    TMPVAL(8)//法语
#define TMP_KOREAN  	    TMPVAL(9)//韩国
#define TMP_VIETNAMESE      TMPVAL(10)//越南语 20180827 hz

extern  LANG_ID m_currentLangId; //全局当前语言ID

static void LanguagePageIn()
{
	UI16 i,j;
	UI16 lang_id;

	lang_id = VarAdrToUI16(LANGE_SELECT);
	if(lang_id == 0)
	{
		VarAdrSetInt(TMP_ENGLIASH,1);
		for(i=1;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 1)
	{
		VarAdrSetInt(TMP_CHINESE,1);
		VarAdrSetInt(TMP_ENGLIASH,0);
		for(i=2;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 2)
	{
		VarAdrSetInt(TMP_PERSIAN,1);
		for(j=0;j<2;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		for(i=3;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 3)
	{
		VarAdrSetInt(TMP_RUSSIAN,1);
		for(j=0;j<3;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		for(i=4;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 4)
	{
		VarAdrSetInt(TMP_SPANISH,1);
		for(j=0;j<4;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		for(i=5;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 5)
	{
		VarAdrSetInt(TMP_TURKEY,1);
		for(j=0;j<5;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		for(i=6;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 6)
	{
		VarAdrSetInt(TMP_PORTUGAL,1);
		for(j=0;j<6;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		for(i=7;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 7)
	{
		VarAdrSetInt(TMP_ARABIC,1);
		for(j=0;j<7;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		for(i=8;i<11;i++)
			VarAdrSetInt(TMP_ENGLIASH+i,0);
	}
	else if(lang_id == 8)
	{
		VarAdrSetInt(TMP_FRENCH,1);
		for(j=0;j<8;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		VarAdrSetInt(TMP_KOREAN,0);
		VarAdrSetInt(TMP_VIETNAMESE,0);      // 20180827 hz
	}
	else if(lang_id == 9)
	{
		VarAdrSetInt(TMP_KOREAN,1);
		for(j=0;j<9;j++)
			VarAdrSetInt(TMP_ENGLIASH+j,0);
		VarAdrSetInt(TMP_VIETNAMESE,0);      // 20180827 hz
	}
	//20180827 hz
	else if (lang_id == 10)
	{
		VarAdrSetInt(TMP_VIETNAMESE, 1);
		for (j = 0; j < 10; j++)
		{
			VarAdrSetInt(TMP_ENGLIASH + j, 0);
		}
	}
}

static void LangSetChg(UI32 address)
{
	PVAR pdata;
	UI16 wValue,i,j;
	UI16 index;

	index =(UI16)(address - TMP_ENGLIASH);
	wValue = VarAdrToUI16(TMP_ENGLIASH+index);
	if(index ==0)
	{
		if(wValue==1)
		{
			for(i=1;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==1)
	{
		if(wValue==1)
		{
			VarAdrSetInt(TMP_ENGLIASH,0);
			for(i=2;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==2)
	{
		if(wValue==1)
		{
			for(j=0;j<2;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			for(i=3;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==3)
	{
		if(wValue==1)
		{
			for(j=0;j<3;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			for(i=4;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==4)
	{
		if(wValue==1)
		{
			for(j=0;j<4;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			for(i=5;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==5)
	{
		if(wValue==1)
		{
			for(j=0;j<5;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			for(i=6;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==6)
	{
		if(wValue==1)
		{
			for(j=0;j<6;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			for(i=7;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==7)
	{
		if(wValue==1)
		{
			for(j=0;j<7;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			for(i=8;i<11;i++)
				VarAdrSetInt(TMP_ENGLIASH+i,0);
		}
	}
	else if(index ==8)
	{
		if(wValue==1)
		{
			for(j=0;j<8;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			VarAdrSetInt(TMP_KOREAN,0);
			VarAdrSetInt(TMP_VIETNAMESE,0);      // 20180827 hz
		}
	}
	else if(index ==9)
	{
		if(wValue==1)
		{
			for(j=0;j<9;j++)
				VarAdrSetInt(TMP_ENGLIASH+j,0);
			VarAdrSetInt(TMP_VIETNAMESE,0);      // 20180827 hz
		}
	}
	// 20180827 hz
	else if (index == 10)
	{
		if(wValue == 1)
			for (j = 0; j < 10; j++)
			{
				VarAdrSetInt(TMP_ENGLIASH + j, 0);
			}
	}
	VarAdrSetInt(LANGE_SELECT,index);
	if(pdata = VarAdrToVar(LANGE_SELECT))
		VarSaveInt(pdata);
	m_currentLangId =(LANG_ID)index;

}

static void CtlConnect()
{
}
static int LanguageOnInit(Language_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int LanguageOnShow(Language_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_USERLOCK);//20210914 dyl touch

	LanguagePageIn();

    return 0;
}
static int LanguageOnHide(Language_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    SetPageLockMode(MODE_NOLOCK);//20210914 dyl touch

    return 0;
}
static int LanguageOnUpdate(Language_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
    return 0;
}
static int LanguageOnChange(Language_PG* ppg, PWGT pwgt, MSG_ID msg_id, int msg_value, int msg_para)
{
	UI32 addr;
	PATTR pattr;

	pattr = (PATTR)msg_para;

	addr = OBJAttrGetAdr(pattr);

	if(addr >= TMP_ENGLIASH && addr <= TMP_VIETNAMESE)   // 20180827 hz
	{
		LangSetChg(addr);
	}

    return 0;
}
