#include "texttbl.h"
#include "oprintf.h" 
#include "language.h"

#define TEXT_TABLE                  DATA_PATH"text_table.map"

PHASH_TBL pmap_texttbl=NULL;//

void TextTblInital()
{
	ParseCSVFile(TEXT_TABLE,&pmap_texttbl);//
}

const char* GetTextTran(int mapid )
{
	if(pmap_texttbl == NULL)
		return NULL;
	return GetMapTran(pmap_texttbl,mapid);
}
