//
// Created by limbang-pc on 2019/12/31.
//

#ifndef SRC_COMPONENTS_USER_INCLUDE_USER_RELAY_H_
#define SRC_COMPONENTS_USER_INCLUDE_USER_RELAY_H_

#include "relay.h"
#include "user_led.h"
#include "user_mqtt.h"

void USER_FUNC user_relay_init(void);

void USER_FUNC user_relay_led_state(void);

void USER_FUNC user_relay_aLL_set(relay_status_e status);

relay_status_e USER_FUNC user_relay_states_get(uint32_t id);

relay_status_e USER_FUNC user_relay_states_set(uint32_t id,relay_status_e status);

#endif //SRC_COMPONENTS_USER_INCLUDE_USER_RELAY_H_
