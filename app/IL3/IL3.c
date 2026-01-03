#ifdef PLC_ENABLE

#include	"IL3.h"
#include	"FunEx.h"
#include	"stdlib.h"
#include	"ioproc.h"
#include	"memtbl.h"
#include	"printf.h"
#include 	"actctrl.h"

DWORD				dwInputX,dwInputXB;//X输入
DWORD				dwOutputY,dwOutputYB,dwOutPutMark;//Y输出

WORD				*pIL;//扫描指针
DWORD				dwLength;//所有BIN文件的字数

void				(*Fun[256])();

BOOL				bRight;
WORD				wLD;

BOOL				bRightMpp[20];
WORD				wCurMpp;

DWORD				dwMRegister[20],dwMRegisterB[20];//M继电器
DWORD				dwSRegister[20],dwSRegisterB[20];//S继电器
TIMESTRU            Timer[600];//T
COUNTSTRU           Counter[500];//C
WORD				wVZRegisterB[16],wVZRegister[16];

FORSTRU             Forer[20];
WORD				wForPos;
CALLSTRU            Caller[20];
WORD                wCallerPos;
ADDRESSTABLESTRU	AddressTable[300];

WORD				wPlcUpdate = 0;
WORD				wPLcCrcOK = 0;
BOOL				bReceiveBegin;
DWORD				dwPlcFileLength;
CanPlcTestComm		g_CanPlcTestComm;
WORD				wPlcGetBuffer;
DWORD				PlcIAPToFlashAddress;
WORD				*PlcRecFileAdd;

void InitAnaIL()
{
	WORD	i;
	dwInputX=dwInputXB=0;
	dwOutputY=dwOutputYB=0;
	wPlcGetBuffer=0;
	PlcIAPToFlashAddress=Addr_PlcEntry;

	for(i=0;i<600;i++)
		Timer[i].bTimeRun=FALSE;

	for(i=0;i<500;i++)
		Counter[i].bCountRun=FALSE;

	for(i= 0; i<PLC_D_USER_SIZE; i++)
		m_plc_d[i + PLC_D_USER_OFFSET]=0;

	for(i=0;i<20;i++)
	{
		dwMRegister[i]=0;
		dwMRegisterB[i]=0;
	}

	wForPos=0;

	wCallerPos=0;
	wPlcUpdate=0;
	bReceiveBegin=TRUE;

	bRight=TRUE;

	g_CanPlcTestComm.flag_send=FALSE;
	g_CanPlcTestComm.wrxPos=0;

	CrcCheck();
	
	Fun[7]=Command8;
	Fun[8]=Command9;
	Fun[9]=Command10;
	Fun[10]=Command11;
	Fun[11]=Command12;
	Fun[12]=Command13;
	Fun[13]=Command14;
	Fun[14]=Command15;
	Fun[15]=Command16;
	Fun[16]=Command17;
	Fun[17]=Command18;
	Fun[18]=Command19;
	Fun[19]=Command20;
	Fun[20]=Command21;
	Fun[21]=Command22;
	Fun[22]=Command23;
	Fun[23]=Command24;
	Fun[24]=Command25;
	
	Fun[29]=Command30;
	Fun[30]=Command31;
	Fun[31]=Command32;
	Fun[32]=Command33;
	Fun[33]=Command34;
	Fun[34]=Command35;
	Fun[35]=Command36;
	Fun[36]=Command37;
	Fun[37]=Command38;
	Fun[38]=Command39;
	Fun[39]=Command40;
	Fun[40]=Command41;
	Fun[41]=Command42;
	Fun[42]=Command43;
	Fun[43]=Command44;
	Fun[44]=Command45;
	Fun[45]=Command46;
	Fun[46]=Command47;
	Fun[47]=Command48;
	Fun[48]=Command49;
	Fun[49]=Command50;
	Fun[50]=Command51;
	Fun[51]=Command52;
	Fun[52]=Command53;
	Fun[53]=Command54;
	Fun[54]=Command55;
	Fun[55]=Command56;
	Fun[56]=Command57;
	Fun[57]=Command58;
	Fun[58]=Command59;
	Fun[59]=Command60;
	Fun[60]=Command61;
	Fun[61]=Command62;
	Fun[62]=Command63;
	Fun[63]=Command64;
	Fun[64]=Command65;
	Fun[65]=Command66;
	Fun[66]=Command67;
	Fun[67]=Command68;
	Fun[68]=Command69;
	Fun[69]=Command70;
	Fun[70]=Command71;
	Fun[71]=Command72;
	Fun[72]=Command73;
	
	Fun[75]=Command76;
	Fun[76]=Command77;
	Fun[77]=Command78;
	Fun[78]=Command79;
	Fun[79]=Command80;
	Fun[80]=Command81;
	Fun[81]=Command82;
	Fun[82]=Command83;
	Fun[83]=Command84;
	Fun[84]=Command85;
	Fun[85]=Command86;
	Fun[86]=Command87;
	Fun[87]=Command88;
	Fun[88]=Command89;
	Fun[89]=Command90;
	Fun[90]=Command91;
	Fun[91]=Command92;
	Fun[92]=Command93;
	Fun[93]=Command94;
	Fun[94]=Command95;
	Fun[95]=Command96;
	Fun[96]=Command97;
	Fun[97]=Command98;
	Fun[98]=Command99;
	Fun[99]=Command100;
	Fun[100]=Command101;
	Fun[101]=Command102;
	Fun[102]=Command103;
	Fun[103]=Command104;
	Fun[104]=Command105;
	Fun[105]=Command106;
	Fun[106]=Command107;
	Fun[107]=Command108;
	Fun[108]=Command109;
	Fun[109]=Command110;
	Fun[110]=Command111;
	Fun[111]=Command112;
	Fun[112]=Command113;
	Fun[113]=Command114;
	Fun[114]=Command115;
	Fun[115]=Command116;
	Fun[116]=Command117;
	Fun[117]=Command118;

	Fun[118]=Command119;
	Fun[119]=Command120;
	Fun[120]=Command121;
	Fun[121]=Command122;
	Fun[122]=Command123;
	Fun[123]=Command124;
	Fun[124]=Command125;
	Fun[125]=Command126;
	Fun[126]=Command127;
	Fun[127]=Command128;
	Fun[128]=Command129;
	Fun[129]=Command130;
	Fun[130]=Command131;
	Fun[131]=Command132;
	Fun[132]=Command133;
	Fun[133]=Command134;
	Fun[134]=Command135;
	Fun[135]=Command136;
	Fun[136]=Command137;
	Fun[137]=Command138;
	Fun[138]=Command139;
	Fun[139]=Command140;
	Fun[140]=Command141;
	Fun[141]=Command142;
	Fun[142]=Command143;
	Fun[143]=Command144;
	Fun[144]=Command145;
	Fun[145]=Command146;
	Fun[146]=Command147;
	Fun[147]=Command148;
	Fun[148]=Command149;
	Fun[149]=Command150;
	Fun[150]=Command151;
	Fun[151]=Command152;
	Fun[152]=Command153;
	Fun[153]=Command154;
	Fun[154]=Command155;
	Fun[155]=Command156;
	Fun[156]=Command157;
	Fun[157]=Command158;
	Fun[158]=Command159;
	Fun[159]=Command160;
	Fun[160]=Command161;
	Fun[161]=Command162;
	Fun[162]=Command163;
	Fun[163]=Command164;
	Fun[164]=Command165;
	Fun[165]=Command166;
	Fun[166]=Command167;
	Fun[167]=Command168;
	Fun[168]=Command169;
	Fun[169]=Command170;
	Fun[170]=Command171;
	Fun[171]=Command172;
	Fun[172]=Command173;
	Fun[173]=Command174;
	Fun[174]=Command175;
	Fun[175]=Command176;
	Fun[176]=Command177;
	Fun[177]=Command178;
	Fun[178]=Command179;
	Fun[179]=Command180;
	Fun[180]=Command181;
	Fun[181]=Command182;
	Fun[182]=Command183;
	Fun[183]=Command184;
	Fun[184]=Command185;
	Fun[185]=Command186;
	Fun[186]=Command187;
	Fun[187]=Command188;
	Fun[188]=Command189;
	Fun[189]=Command190;
	Fun[190]=Command191;
	Fun[191]=Command192;
	Fun[192]=Command193;
	Fun[193]=Command194;
	Fun[194]=Command195;
	Fun[195]=Command196;
	Fun[196]=Command197;
	Fun[197]=Command198;
	Fun[198]=Command199;
	Fun[199]=Command200;
	Fun[200]=Command201;
	Fun[201]=Command202;
	Fun[202]=Command203;

	Fun[203]=Command204;
	Fun[204]=Command205;
	Fun[205]=Command206;
	Fun[206]=Command207;
	Fun[207]=Command208;
	Fun[208]=Command209;
	Fun[209]=Command210;
	Fun[210]=Command211;
	Fun[211]=Command212;
	Fun[212]=Command213;
	Fun[213]=Command214;
	Fun[214]=Command215;
	Fun[215]=Command216;
	Fun[216]=Command217;

	for(i = 255; i > 0; i--)
		Fun[i] = Fun[i -1];

	InitAddressTable();

#if 0
	InitFunEx();
#endif
}


