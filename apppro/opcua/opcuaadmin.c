#include "command.h"
#include "oprintf.h" 
#include "opcuaadmin.h"
#include <signal.h>
#ifndef WIN32
#include <sys/timerfd.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "mxml.h"
#include "dbenum.h"

#define CODE_BG			(0)		//CODE 起始
#define CODE_ED			(200)	//CODE结束
#define OPCUAXML		SAVE_PATH"opcuaparam.xml"

extern UI32 g_warm1,g_warm2,g_warm3;

UA_Boolean running = true;

UA_Server *server = NULL;

static void stopHandler(int sign)
{
    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

//opcua读取
static UA_StatusCode readForVariable(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue)
{

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "read variable value");

    UA_Int32 * currentVal = (UA_Int32*)nodeContext;

    UA_Variant_setScalarCopy(&dataValue->value, currentVal,
                         &UA_TYPES[UA_TYPES_INT32]);

    dataValue->hasValue = true;

    if(nodeId == NULL)
    {
        ;
    }
    else
    {
        (*currentVal) = VarAdrToInt(nodeId->identifier.numeric);
    }


    return UA_STATUSCODE_GOOD;
}

//opcua写入
static UA_StatusCode writeForVariable(UA_Server *server,
                 const UA_NodeId *sessionId, void *sessionContext,
                 const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data)
{
    UA_Int32 * currentVal = (UA_Int32*)nodeContext;

    if (data->value.type == &UA_TYPES[UA_TYPES_INT32])
    {
        UA_Int32 updateVal = *(UA_Int32 *)data->value.data;
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "write variable value: %d", updateVal);


        (*currentVal) = updateVal;
        //::SetParamValue(nodeId->identifier.numeric,updateVal);修改面板数据
    }


    return UA_STATUSCODE_GOOD;
}

/*
** 添加变量
*/
static UA_NodeId addIntegerVariable(UA_Server *server, UA_NodeId parentNodeId,
                    UA_NodeId referenceType, const char *name,UI32 dwaddr)
{
    UA_StatusCode retval;

    /* Define the attribute of the myInteger variable node */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 myInteger = 0;
    UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)name);
    attr.displayName = UA_LOCALIZEDTEXT((char*)"en-US", (char*)name);
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    /* Add the variable node to the information model */
    UA_NodeId myIntegerNodeIdori =  UA_NODEID_NUMERIC(1, dwaddr);
    UA_NodeId myIntegerNodeId;
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, (char*)name);
    retval = UA_Server_addVariableNode(server, myIntegerNodeIdori, parentNodeId,
                                referenceType, myIntegerName,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                attr, NULL, &myIntegerNodeId);

    if (retval == UA_STATUSCODE_GOOD)
    {
        return myIntegerNodeId;
    }
    else
    {
        return UA_NODEID_NULL;
    }

}

/*
** 添加数据源
*/
static void addDataSource(UA_Server *server,UI32 dwaddr,const char *name)
{
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId referenceType = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);

    // 添加变量
    UA_NodeId retNodeId = addIntegerVariable(server, parentNodeId,
                                    referenceType, name,dwaddr);

    if(UA_NodeId_equal(&retNodeId, &UA_NODEID_NULL))
    {
        return;
    }

    // 准备数据源
    UA_DataSource varDataSource;
    varDataSource.read = readForVariable;
    varDataSource.write = writeForVariable;

    // 给变量添加数据源
    UA_Server_setVariableNode_dataSource(server, retNodeId, varDataSource);

    // 给nodeContext分配内存空间
    UA_Int32 *nodeContext = (UA_Int32 *)UA_malloc(sizeof(UA_Int32));
    (*nodeContext) = 0;

    // 给变量设置nodeConetext
    UA_Server_setNodeContext(server, retNodeId, nodeContext);

}

static void readXml(mxml_node_t* toptree)
{
    mxml_node_t * node = NULL;
    const char *paddr;
    const char *pname;
    UI32 dwAddr;
    UI16 codelimit ;

    for(node = mxmlFindElement(toptree,toptree,"param3",NULL,NULL,MXML_DESCEND),codelimit=CODE_BG;
        node != NULL;
        node = mxmlFindElement(node,toptree,"param3",NULL,NULL,MXML_DESCEND))
    {
        pname = mxmlElementGetAttr(node,"name");
        paddr = mxmlElementGetAttr(node,"addr");
        if((paddr!=NULL) && (pname!=NULL) && (codelimit < CODE_ED))
        {
            dwAddr = StrToWord((char *)paddr,0);
            if(server)
            {
                addDataSource(server,dwAddr,pname);
            }
            codelimit++;
        }
    }
}

static void GetOpcuaAddrXml(const char* filename)
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
                readXml(tree);
                mxmlDelete(tree);
            }
            else
            {
                oprintf("opcua addrxml error\n");
            }
            OBJMemFree(buf);
        }
    }
    else {
        oprintf("opcua addrxml error not find file\n");
    }
}

//初始化opcua
void initOpcua()
{
    oprintf("Start opcua-\n");

    server = UA_Server_new();
    if(server)
    {
        char ip[20] = "";
        UI8 iptmp = 0;
        UI8 ip0 = NetCtl_ReadIP(0,iptmp);
        UI8 ip1 = NetCtl_ReadIP(1,iptmp);
        UI8 ip2 = NetCtl_ReadIP(2,iptmp);
        UI8 ip3 = NetCtl_ReadIP(3,iptmp);
        sprintf(ip,"%d.%d.%d.%d",ip0,ip1,ip2,ip3);
        UA_ServerConfig_setDefault(UA_Server_getConfig(server));
        UA_ServerConfig_setCustomHostname(UA_Server_getConfig(server), UA_String_fromChars(ip));

        addAddr();

        UA_Server_run_startup(server);
    }

}

//加载xml文件，根据这里的dic，确定opcua需要的地址
void addAddr()
{
    if(server)
    {
        GetOpcuaAddrXml(OPCUAXML);
    }
}

//特殊处理dic的值，以及推力座转换
void SpcAddrFlash()
{
    VarAdrSetInt(p_PP_STATE_Warn1,g_warm1);//20250321 jhh 修改opcua警报地址
    VarAdrSetInt(p_PP_STATE_Warn2,g_warm2);
}

void flashOpcua()
{
    if(running)
    {
        if(server)
        {
            static UI32 time_1ms = 0;
            if(GetTick() - time_1ms >= 400)
            {
                time_1ms = GetTick();
                SpcAddrFlash();
                UA_Server_run_iterate(server, false);
            }
        }
    }
}

void closeOpcua()
{
    if(server)
    {
        UA_StatusCode retval = UA_Server_run_shutdown(server);

        UA_Server_delete(server);
    }
}


