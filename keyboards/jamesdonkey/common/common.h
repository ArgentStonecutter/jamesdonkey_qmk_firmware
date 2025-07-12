/* Copyright 2022 @ Keychron (https://www.keychron.com)
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

#include "stdint.h"
#include "config.h"

#define KC_TASK KC_TASK_VIEW
#define KC_FILE KC_FILE_EXPLORER
#define KC_SNAP KC_SCREEN_SHOT
#define KC_CTANA KC_CORTANA
#define KC_OSTG KC_OS_TOGGLE
#define KC_WLCK KC_WIN_LOCK_SCREEN
#define KC_MLCK KC_MAC_LOCK_SCREEN
#define SIDE_MOD SIDE_LED_STEP
#define SIDE_HUE SIDE_LED_HUE
#define SIDE_VAI SIDE_LED_VA_INCREASE
#define SIDE_VAD SIDE_LED_VAL_DECREASE

enum {
    KC_LOPTN = QK_KB_0,
    KC_ROPTN,
    KC_LCMMD,
    KC_RCMMD,
    KC_MCTRL,
    KC_LNPAD,
    KC_TASK_VIEW,
    KC_FILE_EXPLORER,
    KC_SCREEN_SHOT,
    KC_CORTANA,
#ifdef WIN_LOCK_SCREEN_ENABLE
    KC_WIN_LOCK_SCREEN,
    __KC_WIN_LOCK_SCREEN_NEXT,
#else
    __KC_WIN_LOCK_SCREEN_NEXT = KC_CORTANA + 1,
#endif
#ifdef MAC_LOCK_SCREEN_ENABLE
    KC_MAC_LOCK_SCREEN = __KC_WIN_LOCK_SCREEN_NEXT,
    __KC_MAC_LOCK_SCREEN_NEXT,
#else
    __KC_MAC_LOCK_SCREEN_NEXT = __KC_WIN_LOCK_SCREEN_NEXT,
#endif
    KC_SIRI = __KC_MAC_LOCK_SCREEN_NEXT,
#ifdef LK_WIRELESS_ENABLE
    BT_HST1,
    BT_HST2,
    BT_HST3,
    P2P4G,
    BAT_LVL,
#    if defined(TRANSPORT_SOFT_SWITCH_ENABLE)
    MD_USB,
    __MD_USB_NEXT,
#    else
    __MD_USB_NEXT = BAT_LVL + 1,
#    endif
    __LK_WIRELESS_ENABLE_NEXT = __MD_USB_NEXT,
#else
    BT_HST1                   = _______,
    BT_HST2                   = _______,
    BT_HST3                   = _______,
    P2P4G                     = _______,
    BAT_LVL                   = _______,
    __LK_WIRELESS_ENABLE_NEXT = KC_SIRI + 1,
#endif
#ifdef OS_TOGGLE_ENABLE
    KC_OS_TOGGLE = __LK_WIRELESS_ENABLE_NEXT,
    __KC_OS_TOGGLE_NEXT,
#else
    __KC_OS_TOGGLE_NEXT       = __LK_WIRELESS_ENABLE_NEXT,
#endif
#ifdef ANANLOG_MATRIX
    PROF1 = __KC_OS_TOGGLE_NEXT,
    PROF2,
    PROF3,
    __ANANLOG_MATRIX_NEXT,
#else
    __ANANLOG_MATRIX_NEXT     = __KC_OS_TOGGLE_NEXT,
#endif
#ifdef SIDE_LED_VDD
    SIDE_LED_STEP = __ANANLOG_MATRIX_NEXT,
    SIDE_LED_HUE,
    SIDE_LED_VA_INCREASE,
    SIDE_LED_VAL_DECREASE,
    __SIDE_LED_VDD_NEXT,
#else
    __SIDE_LED_VDD_NEXT       = __ANANLOG_MATRIX_NEXT,
#endif
#ifdef LED_MATRIX_ENABLE
    BL_SPI = __SIDE_LED_VDD_NEXT,
    BL_SPD,
#endif

    NEW_SAFE_RANGE = __SIDE_LED_VDD_NEXT,
};

#define KC_WLCK KC_WIN_LOCK_SCREEN
#define KC_MLCK KC_MAC_LOCK_SCREEN

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

bool process_record_common(uint16_t keycode, keyrecord_t *record);
void common_task(void);

#ifdef ENCODER_ENABLE
void encoder_cb_init(void);
#endif

#ifdef SIDE_LED_VDD
bool process_record_sideled_common(uint16_t keycode, keyrecord_t *record);
#endif

#if (EECONFIG_KB_DATA_SIZE == 1)
uint8_t eeprom_read_transport(void);
void    eeprom_update_transport(uint8_t val);
#endif
