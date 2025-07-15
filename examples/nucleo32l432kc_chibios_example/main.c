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


/* IMPORTANT:
 * - To use float support, change CHPRINTF_USE_FLOAT to TRUE in ChibiOS/os/hal/lib/streams/chprintf.h.
 * - To use math, include <math.h> and link with -lm. (.e.g. in Makefile: ULIBS = -lm)
 */

#include <math.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "ee_pmw3901mb_driver.h"

/* Serial / Virtual COM Port related */
#define VIRTUAL_COM_TX_LINE         LINE_VCP_TX // UART2_TX (PA2)
#define VIRTUAL_COM_TX_LINE_MODE    PAL_MODE_ALTERNATE(7U) /* AF7: USART2_TX */ | \
                                    PAL_STM32_OTYPE_PUSHPULL | \
                                    PAL_STM32_OSPEED_MID2 | \
                                    PAL_STM32_PUPDR_FLOATING
#define VIRTUAL_COM_RX_LINE         LINE_VCP_RX // UART2_RX (PA15)
#define VIRTUAL_COM_RX_LINE_MODE    PAL_MODE_ALTERNATE(3U) /* AF3: USART2_RX */ | \
                                    PAL_STM32_OTYPE_PUSHPULL | \
                                    PAL_STM32_OSPEED_MID2 | \
                                    PAL_STM32_PUPDR_FLOATING
#define my_serial_driver (&SD2)
#define my_serial_stream (BaseSequentialStream*) my_serial_driver

/* SPI related */
#define SPI_MOSI_LINE       LINE_ARD_A6 // SPI1_MOSI (PA7)
#define SPI_MOSI_LINE_MODE  PAL_MODE_ALTERNATE(5U) /* AF5: SPI1_MOSI */ | \
                            PAL_STM32_OSPEED_HIGHEST
#define SPI_MISO_LINE       LINE_ARD_A5 // SPI1_MISO (PA6)
#define SPI_MISO_LINE_MODE  PAL_MODE_ALTERNATE(5U) /* AF5: SPI1_MISO */ | \
                            PAL_STM32_PUPDR_PULLUP /* Added SW PULL-UP (No HW PULL-UP on module) */ | \
                            PAL_STM32_OSPEED_HIGHEST
#define SPI_SCLK_LINE       LINE_ARD_A4 // SPI1_SCLK (PA5)
#define SPI_SCLK_LINE_MODE  PAL_MODE_ALTERNATE(5U) /* AF5: SPI1_SCK */| \
                            PAL_STM32_OSPEED_HIGHEST
#define SPI_CS_LINE         LINE_ARD_A3 // "Chip Select" (PA4)
#define SPI_CS_LINE_MODE    PAL_MODE_OUTPUT_PUSHPULL | \
                            PAL_STM32_PUPDR_PULLUP /* Added SW PULL-UP (No HW PULL-UP on module) */
#define MOT_INT_LINE        LINE_ARD_A2 // "Motion Interrupt" (PA3)
#define MOT_INT_LINE_MODE   PAL_MODE_OUTPUT_PUSHPULL | \
                            PAL_STM32_PUPDR_PULLUP /* Added SW PULL-UP (No HW PULL-UP on module) */
#define RESET_LINE          LINE_ARD_A1 // "Reset" (PA1)
#define RESET_LINE_MODE     PAL_MODE_OUTPUT_PUSHPULL  /* No need for SW PULL-UP (Module has HW PULL-UP) */
#define my_spi_driver (&SPID1)

/* Serial Configuration for Virtual COM Port */
static const SerialConfig my_serial_cfg = {
    .speed  = 115200,               // Baud rate
    .cr1    = 0,                    // No parity, 8-bit data (default)
    .cr2    = USART_CR2_STOP1_BITS, // Stop1 bit
    .cr3    = 0                     // No hardware flow control
};

/*
 * Non-circular SPI.
 * No slave.
 * No callbacks (error and data)
 * CR1: BR = 0b0101 (f_PCLK/64 prescaler), CPOL = 1 CPHA = 1 (SPI Mode 3). CR2: DS = 0b0111 (8-bit data length)
 */
