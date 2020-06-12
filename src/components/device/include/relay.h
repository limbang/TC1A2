/*
 * relay.h
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#ifndef SRC_COMPONENTS_USER_INCLUDE_RELAY_H_
#define SRC_COMPONENTS_USER_INCLUDE_RELAY_H_

#include "gpio.h"

typedef void *relay_handle_t;

typedef enum {
    RELAY_STATUS_CLOSE, /* relay close */
    RELAY_STATUS_OPEN   /* relay open */
} relay_status_e;

relay_handle_t USER_FUNC relay_create(gpio_number_e gpio_num, gpio_level_e trigger_level);

void USER_FUNC relay_delete(relay_handle_t handle);

bool USER_FUNC relay_states_set(relay_handle_t handle, relay_status_e status);

relay_status_e USER_FUNC relay_states_get(relay_handle_t handle);

#endif /* SRC_COMPONENTS_USER_INCLUDE_RELAY_H_ */
