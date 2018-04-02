/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_spi.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	SPI Slave Control
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "stm32f0xx_hal.h"

#include "sd_buffer.h"
#include "sd_uart.h"
#include "sd_spi.h"

/**
 * @defgroup	SD_SPI SPI
 * @brief	SPI slave configuration and handling
 * @{
 */
  
  
/**
 * @defgroup SD_SPI_Variables SPI Variables
 * @{
 */
static uint8_t spi_pkt_buff[256];
SPI_HandleTypeDef hspi2;
struct sd_spi_dev spi2_dev;			/**< SPI2 device */
/**
 * @}
 */
  

/**
 * @defgroup SD_SPI_Functions SPI Functions
 * @{
 */

/**
 * @defgroup SD_SPI_Private_Functions SPI Private Functions
 * @{
 */
static enum sd_spi_error sd_spi_rx_isr		(struct sd_spi_dev *dev);
static inline void sd_spi_error_cb		(struct sd_spi_dev *dev);
/**
 * @}
 */

/* SPI2 init function */
void sd_spi2_init(void)
{
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_SLAVE;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLED;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	hspi2.Init.CRCPolynomial = 7;
	hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLED;
	HAL_SPI_Init(&hspi2);
	
	/* Initialize SPI devide */
//	sd_spi_dev_init(&spi2_dev, &hspi2);
}



void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if (hspi->Instance==SPI2) {
		/* Peripheral clock enable */
		__SPI2_CLK_ENABLE();
  
		/**
		 * ### SPI2 GPIO Configuration    
		 * PB15     ------> SPI2_MOSI
		 * PB14     ------> SPI2_MISO
		 * PB13     ------> SPI2_SCK
		 * PB12     ------> SPI2_NSS 
		 */
		GPIO_InitStruct.Pin = ZYNQ_SPI_MOSI_Pin|ZYNQ_SPI_MISO_Pin|ZYNQ_SPI_CLK_Pin|ZYNQ_SPI_NSS_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI2_IRQn);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
	if (hspi->Instance==SPI2) {
		/* Peripheral clock disable */
		__SPI2_CLK_DISABLE();
  
		/**
		 * SPI2 GPIO Configuration    
		 * PB15     ------> SPI2_MOSI
		 * PB14     ------> SPI2_MISO
		 * PB13     ------> SPI2_SCK
		 * PB12     ------> SPI2_NSS 
		 */
		HAL_GPIO_DeInit(GPIOB, ZYNQ_SPI_MOSI_Pin|ZYNQ_SPI_MISO_Pin|ZYNQ_SPI_CLK_Pin|ZYNQ_SPI_NSS_Pin);

		/* Peripheral interrupt Deinit*/
		HAL_NVIC_DisableIRQ(SPI2_IRQn);

	}
} 


/**
 * @brief	SPI Device Initialization
 *		Initialize SPI device structure
 *
 * @param  spi_dev:   Device structure to initialize
 * @retval SD_SPI_SUCCESS on success, error state otherwise
 */
enum sd_spi_error sd_spi_dev_init(struct sd_spi_dev *dev,
					SPI_HandleTypeDef *spi)
{
	/* Reset the UART device structure */
	memset(dev, 0, sizeof(struct sd_spi_dev));
	
	dev->spi = spi;
  
	return SD_SPI_SUCCESS;
}


/**
 * @brief	SPI Receive Interrupt Initialization
 *		Enable and prepare the SPI to receive on interrupt
 *
 *		@note This function expects that the SPI peripheral and the
 *		buffer have already been inititialized and are ready to be used.
 *
 * @param  dev:	Device to initialize SPI and buffer
 * @param  spi:	SPI peripheral handle
 * @param  rx_buff:	Buffer to use for UART receive function
 * @retval SD_SPI_SUCCESS on success, error state otherwise
 */
enum sd_spi_error sd_spi_rx_init(struct sd_spi_dev *dev,
					struct sd_cbuf *rx_buff) 
{
	SPI_HandleTypeDef *spi = dev->spi;

	if ((dev == NULL) || (spi == NULL))
		goto error;
					    
	dev->spi = spi;	     /* Set the UART handle pointer */
	   
	/* Process Locked */		    
	if (spi->Lock == HAL_LOCKED)
		return SD_SPI_LOCKED;
	else
		spi->Lock = HAL_LOCKED;

	/* Configure communication */
	spi->State       = HAL_SPI_STATE_BUSY_RX;
	spi->ErrorCode   = HAL_SPI_ERROR_NONE;


	if (spi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE) {
		spi->CRCSize = 1;
		if ((spi->Init.DataSize <= SPI_DATASIZE_8BIT) && (spi->Init.CRCLength == SPI_CRC_LENGTH_16BIT)) {
			spi->CRCSize = 2;
		}
	} else {
		spi->CRCSize = 0;
	}

