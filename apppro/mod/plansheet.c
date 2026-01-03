#include "plansheet.h"
#include "strhash.h"
#include "oprintf.h"
#include "taskmoni.h"
#include "oprecord.h"
#include "moldset.h"
#include "cJSON.h"
#include <stdio.h>
#include "database.h"
#include "time.h"
#ifndef WIN32
#define PANELDB_PRENAME "p.PP.NET."
#define CFG_PLAN    PANELDB_PRENAME"CFG_PLAN"
#define PLAN_DEBUG(fmt, ...)  oprintf("[D]%s()(%d):" fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define     MAX_HISTORY         (5)     //历史派单最大保存数量
#define     PER_SENDSHEET       (10)    //每10模发送派单信息

#define CFG_USERLOGIN               0x33180011 //用户登录功能

static PLANSHEETST g_planst;

static void PlanSetProductParam();

static void PlanInitIDFile()
{
    //计划单部分
    IOT_AddPackId(FID_REVSHEET,SHEET_TMPFILE);
    IOT_AddPackId(FID_CURSHEET,SHEET_CURFILE);
    IOT_AddPackId(FID_PLANHISTORY,SHEET_HISTORYFILE);
    IOT_AddPackId(FID_DELETESHEET,SHEET_DELFILE);
    IOT_AddPackId(FID_LASTSHEET,SHEET_OLDSHEETFILE);

    //补登部分
    IOT_AddPackId(FID_ERRORINFO,SHEET_ERRORFILE);
    IOT_AddPackId(FID_SUPPLEMENTINFO,SHEET_SUPPLEMENTFILE);
    IOT_AddPackId(FID_SUPPLEMENTSHEET,SHEET_CURSUPPLEMENTFILE);

    //其它
    IOT_AddPackId(FID_MOULDTX,MOULD_FILE);
    IOT_AddPackId(FID_MOULDRX,MOULD_RXFILE);
    IOT_AddPackId(FID_CURMOULD,MOULD_FILE);
    IOT_AddPackId(FID_PARAM,PARAM_FILE);

    //NET_VER2
    IOT_AddPackId(FID_USERREQINFO,USER_REQFILE);
    IOT_AddPackId(FID_USERINFO,USER_INFOFILE);
}



static void PlanInsertMold(void* pdat, int para1, int para2)
{
    MOLDSET_ITEM  info;
    DB_MOLDSET_EX   moldSetx;
    int i=1;
    BOOL b_empty = false;
    int emptypos = 0;
    int lastpos = 0;

    FILEHD filehd;

    for(i=1;i<=MAX_MOLDSET;i++)
    {
        if(!ReadMoldsetInfoTemp(i, &info))
        {
            emptypos=i;
            b_empty = true;
            break;
        }
        else
        {
            lastpos = i;
            if(lastpos == 99)
            {
                lastpos = 0;
            }
        }
    }

    filehd = FileOpen(MOULD_FILE);

    if(FILEHD_IS_OK(filehd))
    {
        FileSeek(filehd,0);
        FileRead(filehd,&info,sizeof(MOLDSET_ITEM),NULL);
        FileRead(filehd,&moldSetx,sizeof(DB_MOLDSET_EX),NULL);
        FileClose(filehd);
    }
    else
        return;

    if(b_empty)
    {
        SaveMoldsetInfo(emptypos,&info);
        SaveMoldSetEx(emptypos,&moldSetx);
    }
    else
    {
        SaveMoldsetInfo(lastpos,&info);
        SaveMoldSetEx(lastpos,&moldSetx);
    }
}

static void PlanSetDBByName(char * name ,char *v)
{
    if(name && v)
    {
        char prename[32];
        snprintf(prename,32,"%s%s",PANELDB_PRENAME,name);
        if(VarNameSetStr(prename,v)==NULL)
        {
            PLAN_DEBUG("set %s addr failed!",prename);
        }
    }
}

/**
 * @brief PlanSheetDBSync 同步JSON信息到数据库,要求数据库中数据名和 json中的保持一致
 *                          pjson只遍历下一级的元素
 */
static void PlanDBSync(cJSON * pjson)
{
    if(pjson != NULL)
    {
        cJSON * pitem = pjson->child;
        char *value = NULL;
        while(pitem)
        {
            value = cJSON_PrintItem(pitem);
            if(value != NULL)
            {
                PlanSetDBByName(pitem->string,value);
                OBJMemFree(value);
                value = NULL;
            }
            pitem = pitem->next;
        }
    }
}

/**
 * @brief PlanSheetSync 同步当前派单的数据到 数据库中
 */
