/**
 *******************************************************************************
 *******************************************************************************
 *
 * @file	sd_led_pattern.c
 * @author	R. Bush
 * @email	bush@krtkl.com
 * @version	v1.0
 * @date	2016 March 28
 * @brief	LED Patterns and Functions 
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
#include "stm32f0xx.h"

#include "std_defs.h"

#include "sd_led.h"
#include "sd_led_pattern.h"


/**
 * @defgroup	SD_LED_Pattern LED Patterns
 * @brief	Patterns for LEDs
 * @{
 */
  
/**
 * @defgroup SD_LED_Pattern_Definitions LED Pattern Definitions
 * @{
 */

/**
 * @brief LED breathe pattern
 */
uint32_t led_breathe_pattern[] = {
   0,    0,    0,    0,    0,    0,    1,    1,    2,    2,    3,    4,    4,    5,    6,    7,    8,    9,   10,   12,   13,   14,   16,   17,   19, 
  20,   22,   24,   26,   28,   30,   32,   34,   36,   38,   40,   43,   45,   48,   50,   53,   56,   58,   61,   64,   67,   70,   73,   76,   79, 
  82,   86,   89,   92,   96,  100,  103,  107,  110,  114,  118,  122,  126,  130,  134,  138,  142,  147,  151,  155,  160,  164,  169,  173,  178, 
 183,  187,  192,  197,  202,  207,  212,  217,  222,  227,  233,  238,  243,  249,  254,  259,  265,  271,  276,  282,  288,  293,  299,  305,  311, 
 317,  323,  329,  335,  341,  347,  353,  360,  366,  372,  379,  385,  392,  398,  405,  411,  418,  424,  431,  438,  445,  451,  458,  465,  472, 
 479,  486,  493,  500,  507,  514,  521,  529,  536,  543,  550,  557,  565,  572,  579,  587,  594,  602,  609,  617,  624,  632,  639,  647,  655, 
 662,  670,  677,  685,  693,  701,  708,  716,  724,  732,  740,  747,  755,  763,  771,  779,  787,  795,  803,  810,  818,  826,  834,  842,  850, 
 858,  866,  874,  882,  890,  898,  906,  914,  922,  930,  938,  946,  954,  963,  971,  979,  987,  995, 1003, 1011, 1019, 1027, 1035, 1043, 1051, 
1059, 1067, 1075, 1083, 1091, 1099, 1107, 1114, 1122, 1130, 1138, 1146, 1154, 1162, 1170, 1177, 1185, 1193, 1201, 1209, 1216, 1224, 1232, 1240, 1247, 
1255, 1262, 1270, 1278, 1285, 1293, 1300, 1308, 1315, 1323, 1330, 1338, 1345, 1352, 1360, 1367, 1374, 1381, 1388, 1396, 1403, 1410, 1417, 1424, 1431, 
1438, 1445, 1452, 1459, 1466, 1472, 1479, 1486, 1493, 1499, 1506, 1512, 1519, 1525, 1532, 1538, 1545, 1551, 1557, 1564, 1570, 1576, 1582, 1588, 1594, 
1600, 1606, 1612, 1618, 1624, 1629, 1635, 1641, 1646, 1652, 1658, 1663, 1668, 1674, 1679, 1684, 1690, 1695, 1700, 1705, 1710, 1715, 1720, 1725, 1730, 
1734, 1739, 1744, 1748, 1753, 1757, 1762, 1766, 1770, 1775, 1779, 1783, 1787, 1791, 1795, 1799, 1803, 1807, 1810, 1814, 1817, 1821, 1825, 1828, 1831, 
1835, 1838, 1841, 1844, 1847, 1850, 1853, 1856, 1859, 1861, 1864, 1867, 1869, 1872, 1874, 1877, 1879, 1881, 1883, 1885, 1887, 1889, 1891, 1893, 1895, 
1897, 1898, 1900, 1901, 1903, 1904, 1905, 1907, 1908, 1909, 1910, 1911, 1912, 1913, 1913, 1914, 1915, 1915, 1916, 1916, 1917, 1917, 1917, 1917, 1917, 
1918, 1917, 1917, 1917, 1917, 1917, 1916, 1916, 1915, 1915, 1914, 1913, 1913, 1912, 1911, 1910, 1909, 1908, 1907, 1905, 1904, 1903, 1901, 1900, 1898, 
1897, 1895, 1893, 1891, 1889, 1887, 1885, 1883, 1881, 1879, 1877, 1874, 1872, 1869, 1867, 1864, 1861, 1859, 1856, 1853, 1850, 1847, 1844, 1841, 1838, 
1835, 1831, 1828, 1825, 1821, 1817, 1814, 1810, 1807, 1803, 1799, 1795, 1791, 1787, 1783, 1779, 1775, 1770, 1766, 1762, 1757, 1753, 1748, 1744, 1739, 
1734, 1730, 1725, 1720, 1715, 1710, 1705, 1700, 1695, 1690, 1684, 1679, 1674, 1668, 1663, 1658, 1652, 1646, 1641, 1635, 1629, 1624, 1618, 1612, 1606, 
1600, 1594, 1588, 1582, 1576, 1570, 1564, 1557, 1551, 1545, 1538, 1532, 1525, 1519, 1512, 1506, 1499, 1493, 1486, 1479, 1472, 1466, 1459, 1452, 1445, 
1438, 1431, 1424, 1417, 1410, 1403, 1396, 1388, 1381, 1374, 1367, 1360, 1352, 1345, 1338, 1330, 1323, 1315, 1308, 1300, 1293, 1285, 1278, 1270, 1262, 
1255, 1247, 1240, 1232, 1224, 1216, 1209, 1201, 1193, 1185, 1177, 1170, 1162, 1154, 1146, 1138, 1130, 1122, 1114, 1107, 1099, 1091, 1083, 1075, 1067, 
1059, 1051, 1043, 1035, 1027, 1019, 1011, 1003,  995,  987,  979,  971,  963,  954,  946,  938,  930,  922,  914,  906,  898,  890,  882,  874,  866, 
 858,  850,  842,  834,  826,  818,  810,  803,  795,  787,  779,  771,  763,  755,  747,  740,  732,  724,  716,  708,  701,  693,  685,  677,  670, 
 662,  655,  647,  639,  632,  624,  617,  609,  602,  594,  587,  579,  572,  565,  557,  550,  543,  536,  529,  521,  514,  507,  500,  493,  486, 
 479,  472,  465,  458,  451,  445,  438,  431,  424,  418,  411,  405,  398,  392,  385,  379,  372,  366,  360,  353,  347,  341,  335,  329,  323, 
 317,  311,  305,  299,  293,  288,  282,  276,  271,  265,  259,  254,  249,  243,  238,  233,  227,  222,  217,  212,  207,  202,  197,  192,  187, 
 183,  178,  173,  169,  164,  160,  155,  151,  147,  142,  138,  134,  130,  126,  122,  118,  114,  110,  107,  103,  100,   96,   92,   89,   86, 
  82,   79,   76,   73,   70,   67,   64,   61,   58,   56,   53,   50,   48,   45,   43,   40,   38,   36,   34,   32,   30,   28,   26,   24,   22, 
  20,   19,   17,   16,   14,   13,   12,   10,    9,    8,    7,    6,    5,    4,    4,    3,    2,    2,    1,    1,    0,    0,    0,    0,    0, 
   0,    0,    1,    2,    4,    7,   10,   14,   19,   24,   30,   36,   43,   50,   58,   67,   76,   86,   96,  107,  118,  130,  142,  155,  169, 
 183,  197,  212,  227,  243,  259,  276,  293,  311,  329,  347,  366,  385,  405,  424,  445,  465,  486,  507,  529,  550,  572,  594,  617,  639, 
 662,  685,  708,  732,  755,  779,  803,  826,  850,  874,  898,  922,  946,  971,  995, 1019, 1043, 1067, 1091, 1114, 1138, 1162, 1185, 1209, 1232, 
1255, 1278, 1300, 1323, 1345, 1367, 1388, 1410, 1431, 1452, 1472, 1493, 1512, 1532, 1551, 1570, 1588, 1606, 1624, 1641, 1658, 1674, 1690, 1705, 1720, 
1734, 1748, 1762, 1775, 1787, 1799, 1810, 1821, 1831, 1841, 1850, 1859, 1867, 1874, 1881, 1887, 1893, 1898, 1903, 1907, 1910, 1913, 1915, 1916, 1917, 
1918, 1917, 1916, 1915, 1913, 1910, 1907, 1903, 1898, 1893, 1887, 1881, 1874, 1867, 1859, 1850, 1841, 1831, 1821, 1810, 1799, 1787, 1775, 1762, 1748, 
1734, 1720, 1705, 1690, 1674, 1658, 1641, 1624, 1606, 1588, 1570, 1551, 1532, 1512, 1493, 1472, 1452, 1431, 1410, 1388, 1367, 1345, 1323, 1300, 1278, 
1255, 1232, 1209, 1185, 1162, 1138, 1114, 1091, 1067, 1043, 1019,  995,  971,  946,  922,  898,  874,  850,  826,  803,  779,  755,  732,  708,  685, 
 662,  639,  617,  594,  572,  550,  529,  507,  486,  465,  445,  424,  405,  385,  366,  347,  329,  311,  293,  276,  259,  243,  227,  212,  197, 
 183,  169,  155,  142,  130,  118,  107,   96,   86,   76,   67,   58,   50,   43,   36,   30,   24,   19,   14,   10,    7,    4,    2,    1,    0, 
};


