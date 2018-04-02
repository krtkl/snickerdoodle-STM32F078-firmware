/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_dac.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	Digital to Analog Converter Configuration and Control
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
#include "stm32f0xx_hal.h"

#include "sd_gpio.h"
#include "sd_dac.h"



static DAC_HandleTypeDef hdac;


/**
 * @addtogroup SD_DAC_Functions DAC Functions
 * @{
 */

/**
 * @brief	DAC Initialization
 *		Configure digital-to-analog converter peripheral and channels
 */

void sd_dac_init(void)
{
	DAC_ChannelConfTypeDef sConfig;

	/**
	 * DAC Initialization 
	 */
	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);

	/**DAC channel OUT1 config 
	*/
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

	/**
	 * DAC channel OUT2 config 
	 */
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2);
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if (hdac->Instance == DAC) {
		/* Peripheral clock enable */
		__DAC1_CLK_ENABLE();
  
		GPIO_InitStruct.Pin = J2_P4_Pin | J2_P3_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
	if (hdac->Instance == DAC) {
		/* Peripheral clock disable */
		__DAC1_CLK_DISABLE();
  
		HAL_GPIO_DeInit(GPIOA, J2_P4_Pin|J2_P3_Pin);
	}
} 
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_dac.c]
 */
