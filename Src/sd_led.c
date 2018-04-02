/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_led.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	LED Control Functions 
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

#include "sd_tim.h"
#include "sd_led.h"


/**
 * @defgroup	SD_LED LEDs
 * @brief	LED configuration and output control
 * @{
 */


/**
 * @defgroup SD_LED_Exported_Variables LED Exported Variables
 * @{
 */
struct sd_led fault_led;		/**< Fault status LED */
struct sd_led usb_led;			/**< USB status LED */
struct sd_led bluetooth_led;		/**< Bluetooth status LED */
struct sd_led app_led;			/**< User Application LED */
struct sd_led wireless_led;		/**< Wireless link status LED */
/**
 * @}
 */

/**
 * @defgroup SD_LED_Functions LED functions
 * @{
 */


/**
 * @brief	LED initialization
 *		Initialize an LED configuration on a timer peripheral with a
 *		pattern
 *
 * @param  led:		LED configuration
 * @param  tim:		Timer handle associated with the LED
 * @param  chan:	Timer channel for the LED
 * @param  pattern:	The pattern to initialize the LED with
 * @param  len:		Length of the pattern
 * @param  up_tim:	Update timer for patter queue
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_init(struct sd_led *led,
				TIM_HandleTypeDef *tim,
				uint32_t chan,
				uint32_t *pattern,
				uint32_t len,
				TIM_HandleTypeDef *up_tim)
{
	uint8_t reenable_timer;
			      
	/* Check for NULL pointers for the config and timer handle */
	if (led == NULL)
		return SD_LED_NULL_HANDLE_ERROR;

	if (led->tim == NULL)
		return SD_LED_NULL_TIMER_ERROR; 

	/* Check if the update timer has been initialized */
	if ((up_tim->Instance->CR1 & (TIM_CR1_CEN)) && 
	    (up_tim->Instance->DIER & (TIM_IT_UPDATE))) {
		reenable_timer = 1;
		HAL_TIM_Base_Stop_IT(up_tim);
	}
       
	/* Set the configuration of the LED */
	led->tim = tim;
	led->chan = chan;
	led->pattern = pattern;
	led->pattern_len = len;
	led->pattern_idx = 0;
	led->up_tim = up_tim;
       
	/**
	 * Only reenable the update timer if it was running prior to this
	 * function being called
	 */  
	if (reenable_timer)
		HAL_TIM_Base_Start_IT(up_tim);
  
	return SD_LED_SUCCESS;
}


/**
 * @brief	LED Enable
 */
enum sd_led_err sd_led_enable(struct sd_led *led, uint8_t en)
{
	TIM_OC_InitTypeDef tim_cfg = {
		TIM_OCMODE_PWM1,
		0,
		TIM_OCPOLARITY_LOW,
		TIM_OCNPOLARITY_LOW,
		TIM_OCFAST_DISABLE,
		TIM_OCIDLESTATE_RESET,
		TIM_OCNIDLESTATE_RESET
	};

	if (led == NULL)
		return SD_LED_NULL_HANDLE_ERROR;
	
	if (en) {
		HAL_TIM_PWM_ConfigChannel(led->tim, led->cfg, led->chan);
		HAL_TIM_PWM_Start(led->tim, led->chan);
		led->state = SD_LED_RUNNING;
	} else {
		sd_led_reset(led);
		HAL_TIM_PWM_ConfigChannel(led->tim, &tim_cfg, led->chan);
		HAL_TIM_PWM_Stop(led->tim, led->chan);
		led->state = SD_LED_NOT_READY;
	}
	return SD_LED_SUCCESS;
}



/**
 * @brief	LED reset
 *		Reset the LED configuration and clear the pattern
 *
 * @param  led:		LED configuration
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_reset(struct sd_led *led)
{
  
	/* Check for NULL pointers for the config and timer handle */
	if (led == NULL)
		return SD_LED_NULL_HANDLE_ERROR;
	if (led->tim == NULL)
		return SD_LED_NULL_TIMER_ERROR;
  
	/* Disable update timer interrupt */
	HAL_TIM_Base_Stop_IT(led->up_tim);
  
	/* Set the pattern value in the configuration */
	led->cfg->Pulse = 0;
  
	/* Set the configuration */
	HAL_TIM_PWM_ConfigChannel(led->tim, led->cfg, led->chan);
	
	/* Remove the pattern from the LED */
	led->pattern = NULL;
	led->pattern_next = NULL;
	led->pattern_idx = 0;
	led->pattern_len = 0;
	led->repeat = 0;
    
	/* Enable update timer interrupt */
	HAL_TIM_Base_Start_IT(led->up_tim);
  
	return SD_LED_SUCCESS;
}


/**
 * @brief	LED queue handler 
 *		Execute the LED pattern state machine
 *
 * @param  led:		The LED pattern handle
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_queuehandler(struct sd_led *led)
{
  
	/* Check for NULL pointers for the config and timer handle */
	if (led == NULL) 
		return SD_LED_NULL_HANDLE_ERROR;
	if (led->tim == NULL) 
		return SD_LED_NULL_TIMER_ERROR;
  
	/* For uninitialized patterns, do nothing */
	if (led->pattern == NULL)
		return SD_LED_IDLE;
  
	/* Restart the pattern index if necessary */
	if (led->pattern_idx >= led->pattern_len ) {
		led->pattern_idx = 0;
    
		/* Is the pattern repeating? */
		if (!(led->repeat)) {
				led->pattern = NULL;
				led->pattern_len = 0;
				return SD_LED_SUCCESS;  /* Return without configuring the PWM */
		} else if (led->pattern_next != NULL)  {
			/* Is there a new pattern to start? */
			led->pattern = led->pattern_next;
			led->pattern_next = NULL;
		}
	}
  
	/* Set the pattern value in the configuration */
	led->cfg->Pulse = led->pattern[led->pattern_idx++];
  
	/* Set the configuration */
	HAL_TIM_PWM_ConfigChannel(led->tim, led->cfg, led->chan);
  
	/* Set the PWM Output */
	HAL_TIM_PWM_Start(led->tim, led->chan);
  
	return SD_LED_SUCCESS;
}


/**
 * @brief	LED set pattern
 *		Set pattern for LED output
 *
 * @param  led:		The LED pattern handle
 * @param  pattern:	The pattern to use for the LED
 * @param  len:		The length of the pattern
 * @param  repeat: 	Set the pattern to repeat after ending
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_set_pattern(struct sd_led *led, 
					uint32_t *pattern,
					uint32_t len,
					uint8_t repeat)
{
	/* Check for NULL pointers for the config and timer handle */
	if ((led == NULL) || (led->tim == NULL)) 
		return SD_LED_NULL_HANDLE_ERROR;
  
	/* Disable the timer interrupt */
	//HAL_TIM_Base_Stop_IT( LED->Update_HTIMx );
  
	led->pattern = pattern;
	led->pattern_len = len;
	led->pattern_idx = 0;
	led->repeat = repeat;
  
	/* Reenable the timer */
	//HAL_TIM_Base_Start_IT( LED->Update_HTIMx );
 
	return SD_LED_SUCCESS;
}
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_led.c]
 */
