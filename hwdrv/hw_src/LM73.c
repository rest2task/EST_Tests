#include "Typedef.h"
#include "fsl_lpi2c.h"
#include "bsp_lpi2c.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "LM73.h"
#include "tempctrl.h"
#include "Cpld.h"

#define LM73_ADDR   0x92
#define NST112_ADDR 0x90
extern lpi2c_master_handle_t g_m_handle;
LM73_STRU				g_lm73; 			//用于LM73初始化 zf 20160927
uint16 I2C1_ADDRESS = LM73_ADDR;

/***********************************************************
  * @函数名称	LM73_write_register
  * @函数说明   写数据到LM73寄存器
  * @输入参数   writedata：要写入的数据，intorno：写入方式，1中断方式，0连续方式
  * @输出参数   无
  * @相关参数	reg_add:寄存器地址，reg_data:要写入的数据
  * @返回参数   无
 **********************************************************/ 
uint32_t LM73_write_register(uint8_t* writedata,uint8_t intorno)
{ 
  lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;
	LPI2C_MasterEnable(I2C_MASTER,true);
//  EEPROM_DEBUG_FUNC();
   if(HW_SURTEMPNEW == 0)
  	I2C1_ADDRESS = LM73_ADDR;
  else
  	I2C1_ADDRESS = NST112_ADDR;
  
  masterXfer.slaveAddress = I2C1_ADDRESS>>1;							//IIC从机地址 
  masterXfer.direction = kLPI2C_Write;								//写入数据
  masterXfer.subaddress = writedata[0];								//读取寄存器地址
  masterXfer.subaddressSize = 1;									//长度默认一个字节
  masterXfer.data = writedata+1;									//要写入的数据
  masterXfer.dataSize = 1;											//长度默认一个字节
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  if(intorno == 1)													//中断方式
	  reVal = LPI2C_MasterTransferNonBlocking(I2C_MASTER,&g_m_handle, &masterXfer);
  else																//等待延迟方式
	  reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);
  
  if (reVal != kStatus_Success)
  {
      return 1;
  }
  
  return 0;

}

/***********************************************************
  * @函数名称	Start_ReadLM73
  * @函数说明   LM73读取启动
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 **********************************************************/
void Start_ReadLM73()
{
	  lpi2c_master_transfer_t masterXfer = {0};
	  LPI2C_MasterEnable(I2C_MASTER,true);
	  //status_t reVal = kStatus_Fail;
	 // EEPROM_DEBUG_FUNC();
	
	  /* subAddress = ReadAddr, data = pBuffer 自从机处接收
		起始信号start + 设备地址slaveaddress(w 写方向) + 子地址subAddress + 
		重复起始信号repeated start + 设备地址slaveaddress(r 读方向) + 
		接收缓冲数据rx data buffer + 停止信号stop */
	if(HW_SURTEMPNEW == 0)
		I2C1_ADDRESS = LM73_ADDR;
	else
		I2C1_ADDRESS = NST112_ADDR;
	  masterXfer.slaveAddress = I2C1_ADDRESS>>1;
	  masterXfer.direction = kLPI2C_Read;
	  masterXfer.subaddress = 0;										//要读数据的寄存器地址
	  masterXfer.subaddressSize = 0;
	  masterXfer.data = g_lm73.read_buf;								//数据缓冲区
	  masterXfer.dataSize = 2;											//读取数据的个数
	  masterXfer.flags = kLPI2C_TransferDefaultFlag;
	//	reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);	//I2C_MASTER
	  LPI2C_MasterTransferNonBlocking(I2C_MASTER,&g_m_handle, &masterXfer);//I2C_MASTER
}
		

/***********************************************************
  * @函数名称   ReadValid_LM73
  * @函数说明   读取LM73的温度，并转化成温度*10
  * @输入参数   无
  * @输出参数   无
  * @返回参数   temp: 温度
 **********************************************************/
WORD ReadValid_LM73(void)
{
    DWORD temp;
    UI16 tmp_temp = 0;
    
    temp=g_lm73.read_buf[0];
    temp=(temp<<8);
    temp= (temp|g_lm73.read_buf[1]);   
	
	
	if(HW_SURTEMPNEW == 1)
	{
		if(temp&0x8000) 								//负温度
		{
			temp = ~temp;
			temp = ((temp + 1) & 0x7fff) >> 4;
			temp = (WORD)(temp * 6250 / 10000); 		//为保留小数点后一位，故乘10处理	 0.03125 * 10
			temp = (WORD)(temp | 0x8000);				//负温度把最高位置1
		}
		else
		{
			temp = (temp >> 4);
			temp = (WORD)(temp * 6250 / 10000); 		//为保留小数点后一位，故乘10处理
		}

	}
	else
	{
		if(temp&0x8000)                                    //负温度
		{
			temp = ~temp;
			temp = ((temp + 1) & 0x7fff) >> 2;
			temp = (WORD)(temp * 3125 / 10000);            //为保留小数点后一位，故乘10处理     0.03125 * 10
			temp = (WORD)(temp | 0x8000);                  //负温度把最高位置1
		}
		else
		{
			temp = (temp >> 2);
			temp = (WORD)(temp * 3125 / 10000);            //为保留小数点后一位，故乘10处理
		}
	}

    if(temp&0x8000)
    {
        tmp_temp = temp&0x7FFF;
        if(tmp_temp > 100)  //室温不可以低于-10度
            tmp_temp = 100;
        temp = tmp_temp | 0x8000;
    }
    else
    {
        if(temp > 600)  //室温不可以高于60度
            temp = 600;
    }
    
    return (WORD)temp;
}

