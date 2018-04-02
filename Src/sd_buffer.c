/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_buffer.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	Circular buffers 
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
#include <string.h>

#include "stm32f0xx_hal.h"

#include "sd_buffer.h"

/**
 * @defgroup	SD_Buffer Circular Buffer
 * @brief	Circular buffers for storing received data from peripherals
 * @{
 */

/**
 * @defgroup SD_Buffer_Functions Buffer Functions
 * @{
 */

/**
 * @brief	Initialize Character Buffer
 *
 * @param  buffer:   The buffer to initialize
 * @param  size:     The length of the buffer
 * @param  pbuff:    Pointer to location of character array to hold buffer data
 * @retval SD_BUFF_SUCCESS on success, error status otherwise
 */
enum sd_buff_err sd_char_buff_init(struct sd_cbuf *buff,
					uint32_t size,
					uint8_t *pbuff)
{
	/* Initialize the buffer to all zeros */
	memset(buff, 0, sizeof(struct sd_cbuf));
  
	/* Set the default values of the buffer */
	buff->size = size;
	buff->buff = pbuff;
    
	/* Buffer has been initialized */
	buff->state = SD_BUFF_READY;
   
	return SD_BUFF_SUCCESS;
}



/**
 * @brief	Initialize Double Buffer
 *
 * @param buff:	The buffer to initialize
 * @param size:	Length of the buffer
 * @param pbuff:	Pointer to buffer array
 * @retval SD_BUFF_SUCCESS on success, error status otherwise
 */
enum sd_buff_err sd_doub_buff_init(struct sd_doub_buff *buff,
				   uint32_t size,
				   uint8_t **pbuff)
{
	memset(buff, 0, sizeof(struct sd_doub_buff));
	
	buff->size = size;
	buff->buff = pbuff;

	return SD_BUFF_SUCCESS;
}


/**
 * @brief	Put Character in Buffer
 *		Write a character to the buffer
 *
 * @param  buffer:   The buffer to write to
 * @param  data:     The character to write
 * @retval SD_BUFF_SUCCESS on success, error status otherwise
 */
enum sd_buff_err sd_char_buff_putc(struct sd_cbuf *buffer,
					uint8_t data)
{
	if (buffer == NULL)
		return SD_BUFF_NULL_ERROR;
	
	/* Check if the buffer is ready to be used */
	if (buffer->state != SD_BUFF_READY)
		return SD_BUFF_LOCKED;
  
	/* Set the state to busy */
	buffer->state = SD_BUFF_BUSY;
  
	buffer->buff[buffer->in++] = data;
  
	/* Check for end of buffer and wrap around */
	if (++buffer->in >= buffer->size) 
		buffer->in = 0;
  
	/* Increase the length of the buffer */
	buffer->len++;
  
	/* Release the buffer */
	buffer->state = SD_BUFF_READY;
  
	return SD_BUFF_SUCCESS;
}


/**
 * @brief	Put Character in Buffer with Timeout
 *		Write a character to the buffer with a timeout
 *
 * @param  buffer:   The buffer to write to
 * @param  data:     The character to write
 * @param  timeout:  The timeout used to write the character
 * @retval SD_BUFF_SUCCESS on success, error status otherwise
 */
enum sd_buff_err sd_char_buff_putc_to(struct sd_cbuf *buffer,
					uint8_t data,
					uint32_t timeout)
{
	uint32_t time = HAL_GetTick();
	
	if (buffer == NULL)
		return SD_BUFF_NULL_ERROR;
	
	/* Check if the buffer is ready to be used */
	while (buffer->state != SD_BUFF_READY) {
		/* Check for timeout */
		if ((timeout == 0) || (HAL_GetTick() - time) > timeout)
			return SD_BUFF_TIMEOUT;
	}
  
	/* Set the state to busy */
	buffer->state = SD_BUFF_BUSY;
  
	buffer->buff[buffer->in++] = data;
  
	/* Check for end of buffer and wrap around */
	if (++buffer->in >= buffer->size) 
		buffer->in = 0;
  
	/* Increase the length of the buffer */
	buffer->len++;
  
	/* Release the buffer */
	buffer->state = SD_BUFF_READY;
  
	return SD_BUFF_SUCCESS;
}