/**
 * @brief LED rapid pattern
 */
static uint32_t led_rapid_pattern[] = {
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 
1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 
1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 
1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 
1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919
};


/**
 * @brief LED short blink pattern
 */
static uint32_t led_short_blink_pattern[] = {
1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 1919, 0
};


/**
 * @brief LED solid pattern
 */
static uint32_t led_solid_pattern[] = {
1919,
};
/**
 * SD_LED_Pattern_Definitions
 * @}
 */


/**
 * @defgroup	SD_LED_Pattern_Functions LED Pattern Functions
 * @{
 */


/**
 * @brief	LED Breathe
 *		Set the LED to breathe
 *
 * @param led:		The LED handle
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_breathe(struct sd_led *led)
{
	/* Set the pattern */
	return sd_led_set_pattern(led,
				led_breathe_pattern,
				COUNT_OF(led_breathe_pattern),
				1);
}


/**
 * @brief	LED blink 
 *		Set a short non-repeating blink on a single LED
 *
 * @param  led:	The LED pattern handle
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_blink(struct sd_led *led)
{
	/* Set the pattern */
	return sd_led_set_pattern(led, 
				led_short_blink_pattern,
				COUNT_OF(led_short_blink_pattern),
				0);
}


/**
 * @brief	LED rapid blinking
 *		Set an LED to a continuous rapid blinking
 *
 * @param  led:	The LED pattern handle
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_rapid(struct sd_led *led)
{
	/* Set the pattern */
	return sd_led_set_pattern(led,
				led_rapid_pattern,
				COUNT_OF(led_rapid_pattern),
				1);
}		 

/**
 * @brief	LED solid on
 *		Set an LED to an continuous on state
 *
 * @param  led:	The LED pattern handle
 * @retval SD_LED_SUCCESS on success, error status otherwise
 */
enum sd_led_err sd_led_solid(struct sd_led *led)
{
	/* Set the pattern */
	return sd_led_set_pattern(led,
				led_solid_pattern, 
				1, 
				1);
}
/**
 * @}
 */

/**
 * @}
 */

/*
 * END OF FILE [sd_led_pattern.c]
 */
