/*
 * mqtt.c
 *
 *  Created on: 2019年12月27日
 *      Author: limbang-pc
 */

#include "mqtt.h"

/* MQTT客户端缓存长度 */
#define MQTT_CLIENT_BUF_LENGTH 1024
/* MQTT客户端读取缓存长度 */
#define MQTT_CLIENT_READ_BUF_LENGTH 1024
/* MQTT客户端指令超时时间(毫秒) */
#define MQTT_CLIENT_COMMAND_IMEOUT_MS 1000
/* MQTT客户端心跳时间(秒) */
#define MQTT_CLIENT_KEEP_ALIVE_INTERVAL_S 60

/* 可用主题最大长度 */
#define AVAILABILITY_TOPIC_MAXIMUM_LENGTH 100

typedef struct {
    char server_address[SERVER_ADDRESS_MAXIMUM_LENGTH];
    unsigned short port;
    char client_id[CLIENT_ID_MAXIMUM_LENGTH];
    char username[USERNAME_MAXIMUM_LENGTH];
    char password[PASSWORD_MAXIMUM_LENGTH];
} mqtt_config_t;

typedef struct {
    mqtt_config_t *config;
    Client mqtt_client;
    mqtt_connect_status_cb mqtt_cb_func;
    char availability_topic[AVAILABILITY_TOPIC_MAXIMUM_LENGTH];
    hfthread_hande_t *mqtt_thread;
} mqtt_device_t;

/**
 * mqtt topic subscribe
 *
 * @param handle
 * @param topic
 * @param qos
 * @param msg_cb messagee Callback
 * @return
 */
bool USER_FUNC mqtt_subscribe_topic(mqtt_client_handle_t handle, char *topic, enum QoS qos, messageHandler msg_cb) {
    mqtt_device_t *mqtt_device = (mqtt_device_t *) handle;
    if (MQTTSubscribe(&mqtt_device->mqtt_client, topic, qos, msg_cb) == SUCCESS)
        return true;
    else
        return false;
}

/**
 * mqtt message publish
 *
 * @param topic
 * @param data
 * @param qos
 * @return
 */
int USER_FUNC mqtt_message_publish(mqtt_client_handle_t handle, char *topic, const char *data, enum QoS qos) {
    mqtt_device_t *mqtt_device = (mqtt_device_t *) handle;

    MQTTMessage message;
    message.qos = qos;
    message.retained = FALSE;
    message.dup = FALSE;

    message.payload = (void*) data;
    message.payloadlen = strlen(data);

    return MQTTPublish(&mqtt_device->mqtt_client, topic, &message);
}

/**
 * mqtt thread
 */