static SPIConfig my_spi_cfg = {
    .circular   = false,                        // Non-circular mode
    .slave      = false,                        // Master mode
    .data_cb    = NULL,                         // No callback
    .error_cb   = NULL,                         // No callback
    .ssline     = SPI_CS_LINE,                  // Chip Select Pin
    .cr1        = SPI_CR1_BR_2 | SPI_CR1_BR_0 | // BR = 0b0101 => 1.25MHz (f_PCLK/64). Nucleo-L432KC default STM32_PCLK2 (used by SPI1) is 80MHz.
                  SPI_CR1_CPOL | SPI_CR1_CPHA | // (SPI Mode 3) CPOL = 1 (CLK 1 at idle) and CPHA = 1 (2nd CLK trans data capture edge), 
                  0,                            // MSTR = 0 (is set by chibios), and more...
    .cr2        = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | // DS = 0b0111 (8-bit data length)
                  0                                            // FRF = 0 (Motorola Mode), and more...
};


/* System running indicator, LED blinker thread. */
static THD_WORKING_AREA(waThdBlinker, 128);
static THD_FUNCTION(ThdBlinker, arg) {
    (void) arg;
    chRegSetThreadName("blinker");
    while(true) {
        palToggleLine(LINE_LED_GREEN);
        chThdSleepMilliseconds(500);
    }
}

float calc_angle(int16_t *delta_x, int16_t *delta_y) {
    return atan2((float) *delta_y, (float) *delta_x) * (180.0f / 3.14159265f);
}

double calc_magnitude(int16_t *delta_x, int16_t *delta_y) {
    return sqrt((double)(*delta_x * *delta_x + *delta_y * *delta_y));
}

/* Main function */
int main(void) {

    /* System initializations. */
    halInit();
    chSysInit();

    /* Creating a dummy blinker thread. */
    chThdCreateStatic(waThdBlinker, sizeof(waThdBlinker), NORMALPRIO, ThdBlinker, NULL);
    
    /* Configure Serial Driver for Virtual COM Port pins */
    palSetLineMode(VIRTUAL_COM_TX_LINE, VIRTUAL_COM_TX_LINE_MODE);
    palSetLineMode(VIRTUAL_COM_RX_LINE, VIRTUAL_COM_RX_LINE_MODE);

    /* Starting Serial Driver with configuration. */
    sdStart(my_serial_driver, &my_serial_cfg);

    /* Configuring SPI pins */
    palSetLineMode(SPI_MOSI_LINE, SPI_MOSI_LINE_MODE);
    palSetLineMode(SPI_MISO_LINE, SPI_MISO_LINE_MODE);
    palSetLineMode(SPI_SCLK_LINE, SPI_SCLK_LINE_MODE);
    palSetLineMode(SPI_CS_LINE, SPI_CS_LINE_MODE);
    palSetLineMode(MOT_INT_LINE, MOT_INT_LINE_MODE);
    palSetLineMode(RESET_LINE, RESET_LINE_MODE);

    /* Print initial message */
    chprintf(my_serial_stream, "Starting...\r\n");

    uint8_t product_id = 0x00;
    int16_t delta_x = 0;
    int16_t delta_y = 0;
    uint8_t status_code = 0x00;

    ee_pmw3901mb_init_driver(my_spi_driver, &my_spi_cfg);

    // Product ID
    chprintf(my_serial_stream, "Checking product ID...\r\n");
    status_code = ee_pmw3901mb_get_product_id(&product_id);
    if(status_code == 0){
        chprintf(my_serial_stream, "Product ID: 0x%02X \r\n", product_id);
    }else{
        chprintf(my_serial_stream, "Failed to read product ID!\r\n");
        chprintf(my_serial_stream, "Status Code: 0x%02X \r\n", status_code);
        chprintf(my_serial_stream, "NB! System loop is stopping!\r\n");
        while (true) {
            chThdSleepMilliseconds(1000);
        }
    }
    product_id = 0x00;


    // Main Thread
    while (true) {

        // Reading X and Y values
        status_code = ee_pmw3901mb_get_delta_x_y(&delta_x, &delta_y);

        if(status_code == 0){
            if(CHPRINTF_USE_FLOAT) {
                chprintf(my_serial_stream, "X: %d , Y: %d , Magnitue: %f , Angle (deg.): %f \r\n", 
                delta_x,
                delta_y,
                calc_magnitude(&delta_x, &delta_y),
                calc_angle(&delta_x, &delta_y));
            }else{
                chprintf(my_serial_stream, "X: %d , Y: %d \r\n", delta_x, delta_y);
            }
        }else{
            chprintf(my_serial_stream, "Failed to read!\r\n");
            chprintf(my_serial_stream, "Status Code: 0x%04X \r\n", status_code);
        }
        delta_x = 0x00;
        delta_y = 0x00;
        status_code = 0x00;
        chThdSleepMilliseconds(300);
        
    }

}

