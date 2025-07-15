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
#define REG_PRODUCT_ID          0x00 // RO  // Product ID
#define REG_REVISION_ID         0x01 // RO  // Revision ID
#define REG_MOTION              0x02 // R/W // (?)
#define REG_DELTA_X_L           0x03 // RO  // Delta X [0:7]
#define REG_DELTA_X_H           0x04 // RO  // Delta X [8:15]
#define REG_DELTA_Y_L           0x05 // RO  // Delta Y [0:7]
#define REG_DELTA_Y_H           0x06 // RO  // Delta Y [8:15]
#define REG_SQUAL               0x07 // RO  // (?)
#define REG_RAWDATA_SUM         0x08 // RO  // Sum of raw pixel data
#define REG_MAXIMUM_RAWDATA     0x09 // RO  // Max of raw pixel data
#define REG_MINIMUM_RAWDATA     0x0A // RO  // Min of raw pixel data
#define REG_SHUTTER_LOWER       0x0B // RO  // (?)
#define REG_SHUTTER_UPPER       0x0C // RO  // (?)
#define REG_OBSERVATION         0x15 // R/W // (?)
#define REG_MOTION_BURST        0x16 // RO  // (?)
#define REG_POWER_UP_RESET      0x3A // WO  // Reset Sensor
#define REG_SHUTDOWN            0x3B // WO  // Shutdown Sensor
#define REG_RAWDATA_GRAB        0x58 // R/W // (?)
#define REG_RAWDATA_GRAB_STATUS 0x59 // RO  // (?)
#define REG_INVERSE_PRODUCT_ID  0x5F // RO  // Inverse Product ID

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


uint8_t ee_pmw3901mb_init_driver(void* spi_driver, void* spi_config){
    if(spi_driver == NULL) return 1;

    uint8_t status_code = 0;

    status_code = ee_pmw3901mb_spi_init(spi_driver, spi_config);
    if(status_code != 0) return 1;

    status_code = ee_pmw3901mb_power_up_reset();
    if(status_code != 0) return 1;
    
    status_code = ee_pmw3901mb_wait_ms(50U); // Wait 50 ms while system resets
    if(status_code != 0) return 1;

    //status_code = ee_pmw3901mb_perf_opt();
    status_code = ee_pmw3901mb_perf_opt_v2();
    if(status_code != 0) return 1;

    status_code = ee_pmw3901mb_wait_ms(5U); // Wait 5 ms while system resets
    if(status_code != 0) return 1;
    
    return status_code;
}

uint8_t ee_pmw3901mb_get_product_id(uint8_t* product_id){
    if(product_id == NULL) return 1;
    return ee_pmw3901mb_spi_read(REG_PRODUCT_ID, product_id, 1U);
}

uint8_t ee_pmw3901mb_get_revision_id(uint8_t* revision_id){
    if(revision_id == NULL) return 1;
    return ee_pmw3901mb_spi_read(REG_REVISION_ID, revision_id, 1U);
}

uint8_t ee_pmw3901mb_get_delta_x_y(int16_t* delta_x, int16_t* delta_y){
    if(delta_x == NULL || delta_y == NULL ) return 1;
    uint8_t status_code = 0;

    // Must read motion for each time reading changes to delta x and delta y
    uint8_t motion = 0;
    status_code = ee_pmw3901mb_spi_read(REG_MOTION, &motion, 1U);
    if(status_code != 0) return status_code;

    uint8_t delta_x_l = 0;
    uint8_t delta_x_h = 0;

    status_code = ee_pmw3901mb_spi_read(REG_DELTA_X_L, &delta_x_l, 1U);
    if(status_code != 0) return status_code;
    status_code = ee_pmw3901mb_spi_read(REG_DELTA_X_H, &delta_x_h, 1U);
    if(status_code != 0) return status_code;

    uint8_t delta_y_l = 0;
    uint8_t delta_y_h = 0;

    status_code = ee_pmw3901mb_spi_read(REG_DELTA_Y_L, &delta_y_l, 1U);
    if(status_code != 0) return status_code;
    status_code = ee_pmw3901mb_spi_read(REG_DELTA_Y_H, &delta_y_h, 1U);
    if(status_code != 0) return status_code;
    
    *delta_x = (int16_t) ((delta_x_h << 8) | (delta_x_l));
    *delta_y = (int16_t) ((delta_y_h << 8) | (delta_y_l));

    return status_code;
}

uint8_t ee_pmw3901mb_power_up_reset(void){
    uint8_t value = 0x5A;
    return ee_pmw3901mb_spi_write(REG_POWER_UP_RESET, &value);
}

uint8_t ee_pmw3901mb_shutdown(void){
    uint8_t value = 0x00;
    return ee_pmw3901mb_spi_write(REG_SHUTDOWN, &value);
}

uint8_t ee_pmw3901mb_get_inverse_product_id(uint8_t* inv_product_id){
    if(inv_product_id == NULL) return 1;
    return ee_pmw3901mb_spi_read(REG_INVERSE_PRODUCT_ID, inv_product_id, 1U);
}

