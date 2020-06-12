//
// Created by limbang-pc on 2019/12/31.
//

#ifndef SRC_COMPONENTS_USER_INCLUDE_USER_LED_H_
#define SRC_COMPONENTS_USER_INCLUDE_USER_LED_H_

#include "led.h"

void USER_FUNC user_led_init(void);

void USER_FUNC user_led_blink(void);

void USER_FUNC user_led_on(void);

void USER_FUNC user_led_off(void);

led_status_e USER_FUNC user_led_states_get(void);

#endif //SRC_COMPONENTS_USER_INCLUDE_USER_LED_H_