static void PlanSheetSync()
{
    if(jf_isValid(g_planst.curPlan) )
        PlanDBSync(g_planst.curPlan->jsondata);
}

/**
 * @brief PlanUpdateSupp 同步本地补登总信息表
 */
static void PlanUpdateSupp()
{
    char * partid = VarNameToStr(PANELDB_PRENAME"partId");
    if(jf_isValid(g_planst.supp))
    {
        cJSON * pitem = NULL;
        pitem = cJSON_GetObjectItem(g_planst.supp->jsondata,partid);
        g_planst.curSup->jsondata = pitem;
    }
}

/**
 * @brief PlanSyncHistory 同步历史派单的数据到数据库
 */
static void PlanSyncHistory()
{
    UI32 addrname1 = VarNameToVarAdr(PANELDB_PRENAME"HISNAME1");
    UI32 addrstate1 = VarNameToVarAdr(PANELDB_PRENAME"HISSTATE1");
    UI32 addrcur1 = VarNameToVarAdr(PANELDB_PRENAME"HISCUR1");
    UI32 addrtotal1 = VarNameToVarAdr(PANELDB_PRENAME"HISTOTAL1");

    if(jf_isValid(g_planst.historyPlan))
    {
        int name=0;
        int state=0;
        int curvalue=0;
        int totalvalue=0;
        int supcnt =0;
        int i=0;
        BOOL ret = TRUE;
        cJSON *pitem = NULL;

        for(i=0;i < MAX_HISTORY ;i++)
        {
            pitem = cJSON_GetArrayItem(g_planst.historyPlan->jsondata,i);
            if(pitem == NULL)
                continue;
            ret &= js_getIntData(pitem,"partId",&name);
            ret &= js_getIntData(pitem,"state",&state);
            ret &= js_getIntData(pitem,"curOKCount",&curvalue);
            ret &= js_getIntData(pitem,"productCount",&totalvalue);
            ret &= js_getIntData(pitem,"supcnt",&supcnt);
            if(ret)
            {
                VarAdrSetInt(addrname1+i,name);
                VarAdrSetInt(addrstate1+i,state+1);//0 未完成，对应图片索引1，1：完成对应图片索引2
                VarAdrSetInt(addrcur1+i,curvalue);
                VarAdrSetInt(addrtotal1+i,totalvalue+supcnt);
            }
        }
    }
}

/**
 * @brief PlanRevSheetSlot  在手动状态下有效
 * @param pdat
 * @param para1
 * @param para2
 */
static void PlanRevSheetSlot(void* pdat, int para1, int para2)
{
    if (OperateModeIndex() == MODE_MANUAL)//手动状态下有效
    {
        if(g_planst.newPlan != NULL)
        {
            jf_deleteJsFile(g_planst.newPlan);
            g_planst.newPlan = NULL;
        }
        g_planst.newPlan = jf_openJsFile(SHEET_TMPFILE);
        if(jf_isValid(g_planst.newPlan))
        {
            int badcnt = 0;
            //保存到上一个单的文件中
            jf_saveTofile(g_planst.curPlan,SHEET_OLDSHEETFILE);

            jf_getIntData(g_planst.curSup,"count",&badcnt);
            jf_setIntData(g_planst.curPlan,"supcnt",badcnt);//记录补登信息

            //保存到历史派单中
            int size = 0;
            if(g_planst.historyPlan->jsondata != NULL)
                size = cJSON_GetArraySize(g_planst.historyPlan->jsondata);
            else
                g_planst.historyPlan->jsondata = cJSON_CreateArray();
            jf_addAsFirstToArry(g_planst.historyPlan,g_planst.curPlan->jsondata);

            if(size >= MAX_HISTORY)
            {
                jf_delArryItem(g_planst.historyPlan,MAX_HISTORY);
            }
            jf_save(g_planst.historyPlan);

            //同步历史单数据到数据库
            PlanSyncHistory();

            //发送上一个派单
            IOT_TxPackStart(FID_LASTSHEET,NULL);

            //把新建的派单拷贝到当前派单中
            jf_copyJsonData(g_planst.curPlan,g_planst.newPlan);
            jf_save(g_planst.curPlan);

            //同步派单的数据库
            PlanSheetSync();

            //更新当前补登信息
            PlanUpdateSupp();

            //基于当前的派单信息，重新设置生产配置
            PlanSetProductParam();
        }
    }
}

