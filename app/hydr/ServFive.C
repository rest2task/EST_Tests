/*===========================================================================+
|  Class    :                                      |
|  Task     :                                      |
|----------------------------------------------------------------------------|
|  Compile  :                                     |
|  Link     :                                       |
|  Call     :                                        |
|----------------------------------------------------------------------------|
|  Author   :                                      |
|  Version  :                                       |
|  Creation :                                       |
|  Revision :                                       |
+===========================================================================*/
#include    "common.h"    
#include    "movectrl.h"
#include    "canhead.h"                                  
#include    "configdf.h"
#include    "hydr_dic.h"
#include    "machine.h"
#include    "machapi.h"
#include    "da.h"
#include	"hwcfg.h"
#include	"com.h"
#include 	"cfgdb.h"
#include	"admeter.h"
#include	"alarm.h"
#include    "act.h"
#include    "ServFive.h"
#include    "hydr.h"
#include	"hydrpos.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
MOVE_STRU_5		 p_Work_5[4];
PUMP_STRU_5      g_pump_5;
extern HYDR g_hydrout;
extern	 MACHINE m_machine;
extern ADMETER m_ad;

UI16 GetMaxRPM(WORD ServoNo);

void    InitServo_PISet(WORD portno)      //伺服读写步骤初始化与PI参数发送
{
    PPUMP_STRU_5 pp = &g_pump_5;

  	pp->writestep = 0;	
  	pp->readstep  = 0;
	pp->servocanmode[portno] = PARAPISET;  //发送PI
}

void     SetPressData_5(WORD portno,UI32 pres)
{
	PHYDR     ph = &g_hydrout;
  
	if(portno == 0 || portno > ph->max_hydr_port)
		return;

	g_hydric[portno].Press = pres/ACCURACY;   
}


void    SetFlowData_5(WORD portno,UI32 flow, WORD srvlmt)
{
	PHYDR     ph = &g_hydrout;
	PSERVO ps = &g_hydrservo;
  
  	if(portno == 0 || portno > ph->max_hydr_port)
  		return;

	
 
  	g_hydric[portno].Flow	= flow/(ACCURACY/100);
	
  	if(!ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(XF6_CANSRV))   //仅限伺服5模式下的处理方式。伺服机中对该点有其他处理
  	{
		if(g_hydric[1].Press == 0  && g_hydric[1].Flow == 0)
		{
			if(ChkSysCfg(XF5_PUMP1))
				SetPOBit(ps->po->O_SV_REL,0);
			else
				SetPOBit(ps->po->O_SV_REL,1);
		}
	    else
	    {
			if(ChkSysCfg(XF5_PUMP1))
				SetPOBit(ps->po->O_SV_REL,1);
			else
				SetPOBit(ps->po->O_SV_REL,0);	
		}
  	}

  	g_pump_5.flowlmtServo_5[portno-1] = srvlmt;    //伺服5模式  2014-09-23
}


/*          PDO数据写入，用于伺服5模式时通讯测试等功能 */
void        ServoDataPdoWrite(int Servoid)
{
  int  i;
  PPUMP_STRU_5 pp = &g_pump_5;
  
  g_canservocomm[Servoid].flag_send = true;
  for (i=0;i<3;i++)
  {
    g_canservocomm[Servoid].pdo_tx_data[i] = pp->ServoPDOWrite.wData[i];
  }
}


void WriteStReadCold(WORD portno)
{
    PPUMP_STRU_5 pp = &g_pump_5;
	
  	pp->ServoPDOWrite.wData[0]=0x03;      //读命令
  	pp->ServoPDOWrite.wData[1]=0x3000;    //读取功耗32的高16位
  	pp->cantestmode  =  2;
  	ServoDataPdoWrite(portno);			
}

void        ServoDataPdoWriteMode5(int Servoid)
{
  int  i;
  PPUMP_STRU_5 pp = &g_pump_5;
  
  g_canservocomm[Servoid].flag_send = true;
  for (i=0;i<4;i++)
  {
    g_canservocomm[Servoid].pdo_tx_data[i] = pp->ServoPDOWrite.wData[i];
  }
}

