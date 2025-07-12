/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 #include "chyt_3528.h"
#include "gpio.h"
#include "util.h"
#include "chibios_config.h"

// Define the spi your LEDs are plugged to here
#ifndef CHYT3528_SPI_DRIVER
#    define CHYT3528_SPI_DRIVER SPID1
#endif

#ifndef CHYT3528_SPI_MOSI_PAL_MODE
#    define CHYT3528_SPI_MOSI_PAL_MODE 5
#endif

#ifndef CHYT3528_SPI_SCK_PAL_MODE
#    define CHYT3528_SPI_SCK_PAL_MODE 5
#endif

#ifndef CHYT3528_SPI_DIVISOR
#    define CHYT3528_SPI_DIVISOR 16
#endif

// Push Pull or Open Drain Configuration
// Default Push Pull
#ifndef CHYT3528_EXTERNAL_PULLUP
#    if defined(USE_GPIOV1)
#        define CHYT3528_MOSI_OUTPUT_MODE PAL_MODE_ALTERNATE_PUSHPULL
#    else
#        define CHYT3528_MOSI_OUTPUT_MODE PAL_MODE_ALTERNATE(CHYT3528_SPI_MOSI_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL
#    endif
#else
#    if defined(USE_GPIOV1)
#        define CHYT3528_MOSI_OUTPUT_MODE PAL_MODE_ALTERNATE_OPENDRAIN
#    else
#        define CHYT3528_MOSI_OUTPUT_MODE PAL_MODE_ALTERNATE(CHYT3528_SPI_MOSI_PAL_MODE) | PAL_OUTPUT_TYPE_OPENDRAIN
#    endif
#endif

// Define SPI config speed
// baudrate should target 3.2MHz
// F072 fpclk = 48MHz
// 48/16 = 3Mhz
#if CHYT3528_SPI_DIVISOR == 2
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (0)
#elif CHYT3528_SPI_DIVISOR == 4
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (SPI_CR1_BR_0)
#elif CHYT3528_SPI_DIVISOR == 8
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (SPI_CR1_BR_1)
#elif CHYT3528_SPI_DIVISOR == 16 // default
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (SPI_CR1_BR_1 | SPI_CR1_BR_0)
#elif CHYT3528_SPI_DIVISOR == 32
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (SPI_CR1_BR_2)
#elif CHYT3528_SPI_DIVISOR == 64
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (SPI_CR1_BR_2 | SPI_CR1_BR_0)
#elif CHYT3528_SPI_DIVISOR == 128
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (SPI_CR1_BR_2 | SPI_CR1_BR_1)
#elif CHYT3528_SPI_DIVISOR == 256
#    define CHYT3528_SPI_DIVISOR_CR1_BR_X (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)
#else
#    error "Configured CHYT3528_SPI_DIVISOR value is not supported at this time."
#endif

// Use SPI circular buffer
#ifdef CHYT3528_SPI_USE_CIRCULAR_BUFFER
#    define CHYT3528_SPI_BUFFER_MODE 1 // circular buffer
#else
#    define CHYT3528_SPI_BUFFER_MODE 0 // normal buffer
#endif

#if defined(USE_GPIOV1)
#    define CHYT3528_SCK_OUTPUT_MODE PAL_MODE_ALTERNATE_PUSHPULL
#else
#    define CHYT3528_SCK_OUTPUT_MODE PAL_MODE_ALTERNATE(CHYT3528_SPI_SCK_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL
#endif

#define BYTES_FOR_LED_BYTE 4
#ifdef RGBW
#    define CHYT3528_CHANNELS 4
#else
#    define CHYT3528_CHANNELS 3
#endif
#define BYTES_FOR_LED (BYTES_FOR_LED_BYTE * CHYT3528_CHANNELS)
#define DATA_SIZE (BYTES_FOR_LED * CHYT3528_LED_COUNT)
#define RESET_SIZE (1000 * CHYT3528_TRST_US / (2 * CHYT3528_TIMING))
#define PREAMBLE_SIZE 4

static uint8_t txbuf[PREAMBLE_SIZE + DATA_SIZE + RESET_SIZE] = {0};

/*
 * As the trick here is to use the SPI to send a huge pattern of 0 and 1 to
 * the chyt3528b protocol, we use this helper function to translate bytes into
 * 0s and 1s for the LED (with the appropriate timing).
 */