uint8_t ee_pmw3901mb_perf_opt(void){
    uint8_t status_code = 0;
    uint8_t value = 0x00;

    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x7F, &value);
    if(status_code != 0) return status_code;

    value = 0xAD;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x61, &value);
    if(status_code != 0) return status_code;

    value = 0x03;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x7F, &value);
    if(status_code != 0) return status_code;

    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x40, &value);
    if(status_code != 0) return status_code;

    value = 0x05;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x7F, &value);
    if(status_code != 0) return status_code;

    value = 0xB3;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x41, &value);
    if(status_code != 0) return status_code;

    value = 0xF1;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x43, &value);
    if(status_code != 0) return status_code;

    value = 0x14;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x45, &value);
    if(status_code != 0) return status_code;

    value = 0x32;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x5B, &value);
    if(status_code != 0) return status_code;

    value = 0x34;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x5F, &value);
    if(status_code != 0) return status_code;

    value = 0x08;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x7B, &value);
    if(status_code != 0) return status_code;

    value = 0x06;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x7F, &value);
    if(status_code != 0) return status_code;

    value = 0x1B;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x44, &value);
    if(status_code != 0) return status_code;
    
    value = 0xBF;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x40, &value);
    if(status_code != 0) return status_code;
    
    value = 0x3F;
    status_code = ee_pmw3901mb_spi_write(PER_REG_0x4E, &value);
    if(status_code != 0) return status_code;

    return status_code; // Success
}