void USER_FUNC mqtt_client_thread(void *arg) {

    mqtt_device_t *mqtt_device = (mqtt_device_t *) arg;

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

    Network client_network;
    NewNetwork(&client_network);

    char *mqtt_client_buf;
    mqtt_client_buf = (char *) hfmem_malloc(MQTT_CLIENT_BUF_LENGTH);
    char *mqtt_client_read_buf;
    mqtt_client_read_buf = (char *) hfmem_malloc(MQTT_CLIENT_READ_BUF_LENGTH);

    while (true) {

        while (!hfwifi_sta_is_connected()) {
            msleep(1000);
        }
        mqtt_device->mqtt_cb_func(MQTT_WIFI_CONNECT_SUCCESS);

        if (ConnectNetwork(&client_network, mqtt_device->config->server_address, mqtt_device->config->port) != SUCCESS) {
            u_printf("[MQTT]:TCPSocket connect fail!\r\n");
            goto MQTT_END;
        }
        mqtt_device->mqtt_cb_func(MQTT_CONNECT_NET_SUCCESS);

        MQTTClient(&mqtt_device->mqtt_client, &client_network, MQTT_CLIENT_COMMAND_IMEOUT_MS, (unsigned char *) mqtt_client_buf,
        MQTT_CLIENT_BUF_LENGTH, (unsigned char *) mqtt_client_read_buf, MQTT_CLIENT_READ_BUF_LENGTH);

        connect_data.willFlag = 1;
        connect_data.will.topicName.cstring = mqtt_device->availability_topic;
        connect_data.will.message.cstring = "offline";
        connect_data.MQTTVersion = 4;
        connect_data.clientID.cstring = mqtt_device->config->client_id;
        connect_data.username.cstring = mqtt_device->config->username;
        connect_data.password.cstring = mqtt_device->config->password;
        connect_data.keepAliveInterval = MQTT_CLIENT_KEEP_ALIVE_INTERVAL_S;
        connect_data.cleansession = 1;

        if (MQTTConnect(&mqtt_device->mqtt_client, &connect_data) != SUCCESS) {
            u_printf("[MQTT]:MQTT Connect fail! \r\n");
            goto MQTT_END;
        }
        mqtt_device->mqtt_cb_func(MQTT_CONNECT_SUCCESS);

        int ret;
        int ping_time = 0;

        ping_time = hfsys_get_time();
        while (true) {

            mqtt_device->mqtt_cb_func(MQTT_EARTBEAT);

            ret = MQTTYield(&mqtt_device->mqtt_client, 300);
            if (ret == FAILURE) {
                goto MQTT_END;
            }

            if (!hfwifi_sta_is_connected()) {
                u_printf("[MQTT]:MQTTClient_thread WiFi disconnected!\r\n");
                goto MQTT_END;
            }

            if (hfsys_get_time() - ping_time > (connect_data.keepAliveInterval - 10) * 1000) {
                ping_time = hfsys_get_time();
                int len = MQTTSerialize_pingreq(mqtt_device->mqtt_client.buf, mqtt_device->mqtt_client.buf_size);
                if (len > 0) {
                    mqtt_device->mqtt_client.ipstack->mqttwrite(mqtt_device->mqtt_client.ipstack, mqtt_device->mqtt_client.buf, len, 100);
                }
            }
        }

        MQTT_END:
        u_printf("[MQTT]:MQTT end!\r\n\r\n");
        mqtt_device->mqtt_cb_func(MQTT_DISCONNECT);
        client_network.disconnect(&client_network);
        msleep(3000);
    }
    if (mqtt_client_buf != NULL)
        hfmem_free(mqtt_client_buf);
    if (mqtt_client_read_buf != NULL)
        hfmem_free(mqtt_client_read_buf);

    u_printf("[MQTT]:MQTTClient_thread destroy.\r\n");
    msleep(1000);
    hfthread_destroy(NULL);
}

/**
 * create mqtt
 *
 * @param address
 * @param port
 * @param client_id
 * @param username
 * @param password
 * @param mqtt_connect_status_cb
 * @param availabilityTopic
 * @return
 */
mqtt_client_handle_t USER_FUNC mqtt_create(char *address, unsigned short port, char *client_id, char *username, char *password,
        mqtt_connect_status_cb mqtt_connect_status_cb, char *availabilityTopic) {
    u_printf("[MQTT]:mqtt create. \r\n");
    mqtt_device_t * mqtt_device = (mqtt_device_t *) hfmem_malloc(sizeof(mqtt_device_t));
    if (mqtt_device == NULL)
        return NULL;

    mqtt_device->config = (mqtt_config_t *) hfmem_malloc(sizeof(mqtt_config_t));

    strcpy(mqtt_device->config->server_address, address);
    mqtt_device->config->port = port;
    strcpy(mqtt_device->config->client_id, client_id);
    strcpy(mqtt_device->config->username, username);
    strcpy(mqtt_device->config->password, password);

    mqtt_device->mqtt_cb_func = mqtt_connect_status_cb;

    strcpy(mqtt_device->availability_topic, availabilityTopic);

    if (hfthread_create(mqtt_client_thread, "MQTT Client Thread", 512, mqtt_device,
    HFTHREAD_PRIORITIES_LOW, mqtt_device->mqtt_thread, NULL) != HF_SUCCESS)
        u_printf("[MQTT]:start MQTT Client Thread fail\r\n");

    return (mqtt_client_handle_t) mqtt_device;
}

void USER_FUNC mqtt_delete(mqtt_client_handle_t handle) {
    if (handle == NULL)
        return;
    mqtt_device_t *mqtt_device = (mqtt_device_t *) handle;
    hfthread_destroy(mqtt_device->mqtt_thread);
    if (mqtt_device->config != NULL)
        hfmem_free(mqtt_device->config);
    hfmem_free(handle);
}
