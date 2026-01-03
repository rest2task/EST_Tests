#include "fsl_flexspi.h"
#include "types.h"
#include "typedef.h"
#define EXAMPLE_FLEXSPI FLEXSPI
#define FLASH_SIZE 0x1000 /* 64Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE 256
#define EXAMPLE_SECTOR 20
#define SECTOR_SIZE 0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK kCLOCK_FlexSpi


#define NOR_CMD_LUT_SEQ_IDX_READID1             10
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS1         11
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1        12
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR1        13
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1 14
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1				15

#define FLASH_BUSY_STATUS_POL 1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK 0x0e


/*******************************************************************************
* Prototypes
******************************************************************************/
extern void   FLEXSPI_UpdateLUT(FLEXSPI_Type *base, uint32_t index, const uint32_t *cmd, uint32_t count);
/*******************************************************************************
 * Variables
 ******************************************************************************/

extern flexspi_device_config_t deviceconfig;
extern const uint32_t customLUT[];


status_t   flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr);
status_t  flexspi_nor_wait_bus_busy(FLEXSPI_Type *base);
status_t   flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
status_t   flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
void   FlexSPI_FlashRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
status_t   flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);
BOOL   flexspi_nor_bus_check_busy(FLEXSPI_Type *base);
BOOL   wait_erase_ok(void);

void FLEXSPI_test_Init(void);

BOOL update_Information(uint32_t iap_dstaddr, uint16_t *iap_data, uint16_t f_offset);

BOOL  Erase_Flash(uint32 DstAddr);
BOOL  Program_IAP_hw(uint32_t SrcAddr, uint32_t DstAddr,uint32_t len);

