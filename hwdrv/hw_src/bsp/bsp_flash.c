#include "fsl_flexspi.h"
#include "bsp_flash.h"
#include "fsl_debug_console.h"
#include "common.h"
#include "cfgdb.h"
#include "hwmid.h"
/*******************************************************************************
* Prototypes
******************************************************************************/
extern void  FLEXSPI_UpdateLUT(FLEXSPI_Type *base, uint32_t index, const uint32_t *cmd, uint32_t count);
/*******************************************************************************
 * Variables
 ******************************************************************************/

extern flexspi_device_config_t deviceconfig;
extern const uint32_t customLUT[];



/*******************************************************************************
 * Code
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk = 120000000,
    .flashSize = FLASH_SIZE,
    .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval = 2,
    .CSHoldTime = 3,
    .CSSetupTime = 3,
    .dataValidTime = 0,
    .columnspace = 0,
    .enableWordAddress = 0,
    .AWRSeqIndex = 0,
    .AWRSeqNumber = 0,
    .ARDSeqIndex = 0,//Normal Read
    .ARDSeqNumber = 1,
    .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

/* Fixed 64 LUTs, suggest to reserve the LUTs in the front */
const uint32_t customLUT[64] = {
        /* Read ID */
        [4 * NOR_CMD_LUT_SEQ_IDX_READID1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xAB, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READID1 + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
        /* Read extend parameters */
        [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
        /* Write Enable */
        [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
        /* Erase Sector  */
        [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        /* Page Program - single mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1 + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
		    /* Normal read mode -SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1 + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
};

BOOL   wait_erase_ok(void)
{
	BOOL status;
	//__disable_irq();										 				//关中断
	//SCB_DisableDCache();													//关闭数据缓冲
	for ( int ix = 0; ix<10000;ix++)
	{
		;
	}
	status = flexspi_nor_bus_check_busy(FLEXSPI);
	//__enable_irq();								   							//开中断
	
	if (status)
		return(0);
	else
		return(1);
}

status_t   flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write neable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

BOOL   flexspi_nor_bus_check_busy(FLEXSPI_Type *base)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUS1;
    flashXfer.data = &readValue;
    flashXfer.dataSize = 4;

        status = FLEXSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
			status = 1;
            return status;
        }
        if (FLASH_BUSY_STATUS_POL)
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

	
		
    return isBusy;
}

status_t   flexspi_nor_wait_bus_busy(FLEXSPI_Type *base)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUS1;
    flashXfer.data = &readValue;
    flashXfer.dataSize = 4;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL)
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }
		
    } while (isBusy);
	
    return status;
}

status_t   flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR1;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }
	
    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

status_t   flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src)
{
    status_t status;
    flexspi_transfer_t flashXfer;
	

    /* Write neable */
		status = flexspi_nor_wait_bus_busy(base);
    status = flexspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1;
    flashXfer.data = (uint32_t *)src;
    flashXfer.dataSize = FLASH_PAGE_SIZE;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

//从FlexSPI Flash中读取数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(最大32bit)
//NumByteToRead:要读取的字节数(最大65535)
void   FlexSPI_FlashRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)  
{
    u32 index=0;
    u32 startaddr=0;
    
    startaddr=FlexSPI_AMBA_BASE+ReadAddr;

    //复位AHB缓冲区，一定要复AHB缓冲区，否则数据连续读取出错！
    FLEXSPI_SoftwareReset(FLEXSPI);     
    for(index=0;index<NumByteToRead;index++)
    {
        pBuffer[index]=*(u8*)(startaddr+index);
    }
}

#if 0
status_t   flexspi_nor_flash_read_sector(FLEXSPI_Type *base, uint32_t address,const uint32_t *src,size_t leng)
{
	//uint32_t temp;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1;
    flashXfer.data = (uint32_t *)src;
    flashXfer.dataSize = leng;

    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

   // *vendorId = temp;

    return status;
}
#endif
status_t  flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId)
{
    uint32_t temp;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READID1;
    flashXfer.data = &temp;
    flashXfer.dataSize = 1;

    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *vendorId = temp;

    return status;
}

void FLEXSPI_test_Init(void)
{	  
	status_t status;	
    uint8_t vendorID = 0;
    /* Update LUT table. */
    FLEXSPI_UpdateLUT(EXAMPLE_FLEXSPI,
												NOR_CMD_LUT_SEQ_IDX_READID1*4,
													&customLUT[NOR_CMD_LUT_SEQ_IDX_READID1*4], 64);
    /* Get vendor ID. */
    status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
//		PRINTF("Get Vendor ID Failure!");
        assert(0);//return status;
    }
//    PRINTF("Vendor ID: 0x%x\r\n", vendorID);

}




/*******************************************************************************
  Function:      update_Information						// 函数名称
  Description:   更新信息交互区	（读出第6个WORD，加1进行写录校验			// 函数功能、性能等的描述
  Input:		 iap_dstaddr ---- 目标地址			// 输入参数说明，包括每个参数的作
  				 *iap_data   ---- 要写入的数据(数组名称）
				 f_offset    ----  用于校验的WORD的偏移地址（目前定为第6个WORD）
                  									// 用、取值说明及参数间关系。
  Output:        无  								// 对输出参数的说明。
  Return:        status 1:操作成功，0：操作失败		// 函数返回值的说明
  Others:        									// 其它说明
*******************************************************************************/
BOOL update_Information(uint32_t iap_dstaddr, uint16_t *iap_data, uint16_t f_offset)
{
	uint16_t writedata;
	uint16_t *p_temp;
	__align(4) uint8_t buf[512];
	uint8_t i = 0;
	
	p_temp = (uint16_t *)iap_dstaddr;
	writedata = (uint16_t)(*(p_temp + f_offset));					//读取数据
	writedata = (uint16_t)(writedata + 1);
	p_temp = iap_data;
	
	for (i = 0; i < 64; i++)
	{																											 
		buf[2*i] = (uint8_t)(*p_temp);
		buf[2*i+1] = (uint8_t)((*p_temp) >> 8);
		p_temp++;
	}
	
	buf[12] = (uint8_t)writedata;
	buf[13] = (uint8_t)(writedata >> 8);

	if (Erase_Flash(iap_dstaddr))
	{
		if (m_program_iap((uint32_t)buf, (uint32_t)iap_dstaddr, 512))
			return (TRUE);
		else
			return (FALSE);
	}
	else
	return (FALSE);
}

/* 擦除一个扇区4k byte,只发送了擦除命令，因为M7擦除时间过长，影响应用程序运行，需要跟wait_erase_ok配合才能确保擦除完成*/
BOOL  Erase_Flash(uint32 DstAddr)
{
  uint8_t  try_count;
  status_t status;

  if ((DstAddr <= 0x607FFFFF) && (DstAddr > 0x60020000))  	//外部的flash地址
  {
	  __disable_irq();													  //关中断
	  SCB_DisableDCache();												  //关闭数据缓冲
  
	
		if((DstAddr % 0x1000) == 0) 							//只有一个扇区的开始才执行擦除动作，防止重复擦除
		{
			try_count = 0;
			do
			{
				status = flexspi_nor_flash_erase_sector(FLEXSPI,DstAddr-0x60000000);	//擦除扇区	
				try_count++;
				if(try_count > 5)
				{
					return(0);
				}
			}
			while (status != kStatus_Success);							//擦除成功
		}
		__enable_irq();															//开中断
		SCB_EnableDCache();														//打开数据缓冲
		return(1);

	}
	return(1);
}

/*******************************************************************************
  Function:      Program_IAP_hw						// 函数名称
  Description:   写FLASH
  Input:		 DstAddr ---- 目标地址（FLASH）			
  				 SrcAddr ---- 源地址（RAM）
				 len    ----  数据长度，以byte为单位，最小大小为256字节
                  																	
  Return:        status 1:操作成功，0：操作失败		      									
*******************************************************************************/
BOOL  Program_IAP_hw(uint32_t SrcAddr, uint32_t DstAddr,uint32_t len)
{	
	uint32_t i;
	status_t status;
	uint16_t SIZE_PAGE_WR, iap_run;
	u8 flash_read_buf[256];
	
	len <<= 2;
	SIZE_PAGE_WR = 0x100;													//单次写入长度

	if (((DstAddr + len) <= 0x607FFFFF) && ((DstAddr + len) > 0x60020000) && (SrcAddr + len) < 0x20280000)	//外部的flash地址
	{
		__disable_irq();
		SCB_DisableDCache();												//关闭数据缓冲	

		iap_run = len / SIZE_PAGE_WR;
		if(len % SIZE_PAGE_WR)
			iap_run++;			
		for ( i = 0; i < iap_run; i++ )
		{

			status = flexspi_nor_flash_page_program(FLEXSPI,DstAddr-0x60000000+i*SIZE_PAGE_WR, (void *)(SrcAddr+i*SIZE_PAGE_WR)); 
			if(status == kStatus_Success)									//烧写成功
			{
				FlexSPI_FlashRead(flash_read_buf,DstAddr-0x60000000+i*SIZE_PAGE_WR,SIZE_PAGE_WR);

				if (memcmp((void *)(SrcAddr+i*SIZE_PAGE_WR), flash_read_buf, SIZE_PAGE_WR))
				{
					__enable_irq();											//开启所有中断
					SCB_EnableDCache(); 									//打开数据缓冲
					return(0);
				}
			}
			else
			{
				__enable_irq();												//开启所有中断
				SCB_EnableDCache(); 										//打开数据缓冲
				return(0);				
			}
		}
		__enable_irq();
		SCB_EnableDCache(); 												//打开数据缓冲

		return (1); 														//写成功


	}
	else
	{
		return (0);															//操作失败
	}
}

