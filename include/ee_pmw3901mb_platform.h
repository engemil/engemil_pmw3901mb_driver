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
 * @file ee_pmw3901mb_platform.h
 * 
 * @brief EngEmil PMW3901MB Platform.
 * 
 */

#ifndef _EE_PMW3901MB_PLATFORM_
#define _EE_PMW3901MB_PLATFORM_

#include <stdint.h>
#include <string.h>

// Include platform dependent files here
#include "hal.h"


#ifdef __cplusplus
extern "C"
{
#endif


// Include platform dependent function headers here

/**
 * @brief Initialize SPI.
 * 
 * @param[in] spid_p pointer to the platform specific SPI driver
 * @param[in] spic_p pointer to the platform specific SPI Config
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_spi_init(SPIDriver* spidriver, SPIConfig* spic_p);

/**
 * @brief Deinitialize SPI.
 * 
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_spi_deinit(void);

/**
 * @brief Read value from register address over SPI.
 * @pre The SPI Driver must be initialized and the driver started.
 * 
 * @param[in] addr starting register address
 * @param[out] data pointer to data buffer
 * @param[in] n number of consecutive registers to read
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_spi_read(uint8_t addr, uint8_t* data, size_t n);

/**
 * @brief Write value to register address over SPI.
 * @pre The SPI Driver must be initialized and the driver started.
 * 
 * @param[in] addr starting register address
 * @param[out] data pointer to data buffer
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_spi_write(uint8_t addr, uint8_t* data);

/**
 * @brief Wait in milliseconds with platform specific function
 * 
 * @param[in] wait_ms milliseconds
 * @return uint8_t status code, 0 success, nonzero on error 
 */
uint8_t ee_pmw3901mb_wait_ms(uint32_t wait_ms);


#ifdef __cplusplus
}
#endif


#endif /* _EE_PMW3901MB_PLATFORM_ */