//次品原因中的语言翻译
const char *glanMap [12]={"eng","ch","ps","rus","spn","tk","port","ar","fr","kor","vn"};
static void PlanInitErrList()
{
    int idlan = VarAdrToInt(LANGE_SELECT);
    const char * curLan = NULL;
    idlan = (idlan >= 12 || idlan<0) ? 5 : idlan;
    curLan = glanMap[idlan];

    jf_deleteJsFile(g_planst.errList);
    g_planst.errList = jf_openJsFile(SHEET_ERRORFILE);
    g_planst.pcurErr = NULL;

    if(jf_isValid(g_planst.errList))
    {
        g_planst.pcurErr = cJSON_GetObjectItem(g_planst.errList->jsondata,curLan);
    }

    if(g_planst.pcurErr != NULL)
    {
        cJSON * pitem = NULL;
        UI32 addrerr1 = VarNameToVarAdr(PANELDB_PRENAME"ERR1");
        UI32 addrerr12 = VarNameToVarAdr(PANELDB_PRENAME"ERR12");
        UI32 addrerrvis1 = VarNameToVarAdr(PANELDB_PRENAME"ERRVIS1");

        int i=0;
        pitem = g_planst.pcurErr->child;
        for(i=addrerr1;(i<=addrerr12) && (pitem != NULL);i++)   //同步到数据库中
        {
            VarAdrSetStr(i,pitem->valuestring);
            VarAdrSetInt(addrerrvis1+i-addrerr1,1);
            pitem = pitem->next;
        }

        for(;i<=addrerr12;i++)
        {
            VarAdrSetStr(i,"");
            VarAdrSetInt(addrerrvis1+i-addrerr1,0);
        }
    }
}

/**
 * @brief PlanRevErrList 接收到次品列表
 * @param pdat
 * @param para1
 * @param para2
 */
static void PlanRevErrList(void* pdat, int para1, int para2)
{
    PlanInitErrList();//reload
}

/**
 * @brief PlanSendSuppEnd 发送补登信息回调函数
 * @param pdat
 * @param para1
 * @param para2
 */
static void PlanSendSuppEnd(void* pdat, int para1, int para2)
{
    PLAN_DEBUG("send ok");
}

/**
 * @brief PlanInitIDCallBack  init call back by group id
 */
static void PlanInitIDCallBack()
{
    IOT_SetPackIdCallBack(FID_MOULDRX,PlanInsertMold);
    IOT_SetPackIdCallBack(FID_REVSHEET,PlanRevSheetSlot);

    IOT_SetPackIdCallBack(FID_ERRORINFO,PlanRevErrList);

    IOT_SetPackIdCallBack(FID_SUPPLEMENTSHEET,PlanSendSuppEnd);
}

static void PlanInitSheetInfo()
{
    g_planst.historyPlan = jf_openJsFile(SHEET_HISTORYFILE);
    g_planst.curPlan = jf_openJsFile(SHEET_CURFILE);
}

static void PlanInitSuppInfo()
{
    g_planst.supp = jf_openJsFile(SHEET_SUPPLEMENTFILE);
    g_planst.curSup = jf_openJsFile(SHEET_CURSUPPLEMENTFILE);
}

void PlanInit()
{
    if(!FileExist(PLAN_DIR))
    {
        MkDir(PLAN_DIR);
    }

    PlanInitIDFile();
    PlanInitIDCallBack();

    PlanInitSheetInfo();

    PlanInitErrList();

    PlanInitSuppInfo();

    //同步派单的数据库
    PlanSheetSync();
    PlanSyncHistory();

    UserLoginInit();
}

void PlanUpdate()
{
    static UI32 staticShotcnt = 0;
    UI32 shotcnt = VarAdrToInt(PROD_SHOTCNT);
    if((staticShotcnt != shotcnt) && (OperateModeIndex() != MODE_MANUAL))
    {
        staticShotcnt = shotcnt;

        if((jf_isValid(g_planst.curPlan)) && (VarNameToInt(CFG_PLAN)))
        {
            BOOL finish = FALSE;
            int realcount = 0;
            //UI32 percount = 1;
            BOOL issend = FALSE;
            int plancnt = 0;
            int badshot = 0;
            int destcount = 0;

            jf_getIntData(g_planst.curPlan,"curOKCount",&realcount);
            //percount = VarAdrToInt(PROD_PERCNT);
            issend = realcount%PER_SENDSHEET == (PER_SENDSHEET - 1);
            jf_getIntData(g_planst.curPlan,"productCount",&plancnt);
            jf_getIntData(g_planst.curSup,"count",&badshot);

            destcount = plancnt + badshot;
            realcount++ ;
            if(destcount == realcount)
            {
                finish = TRUE;
            }
            else if(destcount < realcount)//超过设定值
            {
                return;
            }

            if(realcount == 1)//首模，记录起始时间
            {
                char strtmp[32];
                TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",(UI32)time(NULL));
                jf_setStrData(g_planst.curPlan,"realSTime",strtmp);
            }

            if(finish)//结束，记录完成时间
            {
                char strtmp[32];
                TimeToStr(strtmp,"yyyy-MM-dd hh:mm:ss",(UI32)time(NULL));
                jf_setStrData(g_planst.curPlan,"realETime",strtmp);
                jf_setIntData(g_planst.curPlan,"state",1);
            }
            jf_setIntData(g_planst.curPlan,"curOKCount",realcount);

            jf_save(g_planst.curPlan);

            //同步派单的数据库
            PlanSheetSync();

            if(issend || finish)
            {
                PlanSendFile(FID_CURSHEET,0);
            }
        }
    }
}

