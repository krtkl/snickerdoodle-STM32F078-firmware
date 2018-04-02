/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_uart.h
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	UART Control 
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

#ifndef __SD_UART_H
#define __SD_UART_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SD_UART
 * @{
 */

//#define CIRCBUFF			0x000001
//#define DOUBBUFF			0x000002
//#define BUFFTYPE 			CIRCBUFF

/**
 * @defgroup SD_UART_Defines UART Definitions
 * @{
 */
#define SD_UART_ERROR_NONE		((uint32_t)0x00)
#define SD_UART_ERROR_PARITY		((uint32_t)0x01)
#define SD_UART_ERROR_FRAMING		((uint32_t)0x02)
#define SD_UART_ERROR_NOISE		((uint32_t)0x04)
#define SD_UART_ERROR_OVERRUN		((uint32_t)0x08)

/** 
 * @brief  Check whether an error interrupt has occured.
 *	 Checks the 4 LSBs of the interrupt and status register (ISR) to 
 *	 determine if the overrun, noise, framing or parity error flags have 
 *	 been set.
 *
 * @param  \__HANDLE__: specifies the UART handle.
 *	 This parameter can be UARTx where x: 1, 2, 3 or 4 to select the USART or 
 *	 UART peripheral. (datasheet: up to four USART/UARTs)
 * @retval The state of the error flags __IT__ (TRUE or FALSE).
 */  
#define __SD_UART_ERROR_IT(__HANDLE__) ((__HANDLE__)->Instance->ISR & ((uint32_t)0x0F))

/**
 * @brief  Clear error interrupts
 *	 Clears the 4 LSBs of the interrupt flag clear register (ICR).
 *
 * @param  \__HANDLE__: specifies the UART handle.
 *	 This parameter can be UARTx where x: 1, 2, 3 or 4 to select the USART or 
 *	 UART peripheral. (datasheet: up to four USART/UARTs)
 * @retval None
 */
#define __SD_UART_CLEAR_ERROR_IT(__HANDLE__) ((__HANDLE__)->Instance->ICR = ((uint32_t)0x0F))
/**
 * @}
 */


/**
 * @addtogroup SD_UART_Exported_Variables
 * @{
 */
extern struct sd_uart_dev uart1_dev;
extern struct sd_uart_dev uart2_dev;
/**
 * @}
 */


/**
 * @defgroup SD_UART_Types UART Types
 * @{
 */
  
/**
 * @enum  sd_uart_err
 * @brief UART error enumeration
 */
enum sd_uart_err {
	SD_UART_SUCCESS = 0x4A0D,		/**< Successful execution */
	SD_UART_NULL_ERROR,			/**< NULL pointer error */
	SD_UART_LOCKED,				/**< The buffer is being used */
	SD_UART_TIMEOUT,			/**< The operation timed out */
	SD_UART_ERROR				/**< General execution error */
};

/**
 * @enum   sd_uart_state
 * @brief  UART state enumeration
 */
enum sd_uart_state {
	SD_UART_INITIALIZED = 0x0E60,		/**< UART initialized */
	SD_UART_READY,				/**< UART ready */
	SD_UART_BUSY,				/**< UART in use */
	SD_UART_OVERFLOW			/**< UART buffer has been overrun */
};
  
/**
 * @struct sd_uart_dev
 * @brief  UART device structure
 */
struct sd_uart_dev {
	UART_HandleTypeDef      *uart;		/**< UART Handle */
#if BUFFTYPE == CIRCBUFF
	struct sd_cbuf	*rx_buff;	/**< Buffer for UART receive */
#elif BUFFTYPE == DOUBBUFF
	struct sd_doub_buff     *rx_buff;	/**< Buffer for UART receive */
#endif
	enum sd_uart_state      state;		/**< State of the UART */
	uint8_t		 error;			/**< Error flags from the peripheral */
};

/**
 * @}
 */

/**
 * @addtogroup SD_UART_Functions
 * @{
 */
void sd_usart1_init			(void);
void sd_usart2_init			(void);
void sd_usart3_init			(void);
enum sd_uart_err sd_uart_rx_init	(struct sd_uart_dev *dev,
#if BUFFTYPE == CIRCBUFF
					 struct sd_cbuf *rx_buff);
#elif BUFFTYPE == DOUBBUFF
					 struct sd_doub_buff *rx_buff);
#endif
enum sd_uart_err sd_uart_transmit	(struct sd_uart_dev *dev,
						uint8_t *buf,
						uint32_t len,
						uint32_t timeout);
void sd_uart_irqhandler			(struct sd_uart_dev *dev);
/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#else
#error "Multiple inclusion of sd_uart.h"
#endif /* __SD_UART_H */

/*
 * END OF FILE [sd_uart.h]
 */