BOOL PLCIsUpdate()
{
	return (wPlcUpdate != 0);
}

WORD AnalysisIL(WORD wSub)
{
	WORD  wCommand,wCom1,wCom2,wCom3,wCom4,wCom5;

	if(!wPLcCrcOK)
		return 0;
	
	if(wPlcUpdate!=0)
		return 0;
	
	if(AddressTable[wSub].wSub>300)
		return 0;

	dwOutPutMark=0;
	wLD=0;
	wCallerPos=0;
	dwInputX=dwInputXB;
	dwInputXB=GetPI32(0);
	dwOutputY=GetPO32(0);
	memcpy(dwMRegisterB, dwMRegister, sizeof(dwMRegisterB));

	if(wSub != 0)
		pIL=(WORD*)((DWORD)(AddressTable[wSub].wAddressIn+ILADDRESS));
	else
		pIL=(WORD*)((DWORD)(ILADDRESS));
	
	//wCom1=*(pIL++);
	//wCom2=*(pIL++);
	//wCom3=*(pIL++);
	//wCom4=*(pIL++);
	//wCom5=*(pIL++);
	//db_printf("PLC:%x,%x,%x,%x,%x\r\n",wCom1,wCom2,wCom3,wCom4,wCom5);
	//db_printf("PLC:%x\r\n",dwInputXB);
	while(1)
	{
		wCommand=*(pIL++);
		//db_printf("PLC:%x,%d\r\n",pIL,wCommand);
		if(wCommand==82)
			break;
		else if(wCommand==77)
		{
			if(wCallerPos==0)
				break;
		}

		Fun[wCommand]();
	}
	
	dwOutputY&=~dwOutPutMark;
	dwOutputY|=dwOutputYB;
	memcpy(dwMRegister, dwMRegisterB, sizeof(dwMRegister));
	SetPO32(0,dwOutputY);
	return 1;
}

void CrcCheck()
{
	WORD wLen=*(WORD*)ILLENADDRESS;
	WORD wCrc=*(WORD*)(ILADDRESS+wLen*2);
	DWORD dwLen=(ILADDRESS-PLCSTARTADDRESS)+wLen*2;

	if(*(UI16*)PLCSTARTADDRESS == 0xBEBE && CRC_CCITT16((UI8*)PLCSTARTADDRESS,dwLen) ==wCrc)
	{
		wPLcCrcOK=1;
		db_printf("PLC CRC ok, %04x\r\n", wCrc);
	}
	else
	{
		wPLcCrcOK=0;
		db_printf("PLC CRC error\r\n");
	}
}

void SetOut(WORD wAddress,BOOL bMark)
{
	if(wAddress>=32)
		return;
	dwOutPutMark|=(1<<wAddress);
	if(bMark)
	{
		dwOutputYB|=(1<<wAddress);
	}
	else
	{
		dwOutputYB&=~(1<<wAddress);
	}
}

WORD  AnaValue(WORD wType)
{
	WORD i,wi;
	WORD wVal1,wVal2,wVal3;
	WORD wValue,wNum,wPos;
	wValue=0;
	switch(wType)
	{
	case 1://KH、T、C、D、V/Z
		wVal1=*(pIL++);
		wVal2=*(pIL++);
		if(wVal1==1)
			wValue=wVal2;
		else if(wVal1==2)
			wValue=Timer[wVal2].wCurTime-Timer[wVal2].wBeginTime;
		else if(wVal1==3)
			wValue=Counter[wVal2].wCurCount;
		else if(wVal1==4)
			wValue=m_plc_d[wVal2];
		else if(wVal1==5)
			wValue=wVZRegisterB[wVal2];
		break;
	case 2://KnX、KnY、KnM、KnS
		wVal1=*(pIL++);
		wVal2=*(pIL++);
		wVal3=*(pIL++);
		if(wVal1==1)
		{
			for(i=wVal2*4+wVal3;i>wVal3;i--)
			{
				wValue=wValue<<1;
				wi=i-1;
				wValue|=(WORD)((dwInputXB>>wi)&0x01);
			}
		}
		else if(wVal1==2)
		{
			for(i=wVal2*4+wVal3;i>wVal3;i--)
			{
				wValue=wValue<<1;
				wi=i-1;
				wValue|=(WORD)((dwOutputYB>>wi)&0x01);
			}
		}
		else if(wVal1==3)
		{
			for(i=wVal2*4+wVal3;i>wVal3;i--)
			{
				wi=i-1;
				wValue=wValue<<1;
				wNum=wi/32;wPos=wi%32;
				wValue|=((dwMRegisterB[wNum]>>wPos)&0x01);
			}
		}
		else if(wVal1==4)
		{
			for(i=wVal2*4+wVal3;i>wVal3;i--)
			{
				wi=i-1;
				wValue=wValue<<1;
				wNum=wi/32;wPos=wi%32;
				wValue|=((dwSRegister[wNum]>>wPos)&0x01);
			}
		}
		break;
	default:
		break;
	}

	return wValue;
}

void InitAddressTable()
{
	WORD i,*pIlAddTab;
	pIlAddTab=(WORD*)(TABLEADDRESS);
	for(i=0;i<300;i++)
	{
		AddressTable[i].wSub=*(pIlAddTab++);
		AddressTable[i].wAddressIn=*(pIlAddTab++);
		AddressTable[i].wAddressOut=*(pIlAddTab++);
	}
}

void  ILJmp(BOOL bJmp)
{
	WORD  wVal1,wVal2,wVal3,wVal4;
	DWORD dAddFalse,dAddOut;
	if(bJmp)
	{
		pIL+=2;
	}
	else
	{
		wVal1=*(pIL++);wVal2=*(pIL++);
		pIL+=2;
		wVal3=*(pIL++);wVal4=*(pIL++);
		dAddFalse=(DWORD)((wVal2<<16)|wVal1);
		dAddOut=(DWORD)((wVal4<<16)|wVal3);
		CloseOut(dAddFalse,dAddOut);
		pIL-=6;
	}

	wVal1=*(pIL++);wVal2=*(pIL++);
	pIL=(WORD *)(((DWORD)ILADDRESS)+((DWORD)((wVal2<<16)|wVal1)));
}

