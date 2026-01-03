#ifndef _STANDARDCURVB_h
#define _STANDARDCURVB_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "lightgraphwgt.h"

/**
 * @brief      : 标准曲线保存
 *
 * @param      : datalength数组大小为4个，分别传入含有曲线数据的指针，curve_id代表曲线编号
 * @return     : 是否成功保存
 * @retval     : 成功返回TRUE，失败返回FALSE
 * @note       : 先写入四条曲线的长度，在分别写入各条曲线的数据
 * @attention  : datalength数组大小为4个，分别表示4条曲线的长度
 * @author     : cyx
 * @date       : 20191024
 */
BOOL SaveStandardCurve(UI32 datalength[],LDPoint * src1,LDPoint * src2,LDPoint * src3,LDPoint * src4,int curve_id);

/**
 * @brief      : 加载保存的标准曲线
 *
 * @param      : pwgt表示各条曲线代码的控件，curve_id代表曲线编号
 * @return     : 无
 * @retval     : 无
 * @note       : 如果存在对应的保存文件，则加载相应数据
 * @attention  : 保存后需重新加载一次
 * @author     : cyx
 * @date       : 20191024
 */
void LoadStandardCurve(PWGT pwgt1,PWGT pwgt2,PWGT pwgt3,PWGT pwgt4,int curve_id);

/**
 * @brief      : 标准曲线保存
 *
 * @param      : datalength数组大小为curve_num个，分别传入含有曲线数据的指针，curve_id代表曲线编号
 * @return     : 是否成功保存
 * @retval     : 成功返回TRUE，失败返回FALSE
 * @note       : 先写入各条曲线的长度，在分别写入各条曲线的数据
 * @attention  : datalength数组大小为curve_num个，分别表示curve_num条曲线的长度
 * @author     : cyx
 * @date       : 20191226
 */
BOOL SaveSpecStandardCurve(UI32 datalength[],LDPoint * src[],int curve_num,int curve_id);

/**
 * @brief      : 加载保存的标准曲线
 *
 * @param      : pwgt表示各条曲线代码的控件，curve_id代表曲线编号
 * @return     : 无
 * @retval     : 无
 * @note       : 如果存在对应的保存文件，则加载相应数据
 * @attention  : 保存后需重新加载一次
 * @author     : cyx
 * @date       : 20191226
 */
void LoadSpecStandardCurve(PWGT pwgt[],int curve_num,int curve_id);

BOOL ExportInjectStandardCurve();
BOOL ExportChargeStandardCurve();
BOOL ExportOpenStandardCurve();
BOOL ExportCloseStandardCurve();
BOOL ExportInjectPosStandardCurve();
BOOL ExportHoldStandardCurve();

int StrAdrToInt(UI32 adr);

#define OPN_STANDARDCURVE_SAVEPATH SAVE_PATH"opnstandardcurve.dat"
#define CLS_STANDARDCURVE_SAVEPATH SAVE_PATH"clsstandardcurve.dat"
#define CHG_STANDARDCURVE_SAVEPATH SAVE_PATH"chgstandardcurve.dat"
#define INJ_STANDARDCURVE_SAVEPATH SAVE_PATH"injstandardcurve.dat"
#define EJE_STANDARDCURVE_SAVEPATH SAVE_PATH"ejestandardcurve.dat"

//射出曲线选择位置模式
#define G_INJECT_HOLD_TM    128 //仅用于区分保存路径
#define G_INJECT_INJ_POS    228 //仅用于区分保存路径
#define INJ_HOLD_TM_STANDARDCURVE_SAVEPATH SAVE_PATH"injholdtmstandardcurve.dat"
#define INJ_INJ_POS_STANDARDCURVE_SAVEPATH SAVE_PATH"injinjposstandardcurve.dat"

#ifdef __cplusplus
}
#endif

#endif