/*          PDO数据读取，Servoid 伺服驱动器编号        */
void        ServoDataPdoRead(int Servoid)
{   
	int i;
	PPUMP_STRU_5 pp = &g_pump_5;
  
	for (i=0;i<3;i++)
	{	
		pp->ServoPDORead.wData[i] = g_canservocomm[Servoid].pdo_rx_data[i] ;
	}
}

void        CanServoWriteTask_TransCheck(int portno)
{	
	PSERVO    ps = &g_hydrservo;
	PPUMP_STRU_5 pp = &g_pump_5;
	
	switch (pp->writestep)
	{
	  case 0:                                               //通讯检测流程  
	  pp->ServoPDOWrite.wData[0]=0x03;
	  pp->ServoPDOWrite.wData[1]=0x8001;
	  ServoDataPdoWrite(portno);
	  pp->write1ms = GetMsTick();
	  ps->mh->WRITECNT=1;
	  pp->writestep ++;
	  break;
	case 1:
	  if ((WORD)(GetMsTick() - pp->write1ms)>=10)
	  {
	    ServoDataPdoWrite(portno);
	    pp->write1ms = GetMsTick();
	    ps->mh->WRITECNT++;
	  }
	  break;
	}
}

UI16 SerSendList(UI16 flowlmt,UI16 portno)/*  注意:pp->svrflag位操作表示几号机处于主机模式和从机模式，
                                             所以在程序中当某动作把某个驱动器做为主机时一定要此WORD上的相应位置上1
                                           主机转为从机的时候一定要将相应位置为0,1号驱动器只能做为主机*/
{
	PPUMP_STRU_5 pp = &g_pump_5;
	UI16  	  pumplist;
	int       i;
	if(portno>0)
	{
		for(i=portno-1;i>=0;i--)
		{
			if(pp->svrflag>>i&1)
			{
				pumplist = pumplist&(~(pp->flowlmtServo_5[i])); // 2号通道时，i为0，先清除1号通道运行时，附上的值
			}                                                  // 同理，3号通道时，i为1和0，先清除1号和2号通道运行时，附上的值
		}
		pumplist = pumplist|(1<<portno);   //对应输出通道强制附上泵浦
	}
	else
		pumplist = flowlmt|(1<<portno);    // 1号通道时，强制附上1号泵
	return pumplist;
}

UI16      tmp,pumplist,tmp1,tmp2,tmp3,tmp4;
void  CanServoStateReadWrite(WORD portno,WORD cnt,WORD cmd,WORD para1,WORD para2)
{
	int k;
	PPUMP_STRU_5 pp = &g_pump_5;
	PHYDR     ph = &g_hydrout;
	WORD*     pwrm;	
	
	tmp = 0;
	tmp1 = 0;
	tmp2 = 0;
	tmp3 =0;
	pumplist = 0;
	tmp4 = 0;
	pwrm  =	  (WORD*)&(ph->po->O_PWRM_P1);	
	if (ChkSysCfg(C08_MSERVO))		                                          //	1: 使用多伺服大机功能
    {
    	pumplist = SerSendList(g_pump_5.flowlmtServo_5[portno-1],(portno-1));        
    } 
	if(portno ==1)
	{
		for (k=0;k<8;k++)                                                       //PUMP配置输出
		{
			if(DAIsAdjust())
			{
			}
			else
			{
				if(g_hydric[1].Press != 0  && g_hydric[1].Flow != 0)
					SetPOBit(*(pwrm+k),(g_pump_5.flowlmtServo_5[portno-1]>>k)&1);                           //PUMP配置阀输出只支持1号驱动器的PUMP配置
				else
					SetPOBit(*(pwrm+k),0); 
			}
		}      	
		if(ChkSysCfg(C08_MSERVO))
		{
			pumplist = (g_pump_5.flowlmtServo_5[portno-1] & (~(pp->svrflag)))|0x01;		//为了保证在其他驱动做主机的时候不发配置防止ERR47
		}
		else
			pumplist = 0x01;
		if(g_press[portno].pidpara.td == INJDFLAG)
		  pumplist = (pumplist|0x80);
	}
	if(cmd == 3)
	{
	    if(ChkSysCfg(C08_MCORE))
        {
            para1 = 210;
            para2 = 10;
        }
        
		tmp = (((para2&0x300)>>8));
		tmp1 = ((para1&0x3FF)<<2);
		tmp2 = (cmd<<13);
		tmp3 =( tmp|tmp1|tmp2);
		tmp4 =  ((para2&0xFF)<<8)|pumplist;
	}
	else
	{
		tmp3 =( (((cnt)|(cmd<<5))<<8)|(para1>>8));          
		tmp4 = (para1<<8)|pumplist;
	}
	tmp3 = ((tmp3>>8)|(tmp3<<8));                 //高低位反位
    pp->ServoPDOWrite.wData[0]=tmp3;              //发送索引以及需要回复的类型以及数据的高八位
	pp->ServoPDOWrite.wData[1]=tmp4;	          //低八位发送泵部配置以及数据的低八位
    pp->ServoPDOWrite.wData[2]=g_hydric[portno].Press;
    pp->ServoPDOWrite.wData[3]=g_hydric[portno].Flow;
    ServoDataPdoWriteMode5(portno);	
}

