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

#include QMK_KEYBOARD_H
#include "common.h"

enum cf_keycode {
    CF_TOGGLE = NEW_SAFE_RANGE,
    AP_GLOB,
};

enum layers {
    _WB,
    _WF,
    _MB,
    _MF,
    _BL,
};
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_WB] = LAYOUT_ansi_83(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,  KC_DEL,   KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,   KC_EQL,  KC_BSPC,  KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,   KC_RBRC, KC_BSLS,  KC_END,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,   KC_ENT,            KC_PGUP,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,   KC_RSFT, KC_UP,    KC_PGDN,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                       KC_RALT,  MO(_WF),   KC_RCTL,  KC_LEFT, KC_DOWN,  KC_RGHT),

    [_WF] = LAYOUT_ansi_83(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,  KC_PGUP,  _______,
        _______,  GU_TOGG,  _______,                                _______,                      DF(_MB),  _______,  MO(_BL),  KC_HOME,  KC_PGDN,  KC_END),

    [_MB] = LAYOUT_ansi_83(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,  KC_DEL,   KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,   KC_EQL,  KC_BSPC,  KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,   KC_RBRC, KC_BSLS,  KC_END,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,   KC_ENT,            KC_PGUP,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,   KC_RSFT, KC_UP,    KC_PGDN,
        KC_LCTL,  KC_LALT,  KC_LGUI,                                KC_SPC,                       KC_RGUI,  MO(_MF),   KC_RCTL,  KC_LEFT, KC_DOWN,  KC_RGHT),

    [_MF] = LAYOUT_ansi_83(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,  KC_PGUP,  _______,
        _______,  _______,  _______,                                _______,                      DF(_WB),  _______,  MO(_BL),  KC_HOME,  KC_PGDN,  KC_END),

    [_BL] = LAYOUT_ansi_83(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_TOG,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_SAD,  RGB_SAI,  RGB_MOD,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_HUD,  RGB_HUI,  _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_VAI,  CF_TOGGLE,
        _______,  _______,  _______,                                _______,                      _______,  _______,  _______,  RGB_SPD,  RGB_VAD,  RGB_SPI)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_WB] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_WF] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [_MB] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_MF] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [_BL] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)}
};
#endif // ENCODER_MAP_ENABLE

// clang-format on
#ifdef RGB_MATRIX_ENABLE
static uint8_t cf_magic = 0;
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DF(_WB):
            if (record->event.pressed) {
                set_single_persistent_default_layer(_WB);
            }
            return false;

        case DF(_MB):
            if (record->event.pressed) {
                set_single_persistent_default_layer(_MB);
            }
            return false;

#ifdef RGB_MATRIX_ENABLE
        case CF_TOGGLE:
            if (record->event.pressed)
                cf_magic = !cf_magic;
            return false;
#endif
	case AP_GLOB:
	    host_consumer_send(record->event.pressed ? AC_NEXT_KEYBOARD_LAYOUT_SELECT : 0);
	    return false;
    }

    if (!process_record_common(keycode, record)) {
        return false;
    }
    return true;
}

#ifdef RGB_MATRIX_ENABLE
static int colors[][3] = {
    {-1, -1, -1},
    {RGB_BLUE},
    {-1, -1, -1},
    {RGB_RED},
    {RGB_CYAN},
    {RGB_PURPLE},
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (!cf_magic)
        return true;

    int act = get_highest_layer(layer_state);
    int def = get_highest_layer(default_layer_state);

    // Ignore default layers
    if(act == def)
        return true;

    int *color_p = colors[act];

    // Ignore the special color -1, -1, -1
    if(color_p[0] == -1)
        return true;

    // Scale color to brightness level;
    int color[3];
    uint8_t rgb_val = rgblight_get_val();
    for (int i = 0; i < 3; i++) {
        int level = color_p[i];
        level *= rgb_val;
        level >>= 8;

        color[i] = level;
    }

    // Walk the matrix
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            // Ignore special keys (KC_NO, KC_TRNS, etc)
            uint16_t code = keymap_key_to_keycode(act, (keypos_t){col,row});
            if (IS_INTERNAL_KEYCODE(code))
                continue;

            // Look up the LED
            uint8_t index = g_led_config.matrix_co[row][col];

            // Ignore LEDs that aren't actually LEDs
            if (index < led_min || index >= led_max || index == NO_LED)
                continue;

            // Set the color
            rgb_matrix_set_color(index, color[0], color[1], color[2]);
        }
    }

    return false;
}

#endif
