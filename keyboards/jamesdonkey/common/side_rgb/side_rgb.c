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

#include QMK_KEYBOARD_H
#include "common.h"
#ifdef FACTORY_TEST_ENABLE
#    include "factory_test.h"
#endif
#ifdef LK_WIRELESS_ENABLE
#    include "lkbt51.h"
#endif
#include "chyt_3528.h"

#define SIDE_RGB_MATRIX_HUE_STEP 8
#define SIDE_RGB_MATRIX_VAL_STEP 16

#define SIDE_RGB_MATRIX_EFFECT_MAX 4
#define SIDE_RGB_MATRIX_MOD_STEP 1
#define SIDE_RGB_MATRIX_LED_FLUSH_LIMIT 8 // 16

rgb_led_t    rgb_led_chyt3528[CHYT3528_LED_COUNT];
RGB          side_rgb;
rgb_config_t side_rgb_matrix_config;
rgb_config_t side_rgb_matrix_config_temp;

enum {
    SOLID = 1,
    BREATHING,
    CYCLE_HUE,
    CHASING,
};

#ifndef SIDE_EECONFIG_RGB_MATRIX
#    define SIDE_EECONFIG_RGB_MATRIX EECONFIG_KB_DATABLOCK
#endif
#if EECONFIG_KB_DATA_SIZE >= 8
EECONFIG_DEBOUNCE_HELPER(side_rgb_matrix, SIDE_EECONFIG_RGB_MATRIX, side_rgb_matrix_config);
#else
#    error "EECONFIG_KB_DATA_SIZE < 8"
#endif
#if !defined(SIDE_RGB_MATRIX_DEFAULT_HUE)
#    define SIDE_RGB_MATRIX_DEFAULT_HUE 0
#endif

#if !defined(SIDE_RGB_MATRIX_DEFAULT_SAT)
#    define SIDE_RGB_MATRIX_DEFAULT_SAT 255
#endif

#if !defined(SIDE_RGB_MATRIX_DEFAULT_VAL)
#    define SIDE_RGB_MATRIX_DEFAULT_VAL 255
#endif

#if !defined(SIDE_RGB_MATRIX_DEFAULT_SPD)
#    define SIDE_RGB_MATRIX_DEFAULT_SPD 1
#endif
#define SIDE_LED_FLAG_ALL 11
#define SIDE_LED_FLAG_NONE 0x00
static uint8_t hue_temp;
extern void    chyt3528_init(void);
extern bool    side_empty_voltage_flag;

/*hsv.v val increase*/
uint8_t side_qadd8(uint8_t i, uint8_t j) {
    uint16_t t = i + j;
    // if (t >= 255) t = 255;
    if (t >= 240) t = 240;
    return t;
}
/*hsv.v val decrease*/
uint8_t side_qsub8(uint8_t i, uint8_t j) {
    int16_t t;

    if (i > j)
        t = i - j;
    else
        t = 0;
    if (t <= 0) t = 0;
    return t;
}
/*hsv.v val cycle*/
uint8_t side_qcycle8(uint8_t i, uint8_t j, uint8_t k) {
    uint16_t       t;
    static uint8_t Status_reverse = 0xff;

    if (Status_reverse) {
        t = i + j;
        if (t >= k) {
            t              = k;
            Status_reverse = ~Status_reverse;
        }
    } else {
        if (i > j)
            t = i - j;
        else
            t = 0;

        if (t <= 0) {
            t              = 0;
            Status_reverse = ~Status_reverse;
        }
    }

    return t;
}

/*hsv.h hue increase*/
uint8_t side_qcycleadd8(uint8_t i, uint8_t j) {
    uint16_t t = i + j;
    if (t >= 255) t = 0;
    return t;
}
/*hsv.h hue decrease*/
uint8_t side_qcyclesub8(uint8_t i, uint8_t j) {
    int16_t t;

    if (i > j)
        t = i - j;
    else
        t = 0;
    if (t <= 0) t = 255;
    return t;
}

