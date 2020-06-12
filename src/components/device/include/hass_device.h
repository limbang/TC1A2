//
// Created by limbang-pc on 2019/12/31.
//

#ifndef SRC_COMPONENTS_DEVICE_INCLUDE_HASS_DEVICE_H_
#define SRC_COMPONENTS_DEVICE_INCLUDE_HASS_DEVICE_H_

#include <hsf.h>

typedef void *hass_device_handle_t;

hass_device_handle_t USER_FUNC hass_device_create(char *identifiers, char *manufacturer, char *name, char *model, char *version, char *ip);

void USER_FUNC hass_device_delete(hass_device_handle_t *handle);

char *USER_FUNC hass_device_identifiers_get(hass_device_handle_t handle);

char *USER_FUNC hass_device_manufacturer_get(hass_device_handle_t handle);

char *USER_FUNC hass_device_name_get(hass_device_handle_t handle);

char *USER_FUNC hass_device_model_get(hass_device_handle_t handle);

char *USER_FUNC hass_device_version_get(hass_device_handle_t handle);

char *USER_FUNC hass_device_ip(hass_device_handle_t handle);

#endif //SRC_COMPONENTS_DEVICE_INCLUDE_HASS_DEVICE_H_
