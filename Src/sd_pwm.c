/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_pwm.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	PWM Control Functions 
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

#include "sd_pwm.h"
#include "sd_led.h"

/**
 * @defgroup	SD_PWM PWM
 * @brief	PWM configuration and output control
 * @{
 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;


static TIM_OC_InitTypeDef fault_cfg = {
	TIM_OCMODE_PWM1,
	0,
	TIM_OCPOLARITY_LOW,
	TIM_OCNPOLARITY_LOW,
	TIM_OCFAST_DISABLE,
	TIM_OCIDLESTATE_RESET,
	TIM_OCNIDLESTATE_RESET
};

static TIM_OC_InitTypeDef usb_cfg = {
	TIM_OCMODE_PWM1,
	0,
	TIM_OCPOLARITY_HIGH,
	TIM_OCNPOLARITY_LOW,
	TIM_OCFAST_DISABLE,
	TIM_OCIDLESTATE_RESET,
	TIM_OCNIDLESTATE_RESET
};

static TIM_OC_InitTypeDef bluetooth_cfg = {
	TIM_OCMODE_PWM1,
	0,
	TIM_OCPOLARITY_LOW,
	TIM_OCNPOLARITY_LOW,
	TIM_OCFAST_DISABLE,
	TIM_OCIDLESTATE_RESET,
	TIM_OCNIDLESTATE_RESET
};

static TIM_OC_InitTypeDef app_cfg = {
	TIM_OCMODE_PWM1,
	0,
	TIM_OCPOLARITY_LOW,
	TIM_OCNPOLARITY_LOW,
	TIM_OCFAST_DISABLE,
	TIM_OCIDLESTATE_RESET,
	TIM_OCNIDLESTATE_RESET
};

static TIM_OC_InitTypeDef wireless_cfg = {
	TIM_OCMODE_PWM1,
	0,
	TIM_OCPOLARITY_LOW,
	TIM_OCNPOLARITY_LOW,
	TIM_OCFAST_DISABLE,
	TIM_OCIDLESTATE_RESET,
	TIM_OCNIDLESTATE_RESET
};

/**
 * @addtogroup SD_PWM_Functions PWM Functions
 * @{
 */

/**
 * @brief	Timer 1 Initialization
 */
void sd_tim1_init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 1919;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	HAL_TIM_PWM_Init(&htim1);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

	HAL_TIM_PWM_ConfigChannel(&htim1, &fault_cfg, TIM_CHANNEL_1);
}


/**
 * @brief	Timer 3 Initialization
 */
void sd_tim3_init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 1919;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&htim3);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
	
	HAL_TIM_PWM_ConfigChannel(&htim3, &usb_cfg, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htim3, &bluetooth_cfg, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htim3, &app_cfg, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htim3, &wireless_cfg, TIM_CHANNEL_4);
}



void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if (htim_pwm->Instance==TIM1) {
		/* Peripheral clock enable */
		__TIM1_CLK_ENABLE();
  
		/**
		 * ### TIM1 GPIO Configuration
		 * 
		 * Pin/Port	| Channel	| LED
		 * -------------|---------------|----------------
		 * PA8		| Channel 1 	| fault_led
		 */
		GPIO_InitStruct.Pin = RED_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
		HAL_GPIO_Init(RED_GPIO_Port, &GPIO_InitStruct);

		fault_led.tim = htim_pwm;
		fault_led.chan = TIM_CHANNEL_1;
		fault_led.cfg = &fault_cfg;

	} else if (htim_pwm->Instance==TIM3) {
		/* Peripheral clock enable */
		__TIM3_CLK_ENABLE();
  
		/**
		 * ### TIM3 GPIO Configuration
		 * 
		 * Pin/Port	| Channel	| LED
		 * -------------|---------------|---------------
		 * PC6		| Channel 1	| usb_led
		 * PC7		| Channel 2	| bluetooth_led
		 * PC8		| Channel 3	| app_led
		 * PC9		| Channel 4	| wireless_led
		 */
		GPIO_InitStruct.Pin = ORANGE_Pin |
				      WHITE_Pin |
				      BLUE_Pin |
				      GREEN_Pin;
		
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF0_TIM3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		usb_led.tim = htim_pwm;
		usb_led.chan = TIM_CHANNEL_1;
		usb_led.cfg = &usb_cfg;

		bluetooth_led.tim = htim_pwm;
		bluetooth_led.chan = TIM_CHANNEL_2;
		bluetooth_led.cfg = &bluetooth_cfg;

		app_led.tim = htim_pwm;
		app_led.chan = TIM_CHANNEL_3;
		app_led.cfg = &app_cfg;

		wireless_led.tim = htim_pwm;
		wireless_led.chan = TIM_CHANNEL_4;
		wireless_led.cfg = &wireless_cfg;
	}
}


void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
	if (htim_pwm->Instance == TIM1) {
		/* Peripheral clock disable */
		__TIM1_CLK_DISABLE();
  
		/**
		 * ### TIM1 GPIO Configuration
		 * 
		 * Pin/Port	| Channel	| LED
		 * -------------|---------------|----------------
		 * PA8		| Channel 1 	| fault_led
		 */
		HAL_GPIO_DeInit(RED_GPIO_Port, RED_Pin);
	} else if (htim_pwm->Instance == TIM3) {
		/* Peripheral clock disable */
		__TIM3_CLK_DISABLE();
  
		/**
		 * ### TIM3 GPIO Configuration
		 * 
		 * Pin/Port	| Channel	| LED
		 * -------------|---------------|---------------
		 * PC6		| Channel 1	| usb_led
		 * PC7		| Channel 2	| bluetooth_led
		 * PC8		| Channel 3	| app_led
		 * PC9		| Channel 4	| wireless_led
		 */
		HAL_GPIO_DeInit(GPIOC, ORANGE_Pin | WHITE_Pin | BLUE_Pin | GREEN_Pin);
	}
}
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_pwm.c]
 */