/*side_rgb_matrix_config.mode  MOD increase*/
uint8_t side_madd1(uint8_t i, uint8_t j) {
    uint16_t t = i + j;
    if (t > SIDE_RGB_MATRIX_EFFECT_MAX) t = 0;
    return t;
}

void side_light_power_off(void) {
    setPinOutput(SIDE_LED_VDD);
    writePin(SIDE_LED_VDD, !SIDE_LED_VDD_EN);
}

void side_light_power_on(void) {
    if (side_empty_voltage_flag) {
        side_rgb_matrix_config.enable = 0;
        side_rgb_matrix_config.mode   = 0;
    } else {
        setPinOutput(SIDE_LED_VDD);
        writePin(SIDE_LED_VDD, SIDE_LED_VDD_EN);
        chyt3528_init();
    }
}

void eeconfig_update_side_rgb_matrix_default(void) {
    side_rgb_matrix_config.enable = 1;
    side_rgb_matrix_config.mode   = CHASING; // SOLID  CHASING
    side_rgb_matrix_config.hsv    = (HSV){SIDE_RGB_MATRIX_DEFAULT_HUE, SIDE_RGB_MATRIX_DEFAULT_SAT, SIDE_RGB_MATRIX_DEFAULT_VAL};
    side_rgb_matrix_config.speed  = SIDE_RGB_MATRIX_DEFAULT_SPD;
    side_rgb_matrix_config.flags  = SIDE_LED_FLAG_ALL;
    eeconfig_flush_side_rgb_matrix(true);
}

void init_side_rgb_matrix_config(void) {
    eeconfig_init_side_rgb_matrix();
    if (!eeconfig_is_enabled()) {
        eeconfig_update_side_rgb_matrix_default();
    }
    side_rgb_matrix_config_temp = side_rgb_matrix_config;
}