void  CloseOut(DWORD dAddFalse,DWORD dAddOut)
{
	WORD *pILOut,wCommand,wVal1,wVal2;
	if((dAddOut!=0)&&(dAddOut<dAddFalse))
	{
		pILOut=(WORD *)(((DWORD)ILADDRESS)+dAddOut);
		wCommand=*(pILOut++);
		switch(wCommand)
		{
		case 131:
			Cmd1(pILOut);
			pILOut+=5;
			break;
		case 132:
			Cmd2(pILOut);
			pILOut+=5;
			break;
		case 133:
			Cmd3(pILOut);
			pILOut+=7;
			break;
		case 134:
			Cmd4(pILOut);
			pILOut+=7;
			break;
		case 135:
			Cmd5(pILOut);
			pILOut+=5;
			break;
		default:
			break;
		}
		wVal1=*(pILOut++);wVal2=*(pILOut++);
		dAddOut=(DWORD)((wVal2<<16)|wVal1);
		CloseOut(dAddFalse,dAddOut);
	}
}

void Command8()
{
	WORD wVal5,wVal6,wValue1,wValue2,wNum,wPos;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal5=*(pIL++);
	wVal6=*(pIL++);

	switch(wVal5)
	{
	case 1:
		SetOut(wVal6,FALSE);
		wVal6++;
		SetOut(wVal6,FALSE);
		wVal6++;
		SetOut(wVal6,FALSE);
		break;
	case 2:
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal6++;
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal6++;
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal6++;
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal6++;
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal6-=2;

	if(wValue1==wValue2)
		wVal6+=1;
	else if(wValue1<wValue2)
		wVal6+=2;

	switch(wVal5)
	{
	case 1:
		SetOut(wVal6,TRUE);
		break;
	case 2:
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal6/32;
		wPos=wVal6%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command9()
{
	WORD wVal6,wVal7;
	WORD wValue1,wValue2,wNum,wPos;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal6=*(pIL++);
	wVal7=*(pIL++);

	switch(wVal6)
	{
	case 1:
		SetOut(wVal7,FALSE);
		wVal7++;
		SetOut(wVal7,FALSE);
		wVal7++;
		SetOut(wVal7,FALSE);
		break;
	case 2:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal7-=2;

	if(wValue1==wValue2)
		wVal7+=1;
	else if(wValue1<wValue2)
		wVal7+=2;

	switch(wVal6)
	{
	case 1:
		SetOut(wVal7,TRUE);
		break;
	case 2:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command10()
{
	WORD wVal6,wVal7;
	WORD wValue1,wValue2,wNum,wPos;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal6=*(pIL++);
	wVal7=*(pIL++);
	switch(wVal6)
	{
	case 1:
		SetOut(wVal7,FALSE);
		wVal7++;
		SetOut(wVal7,FALSE);
		wVal7++;
		SetOut(wVal7,FALSE);
		break;
	case 2:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal7++;
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal7-=2;
	if(wValue1==wValue2)
		wVal7+=1;
	else if(wValue1<wValue2)
		wVal7+=2;

	switch(wVal6)
	{
	case 1:
		SetOut(wVal7,TRUE);
		break;
	case 2:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal7/32;
		wPos=wVal7%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command11()
{
	WORD wVal7,wVal8;
	WORD wValue1,wValue2,wNum,wPos;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal7=*(pIL++);
	wVal8=*(pIL++);

	switch(wVal7)
	{
	case 1:
		SetOut(wVal8,FALSE);
		wVal8++;
		SetOut(wVal8,FALSE);
		wVal8++;
		SetOut(wVal8,FALSE);
		break;
	case 2:
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal8++;
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal8++;
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal8++;
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal8++;
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal8-=2;

	if(wValue1==wValue2)
		wVal8+=1;
	else if(wValue1<wValue2)
		wVal8+=2;

	switch(wVal7)
	{
	case 1:
		SetOut(wVal8,TRUE);
		break;
	case 2:
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal8/32;
		wPos=wVal8%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command12()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);
	wValue=AnaValue(1);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command13()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);
	wValue=AnaValue(1);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command14()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);
	wValue=AnaValue(1);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command15()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);
	wValue=AnaValue(2);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command16()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);
	wValue=AnaValue(2);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command17()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);
	wValue=AnaValue(2);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command18()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;


	wValue1=AnaValue(2);
	wValue2=AnaValue(2);
	wValue=AnaValue(1);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command19()
{
	WORD wVal1,wVal2;
	WORD wValue1,wValue2,wValue,wNum,wPos;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);
	wValue=AnaValue(2);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		wVal2++;
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		wVal2++;
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	default:
		break;
	}
	wVal2-=2;

	if(wValue>=wValue1&&wValue<=wValue2)
		wVal2+=1;
	else if(wValue>wValue2)
		wVal2+=2;

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;
		wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command20()
{
	WORD wValue,wVal1,wVal2;
	wValue=AnaValue(1);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command21()
{
	WORD wValue,wVal1,wVal2;
	wValue=AnaValue(2);

	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command22()
{
	WORD i;
	WORD wValue,wVal1,wVal2,wVal3;
	WORD wNum,wPos;
	wValue=AnaValue(1);

	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);

	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal2*4+wVal3;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal2*4+wVal3;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]&=~(1<<wPos);
			dwMRegisterB[wNum]|=(((wValue>>(i-wVal3))&0x01)<<wPos);
		}
		break;
	case 3:
		for(i=wVal3;i<wVal2*4+wVal3;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]&=~(1<<wPos);
			dwSRegisterB[wNum]|=(((wValue>>(i-wVal3))&0x01)<<wPos);
		}
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command23()
{
	WORD i;
	WORD wValue,wVal1,wVal2,wVal3;
	WORD wNum,wPos;
	//m_plc_d[DATEMP_REAL0]=2;
	wValue=AnaValue(2);

	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);

	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal2*4+wVal3;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal2*4+wVal3;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]&=~(1<<wPos);
			dwMRegisterB[wNum]|=(((wValue>>(i-wVal3))&0x01)<<wPos);
		}
		break;
	case 3:
		for(i=wVal3;i<wVal2*4+wVal3;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]&=~(1<<wPos);
			dwSRegisterB[wNum]|=(((wValue>>(i-wVal3))&0x01)<<wPos);
		}
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command24()
{
	WORD wValue1,wValue2,wVal1,wVal2,wVal3,wVal4;
	WORD wNum,wPos;

	wValue1=AnaValue(1);
	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);
	wVal4=*(pIL++);

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	if((wValue1>>wValue2)&0x01)
	{
		switch(wVal1)
		{
		case 1:
			SetOut(wVal2,TRUE);
			break;
		case 2:
			wNum=wVal2/32;wPos=wVal2%32;
			dwMRegisterB[wNum]|=(1<<wPos);
			break;
		case 3:
			wNum=wVal2/32;wPos=wVal2%32;
			dwSRegisterB[wNum]|=(1<<wPos);
			break;
		default:
			break;
		}
	}
	else
	{
		switch(wVal1)
		{
		case 1:
			SetOut(wVal2,FALSE);
			break;
		case 2:
			wNum=wVal2/32;wPos=wVal2%32;
			dwMRegisterB[wNum]&=~(1<<wPos);
			break;
		case 3:
			wNum=wVal2/32;wPos=wVal2%32;
			dwSRegisterB[wNum]&=~(1<<wPos);
			break;
		default:
			break;
		}
	}

	ILJmp(TRUE);
}

