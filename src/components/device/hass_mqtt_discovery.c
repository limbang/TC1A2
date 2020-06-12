//
// Created by limbang-pc on 2019/12/31.
//

#include "hass_mqtt_discovery.h"

#include <string.h>

#include "cJSON.h"

#define DISCOVERY_PREFIX_MAXIMUM_LENGTH 100
#define AVAILABILITY_TOPIC_MAXIMUM_LENGTH 100
#define COMMAND_TOPIC_MAXIMUM_LENGTH 100

typedef struct {
    char discovery_prefix[DISCOVERY_PREFIX_MAXIMUM_LENGTH];
    char availability_topic[AVAILABILITY_TOPIC_MAXIMUM_LENGTH];
    char command_topic[COMMAND_TOPIC_MAXIMUM_LENGTH];
} hass_mqtt_discovery_t;

typedef enum {
    HASS_MQTT_DISCOVERY_AVAILABILITY_TOPIC,
    HASS_MQTT_DISCOVERY_COMMAND_TOPIC
} hass_value_type_e;

/**
 * Create hass mqtt
 *
 * @param discovery_prefix
 * @param handle
 * @return
 */
hass_mqss_discovery_handle_t USER_FUNC hass_mqtt_discovery_create(char *discovery_prefix, hass_component_handle_t handle) {
    hass_mqtt_discovery_t *mqtt_discovery = (hass_mqtt_discovery_t *) hfmem_malloc(sizeof(hass_mqtt_discovery_t));
    strcpy(mqtt_discovery->discovery_prefix, discovery_prefix);
    sprintf(mqtt_discovery->availability_topic, "%s/%s/available", discovery_prefix,
            hass_component_identifiers_get(handle));
    sprintf(mqtt_discovery->command_topic, "%s/%s/+/set", discovery_prefix, hass_component_identifiers_get(handle));
    return (hass_mqss_discovery_handle_t) mqtt_discovery;
}

/**
 * Delete hass mqtt
 *
 * @param handle mqtt Discovery Handle
 */
void USER_FUNC hass_mqtt_discovery_delete(hass_mqss_discovery_handle_t *handle) {
    if (*handle != NULL) {
        hfmem_free(*handle);
        *handle = NULL;
    }
}

/**
 * component config topic set
 *
 * @param mqtt_discovery_handle
 * @param component_handle
 */
void USER_FUNC hass_component_config_topic_set(hass_mqss_discovery_handle_t mqtt_discovery_handle, hass_component_handle_t component_handle) {
    hass_mqtt_discovery_t *mqtt_discovery = (hass_mqtt_discovery_t *) mqtt_discovery_handle;

    char *component_type_name = NULL;
    switch (hass_component_type_get(component_handle)) {
    case SWITCH:
        component_type_name = "switch";
        break;
    case SENSOR:
        component_type_name = "sensor";
        break;
    }

    sprintf(hass_component_config_topic_get(component_handle), "%s/%s/%s/%lu/config", mqtt_discovery->discovery_prefix, component_type_name,
            hass_component_identifiers_get(component_handle), hass_component_id_get(component_handle));

}

/**
 * component discovery config json get
 *
 * @param mqttDiscoveryHandle
 * @param componentHandle
 * @param deviceHandle
 * @return
 */
