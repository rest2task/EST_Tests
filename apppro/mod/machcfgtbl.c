#include "machcfgtbl.h"
#include "file.h"
#include "mxml.h"
#include "information.h"
#include "obj.h"
#include "vardb.h"
#include "oprintf.h"
#include "common.h"
enum enADDR
{
	ADDR_PART=0,//部件类别
	ADDR_PARTNUM,//部件号
	ADDR_SUBID,//子类号
	ADDR_PARAM,//具体参数
	ADDR_END//结束
};

typedef struct tyADDR_INFO
{
	const char*		type_name;
	int				type;
	UI32			leftshift;//左移位数
}ADDR_INFO;

ADDR_INFO  g_ADDRS[]=
{
	{"\xE9\x83\xA8\xE4\xBB\xB6\xE7\xB1\xBB\xE5\x88\xAB",ADDR_PART,24},//部件类别
	{"\xE9\x83\xA8\xE4\xBB\xB6",ADDR_PARTNUM,20},//部件号
	{"\xE5\xAD\x90\xE7\xB1\xBB",ADDR_SUBID,16},//子类号
	{"\xE5\x8F\x82\xE6\x95\xB0",ADDR_PARAM,0}//参数
};
const char g_valuecode[] = "\xE5\x80\xBC";//“值”的urlEncode码

#define		CODE_BG			(0)		//CODE 起始
#define		CODE_ED			(39)	//CODE结束
#define		METER_BG				(0)	//电尺部分
#define		METER_ED				(8)	

/************************************************************************/
/*遍历整个节点，并提取值进行对应地址赋值                        */
/************************************************************************/
void SearchNode(mxml_node_t* toptree,int level,UI32 adr)
{
	if(toptree == NULL && (level >= ADDR_END))
	{
		return;
	}
	else
	{
		mxml_node_t * node = NULL;
		const char *pvalue;
		UI16 value;
		UI16 offset=0;//偏移
		const char * name = g_ADDRS[level].type_name;
		for(node = mxmlFindElement(toptree,toptree,name,NULL,NULL,MXML_DESCEND);
			node != NULL;
			node = mxmlFindElement(node,toptree,name,NULL,NULL,MXML_DESCEND))
		{
			if(level == ADDR_PARAM)
			{
				pvalue = mxmlElementGetAttr(node,g_valuecode);
				if(pvalue!=NULL )
				{
					value = (UI16)StrToWord((char *)pvalue,0);
					VarAdrSetInt((adr | offset++),value);		
				}
			}
			else
			{
				pvalue = mxmlElementGetAttr(node,"ID");

				if(pvalue!=NULL )
				{
					UI32 tmpadr;
					value = (UI16)StrToWord((char *)pvalue,0); 

					if(level == ADDR_PARTNUM)//部件号应从1开始，实际文件中从0开始
					{
						value += 1;
					}
					tmpadr =adr | (value << g_ADDRS[level].leftshift);
					SearchNode(node,level+1,tmpadr);
				}
			}
		}
	}
}

void LoadPanelData(const char *filename)
{
	if(FileExist(filename))
	{
		FILEHD fd = FileOpen(filename);
		if(FILEHD_IS_OK(fd))
		{
			char *buf=NULL;
			int size;
			mxml_node_t * tree;
			size = FileGetSize(fd,filename);
			buf = (char*)OBJMemMalloc(size + 1);
			if(buf != NULL)
			{
				FileRead(fd,buf,size,NULL);
				FileClose(fd);
				
				/*pad string terminator  at tail*/
				buf[size] = '\0';
				tree = mxmlLoadString(NULL,buf,MXML_NO_CALLBACK);
				if(tree != NULL)
				{
					SearchNode(tree,ADDR_PART,0);
					mxmlDelete(tree);
				}
				else
				{
					oprintf("mxmlLoadString error\n");
				}
				
				OBJMemFree(buf);
			}
		}
	}
}

static void ParseXmlFile(mxml_node_t* toptree,MCCFG* tblcfg)
{
	mxml_node_t * node = NULL;
	const char *pvalue;
	UI32 value;
	UI16 codelimit ;
	UI16 meterlimit;
	UI16 * pdata;
	if(tblcfg == NULL)
		return;
	//CODE 部分
	pdata = (UI16 *)(&tblcfg->codetbl);
	for(node = mxmlFindElement(toptree,toptree,"code_table",NULL,NULL,MXML_DESCEND),codelimit=CODE_BG;
		node != NULL;
		node = mxmlFindElement(node,toptree,"code_table",NULL,NULL,MXML_DESCEND))
	{
		pvalue = mxmlElementGetAttr(node,"\xe9\x85\x8d\xe9\x80\x89");//“配选”
		if(pvalue!=NULL && (codelimit < CODE_ED))
		{
			value = StrToWord((char *)pvalue,0);
			*(pdata+codelimit) = (UI16)value;	//VarAdrSetInt(addr++,value);
			codelimit++;
		}
	}
	//电子尺部分
	pdata = (UI16 *)(&tblcfg->meter);
	for(node = mxmlFindElement(toptree,toptree,"meter_table",NULL,NULL,MXML_DESCEND),meterlimit=METER_BG;
		node != NULL;
		node = mxmlFindElement(node,toptree,"meter_table",NULL,NULL,MXML_DESCEND))
	{
		pvalue = mxmlElementGetAttr(node,"\xE7\x94\xB5\xE5\xAD\x90\xE5\xB0\xBA\xE8\xA1\x8C\xE7\xA8\x8B");//"电尺行程"
		if(pvalue!=NULL && (meterlimit < METER_ED))
		{
			value = StrToWord((char *)pvalue,0);
			*(pdata+3*meterlimit) = (UI16)value;//VarAdrSetInt(addr++,value);
		}
		pvalue = mxmlElementGetAttr(node,"\xE6\x9C\xBA\xE6\xA2\xB0\xE8\xA1\x8C\xE7\xA8\x8B");//"机械行程"
		if(pvalue!=NULL && (meterlimit < METER_ED))
		{
			value = StrToWord((char *)pvalue,0);
			*(pdata+3*meterlimit+1) = (UI16)value;//VarAdrSetInt(addr1++,value);
		}
		meterlimit++;
	}
}

void GetMainCfg(const char* filename,MCCFG* tblcfg)
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
				ParseXmlFile(tree,tblcfg);
				mxmlDelete(tree);
			}
			else
			{
				oprintf("mxmlLoadString error\n");
			}
			OBJMemFree(buf);
		}
	}
}
