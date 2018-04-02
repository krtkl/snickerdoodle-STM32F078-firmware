/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_i2c.h
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	I2C Control 
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

#ifndef __SD_I2C_H
#define __SD_I2C_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SD_I2C
 * @{
 */

/**
 * @defgroup SD_I2C_Defines I2C Definitions
 * @{
 */
#define SD_I2C_SWITCH_ADDR		0xE0	/**< I2C Switch address */
  
#define SD_I2C_CHANNEL_NONE		0x00	/**< No channels enabled */
#define SD_I2C_CHANNEL_0		0x01	/**< Switch channel 0 */
#define SD_I2C_CHANNEL_1		0x02	/**< Switch channel 1 */
#define SD_I2C_CHANNEL_2		0x04	/**< Switch channel 2 */
#define SD_I2C_CHANNEL_3		0x08	/**< Switch channel 3 */
#define SD_I2C_CHANNEL_4		0x10	/**< Switch channel 4 */
#define SD_I2C_CHANNEL_5		0x20	/**< Switch channel 5 */
#define SD_I2C_CHANNEL_6		0x40	/**< Switch channel 6 */
#define SD_I2C_CHANNEL_7		0x80	/**< Switch channel 7 */

#define SD_I2C_JA1	SD_I2C_CHANNEL_0	/**< JA1 channel */
#define SD_I2C_JA2	SD_I2C_CHANNEL_1	/**< JA2 channel */
#define SD_I2C_JB1	SD_I2C_CHANNEL_2	/**< JB1 channel */
#define SD_I2C_JB2	SD_I2C_CHANNEL_3	/**< JB2 channel */
#define SD_I2C_JC1	SD_I2C_CHANNEL_4	/**< JC1 channel */
#define SD_I2C_J3	SD_I2C_CHANNEL_5	/**< J3 channel */
#define SD_I2C_J2	SD_I2C_CHANNEL_7	/**< J2 channel */
/**
 * SD_I2C_Defines
 * @}
 */

/**
 * @addtogroup SD_I2C_Exported_Variables
 * @{
 */
extern struct sd_i2c_dev i2c1_dev;
extern struct sd_i2c_dev i2c2_dev;
/**
 * @}
 */


/**
 * @defgroup SD_I2C_Types I2C Types
 * @{
 */
  
/**
 * @enum  sd_i2c_err
 * @brief I2C error enumeration
 */
enum sd_i2c_err {
	SD_I2C_SUCCESS = 0x4A0D,		/**< Successful execution */
	SD_I2C_NULL_ERROR,			/**< NULL pointer error */
	SD_I2C_LOCKED,				/**< The buffer is being used */
	SD_I2C_TIMEOUT,				/**< The operation timed out */
	SD_I2C_ERROR				/**< General execution error */
};

/**
 * @enum   sd_i2c_state
 * @brief  I2C state enumeration
 */
enum sd_i2c_state {
	SD_I2C_INITIALIZED = 0x0E60,		/**< I2C initialized */
	SD_I2C_READY,				/**< I2C ready */
	SD_I2C_BUSY,				/**< I2C in use */
};

  
/**
 * @struct sd_i2c_dev
 * @brief  I2C device structure
 */
struct sd_i2c_dev {
	I2C_HandleTypeDef       *i2c;		/**< I2C Handle */
	struct sd_cbuf     *rx_buff;	/**< Buffer for I2C receive */
	enum sd_i2c_state       state;		/**< State of the I2C */
	uint8_t			error;		/**< Error flags from the peripheral */
};
/**
 * @}
 */


/** 
 * @addtogroup SD_I2C_Functions
 * @{
 */
void sd_i2c1_init			(void);
void sd_i2c2_init			(void);
enum sd_i2c_err sd_i2C_set_chan		(struct sd_i2c_dev *dev, 
						uint8_t chan);
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
#error "Multiple inclusion of sd_i2c.h"
#endif /* __SD_I2C_H */

/*
 * END OF FILE [sd_i2c.h]
 */