/**
 * @brief	Write Data to Buffer
 *		Write a specified length of characters from a data pointer to
 *		the buffer
 *
 * @param  buffer:   The buffer to write to
 * @param  data:     The data pointer to data to write to buffer
 * @param  len:      The length of the buffer to write
 * @retval SD_BUFF_SUCCESS on success, error status otherwise    
 */
enum sd_buff_err sd_char_buff_write(struct sd_cbuf *buffer,
					uint8_t * data,
					uint32_t len)
{
	if (buffer == NULL)
		return SD_BUFF_NULL_ERROR;
	if (len == 0)
		return SD_BUFF_ERROR;
  
	/* Check if the buffer was newly initialized */
	if (buffer->state == SD_BUFF_INITIALIZED ) 
		buffer->state = SD_BUFF_READY;
  
	if (buffer->state != SD_BUFF_READY)
		return SD_BUFF_LOCKED;

	/* Set the state to busy */
	buffer->state = SD_BUFF_BUSY;
  
	/* If the buffer is at the end, wrap around */
	if (buffer->in >= buffer->size)
		buffer->in = 0; 
  
	while (len--) {
		/* Add the data to the buffer */
		buffer->buff[buffer->in] = *data++;
    
		/* Check for end of buffer and wrap around */
		if (++buffer->in >= buffer->size ) buffer->in = 0;
    
		/* Increase count (length) of buffer */
		buffer->len++;
	}
  
	/* Release the buffer */
	buffer->state = SD_BUFF_READY;
  
	/* Execute user-defined callback */
	sd_char_write_end_cb(buffer);
  
	return SD_BUFF_SUCCESS;
}


/**
 * @brief	Read Length of Characters from Buffer
 *		Copy a specified length of the buffer to a pointer
 *
 * @param  buff:   The buffer to read from
 * @param  data:   The data pointer to write the buffer to
 * @param  len:    The length of the buffer to read
 * @retval SD_BUFF_SUCCESS on success, error status otherwise
 */
enum sd_buff_err sd_char_buff_read(struct sd_cbuf *buff,
					uint8_t *data,
					uint32_t *len)
{			   
	uint32_t tmp_len;
				       
	if (buff == NULL)
		return SD_BUFF_NULL_ERROR;
	if (len == NULL)
		return SD_BUFF_ERROR;  
	
	/* Check if the buffer was newly initialized */
	if (buff->state == SD_BUFF_INITIALIZED) 
		buff->state = SD_BUFF_READY;
  
	/* Wait until the buffer is available */
	if (buff->state != SD_BUFF_READY)
		return SD_BUFF_LOCKED;
  
	/* Set the state to busy */
	buff->state = SD_BUFF_BUSY;
  
	/* If the buffer is at the end, wrap around */
	if (buff->out >= buff->len)
		buff->out = 0;
  
	while (buff->len) {
		/**
		 * @warning This does not destroy the contents of the buffer.
		 * The circular nature of the buffer will allow any characters
		 * to be written over, including characters that have not been
		 * read yet (buffer overflow condition).
		 */
 *data++ = buff->buff[buff->out++];
    
		/* Increase the length of the buffer read */
		tmp_len++;
    
		/* Check for end of buffer and wrap around */
		if (buff->out >= buff->size )
			buff->out = 0;
    
		if (buff->len-- <= 0 )
			buff->len = 0;
	}
  
	/* Set the length of the buffer that was read */
	*len = tmp_len;
  
	/* Release the buffer */
	buff->state = SD_BUFF_READY;
  
	/* Execute user-defined callback */
	sd_char_read_end_cb(buff);
  
	return SD_BUFF_SUCCESS;
}


/**
 * @brief	Get Character from Buffer
 *		Get a single character from the buffer
 *
 * @param  buff:     The buffer to read from
 * @param  data:     Pointer to character to write to
 * @retval SD_BUFF_SUCCESS on success, error status otherwise
 */