char *USER_FUNC hass_component_config_json_get(hass_mqss_discovery_handle_t mqtt_discovery_handle, hass_component_handle_t component_handle,
        hass_device_handle_t device_handle) {
    hass_mqtt_discovery_t *mqtt_discovery = (hass_mqtt_discovery_t *) mqtt_discovery_handle;

    char commandTopic[COMMAND_TOPIC_MAXIMUM_LENGTH];

    //	创建HA发现Json对象
    cJSON *root = cJSON_CreateObject();

    sprintf(hass_component_state_topic_get(component_handle), "%s/%s/%lu/state", mqtt_discovery->discovery_prefix, hass_component_identifiers_get(component_handle),
            hass_component_id_get(component_handle));

    cJSON_AddStringToObject(root, "name", hass_component_name_get(component_handle));
    cJSON_AddStringToObject(root, "icon", hass_component_icon_get(component_handle));
    cJSON_AddStringToObject(root, "stat_t", hass_component_state_topic_get(component_handle));
    cJSON_AddStringToObject(root, "avty_t", mqtt_discovery->availability_topic);

    // 设备上下线有效值
    cJSON_AddStringToObject(root, "pl_avail", "online");
    cJSON_AddStringToObject(root, "pl_not_avail", "offline");

    // 根据组件类型设置
    switch (hass_component_type_get(component_handle)) {
    case SWITCH:
        sprintf(commandTopic, "%s/%s/%lu/set", mqtt_discovery->discovery_prefix, hass_component_identifiers_get(component_handle),
                hass_component_id_get(component_handle));
        cJSON_AddStringToObject(root, "pl_on", "ON");
        cJSON_AddStringToObject(root, "pl_off", "OFF");
        cJSON_AddStringToObject(root, "cmd_t", commandTopic);
        break;
    case SENSOR:
        cJSON_AddStringToObject(root, "dev_cla", hass_component_device_class_get(component_handle));
        cJSON_AddStringToObject(root, "unit_of_meas", hass_component_unit_of_measurement_get(component_handle));
        cJSON_AddStringToObject(root, "val_tpl", hass_component_value_template_get(component_handle));
        break;
    }

    if (device_handle != NULL) {
        //	创建设备Json对象
        cJSON *jsonDevice = cJSON_CreateObject();
        cJSON_AddStringToObject(jsonDevice, "ids", hass_device_identifiers_get(device_handle));
        cJSON_AddStringToObject(jsonDevice, "mf", hass_device_manufacturer_get(device_handle));
        cJSON_AddStringToObject(jsonDevice, "name", hass_device_name_get(device_handle));
        cJSON_AddStringToObject(jsonDevice, "mdl", hass_device_model_get(device_handle));
        cJSON_AddStringToObject(jsonDevice, "sw", hass_device_version_get(device_handle));
        // connections 连接信息
        cJSON *jsonDeviceConnections = cJSON_CreateArray();
        cJSON *jsonDeviceConnectionsMac = cJSON_CreateArray();
        cJSON_AddItemToArray(jsonDeviceConnectionsMac, cJSON_CreateString("mac"));
        cJSON_AddItemToArray(jsonDeviceConnectionsMac, cJSON_CreateString(hass_device_identifiers_get(device_handle)));
        cJSON_AddItemToArray(jsonDeviceConnections, jsonDeviceConnectionsMac);
        cJSON *jsonDeviceConnectionsIp = cJSON_CreateArray();
        cJSON_AddItemToArray(jsonDeviceConnectionsIp, cJSON_CreateString("ip"));
        cJSON_AddItemToArray(jsonDeviceConnectionsIp, cJSON_CreateString(hass_device_ip(device_handle)));
        cJSON_AddItemToArray(jsonDeviceConnections, jsonDeviceConnectionsIp);
        // 添加连接信息到设备
        cJSON_AddItemToObject(jsonDevice, "cns", jsonDeviceConnections);
        // 添加设备信息到HA发现JSON
        cJSON_AddItemToObject(root, "dev", jsonDevice);
    }
    // 获取格式化的JSON字符串
    char *res = cJSON_PrintUnformatted(root);
    // 释放JSON内存
    cJSON_Delete(root);
    return res;
}

/**
 * get value
 *
 * @param mqttDiscoveryHandle
 * @param type
 * @return
 */
static char *USER_FUNC hass_mqss_discovery_value_get(hass_mqss_discovery_handle_t handle, hass_value_type_e type) {
    hass_mqtt_discovery_t *mqtt_discovery = (hass_mqtt_discovery_t *) handle;
    switch (type) {
    case HASS_MQTT_DISCOVERY_AVAILABILITY_TOPIC:
        return mqtt_discovery->availability_topic;
    case HASS_MQTT_DISCOVERY_COMMAND_TOPIC:
        return mqtt_discovery->command_topic;
    default:
        return NULL;
    }
}

char *USER_FUNC hass_mqtt_discovery_availability_topic_get(hass_mqss_discovery_handle_t handle) {
    return hass_mqss_discovery_value_get(handle, HASS_MQTT_DISCOVERY_AVAILABILITY_TOPIC);
}

char *USER_FUNC hass_mqtt_discovery_command_topic_get(hass_mqss_discovery_handle_t handle) {
    return hass_mqss_discovery_value_get(handle, HASS_MQTT_DISCOVERY_COMMAND_TOPIC);
}
