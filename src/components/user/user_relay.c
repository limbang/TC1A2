//
// Created by limbang-pc on 2019/12/31.
//

#include "user_relay.h"

relay_handle_t _relays[6];
uint32_t _relays_gpio[6] = { GPIO22, GPIO24, GPIO8, GPIO23, GPADC0, GPIO3 };

/**
 * 继电器状态设置
 * @param id id
 */
relay_status_e USER_FUNC user_relay_states_set(uint32_t id,relay_status_e status) {
    return relay_states_set(_relays[id],status);
}


/**
 * 继电器状态获取
 * @param id id
 */
relay_status_e USER_FUNC user_relay_states_get(uint32_t id) {
    return relay_states_get(_relays[id]);
}

/**
 * 判断继电器是否全部关闭,如果全部关闭就熄灭LED灯
 */
void USER_FUNC user_relay_led_state(void) {
    for (int i = 0; i < 6; i++) {
        if (relay_states_get(_relays[i]) == RELAY_STATUS_OPEN){
            if(user_led_states_get() == LED_STATUS_OFF)
                user_led_on();
            return;
        }
    }
    user_led_off();
}

/**
 * 全部继电器控制
 * @param status 状态
 */
void USER_FUNC user_relay_aLL_set(relay_status_e status) {
    for (int i = 0; i < 6; i++) {
        relay_states_set(_relays[i], status);
    }
    mqtt_relay_update_flag(true);
}

void USER_FUNC user_relay_init(void) {
    // 初始化继电器
    for (int i = 0; i < 6; i++) {
        _relays[i] = relay_create(_relays_gpio[i], GPIO_LOW);
    }
}