void Command25()
{
	WORD wValue1,wValue2,wVal1,wVal2,wVal3,wVal4;
	WORD wNum,wPos;

	wValue1=AnaValue(2);
	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);
	wVal4=*(pIL++);

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];
	//m_plc_d[DATEMP_REAL0]=wValue1;
	//m_plc_d[DATEMP_REAL1]=wValue2;
	if((wValue1>>wValue2)&0x01)
	{
		switch(wVal1)
		{
		case 1:
			SetOut(wVal2,TRUE);
			break;
		case 2:
			wNum=wVal2/32;wPos=wVal2%32;
			dwMRegisterB[wNum]|=(1<<wPos);
			break;
		case 3:
			wNum=wVal2/32;wPos=wVal2%32;
			dwSRegisterB[wNum]|=(1<<wPos);
			break;
		default:
			break;
		}
	}
	else
	{
		switch(wVal1)
		{
		case 1:
			SetOut(wVal2,FALSE);
			break;
		case 2:
			wNum=wVal2/32;wPos=wVal2%32;
			dwMRegisterB[wNum]&=~(1<<wPos);
			break;
		case 3:
			wNum=wVal2/32;wPos=wVal2%32;
			dwSRegisterB[wNum]&=~(1<<wPos);
			break;
		default:
			break;
		}
	}

	ILJmp(TRUE);
}


void Command30()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command31()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}

	ILJmp(TRUE);
}

void Command32()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command33()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command34()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command35()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command36()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command37()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1+wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command38()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command39()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command40()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command41()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command42()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command43()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command44()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command45()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1-wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command46()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wCurTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command47()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command48()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command49()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command50()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=(WORD)(Timer[wVal2].wBeginTime+wValue);
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command51()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			if(i<32)
				dwInputXB|=(((wValue>>(i-wVal3))&0x01)<<i);
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command52()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command53()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1*wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command54()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wCurTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command55()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command56()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command57()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command58()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command59()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command60()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command61()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1/wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command62()
{
	WORD wVal1,wVal2;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		m_plc_d[wVal2]=abs(m_plc_d[wVal2]);
	else
		wVZRegisterB[wVal2]=abs(wVZRegisterB[wVal2]);

	ILJmp(TRUE);
}

void Command63()
{
	DWORD dValue;
	WORD wVal1,wVal2;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
	{
		dValue=(m_plc_d[wVal2+1]<<16)|m_plc_d[wVal2];
		dValue=abs(dValue);
		m_plc_d[wVal2]=dValue;
		m_plc_d[wVal2+1]=dValue>>16;
	}
	else
	{
		dValue=(wVZRegister[wVal2+1]<<16)|wVZRegister[wVal2];
		dValue=abs(dValue);
		wVZRegisterB[wVal2]=dValue;
		wVZRegisterB[wVal2+1]=dValue>>16;
	}
	ILJmp(TRUE);
}

void Command64()
{
	WORD wValue;
	WORD wVal1,wVal2;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
	{
		wValue=m_plc_d[wVal2];
		m_plc_d[wVal2]=(WORD)(((wValue<<8)&0xff00)|((wValue>>8)&0xff));
	}
	else if(wVal1==2)
	{
		wValue=wVZRegisterB[wVal2];
		wVZRegisterB[wVal2-4096]=(WORD)(((wValue<<8)&0xff00)|((wValue>>8)&0xff));
	}
	ILJmp(TRUE);
}

void Command65()
{
	WORD i,wValue;
	WORD wVal1,wVal2;
	wVal1=*(pIL++);
	wVal2=*(pIL++);

	wValue=0;
	for(i=0;i<16;i++)
	{
		if((m_plc_d[wVal1]>>i)&0x01)
			wValue++;
	}

	m_plc_d[wVal2]=wValue;
	ILJmp(TRUE);
}

void Command66()
{
	WORD i,wNum,wPos;
	WORD wVal1,wVal2,wVal3;
	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);

	switch(wVal1)
	{
	case 1:
		for(i=wVal2;i<wVal3;i++)
		{
			Timer[i].wCurTime=Timer[i].wBeginTime;
		}
		break;
	case 2:
		for(i=wVal2;i<wVal3;i++)
		{
			Counter[i].wCurCount=0;
		}
		break;
	case 3:
		for(i=wVal2;i<wVal3;i++)
		{
			m_plc_d[i]=0;
		}
		break;
	case 4:
		for(i=wVal2;i<wVal3;i++)
		{
			SetOut(i,FALSE);
		}
		break;
	case 5:
		for(i=wVal2;i<wVal3;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]&=~(0x01<<wPos);
		}
		break;
	case 6:
		for(i=wVal2;i<wVal3;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]&=~(0x01<<wPos);
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command67()
{
	WORD wVal1,wVal2;
	pIL++;
	wVal1=*(pIL++);
	wVal2=*(pIL++);

	pIL=(WORD *)(((DWORD)ILADDRESS)+((DWORD)((wVal2<<16)|wVal1)));
}

void Command68()
{
	WORD wValue;
	wValue=AnaValue(1);

	Forer[wForPos].pIL=pIL+6;
	Forer[wForPos].wCount=wValue+1;
	wForPos++;
	ILJmp(TRUE);
}

void Command69()
{
	WORD wValue;
	wValue=AnaValue(2);

	Forer[wForPos].pIL=pIL+6;
	Forer[wForPos].wCount=wValue+1;
	wForPos++;

	ILJmp(TRUE);
}

void Command70()
{
	/*WORD wVal1,wVal2;
	WORD *pIL1;
	wVal1=*(pIL++);
	wVal2=*(pIL++);*/

	Forer[wForPos - 1].wCount--;
	if(Forer[wForPos - 1].wCount<2)
	{
		wForPos--;
		ILJmp(TRUE);
	}
	else
	{
		pIL=Forer[wForPos-1].pIL;
	}
}

