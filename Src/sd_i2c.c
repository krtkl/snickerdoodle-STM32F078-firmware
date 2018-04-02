/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_i2c.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	I2C Control 
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

#include "sd_i2c.h"
  
  
/**
 * @defgroup	SD_I2C I2C
 * @brief	I2C communication with connected devices
 * @{
 */


/**
 * @defgroup SD_I2C_Private_Variables I2C Private Variables
 * @{
 */
static uint8_t sd_i2c_chan;

static I2C_HandleTypeDef hi2c1;			/**< I2C1 handle */
static I2C_HandleTypeDef hi2c2;			/**< I2C2 handle */
/**
 * @}
 */
  
  
/**
 * @defgroup SD_I2C_Exported_Variables I2C Exported Variables
 * @{
 */
struct sd_i2c_dev i2c1_dev;			/**< I2C1 device */      
struct sd_i2c_dev i2c2_dev;			/**< I2C2 device */
/**
 * @}
 */

static enum sd_i2c_err sd_i2c_dev_init	(struct sd_i2c_dev *dev, 
						I2C_HandleTypeDef *hi2c);

/**
 * @defgroup SD_I2C_Functions I2C Functions
 * @{
 */

/**
 * @brief	I2C1 Initialization
 *		Initialize low-level hardware peripheral and set device handle
 */
void sd_i2c1_init(void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x20303E5D;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	HAL_I2C_Init(&hi2c1);

	/**
	 * Configure Analog filter 
	 */
	HAL_I2CEx_AnalogFilter_Config(&hi2c1, I2C_ANALOGFILTER_ENABLED);
	
	/* Initialize I2C device */
	sd_i2c_dev_init(&i2c1_dev, &hi2c1);
}


/**
 * @brief	I2C2 Initialization
 *		Initialize low-level hardware peripheral and set device handle
 */
void sd_i2c2_init(void)
{
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x20303E5D;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	HAL_I2C_Init(&hi2c2);

	/**
	 * Configure Analog filter 
	 */
	HAL_I2CEx_AnalogFilter_Config(&hi2c2, I2C_ANALOGFILTER_ENABLED);
	
	/* Initialize I2C device */
	sd_i2c_dev_init(&i2c2_dev, &hi2c2);
}



void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if (hi2c->Instance==I2C1) {
		/**
		 * ### I2C1 GPIO Configuration
		 * 
		 * Pin/Port	| Signal
		 * ------------|---------------
		 * PB8		| SCL
		 * PB9		| SDA 
		 */
		GPIO_InitStruct.Pin = SMB_I2C_SCL_Pin|SMB_I2C_SDA_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral clock enable */
		__I2C1_CLK_ENABLE();
	} else if (hi2c->Instance==I2C2) {
		/**
		 * ### I2C2 GPIO Configuration
		 * 
		 * Pin/Port	| Signal
		 * ------------|---------------
		 * PB10	| SCL
		 * PB11	| SDA 
		 */
		GPIO_InitStruct.Pin = CRYPTO_I2C_SCL_Pin|CRYPTO_I2C_SDA_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_I2C2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral clock enable */
		__I2C2_CLK_ENABLE();
	}
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
	if (hi2c->Instance==I2C1) {
		/* Peripheral clock disable */
		__I2C1_CLK_DISABLE();
  
		HAL_GPIO_DeInit(GPIOB, SMB_I2C_SCL_Pin|SMB_I2C_SDA_Pin);

	} else if (hi2c->Instance==I2C2) {
		/* Peripheral clock disable */
		__I2C2_CLK_DISABLE();
  
		HAL_GPIO_DeInit(GPIOB, CRYPTO_I2C_SCL_Pin|CRYPTO_I2C_SDA_Pin);
	}
} 

  
/**
 * @brief	Initialize I2C channel
 *		Initialize I2C channel with a peripheral handle and set the
 *		device as ready.
 *
 * @param  dev:	I2C device to initialize
 * @param  hi2c:       I2C device handle to use
 * @retval SD_I2C_SUCCESS on success, error state otherwise
 */
enum sd_i2c_err sd_i2c_dev_init(struct sd_i2c_dev *dev, I2C_HandleTypeDef *hi2c)
{
	memset(dev, 0, sizeof(struct sd_i2c_dev));
	
	dev->i2c = hi2c;
	dev->state = SD_I2C_READY;
	
	return SD_I2C_SUCCESS;
}
  
  
/**
 * @brief	Set I2C channel 
 *		Set I2C channel (more than one channel may be specified) to
 *		broadcast messages
 *
 * @param  dev:	I2C device to configure
 * @param  chan:       Channel(s) to set on the I2C switch
 * @retval SD_I2C_SUCCESS on success, error state otherwise
 */
enum sd_i2c_err sd_i2C_set_chan(struct sd_i2c_dev *dev, uint8_t chan)
{
	sd_i2c_chan = chan;
	
	/* Set the output state of the switch */
	if (HAL_I2C_Master_Transmit(dev->i2c, SD_I2C_SWITCH_ADDR, &sd_i2c_chan, 1, 1000) != HAL_OK)
		return SD_I2C_ERROR;

	return SD_I2C_SUCCESS;
}


/**
 * @brief	Enable I2C channel
 *		Enable an I2C channel without altering previously configured
 *		channels. More than one channel may be specified.
 *
 * @param  dev:	I2C device to configure
 * @param  chan:       Channel to enable on the I2C switch
 * @retval SD_I2C_SUCCESS on success, error state otherwise
 */
enum sd_i2c_err sd_i2c_en_chan(struct sd_i2c_dev *dev, uint8_t chan)
{
	sd_i2c_chan |= chan;
	
	if (HAL_I2C_Master_Transmit(dev->i2c, SD_I2C_SWITCH_ADDR, &sd_i2c_chan, 1, 1000) != HAL_OK)
		return SD_I2C_ERROR;
	
	return SD_I2C_SUCCESS;
}


/**
 * @brief	Disable I2C channel
 *		Disable an I2C channel without altering previously configured
 *		channels. More than one channel may be specified.
 *
 * @param  dev:	I2C device to configure
 * @param  chan:       Channel to disable on the I2C switch
 * @retval SD_I2C_SUCCESS on success, error state otherwise
 */
enum sd_i2c_err sd_i2c_dis_chan(struct sd_i2c_dev *dev, uint8_t chan)
{
	sd_i2c_chan &= ~(chan);
	
	if (HAL_I2C_Master_Transmit(dev->i2c, SD_I2C_SWITCH_ADDR, &sd_i2c_chan, 1, 1000) != HAL_OK)
		return SD_I2C_ERROR;
	
	return SD_I2C_SUCCESS;
}
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_i2c.c]
 */

