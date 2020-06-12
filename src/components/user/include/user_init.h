/*
 * userInit.h
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#ifndef SRC_COMPONENTS_USER_INCLUDE_USER_INIT_H_
#define SRC_COMPONENTS_USER_INCLUDE_USER_INIT_H_

#include "user_key.h"
#include "user_led.h"
#include "user_mqtt.h"
#include "user_power_meter.h"
#include "user_relay.h"

void USER_FUNC user_ip_set(char *ipAddress);

int USER_FUNC user_hass_mqtt_cmd_cb(char *argv[],int argc);

void USER_FUNC user_init(void);

#endif /* SRC_COMPONENTS_USER_INCLUDE_USER_INIT_H_ */