void Command71()
{
	WORD wVal1,wVal2,wNum,wPos;
	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;wPos=wVal2%32;
		dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;wPos=wVal2%32;
		dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command72()
{
	WORD wVal1,wVal2,wNum,wPos;
	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		SetOut(wVal2,FALSE);
		break;
	case 2:
		wNum=wVal2/32;wPos=wVal2%32;
		dwMRegisterB[wNum]&=~(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;wPos=wVal2%32;
		dwSRegisterB[wNum]&=~(1<<wPos);
		break;
	case 4:
		Counter[wVal2].wCurCount=0;
		Counter[wVal2].bCountRun=FALSE;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command73()
{
	WORD wVal1,wVal2,wNum,wPos;
	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		if((dwOutputYB>>wVal2)&0x01)
			SetOut(wVal2,FALSE);
		else
			SetOut(wVal2,TRUE);
		break;
	case 2:
		wNum=wVal2/32;wPos=wVal2%32;
		if((dwMRegister[wNum]>>wPos)&0x01)
			dwMRegisterB[wNum]&=~(1<<wPos);
		else
			dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;wPos=wVal2%32;
		if((dwSRegister[wNum]>>wPos)&0x01)
			dwSRegisterB[wNum]&=~(1<<wPos);
		else
			dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command76()
{
	WORD wVal0,wVal1,wVal2;
	wVal0=*(pIL++);
	wVal1=*(pIL++);
	wVal2=*(pIL++);

	if(wCallerPos > 20)
	{
		db_printf("call error\r\n");
	}
	
	Caller[wCallerPos].pIL=pIL;
	wCallerPos++;
	
	pIL=(WORD *)(((DWORD)ILADDRESS)+((DWORD)((wVal2<<16)|wVal1)));
}

void Command77()
{

	wCallerPos--;
	pIL=Caller[wCallerPos].pIL;
	ILJmp(TRUE);
}

void Command78()
{
	WORD wVal1,wVal2,wVal3,wVal4;
	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);
	wVal4=*(pIL++);
	ILJmp(TRUE);
}

void Command79()
{
	WORD wVal1,wVal2,wVal3,wValue;
	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);

	switch(wVal1)
	{
	case 1:
		wValue=Timer[wVal2].wCurTime;
		wValue=(wValue>>wVal3)|(wValue<<(16-wVal3));
		Timer[wVal2].wCurTime=wValue;
		break;
	case 2:
		wValue=Counter[wVal2].wCurCount;
		wValue=(wValue>>wVal3)|(wValue<<(16-wVal3));
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		wValue=m_plc_d[wVal2];
		wValue=(wValue>>wVal3)|(wValue<<(16-wVal3));
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wValue=wVZRegisterB[wVal2];
		wValue=(wValue>>wVal3)|(wValue<<(16-wVal3));
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command80()
{
	
}

void Command81()
{
	WORD wVal1,wVal2,wVal3,wValue;
	wVal1=*(pIL++);
	wVal2=*(pIL++);
	wVal3=*(pIL++);

	switch(wVal1)
	{
	case 1:
		wValue=Timer[wVal2].wCurTime;
		wValue=(wValue<<wVal3)|(wValue>>(16-wVal3));
		Timer[wVal2].wCurTime=wValue;
		break;
	case 2:
		wValue=Counter[wVal2].wCurCount;
		wValue=(wValue<<wVal3)|(wValue>>(16-wVal3));
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		wValue=m_plc_d[wVal2];
		wValue=(wValue<<wVal3)|(wValue>>(16-wVal3));
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wValue=wVZRegisterB[wVal2];
		wValue=(wValue<<wVal3)|(wValue>>(16-wVal3));
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command82()
{
	
}

void Command83()
{
	WORD wVal1,wVal2,wNum,wPos;
	wVal1=*(pIL++);
	wVal2=*(pIL++);

	switch(wVal1)
	{
	case 1:
		if(wVal2<16)
		{
			if((dwOutputYB>>wVal2)&0x01)
				SetOut(wVal2,FALSE);
			else
				SetOut(wVal2,TRUE);
		}
		break;
	case 2:
		wNum=wVal2/32;wPos=wVal2%32;
		if((dwMRegister[wNum]>>wPos)&0x01)
			dwMRegisterB[wNum]&=~(1<<wPos);
		else
			dwMRegisterB[wNum]|=(1<<wPos);
		break;
	case 3:
		wNum=wVal2/32;wPos=wVal2%32;
		if((dwSRegister[wNum]>>wPos)&0x01)
			dwSRegisterB[wNum]&=~(1<<wPos);
		else
			dwSRegisterB[wNum]|=(1<<wPos);
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command84()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command85()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command86()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command87()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command88()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command89()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command90()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command91()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1&wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command92()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command93()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command94()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command95()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command96()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command97()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command98()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command99()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1|wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command100()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command101()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command102()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command103()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command104()
{
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		Timer[wVal2].wCurTime=Timer[wVal2].wBeginTime+wValue;
		break;
	case 2:
		Counter[wVal2].wCurCount=wValue;
		break;
	case 3:
		m_plc_d[wVal2]=wValue;
		break;
	case 4:
		wVZRegisterB[wVal2]=wValue;
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command105()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(1);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command106()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(1);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command107()
{
	WORD i,wNum,wPos;
	WORD wValue1,wValue2,wValue;
	WORD wVal1,wVal2,wVal3;

	wValue1=AnaValue(2);
	wValue2=AnaValue(2);

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);

	wValue=wValue1^wValue2;
	switch(wVal1)
	{
	case 1:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			SetOut(i,((wValue>>(i-wVal3))&0x01));
		}
		break;
	case 2:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwMRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	case 3:
		for(i=wVal3;i<wVal3+wVal2*4;i++)
		{
			wNum=i/32;wPos=i%32;
			dwSRegisterB[wNum]|=((wValue>>(i-wVal3))&0x01)<<wPos;
		}
		break;
	default:
		break;
	}
	ILJmp(TRUE);
}

void Command108()
{
	WORD wVal1,wVal2,wVal3,wVal4,wVal5,wVal6,wVal7,wVal8,wVal9,wVal10;

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);
	wVal4=*(pIL++);wVal5=*(pIL++);
	wVal6=*(pIL++);wVal7=*(pIL++);wVal8=*(pIL++);
	wVal9=*(pIL++);wVal10=*(pIL++);

	if(wVal1==2)
		wVal2=m_plc_d[wVal2];

	if(wVal3==2)
		wVal4=m_plc_d[wVal4];

	if(wVal5==2)
		wVal6=m_plc_d[wVal6];

	if(wVal7==2)
		wVal8=m_plc_d[wVal8];

	if(wVal9==2)
		wVal10=m_plc_d[wVal10];

	//hydroutramp(wVal2,wVal4,wVal6,wVal8,wVal10);
	ILJmp(TRUE);
}

void Command109()
{
	WORD wVal1,wVal2,wVal3,wVal4;

	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==2)
		wVal2=m_plc_d[wVal2];
	if(wVal3==2)
		wVal4=m_plc_d[wVal4];

	SetPOBitDelay(wVal2,1,wVal4);
	ILJmp(TRUE);
}

void Command110()
{
	WORD wVal1,wVal2,wVal3,wVal4;

	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==2)
		wVal2=m_plc_d[wVal2];
	if(wVal3==2)
		wVal4=m_plc_d[wVal4];

	SetPOBitDelay(wVal2,0,wVal4);
	ILJmp(TRUE);
}

void Command111()
{
	ILJmp(TRUE);
}

void Command112()
{
	ILJmp(TRUE);
}

void Command113()
{
	ILJmp(TRUE);
}

void Command114()
{
	ILJmp(TRUE);
}

void Command115()
{
	ILJmp(TRUE);
}

void Command116()
{
	ILJmp(TRUE);
}

void Command117()
{
	ILJmp(TRUE);
}

void Command118()
{
	WORD wVal1,wVal2,wVal3,wVal4,wVal5,wVal6;
	WORD i;

	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);
	wVal4=*(pIL++);wVal5=*(pIL++);wVal6=*(pIL++);

	for(i=0;i<wVal6;i++)
	{
		m_plc_d[wVal4+i]=m_plc_d[wVal2+i];
	}
	ILJmp(TRUE);
}

//void Command119()
//{
//	WORD wVal1,wVal2,wVal3,wVal4,wVal5;
//	WORD wValue1,wValue2;
//
//	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);
//	wVal4=*(pIL++);wVal5=*(pIL++);
//
//	if(!bRight[0])
//		return;
//
//
//	if(wVal4==1)
//		wValue2=wVal5;
//	else
//		wValue2=m_plc_d[wVal4];
//
//	if(wVal1==1)
//		wValue1=wVal2+wValue2;
//	else
//	{
//		wVal2=wVal2+wValue2;
//		wValue1=m_plc_d[wVal2];
//	}
//
//
//	m_plc_d[wVal3]=wValue1;
//}

void Command119()
{
	WORD wValue1;BOOL bJmp;
	wValue1=*(pIL++);
	bJmp=(BOOL)((dwInputXB>>wValue1)&0x01);

	ILJmp(bJmp);
}

void  Command120()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	bGet=(BOOL)((dwOutputYB>>wValue1)&0x01);
	ILJmp(bGet);
}

void  Command121()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	bGet=(BOOL)((dwMRegisterB[wNum]>>wPos)&0x01);
	ILJmp(bGet);
}

void  Command122()
{
	WORD wValue1,wCurTime;BOOL bGet;
	wValue1=*(pIL++);

	if(wValue1<200)
		wCurTime=A_1msclock;
	else if(wValue1>199&&wValue1<400)
		wCurTime=A_10msclock;
	else
		wCurTime=A_100msclock;

	bGet=FALSE;
	if(Timer[wValue1].bTimeRun)
	{
		if(((WORD)(wCurTime-Timer[wValue1].wBeginTime))>Timer[wValue1].wTimeSpan)
			bGet=TRUE;
	}
	
	ILJmp(bGet);
}

void  Command123()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	bGet=FALSE;
	if(Counter[wValue1].bCountRun)
	{
		if(Counter[wValue1].wCurCount>=Counter[wValue1].wSumCount)
			bGet=TRUE;
	}
	ILJmp(bGet);
}

