#include "selfstudy.h"
#include "database.h"

/**
 * @brief      : 卓越自学习数据接收
 * @param      :
 * @return     :
 * @retval     :
 * @note       :
 * @attention  :
 * @author     : cyx
 * @date       : 20200622
 */
int RxCmd_Study(UI16* pdata)
{
    if(pdata[0] == 0x02)//自学习步骤命令
    {
        if(pdata[1] >= 1)//数据长度
        {
            VarAdrSetInt(p_PP_MACHSET_STUDY_STEP, pdata[2]);//动作步骤
        }

        if(pdata[1] >= 2)//数据长度
        {
            if(pdata[2] == 0x50)
            {
                if(pdata[3] >= 100)
                {
                    VarAdrSetInt(p_PP_MACHSET_STUDY_PROCESS, 100);
                }
                else
                {
                    VarAdrSetInt(p_PP_MACHSET_STUDY_PROCESS, pdata[3]);//动作进度条
                }
            }
            else if(pdata[2] == 0x60)
            {
                VarAdrSetInt(p_PP_MACHSET_STUDY_ENERGY, pdata[3]);//动能转换效率
            }
            else if(pdata[2] == 0x61)
            {
                VarAdrSetInt(p_PP_MACHSET_STUDY_ENERGY2, pdata[3]);//动能转换效率
            }
        }

        if(pdata[1] >= 3)//数据长度
        {
            if(pdata[2] == 0x61)
            {
                VarAdrSetInt(p_PP_MACHSET_STUDY_DEADTM2, pdata[4]);//死区时间
            }
            else
            {
                VarAdrSetInt(p_PP_MACHSET_STUDY_DEADTM, pdata[4]);//死区时间
            }
        }

        if(pdata[1] >= 4)//数据长度
        {
            if(pdata[2] == 0x61)
            {
                VarAdrSetInt(p_PP_MACHSET_STUDY_RAMP2, pdata[5]);//最大斜率
            }
            else
            {
                VarAdrSetInt(p_PP_MACHSET_STUDY_RAMP, pdata[5]);//最大斜率
            }
        }
    }
    return 0;
}
