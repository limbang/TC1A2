/*
 * userInit.c
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#include "user_init.h"

/* MAC地址最大长度 */
#define MAC_ADDRESS_MAXIMUM_LENGTH 21
/* IP地址最大长度 */
#define IP_ADDRESS_MAXIMUM_LENGTH 11

/* MAC地址 */
char mac[MAC_ADDRESS_MAXIMUM_LENGTH];
/* IP地址 */
char ip[IP_ADDRESS_MAXIMUM_LENGTH];

typedef struct {
    char address[SERVER_ADDRESS_MAXIMUM_LENGTH];
    unsigned short port;
    char username[USERNAME_MAXIMUM_LENGTH];
    char password[PASSWORD_MAXIMUM_LENGTH];
    pm_run_mode_e mode;
    char discovery_prefix[DISCOVERY_PREFIX_MAXIMUM_LENGTH];
} hass_config_t;

hass_config_t *hass_config;

/**
 * 获取到IP的时候调用,传ip地址过来
 */
void USER_FUNC user_ip_set(char *ipAddress) {
    strcpy(ip, ipAddress);
}

/**
 * hass mqtt 服务器连接设置 at cmd 回调函数
 */
int USER_FUNC user_hass_mqtt_cmd_cb(char *argv[], int argc) {
    if (argc < 4 || argc > 6)
        return -1;

//    for (int i = 0; i < argc; i++) {
//        u_printf("[hass mqtt cmd]:%d=%s\r\n", i, argv[i]);
//    }

    strcpy(hass_config->address, argv[0]);
    hass_config->port = atoi(argv[1]);
    strcpy(hass_config->username, argv[2]);
    strcpy(hass_config->password, argv[3]);

    if(argc >= 5)
        hass_config->mode = atoi(argv[4]);
    else
        hass_config->mode = PM_RUN_MODE_1;

    if(argc == 6)
        strcpy(hass_config->discovery_prefix, argv[5]);
    else
        strcpy(hass_config->discovery_prefix, "homeassistant");

    // 擦除 flash 页
    if (hfuflash_erase_page(0, 1) != HF_SUCCESS)
        u_printf("[hass mqtt cmd]:flash erase page fail.\r\n");

    // 写配置标志 标志等于 1 代表有配置
    uint8_t flg = 1;
    if (hfuflash_write(0, (char*) &flg, sizeof(uint8_t)) < 0)
        u_printf("[hass mqtt cmd]:flash write fail.\r\n");

    // 写配置到 flash
    uint8_t *p = (uint8_t *) (hass_config);
    for (int i = 1; i < (sizeof(hass_config_t) + 1); i++) {
        if (hfuflash_write(i, (char*) (p + i - 1), sizeof(uint8_t)) < 0) {
            u_printf("[hass mqtt cmd]:flash write fail.\r\n");
            break;
        }
    }

    return 0;
}

/**
 * 用户 flash 读取配置
 */
static bool USER_FUNC user_read_configure(void) {
    //读 flash 标志 标志等于 1 代表有配置
    uint8_t flg;
    if (hfuflash_read(0, (char*) &flg, sizeof(uint8_t)) > 0) {
        if (flg == 1) {
            // 读配置
            uint8_t *p = (uint8_t *) (hass_config);
            for (int i = 1; i < (sizeof(hass_config_t) + 1); i++) {
                if (hfuflash_read(i, (char*) (p + i - 1), sizeof(uint8_t)) < 0) {
                    u_printf("[hass mqtt cmd]:flash read fail.\r\n");
                    break;
                }
            }
            return true;
        }
    }
    return false;
}

/**
 * 初始化硬件
 */
void USER_FUNC user_init(void) {
    // 获取MAC地址
    if (hfnet_get_mac_address(mac) != HF_SUCCESS)
        u_printf("[init]:MAC:address get fail.\r\n");

    hass_config = (hass_config_t *) hfmem_malloc(sizeof(hass_config_t));

    user_key_init();
    user_led_init();
    user_relay_init();
    if (user_read_configure()) {
        user_power_meter_init(hass_config->mode);
        user_mqtt_init(mac, ip, hass_config->address, hass_config->port, hass_config->username, hass_config->password, hass_config->discovery_prefix);
    } else {
        u_printf(
                "[init]:Hass mqtt configuration is not initialized, enter the command \"AT+HASSMQTT=address,port,username,password,powermeterMode,discoveryPrefix\"configuration\r\n");
    }

    hfmem_free(hass_config);
}
