/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_spi_bridge.h
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

#ifndef __SD_SPI_BRIDGE_H
#define __SD_SPI_BRIDGE_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SD_SPI_Bridge
 * @{
 */
#define SD_SPBR_NDEV			(3)

#define SD_SPBR_I2C_STATUS		((uint8_t)(1 << 0))
#define SD_SPBR_LED_STATUS		((uint8_t)(1 << 1))
#define SD_SPBR_UART_STATUS		((uint8_t)(1 << 2))


#define SD_SPBR_DEV_MASK		(0xF8)

#define SD_SPBR_I2C_DEV			(0x80)


#define SD_SPBR_CMD_MASK		(0xE0)

#define SD_SPBR_I2C_RW_CMD		(0xA0)

#define SD_SPBR_I2C_READ_TRANS		((uint8_t)(1 << 2))
#define SD_SPBR_I2C_ADDR_10BIT		((uint8_t)(1 << 3))
#define SD_SPBR_I2C_FREQ_400K		((uint8_t)(1 << 4))



#define SD_SPBR_NULL_DEV		{ 0, NULL, NULL }
#define SD_SPBR_NULL_OP			{ 0, NULL, NULL }



enum sd_spbr_state {
	SD_SPBR_READ_DEVICE,
	SD_SPBR_READ_CMD,
	SD_SPBR_IN_PROCESS
};


/**
 * @struct sd_spbr_cmd
 * @brief  SPI bridge device operation structure
 */
struct sd_spbr_cmd {
	uint8_t opcode;
	void	(*start)(uint8_t *data);
	void	(*run)(uint8_t *data);
	void	(*end)(uint8_t *data);
};


/**
 * @struct sd_spbr_dev
 * @brief  SPI bridge device structure
 */
struct sd_spbr_dev {
	uint8_t			id;					/**< Device ID */
	struct sd_spbr_cmd	*curr_cmd;				/**< Current command */
	struct sd_spbr_cmd	*cmds;					/**< Device commands */
	void    		(*dev_start)(uint8_t *data);		/**< Device start callback */
};


/**
 * @struct sd_spbr
 * @brief  SPI bridge structure
 */
struct sd_spbr {
	SPI_HandleTypeDef	*spi;
	struct sd_spbr_dev	*curr_dev;				/**< Current device */
	struct sd_spbr_dev	*devs;					/**< Bridge devices */ 
	enum sd_spbr_state 	state;					/**< Bridge state */
};


extern struct sd_spbr spi2_spbr;

void sd_spbr_init(struct sd_spbr *spbr, SPI_HandleTypeDef *spi);
void sd_spbr_irqhandler(struct sd_spbr *spbr);

/**
 * SD_SPI_Bridge
 * @}
 */

#ifdef __cplusplus
}
#endif
#else
#error "Multiple inclusion of sd_spi_bridge.h"
#endif /* __SD_SPI_BRIDGE_H */

/*
 * END OF FILE [sd_spi_bridge.h]
 */
