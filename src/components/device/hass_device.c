//
// Created by limbang-pc on 2019/12/31.
//

#include "hass_device.h"

#include <string.h>


#define IDENTIFIERS_MAXIMUM_LENGTH 20
#define MANUFACTURER_MAXIMUM_LENGTH 20
#define NAME_MAXIMUM_LENGTH 20
#define MODEL_MAXIMUM_LENGTH 10
#define VERSION_MAXIMUM_LENGTH 15
#define IP_MAXIMUM_LENGTH 16

/**
 * hass mqtt device
 */
typedef struct {
    char identifiers[IDENTIFIERS_MAXIMUM_LENGTH];
    char manufacturer[MANUFACTURER_MAXIMUM_LENGTH];
    char name[NAME_MAXIMUM_LENGTH];
    char model[MODEL_MAXIMUM_LENGTH];
    char version[VERSION_MAXIMUM_LENGTH];
    char ip[IP_MAXIMUM_LENGTH];
} hass_device_t;

typedef enum {
    HASS_DEVICE_IDENTIFIERS = 0,
    HASS_DEVICE_MANUFACTURER,
    HASS_DEVICE_NAME,
    HASS_DEVICE_MODEL,
    HASS_DEVICE_VERSION,
    HASS_DEVICE_IP
} hass_value_type_e;

/**
 * Create device
 *
 * @param identifiers
 * @param manufacturer
 * @param name
 * @param model
 * @param version
 * @param ip
 * @return
 */
hass_device_handle_t USER_FUNC hass_device_create(char *identifiers, char *manufacturer, char *name, char *model, char *version, char *ip) {
    hass_device_t *device = (hass_device_t *) hfmem_malloc(sizeof(hass_device_t));
    strcpy(device->identifiers, identifiers);
    strcpy(device->manufacturer, manufacturer);
    strcpy(device->name, name);
    strcpy(device->model, model);
    strcpy(device->version, version);
    strcpy(device->ip, ip);
    return (hass_device_handle_t) device;
}

/**
 * Delete device
 *
 * @param handle
 */
void USER_FUNC hass_device_delete(hass_device_handle_t *handle) {
    if (*handle != NULL) {
        hfmem_free(*handle);
        *handle = NULL;
    }
}

/**
 * get value
 *
 * @param handle
 * @param type
 * @return
 */
static char *USER_FUNC hass_device_value_get(hass_device_handle_t handle, hass_value_type_e type) {
    hass_device_t *device = (hass_device_t *) handle;
    switch (type) {
    case HASS_DEVICE_IDENTIFIERS:
        return device->identifiers;
    case HASS_DEVICE_MANUFACTURER:
        return device->manufacturer;
    case HASS_DEVICE_NAME:
        return device->name;
    case HASS_DEVICE_MODEL:
        return device->model;
    case HASS_DEVICE_VERSION:
        return device->version;
    case HASS_DEVICE_IP:
        return device->ip;
    default:
        return NULL;
    }
}

char *USER_FUNC hass_device_identifiers_get(hass_device_handle_t handle) {
    return hass_device_value_get(handle, HASS_DEVICE_IDENTIFIERS);
}

char *USER_FUNC hass_device_manufacturer_get(hass_device_handle_t handle) {
    return hass_device_value_get(handle, HASS_DEVICE_MANUFACTURER);
}

char *USER_FUNC hass_device_name_get(hass_device_handle_t handle) {
    return hass_device_value_get(handle, HASS_DEVICE_NAME);
}

char *USER_FUNC hass_device_model_get(hass_device_handle_t handle) {
    return hass_device_value_get(handle, HASS_DEVICE_MODEL);
}

char *USER_FUNC hass_device_version_get(hass_device_handle_t handle) {
    return hass_device_value_get(handle, HASS_DEVICE_VERSION);
}

char *USER_FUNC hass_device_ip(hass_device_handle_t handle) {
    return hass_device_value_get(handle, HASS_DEVICE_IP);
}
