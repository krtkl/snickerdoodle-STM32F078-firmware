/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_adc.h
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 June 28
 * @brief	Analog to Digital Conversion
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

#ifndef __SD_ADC_H
#define __SD_ADC_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SD_ADC
 * @{
 */

/**
 * @defgroup SD_ADC_Defines ADC Definitions
 * @{
 */

/**
 * @}
 */


/**
 * @addtogroup SD_ADC_Exported_Variables
 * @{
 */
extern struct sd_adc_dev adc_dev;

/**
 * @}
 */


/**
 * @defgroup SD_ADC_Types ADC Types
 * @{
 */

/**
 * @enum sd_adc_err
 */
enum sd_adc_err {
	SD_ADC_SUCCESS,
	SD_ADC_ERROR
};


/**
 * @struct sd_adc_dev
 */
struct sd_adc_dev {
	ADC_HandleTypeDef	*adc;		/**< ADC handle */
	uint16_t		error;		/**< Error code */
	struct sd_adc_chan	*chan;		/**< Channels */
	uint16_t		nchan;		/**< Number of channels */
};


/**
 * @struct sd_adc_chan
 */
struct sd_adc_chan {
	uint16_t		chan;		/**< ADC channel */
	uint16_t		high_thresh;	/**< Upper bound */
	uint16_t		low_thresh;	/**< Lower bound */
	void			(*out_win_cb)(struct sd_adc_chan *chan);	/* In threshold window callback */
	void			(*in_win_cb)(struct sd_adc_chan *chan);		/* Out of threshold window callback */
};


/**
 * @}
 */

/**
 * @addtogroup SD_ADC_Functions
 * @{
 */
void sd_adc_init			(void);
void sd_adc_monitor			(struct sd_adc_dev *dev);
void sd_adc_monitor_enable		(void);
void sd_adc_watchdog_enable		(void);
void sd_adc_print			(void);
void sd_adc_irqhandler			(struct sd_adc_dev *dev);
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
#error "Multiple inclusion of sd_adc.h"
#endif /* __SD_ADC_H */

/*
 * END OF FILE [sd_adc.h]
 */
