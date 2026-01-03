#ifndef _MIDHW_H_
#define	_MIDHW_H_

#include "typedef.h"
#include "bsp_key_it.h"
#include "Board.h"
#include "cpld.h"
#include "Wdog.h"
#include "LM73.h"
#include "useriap.h"

#define HWGetAdValue		m_get_adhw
#define	GetADHW_Y209		m_get_adhwf508
#define HWDASet				m_set_dahw

//输入输出点接口
#define HW_PI_IN_0			port3000			
#define HW_PI_IN_1			port3001	
#define HW_PI_IN_2			port3002   
#define HW_PO_OUT_0			port4000
#define HW_PO_OUT_1			port4001
#define HW_PO_OUT_2			port4002
#define HW_PO_OUT_3			port4003

//急停键接口
#define RESETKEYIDR 	GPIO_PinRead(RESET_KEY_GPIO, RESET_KEY_GPIO_PIN)
//手动键接口
#define	MANUALKEYIDR 	GPIO_PinRead(MANUAL_KEY_GPIO, MANUAL_KEY_GPIO_PIN)
//主机重启接口
#define HWRESET_SET		INRST_KEY_GPIO->DR &= ~(1u << INRST_KEY_GPIO_PIN)
//CPLD延迟
#define CPLD_WAIT()		cpld_acc_wait()


void m_change_channels(int T_Channels);
void m_start_temp_sampling(uint8_t T_Channels) ;
WORD m_read_temp_counter(void) ;
void m_clear_temp_counter(void);
void m_ad_start(void);
void m_extad_start(void);
UI8 m_get_adrang_5v(void);
UI8 m_get_adrang_10v(void);
WORD m_get_adhw(int portno);
void m_da_start(uint8_t sel, uint16_t val);
void m_enable_daccurrent_out(void);
void m_diable_daccurrent_out(void);
BOOL m_da_protect(void);
void m_io_check_hv(void);
void m_uart_send_byte(LPUART_Type *base, uint8_t data);
void m_uart_disable_tx(void);
void m_uart_enable_tx(void);
void m_uart_485_disable_tx(void);
void m_uart_485_enable_tx(void);
void m_can_init_device(unsigned char Ch, unsigned long Baud);
void m_can2_init_device(unsigned long Baud);
BOOL m_can2_tx_compltet(void);
void m_can_receive_block(void);
void m_I2C_LM73_Init(void);
void m_I2C_LM73_WriteReset(void);
void m_I2C_LM73_WriteConfig(void);
void m_I2C_LM73_WriteControl(void);
void m_I2C_LM73_WriteAddr(void);	
void m_start_readlm73(void);
BOOL m_get_lm73value(void);
WORD m_read_validlm73(void);
BOOL m_clr_flag(void);
int m_program_erase(uint32 DstAddr);
BOOL  m_program_iap(uint32_t SrcAddr, uint32_t DstAddr,uint32_t len);
void m_on_run_led(void);
void m_off_run_led(void);
void m_on_uart_led(void);
void m_off_uart_led(void);
void m_on_cana_led(void);
void m_off_cana_led(void);
void m_on_canb_led(void);
void m_off_canb_led(void);
UI16 m_get_hwtype(void);
BOOL m_cpld_test_card(void);
void HwInitial(void);
WORD m_get_adhwf508(int portno);
void m_set_dahw(int portno, WORD value);
WORD m_read_I2C2(void);

void m_Udp_init(void);
void m_Udp_ISRHandler(void);
void m_lwip_pkt_handle(void);
UI32 m_udp_request_finished(void);
void m_udp_senddata(UI8* databuf ,int len);
UI8* m_get_udpdata_adr(void);
void m_lwip_periodic_handle(void);
BOOL m_get_udpdata_flag(void);
void m_clear_udpdata_flag(void);
UI16 m_get_udplen(void);
void m_set_udplen(UI16 len);
void m_dm_init(void);
void m_lm73_poweron(void);
void m_lm73_poweroff(void);





#endif