void  CanServoStateReadWriteChg(WORD portno)//主机转丛机发送数据格式
{
	PPUMP_STRU_5 pp = &g_pump_5;
	if(portno<2)
		return;
	pp->ServoPDOWrite.wData[0]=0;             //发送索引以及需要回复的类型以及数据的高八位
	pp->ServoPDOWrite.wData[1]=0;	          //低八位发送泵部配置以及数据的低八位
    pp->ServoPDOWrite.wData[2]=0;
    pp->ServoPDOWrite.wData[3]=0;
    ServoDataPdoWriteMode5(portno);	
}

void CanServoReadTaskMode5(int i)
{
	PSERVO    ps = &g_hydrservo;
	PPUMP_STRU_5 pp = &g_pump_5;
	UI16 k,x,ind,ly;
    UI16 servo_P,servo_I;

	switch(g_canservocomm[i].pdo_rx_mode5_data[0])
	{
		/* 索引值意义:共32个
		3 : 输出电流    6 : 输出功率  8 : 驱动器温度   19:从泵在线状态*/
		case RUNSTATE:
			ps->st->SERVO_ST[i-1] = (((g_canservocomm[i].pdo_rx_mode5_data[3]>>3)&0x07)|(ps->st->SERVO_ST[i-1]&0x8080));   //低BYTE，最后3位表示驱动器状态  高BYTE表示离线在线	
			ps->st->SERVO_PRESS[i-1] 	 = g_canservocomm[i].pdo_rx_mode5_data[4];        //实际压力
 			ps->st->SERVO_SPEED[i-1]     = g_canservocomm[i].pdo_rx_mode5_data[5];        //转速   		

			if(g_canservocomm[i].pdo_rx_mode5_data[1]==3)  //回复的电流监控
 			{
 				ps->st->SERVO_CURRENT[i-1] = g_canservocomm[i].pdo_rx_mode5_data[2];
				pp->statetcnt[i] = (pp->statetcnt[i] | (0x0001<<0)); // 置标记位
 			}
			else if(g_canservocomm[i].pdo_rx_mode5_data[1]==6)//回复功率监控
			{
				ps->st->SERVO_POWER[i-1] = g_canservocomm[i].pdo_rx_mode5_data[2];
				pp->ServoPower = ps->st->SERVO_POWER[0];
				pp->statetcnt[i] = (pp->statetcnt[i] | (0x0001<<1)); // 置标记位
			}
			else if(g_canservocomm[i].pdo_rx_mode5_data[1]==8)//回复驱动器模块温度
			{
				ps->st->SERVO_TEMP[i-1] = g_canservocomm[i].pdo_rx_mode5_data[2];
				pp->statetcnt[i] = (pp->statetcnt[i] | (0x0001<<2)); // 置标记位
			}
			else if(g_canservocomm[i].pdo_rx_mode5_data[1]==19)//回复驱动器在线离线状态
			{
				if(i==1)
				{
					for(ind=1;ind<8;ind++)
					{
							if((!(pp->svrflag>>(ind-1)&1))||(ind==1))	 //修改2号等油路做主机时，面板上2号驱动器绿色在线标记会消失
								ps->st->SERVO_ST[ind-1] = ((ps->st->SERVO_ST[ind-1]&0xFF7F)|(((g_canservocomm[i].pdo_rx_mode5_data[2]>>(ind-1))&1)<<7));
					}
				}
				pp->statetcnt[i] = (pp->statetcnt[i] | (0x0001<<3)); // 置标记位
			}			
			else
				pp->statetcnt[i] = 0;
			g_canservocomm[i].pdo_rx_mode5_data[0] = 0;
			break;
		case PARAREAD:
			break;
		case PARAWRITE:
			break;
		case PARAPISET:  //PI发送成功
		    if(ChkSysCfg(C08_MCORE))
            {      
                servo_P = 210;
                servo_I = 10;
            }
            else
            {      
                servo_P = p_Work_5[i].PIDPara_P_5;
                servo_I = p_Work_5[i].PIDPara_I_5;
            }
            
			if((g_canservocomm[i].pdo_rx_mode5_data[1]==servo_P)&&(g_canservocomm[i].pdo_rx_mode5_data[2]==servo_I))
			{
				pp->servocanmode[i] = RUNSTATE; //PI发送成功转入状态循环序列
			}
			g_canservocomm[i].pdo_rx_mode5_data[0] = 0;
			break;		
		case CHGCOMPARA:
			break;			
		default:
			break;	
	}
	if((UI16)(Get100UsTick() - pp->servocomst100us[1])>10000)
	{
		for(ind=0;ind<8;ind++)
		{
			ps->st->SERVO_ST[ind] = 0;
			for(ly = 0;ly<6;ly++)
				g_canservocomm[ind].pdo_rx_mode5_data[ly] = 0;
		}
	}
	if (ChkSysCfg(C08_MSERVO)&&pp->cantestmode ==1)		//	1: 使用多伺服大机功能
      k = pp->ServoNum;
    else
      k = i;
    ServoDataPdoRead(k);
    if ((g_canservocomm[k].pdo_rx_data[0]&0x00ff)==0x03)
    {
	  if(g_canservocomm[k].pdo_rx_data[1] == 0x8001)
	  {
      	ps->mh->READCNT++;
      	g_canservocomm[k].pdo_rx_data[0]=0;
		 pp->servocomst100us[1] = Get100UsTick();
	  }
	  else if(g_canservocomm[k].pdo_rx_data[1] == 0x7032) //功耗回复
	  {
	  	g_canservocomm[k].pdo_rx_data[0]=0;
		g_canservocomm[k].pdo_rx_data[1]=0;
	  	for(x=0;x<8;x++)
	  	{
			if((pp->serpower[x].readflag[k-1]&0x01)==0x01) //动作开始功耗读取完成
			{
				pp->serpower[x].powerstart[k-1]= g_canservocomm[k].pdo_rx_data[2];
				pp->serpower[x].readflag[k-1] |= 0x04;
				pp->serpower[x].readflag[k-1] &= (~0x01);
			}
			if((pp->serpower[x].readflag[k-1]&0x02)==0x02)//动作结束功耗读取完成
			{
				pp->serpower[x].powerend[k-1]= g_canservocomm[k].pdo_rx_data[2];
				pp->serpower[x].readflag[k-1] |= 0x08;
				pp->serpower[x].readflag[k-1] &= (~0x02);
			}
	  	}
		
	  }
    }
}