bool process_record_sideled_common(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SIDE_HUE:
            if (side_rgb_matrix_config.mode == 0) return true;
            if (record->event.pressed) {
            } else if ((side_rgb_matrix_config.mode != CYCLE_HUE) && (side_rgb_matrix_config.mode != CHASING)) {
                side_rgb_matrix_config.hsv.h      = side_rgb_matrix_config.hsv.h + SIDE_RGB_MATRIX_HUE_STEP;
                side_rgb_matrix_config_temp.hsv.h = side_rgb_matrix_config.hsv.h;
                side_rgb                          = hsv_to_rgb_nocie(side_rgb_matrix_config.hsv);
                for (uint8_t i = 0; i < CHYT3528_LED_COUNT; i++) {
                    rgb_led_chyt3528[i].r = side_rgb.r;
                    rgb_led_chyt3528[i].g = side_rgb.g;
                    rgb_led_chyt3528[i].b = side_rgb.b;
                }
                if (side_rgb_matrix_config.mode != BREATHING) chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);

                eeconfig_flush_side_rgb_matrix(true);
            }
            return false;

        case SIDE_VAI:
            if (side_rgb_matrix_config.mode == 0) return true;
            if (record->event.pressed) {
            } else {
                side_rgb_matrix_config.hsv.v = side_qadd8(side_rgb_matrix_config.hsv.v, SIDE_RGB_MATRIX_VAL_STEP);

                if (side_rgb_matrix_config.mode == CHASING) side_rgb_matrix_config_temp.hsv.v = side_rgb_matrix_config.hsv.v;
                if ((side_rgb_matrix_config.mode != BREATHING) && (side_rgb_matrix_config.mode != CHASING)) {
                    side_rgb_matrix_config_temp.hsv.v = side_rgb_matrix_config.hsv.v;
                    side_rgb                          = hsv_to_rgb_nocie(side_rgb_matrix_config.hsv);
                    for (uint8_t i = 0; i < CHYT3528_LED_COUNT; i++) {
                        rgb_led_chyt3528[i].r = side_rgb.r;
                        rgb_led_chyt3528[i].g = side_rgb.g;
                        rgb_led_chyt3528[i].b = side_rgb.b;
                    }
                    if (side_rgb_matrix_config.mode != CYCLE_HUE) chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);
                }

                eeconfig_flush_side_rgb_matrix(true);
            }
            return false;

        case SIDE_VAD:
            if (side_rgb_matrix_config.mode == 0) return true;
            if (record->event.pressed) {
            } else {
                side_rgb_matrix_config.hsv.v = side_qsub8(side_rgb_matrix_config.hsv.v, SIDE_RGB_MATRIX_VAL_STEP);

                if (side_rgb_matrix_config.mode == CHASING) side_rgb_matrix_config_temp.hsv.v = side_rgb_matrix_config.hsv.v;
                if ((side_rgb_matrix_config.mode != BREATHING) && (side_rgb_matrix_config.mode != CHASING)) {
                    side_rgb_matrix_config_temp.hsv.v = side_rgb_matrix_config.hsv.v;
                    side_rgb                          = hsv_to_rgb_nocie(side_rgb_matrix_config.hsv);
                    for (uint8_t i = 0; i < CHYT3528_LED_COUNT; i++) {
                        rgb_led_chyt3528[i].r = side_rgb.r;
                        rgb_led_chyt3528[i].g = side_rgb.g;
                        rgb_led_chyt3528[i].b = side_rgb.b;
                    }
                    if (side_rgb_matrix_config.mode != CYCLE_HUE) chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);
                }

                eeconfig_flush_side_rgb_matrix(true);
            }
            return false;

        case SIDE_MOD:
            if (record->event.pressed) {
            } else {
                side_rgb_matrix_config_temp      = side_rgb_matrix_config;
                side_rgb_matrix_config.mode      = side_madd1(side_rgb_matrix_config.mode, SIDE_RGB_MATRIX_MOD_STEP);
                side_rgb_matrix_config_temp.mode = side_rgb_matrix_config.mode;

                if (side_rgb_matrix_config.mode) {
                    side_rgb_matrix_config.enable = 1;
                } else {
                    side_rgb_matrix_config.enable ^= 1;
                    for (uint8_t i = 0; i < CHYT3528_LED_COUNT; i++) {
                        rgb_led_chyt3528[i].r = 0;
                        rgb_led_chyt3528[i].g = 0;
                        rgb_led_chyt3528[i].b = 0;
                    }
                    chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);
                }

                hue_temp = side_rgb_matrix_config.hsv.h;
                eeconfig_flush_side_rgb_matrix(true);
            }
            return false;
        default:
            return true; // Process all other keycodes normally
    }
}

