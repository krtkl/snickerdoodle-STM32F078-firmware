/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_adc.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 June 30
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

/*------------------------------- Includes -----------------------------------*/
#include "stm32f0xx_hal.h"

#include "std_defs.h"
#include "sd_adc.h"
#include "sd_led.h"
#include "sd_led_pattern.h"

#include "usbd_cdc.h"
#include "sd_usbd_cdc_if.h"


struct sd_adc_bound {
	uint16_t	upper;
	uint16_t	lower;
};

/**
 * @defgroup	SD_ADC ADC
 * @brief	ADC configuration and control
 * @{
 */
struct sd_adc_cmd {
	const char		*name;
	uint16_t		chan;
	double			scale;
	uint16_t		meas;
	struct sd_adc_bound	limit;
	uint8_t			monitor;
};


#define ADC_VREF				(3.300F)
#define ADC_MAX 				(4095U)
#define NADC_CHANNELS				(9U)

/**
 * @defgroup	SD_ADC_Private_Variables ADC Private Variables
 * @{
 */
static ADC_HandleTypeDef hadc;


static uint8_t adc_print_buf[256];

static struct sd_adc_cmd adc_tab[NADC_CHANNELS] = {
	/* COMMAND		CHANNEL		SCALE	*/
	{ "VIN_SENSE",		ADC_CHANNEL_0,	6.0 / 1.0,	0,	4095,	0,	0	},
	{ "VUSB_SENSE",		ADC_CHANNEL_1,	2.0 / 1.0,	0,	4095,	0,	0	},
	{ "MAIN_3V3_SENSE",	ADC_CHANNEL_2,	1.2 / 1.0,	0,	3583,	3242,	0	},
	{ "MAIN_1V8_SENSE",	ADC_CHANNEL_3,	1.0 / 1.0,	0,	2345,	2122,	0	},
	{ "VDDIO2_3V3_SENSE",	ADC_CHANNEL_6,	1.2 / 1.0,	0,	3583,	3242,	0	},
	{ "ZYNQ_1V0_SENSE",	ADC_CHANNEL_8,	1.0 / 1.0,	0,	1303,	1179,	0	},
	{ "ZYNQ_1V8_SENSE",	ADC_CHANNEL_9,	1.0 / 1.0,	0,	2345,	2122,	0	},
	{ "ZYNQ_1V2_SENSE",	ADC_CHANNEL_12,	1.0 / 1.0,	0,	1564,	1415,	0	},
	{ "ZYNQ_3V3_SENSE",	ADC_CHANNEL_13,	1.2 / 1.0,	0,	3583,	3242,	0	},
};

static struct sd_adc_cmd *adc_chan = adc_tab;

/**
 * @}
 */

/**
 * @defgroup	SD_ADC_Exported_Variables ADC Exported Variables
 * @{
 */
struct sd_adc_dev adc_dev;
/**
 * @}
 */

/**
 * @addtogroup SD_ADC_Functions ADC Functions
 * @{
 */

enum sd_adc_err sd_adc_dev_init		(struct sd_adc_dev *dev,
					 ADC_HandleTypeDef *adc);