void CanServoWriteTask(WORD portno)
{
	PPUMP_STRU_5 pp = &g_pump_5;
	UI16	ind,i;
	
	if(pp->cantestmode  == 1)
	{
		if(ChkSysCfg(C08_MSERVO))
			CanServoWriteTask_TransCheck(pp->ServoNum);
		else
			CanServoWriteTask_TransCheck(portno);	
	}
	else
	{
		switch (pp->writestep)
		{
			case 0:                          //标准P、F参数传送	
		    /* 索引值意义:共32个
			3 : 输出电流    6 : 输出功率  8 : 驱动器温度*/
		  	if(pp->servocanmode[portno] == RUNSTATE)  //状态数据请求
		  	{
		  		if(pp->statetcnt[portno]>=15)
					pp->statetcnt[portno] = 0;			
				for(i=0;i<5;i++)
				{			
					if(pp->statetcnt[portno] ==0)
					{
						ind = 0;
						break;
					}
			   		else if((pp->statetcnt[portno]&(0x0001<<i))&&((pp->statetcnt[portno]&(0x0001<<(i+1)))==0))
			   		{
						ind = i+1;
						break;
			   		}
				}
				if(ind == 0)
					ind = 3;
				else if(ind == 1)
					ind = 6;
				else if(ind == 2)
					ind = 8;
				else if(ind == 3)
					ind = 19;
				else
					ind = 3;
		  		CanServoStateReadWrite(portno,ind,RUNSTATE,0,0);//状态数据请求
		  	}
			else if(pp->servocanmode[portno] == PARAREAD)  //数据读取
			{
			}
			else if(pp->servocanmode[portno] == PARAWRITE)  //数据写入
			{
			}
			else if(pp->servocanmode[portno] == PARAPISET)  //PI发送
			{
				CanServoStateReadWrite(portno,ind,PARAPISET,p_Work_5[portno].PIDPara_P_5,p_Work_5[portno].PIDPara_I_5);
			}		
		  break;    

		}  
 	}
}


