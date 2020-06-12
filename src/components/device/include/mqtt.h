/*
 * mqtt.h
 *
 *  Created on: 2019年12月27日
 *      Author: limbang-pc
 */

#ifndef SRC_COMPONENTS_DEVICE_INCLUDE_MQTT_H_
#define SRC_COMPONENTS_DEVICE_INCLUDE_MQTT_H_

#include "MQTTClient.h"
#include "MQTTHF_LPx30.h"

/* 服务器地址最大长度 */
#define SERVER_ADDRESS_MAXIMUM_LENGTH 100
/* 客户端ID最大长度 */
#define CLIENT_ID_MAXIMUM_LENGTH 100
/* 用户名最大长度 */
#define USERNAME_MAXIMUM_LENGTH 100
/* 密码最大长度 */
#define PASSWORD_MAXIMUM_LENGTH 100

#define DISCOVERY_PREFIX_MAXIMUM_LENGTH 100

typedef enum {
    MQTT_CONNECT_SUCCESS = 0, /* MQTT连接成功 */
    MQTT_DISCONNECT, /* MQTT断开连接 */
    MQTT_WIFI_CONNECT_SUCCESS, /* MQTT WIFI连接成功 */
    MQTT_CONNECT_NET_SUCCESS, /* MQTT 网络连接成功 */
    MQTT_EARTBEAT /* MQTT 心跳包 */
} mqtt_cb_type_e;

typedef void (*mqtt_connect_status_cb)(mqtt_cb_type_e);

typedef void *mqtt_client_handle_t;

mqtt_client_handle_t USER_FUNC mqtt_create(char *address, unsigned short port, char *client_id, char *username, char *password,
        mqtt_connect_status_cb mqttCallback, char *availabilityTopic);

bool USER_FUNC mqtt_subscribe_topic(mqtt_client_handle_t handle, char *topic, enum QoS qos, messageHandler messageeCallback);

void USER_FUNC mqtt_delete(mqtt_client_handle_t handle);

int USER_FUNC mqtt_message_publish(mqtt_client_handle_t handle, char *topic, const char *data, enum QoS qos);

#endif /* SRC_COMPONENTS_DEVICE_INCLUDE_MQTT_H_ */
