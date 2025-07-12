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

#include "quantum.h"
#include "task.h"
#include "common.h"
#ifdef FACTORY_TEST_ENABLE
#    include "factory_test.h"
#endif
#ifdef LK_WIRELESS_ENABLE
#    include "lkbt51.h"
#    include "wireless.h"
#    include "transport.h"
#    include "wireless_common.h"
#    include "battery.h"
#endif

#define POWER_ON_LED_DURATION 3000
static uint32_t power_on_indicator_timer;

#ifdef BT_INDICATION_LED_PIN_LIST
pin_t bt_led_pins[] = BT_INDICATION_LED_PIN_LIST;
#endif

void keyboard_post_init_kb(void) {
    lkbt51_init(true);
#ifdef LK_WIRELESS_ENABLE
    palSetLineMode(BT_MODE_SELECT_PIN, PAL_MODE_INPUT);
#    ifdef P2P4_MODE_SELECT_PIN
    palSetLineMode(P2P4_MODE_SELECT_PIN, PAL_MODE_INPUT);
#    elif defined(USB_MODE_SELECT_PIN)
    palSetLineMode(USB_MODE_SELECT_PIN, PAL_MODE_INPUT);
#    endif

    writePin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
    lkbt51_init(false);
    wireless_init();
#endif

    power_on_indicator_timer = timer_read32();
#ifdef ENCODER_ENABLE
    encoder_cb_init();
#endif
    keyboard_post_init_user();
}

bool task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;

            if (!host_keyboard_led_state().caps_lock) writePin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
#ifdef LK_WIRELESS_ENABLE
            writePin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);
            if (get_transport() != TRANSPORT_P2P4) {
                for (uint8_t i = 0; i < sizeof(bt_led_pins) / sizeof(pin_t); i++)
                    writePin(bt_led_pins[i], 1);
                writePin(P24G_INDICATION_LED_PIN, 1);
            }

            if (get_transport() != TRANSPORT_BLUETOOTH) {
                writePin(P24G_INDICATION_LED_PIN, 1);
            }
#endif
        } else {
            writePin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
#ifdef LK_WIRELESS_ENABLE
            writePin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
            if (get_transport() != TRANSPORT_P2P4) {
                for (uint8_t i = 0; i < sizeof(bt_led_pins) / sizeof(pin_t); i++)
                    writePin(bt_led_pins[i], 0);
                writePin(P24G_INDICATION_LED_PIN, 1);
            }
            if (get_transport() != TRANSPORT_BLUETOOTH) {
                writePin(P24G_INDICATION_LED_PIN, 0);
            }
#endif
        }
    }
    return true;
}

#ifdef LK_WIRELESS_ENABLE
bool lpm_is_kb_idle(void) {
    return power_on_indicator_timer == 0 && !factory_reset_indicating();
}
#endif
