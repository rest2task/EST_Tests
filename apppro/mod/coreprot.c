#include "coreprot.h"
#include "time.h"
#include "stdio.h"
#include "file.h"
#include "obj.h"
#include "database.h"
//#include "ctrlcmd.h"
//#include "protcmd.h"
#include "oprintf.h"
#include "pushtab.h"
#include "protcmd.h"

#define CORENUM 6


//20201110 dyl core pos mode
void SyncCoreAdvRetPos(UI32 dwAddr)
{
    int i;
    for(i=0;i<CORENUM;i++)
    {
        if(VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x100000*i) == 0) //core adv
        {
            //20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题 -- 此处不进行发送，后面一起发送
            VarAdrSetInt(d_coreA_ACTPARA_ADPOSI_COREINACT+0x100000*i,VarAdrToUI16(dwAddr));
            VarAdrSaveInt(d_coreA_ACTPARA_ADPOSI_COREINACT+0x100000*i);
            //20210730 dyl 中子有效区位置
            if(PushtableType()){
                CoreEffeAreaByPushtab_SendAll(d_coreA_ACTPARA_ADPOSI_COREINACT+0x100000*i, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+0x100000*i);
            }
        }
        else
        {
            ;
        }

        if(VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x100000*i) == 0)//20230303 dyl 中子动作点为开模后生效 //core ret
        {
            //20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题 -- 此处不进行发送，后面一起发送
            VarAdrSetInt(d_coreA_ACTPARA_ADPOSI_COREOUTACT+0x100000*i,VarAdrToUI16(dwAddr));
            VarAdrSaveInt(d_coreA_ACTPARA_ADPOSI_COREOUTACT+0x100000*i);
            //20210730 dyl 中子有效区位置
            if(PushtableType()){
                CoreEffeAreaByPushtab_SendAll(d_coreA_ACTPARA_ADPOSI_COREOUTACT+0x100000*i, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+0x100000*i);
            }
        }
        else
        {
            ;
        }

        //20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题 -- 此处一起发送
        if((VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD+0x100000*i) == 0) || (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD+0x100000*i) == 0))
        {
            UI16 pdata[2];
            UI32 startaddr = d_coreA_ACTPARA_ADPOSI_COREINACT+0x100000*i;
            UI16 ppart = (startaddr>>16)&0xFFF0;
            UI8 subid = (startaddr>>16)&0xf;
            UI16 offset = startaddr&0xFFFF;
            if(PushtableType())
            {
                pdata[0] = UnTransMeter(VarAdrToUI16((d_coreA_ACTPARA_ADPOSI_COREINACT+0x100000*i)), FALSE);
                pdata[1] = UnTransMeter(VarAdrToUI16((d_coreA_ACTPARA_ADPOSI_COREOUTACT+0x100000*i)), FALSE);
            }
            else
            {
                pdata[0] = VarAdrToUI16((d_coreA_ACTPARA_ADPOSI_COREINACT+0x100000*i));
                pdata[1] = VarAdrToUI16((d_coreA_ACTPARA_ADPOSI_COREOUTACT+0x100000*i));
            }
            ProtSendPartSubData(ppart,subid,offset,2,pdata);

            if(PushtableType())
            {
                UI16 pdata2[4];
                startaddr = d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+0x100000*i;
                ppart = (startaddr>>16)&0xFFF0;
                subid = (startaddr>>16)&0xf;
                offset = startaddr&0xFFFF;
                pdata2[0] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS+0x100000*i));
                pdata2[1] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREINMINUS+0x100000*i));
                pdata2[2] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS+0x100000*i));
                pdata2[3] = VarAdrToUI16((d_coreA_MACHSET_ADPOSI_EFFCOREOUTMINUS+0x100000*i));
                ProtSendPartSubData(ppart,subid,offset,4,pdata2);
            }

            //oprintf("send------------------\n");
        }
    }
}