/* ADC init function */
void sd_adc_init(void)
{
	/**
	 * Configure the global features of the ADC (Clock, Resolution, Data
	 * Alignment and number of conversion)
	 */
	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC;
	hadc.Init.Resolution = ADC_RESOLUTION12b;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	hadc.Init.EOCSelection = EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = OVR_DATA_PRESERVED;
	HAL_ADC_Init(&hadc);
	
	sd_adc_dev_init(&adc_dev, &hadc);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if (hadc->Instance==ADC1) {
		/* Peripheral clock enable */
		__ADC1_CLK_ENABLE();
  
		/**
		 * ### ADC GPIO Configuration
		 * 
		 * Pin/Port	| Channel
		 * -------------|---------------
		 * PA0		| Channel 0
		 * PA1		| Channel 1
		 * PA2		| Channel 2
		 * PA3		| Channel 3
		 * PA6		| Channel 6
		 * PB0		| Channel 8
		 * PB1		| Channel 9 
		 * PC2		| Channel 12
		 * PC3		| Channel 13
		 */
		GPIO_InitStruct.Pin = ZYNQ_1V2_SENSE_Pin |
				      ZYNQ_3V3_SENSE_Pin;
				      
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = MAIN_3V3_SENSE_Pin |
				      VIN_SENSE_Pin |
				      MAIN_1V8_SENSE_Pin |
				      VDDIO2_3V3_SENSE_Pin |
				      VUSB_SENSE_Pin;
    
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = ZYNQ_1V0_SENSE_Pin|ZYNQ_1V8_SENSE_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
		
		HAL_ADCEx_Calibration_Start(hadc);
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
	uint32_t pin;
 
	if (hadc->Instance==ADC1) {
		/* Peripheral clock disable */
		__ADC1_CLK_DISABLE();
  
		pin = ZYNQ_1V2_SENSE_Pin |
		      ZYNQ_3V3_SENSE_Pin;
		HAL_GPIO_DeInit(GPIOC, pin);

		pin = MAIN_3V3_SENSE_Pin |
		      VIN_SENSE_Pin |
		      MAIN_1V8_SENSE_Pin |
		      VDDIO2_3V3_SENSE_Pin |
		      VUSB_SENSE_Pin;
		HAL_GPIO_DeInit(GPIOA, pin);

		pin = ZYNQ_1V0_SENSE_Pin |
		      ZYNQ_1V8_SENSE_Pin;
		HAL_GPIO_DeInit(GPIOB, pin);

	/* Peripheral interrupt Deinit*/
	HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
	}
}


/**
 * @brief	ADC Device Initialization
 *
 * @param dev:		ADC device to initialize
 * @param adc:		ADC handle to attach to device
 * @retval SD_ADC_SUCCESS on success, error status otherwise
 */
enum sd_adc_err sd_adc_dev_init(struct sd_adc_dev *dev, ADC_HandleTypeDef *adc)
{
	dev->adc = adc;
	
	/* Set the sampling time and channel selection registers */
	adc->Instance->SMPR = ADC_SAMPLETIME_71CYCLES_5;
	adc_dev.adc->Instance->CHSELR = (1U << ADC_CHANNEL_0) |
				(1U << ADC_CHANNEL_1) |
				(1U << ADC_CHANNEL_2) |
				(1U << ADC_CHANNEL_3) |
				(1U << ADC_CHANNEL_6) |
				(1U << ADC_CHANNEL_8) |
				(1U << ADC_CHANNEL_9) |
				(1U << ADC_CHANNEL_12) |
				(1U << ADC_CHANNEL_13);
	
	/* Start the ADC in interrupt mode */
	HAL_ADC_Start_IT(adc);
	
	return SD_ADC_SUCCESS;
}


/**
 * @brief	ADC Watchdog Enable
 *
 * @retval None
 */
void sd_adc_watchdog_enable(void)
{
	ADC_AnalogWDGConfTypeDef adc_wd_config;

	adc_wd_config.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
	adc_wd_config.Channel = ADC_CHANNEL_2;
	adc_wd_config.ITMode = ENABLE;
	adc_wd_config.HighThreshold = 3583;
	adc_wd_config.LowThreshold = 3242;

	HAL_ADC_AnalogWDGConfig(&hadc, &adc_wd_config);
}


/**
 * @brief	ADC Interrupt Handler
 *		Handle interrupt events on the ADC peripheral
 *
 * @param  dev:	ADC device
 * @retval None
 */
void sd_adc_irqhandler(struct sd_adc_dev *dev) 
{
	ADC_HandleTypeDef *hadc = dev->adc;
  
	/* Check the parameters */
	assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
	assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
	assert_param(IS_ADC_EOC_SELECTION(hadc->Init.EOCSelection));
  
  	/* Check if the analog watchdog triggered interrupt */
	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_AWD) &&
	    __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_AWD)) {
	    
	    /* Disable the Zynq clock */
//	    sd_zynq_clk_enable(FALSE);
	    
	    /* Disable the Zynq power rails */
//	    sd_zynq_disable();
	}
  
	/* Check if an end of conversion triggered interrupt */
	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC) && 
	     __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOC)) {
    
		/* Update state machine on conversion status if not in error state */
		if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL)) {
      
			/* Set ADC state */
			SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC); 
		}
    
		/* Record channel measurement */
		(adc_chan++)->meas = hadc->Instance->DR;
    
		/* Clear regular group conversion flag */
		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
	}
	
	/* Check if end of sequence triggered interrupt */
	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS) && 
	    __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOS)) {
	    
		/* Reset to beginning of table */
		adc_chan = &adc_tab[0];

		/* Clear interrupt flag */
		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOS);
	}
  
	/*------------------- Check overrun interrupt flag -------------------*/
	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_OVR) &&
	    __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_OVR)) {
		/* If overrun is set to overwrite previous data (default setting),        */
		/* overrun event is not considered as an error.                           */
		/* (cf ref manual "Managing conversions without using the DMA and without */
		/* overrun ")                                                             */
		/* Exception for usage with DMA overrun event always considered as an     */
		/* error.                                                                 */
		if ((hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED) ||
		    HAL_IS_BIT_SET(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN)) {
			/* Set ADC error code overrun bit */
			hadc->ErrorCode |= HAL_ADC_ERROR_OVR;
      
			/* Clear ADC overrun flag */
			__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
      
			/* Error callback */ 
			HAL_ADC_ErrorCallback(hadc);
		}
    
		/* Clear the Overrun flag */
		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
	}
}


/**
 * @brief	Print the ADC Values
 */
void sd_adc_print(void)
{
	int len, i;
	double val;
	len = sprintf((char *)adc_print_buf, "\033[2J\033[H");
	
	for (i = 0; i < NADC_CHANNELS; i++) {
		val = adc_tab[i].meas * (ADC_VREF / ADC_MAX);
		val *= adc_tab[i].scale;
		len += sprintf((char *)(&adc_print_buf[len]), "%-18s: %4d %6.4fV\r\n", adc_tab[i].name, adc_tab[i].meas, val);
	}

	sd_cdc_transmit(adc_print_buf, strlen((char *)adc_print_buf));
}


/**
 * @brief	Check ADC Limits
 */
void sd_adc_monitor(struct sd_adc_dev *dev)
{
	int i;
	struct sd_adc_cmd *adc = &adc_tab[0];

	/* Determine which channels are set for monitoring and thresholds */
	for (i = 0; i < NADC_CHANNELS; i++) {
		if (!adc->monitor) {
			adc++;
			continue;
		}
	
		if ((adc->meas > adc->limit.upper) || (adc->meas < adc->limit.lower))
			sd_led_blink(&fault_led);

		adc++;
	}
}
/**
 * @}
 */

/**  
 * @}
 */

/*
 * END OF FILE [sd_adc.c]
 */
