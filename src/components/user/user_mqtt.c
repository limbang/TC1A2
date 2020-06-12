//
// Created by limbang-pc on 2019/12/31.
//

#include "include/user_mqtt.h"

#include "hass_component.h"
#include "hass_device.h"
#include "hass_mqtt_discovery.h"
#include "include/user_power_meter.h"
#include "user_led.h"
#include "user_relay.h"


#define MANUFACTURER "LIMBANG"
#define PRODUCT_NAME "TC1\u667a\u80fd\u63d2\u677f"
#define HARDWARE_MODEL "TC1_A2"
#define FIRMWARE_VERSION "v0.2.12.31"

hass_mqss_discovery_handle_t _mqtt_discovery_handle;
hass_device_handle_t _device_handle;
hass_component_handle_t _component_handle[10];
mqtt_client_handle_t _mqtt_clien;

bool relay_update_flg = false;
uint32_t _component_num = 7;

void USER_FUNC component_num_set(uint32_t component_num){
    _component_num = component_num;
}

/**
 * 设置继电器状态更新旗帜,如果更新了就会发送到服务器
 */
void USER_FUNC mqtt_relay_update_flag(bool update_flg) {
    relay_update_flg = update_flg;
}

/**
 * MQTT 配置发送
 *
 * @param id id
 * @return
 */
bool USER_FUNC mqtt_hass_config_publish(int id) {
    if (mqtt_message_publish(_mqtt_clien, hass_component_config_topic_get(_component_handle[id]),
            hass_component_config_json_get(_mqtt_discovery_handle, _component_handle[id], _device_handle), QOS0) == HF_SUCCESS)
        return true;
    else
        return false;
}

/**
 * MQTT 状态发送
 *
 * @param id id
 * @param message  消息
 * @return
 */
bool USER_FUNC mqtt_state_publish(int id, const char *message) {
    if (mqtt_message_publish(_mqtt_clien, hass_component_state_topic_get(_component_handle[id]), message, QOS0) == HF_SUCCESS)
        return true;
    else
        return false;
}

/**
 * MQTT控制主题收到消息回调
 */
void USER_FUNC mqtt_cmd_topic_Msg_cb(MessageData* meg) {
    MQTTMessage* message = meg->message;
    u_printf("************ Receive CMD Message ***********\r\n");
    u_printf("%.*s\r\n", meg->topicName->lenstring.len, meg->topicName->lenstring.data);
    u_printf("%.*s\r\n", (int )message->payloadlen, (char* )message->payload);

    // 从主题提取id，并转成int
    char *tem = strchr(meg->topicName->lenstring.data,'/' );
    tem = strchr(tem+1,'/');
    char id_str[4] ;
    strncpy(id_str,tem+1,strlen(tem)-5);
    int id = atoi(id_str);

    // 提取状态
    char state[4] = { 0 };
    strncpy(state, (char*) message->payload, (int) message->payloadlen);

    // 根据id控制设备
    if(id < 6){
        if (strcmp(state, "ON") == 0){
            user_relay_states_set(id, RELAY_STATUS_OPEN);
            if(user_led_states_get() == LED_STATUS_OFF)
                user_led_on();
        }else if (strcmp(state, "OFF") == 0){
            user_relay_states_set(id, RELAY_STATUS_CLOSE);
            user_relay_led_state();
        }
        mqtt_state_publish(id,state);
    }

    // 设置 bl0937 cf1 的模式
    if(id == 8){
        if (strcmp(state, "ON") == 0){
            //userPowerSelSet(GPIO_HIGH);
        }else if (strcmp(state, "OFF") == 0){
            //userPowerSelSet(GPIO_LOW);
        }
        mqtt_state_publish(id,state);
    }

}

/**
 * MQTT回调处理函数
 */
