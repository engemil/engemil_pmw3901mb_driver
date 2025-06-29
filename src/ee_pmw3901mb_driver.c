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


// SPI related
#define SPI_RW_BIT_READ         0x00
#define SPI_RW_BIT_WRITE        0x80

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


uint8_t ee_pmw3901mb_spi_init(SPIDriver* spidriver){
    return 0;
}

uint8_t ee_pmw3901mb_spi_deinit(SPIDriver* spidriver){
    return 0;
}

uint8_t ee_pmw3901mb_spi_read(SPIDriver* spidriver, uint8_t addr, uint16_t* data, uint8_t len){

    // reg |= SPI_RW_BIT_READ;

    // TODO: TEST and REWRITE

    uint8_t txbuf[2], rxbuf[2];
    /* Preparing the transmission buffer. */
    txbuf[0] = (SPI_RW_BIT_READ << 7) | ((addr) << 0);
    /* Performing an exchange. */
    spiSelect(spidriver);
    spiExchange(spidriver, 2, txbuf, rxbuf);
    spiUnselect(spidriver);
    *data = rxbuf[1];

    return 0;
}

uint8_t ee_pmw3901mb_spi_write(SPIDriver* spidriver, uint8_t addr, uint16_t* data, uint8_t len){
    return 0;
}

uint8_t ee_pmw3901mb_spi_write_perf_opt(SPIDriver* spidriver){
    return 0;
}
