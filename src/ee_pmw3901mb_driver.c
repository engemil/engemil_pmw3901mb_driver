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

#include "ee_pmw3901mb_driver.h"

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

// Register Default Values
#define DEF_REG_PRODUCT_ID          0x49
#define DEF_REG_REVERSE_PRODUCT_ID  0xB6

// Performance Optimization Registers (PixArt proprietary information, hence address used as name)
#define PER_REG_0x40            0x40
#define PER_REG_0x41            0x41
#define PER_REG_0x43            0x43
#define PER_REG_0x44            0x44
#define PER_REG_0x45            0x45
#define PER_REG_0x4E            0x4E
#define PER_REG_0x5B            0x5B
#define PER_REG_0x5F            0x5F
#define PER_REG_0x61            0x61
#define PER_REG_0x7B            0x7B
#define PER_REG_0x7F            0x7F


uint8_t ee_pmw3901mb_get_product_id(uin8t_t* product_id){
    uint8_t status_code = 0U;
    status_code = ee_pmw3901mb_spi_read(REG_PRODUCT_ID, product_id, 1U);
    return status_code; // Success
}

// TODO: continue adding functions

