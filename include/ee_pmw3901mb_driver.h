/*
MIT License

Copyright (c) 2025 EngEmil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * @file ee_pmw3901mb_driver.h
 * 
 * @brief EngEmil PMW3901MB Driver.
 * 
 */

#ifndef _EE_PMW3901MB_DRIVER_
#define _EE_PMW3901MB_DRIVER_

#include <stdint.h>
#include "hal.h"

// Registers List
#define REG_PRODUCT_ID          0x00 // RO
#define REG_REVISION_ID         0x01 // RO
#define REG_MOTION              0x02 // R/W
#define REG_DELTA_X_L           0x03 // RO
#define REG_DELTA_X_H           0x04 // RO
#define REG_DELTA_Y_L           0x05 // RO
#define REG_DELTA_Y_H           0x06 // RO
#define REG_SQUAL               0x07 // RO
#define REG_RAWDATA_SUM         0x08 // RO
#define REG_MAXIMUM_RAWDATA     0x09 // RO
#define REG_MINIMUM_RAWDATA     0x0A // RO
#define REG_SHUTTER_LOWER       0x0B // RO
#define REG_SHUTTER_UPPER       0x0C // RO
#define REG_OBSERVATION         0x15 // R/W
#define REG_MOTION_BURST        0x16 // RO
#define REG_POWER_UP_RESET      0x3A // WO
#define REG_SHUTDOWN            0x3B // WO
#define REG_RAWDATA_GRAB        0x58 // R/W
#define REG_RAWDATA_GRAB_STATUS 0x59 // RO
#define REG_INVERSE_PRODUCT_ID  0x5F // RO


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Initialize SPI.
 * 
 * @param[in] spidriver pointer to the platform specific SPI driver
 * @return uint8_t status code, 0 success, nonzero on error
 *           
 */
uint8_t ee_pmw3901mb_spi_init(SPIDriver* spidriver);

/**
 * @brief Deinitialize SPI.
 * 
 * @param[in] spidriver pointer to the platform specific SPI driver
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_spi_deinit(SPIDriver* spidriver);

/**
 * @brief Read value from register address over SPI.
 * 
 * @param[in] spidriver pointer to the platform specific SPI driver
 * @param[in] addr register address
 * @param[out] data pointer to a data buffer 
 * @param[in] len length of data buffer
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_spi_read(SPIDriver* spidriver, uint8_t addr, uint8_t* data, uint8_t len);

/**
 * @brief Write value to register address over SPI.
 * 
 * @param[in] spidriver pointer to the platform specific SPI driver
 * @param[in] addr register address
 * @param[in] data pointer to a data buffer
 * @param[in] len length of data buffer
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_spi_write(SPIDriver* spidriver, uint8_t addr, uint8_t* data, uint8_t len);

/**
 * @brief Run the PMW3901MB performance optimization sequence with the performance optimization registers, as defined in the datasheet.
 * 
 * @param[in] spidriver pointer to the platform specific SPI driver
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_perf_opt(SPIDriver* spidriver);


#ifdef __cplusplus
}
#endif


#endif /* _EE_PMW3901MB_DRIVER_ */