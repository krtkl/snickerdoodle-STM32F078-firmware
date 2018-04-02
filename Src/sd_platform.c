/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_platform.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	Platform Controller Functions 
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
#include "usb_device.h"
#include "sd_gpio.h"
#include "usbd_cdc.h"
#include "sd_usbd_cdc_if.h"
#include "sd_buffer.h"
#include "sd_uart.h"
#include "sd_i2c.h"
#include "sd_spi.h"
#include "sd_led.h"
#include "sd_led_pattern.h"
#include "sd_button.h"
#include "sd_platform.h"

/**
 * @defgroup	SD_Platform Platform Configuration
 * @brief	Platform specific board level configuration and control
 * @{
 */

#define SD_PLATFORM_NORMAL_BOOT		0x00	/**< Not boot select, boot normally */
#define SD_PLATFORM_DFU_BOOT		0x03	/**< Both buttons pressed */


/**
 * @defgroup SD_Platform_Variables Platform Variables
 * @brief snickerdoodle platform variables
 * @{
 */
struct sd_cbuf uart1_rx_cbuff;			/**< UART1 circular buffer */
struct sd_cbuf uart2_rx_cbuff;			/**< UART2 circular buffer */

struct sd_doub_buff cons_dbuff;				/**< Console double buffer */

uint8_t uart1_rx_buff[CONS_UART_RX_BUFF_SIZE];		/**< UART1 receive character buffer */
uint8_t uart2_rx_buff[CONS_UART_RX_BUFF_SIZE];		/**< UART2 receive character buffer */

uint8_t cons_buff[2][CONS_UART_RX_BUFF_SIZE];		/**< Console double buffer */

//struct sd_hci_dev wl18xxbt_dev;				/**< Bluetooth HCI device */

uint8_t hci_params_buff[CONS_UART_RX_BUFF_SIZE];	/**< HCI parameter buffer */
/**
 * SD_Platform_Variables
 * @}
 */

/**
 * @defgroup SD_Platform_Boot_Config Zynq Boot Configuration
 * @brief Zynq processing subsystem boot configuration
 * @{
 */



/**
 * @brief	Zynq Clock Enable
 *		Enables the main clock source for the Zynq APU
 * @param enable:	State to set for clock
 * @retval None
 */
void sd_zynq_clk_enable(uint8_t enable)
{
	GPIO_PinState en = enable ? GPIO_PIN_SET : GPIO_PIN_RESET;
	
	HAL_GPIO_WritePin(ZYNQ_CLK_EN_GPIO_Port, 
			  ZYNQ_CLK_EN_Pin, en);

}


/**
 * @brief	Zynq JTAG Enable
 *		Enables the JTAG interface for programming the PS or PL
 *
 * @param enable:	State to set for JTAG interface
 * @retval None
 */
void sd_zynq_jtag_enable(uint8_t enable)
{
	GPIO_PinState en = enable ? GPIO_PIN_SET : GPIO_PIN_RESET;

	HAL_GPIO_WritePin(ZYNQ_JTAG_NRST_GPIO_Port, 
			  ZYNQ_JTAG_NRST_Pin, en);
}


/**
 * @brief	Wireless Clock Enable
 *		Enable the wireless module 32kHz clock source
 *
 * @param enable:	State to set for clock
 * @retval None
 */
void sd_wl_clk_enable(uint8_t enable)
{
	GPIO_PinState en = enable ? GPIO_PIN_SET : GPIO_PIN_RESET;

	HAL_GPIO_WritePin(WL_32KHZ_CLK_EN_GPIO_Port, 
			  WL_32KHZ_CLK_EN_Pin, en);
}


/**
 * @brief	Bluetooth Enable
 *		Enable the Bluetooth interface on the wireless module
 *
 * @param enable:	State to set for Bluetooth interface
 * @retval None
 */
void sd_bt_enable(uint8_t enable)
{
	GPIO_PinState en = enable ? GPIO_PIN_SET : GPIO_PIN_RESET;
	
	HAL_GPIO_WritePin(WL18xx_BT_EN_GPIO_Port, 
			  WL18xx_BT_EN_Pin, en);
}




/**
 * @brief	Zynq Boot Config
 *		Outputs boot select pins to zynq for desired boot media source
 *
 * @param  boot:       Value defining boot pin outputs
 * @retval None
 */