/**
 * @brief 根据当前的选择来重新设置生产部分的值
 */
static void PlanSetProductParam()
{
    if((jf_isValid(g_planst.curPlan)) && (VarNameToInt(CFG_PLAN)))//在派单使能状态下有效
    {
        int permold = 1;
        int prodCount = 0;
        int curOKCount = 0;
        int badshot = 0;
        int opencount = 0;
        BOOL ret = TRUE;
        IOT_SaveTotalClampOpen();
        ret &= jf_getIntData(g_planst.curPlan,"countPerMold",&permold);
        ret &= jf_getIntData(g_planst.curPlan,"productCount",&prodCount);
        ret &= jf_getIntData(g_planst.curPlan,"curOKCount",&curOKCount);
        jf_getIntData(g_planst.curSup,"count",&badshot);
        if(!ret)
            return;

        //产品数应当是 补登数+预期设定值
        prodCount = prodCount + badshot - curOKCount;

        VarAdrSetInt(CLAMP_MOLD_MOLDREACHCHK,1);
        VarAdrSetInt(PROD_PERCNT,permold);
        VarAdrSaveInt(CLAMP_MOLD_MOLDREACHCHK);
        VarAdrSaveInt(PROD_PERCNT);

        VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM0,0);
        VarSendByAdr(CLAMP_STATE_MOLDOPNNUM0);
        VarAdrSetInt(CLAMP_STATE_MOLDOPNNUM1,0);
        VarSendByAdr(CLAMP_STATE_MOLDOPNNUM1);
        VarAdrSetInt(PROD_TOTAL_SHOTCNT,0);
        VarAdrSaveInt(PROD_TOTAL_SHOTCNT);

        if(prodCount%permold==0)//可以整除
        {
            opencount =prodCount/permold;
        }
        else
        {
            opencount =prodCount/permold+1;
        }

        VarSendSaveSetIntByAdr(CLAMP_MOLD_PRODUCTSETTING1,opencount>>16);
        VarSendSaveSetIntByAdr(CLAMP_MOLD_PRODUCTSETTING2,opencount&0xffff);
        VarAdrSetInt(PROD_TARGETCNT,opencount);

        VarAdrSetInt(CLAMP_STATE_VALIDMOLD,0);
        VarAdrSetInt(PROD_TARGETCNT,0);
        VarAdrSetInt(PROD_TOTALPRODCNT,0);

        VarSendSaveSetIntByAdr(CLAMP_STATE_VALIDMOLD,0);//下位机根据该值来判断是否到达设定个数

    }
}

static void PlanCleanProdParam()
{
    VarSendSaveSetIntByAdr(CLAMP_MOLD_PRODUCTSETTING1,0);
    VarSendSaveSetIntByAdr(CLAMP_MOLD_PRODUCTSETTING2,0);
    VarAdrSetInt(PROD_TARGETCNT,0);
}

void PlanSetEnable(BOOL enable)
{
    VarNameSetInt(CFG_PLAN,enable);

    if(enable)
        PlanSetProductParam();
    else
        PlanCleanProdParam();
}

/**
 * @brief PlanCreateNewSupp 根据填入的信息创建 一张补登表，保存到 SHEET_TMPSUPPLEMENTFILE文件中
 *SHEET_TMPSUPPLEMENTFILE:  {"partSn":"TEST-00001-005","productAllSn":"TEST-00001","count":3,"errorMap":{"5":"3","6":"0","7":"0"}}
 */
