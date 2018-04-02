/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_uart.c
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

/*------------------------------- Includes -----------------------------------*/
#include <string.h>

#include "stm32f0xx_hal.h"

#include "sd_buffer.h"
#include "sd_uart.h"

#include "sd_tim.h"
#include "sd_led.h"
#include "sd_led_pattern.h"
  

/**
 * @defgroup	SD_UART UART
 * @brief	UART configuration and control
 * @{
 */


/**
 * @defgroup	SD_UART_Private_Variables UART Private Variables
 * @{
 */
static UART_HandleTypeDef huart1;		/**< UART 1 handle */
static UART_HandleTypeDef huart2;		/**< UART 2 handle */
static UART_HandleTypeDef huart3;		/**< UART 3 handle */
/**
 * @}
 */

/**
 * @defgroup	SD_UART_Exported_Variables UART Exported Variables
 * @{
 */
struct sd_uart_dev uart1_dev;			/**< UART1 device */
struct sd_uart_dev uart2_dev;			/**< UART2 device */
struct sd_uart_dev uart3_dev;			/**< UART3 device */
/**
 * @}
 */

static enum sd_uart_err sd_uart_dev_init	(struct sd_uart_dev *dev,
							UART_HandleTypeDef *huart);
static enum sd_uart_err sd_uart_rx_isr		(struct sd_uart_dev *dev);
static inline void sd_uart_err_cb		(struct sd_uart_dev *dev);

/**
 * @addtogroup SD_UART_Functions UART Functions
 * @{
 */


/**
 * @brief	USART 1 Initialization
 */
void sd_usart1_init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart1);
	
	/* Initialize the UART device */
	sd_uart_dev_init(&uart1_dev, &huart1);	     
}


/**
 * @brief	USART 2 Initialization
 */
void sd_usart2_init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart2);
	
	/* Initialize the UART devices */				 
	sd_uart_dev_init(&uart2_dev, &huart2);
}


/**
 * @brief	USART 3 Initialization
 */
void sd_usart3_init(void)
{
	huart2.Instance = USART3;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart3);
	
	/* Initialize the UART devices */				 
	sd_uart_dev_init(&uart3_dev, &huart3);
}




void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if (huart->Instance == USART1) {
		/* Peripheral clock enable */
		__USART1_CLK_ENABLE();
  
  
  		/**
		 * ### USART 1 GPIO Configuration
		 * 
		 * Pin/Port	| Signal
		 * -------------|---------------
		 * PB7		| USART1_RX
		 * PB6		| USART1_TX
		 */
		GPIO_InitStruct.Pin = ZYNQ_CONSOLE_TX_Pin|ZYNQ_CONSOLE_RX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		
	} else if (huart->Instance == USART2) {
		/* Peripheral clock enable */
		__USART2_CLK_ENABLE();
  
    		/**
		 * ### USART 2 GPIO Configuration
		 * 
		 * Pin/Port	| Signal
		 * -------------|---------------
		 * PD5		| USART2_TX
		 * PD6		| USART2_RX
		 * PD4		| USART2_RTS
		 * PD3		| USART2_CTS
		 */
		GPIO_InitStruct.Pin = BT_HCI_RX_Pin|BT_HCI_TX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_USART2;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = BT_HCI_CTS_Pin|BT_HCI_RTS_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_USART2;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);

	} else if (huart->Instance == USART3) {
		/* Peripheral clock enable */
		__USART3_CLK_ENABLE();
  
    		/**
		 * ### USART 2 GPIO Configuration
		 * 
		 * Pin/Port	| Signal
		 * -------------|---------------
		 * PC4		| USART3_TX
		 * PC5		| USART3_RX
		 */
		GPIO_InitStruct.Pin =  WL_RS232_TX_Pin |  WL_RS232_RX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_USART3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(USART3_4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART3_4_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART1) {
		/* Peripheral clock disable */
		__USART1_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOB, 
				ZYNQ_CONSOLE_TX_Pin |
				ZYNQ_CONSOLE_RX_Pin);

		/* Peripheral interrupt Deinit*/
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		
	} else if (huart->Instance == USART2) {
		/* Peripheral clock disable */
		__USART2_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOD,
				BT_HCI_RX_Pin |
				BT_HCI_TX_Pin | 
				BT_HCI_CTS_Pin |
				BT_HCI_RTS_Pin);

		/* Peripheral interrupt Deinit*/
		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
} 



/**
 * @brief  	UART Device Initialization
 *	 	Initialize UART device structure
 *
 * @param  uart_dev:   Device structure to initialize
 * @retval SD_UART_SUCCESS on success, error state otherwise
 */
enum sd_uart_err sd_uart_dev_init(struct sd_uart_dev *dev,
					UART_HandleTypeDef *huart)
{
	if (huart == NULL)
		return SD_UART_ERROR;
	
	/* Reset the UART device structure */
	memset(dev, 0, sizeof(struct sd_uart_dev));
  
	dev->uart = huart;	 	/* Set the UART handle pointer */
	
	
	return SD_UART_SUCCESS;
}


