#include "iapadmin.h"
#include "oprintf.h" 
#include "protcmd.h"
#include "ctrlcmd.h"
#include "canmod.h"
#include "stdio.h"

#define    IAP_PATH	      USB_BOOT_PATH"iap/iap-" //20211229 dyl
UI16 nSend_IAPPack = 0;//20211229 dyl 当前发送的IAP包号
BOOL iap_bFirstPack = FALSE;//第一包发送后，证明扩展板重启完毕，但是还未收到回复包
UI32 niap_nfilelen;//读取到的文件长度
UI8* miap_buffer = NULL;//缓冲区指针
BOOL g_biap_issending = FALSE;//程序发送状态标志
//BOOL iap_bUpdateSuccess = FALSE;//程序更新成功标志
UI16 iap_UpdateState;
#define  PKG_UPDATE_DATA_LEN	(128)	//程序更新时数据部分的长度，固定128  ui16计数

void Iap_Send(int nparam1)
{
    if(g_biap_issending){//已处于更新状态 则取消发送
        Iap_SendCancel();
        nSend_IAPPack = 0;
        return;
    }
    iap_bFirstPack = FALSE;
    g_biap_issending = TRUE;
    iap_UpdateState = IAPUPDATE_START;
    /*****************先将文件读取出来放入缓冲区********************/
    char filename[64];
    snprintf(filename,sizeof(filename)-1,"%s%04d.bin",IAP_PATH,nparam1);
    if(FileExist(filename))
    {
        FILEHD filehd;
//        UI8* miap_buffer = NULL;
        filehd = FileOpen(filename);
        niap_nfilelen = FileGetSize(filehd, filename);
        miap_buffer = OBJMemMalloc(niap_nfilelen);
        if(miap_buffer != NULL)
        {
            FileRead(filehd,miap_buffer,niap_nfilelen,NULL);
            FileClose(filehd);
        }
        Total_filesize = niap_nfilelen;
        niap_nfilelen = (Total_filesize+1)/(sizeof(UI16));
    }
}

void Iap_SendCancel()
{
    /************更新取消，将标志、包号清零***************/
    if(g_biap_issending){
        OBJMemFree(miap_buffer);
        miap_buffer = NULL;
//        printf("iap_SendCancel\n");
        g_biap_issending = FALSE;
    }
    iap_UpdateState = IAPUPDATE_CANCEL;
    nSend_IAPPack = 0;
    g_nPrgPack = 0;
}

void Monitor_Iap()
{
    static UI16 wData;
    if(!g_biap_issending)//根据标志才运行下面代码
    {
        return;
    }
    if(nSend_IAPPack == 1 && !iap_bFirstPack){  //第一包发送后，证明扩展板重启完毕，但是还未收到回复包
        iap_bFirstPack = TRUE;
        g_nPrgPack = 0;
        iap_UpdateState = IAPUPDATE_WAIT;
        return;
    }
    if (g_nPrgPack!=nSend_IAPPack) //未收到数据，返回
    {
        iap_UpdateState = IAPUPDATE_WAIT;
        return;
    }
    //升级完成
//    oprintf("niap_nfilelen=%d\n",niap_nfilelen);
    if(niap_nfilelen == 0){//发送状态并且文件大小为0 表明发送完毕
        Iap_SendCancel();
        iap_UpdateState = IAPUPDATE_OK;//iap_bUpdateSuccess = TRUE;
        return;
    }
    else {
        iap_UpdateState = IAPUPDATE_ING;
    }
    //读取文件
    if(niap_nfilelen > PKG_UPDATE_DATA_LEN){
        wData = PKG_UPDATE_DATA_LEN;
    }
    else{
        wData = niap_nfilelen;
        nSend_IAPPack |= 0x8000;//最后一包的包号
    }
    niap_nfilelen-=wData;//用于判断是否发送完毕

    memcpy((char*)g_wPrgBuf, miap_buffer+(nSend_IAPPack&0x7FFF)*(PKG_UPDATE_DATA_LEN*sizeof(UI16)), wData*sizeof(UI16));//将文件读入缓存
//oprintf("nSend_IAPPack=%l,g_nPrgPack=%d\n",nSend_IAPPack,g_nPrgPack);
    nSend_IAPPack++;//发送的包号

    //发送数据

    CreateTaskCan(CAN_TX_IAPPRG,(g_CurrentChoose_Iap<<3|g_CurrentChoose_IapNum),nSend_IAPPack,wData);
}

BOOL IsIapSending()
{
    return g_biap_issending;
}

UI16 GetIapUpdateState()
{
    return iap_UpdateState;
}

UI16 GetSend_IAPPack()
{
    return nSend_IAPPack;
}

void ResetStart()
{
    nSend_IAPPack = 0;
    g_nPrgPack = 0;
    iap_UpdateState = IAPUPDATE_CANCEL;//iap_bUpdateSuccess = FALSE;
}
