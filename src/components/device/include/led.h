/*
 * led.h
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#ifndef SRC_COMPONENTS_USER_INCLUDE_LED_H_
#define SRC_COMPONENTS_USER_INCLUDE_LED_H_

#include "gpio.h"

typedef void *led_handle_t;

typedef enum {
    LED_STATUS_OFF, /* LED状态关闭 */
    LED_STATUS_ON, /* LED状态打开 */
} led_status_e;

led_handle_t USER_FUNC led_create(gpio_number_e gpio_num, gpio_level_e trigger_level);

void USER_FUNC led_delete(led_handle_t handle);

bool USER_FUNC led_status_set(led_handle_t handle, led_status_e status);

void USER_FUNC led_blink_set(led_handle_t handle);

led_status_e USER_FUNC led_states_get(led_handle_t handle);

#endif /* SRC_COMPONENTS_USER_INCLUDE_LED_H_ */