void  Command124()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	bGet=(BOOL)((dwSRegisterB[wNum]>>wPos)&0x01);
	ILJmp(bGet);
}

void Command125()
{
	WORD wValue1;BOOL bGet,bJmp;
	wValue1=*(pIL++);
	bGet=(BOOL)((dwInputXB>>wValue1)&0x01);
	if(bGet)
		bJmp=FALSE;
	else
		bJmp=TRUE;

	ILJmp(bJmp);
}

void  Command126()
{
	WORD wValue1;BOOL bGet,bJmp;
	wValue1=*(pIL++);
	bGet=(BOOL)((dwOutputYB>>wValue1)&0x01);
	if(bGet)
		bJmp=FALSE;
	else
		bJmp=TRUE;

	ILJmp(bJmp);
}

void  Command127()
{
	WORD wValue1,wNum,wPos;BOOL bGet,bJmp;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	bGet=(BOOL)((dwMRegisterB[wNum]>>wPos)&0x01);
	if(bGet)
		bJmp=FALSE;
	else
		bJmp=TRUE;
	ILJmp(bJmp);
}

void  Command128()
{
	WORD wValue1,wCurTime;BOOL bGet,bJmp;
	wValue1=*(pIL++);

	if(wValue1<200)
		wCurTime=A_1msclock;
	else if(wValue1>199&&wValue1<400)
		wCurTime=A_10msclock;
	else
		wCurTime=A_100msclock;

	bGet=FALSE;
	if((((WORD)(wCurTime-Timer[wValue1].wBeginTime))<Timer[wValue1].wTimeSpan)
		||!Timer[wValue1].bTimeRun)
		bGet=TRUE;

	if(bGet)
		bJmp=FALSE;
	else
		bJmp=TRUE;
	ILJmp(bJmp);
}

void  Command129()
{
	WORD wValue1;BOOL bGet,bJmp;
	wValue1=*(pIL++);
	bGet=FALSE;
	if(Counter[wValue1].wCurCount>=Counter[wValue1].wSumCount)
		bGet=TRUE;
	if(bGet)
		bJmp=FALSE;
	else
		bJmp=TRUE;
	ILJmp(bJmp);
}

void  Command130()
{
	WORD wValue1,wNum,wPos;BOOL bGet,bJmp;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	bGet=(BOOL)((dwSRegisterB[wNum]>>wPos)&0x01);
	if(bGet)
		bJmp=FALSE;
	else
		bJmp=TRUE;
	ILJmp(bJmp);
}

void  Command131()
{
	WORD wValue1;
	wValue1=*(pIL++);
	dwOutputYB|=(DWORD)(1<<wValue1);
	SetOut(wValue1,TRUE);

	ILJmp(TRUE);
}

void  Command132()
{
	WORD wValue1,wNum,wPos;

	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	dwMRegisterB[wNum]|=(DWORD)(1<<wPos);
	ILJmp(TRUE);
}

void  Command133()
{
	WORD wValue1,wValue2,wValue3,wValue;

	wValue1=*(pIL++);wValue2=*(pIL++);wValue3=*(pIL++);
	if(wValue2==1)
		wValue=wValue3;
	else if(wValue2==2)
		wValue=m_plc_d[wValue3];
	
	if(!Timer[wValue1].bTimeRun)
	{
		Timer[wValue1].bTimeRun=TRUE;
		if(wValue1<200)
			Timer[wValue1].wBeginTime=A_1msclock;
		else if(wValue1>199&&wValue1<400)
			Timer[wValue1].wBeginTime=A_10msclock;
		else if(wValue1>399&&wValue1<600)
			Timer[wValue1].wBeginTime=A_100msclock;

		Timer[wValue1].wTimeSpan=wValue;
	}

	ILJmp(TRUE);
}

void  Command134()
{
	WORD wValue1,wValue2,wValue3,wValue;

	wValue1=*(pIL++);wValue2=*(pIL++);wValue3=*(pIL++);
	if(wValue2==1)
		wValue=wValue3;
	else if(wValue2==2)
		wValue=m_plc_d[wValue3];
	if(bRight)
	{
		if(Counter[wValue1].bCountRun)
		{
			Counter[wValue1].wCurCount++;
		}
		else
		{
			Counter[wValue1].bCountRun=TRUE;
			Counter[wValue1].wSumCount=wValue;
			Counter[wValue1].wCurCount=0;
		}
	}

	ILJmp(TRUE);
}

void  Command135()
{
	WORD wValue1,wNum,wPos;

	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	dwSRegisterB[wNum]|=(DWORD)(1<<wPos);

	ILJmp(TRUE);
}

void  Command136()
{
	WORD wValue1;BOOL bJmp;
	wValue1=*(pIL++);
	bJmp=(BOOL)((dwInputXB>>wValue1)&0x01);

	ILJmp(bJmp);
}

void  Command137()
{
	WORD wValue1;BOOL bJmp;
	wValue1=*(pIL++);
	bJmp=(BOOL)((dwOutputYB>>wValue1)&0x01);

	ILJmp(bJmp);
}

void  Command138()
{
	WORD wValue1,wNum,wPos;BOOL bJmp;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	bJmp=(BOOL)((dwMRegisterB[wNum]>>wPos)&0x01);
	
	ILJmp(bJmp);
}

void  Command139()
{
	WORD wValue1,wCurTime;BOOL bGet;
	wValue1=*(pIL++);

	if(wValue1<200)
		wCurTime=A_1msclock;
	else if(wValue1>199&&wValue1<400)
		wCurTime=A_10msclock;
	else
		wCurTime=A_100msclock;

	bGet=FALSE;
	if(Timer[wValue1].bTimeRun)
	{
		if(((WORD)(wCurTime-Timer[wValue1].wBeginTime))>Timer[wValue1].wTimeSpan)
			bGet=TRUE;
	}

	ILJmp(bGet);
}

void  Command140()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	bGet=FALSE;
	if(Counter[wValue1].bCountRun)
	{
		if(Counter[wValue1].wCurCount>=Counter[wValue1].wSumCount)
			bGet=TRUE;
	}

	ILJmp(bGet);
}

void  Command141()
{
	WORD wValue1,wNum,wPos;BOOL bJmp;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	bJmp=(BOOL)((dwSRegisterB[wNum]>>wPos)&0x01);
	
	ILJmp(bJmp);
}