/**
 * @brief	UART Receive Interrupt Initialization
 *	 	Enable and prepare the UART to receive on interrupt
 *
 *	 	@note This function expects that the UART peripheral and the
 *		buffer have already been inititialized and are ready to be used.
 *
 * @param  uart_dev:	Device to initialize UART and buffer with
 * @param  huart:	UART peripheral handle
 * @param  rx_buff:	Buffer to use for UART receive function
 * @retval SD_UART_SUCCESS on success, error state otherwise
 */
enum sd_uart_err sd_uart_rx_init(struct sd_uart_dev *dev,
				 struct sd_cbuf *rx_buff)
{
	UART_HandleTypeDef *huart = dev->uart;
	
	if ((huart == NULL) || (rx_buff == NULL))
		return SD_UART_ERROR;
					    
	dev->rx_buff = rx_buff;    /* Set the RX buffer pointer */
	   
	/* Process Locked */		    
	if (huart->Lock == HAL_LOCKED) 
		return SD_UART_LOCKED;
	else
		huart->Lock = HAL_LOCKED;

	/* Computation of UART mask to apply to RDR register */
	UART_MASK_COMPUTATION(huart);

	huart->ErrorCode = HAL_UART_ERROR_NONE;

	/* Enable the UART Parity Error Interrupt */
	__HAL_UART_ENABLE_IT(huart, UART_IT_PE);

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

	/* Process Unlocked */
	huart->Lock = HAL_UNLOCKED;

	/* Enable the UART Data Register not empty Interrupt */
	__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

	return SD_UART_SUCCESS;
}


/**
 * @brief	UART Interrupt Handler
 *		Handle interrupt events on the UART peripheral
 *
 * @param  dev:	UART device that triggered the interrupt
 * @retval None
 */
void sd_uart_irqhandler(struct sd_uart_dev *dev) 
{
	UART_HandleTypeDef *huart = dev->uart;
  
	/* Check for errors and store them in the device error state */
	if ((dev->error |= __SD_UART_ERROR_IT(huart))) {
		__SD_UART_CLEAR_ERROR_IT(huart);	/* Clear the error flags */
		sd_uart_err_cb(dev);			/* Execute error callback */
		dev->error = SD_UART_ERROR_NONE;	/* Clear the error */
		return;				     	/* Return without reading or writing */
	}

	/* Receive Interrupt */
	if ((__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET) && 
	    (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) != RESET)) {
		sd_uart_rx_isr(dev);
		/* Clear RXNE interrupt flag */
		__HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);
	}
       
	/* Transmit Interrupt */
	if ((__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET) &&
	    (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE) != RESET)) {
		UART_Transmit_IT(huart);
	}

	/* Transmit End Interrupt */
	if ((__HAL_UART_GET_IT(huart, UART_IT_TC) != RESET) &&
	    (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC) != RESET)) {
		UART_EndTransmit_IT(huart);
	}  
}


/**
 * @brief	Receive Interrupt Routine
 * 		
 *		@note This function is called from within the interrupt handler
 *		when the the RXNE interrupt is enabled and set.
 *
 * @param  dev:	The UART peripheral that triggered interrupt
 * @retval SD_UART_SUCCESS on success, error status otherwise
 */
inline enum sd_uart_err sd_uart_rx_isr(struct sd_uart_dev *dev)
{
	struct sd_cbuf *rx_buff = dev->rx_buff;
	UART_HandleTypeDef *huart = dev->uart;
	uint16_t mask = huart->Mask;

	/** Put the character in the current buffer */
	rx_buff->buff[rx_buff->in++] = (uint8_t)(huart->Instance->RDR & (uint8_t)mask);
	
	/* Wrap buffer if at the end */
	if (rx_buff->in >= rx_buff->size)
		rx_buff->in = 0;

	if (++rx_buff->len >= rx_buff->size) {
		rx_buff->state = SD_BUFF_OVERFLOW;
		return SD_UART_ERROR;
	}

	return SD_UART_SUCCESS;
}



/**
 * @brief	Transmit Data In Blocking Mode
 *
 * @param  dev:	The UART peripheral to trasmit over
 * @param  buf:	Buffer data to trasmit
 * @param  len:	Length of buffer data
 * @param  timeout:	Timeout for transmission activity
 * @retval SD_UART_SUCCESS on success, error status otherwise
 */
inline enum sd_uart_err sd_uart_transmit(struct sd_uart_dev *dev,
					uint8_t *buf,
					uint32_t len,
					uint32_t timeout)
{
	if (HAL_UART_Transmit(dev->uart, buf, len, timeout) == HAL_OK)
		return SD_UART_SUCCESS;
	
	return SD_UART_ERROR;
}


/**
 * @brief	Handle UART errors
 *
 * @param  huart:      UART handle with error triggered error interrupt
 * @retval none
 */
static inline void sd_uart_err_cb(struct sd_uart_dev *dev)
{
  /* Handle errors */
}
/**
 * SD_UART_Functions
 * @}
 */

/**  
 * SD_UART
 * @}
 */

/*
 * END OF FILE [sd_uart.c]
 */
