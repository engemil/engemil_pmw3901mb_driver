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

#include "ee_pmw3901mb_platform.h"


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Get Product ID
 * 
 * @param[out] product_id pointer to the return value
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_get_product_id(uint8_t* product_id);

/**
 * @brief Get Revision ID
 * 
 * @param[out] revision_id pointer to the return value 
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_get_revision_id(uint8_t* revision_id);

// (?)
//uint8_t ee_pmw3901mb_get_motion(void);

// (?)
//uint8_t ee_pmw3901mb_set_motion(void);

/**
 * @brief Get Delta X
 * 
 * @param[out] delta_x pointer to the return value 
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_get_delta_x(int16_t* delta_x);

/**
 * @brief Get Delta Y
 * 
 * @param[out] delta_y pointer to the return value 
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_get_delta_y(int16_t* delta_y);

/**
 * @brief Get Delta X and Delta Y
 * 
 * @param[out] delta_x pointer to the return value 
 * @param[out] delta_y pointer to the return value 
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_get_delta_x_y(int16_t* delta_x, int16_t* delta_y);

// (?)
//uint8_t ee_pmw3901mb_get_squal(void);

// (?)
//uint8_t ee_pmw3901mb_get_rawdata_sum(void);

// (?)
//uint8_t ee_pmw3901mb_get_maximum_rawdata(void);

// (?)
//uint8_t ee_pmw3901mb_get_minimum_rawdata(void);

// (?)
//uint8_t ee_pmw3901mb_get_shutter_lower(void);

// (?)
//uint8_t ee_pmw3901mb_get_shutter_upper(void);

// (?)
//uint8_t ee_pmw3901mb_get_observation(void);

// (?)
//uint8_t ee_pmw3901mb_set_observation(void);

// (?)
//uint8_t ee_pmw3901mb_get_motion_burst(void);

uint8_t ee_pmw3901mb_power_up_reset(void);

uint8_t ee_pmw3901mb_shutdown(void);

// (?)
//uint8_t ee_pmw3901mb_get_rawdata_grab(void);

// (?)
//uint8_t ee_pmw3901mb_set_rawdata_grab(void);

// (?)
//uint8_t ee_pmw3901mb_get_rawdata_grab_status(void);

/**
 * @brief Get Inverse Product ID
 * 
 * @param[out] inv_product_id pointer to the return value 
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_inverse_product_id(uint8_t* inv_product_id);

/**
 * @brief Run the PMW3901MB performance optimization sequence.
 * @pre The SPI Driver must be initialized and the driver started.
 * 
 * @return uint8_t status code, 0 success, nonzero on error
 */
uint8_t ee_pmw3901mb_perf_opt(void);


#ifdef __cplusplus
}
#endif


#endif /* _EE_PMW3901MB_DRIVER_ */