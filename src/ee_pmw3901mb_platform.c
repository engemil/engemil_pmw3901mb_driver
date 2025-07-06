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

#include "ee_pmw3901mb_platform.h"

// R/W bit for PMW3901MB is in the MSB-bit of the 1st byte (SPI address) in the SPI Command Format
#define SPI_RW_BIT_READ_MASK    0x7F
#define SPI_RW_BIT_WRITE_MASK   0x80

// Include platform dependent macros and variables here

static SPIDriver* spi_driver = NULL;
static SPIConfig* spi_config = NULL;

// Include platform dependent function headers here

uint8_t ee_pmw3901mb_spi_init(SPIDriver* spid_p, SPIConfig* spic_p){
    if(spid_p == NULL) return 1; // Error: NULL pointer passed

    spi_driver = spid_p;
    spi_config = spic_p;
    return 0; // Success
}

uint8_t ee_pmw3901mb_spi_deinit(void){
    if(spi_driver == NULL) return 1; // Error: Not initialized / Already Deinitialized

    spi_driver = NULL;

    return 0;
}

uint8_t ee_pmw3901mb_spi_read(uint8_t addr, uint8_t* data, size_t n){
    
    if(spi_driver == NULL) return 1; // Error: SPI Driver is NULL
    if(n == 0U || n > 8U) return 2; // Error: Invalid length

    uint8_t txbuf = 0;
    /* Preparing the transmission buffer with R/W bit to Read. */
    txbuf = (SPI_RW_BIT_READ_MASK & addr);

    spiStart(spi_driver, spi_config);
    spiSelect(spi_driver);
    
    /* Sending the command. The data coming back is ignored. */
    spiSend(spi_driver, 1U, &txbuf);
    /* Reading back as many register as the value of n. */
    spiReceive(spi_driver, n, data);

    spiUnselect(spi_driver);
    spiStop(spi_driver);

    return 0; // Success
}

uint8_t ee_pmw3901mb_spi_write(uint8_t addr, uint8_t* data, size_t n){

    if(spi_driver == NULL) return 1; // Error: SPI Driver is NULL
    if(n == 0U || n > 8U) return 2; // Error: Invalid length

    uint8_t txbuf = 0;
    /* Preparing the transmission buffer with R/W bit to Write. */
    txbuf = (SPI_RW_BIT_WRITE_MASK | addr);

    spiStart(spi_driver, spi_config);
    spiSelect(spi_driver);
    
    /* Sending the command. The data coming back is ignored. */
    spiSend(spi_driver, 1U, &txbuf);
    /* Writing as many register as the value of n. */
    // TODO: Figure out why this crashes the system!
    //spiSend(spi_driver, n, data);

    spiUnselect(spi_driver);
    spiStop(spi_driver);

    return 0; // Success
}

uint8_t ee_pmw3901mb_wait_ms(uint32_t wait_ms){
    chThdSleepMilliseconds(wait_ms);
    return 0;
}

