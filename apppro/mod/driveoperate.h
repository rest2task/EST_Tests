#ifndef _DRIVEOPERATE_h
#define _DRIVEOPERATE_h

#ifdef __cplusplus
extern "C"
{ 
#endif

#include "mod.h"
#include "tablectl.h"
#include "protcmd.h"

#define UPDATE_DRIVEPARA    0x331700BC //标记-用于刷新驱动器参数请求
#define READ_PACK           0x331700BD //读当前驱动器参数总包数
#define WRITE_PACK          0x331700BE //写当前驱动器参数总包数
#define CUR_PACKTYPE        0x331700BF //包号类型-用于区别当前请求的包属于哪个画面
#define CUR_OPERATETYPE     0x331700C0 //增加本地导入导出功能 0:无导入导出操作；1:U盘导入导出；2:本地面板导入导出

#define         COMM_SERVO_REQ       0x101   //读驱动器参数
#define         COMM_SERVO_RSP       0x102   //写驱动器参数

//驱动器参数交互
#define MAX_PACKFORTABLE    80  //80包*sizeof（TBL_DRIVEPAR）
#define NOOPERATEPAR        0
#define READDRIVEPAR        1
#define WRITEDRIVEPAR       2
#define PACKNULL            0//包号类型，用于区别当前请求的包属于哪个画面
#define PACKSERVOPARAM      1//包号类型，用于区别当前请求的包属于哪个画面
#define PACKSERVOOPERATE    2//包号类型，用于区别当前请求的包属于哪个画面
#define PACKSINGLEDRIVEPAR  3//包号类型，用于区别当前请求的包属于哪个画面
#define PACKSERVOPARAM2     4//包号类型，用于区别当前请求的包属于哪个画面 驱动器参数—伺服参数2
//本地导入导出功能 0:无导入导出操作；1:U盘导入导出；2:本地面板导入导出
#define OPTNULL             0
#define UDISKOPT            1
#define LOCALPANELOPT       2
#define MAX_DRIVETABLE      80 //80包*sizeof（TBL_DRIVEPAR）

/**
 * @brief         : 删除哈希表中对应类型的数据表
 *
 * @param      : 表类型（读/写）
 * @return      :
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */
void RemoveDriveTable(int tbltype);

/**
 * @brief         : 将对应的数据表插入到哈希表中
 *
 * @param      : 表类型（读/写）
 * @param      : 指向该表的结构体指针
 * @return      :
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */
void AddDriveTable(int type, PRO_SDO_RX_LOG *ptableDrive);

/**
 * @brief         : 找到哈希表中对应的数据表
 *
 * @param      : 表类型（读/写）
 * @return      :
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */
PRO_SDO_RX_LOG*  FindDriveTable(int type);

/**
 * @brief         : 得到当前数据表中的行数
 *
 * @param      :
 * @return      :
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */
UI16 GetPackCount();

/**
 * @brief         : 保存驱动器数据到U盘
 *
 * @param      :
 * @param      :
 * @return      :0 失败 1 成功 2 数据有空项
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */
UI16 SaveDriveDataToExcel(char filename[]);
/**
 * @brief         : 保存驱动器数据到面板
 *
 * @param      : 文件名称
 * @param      :
 * @return      :
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */
BOOL SaveDriveDataToExcel_Local(char filename[]);

/**
 * @brief         : 将csv格式的驱动器数据表读入到U盘
 *
 * @param      : 文件名称
 * @param      : 表类型（读/写）
 * @param      :
 * @return      :0 失败 1 成功 2 数据有空项
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */

UI16 LoadDriveData(char filename[],int type);

/**
 * @brief         : //将csv格式的驱动器数据表读入到面板中
 *
 * @param      : 文件名称
 * @param      : 表类型（读/写）
 * @param      : 第几个驱动器
 * @return      :
 * @retval       :
 * @note        :
 * @attention :
 * @author     : 作者（YX）
 * @date        : 日期（20191120）
 */
BOOL LoadDriveData_Local(char filename[],int type,UI16 wSelSn);

#ifdef __cplusplus
}
#endif

#endif