void USER_FUNC mqtt_connect_status_cb_handler(mqtt_cb_type_e cb_type) {
    switch (cb_type) {
    case MQTT_CONNECT_SUCCESS:
        if (mqtt_subscribe_topic(_mqtt_clien, hass_mqtt_discovery_command_topic_get(_mqtt_discovery_handle), QOS0, &mqtt_cmd_topic_Msg_cb)) {
            u_printf("[MQTT]:Subscribe[%s]Topic success.\r\n", hass_mqtt_discovery_command_topic_get(_mqtt_discovery_handle));
            // 发送home assistant 发现配置
            for (int i = 0; i < _component_num; i++) {
                if (!mqtt_hass_config_publish(i))
                    u_printf("[MQTT]:id %d publish discovery config fail.\r\n", i);
            }
            // 发送设备上线
            if (mqtt_message_publish(_mqtt_clien, hass_mqtt_discovery_availability_topic_get(_mqtt_discovery_handle), "online", QOS0) != HF_SUCCESS)
                u_printf("[MQTT]:publish TC1_A2 online fail!\r\n");
        } else
            u_printf("[MQTT]:SubscribeCmdTopic fail!\r\n");
        break;
    case MQTT_DISCONNECT:
        break;
    case MQTT_WIFI_CONNECT_SUCCESS:
        break;
    case MQTT_CONNECT_NET_SUCCESS:
        break;
    case MQTT_EARTBEAT:
        // 继电器状态更新
        if (relay_update_flg) {
            char* hassRelayState[2] = { "OFF", "ON" };
            for (int i = 0; i < 6; i++) {
                mqtt_state_publish(i, hassRelayState[user_relay_states_get(i)]);
            }
            relay_update_flg = false;
        }
        break;
    }
}

//void USER_FUNC power_timer_handler(hftimer_handle_t htimer) {
//    char message[20];
//    sprintf(message, "{\"power\":%u}", getActivePower());
//    mqtt_state_publish(6,message);
//}

void USER_FUNC user_mqtt_init(char *mac, char *ip,char *address, unsigned short port, char *username, char *password,char *discovery_prefix) {
    // 创建设备
    _device_handle = hass_device_create(mac, MANUFACTURER, PRODUCT_NAME,
    HARDWARE_MODEL, FIRMWARE_VERSION, ip);
    // 创建6个开关组件,控制继电器
    char name[15];
    for (int i = 0; i < 6; i++) {
        sprintf(name, "\u63d2\u5ea7%d", i + 1);
        _component_handle[i] = hass_component_create(i, mac, name, SWITCH, "mdi:power-socket-uk", NULL, NULL, NULL);
    }
    // 创建功率传感器组件
    _component_handle[6] = hass_component_create(6, mac, "\u5f53\u524d\u529f\u7387", SENSOR, "mdi:gauge", "W", "{{ value_json.power }}", "Power");
    if(_component_num >= 9){
        // 创建电流传感器组件
        _component_handle[7] = hass_component_create(7, mac, "\u5f53\u524d\u7535\u6d41", SENSOR, "mdi:gauge", "A", "{{ value_json.current }}", "Current");
        // 创建电压传感器组件
        _component_handle[8] = hass_component_create(8, mac, "\u5f53\u524d\u7535\u538b", SENSOR, "mdi:gauge", "V", "{{ value_json.voltage }}", "Voltage");
        if(_component_num == 10){
            // 创建电流电压切换开关组件
            _component_handle[9] = hass_component_create(9, mac, "\u7535\u6d41/\u7535\u538b", SWITCH, "mdi:settings-outline", NULL, NULL,NULL);
        }
    }
    // 创建 hass mqtt 发现
    _mqtt_discovery_handle = hass_mqtt_discovery_create(discovery_prefix, _component_handle[0]);

    // 设置组件的配置主题
    for (int i = 0; i < _component_num; i++) {
        hass_component_config_topic_set(_mqtt_discovery_handle, _component_handle[i]);
    }
    // 创建mqtt客户端
    _mqtt_clien = mqtt_create(address,port,mac,username,password,&mqtt_connect_status_cb_handler,hass_mqtt_discovery_availability_topic_get(_mqtt_discovery_handle));

//    hftimer_handle_t power_Timer = NULL;
//    if ((power_Timer = hftimer_create("powerTimer", 5000, TRUE, 0 ,power_timer_handler, 0)) == NULL)
//            u_printf("[DeBug]:Create power Timer fail\r\n");
//    if (hftimer_start(power_Timer) != HF_SUCCESS)
//            u_printf("[DeBug]:start power Timer fail\r\n");
}
