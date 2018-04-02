/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_buffer.h
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

#ifndef __SD_BUFFER_H
#define __SD_BUFFER_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SD_Buffer
 * @{
 */

/**
 * @defgroup SD_Buffer_Types Buffer Types
 * @{
 */
  
/**
 * @enum  sd_buff_err
 * @brief Buffer error enumeration
 */
enum sd_buff_err {
	SD_BUFF_SUCCESS = 0x4A0D,		/**< Successful execution */
	SD_BUFF_NULL_ERROR,			/**< NULL pointer error */
	SD_BUFF_LOCKED,				/**< The buffer is being used */
	SD_BUFF_EMPTY,				/**< The buffer is empty */
	SD_BUFF_TIMEOUT,			/**< The operation timed out */
	SD_BUFF_ERROR				/**< General execution error */
};

/**
 * @enum   sd_buff_state
 * @brief  Buffer state enumeration
 */
enum sd_buff_state {
	SD_BUFF_INITIALIZED = 0x0E60,		/**< Buffer initialized */
	SD_BUFF_READY,				/**< Buffer ready */
	SD_BUFF_BUSY,				/**< Buffer in use */
	SD_BUFF_OVERFLOW			/**< Buffer has been overrun */
};

/**
 * @struct sd_doub_buff
 * @brief  Double character buffer structure
 */
struct sd_doub_buff {
	uint8_t				**buff;	/**< Buffer pointer */
	uint8_t				idx;	/**< Buffer index */
	uint32_t			in;	/**< Buffer input location */
	uint32_t			len;	/**< Length of the buffer */
	uint32_t			size;	/**< Size of the buffer */
	volatile enum sd_buff_state	state;	/**< State of the buffer */
};

/**
 * @struct sd_cbuf
 * @brief  Character buffer structure
 */
struct sd_cbuf {
	uint8_t				*buff;  /**< Buffer pointer */
	uint32_t			in;     /**< Buffer input location */
	uint32_t			out;    /**< Buffer output location */
	uint32_t			len;    /**< Length of the buffer */
	uint32_t			size;   /**< Size of the buffer */
	volatile enum sd_buff_state     state;  /**< State of the buffer */
};
/**
 * SD_Buffer_Types
 * @}
 */


/**
 * @addtogroup SD_Buffer_Functions
 * @{
 */
enum sd_buff_err sd_char_buff_init	(struct sd_cbuf *buff,
						uint32_t size,
						uint8_t *pbuff);
enum sd_buff_err sd_doub_buff_init	(struct sd_doub_buff *buff,
						uint32_t size,
						uint8_t *pbuff[]);
enum sd_buff_err sd_char_buff_putc	(struct sd_cbuf *buff,
						uint8_t data);
enum sd_buff_err sd_char_buff_putc_to	(struct sd_cbuf *buff,
						uint8_t data,
						uint32_t timeout);
enum sd_buff_err sd_char_buff_write	(struct sd_cbuf *buff,
						uint8_t *data,
						uint32_t len);
enum sd_buff_err sd_char_buff_read	(struct sd_cbuf *buff,
						uint8_t *data, 
						uint32_t *len);
enum sd_buff_err sd_char_buff_getc	(struct sd_cbuf *buff,
						uint8_t *data);
enum sd_buff_err sd_char_buff_getc_to	(struct sd_cbuf *buff,
						uint8_t *data,
						uint32_t timeout);
void sd_char_write_end_cb		(struct sd_cbuf *buff);
void sd_char_read_end_cb		(struct sd_cbuf *buff);
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
#error "Multiple inclusion of sd_buffer.h"
#endif /* __SD_BUFFER_H */

/*
 * END OF FILE [sd_buffer.h]
 */