	/* set fiforxthresold according the reception data length: 8 bit */
	SET_BIT(spi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
//	spi->RxISR = SPI_RxISR_8BIT;

	/* Configure communication direction : 1Line */
	if (spi->Init.Direction == SPI_DIRECTION_1LINE)
		SPI_1LINE_RX(spi);

	/* Reset CRC Calculation */
	if (spi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
		SPI_RESET_CRC(spi);
	

	/* Enable RXNE and ERR interrupt */
	__HAL_SPI_ENABLE_IT(spi, (SPI_IT_RXNE | SPI_IT_ERR));

	/* Check if the SPI is already enabled */
	if ((spi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
		__HAL_SPI_ENABLE(spi);	       /* Enable SPI peripheral */

	error :
		/* Process Unlocked */
		__HAL_UNLOCK(spi);
		return SD_SPI_ERROR;
}


/**
 * @brief	SPI Interrupt Handler
 *		Handle interrupt events on the spi peripheral
 *
 * @param  spi_dev:  SPI device that triggered the interrupt
 * @retval none
 */
void sd_spi_irqhandler(struct sd_spi_dev *dev)
{
	SPI_HandleTypeDef *spi = dev->spi;

	uint32_t itsource = spi->Instance->CR2;
	uint32_t itflag   = spi->Instance->SR;

	/*---------------------- SPI in mode Receiver ------------------------*/
	if (((itflag & SPI_FLAG_OVR) == RESET) &&
	    ((itflag & SPI_FLAG_RXNE) != RESET) && 
	    ((itsource & SPI_IT_RXNE) != RESET)) {
		sd_spi_rx_isr(dev);
		return;
	}

	/*--------------------- SPI in mode Transmitter ----------------------*/
	if (((itflag & SPI_FLAG_TXE) != RESET) && 
	    ((itsource & SPI_IT_TXE) != RESET)) {
		spi->TxISR(spi);
		return;
	}

	/*-------------------- SPI in Error Treatment ------------------------*/
	if ((itflag & (SPI_FLAG_MODF | SPI_FLAG_OVR | SPI_FLAG_FRE)) != RESET) {
		/* SPI Overrun error interrupt occurred */
		if ((itflag & SPI_FLAG_OVR) != RESET) {
			if (spi->State != HAL_SPI_STATE_BUSY_TX) {
				spi->ErrorCode |= HAL_SPI_ERROR_OVR;
				__HAL_SPI_CLEAR_OVRFLAG(spi);
			} else {
				return;
			}
		}

		/* SPI Mode Fault error interrupt occurred */
		if ((itflag & SPI_FLAG_MODF) != RESET) {
			spi->ErrorCode |= HAL_SPI_ERROR_MODF;
			__HAL_SPI_CLEAR_MODFFLAG(spi);
		}

		/* SPI Frame error interrupt occurred */
		if ((itflag & SPI_FLAG_FRE) != RESET) {
			spi->ErrorCode |= HAL_SPI_ERROR_FRE;
			__HAL_SPI_CLEAR_FREFLAG(spi);
		}

		__HAL_SPI_DISABLE_IT(spi, SPI_IT_RXNE | SPI_IT_TXE | SPI_IT_ERR);
		spi->State = HAL_SPI_STATE_READY;
		sd_spi_error_cb(dev);
		return;
	}
}


/**
 * @brief	Receive an amount of data in interrupt mode.
 *
 * @param  spi_dev:   The SPI peripheral that triggered interrupt
 * @retval SD_SPI_SUCCESS on success, error status otherwise
 */
enum sd_spi_error sd_spi_rx_isr(struct sd_spi_dev *dev)
{
	struct sd_cbuf *rx_buff = dev->rx_buff;
	SPI_HandleTypeDef *spi = dev->spi;
	uint8_t c;
	
	rx_buff->buff[rx_buff->in++] = (uint8_t)spi->Instance->DR;
	
	if (rx_buff->in >= rx_buff->size)
		rx_buff->in = 0;
	
	switch (dev->rx_state) {
	case SD_SPI_RX_WAITING:
	/*
	 * Use the received byte to point to the corresponding
	 * peripheral/interface.
	 */
		dev->pkt->hdr = c;
		dev->rx_state = SD_SPI_RX_LENGTH;
		break;
	
	case SD_SPI_RX_LENGTH:
		/*
	 * Use the received byte to determine the length of the packet
	 * message that is to be received.
	 */
		dev->pkt->len = c;
		dev->pkt->pload = &spi_pkt_buff[0];
		dev->rx_state = SD_SPI_RX_PACKET;
		break;
		
	case SD_SPI_RX_PACKET:
		/* In the process of receiving a packet. */
		*(dev->pkt->pload++) = c;
		if (--dev->pkt->len == 0) {
			/* Retransmit the buffer */
			dev->pkt->pload = &spi_pkt_buff[0];
			/* sd_spi_bridge(dev); */
			dev->rx_state = SD_SPI_RX_WAITING;
		}
		break;
	}
	
	
	/**
	 * @todo Determine where to put the collected data based on the first
	 * two bytes (register and packet length) and point to the peripheral
	 * that matches the the register.
	 */    
	
//	if (sd_char_buff_putc_to(rx_buff, c, 100) == SD_BUFF_SUCCESS)
		return SD_SPI_SUCCESS;
//	else
//		return SD_SPI_ERROR;    
}

/**
 * @brief	Handle SPI error interrupts
 *
 * @param  spi:      SPI handle with error triggered error interrupt
 * @retval none
 */
static inline void sd_spi_error_cb(struct sd_spi_dev *dev)
{
  /* Handle errors */
}
/**
 * SD_SPI_Functions
 * @}
 */

/**  
 * SD_SPI
 * @}
 */

/*
 * END OF FILE [sd_spi.c]
 */