void sd_zynq_boot_config(enum sd_zynq_boot boot)
{
	GPIO_PinState mio4_out, mio5_out;
  
	switch (boot) {
	case SD_ZYNQ_BOOT_JTAG:
		mio4_out = GPIO_PIN_RESET;      /* 0 */
		mio5_out = GPIO_PIN_RESET;      /* 0 */
		break;
	
	case SD_ZYNQ_BOOT_NAND:
		mio4_out = GPIO_PIN_SET;	/* 1 */
		mio5_out = GPIO_PIN_RESET;      /* 0 */
		break;
	
	case SD_ZYNQ_BOOT_QSPI:
		mio4_out = GPIO_PIN_RESET;      /* 0 */
		mio5_out = GPIO_PIN_SET;	/* 1 */
		break;
	
	case SD_ZYNQ_BOOT_SD:
		mio4_out = GPIO_PIN_SET;	/* 1 */
		mio5_out = GPIO_PIN_SET;	/* 1 */
		break;
	}
 
	/* Write boot select pins */
	HAL_GPIO_WritePin(MIO4_BOOT_SELECT_GPIO_Port,
			  MIO4_BOOT_SELECT_Pin,
			  mio4_out);
	
	HAL_GPIO_WritePin(MIO5_BOOT_SELECT_GPIO_Port, 
			  MIO5_BOOT_SELECT_Pin,
			  mio5_out);
}


/**
 * @brief	Zynq Reset
 * 		Reset the Zynq APU
 *
 * @param  None
 * @retval None
 */
void sd_zynq_reset(void)
{
	sd_zynq_disable();
	
	/* A Short wait */
	HAL_Delay(100);
	
	sd_zynq_enable();
}


/**
 * @brief	Disable Zynq Power
 *		Disable the Zynq power rails by setting the enable pin low
 *
 * @param  butt:	Button used to trigger this function
 * @retval None
 */
void sd_zynq_disable(void)
{
	HAL_GPIO_WritePin(ZYNQ_POWER_EN_GPIO_Port,
				ZYNQ_POWER_EN_Pin, GPIO_PIN_RESET);
	sd_led_rapid(&app_led);
}


/**
 * @brief	Enable Zynq Power
 *		Enable the Zynq power rails by setting the enable pin high
 *
 * @param  butt:	Button used to trigger this function
 * @retval None
 */
void sd_zynq_enable(void)
{
	HAL_GPIO_WritePin(ZYNQ_POWER_EN_GPIO_Port,
				ZYNQ_POWER_EN_Pin, GPIO_PIN_SET);
				
	sd_led_reset(&app_led);
}
/**
 * SD_Platform_Boot_Config 
 * @}
 */


void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	switch (pin) {
		case ZYNQ_DONE_Pin:
			/* Indicate FPGA configuration done */
			sd_led_breathe(&app_led);
			break;
			
		case ZYNQ_INIT_Pin:
			/* Read the pin state */
			if (HAL_GPIO_ReadPin(ZYNQ_INIT_GPIO_Port, 
					     ZYNQ_INIT_Pin) == GPIO_PIN_SET) {
				sd_led_reset(&app_led);
			} else {
				sd_led_rapid(&app_led);
			}
			break;
		
		case JA1_P2_Pin:
		case JA2_P2_Pin:
		case JB1_P2_Pin:
		case JB2_P2_Pin:
		case JC1_P2_Pin:
		
			break;
		
		/* Unknown pin */
		default:
			return;
		}
}


/**
 * @defgroup SD_Platform_WL18xx_Ant WL18xx Antenna Configuration
 * @{
 */

/**
 * @brief	WL18xx Antenna Configuration
 *		Outputs antenna select pins for the wireless module
 *
 * @param  ant:	Value defining antenna configuration (two LSBs used)
 * @retval None
 */