void  Command142()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	if(((dwInputXB>>wValue1)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;
	
	ILJmp(bGet);
}

void  Command143()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	if(((dwOutputYB>>wValue1)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;
	
	ILJmp(bGet);
}

void  Command144()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	if(((dwMRegisterB[wNum]>>wPos)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;
	
	ILJmp(bGet);
}

void  Command145()
{
	WORD wValue1,wCurTime;BOOL bGet;
	wValue1=*(pIL++);

	if(wValue1<200)
		wCurTime=A_1msclock;
	else if(wValue1>199&&wValue1<400)
		wCurTime=A_10msclock;
	else
		wCurTime=A_100msclock;

	bGet=TRUE;
	if(Timer[wValue1].bTimeRun)
	{
		if(((WORD)(wCurTime-Timer[wValue1].wBeginTime))>Timer[wValue1].wTimeSpan)
			bGet=FALSE;
	}

	ILJmp(bGet);
}

void  Command146()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	bGet=TRUE;
	if(Counter[wValue1].bCountRun)
	{
		if(Counter[wValue1].wCurCount>=Counter[wValue1].wSumCount)
			bGet=FALSE;
	}

	ILJmp(bGet);
}

void  Command147()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	if(((dwSRegisterB[wNum]>>wPos)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;
	
	ILJmp(bGet);
}

void  Command148()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);

	bGet=(BOOL)((dwInputXB>>wValue1)&0x01);

	ILJmp(bGet);
}

void  Command149()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	bGet=(BOOL)((dwOutputYB>>wValue1)&0x01);
	ILJmp(bGet);
}

void  Command150()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	bGet=(BOOL)((dwMRegisterB[wNum]>>wPos)&0x01);

	ILJmp(bGet);
}

void  Command151()
{
	WORD wValue1,wCurTime;BOOL bGet;
	wValue1=*(pIL++);

	if(wValue1<200)
		wCurTime=A_1msclock;
	else if(wValue1>199&&wValue1<400)
		wCurTime=A_10msclock;
	else
		wCurTime=A_100msclock;

	bGet=FALSE;
	if(Timer[wValue1].bTimeRun)
	{
		if(((WORD)(wCurTime-Timer[wValue1].wBeginTime))>Timer[wValue1].wTimeSpan)
			bGet=TRUE;
	}

	ILJmp(bGet);
}

void  Command152()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	bGet=FALSE;
	if(Counter[wValue1].bCountRun)
	{
		if(Counter[wValue1].wCurCount>=Counter[wValue1].wSumCount)
			bGet=TRUE;
	}

	ILJmp(bGet);
}

void  Command153()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;

	bGet=(BOOL)((dwSRegisterB[wNum]>>wPos)&0x01);

	ILJmp(bGet);
}


void  Command154()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	if(((dwInputXB>>wValue1)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;

	ILJmp(bGet);
	
}

void  Command155()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	if(((dwOutputYB>>wValue1)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;
	
	ILJmp(bGet);
}

void  Command156()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	if(((dwMRegisterB[wNum]>>wPos)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;
	
	ILJmp(bGet);
}

void  Command157()
{
	WORD wValue1,wCurTime;BOOL bGet;
	wValue1=*(pIL++);

	if(wValue1<200)
		wCurTime=A_1msclock;
	else if(wValue1>199&&wValue1<400)
		wCurTime=A_10msclock;
	else
		wCurTime=A_100msclock;

	bGet=TRUE;
	if(Timer[wValue1].bTimeRun)
	{
		if(((WORD)(wCurTime-Timer[wValue1].wBeginTime))>Timer[wValue1].wTimeSpan)
			bGet=FALSE;
	}

	ILJmp(bGet);
}

void  Command158()
{
	WORD wValue1;BOOL bGet;
	wValue1=*(pIL++);
	bGet=TRUE;
	if(Counter[wValue1].bCountRun)
	{
		if(Counter[wValue1].wCurCount>=Counter[wValue1].wSumCount)
			bGet=FALSE;
	}

	ILJmp(bGet);
}

void  Command159()
{
	WORD wValue1,wNum,wPos;BOOL bGet;
	wValue1=*(pIL++);
	wNum=wValue1/32;wPos=wValue1%32;
	if(((dwSRegisterB[wNum]>>wPos)&0x01))
		bGet=FALSE;
	else
		bGet=TRUE;
	
	ILJmp(bGet);
}

void  Command160()
{
	ILJmp(TRUE);
}

void  Command161()
{
	ILJmp(TRUE);
}

void  Command162()
{
	ILJmp(TRUE);
}

void  Command163()
{
	ILJmp(TRUE);
}

void  Command164()
{
	ILJmp(TRUE);
}

void Command165()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1==wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}

void Command166()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1>wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}

void Command167()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1<wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command168()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1>=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command169()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1<=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command170()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1!=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}

void Command171()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1==wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}

void Command172()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1>wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command173()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1<wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command174()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1>=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command175()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1<=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command176()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1!=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}


void Command177()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1==wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command178()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1>wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command179()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1<wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command180()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1>=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command181()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1<=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}
void Command182()
{
	WORD wVal1,wVal2,wVal3,wVal4,wValue1,wValue2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);
	wVal3=*(pIL++);wVal4=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	if(wVal3==1)
		wValue2=wVal4;
	else
		wValue2=m_plc_d[wVal4];

	bGet=FALSE;
	if(wValue1!=wValue2)
		bGet=TRUE;

	ILJmp(bGet);
}

void Command183()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//bGet=testpb(wValue1);
	ILJmp(bGet);
}

void Command184()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//bGet=testpcon(wValue1);
	ILJmp(bGet);
}

void Command185()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//bGet=checkusedpb(wValue1);
	ILJmp(bGet);
}

void Command186()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//bGet=testpb(wValue1);

	if(bGet)
		ILJmp(FALSE);
	else
		ILJmp(TRUE);
}

void Command187()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//bGet=testpcon(wValue1);

	ILJmp(bGet);
}

void Command188()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//bGet=checkusedpb(wValue1);
	if(bGet)
		ILJmp(FALSE);
	else
		ILJmp(TRUE);
}


