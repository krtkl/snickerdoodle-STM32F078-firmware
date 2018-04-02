/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_button.h
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
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

#ifndef __SD_BUTTON_H
#define __SD_BUTTON_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SD_Button
 * @{
 */


/**
 * @addtogroup SD_Button_Variables
 * @{
 */
extern struct sd_button	reset_button;
extern struct sd_button select_button;
/**
 * @}
 */
  
/**
 * @defgroup SD_Button_Types Button Types
 * @{
 */

/**
 * @enum  sd_button_err
 * @brief Button error enumeration
 */
enum sd_button_err {
	SD_BUTTON_SUCCESS,		/**< Successful execution */
	SD_BUTTON_ERROR			/**< General execution error */
};

/**
 * @enum   sd_button_state
 * @brief  Button state enumeration
 */
enum sd_button_state {
	SD_BUTTON_IDLE,			/**< Button is in default position */
	SD_BUTTON_DOWN_WAIT,		/**< Button has been pressed */
	SD_BUTTON_DOWN_DEB,		/**< Button press has been debounced */
	SD_BUTTON_UP_WAIT,		/**< Button has been released */
};

/**
 * @struct sd_button
 * @brief  Button structure
 */
struct sd_button {
	GPIO_TypeDef		*port;			/**< Button GPIO port */
	uint16_t		pin;			/**< Button GPIO pin */
	enum sd_button_state	state;			/**< Button state */
	uint32_t		tick;			/**< SysTick value for state entry */
	uint32_t		up_to;			/**< Debounce/timeout for button up */
	uint32_t		down_to;		/**< Debounce/timeout for button down */
	void			(*down_cb)(void);	/**< Button down callback */
	void			(*up_cb)(void);		/**< Button up callback */
};
/**
 * @}
 */

/**
 * @addtogroup SD_Button_Functions
 * @{
 */
enum sd_button_err sd_button_init	(struct sd_button *butt,
						GPIO_TypeDef *port,
						uint16_t pin,
						uint32_t up_to,
						void (*up_cb)(void),
						uint32_t down_to,
						void (*down_cb)(void));
enum sd_button_err sd_button_run	(struct sd_button *butt);
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
#error "Multiple inclusion of sd_button.h"
#endif /* __SD_BUTTON_H */

/*
 * END OF FILE [sd_button.h]
 */
