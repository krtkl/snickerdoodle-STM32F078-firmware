/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_usbd_cdc_if.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	USB Communications Device Class Interface Functions
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

#include "sd_buffer.h"
#include "sd_uart.h"
#include "sd_tim.h"

#include "sd_led.h"

#include "usbd_cdc.h"
#include "sd_usbd_cdc_if.h"

/**
 * @defgroup	SD_USB_CDC USB Communications Device Class Interface
 * @brief	USB CDC interface configuration and control
 * @{
 */

/**
 * @defgroup SD_USB_CDC_Private_Defines
 * @{
 */ 
#define APP_RX_DATA_SIZE  1024
#define APP_TX_DATA_SIZE  1024
/**
 * @}
 */ 

/**
 * @defgroup SD_USB_CDC_Private_Variables
 * @{
 */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

USBD_HandleTypeDef  *hUsbDevice_0;
/**
 * @}
 */ 


/**
 * @defgroup SD_USB_CDC_Exported_Variables
 * @{
 */ 
extern USBD_HandleTypeDef hUsbDeviceFS;
/**
 * @}
 */ 
  
/**
 * @defgroup USBD_CDC_Private_FunctionPrototypes
 * @{
 */
static int8_t sd_cdc_init       (void);
static int8_t sd_cdc_deinit     (void);
static int8_t sd_cdc_control    (uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t sd_cdc_receive    (uint8_t *pbuf, uint32_t *len);
/**
 * @}
 */ 
  
USBD_CDC_ItfTypeDef sd_usbd_interface_fops = {
	sd_cdc_init,
	sd_cdc_deinit,
	sd_cdc_control,  
	sd_cdc_receive
};


/**
 * @defgroup SD_USB_CDC_IF_Functions USB CDC Interface Functions
 * @{
 */

/**
 * @brief	CDC_Init_FS
 *		Initializes the CDC media low layer over the FS USB IP
 *
 * @param  None
 * @retval USBD_OK if all operations are successful else USBD_FAIL or USBD_BUSY
 */
static int8_t sd_cdc_init(void)
{
	hUsbDevice_0 = &hUsbDeviceFS;
	
	/* Set Application Buffers */
	USBD_CDC_SetTxBuffer(hUsbDevice_0, UserTxBufferFS, 0);
	USBD_CDC_SetRxBuffer(hUsbDevice_0, UserRxBufferFS);
  
	USBD_CDC_ReceivePacket(hUsbDevice_0);

	return (USBD_OK);
}


/**
 * @brief	CDC_DeInit_FS
 *		DeInitializes the CDC media low layer
 * 
 * @param  None
 * @retval USBD_OK if all operations are successful else USBD_FAIL or USBD_BUSY
 */
static int8_t sd_cdc_deinit(void)
{
	hUsbDevice_0 = NULL;
	return (USBD_OK);
}


/**
 * @brief	USB Communications Class Device Control Request
 *		Manage the CDC class requests
 *
 * @param  cmd: 	Command code	    
 * @param  pbuf: 	Buffer containing command data (request parameters)
 * @param  length: 	Number of data to be sent (in bytes)
 * @retval USBD_OK if all operations are successful else USBD_FAIL or USBD_BUSY
 */
static int8_t sd_cdc_control(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{ 
	switch (cmd) {
	case CDC_SEND_ENCAPSULATED_COMMAND:
 
		break;

	case CDC_GET_ENCAPSULATED_RESPONSE:
 
		break;

	case CDC_SET_COMM_FEATURE:
 
		break;

	case CDC_GET_COMM_FEATURE:

		break;

	case CDC_CLEAR_COMM_FEATURE:

		break;

/********************************************************************************/
/* Line Coding Structure							*/
/*------------------------------------------------------------------------------*/
/* Offset | Field       | Size | Value  | Description				*/
/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second	*/
/* 4      | bCharFormat |   1  | Number | Stop bits				*/
/*					0 - 1 Stop bit				*/
/*					1 - 1.5 Stop bits			*/
/*					2 - 2 Stop bits				*/
/* 5      | bParityType |  1   | Number | Parity				*/
/*					0 - None				*/
/*					1 - Odd					*/
/*					2 - Even				*/
/*					3 - Mark				*/
/*					4 - Space				*/
/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).		*/
/********************************************************************************/
	case CDC_SET_LINE_CODING:   
	
		break;

	case CDC_GET_LINE_CODING:     

		break;

	case CDC_SET_CONTROL_LINE_STATE:

		break;

	case CDC_SEND_BREAK:
 
		break;    
    
	default:
		break;
	}     

	return (USBD_OK);
}


/**
 * @brief	USB Communications Class Device Receive
 *		Data received over USB OUT endpoint are sent over CDC interface 
 *		through this function.
 *	   
 *		@note This function will block any OUT packet reception on USB
 *		endpoint until exiting this function. If you exit this function
 *		before transfer is complete on CDC interface (ie. using DMA
 *		controller) it will result in receiving more data while previous
 *		ones are still not sent.
 *		 
 * @param  buf: Buffer of data to be received
 * @param  len: Number of data received (in bytes)
 * @retval USBD_OK if all operations are successful else USBD_FAIL or USBD_BUSY
 */
static int8_t sd_cdc_receive(uint8_t *buf, uint32_t *len)
{
	uint8_t result;

	HAL_UART_Transmit((&uart1_dev)->uart, buf, *len, 1000);	/* Bridge to UART */
	result = USBD_CDC_ReceivePacket(hUsbDevice_0);
  
	return result;
}


/**
 * @brief	USB Communications Class Device Transmit
 *		Data send over USB IN endpoint are sent over CDC interface 
 *		through this function.	   
 *		 
 * @param  buf: Buffer of data to be send
 * @param  len: Number of data to be send (in bytes)
 * @retval USBD_OK if all operations are successful else USBD_FAIL or USBD_BUSY
 */
uint8_t sd_cdc_transmit(uint8_t *buf, uint16_t len)
{
	uint8_t result = USBD_OK;
	
	if (hUsbDevice_0 == NULL) 
		return USBD_FAIL;
  
	USBD_CDC_SetTxBuffer(hUsbDevice_0, buf, len);   
	result = USBD_CDC_TransmitPacket(hUsbDevice_0);
	
	return result;
}


uint8_t sd_cdc_get_state(void)
{
	return (hUsbDevice_0->dev_state);
}

/**
 * @}
 */ 

/**
 * @}
 */ 

/*
 * END OF FILE [sd_usbd_cdc_if.c]
 */