void ReadStCold(WORD portno)
{
	PSERVO    ps = &g_hydrservo;
    if ((g_canservocomm[portno].pdo_rx_data[0]&0x00ff)==0x03)
    {
	  if(g_canservocomm[portno].pdo_rx_data[1] == 0x3000) //状态回复
	  {
	  	g_canservocomm[portno].pdo_rx_data[0]=0;
		g_canservocomm[portno].pdo_rx_data[1]=0;
		ps->st->SERVO_ST[portno-1] = (((ps->st->SERVO_ST[portno-1]|g_canservocomm[portno].pdo_rx_data[2])&0x87)|(1<<15));//收到回复提取状态并置位最高位状态启动读取完成
	  }
    }
}


void WriteModeVerify(WORD portno)
{
    PPUMP_STRU_5 pp = &g_pump_5;

  	pp->ServoPDOWrite.wData[0]=0x03;      //读命令
  	pp->ServoPDOWrite.wData[1]=0xa300; 
  	ServoDataPdoWrite(portno);
}

void ReadModeVerify(WORD portno)
{
	PPUMP_STRU_5 pp = &g_pump_5;

    if((g_canservocomm[portno].pdo_rx_data[0]&0x00ff)==0x03 && g_canservocomm[portno].pdo_rx_data[1] == 0xa300)
    {
  	    g_canservocomm[portno].pdo_rx_data[0]=0;
	    g_canservocomm[portno].pdo_rx_data[1]=0;
	   
	    pp->getmodebuf[portno] = g_canservocomm[portno].pdo_rx_data[2];
    }
}

BOOL ServoModeVerify()
{
	PPUMP_STRU_5 pp = &g_pump_5;
	int i;

	for(i=1;i<SERVO_MAX_NUM;i++)
	{
	    pp->getmodebuf[i]=0;	
	    ReadModeVerify(i);
		if(ChkSysCfg(XF6_CANSRV))
		{
			if(ChkSysCfg(CO0_SERVOP))
			{	
			    if(pp->getmodebuf[i]==5)
				{
				    SET_PART_ERR(pp, ER5_SVMODERR);
					return TRUE;
				}
			}
			else
			{	
			    if(pp->getmodebuf[i]==3)
				{
					SET_PART_ERR(pp, ER5_SVMODERR);
					return TRUE;
				}
			}
		} 
	}

	if((WORD)(GetMsTick()-pp->getmodetime) >= 5)
	{
		pp->getmodetime	= GetMsTick();

		if(pp->getmodecount >= (SERVO_MAX_NUM-1))
		    pp->getmodecount=0;
		WriteModeVerify(pp->getmodecount+1);
		pp->getmodecount++;
	}

	if(CHK_PART_ERR(pp, ER5_SVMODERR))
 	    CLR_PART_ERR(pp, ER5_SVMODERR);

	return FALSE;
}


