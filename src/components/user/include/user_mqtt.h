//
// Created by limbang-pc on 2019/12/31.
//

#ifndef SRC_COMPONENTS_USER_INCLUDE_USER_MQTT_H_
#define SRC_COMPONENTS_USER_INCLUDE_USER_MQTT_H_

#include "mqtt.h"

void USER_FUNC component_num_set(uint32_t component_num);

void USER_FUNC mqtt_relay_update_flag(bool updateFlag);

void USER_FUNC user_mqtt_init(char *mac, char *ip,char *address, unsigned short port, char *username, char *password,char *discovery_prefix);

#endif //SRC_COMPONENTS_USER_INCLUDE_USER_MQTT_H_
