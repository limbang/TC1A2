//
// Created by limbang-pc on 2019/12/31.
//

#ifndef SRC_COMPONENTS_DEVICE_INCLUDE_HASS_MQTT_DISCOVERY_H
#define SRC_COMPONENTS_DEVICE_INCLUDE_HASS_MQTT_DISCOVERY_H

#include "hass_component.h"
#include "hass_device.h"

typedef void *hass_mqss_discovery_handle_t;

hass_mqss_discovery_handle_t USER_FUNC hass_mqtt_discovery_create(char *discovery_prefix, hass_component_handle_t handle);

void USER_FUNC hass_mqtt_discovery_delete(hass_mqss_discovery_handle_t *handle);

void USER_FUNC hass_component_config_topic_set(hass_mqss_discovery_handle_t mqtt_discovery_handle, hass_component_handle_t component_Handle);

char *USER_FUNC hass_component_config_json_get(hass_mqss_discovery_handle_t mqtt_discovery_handle, hass_component_handle_t component_Handle,
        hass_device_handle_t device_handle);

char *USER_FUNC hass_mqtt_discovery_availability_topic_get(hass_mqss_discovery_handle_t handle);

char *USER_FUNC hass_mqtt_discovery_command_topic_get(hass_mqss_discovery_handle_t handle);

#endif //SRC_COMPONENTS_DEVICE_INCLUDE_HASS_MQTT_DISCOVERY_H