//20210730 dyl 中子有效区位置
void CoreEffeAreaByPushtab(UI32 Pos_dwAddr, UI32 Effe_dwAddr)
{
    UI16 wValueMin,wValueMax,wValue1,wValue2;
    if (VarAdrToUI16(Pos_dwAddr) >= VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF))
        wValueMin=VarAdrToUI16(Pos_dwAddr)-VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF);
    else
        wValueMin=0;
    wValueMax=VarAdrToUI16(Pos_dwAddr)+VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF);

    UI16 wTran_Pos_dwAddr = UnTransMeter(VarAdrToUI16(Pos_dwAddr),FALSE);//20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题
    wValue1=UnTransMeter(wValueMax,FALSE)-wTran_Pos_dwAddr;
    wValue2=wTran_Pos_dwAddr-UnTransMeter(wValueMin,FALSE);
    VarSendSaveSetIntByAdr(Effe_dwAddr, wValue1);
    VarSendSaveSetIntByAdr(Effe_dwAddr+1, wValue2);
}

//20210730 dyl 中子有效区位置 //20250416 chj 优化修改开模终止位置后，马上按开模按键，按键值下发卡顿问题 -- 这个函数单独给 SyncCoreAdvRetPos 使用
void CoreEffeAreaByPushtab_SendAll(UI32 Pos_dwAddr, UI32 Effe_dwAddr)
{
    UI16 wValueMin,wValueMax,wValue1,wValue2;
    if (VarAdrToUI16(Pos_dwAddr) >= VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF))
        wValueMin=VarAdrToUI16(Pos_dwAddr)-VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF);
    else
        wValueMin=0;
    wValueMax=VarAdrToUI16(Pos_dwAddr)+VarAdrToUI16(d_autoctrl1_MOLDSET_SYS_COREPOSEFF);

    UI16 wTran_Pos_dwAddr = UnTransMeter(VarAdrToUI16(Pos_dwAddr),FALSE);
    wValue1=UnTransMeter(wValueMax,FALSE)-wTran_Pos_dwAddr;
    wValue2=wTran_Pos_dwAddr-UnTransMeter(wValueMin,FALSE);
    VarAdrSetInt(Effe_dwAddr, wValue1);
    VarAdrSaveInt(Effe_dwAddr);
    VarAdrSetInt(Effe_dwAddr+1, wValue2);
    VarAdrSaveInt(Effe_dwAddr+1);
}

