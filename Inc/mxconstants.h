/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


#define MIO5_BOOT_SELECT_Pin 		GPIO_PIN_3
#define MIO5_BOOT_SELECT_GPIO_Port 	GPIOE
#define SMB_I2C_SCL_Pin 		GPIO_PIN_8
#define SMB_I2C_SCL_GPIO_Port 		GPIOB
#define WL_32KHZ_CLK_EN_Pin 		GPIO_PIN_7
#define WL_32KHZ_CLK_EN_GPIO_Port 	GPIOD
#define BT_HCI_RX_Pin 			GPIO_PIN_5
#define BT_HCI_RX_GPIO_Port 		GPIOD
#define MCU_SWCLK_Pin 			GPIO_PIN_14
#define MCU_SWCLK_GPIO_Port 		GPIOA
#define MCU_SWDIO_Pin 			GPIO_PIN_13
#define MCU_SWDIO_GPIO_Port 		GPIOA
#define SELECT_BUTTON_Pin 		GPIO_PIN_4
#define SELECT_BUTTON_GPIO_Port 	GPIOE
#define MIO4_BOOT_SELECT_Pin 		GPIO_PIN_2
#define MIO4_BOOT_SELECT_GPIO_Port 	GPIOE
#define SMB_I2C_SDA_Pin 		GPIO_PIN_9
#define SMB_I2C_SDA_GPIO_Port 		GPIOB
#define ZYNQ_CONSOLE_TX_Pin 		GPIO_PIN_7
#define ZYNQ_CONSOLE_TX_GPIO_Port 	GPIOB
#define ZYNQ_CONSOLE_RX_Pin 		GPIO_PIN_6
#define ZYNQ_CONSOLE_RX_GPIO_Port 	GPIOB
#define BT_HCI_TX_Pin 			GPIO_PIN_6
#define BT_HCI_TX_GPIO_Port 		GPIOD
#define BT_HCI_CTS_Pin 			GPIO_PIN_4
#define BT_HCI_CTS_GPIO_Port 		GPIOD
#define BT_HCI_RTS_Pin 			GPIO_PIN_3
#define BT_HCI_RTS_GPIO_Port 		GPIOD
#define ZYNQ_POWER_EN_Pin 		GPIO_PIN_1
#define ZYNQ_POWER_EN_GPIO_Port 	GPIOD
#define RESET_BUTTON_Pin 		GPIO_PIN_5
#define RESET_BUTTON_GPIO_Port 		GPIOE
#define WDI_Pin 			GPIO_PIN_2
#define WDI_GPIO_Port 			GPIOD
#define ANT_SELECT_2_Pin 		GPIO_PIN_10
#define ANT_SELECT_2_GPIO_Port 		GPIOA
#define ANT_SELECT_1_Pin 		GPIO_PIN_9
#define ANT_SELECT_1_GPIO_Port 		GPIOA
#define RED_Pin 			GPIO_PIN_8
#define RED_GPIO_Port 			GPIOA
#define ORANGE_Pin 			GPIO_PIN_9
#define ORANGE_GPIO_Port 		GPIOC
#define WHITE_Pin 			GPIO_PIN_8
#define WHITE_GPIO_Port 		GPIOC
#define BLUE_Pin 			GPIO_PIN_7
#define BLUE_GPIO_Port 			GPIOC
#define GREEN_Pin 			GPIO_PIN_6
#define GREEN_GPIO_Port 		GPIOC
#define WL_RS232_TX_Pin			GPIO_PIN_4
#define WL_RS232_TX_GPIO_Port		GPIOC
#define WL_RS232_RX_Pin			GPIO_PIN_5
#define WL_RS232_RX_GPIO_Port		GPIOC
#define WL18xx_BT_EN_Pin 		GPIO_PIN_10
#define WL18xx_BT_EN_GPIO_Port 		GPIOF
#define SMB_NRESET_Pin 			GPIO_PIN_15
#define SMB_NRESET_GPIO_Port 		GPIOD
#define JC1_P2_Pin 			GPIO_PIN_14
#define JC1_P2_GPIO_Port 		GPIOD
#define JB2_P2_Pin 			GPIO_PIN_13
#define JB2_P2_GPIO_Port 		GPIOD
#define ZYNQ_JTAG_NRST_Pin 		GPIO_PIN_1
#define ZYNQ_JTAG_NRST_GPIO_Port 	GPIOC
#define ZYNQ_1V2_SENSE_Pin 		GPIO_PIN_2
#define ZYNQ_1V2_SENSE_GPIO_Port 	GPIOC
#define JB1_P2_Pin 			GPIO_PIN_12
#define JB1_P2_GPIO_Port 		GPIOD
#define JA2_P2_Pin 			GPIO_PIN_11
#define JA2_P2_GPIO_Port 		GPIOD
#define JA1_P2_Pin 			GPIO_PIN_10
#define JA1_P2_GPIO_Port 		GPIOD
#define ZYNQ_3V3_SENSE_Pin 		GPIO_PIN_3
#define ZYNQ_3V3_SENSE_GPIO_Port 	GPIOC
#define MAIN_3V3_SENSE_Pin 		GPIO_PIN_2
#define MAIN_3V3_SENSE_GPIO_Port 	GPIOA
#define J2_P4_Pin 			GPIO_PIN_5
#define J2_P4_GPIO_Port 		GPIOA
#define ZYNQ_CLK_EN_Pin 		GPIO_PIN_8
#define ZYNQ_CLK_EN_GPIO_Port 		GPIOD
#define ZYNQ_SPI_MOSI_Pin 		GPIO_PIN_15
#define ZYNQ_SPI_MOSI_GPIO_Port 	GPIOB
#define ZYNQ_SPI_MISO_Pin 		GPIO_PIN_14
#define ZYNQ_SPI_MISO_GPIO_Port 	GPIOB
#define ZYNQ_SPI_CLK_Pin 		GPIO_PIN_13
#define ZYNQ_SPI_CLK_GPIO_Port 		GPIOB
#define VIN_SENSE_Pin 			GPIO_PIN_0
#define VIN_SENSE_GPIO_Port 		GPIOA
#define MAIN_1V8_SENSE_Pin 		GPIO_PIN_3
#define MAIN_1V8_SENSE_GPIO_Port 	GPIOA
#define VDDIO2_3V3_SENSE_Pin 		GPIO_PIN_6
#define VDDIO2_3V3_SENSE_GPIO_Port 	GPIOA
#define CRYPTO_I2C_SCL_Pin 		GPIO_PIN_10
#define CRYPTO_I2C_SCL_GPIO_Port 	GPIOB
#define CRYPTO_I2C_SDA_Pin 		GPIO_PIN_11
#define CRYPTO_I2C_SDA_GPIO_Port 	GPIOB
#define ZYNQ_SPI_NSS_Pin 		GPIO_PIN_12
#define ZYNQ_SPI_NSS_GPIO_Port 		GPIOB
#define VUSB_SENSE_Pin 			GPIO_PIN_1
#define VUSB_SENSE_GPIO_Port 		GPIOA
#define J2_P3_Pin 			GPIO_PIN_4
#define J2_P3_GPIO_Port 		GPIOA
#define ZYNQ_1V0_SENSE_Pin 		GPIO_PIN_0
#define ZYNQ_1V0_SENSE_GPIO_Port 	GPIOB
#define ZYNQ_1V8_SENSE_Pin 		GPIO_PIN_1
#define ZYNQ_1V8_SENSE_GPIO_Port 	GPIOB
#define ZYNQ_POWER_GOOD_Pin 		GPIO_PIN_7
#define ZYNQ_POWER_GOOD_GPIO_Port 	GPIOE
#define ZYNQ_INIT_Pin			GPIO_PIN_6
#define ZYNQ_INIT_GPIO_Port		GPIOE
#define ZYNQ_DONE_Pin			GPIO_PIN_2
#define ZYNQ_DONE_GPIO_Port		GPIOF
#define ZYNQ_PROG_Pin			GPIO_PIN_9
#define ZYNQ_PROG_GPIO_Port		GPIOF
/**
 * @}
 */ 

/**
 * @}
 */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