enum sd_buff_err sd_char_buff_getc(struct sd_cbuf * buff,
					uint8_t *data)
{
				       
	if (buff == NULL)
		return SD_BUFF_NULL_ERROR;
  
	/* Check if the buffer was newly initialized */
	if (buff->state == SD_BUFF_INITIALIZED)
		buff->state = SD_BUFF_READY;
  
	/* Wait until the buffer is available */
	if (buff->state != SD_BUFF_READY) 
		return SD_BUFF_LOCKED;
  
	/* Set the state to busy */
	buff->state = SD_BUFF_BUSY;
  
	/* If the buffer is at the end, wrap around */
	if (buff->out >= buff->size) 
		buff->out = 0;
  
	if (buff->len) {
		/**
		 * @warning This does not destroy the contents of the buffer.
		 * The circular nature of the buffer will allow any characters
		 * to be written over, including characters that have not been
		 * read yet (buffer overflow condition).
		 */
 *data = buff->buff[buff->out++];
    
		/* Check for end of buffer and wrap around */
		if (buff->out >= buff->size) 
			buff->out = 0;
		
		/* Decrement length */
		if (buff->len-- <= 0)
			buff->len = 0;
	} else {
		return SD_BUFF_EMPTY;
	}
  
	/* Release the buffer */
	buff->state = SD_BUFF_READY;
  
	/* Execute user-defined callback */
	sd_char_read_end_cb(buff);
  
	return SD_BUFF_SUCCESS;
}



/**
 * @brief	Get Character from Buffer with Timeout
 *		Get a single character from the buffer with a timeout if the
 *		buffer is locked.
 *
 * @param  buff:     The buffer to read from
 * @param  data:     Pointer to character to write to
 * @param  timeout:  Timeout value to use for read
 * @retval SD_BUFF_SUCCESS on success, error status otherwise
 */
enum sd_buff_err sd_char_buff_getc_to(struct sd_cbuf * buff,
					uint8_t *data,
					uint32_t timeout)
{
	uint32_t time = HAL_GetTick();
	
	if (buff == NULL)
		return SD_BUFF_NULL_ERROR;
  
	/* Check if the buffer is ready to be used */
	while (buff->state != SD_BUFF_READY) {
		/* Check for timeout */
		if ((timeout == 0) || (HAL_GetTick() - time) > timeout)
			return SD_BUFF_TIMEOUT;
	}
  
	/* Set the state to busy */
	buff->state = SD_BUFF_BUSY;
  
	/* If the buffer is at the end, wrap around */
	if (buff->out >= buff->size) 
		buff->out = 0;
  
	if (buff->len) {
		/**
		 * @warning This does not destroy the contents of the buffer.
		 * The circular nature of the buffer will allow any characters
		 * to be written over, including characters that have not been
		 * read yet (buffer overflow condition).
		 */
 *data = buff->buff[buff->out++];
    
		/* Check for end of buffer and wrap around */
		if (buff->out >= buff->size) 
			buff->out = 0;
		
		/* Decrement length */
		if (buff->len-- <= 0)
			buff->len = 0;
	} else {
		return SD_BUFF_EMPTY;
	}
  
	/* Release the buffer */
	buff->state = SD_BUFF_READY;
  
	/* Execute user-defined callback */
	sd_char_read_end_cb(buff);
  
	return SD_BUFF_SUCCESS;
}


/**
 * @brief	Reset buffer
 *		Resets the in and out indexes of the buffer (without removing
 *		contents of the buffer) to the start of the buffer.
 *
 * @param  buff:       The buffer to reset
 * @retval SD_BUFF_SUCCESS on success, error state otherwise
 */
enum sd_buff_err sd_buff_reset(struct sd_cbuf *buff)
{
	
	return SD_BUFF_SUCCESS;
}


/**
 * @brief	Write Command End Callback
 *		User defined function executed when a write operation is
 *		completed
 *
 * 		@note This function should not be modified, when the callback is
 *		needed, the sd_char_write_end_cb can be implemented in the user
 *		file.
 *
 * @param  buff:    The buffer that was written
 * @retval None
 */
__weak void sd_char_write_end_cb(struct sd_cbuf *buff)
{
	
}


/**
 * @brief	Read Command End Callback
 *		User defined function executed when a read operation is
 *		completed
 *
 *		@note This function should not be modified, when the callback is
 *		needed, the sd_char_read_end_cb can be implemented in the user
 *		file.
 *
 * @param  buff:     The buffer that was read
 * @retval none
 */
__weak void sd_char_read_end_cb(struct sd_cbuf *buff)
{

}
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_buffer.c]
 */