void    ServoPIDCtrlProc()
{
	PHYDR     ph = &g_hydrout;
	PSERVO    ps = &g_hydrservo;
	PPUMP_STRU_5 pp = &g_pump_5;
	
	UI16      i;
	if(!ChkSysCfg(CO0_SERVOP)&&ChkSysCfg(XF6_CANSRV))
	{
		if (MotorIsOn())
		{
			if(((WORD)((Get100UsTick())-g_pump_5.F_3msclock)>= (SERVO_PID_TM))||(g_hydrout.press_start == 1))
			{
				g_pump_5.F_3msclock = Get100UsTick();
				g_hydrout.press_start = 0;					
				if(ChkSysCfg(XF6_CANSRV))					//伺服PID计算出的数据发送到驱动器和读取驱动器反馈的数据
				{
					CanServoReadTaskMode5(1);
					CanServoWriteTask(1);
					for(i=2;i<9;i++)
					{
						if((pp->svrflag&(1<<(i-1)))&&ChkSysCfg(C08_MSERVO))//2~8 号驱动器在做为主机时，在动作完成做泄压处理时要保证主机的压力下降到5公斤以下24MS后才停止
							                      							 //和此驱动的通信让此驱动器转为从机
						{								
							CanServoWriteTask(i);
							CanServoReadTaskMode5(i);
							if((g_hydric[i].Press ==0)&&(g_hydric[i].Flow==0)&& ((i <= g_pump_num) && (g_pumpn[i-1]->McPara.ActDelay == 0))
                                && g_pumpn[i-1]->McPara.McSet[0] == 0 && g_pumpn[i-1]->McPara.McSet[1] == 0) //pumpstru最后运行，因此动作开始赋上svrflag标记后，g_hydric[i].Press和g_hydric[i].Flow依旧为0，在pumpstru中才会赋值。会出现凑巧本循环清标记的情况。
							{
								if((g_canservocomm[i].pdo_rx_mode5_data[4]<=50))
								{											
									pp->svrsendcnt[i]++;
									if(pp->svrsendcnt[i]>10)
									{
										pp->svrflag = pp->svrflag&((~(1<<(i-1)))&0xff);
										CanServoStateReadWriteChg(i);
									}
									if(CHK_PART_ERR(ph, ER0_PRESERR))
										CLR_PART_ERR(ph, ER0_PRESERR);
								}
								else
								{
									if((GetMsTick()-pp->pres500ms[i]>500)&&(ChkSysCfg(XF0_PRESDIS)))
									{
										if(!CHK_PART_ERR(ph, ER0_PRESERR))
											SET_PART_ERR(ph, ER0_PRESERR);
									}
								}
							}
							else
							{
								pp->svrsendcnt[i] = 0;
								pp->pres500ms[i]=GetMsTick();
							}
						}
						else
						{
							pp->svrsendcnt[i] = 0;
							pp->pres500ms[i]=GetMsTick();
						}
					}
				}
			}
		}
		else
		{
			if((WORD)((Get100UsTick())-pp->F_3msclock)>= (2000))    //马达不开启的情况下200MS读一次驱动器状态  且只读轮询的状态
			{
				for(i=1;i<8;i++)
				{
					pp->servocanmode[i] = RUNSTATE; //PI发送成功转入状态循环序列
				}
				pp->F_3msclock = Get100UsTick();
				if(pp->cantestmode!=2)
				{
		  			g_hydric[1].Press =0;
		  			g_hydric[1].Flow = 0;
		  			CanServoReadTaskMode5(1);
		  			CanServoWriteTask(1);								
				}
				for(i=2;i<8;i++)
				{
					if(((ps->st->SERVO_ST[i-1])&0x80)) //没主动读取过主动读取一次在线的副泵的在线状态
					{
					    if((pp->servostcoldread>>8)!=(pp->servostcoldread&0xFF))
					    {
							pp->servostcoldread =( pp->servostcoldread|(1<<(i+7)));
							if(((ps->st->SERVO_ST[i-1])>>15)&1)
								pp->servostcoldread = (pp->servostcoldread|(1<<(i-1)));
							else
							{
			  					g_hydric[i].Press =0;
			  					g_hydric[i].Flow = 0;	
								ReadStCold(i);
						    	WriteStReadCold(i);
							}
					    }
						else
						{
							if(pp->cantestmode == 2)
								pp->cantestmode = 0;	
						}							
					}
				}
				
			}
		}
    }
} 

void	InitServoCanComm()
{
	PPUMP_STRU_5 pp = &g_pump_5;
	
	pp->ServoPDOWrite.wData[0]=0xFE00;
	pp->ServoPDOWrite.wData[1]=0;
	pp->ServoPDOWrite.wData[2]=0;
	ServoDataPdoWrite(1);
	CanServoWriteTask(1);
}



