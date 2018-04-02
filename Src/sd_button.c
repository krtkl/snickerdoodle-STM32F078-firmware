/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_button.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 June 16
 * @brief	Button handling 
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

#include "sd_button.h"

/**
 * @defgroup	SD_Button Button Handling
 * @brief	Handles button press debouncing and event callback execution
 * @{
 */


/**
 * @defgroup SD_Button_Variables Button Variables
 * @{
 */
struct sd_button reset_button;				/**< Reset button */
struct sd_button select_button;				/**< Select button */
/**
 * @}
 */


/**
 * @defgroup SD_Button_Functions Button Functions
 * @{
 */

/**
 * @brief 	Initialize Button Port/Pin and State Machine
 *
 * @param  butt:	Button to initialize
 * @param  port:	GPIO port of button
 * @param  pin:	GPIO pin of button
 * @param  up_to:	Button up debounce timeout
 * @param  up_cb:	Button up callback function
 * @param  down_to:	Button down debounce timeout
 * @param  down_cb:	Button down callback function
 * @retval SD_BUTTON_SUCCESS on success, error code otherwise
 */
enum sd_button_err sd_button_init(struct sd_button *butt,
					GPIO_TypeDef *port,
					uint16_t pin,
					uint32_t up_to,
					void (*up_cb)(void),
					uint32_t down_to,
					void (*down_cb)(void))
{
	butt->port = port;
	butt->pin = pin;
	butt->state = SD_BUTTON_IDLE;
	
	butt->up_to = up_to;
	butt->up_cb = up_cb;
	
	butt->down_to = down_to;
	butt->down_cb = down_cb;
	
	return SD_BUTTON_SUCCESS;
}


/**
 * @brief	Run Button State Machine
 *
 * @param  butt:	Button to run state machine
 * @retval SD_BUTTON_SUCCESS on success, error code otherwise
 */
enum sd_button_err sd_button_run(struct sd_button *butt)
{
	switch (butt->state) {
	case SD_BUTTON_IDLE:	
		if (butt->port->IDR & butt->pin) {
			/* The button is not pressed */
		} else {
			/* Record the timer value before transitioning */
			butt->tick = HAL_GetTick();
			butt->state = SD_BUTTON_DOWN_WAIT;	
		}
		break;
	
	case SD_BUTTON_DOWN_WAIT:
		if (butt->port->IDR & butt->pin) {
			/* Debounce was unsuccessful. Return to idle */
			butt->state = SD_BUTTON_IDLE;
		} else {
			/* Check if the timer has expired */
			if (HAL_GetTick() > (butt->tick + butt->down_to)) {
				butt->down_cb();
				butt->state = SD_BUTTON_DOWN_DEB;
			}
		}
			
		break;
	
	case SD_BUTTON_DOWN_DEB:
		if (butt->port->IDR & butt->pin) {
			/* The button has been released */
			butt->tick = HAL_GetTick();
			butt->state = SD_BUTTON_UP_WAIT;
		} else {
			/* The button is still pressed */
		}
		break;
	
	case SD_BUTTON_UP_WAIT:
		if (butt->port->IDR & butt->pin) {
			/* The button detected as released */
			if (HAL_GetTick() > (butt->tick + butt->up_to)) {
				/* Button debounce successful */
				butt->up_cb();
				butt->state = SD_BUTTON_IDLE;
			}
		} else {
			/* The button was detected as pressed */
			butt->state = SD_BUTTON_DOWN_DEB;
		}
		break;
	}
	return SD_BUTTON_SUCCESS;
}
/**
 * SD_Button_Functions
 * @}
 */

/**
 * SD_Button
 * @}
 */

/*
 * END OF FILE [sd_button.c]
 */
