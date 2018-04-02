/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_spi.h
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	SPI Slave Control
 * @copyright	FreeBSD
 *
 *******************************************************************************
 *
 * Copyright (c) krtkl inc., 2016
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************
 */

#ifndef __SD_SPI_H
#define __SD_SPI_H
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup SD_SPI
 * @{
 */
extern SPI_HandleTypeDef hspi2;
extern struct sd_spi_dev spi2_dev;

/**
 * @defgroup SD_SPI_Types SPI Types
 * @{
 */
  
/**
 * @enum  sd_spi_error
 * @brief SPI error enumeration
 */
enum sd_spi_error {
	SD_SPI_SUCCESS = 0x4A0D,		/**< Successful execution */
	SD_SPI_NULL_ERROR,			/**< NULL pointer error */
	SD_SPI_LOCKED,				/**< The buffer is being used */
	SD_SPI_TIMEOUT,				/**< The operation timed out */
	SD_SPI_ERROR				/**< General execution error */
};

/**
 * @enum   sd_spi_state
 * @brief  SPI state enumeration
 */
enum sd_spi_state {
	SD_SPI_INITIALIZED = 0x0E60,		/**< SPI initialized */
	SD_SPI_READY,				/**< SPI ready */
	SD_SPI_BUSY,				/**< SPI in use */
	SD_SPI_OVERFLOW				/**< SPI buffer has been overrun */
};


/**
 * @enum   sd_spi_rx_state
 * @brief  SPI receive state enumeration
 */
enum sd_spi_rx_state {
	SD_SPI_RX_WAITING,			/**< Waiting to receive command byte */
	SD_SPI_RX_LENGTH,			/**< Waiting to receive packet length */
	SD_SPI_RX_PACKET,			/**< Packet receive in process */
};

/**
 * @struct sd_spi_packet
 * @brief  SPI packet structure
 */
struct sd_spi_packet {
	uint8_t hdr;				/**< Packet header */
	uint8_t len;				/**< Packet length */
	uint8_t *pload;				/**< Packet buffer/payload pointer */
};

  
/**
 * @struct sd_spi_dev
 * @brief  SPI device structure
 */
struct sd_spi_dev {
	SPI_HandleTypeDef       *spi;		/**< SPI Handle */
	struct sd_cbuf     *rx_buff;	/**< Buffer for SPI receive */
	enum sd_spi_rx_state    rx_state;	/**< Receive state */
	struct sd_spi_packet    *pkt;		/**< Receive packet */
	enum sd_spi_state       state;		/**< State of the SPI */
	uint8_t			error;		/**< Error flags from the peripheral */
};
/**
 * @}
 */

/**
 * @addtogroup SD_SPI_Functions
 * @{
 */
void sd_spi2_init(void);
enum sd_spi_error sd_spi_dev_init	(struct sd_spi_dev *dev,
						SPI_HandleTypeDef *spi);
enum sd_spi_error sd_spi_rx_init	(struct sd_spi_dev *dev,
						struct sd_cbuf *rx_buff);
void sd_spi_irqhandler			(struct sd_spi_dev *dev);
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
#error "Multiple inclusion of sd_spi.h"
#endif /* __SD_SPI_H */

/*
 * END OF FILE [sd_spi.h]
 */