void sd_wl18xx_ant_config(uint8_t ant)
{
	GPIO_PinState ant_select_1, ant_select_2;
 
	/* Check that the antenna selection is valid */
	if (!IS_WL18xx_ANTENNA_MODE(ant))
		return;

	/* Determine antenna select pins from bits */
	ant_select_1 = (ant & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	ant_select_2 = (ant & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET;

	/* Output GPIO selection pins */
	HAL_GPIO_WritePin(ANT_SELECT_1_GPIO_Port, ANT_SELECT_2_Pin,
			  ant_select_1);					       
	HAL_GPIO_WritePin(ANT_SELECT_2_GPIO_Port, ANT_SELECT_2_Pin,
			  ant_select_2);     
}
/**
 * SD_WL18xx_Ant
 * @}
 */


/**
 * @defgroup SD_UART_USB_Bridge UART to USB Bridge
 * @brief snickerdoodle UART to USB bridge functions
 * @{
 */

/**
 * @brief	UART to USB Bridge Initialization
 *		Initialize UART device structure, UART to USB bridge buffer and 
 *		set the UART device to use the (previously initialized) UART
 *		peripheral and newly initialized buffer.
 *
 * @retval State of UART USB bridge after initialization
 */
enum sd_uart_usb_err sd_uart_usb_bridge_init(void)
{
	/* Initialize the UART device(s) */
//	if (sd_uart_dev_init(&uart1_dev) != SD_UART_SUCCESS)
//		return SD_UART_USB_ERROR;
//	
//	if (sd_uart_dev_init(&uart2_dev) != SD_UART_SUCCESS)
//		return SD_UART_USB_ERROR;
//  
//	/* Initialize the UART <--> USB Buffer */
//	if (sd_char_buff_init(&uart1_rx_cbuff, 
//			      CONS_UART_RX_BUFF_SIZE, 
//			      uart1_rx_buff) != SD_BUFF_SUCCESS)
//		return SD_UART_USB_ERROR;
//	
//	if (sd_char_buff_init(&uart2_rx_cbuff, 
//			      CONS_UART_RX_BUFF_SIZE,
//			      uart2_rx_buff) != SD_BUFF_SUCCESS)
//		return SD_UART_USB_ERROR;
//  
//	/* Set the UART device with the peripheral and buffer*/
//	if (sd_uart_rx_init(&uart1_dev, &huart1, &uart1_rx_cbuff) != SD_BUFF_SUCCESS)
//		return SD_UART_USB_ERROR;
//	
//	if (sd_uart_rx_init(&uart2_dev, &huart2, &uart2_rx_cbuff) != SD_BUFF_SUCCESS)
//		return SD_UART_USB_ERROR;

	return SD_UART_USB_SUCCESS;
}


/**
 * @brief	UART to USB Bridge Transmission
 *		Transmit the buffer that has been recieved on the bridged UART
 *		over USB communications device
 *
 * @param  uart_dev:   UART device structure that is bridged
 * @retval SD_UART_USB_SUCCESS on success, error state otherwise
 */
inline enum sd_uart_usb_err sd_uart_usb_transmit(struct sd_uart_dev *dev)
{
	struct sd_cbuf *rxbuff = dev->rx_buff;
	static uint8_t txbuff[CONS_UART_RX_BUFF_SIZE];
	static uint32_t idx;
	uint8_t state;

	while (rxbuff->len && (idx < CONS_UART_RX_BUFF_SIZE)) {
		/* Check for the end of the buffer and wrap */
		if (rxbuff->out >= rxbuff->size)
			rxbuff->out = 0;
    
		/* Put the character in the secondary buffer */
		txbuff[idx++] = rxbuff->buff[rxbuff->out++];
    
		/* Check for the end of the buffer and wrap */
		if (rxbuff->out >= rxbuff->size)
			rxbuff->out = 0;
    
		if (--rxbuff->len <= 0)
			rxbuff->len = 0;
    
		/* Check if the UART interrupt has been triggered and end this loop */
		if ((__HAL_UART_GET_IT(dev->uart, UART_IT_RXNE) != RESET) && 
		    (__HAL_UART_GET_IT_SOURCE(dev->uart, UART_IT_RXNE) != RESET))
			return SD_UART_USB_SUCCESS;  /* End the loop here so that the UART can operate */
	}

	/* Transmit if there is anything to transmit */
	if (idx) {
		state = sd_cdc_get_state();
		
		if (state == USBD_STATE_CONFIGURED) {
			sd_cdc_transmit(txbuff, idx);
			idx = 0;	/* Reset the index once we've transmitted */
		}
	}

	return SD_UART_USB_SUCCESS;
}
/**
 * SD_UART_USB_Bridge
 * @}
 */


/**
 * @defgroup SD_DFU Device Firmware Upgrade
 * @{
 */

/**
 * @brief	Enter Device Firmware Upgrade Mode
 *		Prepare device to enter system memory by disabling peripheral
 *		clocks, disable PLL, clear pending interrupts and disable
 *		interrupts.
 */
void sd_dfu_start(void) 
{
	void (*boot_jump)(void);
	
	/**
	 * Disable peripheral clocks
	 */
	__GPIOA_CLK_DISABLE();
	__GPIOB_CLK_DISABLE();
	__GPIOC_CLK_DISABLE();
	__GPIOD_CLK_DISABLE();
	__GPIOE_CLK_DISABLE();
	__GPIOF_CLK_DISABLE();
	
	/**
	 * Disable PLL
	 */
	RCC->CR    |= (uint32_t)0x00000001;	/* Set HSION bit */
	RCC->CFGR  &= (uint32_t)0x08FFB80C;	/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCOSEL[2:0], MCOPRE[2:0] and PLLNODIV bits */
	RCC->CR    &= (uint32_t)0xFEF6FFFF;	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR    &= (uint32_t)0xFFFBFFFF;	/* Reset HSEBYP bit */
	RCC->CFGR  &= (uint32_t)0xFFC0FFFF;	/* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
	RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;	/* Reset PREDIV1[3:0] bits */
	RCC->CFGR3 &= (uint32_t)0xFFF0FEAC;	/* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
	RCC->CR2   &= (uint32_t)0xFFFFFFFE;	/* Reset HSI14 bit */
	RCC->CIR    = 0x00000000;		/* Disable all interrupts */
	
	/**
	 * Disable systick
	 */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	
	/**
	 * Run to stack pointer and execute code from system memory
	 */
	__set_MSP(BOOT_STACK_ADDR);
	boot_jump = (void (*)(void))(*((uint32_t *)(SYS_MEM_START_ADDR + 4)));
	boot_jump();
	while (1);
}
/**
 * SD_DFU
 * @}
 */


/**
 * @brief	Get button press state
 */
enum sd_boot_button sd_boot_get_button_state(void)
{
	uint8_t state;
	
	state = ((GPIOE->IDR & 0x30) >> 4);
	
	return (enum sd_boot_button)state;
}


/**
 * @brief	Check if boot timer has expired
 *
 * @param  start:      Timer start tick count (ms)
 * @param  end:	Timer end count (ms)
 * @retval TRUE if the timer has expired, FALSE otherwise
 */
uint8_t sd_boot_timer_expired(uint32_t start, uint32_t end)
{
	return (HAL_GetTick() > (start + end));
}


/**
 * @brief	Boot selection
 */
enum sd_zynq_boot sd_boot_select(void)
{
	enum sd_boot_button button_state;
	uint32_t tickstart = HAL_GetTick();
	
	/* Get initial button state */
	button_state = sd_boot_get_button_state();
	
	/* If the button state changes, boot normally */
	while (sd_boot_get_button_state() == button_state) {
		
		/* Handle the button state */
		switch (button_state) {
		case SD_BOOT_DFU:       
			/* Buttons pressed to enter DFU mode */
			if (sd_boot_timer_expired(tickstart, 4000))
				sd_dfu_start();
			break;
		
		case SD_BOOT_QSPI:
			/* Buttons pressed to boot from QSPI flash */
			if (sd_boot_timer_expired(tickstart, 2000))
				return SD_ZYNQ_BOOT_QSPI;
			break;
			
		case SD_BOOT_JTAG:
			/* Buttons pressed to boot from JTAG */
			if (sd_boot_timer_expired(tickstart, 2000))
				return SD_ZYNQ_BOOT_JTAG;
			break;
			
		case SD_BOOT_DEFAULT:
			/* Immediately boot normally from SD */
			return SD_ZYNQ_BOOT_SD;
		}
		HAL_Delay(10);
	}
	/* Boot from SD card by default */
	return SD_ZYNQ_BOOT_SD;
}
/**
 * SD_Platform
 * @}
 */

/*
 * END OF FILE [sd_platform.c]
 */