BOOL PlanCreateNewSupp()
{
    int errcont = VarNameToInt(PANELDB_PRENAME"ERRCOUNT");
    if(errcont > 0 && (VarNameToInt(CFG_PLAN)))
    {
        cJSON *newitem = NULL;
        if((newitem = cJSON_CreateObject()) != NULL)
        {
            UI32 addr1 = VarNameToVarAdr(PANELDB_PRENAME"ERR1CNT");
            UI32 addr12 = VarNameToVarAdr(PANELDB_PRENAME"ERR12CNT");
            cJSON * pitem = NULL;
            cJSON * errMap = NULL;
            pitem = (g_planst.pcurErr != NULL)?g_planst.pcurErr->child : 0;
            UI32 i=0;
            UI32 value = 0;
            int oldCount = 0;
            int oldValue[12] = {0};

            char *partID = VarNameToStr(PANELDB_PRENAME"partId");
            if(partID == NULL)
            {
                return FALSE;
            }

            cJSON_AddStringToObject(newitem,"partSn",partID);
            cJSON_AddStringToObject(newitem,"productAllSn",VarNameToStr(PANELDB_PRENAME"productAllId"));

            //获取旧数据
            if(g_planst.curSup->jsondata != NULL)
            {
                cJSON * errList = NULL;
                errList = cJSON_GetObjectItem(g_planst.curSup->jsondata, "errorMap");
                jf_getIntData(g_planst.curSup,"count",&oldCount);
                for(i = 0;i <= (addr12-addr1) && (pitem != NULL);i++)
                {
                    js_getIntData(errList,pitem->string,&oldValue[i]);
                    pitem = pitem->next;
                }
            }

            jf_setIntData(g_planst.curPlan,"state",0);
            //jf_getIntData(g_planst.curPlan,"supcnt",&oldErrcnt);
            //jf_setIntData(g_planst.curPlan,"supcnt",oldErrcnt+errcont);

            //保存到新补登中
            cJSON_AddIntToObject(newitem,"count",oldCount+errcont);
            pitem = (g_planst.pcurErr != NULL)?g_planst.pcurErr->child : 0;
            errMap = cJSON_CreateObject();
            for(i=addr1;(i<=addr12) && (pitem != NULL);i++)
            {
                value = VarAdrToInt(i);
                cJSON_AddIntToObject(errMap,pitem->string,oldValue[i-addr1]+value);
                pitem = pitem->next;
            }
            cJSON_AddItemToObject(newitem,"errorMap",errMap);

//            if(g_planst.curSup->jsondata != NULL)
//            {
//                cJSON_Delete(g_planst.curSup->jsondata);
//            }
            g_planst.curSup->jsondata = newitem;

            jf_save(g_planst.curSup);
            //保存到总补登信息表中
            if(g_planst.supp->jsondata == NULL)
            {
                g_planst.supp->jsondata = cJSON_CreateObject();
            }

            {
                if(newitem != NULL)
                {
                    if(js_findMatchItem(g_planst.supp->jsondata,partID,cJSON_Object))
                    {
                        cJSON_ReplaceItemInObject(g_planst.supp->jsondata,partID,newitem);
                    }
                    else
                    {
                        cJSON_AddItemToObject( g_planst.supp->jsondata,partID,newitem);
                    }
                    jf_save(g_planst.supp);
                }
            }

            //发送补登信息
            PlanSendFile(FID_SUPPLEMENTSHEET,0);

            PlanSetProductParam();
            return TRUE;
        }

    }
    return FALSE;
}

void PlanReqFile(int fileid,PROT_CB tx_cb)
{
    IOT_RxPackStart(fileid,tx_cb);
}

void PlanSendFile(int fileid, PROT_CB tx_cb)
{
    IOT_TxPackStart(fileid,tx_cb);
}

//通过缓存发送
void PlanSendFileEX(UI16 type_id, UI8 *data, UI16 len, PROT_CB tx_cb)
{
    IOT_TxPackStartEX( type_id,  data, len, tx_cb);
}

/*****USERLOGIN******/
#include "key.h"
#define USERLOINGPAGE   "UserLogin"
static void revUserInfo(void* pdat, int para1, int para2)
{

}
void UserLoginInit()
{
    IOT_SetPackIdCallBack(FID_USERREQINFO,revUserInfo);//用户登入
    IOT_SetPackIdCallBack(FID_USERINFO,revUserInfo);//用户登入
}
void UserLoginShow(int cur_key)
{
    if(cur_key == _ENTER && VarAdrToInt(CFG_USERLOGIN))
    {
        if(PanelFocusPage() == PanelMainPage())
            PanelShowDialogByName(USERLOINGPAGE);
        else
            PanelCloseDialogByName(USERLOINGPAGE);
    }
}
/*****USERLOGIN END*******/
#endif


