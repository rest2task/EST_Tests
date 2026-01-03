#include "curvesctl.h"
#include "stdio.h"
#include "protcmd.h"
#include "graphwgt.h"
#include "strhash.h"
#include "pagefram.h"
#include "panel.h"
#include "oprintf.h"
#include "verinfo.h"
#include "panelcfg.h"
#include "database.h"
#include "key.h"
#include "tablectl.h"
#include "command.h"

#define MAX_CURVES_WIN		(100)
#define CURVEID(v)		(0xC000|v)
#define MIN_PERIOD			(10)	//ms
#define TEMPCURVE_NUM        9  //20190618.cyx 温度曲线条数
#define DRIVE_TEMPCURVE_NUM  4  //20190618.cyx 驱动器温度曲线条数
#define MAX_TEMPCURVE_POINT  2880//温度曲线最大点数 4h，5s取一个点   //20190618.cyx

#define TEMP1_REAL           0x05160003 //温度实际值1
#define TEMP2_REAL           0x05260003 //温度2实际值1//20250312 jhh 增加温度曲线2画面

typedef enum enCURVEOP
{
	OP_CLOSE=1,
	OP_SEND=2,
	OP_STOPSEND=3,
	OP_CLRRECORD=4,
	OP_STOP=5,
	OP_START=6,
	OP_DRAWING=0xff
}CURVEOP;

FPOINT* g_point[MAX_ADDRS] ;//临时存储的数据
FPOINT m_point[TEMPCURVE_NUM][MAX_TEMPCURVE_POINT] = {0};//20190618.cyx 用于存放温度曲线数据
FPOINT m_point2[TEMPCURVE_NUM][MAX_TEMPCURVE_POINT] = {0};//20250312 jhh 增加温度曲线2画面
FPOINT m_drivepoint[DRIVE_TEMPCURVE_NUM][MAX_TEMPCURVE_POINT] = {0};//20190618.cyx 用于存放驱动器温度曲线数据
static BOOL tempcurve_repaint = FALSE;
static BOOL tempcurve_2_repaint = FALSE;//20250312 jhh 增加温度曲线2画面
static UI32 tempcurve_num = 0;
static UI32 tempcurve_2_num = 0;//20250312 jhh 增加温度曲线2画面
static BOOL drivetempcurve_repaint = FALSE;
static UI32 drivetempcurve_num = 0;
#define TMP_BUF		(125)
static BOOL CurvesOP(PGRAPH_WGT pwgt ,int op);
static PHASH_TBL g_pSets = NULL;
static int g_nCount = 0;
typedef struct tyWinCURVE_SETS *PWINCURVES_SETS;
typedef struct tyWinCURVE_SETS WINCURVES_SETS;
struct tyWinCURVE_SETS{
	CWM_HWIN hwin;
	UI32 state;
	PGRAPH_WGT pwgt;
	CURVE_DATA_RX_CB rx_cb;
};

PWINCURVES_SETS NewSets(CWM_HWIN hwin,PGRAPH_WGT pwgt)
{
	PWINCURVES_SETS psets=NULL;
	psets = (PWINCURVES_SETS)OBJMemMalloc(sizeof(WINCURVES_SETS));
	if(psets)
	{
		psets->hwin = hwin;
		psets->pwgt = pwgt;
		psets->state = OP_CLOSE;
		psets->rx_cb = NULL;
        if(pwgt != NULL)
            pwgt->id = CURVEID(g_nCount);
	}
	return psets;
}

void AttachGraph(PGRAPH_WGT pwgt)
{
	if(pwgt == NULL)
		return;	

	if(g_pSets==NULL)
	{
		g_pSets = HashTblAlloc(MAX_CURVES_WIN);
	}
	if(g_pSets!=NULL)
	{
		PWINCURVES_SETS psets=NULL;
		CWM_HWIN hwin = (CWM_HWIN)(((POBJ)pwgt)->pparent->handle);
		if((psets = NewSets(hwin,pwgt))!=NULL)
		{
			pwgt->cmd1.rec_id = CURVEID(g_nCount++);
			HashTblAddPairByInt(g_pSets,pwgt->cmd1.rec_id,psets);
		}
	}
}

void BindGraphCB(int id,CURVE_DATA_RX_CB rx_cb)
{
    if(g_pSets==NULL)
    {
        g_pSets = HashTblAlloc(MAX_CURVES_WIN);
    }
    if(g_pSets!=NULL)
    {
        PWINCURVES_SETS psets=NULL;
        if((psets = NewSets(0,0))!=NULL)
        {
            psets->rx_cb = rx_cb;
            HashTblAddPairByInt(g_pSets,id,psets);
        }
    }
}

