//
// Created by limbang-pc on 2019/12/31.
//

#include "user_led.h"

led_handle_t led;

/**
 * LED闪烁一次,循环的闪烁的时候要加延迟
 */
void USER_FUNC user_led_blink(void) {
    led_blink_set(led);
}

/**
 * LED关闭
 */
void USER_FUNC user_led_off(void) {
    led_status_set(led, LED_STATUS_OFF);
}

/**
 * LED状态获取
 */
led_status_e USER_FUNC user_led_states_get(void) {
    return led_states_get(led);
}

/**
 * LED开启
 */
void USER_FUNC user_led_on(void) {
    led_status_set(led, LED_STATUS_ON);
}

void USER_FUNC user_led_init(void) {
    // 初始化LED
    led = led_create(GPIO5, GPIO_HIGH);
}
