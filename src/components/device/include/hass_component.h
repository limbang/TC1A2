//
// Created by limbang-pc on 2019/12/31.
//

#ifndef SRC_COMPONENTS_DEVICE_INCLUDE_HASS_COMPONENT_H_
#define SRC_COMPONENTS_DEVICE_INCLUDE_HASS_COMPONENT_H_

#include <hsf.h>

typedef void *hass_component_handle_t;

typedef enum {
    SWITCH, SENSOR
} hass_component_type_e;

hass_component_handle_t USER_FUNC hass_component_create(uint32_t id, char *identifiers, char *name, hass_component_type_e type, char *icon,
        char *unitOfMeasurement, char *valueTemplate, char *deviceClass);

void USER_FUNC hass_component_delete(hass_component_handle_t *componentHandle);

uint32_t USER_FUNC hass_component_id_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_identifiers_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_name_get(hass_component_handle_t componentHandle);

hass_component_type_e USER_FUNC hass_component_type_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_icon_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_unit_of_measurement_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_value_template_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_device_class_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_state_topic_get(hass_component_handle_t componentHandle);

char *USER_FUNC hass_component_config_topic_get(hass_component_handle_t componentHandle);

#endif //SRC_COMPONENTS_DEVICE_INCLUDE_HASS_COMPONENT_H_