//20210820 dyl 中子动作点跟随特殊中子功能及联动功能变化
void CorePointByLink()
{
    UI16 SpcCoreFunc=VarAdrToUI16(d_coreA_MOLDSET_FL_SPECIALCODE);
    if(SpcCoreFunc==0){
        if(VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL1)&0x2000)//Link Use
        {
            if(VarAdrToUI16(d_machine1_MOLDSET_FL_CLOSJTCORE)==1)
            {
                if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 1)
                {
                    VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,1);
                }
                if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREINACT) != VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE))
                {
                    VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT, VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE));
                    //20210730 dyl 中子有效区位置
                    if(PushtableType()){
                        CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS);
                    }
                }
            }
            if(VarAdrToUI16(d_machine1_MOLDSET_FL_OPNEJTCORE)==2)
            {
                if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 1)
                {
                    VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,1);
                }
                if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREOUTACT) != VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVEL))
                {
                    VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREOUTACT, VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVEL));
                    //20210730 dyl 中子有效区位置
                    if(PushtableType()){
                        CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS);
                    }
                }
            }
        }
        else
        {
            if(VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 1)
            {
                if(VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) == 0)
                {
                    if(((VarAdrToUI16(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x8400)
                            && ((VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL5)&0x8000))
                            && ((VarAdrToUI16(d_machine1_MACHSET_FL_MACHINTERNAL6) & 0x1020)&&((VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL0)&0x0004)==0))) //HMD
                    {
                        if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREINACT) != VarAdrToUI16(d_clamp1_ACTPARA_ADPOS_OPENSPD3))
                        {
                            VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT,VarAdrToUI16(d_clamp1_ACTPARA_ADPOS_OPENSPD3));
                            //20210730 dyl 中子有效区位置
                            if(PushtableType()){
                                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS);
                            }
                        }
                    }
                    else
                    {
                        if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREINACT) != VarAdrToUI16(d_clamp1_ACTPARA_ADPOSI_OPENENDEND))
                        {
                            VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT, VarAdrToUI16(d_clamp1_ACTPARA_ADPOSI_OPENENDEND));
                            //20210730 dyl 中子有效区位置
                            if(PushtableType()){
                                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS);
                            }
                        }
                    }

                }
                else
                {
                    if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREINACT) != 0)
                    {
                        VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT,0);
                        //20210730 dyl 中子有效区位置
                        if(PushtableType()){
                            CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS);
                        }
                    }
                }
            }
            if(VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 1)//开模中
            {
                if(VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) == 2)
                {
                    if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREOUTACT) != 0)
                    {
                        VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREOUTACT,0);
                        //20210730 dyl 中子有效区位置
                        if(PushtableType()){
                            CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS);
                        }
                    }
                }
                else
                {
                    if(((VarAdrToUI16(d_machine1_MACHSET_FL_CUSTOMID)&0xFF00) == 0x8400)
                            && ((VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL5)&0x8000))
                            && ((VarAdrToUI16(d_machine1_MACHSET_FL_MACHINTERNAL6) & 0x1020)&&((VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL0)&0x0004)==0))) //HMD
                    {
                        if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREOUTACT) != VarAdrToUI16(d_clamp1_ACTPARA_ADPOS_OPENSPD3))
                        {
                            VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREOUTACT,VarAdrToUI16(d_clamp1_ACTPARA_ADPOS_OPENSPD3));
                            //20210730 dyl 中子有效区位置
                            if(PushtableType()){
                                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS);
                            }
                        }
                    }
                    else
                    {
                        if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREOUTACT) != VarAdrToUI16(d_clamp1_ACTPARA_ADPOSI_OPENENDEND))
                        {
                            VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREOUTACT,VarAdrToUI16(d_clamp1_ACTPARA_ADPOSI_OPENENDEND));
                            //20210730 dyl 中子有效区位置
                            if(PushtableType()){
                                CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS);
                            }
                        }
                    }

                }
            }
        }
    }
    else if(SpcCoreFunc==1){
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,0);
        }
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,0);
        }
    }
    else if(SpcCoreFunc==2){
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,0);
        }
        if(VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL1)&0x2000)
        {
            if(VarAdrToUI16(d_machine1_MOLDSET_FL_OPNEJTCORE)==2)
            {
                if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 1)
                {
                    VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,1);
                }
                if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREOUTACT) != VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVEL))
                {
                    VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREOUTACT,VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVEL));
                    //20210730 dyl 中子有效区位置
                    if(PushtableType()){
                        CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREOUTACT, d_coreA_MACHSET_ADPOSI_EFFCOREOUTPLUS);
                    }
                }
            }
        }
    }
    else if(SpcCoreFunc==3){
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,0);
        }
        if(VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL1)&0x2000)
        {
            if(VarAdrToUI16(d_machine1_MOLDSET_FL_CLOSJTCORE)==1)
            {
                if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 1)
                {
                    VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,1);
                }
                if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREINACT) != VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE))
                {
                    VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT,VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE));
                    //20210730 dyl 中子有效区位置
                    if(PushtableType()){
                        CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS);
                    }
                }
            }
        }
    }
    else if(SpcCoreFunc==4){
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 2)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,2);
        }
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 2)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,2);
        }
    }
    else if(SpcCoreFunc==5){
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,0);
        }
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,0);
        }
    }
    else if(SpcCoreFunc==6){
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 2)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,2);
        }
        if(VarAdrToUI16(d_machine1_MACHSET_FL_MACHINECTRL1)&0x2000)
        {
            if(VarAdrToUI16(d_machine1_MOLDSET_FL_CLOSJTCORE)==1)
            {
                if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 1)
                {
                    VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,1);
                }
                if (VarAdrToUI16(d_coreA_ACTPARA_ADPOSI_COREINACT) != VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE))
                {
                    VarSendSaveSetIntByAdr(d_coreA_ACTPARA_ADPOSI_COREINACT,VarAdrToUI16(d_clamp1_MOLDSET_ADPOSI_EJTTRAVELCLOSE));
                    //20210730 dyl 中子有效区位置
                    if(PushtableType()){
                        CoreEffeAreaByPushtab(d_coreA_ACTPARA_ADPOSI_COREINACT, d_coreA_MACHSET_ADPOSI_EFFCOREINPLUS);
                    }
                }
            }
        }
    }
    else if(SpcCoreFunc==7){//20190408 CHJ
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_FWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_FWD,0);
        }
        if (VarAdrToUI16(d_coreA_MOLDSET_ADPOSI_CORE_BWD) != 0)
        {
            VarSendSaveSetIntByAdr(d_coreA_MOLDSET_ADPOSI_CORE_BWD,0);
        }
    }
}
