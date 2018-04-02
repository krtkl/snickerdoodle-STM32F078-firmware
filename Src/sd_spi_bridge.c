/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_spi_bridge.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	SPI Slave Peripheral Bridge Control
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

#include "std_defs.h"

#include "sd_uart.h"
#include "sd_buffer.h"
#include "sd_spi.h"
#include "sd_spi_bridge.h"
  

/**
 * @defgroup	SD_SPI_Bridge SPI Bridge
 * @brief	Bridging from SPI to other peripherals
 * @{
 */
struct sd_spbr spi2_spbr;

static struct sd_spbr_dev spbr_dev_tab[] = {
	{ SD_SPBR_I2C_DEV, NULL, NULL },
	SD_SPBR_NULL_DEV
};

static void sd_spbr_rx_isr(struct sd_spbr *spbr);


/**
 * @brief	SPI Bridge Find Device
 *		Searches a sentinel terminated table pointed to by head for a
 *		matching ID and returns a pointer to the device or NULL
 *
 * @note	To speed up the loops, this function expects the device mask be
 *		applied to the id before it is passed to this function.
 *
 * @param  head:	Pointer to head of table for searching
 * @param  id:		ID value to search for
 * @retval Pointer to matching device or NULL if not found
 */
static struct sd_spbr_dev *sd_spbr_find_device(struct sd_spbr_dev *head, uint8_t id)
{
	struct sd_spbr_dev *pdev = head;

	while ((pdev != NULL) && (pdev->id > 0)) {
		if (pdev->id == id)
			return pdev;
			
		pdev++;
	}

	return NULL;
}


/**
 * @brief	SPI Bridge Find Command
 *		Searches a sentinel terminated table pointed to by head for a
 *		matching ID and returns a pointer to the device or NULL
 *
 * @note	To speed up the loops, this function expects the device mask be
 *		applied to the id before it is passed to this function.
 *
 * @param  head:	Pointer to head of table for searching
 * @param  id:		ID value to search for
 * @retval Pointer to matching device or NULL if not found
 */
static struct sd_spbr_cmd *sd_spbr_find_cmd(struct sd_spbr_cmd *head, uint8_t opcode)
{
	struct sd_spbr_cmd *pcmd = head;
	
	while ((pcmd != NULL) && (pcmd->opcode > 0)) {
		if (pcmd->opcode == opcode)
			return pcmd;
			
		pcmd++;
	}
	
	return NULL;
}


/**
 * @brief	SPI Bridge Initialize
 *
 * @param  spbr:	Pointer to SPI bridge structure to initialize
 * @param  spi:		SPI handle to use for bridge
 * @retval None
 */
void sd_spbr_init(struct sd_spbr *spbr, SPI_HandleTypeDef *spi)
{

	/* Reset the UART device structure */
	memset(spbr, 0, sizeof(struct sd_spbr));
	
	spbr->spi = spi;

	/* Prepare the peripheral to receive */
	/* Enable RXNE and ERR interrupt */
	__HAL_SPI_ENABLE_IT(spi, (SPI_IT_RXNE | SPI_IT_ERR));

	/* Check if the SPI is already enabled */
	if ((spi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
		__HAL_SPI_ENABLE(spi);	       /* Enable SPI peripheral */

	spbr->state = SD_SPBR_READ_DEVICE;
}


/**
 * @brief	SPI Bridge Interrupt Handler
 *		Handle interrupt events on the SPI peripheral
 *
 * @param  spbr:	Pointer to SPI bridge structure
 * @retval None
 */
void sd_spbr_irqhandler(struct sd_spbr *spbr)
{
	SPI_HandleTypeDef *spi = spbr->spi;

	uint32_t itsource = spi->Instance->CR2;
	uint32_t itflag   = spi->Instance->SR;

	/*---------------------- SPI in mode Receiver ------------------------*/
	if (((itflag & SPI_FLAG_OVR) == RESET) &&
	    ((itflag & SPI_FLAG_RXNE) != RESET) && 
	    ((itsource & SPI_IT_RXNE) != RESET)) {
		sd_spbr_rx_isr(spbr);
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
		return;
	}
}


/**
 * @brief	SPI Bridge Receive Interrupt Routine
 *
 * @param  spbr:	Pointer to SPI bridge structure
 * @retval None
 */
static void sd_spbr_rx_isr(struct sd_spbr *spbr)
{
	uint8_t data = (uint8_t)(spbr->spi->Instance->DR);

	switch (spbr->state) {
	case SD_SPBR_READ_DEVICE:
	
		/* Find the device from the table */
		spbr->curr_dev = sd_spbr_find_device(spbr_dev_tab, data & SD_SPBR_DEV_MASK);
		if (spbr->curr_dev == NULL)
			return;
			
		/* Run callback for device, if one exists */
		if (spbr->curr_dev->dev_start != NULL)
			spbr->curr_dev->dev_start(&data);
			
		/* Set the device ID in the transmit buffer */
		HAL_SPI_Transmit_IT(spbr->spi, &data, 1);
		
		spbr->state = SD_SPBR_READ_CMD;
		break;
	
	case SD_SPBR_READ_CMD:
	
		spbr->curr_dev->curr_cmd = sd_spbr_find_cmd(spbr->curr_dev->cmds, 
								(data & SD_SPBR_CMD_MASK));
								
		if (spbr->curr_dev->curr_cmd == NULL) {
			/* A matching command from the device list was not found
			 * This is an exception that needs to be handled
			 */
			spbr->state = SD_SPBR_READ_DEVICE;
			return;
		}
		
		if (spbr->curr_dev->curr_cmd != NULL)
			spbr->curr_dev->curr_cmd->start(&data);
			
		/* Set the device ID in the transmit buffer */
		HAL_SPI_Transmit_IT(spbr->spi, &data, 1);
		
		spbr->state = SD_SPBR_IN_PROCESS;
	
		break;
		
	case SD_SPBR_IN_PROCESS:
	
	
		break;
	}
}
/**  
 * SD_SPI_Bridge
 * @}
 */

/*
 * END OF FILE [sd_spi_bridge.c]
 */
