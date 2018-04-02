/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_gpio.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	GPIO Configuration and Control
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

/**
 * @defgroup	SD_GPIO GPIO
 * @brief	GPIO configuration for peripheral pins including alternate
 *		function configurations
 * @{
 */


/**
 * @addtogroup SD_GPIO_Functions GPIO Functions
 * @{
 */

/**
 * @brief	GPIO Initialization
 *		Configure pins as analog, input, output, event_out and EXTI
 */
void sd_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__GPIOE_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOF_CLK_ENABLE();
	
	
	/** Configure the J2 pins */
	GPIO_InitStruct.Pin = J2_P4_Pin | J2_P3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	

	/** Configure boot select pins */
	GPIO_InitStruct.Pin = MIO5_BOOT_SELECT_Pin | 
			      MIO4_BOOT_SELECT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/** Configure wireless 32kHz clock, Zynq power, and Zynq clock pins,
	 * configure watchdog timer and I$^2$C switch reset pins. */
	GPIO_InitStruct.Pin = WL_32KHZ_CLK_EN_Pin | 
			      ZYNQ_POWER_EN_Pin |
			      WDI_Pin |
			      SMB_NRESET_Pin |
			      ZYNQ_CLK_EN_Pin;
				
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/** Configure button pins and Zynq power good pin */
	GPIO_InitStruct.Pin = SELECT_BUTTON_Pin |
			      RESET_BUTTON_Pin |
			      ZYNQ_POWER_GOOD_Pin;
				
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/** Configure Zynq programming init pin */
	GPIO_InitStruct.Pin = ZYNQ_INIT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ZYNQ_INIT_GPIO_Port, &GPIO_InitStruct);
	
	/** Configure Zynq programming status pins */
	GPIO_InitStruct.Pin = ZYNQ_DONE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/** Configure antenna select pins */
	GPIO_InitStruct.Pin = ANT_SELECT_2_Pin |
			      ANT_SELECT_1_Pin;
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/** Configure Bluetooth enable pin */
	GPIO_InitStruct.Pin = WL18xx_BT_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(WL18xx_BT_EN_GPIO_Port, &GPIO_InitStruct);

	/** Configure external interrupt pins */
	GPIO_InitStruct.Pin = JA1_P2_Pin |
			      JA2_P2_Pin |
			      JB1_P2_Pin |
			      JB2_P2_Pin |
			      JC1_P2_Pin;
				
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/** Configure JTAG nRST pin */
	GPIO_InitStruct.Pin = ZYNQ_JTAG_NRST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(ZYNQ_JTAG_NRST_GPIO_Port, &GPIO_InitStruct);
	
	/* Configure red LED pin */
	GPIO_InitStruct.Pin = RED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(RED_GPIO_Port, &GPIO_InitStruct);
	
	/* Configure orange, white, blue, and green LED pins */
	GPIO_InitStruct.Pin = ORANGE_Pin |
			      WHITE_Pin |
			      BLUE_Pin |
			      GREEN_Pin;
		
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/** Set external interrupt priority and enable */
	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}


/**
 * @brief	Handle EXTI interrupt request
 *
 * @param pin:		Specifies the GPIO pin connected to EXTI line
 * @retval None
 */
void sd_gpio_exti_irqhandler(uint16_t GPIO_Pin)
{
	/* EXTI line interrupt detected */
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		HAL_GPIO_EXTI_Callback(GPIO_Pin);
	}
}
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_gpio.c]
 */
