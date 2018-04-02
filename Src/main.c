/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	main.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	Platform Controller Main Program Body
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

/**
 *******************************************************************************
 *
 * @mainpage
 *
 * # Snickerdoodle Platform Controller Firmware
 *
 */

/*------------------------------- Includes -----------------------------------*/
#include "stm32f0xx_hal.h"

#include "std_defs.h"

#include "sd_dac.h"
#include "sd_adc.h"
#include "sd_tim.h"
#include "usb_device.h"
#include "sd_gpio.h"
#include "sd_pwm.h"

#include "usbd_cdc.h"
#include "sd_usbd_cdc_if.h"
#include "sd_buffer.h"
#include "sd_uart.h"
#include "sd_i2c.h"
#include "sd_led.h"
#include "sd_led_pattern.h"
#include "sd_spi.h"
#include "sd_spi_bridge.h"
#include "sd_button.h"
#include "sd_platform.h"


/**
 * @defgroup	SD_Main Main
 * @brief	Main program entry point and system clock configuration
 * @{
 */

void SystemClock_Config(void);


/**
 * @brief	Main Program Entry Point
 *		Initializes each peripheral along with any interrupts and begins
 *		execution of the user application
 */
int main(void)
{
	enum sd_zynq_boot boot;
	
	/**
	 * Reset of all peripherals, Initializes the Flash interface and the
	 * Systick timer.
	 */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize the GPIO interface */
	sd_gpio_init();
	
	/* Set all of the LEDs on */
	HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHITE_GPIO_Port, WHITE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLUE_GPIO_Port, BLUE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ORANGE_GPIO_Port, ORANGE_Pin, GPIO_PIN_RESET);
	
	/* Check the boot configuration */
	boot = sd_boot_select();
  
	/* Initialize all configured peripherals */
	sd_adc_init();

	sd_i2c1_init();
	sd_i2c2_init();

	sd_spi2_init();
	
	sd_tim1_init();
	sd_tim3_init();
	
	sd_tim7_init();
	
	sd_usart1_init();
	sd_usart2_init();

	sd_zynq_boot_config(boot);	 
  
	/* Initialize the UART buffers */					  
	sd_char_buff_init(&uart2_rx_cbuff, CONS_UART_RX_BUFF_SIZE, uart2_rx_buff);
	
	/* Set the UART devices with the buffers and prepare to receive */
	sd_char_buff_init(&uart1_rx_cbuff, CONS_UART_RX_BUFF_SIZE, uart1_rx_buff);
	sd_uart_rx_init(&uart1_dev, &uart1_rx_cbuff);

	/* Initialize the USB device */
	MX_USB_DEVICE_Init();
 
	/* Initialize buttons and handlers */
	sd_button_init(&reset_button,
			RESET_BUTTON_GPIO_Port,
			RESET_BUTTON_Pin,
			20,
			sd_zynq_enable,
			2000,
			sd_zynq_disable);
			
	/* Configure the antenna */
	sd_wl18xx_ant_config(WL18xx_ANTENNA_SMA);
	
	/* Enable wireless clock */
	sd_wl_clk_enable(TRUE);
	
	sd_bt_enable(TRUE);
	
	/* Enable I2C switch */
	HAL_GPIO_WritePin(SMB_NRESET_GPIO_Port, 
			  SMB_NRESET_Pin, GPIO_PIN_SET);

	/* Start USB, button and LED update timer interrupt */
	HAL_TIM_Base_Start_IT(&htim7);
  
    	/* Enable Zynq JTAG */
	sd_zynq_jtag_enable(TRUE);
	
	/**
	 * @todo Check the main power supplies here before attempting to boot
	 * the Zynq
	 */
	
	/* Short delay */
	HAL_Delay(20);		
	
	/* Enable Zynq power */
	sd_zynq_enable();
	
	/* Poll until power good signal is asserted */
	while (HAL_GPIO_ReadPin(ZYNQ_POWER_GOOD_GPIO_Port,
				ZYNQ_POWER_GOOD_Pin) != GPIO_PIN_SET)
		HAL_Delay(10);

	/* Enable Zynq clock */
	sd_zynq_clk_enable(TRUE);

	/* Infinite loop */
	while (1) {
		HAL_Delay(250);
	}
}


/** 
 * @brief	System Clock Configuration
 *		Configure and initialize the main, systick, timer and peripheral
 *		clocks
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSI48;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
				|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;

	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


#ifdef USE_FULL_ASSERT

/**
 * @brief	Reports the name of the source file and the source line number
 * 		where the assert_param error has occurred.
 *
 * @param file:	pointer to the source file name
 * @param line:	assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/**
	 * User can add his own implementation to report the file name and line
	 * number, ex: 
	 * 	printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	 */
}

#endif
/**
 * @}
 */ 

/*
 * END OF FILE [main.c]
 */
