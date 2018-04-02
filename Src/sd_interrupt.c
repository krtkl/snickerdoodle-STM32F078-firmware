/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_interrupt.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	Interrupt Handlers
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
#include "stm32f0xx.h"

#include "sd_gpio.h"
#include "sd_adc.h"
#include "sd_tim.h"
#include "usbd_cdc.h"
#include "sd_usbd_cdc_if.h"
#include "sd_led.h"
#include "sd_led_pattern.h"
#include "sd_buffer.h"
#include "sd_uart.h"
#include "sd_spi.h"
#include "sd_spi_bridge.h"
#include "sd_button.h"
#include "sd_platform.h"
#include "sd_interrupt.h"


/**
 * @defgroup	SD_Interrupt Interrupt
 * @brief	Interrupt handlers for SysTick and peripheral interrupts
 * @{
 */

extern PCD_HandleTypeDef hpcd_USB_FS;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc;

/**
 * @defgroup SD_Interrupt_Functions Interrupt Functions
 * @{
 */

/**
 * @brief	SysTick Handler
 *		This function handles System tick timer.
 */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


/**
 * @brief	ADC Interrupt Handler
 *		This function handles ADC and COMP interrupts (COMP interrupts
 *		through EXTI lines 21 and 22).
 */
void ADC1_COMP_IRQHandler(void)
{
	sd_adc_irqhandler(&adc_dev);
}


/**
 * @brief	External Interrupt Handler
 *		This function handles EXTI line 2 and 3 interrupts
 */
void EXTI2_3_IRQHandler(void)
{
	sd_gpio_exti_irqhandler(ZYNQ_DONE_Pin);		/** ZYNQ_DONE */
}


/**
 * @brief	External Interrupt Handler
 *		This function handles EXTI line 4 to 15 interrupts.
 */
void EXTI4_15_IRQHandler(void)
{
	sd_gpio_exti_irqhandler(ZYNQ_INIT_Pin);		/** ZYNQ_INIT */
	sd_gpio_exti_irqhandler(JA1_P2_Pin);		/** JA1_P2 */
	sd_gpio_exti_irqhandler(JA2_P2_Pin);		/** JA2_P2 */
	sd_gpio_exti_irqhandler(JB1_P2_Pin);		/** JB1_P2 */
	sd_gpio_exti_irqhandler(JB2_P2_Pin);		/** JB2_P2 */
	sd_gpio_exti_irqhandler(JC1_P2_Pin);		/** JC1_P2 */
}


/**
 * @brief	Timer 7 Interrupt Handler
 *		This function handles TIM7 global interrupt.
 */
void TIM7_IRQHandler(void)
{
	HAL_GPIO_WritePin(J2_P3_GPIO_Port, J2_P3_Pin, GPIO_PIN_SET);

	sd_led_queuehandler(&app_led);
	sd_led_queuehandler(&bluetooth_led);
	sd_led_queuehandler(&usb_led);
	sd_led_queuehandler(&fault_led);
	
	sd_button_run(&reset_button);
	
//	sd_adc_monitor(&adc_dev);
	//sd_button_run(&select_button);
	
	HAL_TIM_IRQHandler(&htim7);

	/* Toggle watchdog pin */
	HAL_GPIO_TogglePin(WDI_GPIO_Port, WDI_Pin);
	
	/* Bridge UART <--> USB */
	sd_uart_usb_transmit(&uart1_dev);
	
	HAL_GPIO_WritePin(J2_P3_GPIO_Port, J2_P3_Pin, GPIO_PIN_RESET);
}

/**
 * @brief	SPI 2 Interrupt Handler
 *		This function handles SPI2 global interrupt.
 */
void SPI2_IRQHandler(void)
{
	sd_spbr_irqhandler(&spi2_spbr);
}

/**
 * @brief	USART 1 Interrupt Handler
 *		This function handles USART1 global interrupt / USART1 wake-up
 *		interrupt through EXTI line 25.
 */
void USART1_IRQHandler(void)
{
	HAL_TIM_Base_Stop_IT(&htim7);		/* Stop update timer */
	sd_uart_irqhandler(&uart1_dev);		/* Handle interrupt */
	HAL_TIM_Base_Start_IT(&htim7);		/* Restart update timer */
}

/**
 * @brief	USART 2 Interrupt Handler
 *		This function handles USART2 global interrupt / USART2 wake-up
 *		interrupt through EXTI line 26.
 */
void USART2_IRQHandler(void)
{
	HAL_TIM_Base_Stop_IT(&htim7);		/* Stop update timer */
	sd_led_blink(&bluetooth_led);		/* Blink indicator */
	sd_uart_irqhandler(&uart2_dev);		/* Handle interrupt */
	HAL_TIM_Base_Start_IT(&htim7);		/* Restart update timer */
}

/**
 * @brief	USART 2 Interrupt Handler
 *		This function handles USART2 global interrupt / USART2 wake-up
 *		interrupt through EXTI line 26.
 */
void USART3_4_IRQHandler(void)
{

}

/**
 * @brief	USB Interrupt Handler
 *		This function handles USB global interrupt / USB wake-up
 *		interrupt through EXTI line 18.
 */
void USB_IRQHandler(void)
{
	sd_led_blink(&usb_led);			/* Blink indicator LED */
	HAL_PCD_IRQHandler(&hpcd_USB_FS);	/* Handle USB interrupt */
}
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_interrupt.c]
 */