void CurvesInital()
{
	int i=0;

	for(i=0;i<MAX_ADDRS;i++)
	{
		if(g_point[i]==NULL)
		{
			g_point[i]=(FPOINT*)OBJMemMalloc(sizeof(FPOINT)*TMP_BUF);
		}
	}
}

BOOL CurvesStart(PGRAPH_WGT pwgt)
{
	int i;
	
	if(pwgt!=NULL)
	{
		UI32 mode = RU32(pwgt->curve_mode);
		PWINCURVES_SETS psets=(PWINCURVES_SETS)HashTblFindPairByInt(g_pSets,pwgt->cmd1.rec_id);
		if(psets!=NULL)
		{
			if(mode == CURVE_PROT)//send cmd
			{
				ProtReqRec(pwgt->cmd1.rec_id,pwgt->cmd1.trigger_id,pwgt->cmd1.rec_period,pwgt->cmd1.rec_total_ms,
					pwgt->cmd1.ind_num,pwgt->cmd2);
				for(i=0;i<pwgt->cmd1.ind_num;i++)
				{
					oprintf("1 rec_id=%d,checkid=%d,pwgt.cmd2 part_id=0x%x,sub_ind=%d,offset=%d:\n",pwgt->cmd1.rec_id,pwgt->cmd1.trigger_id,pwgt->cmd2[i].part_id,pwgt->cmd2[i].sub_ind,pwgt->cmd2[i].offset);
				}

			}
			else if(mode == CURVE_NORMAL)
			{
				//nothing to do
			}
			psets->state = OP_START;
			return TRUE;
		}
	}
	return FALSE;
}