static uint8_t get_protocol_eq(uint8_t data, int pos) {
    uint8_t eq = 0;
    if (data & (1 << (2 * (3 - pos))))
        eq = 0b1110;
    else
        eq = 0b1000;
    if (data & (2 << (2 * (3 - pos))))
        eq += 0b11100000;
    else
        eq += 0b10000000;
    return eq;
}

static void set_led_color_rgb(rgb_led_t color, int pos) {
    uint8_t* tx_start = &txbuf[PREAMBLE_SIZE];

#if (CHYT3528_BYTE_ORDER == CHYT3528_BYTE_ORDER_GRB)
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + j] = get_protocol_eq(color.g, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE + j] = get_protocol_eq(color.r, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 2 + j] = get_protocol_eq(color.b, j);
#elif (CHYT3528_BYTE_ORDER == CHYT3528_BYTE_ORDER_RGB)
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + j] = get_protocol_eq(color.r, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE + j] = get_protocol_eq(color.g, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 2 + j] = get_protocol_eq(color.b, j);
#elif (CHYT3528_BYTE_ORDER == CHYT3528_BYTE_ORDER_BGR)
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + j] = get_protocol_eq(color.b, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE + j] = get_protocol_eq(color.g, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 2 + j] = get_protocol_eq(color.r, j);
#endif
#ifdef RGBWr
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 4 + j] = get_protocol_eq(color.w, j);
#endif
}

void chyt3528_init(void) {
    palSetLineMode(CHYT3528_DI_PIN, CHYT3528_MOSI_OUTPUT_MODE);

#ifdef CHYT3528_SPI_SCK_PIN
    palSetLineMode(CHYT3528_SPI_SCK_PIN, CHYT3528_SCK_OUTPUT_MODE);
#endif // CHYT3528_SPI_SCK_PIN

    // TODO: more dynamic baudrate
    static const SPIConfig spicfg = {
#ifndef HAL_LLD_SELECT_SPI_V2
// HAL_SPI_V1
#    if SPI_SUPPORTS_CIRCULAR == TRUE
        CHYT3528_SPI_BUFFER_MODE,
#    endif
        NULL, // end_cb
        PAL_PORT(CHYT3528_DI_PIN),
        PAL_PAD(CHYT3528_DI_PIN),
#    if defined(WB32F3G71xx) || defined(WB32FQ95xx)
        0,
        0,
        CHYT3528_SPI_DIVISOR
#    else
        CHYT3528_SPI_DIVISOR_CR1_BR_X,
        0
#    endif
#else
    // HAL_SPI_V2
#    if SPI_SUPPORTS_CIRCULAR == TRUE
        CHYT3528_SPI_BUFFER_MODE,
#    endif
#    if SPI_SUPPORTS_SLAVE_MODE == TRUE
        false,
#    endif
        NULL, // data_cb
        NULL, // error_cb
        PAL_PORT(CHYT3528_DI_PIN),
        PAL_PAD(CHYT3528_DI_PIN),
        CHYT3528_SPI_DIVISOR_CR1_BR_X,
        0
#endif
    };

    spiAcquireBus(&CHYT3528_SPI_DRIVER);     /* Acquire ownership of the bus.    */
    spiStart(&CHYT3528_SPI_DRIVER, &spicfg); /* Setup transfer parameters.       */
    spiSelect(&CHYT3528_SPI_DRIVER);         /* Slave Select assertion.          */
#ifdef CHYT3528_SPI_USE_CIRCULAR_BUFFER
    spiStartSend(&CHYT3528_SPI_DRIVER, ARRAY_SIZE(txbuf), txbuf);
#endif
}

void chyt3528_setleds(rgb_led_t* ledarray, uint16_t leds) {
    static bool s_init = false;
    if (!s_init) {
        chyt3528_init();
        s_init = true;
    }

    for (uint8_t i = 0; i < leds; i++) {
        set_led_color_rgb(ledarray[i], i);
    }

    // Send async - each led takes ~0.03ms, 50 leds ~1.5ms, animations flushing faster than send will cause issues.
    // Instead spiSend can be used to send synchronously (or the thread logic can be added back).
#ifndef CHYT3528_SPI_USE_CIRCULAR_BUFFER
#    ifdef CHYT3528_SPI_SYNC
    spiSend(&CHYT3528_SPI_DRIVER, ARRAY_SIZE(txbuf), txbuf);
#    else
    spiStartSend(&CHYT3528_SPI_DRIVER, ARRAY_SIZE(txbuf), txbuf);
#    endif
#endif
}
