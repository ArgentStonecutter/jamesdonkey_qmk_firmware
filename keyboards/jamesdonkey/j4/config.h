/* Copyright 2024 @ James Donkey
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

#pragma once
/* Encoder Configuration */

/* I2C Driver Configuration */
#define I2C1_SCL_PIN B8
#define I2C1_SDA_PIN B9
#define I2C1_CLOCK_SPEED 400000
#define I2C1_DUTY_CYCLE FAST_DUTY_CYCLE_2
#define I2C1_OPMODE OPMODE_I2C

/* EEPROM Driver Configuration */
#define EXTERNAL_EEPROM_BYTE_COUNT 2048
#define EXTERNAL_EEPROM_PAGE_SIZE  32
#define EXTERNAL_EEPROM_WRITE_TIME 3
#define EXTERNAL_EEPROM_I2C_BASE_ADDRESS 0b10100010
#define KEYCODE_BUFFER_ENABLE

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix driver configuration */
#    define DRIVER_COUNT 2
#    define DRIVER_CS_PINS \
        { C13, A15 }
#    define RGB_MATRIX_LED_COUNT 87
#    define SNLED23751_SPI_DIVISOR 16
#    define LED_DRIVER_SHUTDOWN_PIN B7

/* Set LED driver current */
#    define SNLED27351_CURRENT_TUNE \
        { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }

/* Set to infinit, which is use in USB mode by default */
#    define RGB_MATRIX_TIMEOUT RGB_MATRIX_TIMEOUT_INFINITE

/* Allow shutdown of led driver to save power */
#    define RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE
/* Turn off backlight on low brightness to save power */
#    define RGB_MATRIX_BRIGHTNESS_TURN_OFF_VAL 48

/* Indications */
#    define WINLOCK_LED_LIST \
        { 77 }
#    define WIN_LOCK_HOLD_TIME 3000

#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#endif

#ifdef LK_WIRELESS_ENABLE
/* Hardware configuration */
#    define P2P4_MODE_SELECT_PIN B14
#    define BT_MODE_SELECT_PIN B15

#    define LKBT51_RESET_PIN C4
#    define LKBT51_INT_INPUT_PIN B1
#    define BLUETOOTH_INT_OUTPUT_PIN A4

#    define USB_POWER_SENSE_PIN B0
#    define USB_POWER_CONNECTED_LEVEL 0

#    define BAT_CHARGING_PIN B10
#    define BAT_CHARGING_LEVEL 0

#    define BAT_LOW_LED_PIN C5
#    define BAT_LOW_LED_PIN_ON_STATE 1

#    define DP_PULLUP_CONTROL_PIN C6

#    define BT_HOST_DEVICES_COUNT 3

#    define BT_INDICATION_LED_PIN_LIST \
        { A2, A2, A2 }

#    define BT_INDICATION_LED_ON_STATE 0

#    define P24G_INDICATION_LED_PIN A3
#    define P24G_INDICATION_LED_PIN_ON_STATE 0

#    if defined(RGB_MATRIX_ENABLE)


#        define BT_INDICATION_LED_LIST \
            { 17, 18, 19 }

#        define BAT_LEVEL_LED_LIST \
            { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 }

#    define  P24G_INDICATION_LED_INDEX 20

/* Reinit LED driver on tranport changed */
#        define REINIT_LED_DRIVER 1
#    endif

/* Keep USB connection in blueooth mode */
#    define KEEP_USB_CONNECTION_IN_WIRELESS_MODE
/* Enable bluetooth NKRO */
#    define WIRELESS_NKRO_ENABLE
#endif

#if defined(RGB_MATRIX_ENABLE) || defined(LK_WIRELESS_ENABLE)
/* SPI configuration */
#    define SPI_DRIVER SPIDQ
#    define SPI_SCK_PIN A5
#    define SPI_MISO_PIN A6
#    define SPI_MOSI_PIN A7
#endif

/* Factory test keys */
#define FN_KEY_1 MO(1)
#define FN_KEY_2 MO(3)

#define MATRIX_IO_DELAY 10