/**********************************************************************
  * @函数名称	Get_ValueLM73
  * @函数说明   得到温度
  * @输入参数   无
  * @输出参数   无
  * @返回参数   1,0
 *********************************************************************/
BOOL Get_ValueLM73_HAL(void)
{
	if (g_lm73.TEMP_NEW ==1)
	{
		g_ambient.curambient = ReadValid_LM73();
		g_lm73.TEMP_NEW = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
WORD ReadI2C2(void)
{
	DWORD temp;
	Start_ReadLM73();
    temp=g_lm73.read_buf[0];
    temp=(temp<<8);
    temp= (temp|g_lm73.read_buf[1]);    
	
	
	if(HW_SURTEMPNEW == 1)
	{
		if(temp&0x8000)									//负温度
		{
			temp = ~temp;
			temp = ((temp + 1) & 0x7fff) >> 4;
			temp = (WORD)(temp * 6250 / 10000);			//为保留小数点后一位，故乘10处理	 0.03125 * 10
			temp = (WORD)(temp | 0x8000);				//负温度把最高位置1
		}
		else
		{
			temp = (temp >> 4);
			temp = (WORD)(temp * 6250 / 10000);			//为保留小数点后一位，故乘10处理
		}	
	}
	else
	{
		if(temp&0x8000)                                    //负温度
		{
			temp = ~temp;
			temp = ((temp + 1) & 0x7fff) >> 2;
			temp = (WORD)(temp * 3125 / 10000);            //为保留小数点后一位，故乘10处理     0.03125 * 10
			temp = (WORD)(temp | 0x8000);                  //负温度把最高位置1
		}
		else
		{
			temp = (temp >> 2);
			temp = (WORD)(temp * 3125 / 10000);            //为保留小数点后一位，故乘10处理
		}		
	}
   	g_ambient.curambient = ReadValid_LM73();
    return (WORD)temp;	
}


void I2C_STOPBit(void)
{
//	LPI2C_MasterStop(I2C_MASTER);
}

void LM73Config(void)
{
	uint8_t send_buf[2];
	uint8_t *send_P;
	
	if(HW_SURTEMPNEW == 0)
	{
		send_buf[0] = 0x01;
		send_buf[1] = 0x60;
		send_P = send_buf;
		LM73_write_register(send_P,0);
		
		send_buf[0] = 0x04;
		send_buf[1] = 0x60;												//设置精度
		send_P = send_buf;
		LM73_write_register(send_P,0);
		
		send_buf[0] = 0x00;												//地址0x00，温度AD码地址
		send_P = send_buf;
		LM73_write_register(send_P,0);
	}
    LPI2C_MasterEnable(I2C_MASTER,false);
}

/***********************************************************
  * @函数名称	LM73WriteReset
  * @函数说明   LM73配置寄存器设置，使其处于关断模式
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 **********************************************************/
void LM73WriteReset(void)
{
	uint8_t send_buf[2];
	uint8_t *send_P;
	
	if(HW_SURTEMPNEW == 0)
	{	
		send_buf[0] = 0x01;
		send_buf[1] = 0xE0;
		send_P = send_buf;
		g_lm73.WRITE_SUCCESS = 0;
		LM73_write_register(send_P,1);	
	}
	else  // I2C 上下电，重新配置时，发现需要写4次 才会成功。由于NST不需要配置，就改成直接读取的方式来实现写4次
	{
		g_lm73.WRITE_SUCCESS = 0;
		Start_ReadLM73();
	}
}	

/***********************************************************
  * @函数名称	LM73WriteConfig
  * @函数说明   LM73配置寄存器设置，启动正常操作
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 **********************************************************/
void LM73WriteConfig(void)
{
	uint8_t send_buf[2];
	uint8_t *send_P;
	
	if(HW_SURTEMPNEW == 0)
	{	
		send_buf[0] = 0x01;
		send_buf[1] = 0x60;
		send_P = send_buf;
		LM73_write_register(send_P,1);	
	}
	else
	{
		g_lm73.WRITE_SUCCESS = 0;
		Start_ReadLM73();
	}
}	

/***********************************************************
  * @函数名称	LM73WriteControl
  * @函数说明   LM73配置寄存器设置，设置分辨率
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 **********************************************************/
void LM73WriteControl(void)
{
	uint8_t send_buf[2];
	uint8_t *send_P;
	
	if(HW_SURTEMPNEW == 0)
	{	
		send_buf[0] = 0x04;
		send_buf[1] = 0x60;
		send_P = send_buf;
		LM73_write_register(send_P,1);	
	}
	else
	{
		g_lm73.WRITE_SUCCESS = 0;
		Start_ReadLM73();
	}	
}	

/***********************************************************
  * @函数名称	LM73WriteAddr
  * @函数说明   LM73当前寄存器地址设置
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
 **********************************************************/
void LM73WriteAddr(void)
{
	uint8_t send_buf[2];
	uint8_t *send_P;
	
	if(HW_SURTEMPNEW == 0)
	{
		send_buf[0] = 0x00;
		send_buf[1] = 0x00;
		send_P = send_buf;
		LM73_write_register(send_P,1);	
	}
	else
	{
		g_lm73.WRITE_SUCCESS = 0;
		Start_ReadLM73();
	}
}	



