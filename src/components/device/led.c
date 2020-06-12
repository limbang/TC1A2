/*
 * led.c
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#include "led.h"

typedef struct {
    gpio_number_e gpio_num;
    gpio_level_e trigger_level;
    led_status_e status;
} led_device_t;

/**
 * Create led
 *
 * @param gpioNum
 * @param triggerLevel
 * @return ledHandle
 */
led_handle_t USER_FUNC led_create(gpio_number_e gpio_num, gpio_level_e trigger_level) {
    led_device_t *led = (led_device_t *) hfmem_malloc(sizeof(led_device_t));
    if (led == NULL)
        return NULL;
    led->gpio_num = gpio_num;
    led->trigger_level = trigger_level;
    led->status = LED_STATUS_OFF;
    return (led_handle_t) led;
}

/**
 * delete led
 * @param handle
 */
void USER_FUNC led_delete(led_handle_t handle) {
    hfmem_free(handle);
}

/**
 * set led status
 *
 * @param handle
 * @param status LED_STATUS_OFF,LED_STATUS_ON
 * @return
 */
bool USER_FUNC led_status_set(led_handle_t handle, led_status_e status) {
    led_device_t *led = (led_device_t *) handle;
    switch (status) {
    case LED_STATUS_OFF:
    case LED_STATUS_ON:
        if (!gpio_output_set(led->gpio_num, (0x01 & status) ^ (!led->trigger_level)))
            return false;
        break;
    default:
        break;
    }
    led->status = status;
    return true;
}

/**
 * led Blink once, delay should be added when the cycle flashes
 *
 * @param handle
 */
void USER_FUNC led_blink_set(led_handle_t handle) {
    if (led_states_get(handle) == LED_STATUS_ON) {
        led_status_set(handle, LED_STATUS_OFF);
    } else {
        led_status_set(handle, LED_STATUS_ON);
    }
}

/**
 * get led states
 *
 * @param handle
 * @return
 */
led_status_e USER_FUNC led_states_get(led_handle_t handle) {
    led_device_t *led = (led_device_t *) handle;
    return led->status;
}