uint8_t ee_pmw3901mb_perf_opt_v2(void){

    /*
    * Some parts of this file include code adapted from:
    * https://github.com/bitcraze/Bitcraze_PMW3901
    * Licensed under the MIT License.
    *
    * Copyright (c) 2017 Bitcraze
    */
    
    uint8_t status_code = 0;
    uint8_t value = 0x00;

    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;

    value = 0xAD;
    status_code = ee_pmw3901mb_spi_write(0x61, &value);
    if(status_code != 0) return status_code;

    value = 0x03;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;

    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(0x40, &value);
    if(status_code != 0) return status_code;

    value = 0x05;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0xB3;
    status_code = ee_pmw3901mb_spi_write(0x41, &value);
    if(status_code != 0) return status_code;
    
    value = 0xF1;
    status_code = ee_pmw3901mb_spi_write(0x43, &value);
    if(status_code != 0) return status_code;
    
    value = 0x14;
    status_code = ee_pmw3901mb_spi_write(0x45, &value);
    if(status_code != 0) return status_code;
    
    value = 0x32;
    status_code = ee_pmw3901mb_spi_write(0x5B, &value);
    if(status_code != 0) return status_code;
    
    value = 0x34;
    status_code = ee_pmw3901mb_spi_write(0x5F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x08;
    status_code = ee_pmw3901mb_spi_write(0x7B, &value);
    if(status_code != 0) return status_code;
    
    value = 0x06;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x1B;
    status_code = ee_pmw3901mb_spi_write(0x44, &value);
    if(status_code != 0) return status_code;
    
    value = 0xBF;
    status_code = ee_pmw3901mb_spi_write(0x40, &value);
    if(status_code != 0) return status_code;
    
    value = 0x3F;
    status_code = ee_pmw3901mb_spi_write(0x4E, &value);
    if(status_code != 0) return status_code;
    
    value = 0x08;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x20;
    status_code = ee_pmw3901mb_spi_write(0x65, &value);
    if(status_code != 0) return status_code;
    
    value = 0x18;
    status_code = ee_pmw3901mb_spi_write(0x6A, &value);
    if(status_code != 0) return status_code;
    
    value = 0x09;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0xAF;
    status_code = ee_pmw3901mb_spi_write(0x4F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x40;
    status_code = ee_pmw3901mb_spi_write(0x5F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x80;
    status_code = ee_pmw3901mb_spi_write(0x48, &value);
    if(status_code != 0) return status_code;
    
    value = 0x80;
    status_code = ee_pmw3901mb_spi_write(0x49, &value);
    if(status_code != 0) return status_code;
    
    value = 0x77;
    status_code = ee_pmw3901mb_spi_write(0x57, &value);
    if(status_code != 0) return status_code;
    
    value = 0x78;
    status_code = ee_pmw3901mb_spi_write(0x60, &value);
    if(status_code != 0) return status_code;
    
    value = 0x78;
    status_code = ee_pmw3901mb_spi_write(0x61, &value);
    if(status_code != 0) return status_code;
    
    value = 0x08;
    status_code = ee_pmw3901mb_spi_write(0x62, &value);
    if(status_code != 0) return status_code;
    
    value = 0x50;
    status_code = ee_pmw3901mb_spi_write(0x63, &value);
    if(status_code != 0) return status_code;

    value = 0x0A;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x60;
    status_code = ee_pmw3901mb_spi_write(0x45, &value);
    if(status_code != 0) return status_code;

    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x11;
    status_code = ee_pmw3901mb_spi_write(0x4D, &value);
    if(status_code != 0) return status_code;
    
    value = 0x80;
    status_code = ee_pmw3901mb_spi_write(0x55, &value);
    if(status_code != 0) return status_code;
    
    value = 0x1F;
    status_code = ee_pmw3901mb_spi_write(0x74, &value);
    if(status_code != 0) return status_code;
    
    value = 0x1F;
    status_code = ee_pmw3901mb_spi_write(0x75, &value);
    if(status_code != 0) return status_code;
    
    value = 0x78;
    status_code = ee_pmw3901mb_spi_write(0x4A, &value);
    if(status_code != 0) return status_code;
    
    value = 0x78;
    status_code = ee_pmw3901mb_spi_write(0x4B, &value);
    if(status_code != 0) return status_code;
    
    value = 0x08;
    status_code = ee_pmw3901mb_spi_write(0x44, &value);
    if(status_code != 0) return status_code;
    
    value = 0x50;
    status_code = ee_pmw3901mb_spi_write(0x45, &value);
    if(status_code != 0) return status_code;
    
    value = 0xFF;
    status_code = ee_pmw3901mb_spi_write(0x64, &value);
    if(status_code != 0) return status_code;
    
    value = 0x1F;
    status_code = ee_pmw3901mb_spi_write(0x65, &value);
    if(status_code != 0) return status_code;
    
    value = 0x14;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x60;
    status_code = ee_pmw3901mb_spi_write(0x65, &value);
    if(status_code != 0) return status_code;
    
    value = 0x08;
    status_code = ee_pmw3901mb_spi_write(0x66, &value);
    if(status_code != 0) return status_code;
    
    value = 0x78;
    status_code = ee_pmw3901mb_spi_write(0x63, &value);
    if(status_code != 0) return status_code;
    
    value = 0x15;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x58;
    status_code = ee_pmw3901mb_spi_write(0x48, &value);
    if(status_code != 0) return status_code;
    
    value = 0x07;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x0D;
    status_code = ee_pmw3901mb_spi_write(0x41, &value);
    if(status_code != 0) return status_code;
    
    value = 0x14;
    status_code = ee_pmw3901mb_spi_write(0x43, &value);
    if(status_code != 0) return status_code;
    
    value = 0x0E;
    status_code = ee_pmw3901mb_spi_write(0x4B, &value);
    if(status_code != 0) return status_code;
    
    value = 0x0F;
    status_code = ee_pmw3901mb_spi_write(0x45, &value);
    if(status_code != 0) return status_code;
    
    value = 0x42;
    status_code = ee_pmw3901mb_spi_write(0x44, &value);
    if(status_code != 0) return status_code;
    
    value = 0x80;
    status_code = ee_pmw3901mb_spi_write(0x4C, &value);
    if(status_code != 0) return status_code;
    
    value = 0x10;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x02;
    status_code = ee_pmw3901mb_spi_write(0x5B, &value);
    if(status_code != 0) return status_code;
    
    value = 0x07;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x41;
    status_code = ee_pmw3901mb_spi_write(0x40, &value);
    if(status_code != 0) return status_code;
    
    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(0x70, &value);
    if(status_code != 0) return status_code;
    
    ee_pmw3901mb_wait_ms(100U);

    value = 0x44;
    status_code = ee_pmw3901mb_spi_write(0x32, &value);
    if(status_code != 0) return status_code;

    value = 0x07;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x40;
    status_code = ee_pmw3901mb_spi_write(0x40, &value);
    if(status_code != 0) return status_code;
    
    value = 0x06;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0xF0;
    status_code = ee_pmw3901mb_spi_write(0x62, &value);
    if(status_code != 0) return status_code;
    
    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(0x63, &value);
    if(status_code != 0) return status_code;
    
    value = 0x0D;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0xC0;
    status_code = ee_pmw3901mb_spi_write(0x48, &value);
    if(status_code != 0) return status_code;
    
    value = 0xD5;
    status_code = ee_pmw3901mb_spi_write(0x6F, &value);
    if(status_code != 0) return status_code;
    
    value = 0x00;
    status_code = ee_pmw3901mb_spi_write(0x7F, &value);
    if(status_code != 0) return status_code;
    
    value = 0xA0;
    status_code = ee_pmw3901mb_spi_write(0x5B, &value);
    if(status_code != 0) return status_code;
    
    value = 0xA8;
    status_code = ee_pmw3901mb_spi_write(0x4E, &value);
    if(status_code != 0) return status_code;
    
    value = 0x50;
    status_code = ee_pmw3901mb_spi_write(0x5A, &value);
    if(status_code != 0) return status_code;
    
    value = 0x80;
    status_code = ee_pmw3901mb_spi_write(0x40, &value);
    if(status_code != 0) return status_code;

    return status_code; // Success
}