/************************************************************************/
/*自定义发送协议曲线请求 
PROT_DATREC_REQ_PKT cmd1;   //协议数据1
PDATSET_IND cmd2	//协议数据2
*/
/************************************************************************/
BOOL CurvesStartEx(PPROT_DATREC_REQ_PKT cmd1,PDATSET_IND cmd2)
{
	if(cmd1 == NULL || cmd2 == NULL)
		return FALSE;
	{
		int i;
		PWINCURVES_SETS psets=(PWINCURVES_SETS)HashTblFindPairByInt(g_pSets,cmd1->rec_id);
		if(psets!=NULL)
		{
		
			ProtReqRec(cmd1->rec_id,cmd1->trigger_id,cmd1->rec_period,cmd1->rec_total_ms,
				cmd1->ind_num,cmd2);
			for(i=0;i<cmd1->ind_num;i++)
			{
				oprintf("1 rec_id=%d,checkid=%d,pwgt.cmd2 part_id=0x%x,sub_ind=%d,offset=%d:\n",cmd1->rec_id,cmd1->trigger_id,cmd2[i].part_id,cmd2[i].sub_ind,cmd2[i].offset);
			}
			
			psets->state = OP_START;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CurvesStop(PGRAPH_WGT pwgt)
{
	return CurvesOP(pwgt,OP_STOP);
}

/************************************************************************/
/*自定义发送协议曲线停止 

*/
/************************************************************************/
BOOL CurvesStopEx(UI16 rec_id)
{
	return ProtRecOP(rec_id,OP_STOP);
}

BOOL CurvesClose(PGRAPH_WGT pwgt)
{
	return CurvesOP(pwgt,OP_CLOSE);
}
/************************************************************************/
/* 下位机的数据清除  
*/
/************************************************************************/
BOOL CurvesClear(PGRAPH_WGT pwgt)
{
	return CurvesOP(pwgt,OP_CLRRECORD);
}

/************************************************************************/
/* 图像清除   
*/
/************************************************************************/
BOOL GraphClear(PGRAPH_WGT pwgt)
{
	if(pwgt!=NULL)
	{
		Graph_Clear(pwgt);
	}
	return TRUE;
}

static BOOL CurvesOP(PGRAPH_WGT pwgt ,int op)
{
	if(pwgt!=NULL)
	{
		UI32 mode = RU32(pwgt->curve_mode);
		PWINCURVES_SETS psets=(PWINCURVES_SETS)HashTblFindPairByInt(g_pSets,pwgt->cmd1.rec_id);
		if(psets!=NULL)
		{
			if(mode == CURVE_PROT)
			{
				ProtRecOP(pwgt->cmd1.rec_id,op);
			}
			else if(mode == CURVE_NORMAL)
			{

			}
			psets->state = op;
			return TRUE;
		}
		
	}
	return FALSE;
}

/************************************************************************/
/* 曲线增加数据	
pwgt 曲线对应的句柄
ppkt：数据包
*/
/************************************************************************/
static void GraphAddData(PGRAPH_WGT pwgt,void* data)
{
	if(pwgt == NULL || data==NULL)
		return;
	{
		unsigned curveCount=pwgt->curvelist.item_num;//实际曲线个数
		unsigned dataCount=pwgt->cmd1.ind_num;//协议中的数据种类数
		PPROT_DATREC_RSP_PKT ppkt = (PPROT_DATREC_RSP_PKT)data;
		unsigned i=0,j=0;
		I16 *pdata;
		FPOINT** ppoint = g_point;
		if(dataCount == 0)
			return;
		if(ppkt->rec_len!=0)
		{
			if(pwgt->doClear)
			{
				Graph_Clear(pwgt);
			}

			pdata = (I16*)(ppkt+1);
			for(i=0;i<ppkt->rec_len/dataCount;i++)
			{
				for(j=0;j<curveCount;j++)
				{
					ppoint[j][i].y = (pdata[i*dataCount+j]);
					if(curveCount < dataCount)//约定 x轴为数据的最后
					{
						ppoint[j][i].x = (pdata[(i+1)*dataCount-1]);
					}
					else	//X轴为时间	ms
					{
						ppoint[j][i].x = ((ppkt->rec_ticks+i)*pwgt->cmd1.rec_period);
					}
				}
			}
			for(j=0;j < curveCount;j++)
			{
				GraphAddPoint(pwgt,j,ppoint[j],i);
			}
		}
		else
		{
			oprintf("clear\n");
			pwgt->doClear = TRUE;
		}
	}
}
/************************************************************************/
/* 协议获取的数据  
*/
/************************************************************************/
void DataRecRspRx(PPROT_DATREC_RSP_PKT ppkt)
{
	if(ppkt!=NULL)
	{
		PWINCURVES_SETS psets=(PWINCURVES_SETS)HashTblFindPairByInt(g_pSets,ppkt->rec_id);
		if(psets!=NULL)
		{
			if(psets->rx_cb != NULL)//新增回调函数的处理
			{
				(*psets->rx_cb)(psets->pwgt,ppkt);
			}
			else
			{
				GraphAddData(psets->pwgt,ppkt);
			}
			
		}
	}
}

/************************************************************************/
/* 普通定时曲线数据采集  
*/
/************************************************************************/
BOOL  CurvesMonitor(PGRAPH_WGT pwgt)
{
	static UI32 lasttime = 0;//最近时间
	if((pwgt != NULL)  && (RU32(pwgt->curve_mode) == CURVE_NORMAL))
	{
		UI32 period = RU32(pwgt->period);
		UI32 nowtime = GetTick();
		period = (period < MIN_PERIOD)? MIN_PERIOD:period;//不能太小（单位ms）？
		
		if((nowtime - lasttime) >= period)
		{
			unsigned curveCount=pwgt->curvelist.item_num;//实际曲线个数
			PWINCURVES_SETS psets=(PWINCURVES_SETS)HashTblFindPairByInt(g_pSets,pwgt->cmd1.rec_id);
			unsigned i;
			FPOINT point;
			UI32 addrx,addry;
			
			if(psets==NULL)
				return FALSE;
			if(psets->state == OP_START)//首次赋值
			{
				lasttime = nowtime;
				psets->state = OP_DRAWING;
			}

			for(i=0;i<curveCount;i++)
			{
				addrx = RU32(pwgt->curvelist.curves[i]->addrX);
				addry = RU32(pwgt->curvelist.curves[i]->addrY);

				if(addrx==0)//当0时默认为时间
				{
					point.x = (nowtime-lasttime);
				}
				else point.x = VarAdrToInt(addrx);

				point.y = VarAdrToInt(addry);
				
				GraphAddPoint(pwgt,i,&point,1);
			}
			lasttime = nowtime;
		}
		return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/*                     针对协议数据的回调函数设置
*/
/************************************************************************/
CURVE_DATA_RX_CB CurvesSetCbSet(UI16 set_id, CURVE_DATA_RX_CB rx_cb)	
{
	PWINCURVES_SETS psets=(PWINCURVES_SETS)HashTblFindPairByInt(g_pSets,set_id);
	CURVE_DATA_RX_CB oldcb=NULL;
	if(psets==NULL)
		return NULL;
	oldcb = psets->rx_cb;
	psets->rx_cb = rx_cb;
	return oldcb;
}

//20190618.cyx 温度曲线取数据
void TempCurveGetData()
{
    static UI32 time_5s = 0;
    static UI32 time = 0;
    static BOOL heater = FALSE;
    UI16 i;
    UI32 heater_state;

    //20200323
    if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
    {
        heater_state = VarAdrToInt(d_TempCtrlComm1_STATE_FL_HEATERSTATUS);//0E160038 E502  E506

        if(heater_state)//电热开启采集点
        {
            heater = TRUE;
        }
        else
        {
            heater = FALSE;
        }
    }
    else
    {
        heater_state = VarAdrToInt(d_tempctrl1_STATE_FL_HEATERSTATUS); //0516002B

        if(heater_state)//电热开启采集点
        {
            heater = TRUE;

            if(VarAdrToInt(TEMP_CURVE)){
                heater = TRUE;
            }
            else{
                heater = FALSE;
            }
        }
        else
        {
            heater = FALSE;
        }
    }

    if(!heater) return;

    if(GetTick() - time_5s >= 5000)
    {
        if(time > (MAX_TEMPCURVE_POINT - 1))
        {
            time = 0;
            //memset(m_point,0,sizeof(m_point)); //20200827 数据满后不清空
            SetTempCurveRepaint(TRUE);
        }
        for(i = 0; i < TEMPCURVE_NUM; ++i)
        {
            m_point[i][time].x = time;
            //20200323
            if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
            {
                m_point[i][time].y = VarAdrToInt(d_TempCtrlComm1_STATE_DATEMP_REAL_2 + i);
            }
            else
            {
                m_point[i][time].y = VarAdrToInt(TEMP1_REAL + i);
            }
        }
        time++;
        SetTempCurveNum(time);
        time_5s = GetTick();
    }
}

//20250312 jhh 增加温度曲线2画面 温度曲线2取数据
void TempCurveGetData2()
{
    static UI32 time_5s = 0;
    static UI32 time = 0;
    static BOOL heater = FALSE;
    UI16 i;
    UI32 heater_state;

    if(GetSubPartNum(TEMPEXT_ID) < 2)
    {
        return;
    }

    //20200323
    if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
    {
        heater_state = VarAdrToInt(d_TempCtrlComm2_STATE_FL_HEATERSTATUS);//0E160038 E502  E506

        if(heater_state)//电热开启采集点
        {
            heater = TRUE;
        }
        else
        {
            heater = FALSE;
        }
    }
    else
    {
        heater_state = VarAdrToInt(d_tempctrl2_STATE_FL_HEATERSTATUS); //0516002B

        if(heater_state)//电热开启采集点
        {
            heater = TRUE;

            if(VarAdrToInt(TEMP_CURVE)){
                heater = TRUE;
            }
            else{
                heater = FALSE;
            }
        }
        else
        {
            heater = FALSE;
        }
    }

    if(!heater) return;

    if(GetTick() - time_5s >= 5000)
    {
        if(time > (MAX_TEMPCURVE_POINT - 1))
        {
            time = 0;
            //memset(m_point,0,sizeof(m_point)); //20200827 数据满后不清空
            SetTempCurve2Repaint(TRUE);
        }
        for(i = 0; i < TEMPCURVE_NUM; ++i)
        {
            m_point2[i][time].x = time;
            //20200323
            if(IV3200 || IV5200 || IV3300 || IV5300 || IV3100)
            {
                m_point2[i][time].y = VarAdrToInt(d_TempCtrlComm2_STATE_DATEMP_REAL_2 + i);
            }
            else
            {
                m_point2[i][time].y = VarAdrToInt(TEMP2_REAL + i);
            }
        }
        time++;
        SetTempCurve2Num(time);
        time_5s = GetTick();
    }
}

double TempCurveGetPointX(int curvenum,int pointnum)
{
    if(curvenum>=TEMPCURVE_NUM || pointnum>=MAX_TEMPCURVE_POINT)
    {
        return 0.0;
    }

    return m_point[curvenum][pointnum].x;
}

double TempCurveGetPointY(int curvenum,int pointnum)
{
    if(curvenum>=TEMPCURVE_NUM || pointnum>=MAX_TEMPCURVE_POINT)
    {
        return 0.0;
    }

    return m_point[curvenum][pointnum].y;
}

//20250312 jhh 增加温度曲线2画面
double TempCurve2GetPointX(int curvenum,int pointnum)
{
    if(curvenum>=TEMPCURVE_NUM || pointnum>=MAX_TEMPCURVE_POINT)
    {
        return 0.0;
    }

    return m_point2[curvenum][pointnum].x;
}

double TempCurve2GetPointY(int curvenum,int pointnum)
{
    if(curvenum>=TEMPCURVE_NUM || pointnum>=MAX_TEMPCURVE_POINT)
    {
        return 0.0;
    }

    return m_point2[curvenum][pointnum].y;
}

void SetTempCurveRepaint(BOOL isRepaint)
{
    tempcurve_repaint = isRepaint;
}

BOOL GetTempCurveRepaint()
{
    return tempcurve_repaint;
}

void SetTempCurve2Repaint(BOOL isRepaint)//20250312 jhh 增加温度曲线2画面
{
    tempcurve_2_repaint = isRepaint;
}

BOOL GetTempCurve2Repaint()
{
    return tempcurve_2_repaint;
}

void SetTempCurveNum(UI32 num)
{
    tempcurve_num = num;
}

UI32 GetTempCurveNum()
{
    return tempcurve_num;
}

//20250312 jhh 增加温度曲线2画面
void SetTempCurve2Num(UI32 num)
{
    tempcurve_2_num = num;
}

UI32 GetTempCurve2Num()
{
    return tempcurve_2_num;
}

//20190618.cyx 驱动器温度曲线取数据
void DriveTempCurveGetData()
{
    static UI32 time_5s = 0;
    static UI32 time = 0;
    static BOOL motorstatus = FALSE;
    UI16 i;

    if(VarAdrToInt(MOTOR_STATE_STATE))//马达开启采集点
    {
        motorstatus = TRUE;
    }
    else
    {
        motorstatus = FALSE;
    }

    if(!motorstatus) return;

    if(GetTick() - time_5s >= 5000)
    {
        if(time > (MAX_TEMPCURVE_POINT - 1))
        {
            time = 0;
        }
        for(i = 0; i < DRIVE_TEMPCURVE_NUM; ++i)
        {
            m_drivepoint[i][time].x = time;
            m_drivepoint[i][time].y = VarAdrToInt(SERVO_STATE_TEMP1 + i);
        }
        time++;
        SetDriveTempCurveNum(time);
        time_5s = GetTick();
    }
}

double DriveTempCurveGetPointX(int curvenum,int pointnum)
{
    if(curvenum>=DRIVE_TEMPCURVE_NUM || pointnum>=MAX_TEMPCURVE_POINT)
    {
        return 0.0;
    }

    return m_drivepoint[curvenum][pointnum].x;
}

double DriveTempCurveGetPointY(int curvenum,int pointnum)
{
    if(curvenum>=DRIVE_TEMPCURVE_NUM || pointnum>=MAX_TEMPCURVE_POINT)
    {
        return 0.0;
    }

    return m_drivepoint[curvenum][pointnum].y;
}

void SetDriveTempCurveRepaint(BOOL isRepaint)
{
    drivetempcurve_repaint = isRepaint;
}

BOOL GetDriveTempCurveRepaint()
{
    return drivetempcurve_repaint;
}

void SetDriveTempCurveNum(UI32 num)
{
    drivetempcurve_num = num;
}

UI32 GetDriveTempCurveNum()
{
    return drivetempcurve_num;
}


/***************************全电新曲线控件新增**************************************/ //20190925.cyx
BOOL CurvesStart_New(PPROT_DATREC_REQ_PKT cmd1,PDATSET_IND cmd2)
{
    int i;
    if(cmd1 == NULL || cmd2 == NULL)
        return FALSE;

    ProtReqRec(cmd1->rec_id,cmd1->trigger_id,cmd1->rec_period,cmd1->rec_total_ms,
               cmd1->ind_num,cmd2);

    for(i=0;i<cmd1->ind_num;i++)
    {
        oprintf("1 rec_id=%d,checkid=%d,pwgt.cmd2 part_id=0x%x,sub_ind=%x,offset=%x:\n",cmd1->rec_id,cmd1->trigger_id,cmd2[i].part_id,cmd2[i].sub_ind,cmd2[i].offset);
    }

    return TRUE;
}

//下位机的数据清除
BOOL CurvesClear_New(UI16 rec_id)
{
    return ProtRecOP(rec_id,OP_CLRRECORD);
}
