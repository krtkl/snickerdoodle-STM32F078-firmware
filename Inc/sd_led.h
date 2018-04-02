/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_led.h
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

#ifndef __SD_LED_H
#define __SD_LED_H
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup SD_LED
 * @{
 */


/**
 * @addtogroup SD_LED_Exported_Variables
 * @{
 */
extern struct sd_led fault_led;
extern struct sd_led usb_led;
extern struct sd_led bluetooth_led;
extern struct sd_led app_led;
extern struct sd_led wireless_led;
/**
 * @}
 */


/**
 * @defgroup SD_LED_Types LED Types
 * @{
 */
  
/**
 * @enum  sd_led_err
 * @brief LED error status enumeration
 */
enum sd_led_err {
	SD_LED_SUCCESS,				/**< Successful execution */
	SD_LED_IDLE,				/**< LEDs waiting */
	SD_LED_NULL_HANDLE_ERROR,		/**< NULL LED handle error */
	SD_LED_NULL_TIMER_ERROR,		/**< NULL timer handle error */
	SD_LED_ERROR				/**< General error */
};
  

/**
 * @enum  sd_led_state
 * @brief LED state
 */
enum sd_led_state {
	SD_LED_NOT_READY,
	SD_LED_INITIALIZED,
	SD_LED_RUNNING,
};  

/**
 * @struct sd_led
 * @brief  LED pattern configuration structure
 */
struct sd_led {
	TIM_HandleTypeDef	*tim;		/**< LED timer handle */
	TIM_OC_InitTypeDef	*cfg;		/**< LED PWM configuration */
	uint32_t		chan;		/**< LED timer channel */
	TIM_HandleTypeDef	*up_tim;	/**< Update timer handle */
	uint32_t		*pattern;	/**< Pattern for pulses */
	uint32_t		*pattern_next;	/**< Next pattern to run */
	uint32_t		pattern_len;	/**< Pattern length */
	uint32_t		pattern_idx;	/**< Index of current pattern */
	uint8_t			repeat;		/**< Indicates repeated pattern */
	enum sd_led_state	state;		/**< State of the LED */
};
/**
 * @}
 */

/**
 * @addtogroup SD_LED_Functions
 * @{
 */
enum sd_led_err sd_led_init		(struct sd_led *led,
						TIM_HandleTypeDef *tim,
						uint32_t chan,
						uint32_t *pattern,
						uint32_t len,
						TIM_HandleTypeDef *up_tim);
enum sd_led_err sd_led_set_pattern	(struct sd_led *led,
						uint32_t *pattern,
						uint32_t len,
						uint8_t rep);
enum sd_led_err sd_led_enable		(struct sd_led *led,
						uint8_t en);
enum sd_led_err sd_led_reset		(struct sd_led *led);
enum sd_led_err sd_led_queuehandler	(struct sd_led *led);
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
#error "Multiple inclusion of sd_led.h"
#endif /* __SD_LED_H */

/*
 * END OF FILE [sd_led.h]
 */
