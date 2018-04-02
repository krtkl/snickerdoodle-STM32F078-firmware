/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_platform.h
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	Platform Controller Functions 
 * @copyright	FreeBSD
 *
 *******************************************************************************
 *
 * Copyright (c) 2016, krtkl inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are 
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the FreeBSD Project.
 *
 *******************************************************************************
 */

#ifndef __SD_PLATFORM_H
#define __SD_PLATFORM_H
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup SD_Platform
 * @{
 */

/**
 * @defgroup SD_Platform_Defines Platform Definitions
 * @{
 */
#define CONS_UART_RX_BUFF_SIZE  128		/**< Console buffer size */

  
/**
 * @defgroup SD_Antenna_Select WL18xx Antenna Selection
 * @{
 */
#define WL18xx_ANTENNA_SMA	((uint8_t)0x01)	/**< Use SMA jack antenna */
#define WL18xx_ANTENNA_CHIP	((uint8_t)0x02)	/**< Use chip antenna */
/**
 * @}
 */
  
/**
 * @defgroup SD_DFU_Defines Device Firmware Upgrade
 * @{
 */
#define BOOT_STACK_ADDR		0x200014A8	/**< Bootloader stack address */
#define SYS_MEM_START_ADDR	0x1FFFC800	/**< System memory start address */
/**
 * @}
 */
  

/**
 * @brief  Check that the antenna selection is valid
 * @param  __MODE__: The antenna mode selection
 * @retval TRUE if __MODE__ is valid, FALSE otherwise
 */
#define IS_WL18xx_ANTENNA_MODE(__MODE__)  (((__MODE__) == WL18xx_ANTENNA_SMA) || \
					   ((__MODE__) == WL18xx_ANTENNA_CHIP))


/**
 * @brief  Toggle a single bit within a register
 * @param  \__REG\__:  Register to toggle bit
 * @param  \__BIT\__:  Bit (0 index) to toggle
 * @retval New register value with the bit toggled
 */
#define TOGGLE_BIT(__REG__, __BIT__)    ((__REG__ & (1 << __BIT__)) ? \
					 (__REG__ & ~(1 << __BIT__)) : \
					 (__REG__ | (1 << __BIT__)))

/**
 * @}
 */


/**
 * @defgroup SD_Platform_Exported_Variables Platform Exported Variables
 * @{
 */
extern struct sd_cbuf uart1_rx_cbuff;		/**< UART1 circular buffer */
extern struct sd_cbuf uart2_rx_cbuff;		/**< UART2 circular buffer */

extern struct sd_doub_buff cons_dbuff;			/**< Console double buffer */

extern uint8_t uart1_rx_buff[CONS_UART_RX_BUFF_SIZE];	/**< UART1 receive character buffer */
extern uint8_t uart2_rx_buff[CONS_UART_RX_BUFF_SIZE];	/**< UART2 receive character buffer */

extern uint8_t cons_buff[2][CONS_UART_RX_BUFF_SIZE];	/**< Console double buffer */

//extern struct sd_hci_dev wl18xxbt_dev;			/**< Bluetooth HCI device layer */

extern uint8_t hci_params_buff[CONS_UART_RX_BUFF_SIZE];	/**< Bluetooth HCI parameter buffer */
/**
 * @}
 */

/**
 * @addtogroup SD_UART_USB_Bridge
 * @{
 */
  
/**
 * @defgroup SD_UART_USB_Bridge_Types UART USB Bridge Types
 * @{
 */
  
/**
 * @enum sd_uart_usb_err
 * @brief UART to USB bridge error enumeration
 */
enum sd_uart_usb_err {
	SD_UART_USB_SUCCESS,			/**< Successfull execution */
	SD_UART_USB_ERROR			/**< Execution failure */
};
/**
 * @}
 */

/**
 * @}
 */


/**
 * @enum sd_zynq_boot 
 * @brief Zynq boot pin enumeration
 */
enum sd_zynq_boot {
	SD_ZYNQ_BOOT_JTAG = 0x00,		/**< JTAG boot */
	SD_ZYNQ_BOOT_NAND = 0x01,		/**< NAND flash boot */
	SD_ZYNQ_BOOT_QSPI = 0x02,		/**< Quad SPI flash boot */
	SD_ZYNQ_BOOT_SD = 0x03			/**< SD card boot */
};



/**
 * @enum sd_boot_button
 * @brief Boot button press state
 */
enum sd_boot_button {
	SD_BOOT_DFU = 0x00,			/**< Default boot */
	SD_BOOT_QSPI = 0x01,			/**< Boot from QPSI */
	SD_BOOT_JTAG = 0x02,			/**< Boot using JTAG */
	SD_BOOT_DEFAULT = 0x03,			/**< Device firmware boot */
};



/**
 * @defgroup SD_Platform_Function_Declarations Platform Functions
 * @{
 */
void sd_wl18xx_ant_config			(uint8_t ant);
void sd_zynq_clk_enable				(uint8_t en);
void sd_zynq_jtag_enable			(uint8_t en);
void sd_wl_clk_enable				(uint8_t en);
void sd_bt_enable				(uint8_t en);
void sd_zynq_boot_config			(enum sd_zynq_boot boot);
enum sd_zynq_boot sd_boot_select		(void);
enum sd_uart_usb_err sd_uart_usb_bridge_init	(void);
enum sd_uart_usb_err sd_uart_usb_transmit	(struct sd_uart_dev *dev);
void sd_dfu_start				(void);
void sd_zynq_enable				(void);
void sd_zynq_disable				(void);
/**
 * SD_Platform_Function_Declarations
 * @}
 */

/**
 * SD_Platform
 * @}
 */

#ifdef __cplusplus
}
#endif
#else
#error "Multiple inclusion of sd_platform.h"
#endif /* __SD_PLATFORM_H */

/*
 * END OF FILE [sd_platform.h]
 */