void Command189()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(testpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command190()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(testpcon(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command191()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(checkusedpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command192()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(!testpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command193()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(!testpcon(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command194()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(!checkusedpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command195()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(testpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command196()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(testpcon(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command197()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(checkusedpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command198()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(!testpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command199()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(!testpcon(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void Command200()
{
	WORD wVal1,wVal2,wValue1;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	//if(!checkusedpb(wValue1))
	//	bGet=TRUE;
	//else
	//	bGet=FALSE;

	ILJmp(bGet);
}

void  Command201()
{
	pIL++;
	ILJmp(TRUE);
}

void  Command202()
{
	//warmproc();
	ILJmp(TRUE);
}

void  Command203()
{
	//manualinit();
	ILJmp(TRUE);
}

void Command204()
{
	WORD wVal1,wVal2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	bGet=FALSE;
	switch(wVal1)
	{
	case 1:
		if((((dwInputXB>>wVal2)&0x01)==1)&&((dwInputX>>wVal2)&0x01)==0)
				bGet=TRUE;
		break;
	case 2:
		if((((dwOutputYB>>wVal2)&0x01)==1)&&((dwOutputY>>wVal2)&0x01)==0)
			bGet=TRUE;
		break;
	default:
		break;
	}

	ILJmp(bGet);
}

void Command205()
{
	WORD wVal1,wVal2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	bGet=FALSE;
	switch(wVal1)
	{
	case 1:
		if((((dwInputXB>>wVal2)&0x01)==0)&&((dwInputX>>wVal2)&0x01)==1)
			bGet=TRUE;
		break;
	case 2:
		if((((dwOutputYB>>wVal2)&0x01)==0)&&((dwOutputY>>wVal2)&0x01)==1)
			bGet=TRUE;
		break;
	default:
		break;
	}

	ILJmp(bGet);
}

void Command206()
{
	WORD wVal1,wVal2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	bGet=FALSE;
	switch(wVal1)
	{
	case 1:
		if((((dwInputXB>>wVal2)&0x01)==1)&&((dwInputX>>wVal2)&0x01)==0)
			bGet=TRUE;
		break;
	case 2:
		if((((dwOutputYB>>wVal2)&0x01)==1)&&((dwOutputY>>wVal2)&0x01)==0)
			bGet=TRUE;
		break;
	default:
		break;
	}

	ILJmp(bGet);
}

void Command207()
{
	WORD wVal1,wVal2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	bGet=FALSE;
	switch(wVal1)
	{
	case 1:
		if((((dwInputXB>>wVal2)&0x01)==0)&&((dwInputX>>wVal2)&0x01)==1)
			bGet=TRUE;
		break;
	case 2:
		if((((dwOutputYB>>wVal2)&0x01)==0)&&((dwOutputY>>wVal2)&0x01)==1)
			bGet=TRUE;
		break;
	default:
		break;
	}

	ILJmp(bGet);
}

void Command208()
{
	WORD wVal1,wVal2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	bGet=FALSE;
	switch(wVal1)
	{
	case 1:
		if((((dwInputXB>>wVal2)&0x01)==1)&&((dwInputX>>wVal2)&0x01)==0)
			bGet=TRUE;
		break;
	case 2:
		if((((dwOutputYB>>wVal2)&0x01)==1)&&((dwOutputY>>wVal2)&0x01)==0)
			bGet=TRUE;
		break;
	default:
		break;
	}

	ILJmp(bGet);
}

void Command209()
{
	WORD wVal1,wVal2;
	BOOL bGet;
	wVal1=*(pIL++);wVal2=*(pIL++);

	bGet=FALSE;
	switch(wVal1)
	{
	case 1:
		if((((dwInputXB>>wVal2)&0x01)==0)&&((dwInputX>>wVal2)&0x01)==1)
			bGet=TRUE;
		break;
	case 2:
		if((((dwOutputYB>>wVal2)&0x01)==0)&&((dwOutputY>>wVal2)&0x01)==1)
			bGet=TRUE;
		break;
	default:
		break;
	}

	ILJmp(bGet);
}

void Command210()
{
	if(bRight)
		ILJmp(FALSE);
	else
		ILJmp(TRUE);
}

void  Command211(void)
{
#if 0	
	WORD wValue1,wValue2,wValue3,wValue4,wValue5,wValue6,wValue7,wValue8;
	WORD wVal;

	wValue1=AnaValue(1);
	wValue2=AnaValue(1);
	wValue3=AnaValue(1);
	wValue4=AnaValue(1);
	wValue5=AnaValue(1);
	wValue6=AnaValue(1);
	wValue7=AnaValue(1);
	wValue8=AnaValue(1);
	wVal=*(pIL++);
	FunEx[wVal](wValue1,wValue2,wValue3,wValue4,wValue5,wValue6,wValue7,wValue8);

	ILJmp(TRUE);
#endif	
}

void  Command212(void)
{
	WORD wVal1,wVal2,wVal3,wVal4,wVal5,wVal6;
	WORD wValue1,wValue2;
	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);
	wVal4=*(pIL++);wVal5=*(pIL++);wVal6=*(pIL++);
	
	if(wVal2==1)
	{
		wValue1=wVal3;	
	}
	else if(wVal2==2)
	{
		wValue1=m_plc_d[wVal2];
	}

	if(wVal5==1)
	{
		wValue2=wVal6;	
	}
	else if(wVal5==2)
	{
		wValue2=m_plc_d[wVal6];
	}

	m_plc_d[wValue2+wVal4]=m_plc_d[wVal1+wValue1];

	ILJmp(TRUE);
}

void  Command213(void)
{
	WORD wVal1,wVal2,wVal3,wVal4,wVal5,wVal6,wVal7,wVal8;
	WORD wValue1,wValue2,wValue3,wValue4;
	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);wVal4=*(pIL++);
	wVal5=*(pIL++);wVal6=*(pIL++);wVal7=*(pIL++);wVal8=*(pIL++);
	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];
	if(wVal3==1)
		wValue2=wVal4;
	else if(wVal3==2)
		wValue2=m_plc_d[wVal4];
	if(wVal5==1)
		wValue3=wVal6;
	else if(wVal5==2)
		wValue3=m_plc_d[wVal6];
	if(wVal7==1)
		wValue4=wVal8;
	else if(wVal7==2)
		wValue4=m_plc_d[wVal8];

	PLCHoldAndRunActTsk(wValue1,wValue2,(UI16)wValue3,wValue4);
	ILJmp(TRUE);
}
void  Command214(void)
{
	WORD wVal1,wVal2,wVal3,wVal4,wVal5,wVal6;
	WORD wValue1,wValue2,wValue3,wValue4;
	wVal1=*(pIL++);wVal2=*(pIL++);wVal3=*(pIL++);wVal4=*(pIL++);
	wVal5=*(pIL++);wVal6=*(pIL++);
	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];
	if(wVal3==1)
		wValue2=wVal4;
	else if(wVal3==2)
		wValue2=m_plc_d[wVal4];
	if(wVal5==1)
		wValue3=wVal6;
	else if(wVal5==2)
		wValue3=m_plc_d[wVal6];

	PLCRunActTsk(wValue1,(UI16)wValue2,wValue3);
	ILJmp(TRUE);
}
void  Command215(void)
{
	WORD wVal1,wVal2;
	WORD wValue1;
	wVal1=*(pIL++);wVal2=*(pIL++);
	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	PLCEndActTsk(wValue1);
	ILJmp(TRUE);
}
void  Command216(void)
{
	WORD wVal1,wVal2;
	WORD wValue1;
	wVal1=*(pIL++);wVal2=*(pIL++);
	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	PLCHoldActTsk(wValue1);
	ILJmp(TRUE);
}
void  Command217(void)
{
	WORD wVal1,wVal2;
	WORD wValue1;
	wVal1=*(pIL++);wVal2=*(pIL++);
	if(wVal1==1)
		wValue1=wVal2;
	else if(wVal1==2)
		wValue1=m_plc_d[wVal2];

	PLCReActiveActTsk(wValue1);
	ILJmp(TRUE);
}

void  Cmd1(WORD *pILB)
{
	WORD wValue1;
	wValue1=*(pILB++);
	SetOut(wValue1,FALSE);
}
void  Cmd2(WORD *pILB)
{
	WORD wValue1,wNum,wPos;

	wValue1=*(pILB++);
	wNum=wValue1/32;wPos=wValue1%32;
	dwMRegisterB[wNum]&=(DWORD)(~(1<<wPos));
}
void  Cmd3(WORD *pILB)
{
	WORD wValue1,wValue2,wValue3,wValue;

	wValue1=*(pILB++);wValue2=*(pILB++);wValue3=*(pILB++);
	if(wValue2==1)
		wValue=wValue3;
	else if(wValue2==2)
		wValue=m_plc_d[wValue3];

	Timer[wValue1].bTimeRun=FALSE;
}
void  Cmd4(WORD *pILB)
{
	/*WORD wValue1,wValue2,wValue3,wValue;

	wValue1=*(pILB++);wValue2=*(pILB++);wValue3=*(pILB++);*/
}
void  Cmd5(WORD *pILB)
{
	WORD wValue1,wNum,wPos;

	wValue1=*(pILB++);
	wNum=wValue1/32;wPos=wValue1%32;
		
	dwSRegisterB[wNum]&=(DWORD)(~(1<<wPos));
}

#endif
