/*
 * relay.c
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#include "relay.h"

typedef struct {
    gpio_number_e gpio_num;
    gpio_level_e trigger_level;
    relay_status_e status;
} relay_device_t;

/**
 * create relay
 *
 * @param gpio_num
 * @param trigger_level
 * @return
 */
relay_handle_t USER_FUNC relay_create(gpio_number_e gpioNum, gpio_level_e triggerLevel) {
    relay_device_t *relay = (relay_device_t *) hfmem_malloc(sizeof(relay_device_t));
    if (relay == NULL)
        return NULL;
    relay->gpio_num = gpioNum;
    relay->trigger_level = triggerLevel;
    relay->status = RELAY_STATUS_CLOSE;
    return (relay_handle_t) relay;
}

/**
 * delete relay
 *
 * @param handle
 */
void USER_FUNC relay_delete(relay_handle_t handle) {
    hfmem_free(handle);
}

/**
 * set states relay
 *
 * @param handle
 * @param status
 * @return
 */
bool USER_FUNC relay_states_set(relay_handle_t handle, relay_status_e status) {
    relay_device_t *relay = (relay_device_t *) handle;
    if (gpio_output_set(relay->gpio_num, (0x01 & status) ^ (!relay->trigger_level))) {
        relay->status = status;
        return true;
    }
    return false;
}

/**
 * get states relay
 *
 * @param handle
 * @return
 */
relay_status_e USER_FUNC relay_states_get(relay_handle_t handle) {
    relay_device_t *relay = (relay_device_t *) handle;
    return relay->status;
}