void side_rgb_matrix_mode_Task(void) {
    static uint8_t flush_speed = 0;

    switch (side_rgb_matrix_config.mode) {
        case SOLID: // SOLID
            if (++flush_speed >= side_rgb_matrix_config_temp.speed * 25) {
                flush_speed = 0;
                side_rgb    = hsv_to_rgb_nocie(side_rgb_matrix_config.hsv);
                for (uint8_t i = 0; i < CHYT3528_LED_COUNT; i++) {
                    rgb_led_chyt3528[i].r = side_rgb.r;
                    rgb_led_chyt3528[i].g = side_rgb.g;
                    rgb_led_chyt3528[i].b = side_rgb.b;
                }
                chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);
            }
            break;

        case BREATHING: // BREATHING
            if (++flush_speed >= side_rgb_matrix_config_temp.speed * 255 / side_rgb_matrix_config.hsv.v) {
                flush_speed                       = 0;
                side_rgb_matrix_config_temp.hsv.v = side_qcycle8(side_rgb_matrix_config_temp.hsv.v, 1, side_rgb_matrix_config.hsv.v);

                side_rgb = hsv_to_rgb_nocie(side_rgb_matrix_config_temp.hsv);
                for (uint8_t i = 0; i < CHYT3528_LED_COUNT; i++) {
                    rgb_led_chyt3528[i].r = side_rgb.r;
                    rgb_led_chyt3528[i].g = side_rgb.g;
                    rgb_led_chyt3528[i].b = side_rgb.b;
                }
                chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);
            }
            break;

        case CYCLE_HUE: // CYCLE_HUE
            if (++flush_speed >= side_rgb_matrix_config_temp.speed * 2) {
                flush_speed                       = 0;
                side_rgb_matrix_config_temp.hsv.h = side_qcycleadd8(side_rgb_matrix_config_temp.hsv.h, 1);

                side_rgb = hsv_to_rgb_nocie(side_rgb_matrix_config_temp.hsv);
                for (uint8_t i = 0; i < CHYT3528_LED_COUNT; i++) {
                    rgb_led_chyt3528[i].r = side_rgb.r;
                    rgb_led_chyt3528[i].g = side_rgb.g;
                    rgb_led_chyt3528[i].b = side_rgb.b;
                }
                chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);
            }
            break;

        case CHASING: // CHASING
            if (++flush_speed >= side_rgb_matrix_config_temp.speed * 20) {
                flush_speed = 0;

                side_rgb_matrix_config_temp.hsv.h          = hue_temp;
                side_rgb_matrix_config_temp.hsv.h          = side_qcyclesub8(side_rgb_matrix_config_temp.hsv.h, SIDE_RGB_MATRIX_HUE_STEP);
                hue_temp                                   = side_rgb_matrix_config_temp.hsv.h;
                side_rgb                                   = hsv_to_rgb_nocie(side_rgb_matrix_config_temp.hsv);
                rgb_led_chyt3528[CHYT3528_LED_COUNT - 1].r = side_rgb.r;
                rgb_led_chyt3528[CHYT3528_LED_COUNT - 1].g = side_rgb.g;
                rgb_led_chyt3528[CHYT3528_LED_COUNT - 1].b = side_rgb.b;
                for (uint8_t i = CHYT3528_LED_COUNT - 2; i > 0; i--) {
                    side_rgb_matrix_config_temp.hsv.h = side_qcyclesub8(side_rgb_matrix_config_temp.hsv.h, SIDE_RGB_MATRIX_HUE_STEP);
                    side_rgb                          = hsv_to_rgb_nocie(side_rgb_matrix_config_temp.hsv);
                    rgb_led_chyt3528[i].r             = side_rgb.r;
                    rgb_led_chyt3528[i].g             = side_rgb.g;
                    rgb_led_chyt3528[i].b             = side_rgb.b;
                }
                side_rgb_matrix_config_temp.hsv.h = side_qcyclesub8(side_rgb_matrix_config_temp.hsv.h, SIDE_RGB_MATRIX_HUE_STEP);
                side_rgb                          = hsv_to_rgb_nocie(side_rgb_matrix_config_temp.hsv);
                rgb_led_chyt3528[0].r             = side_rgb.r;
                rgb_led_chyt3528[0].g             = side_rgb.g;
                rgb_led_chyt3528[0].b             = side_rgb.b;

                chyt3528_setleds(&rgb_led_chyt3528[0], CHYT3528_LED_COUNT);
            }
            break;

        default:
            break;
    }
}

void housekeeping_task_user(void) {
#ifdef SIDE_LED_VDD
    static uint32_t flush_time = 0;
    if (flush_time == 0) {
        flush_time = timer_read32();
    }

    if ((flush_time && timer_elapsed32(flush_time) >= SIDE_RGB_MATRIX_LED_FLUSH_LIMIT) && (side_rgb_matrix_config.enable == 1)) {
        flush_time = 0;
        side_rgb_matrix_mode_Task();
    }
#endif
}
