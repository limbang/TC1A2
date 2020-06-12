//
// Created by limbang-pc on 2019/12/31.
//

#include "hass_component.h"

#include <string.h>


#define IDENTIFIERS_MAXIMUM_LENGTH 20
#define NAME_MAXIMUM_LENGTH 20
#define ICON_MAXIMUM_LENGTH 30
#define UNIT_OF_MEASUREMENT_MAXIMUM_LENGTH 4
#define VALUE_TEMPLATE_MAXIMUM_LENGTH 100
#define DEVICE_CLASS_MAXIMUM_LENGTH 50
#define CONFIG_TOPIC_MAXIMUM_LENGTH 100
#define STATE_TOPIC_MAXIMUM_LENGTH 100

typedef struct {
    uint32_t id;
    char identifiers[IDENTIFIERS_MAXIMUM_LENGTH];
    char name[NAME_MAXIMUM_LENGTH];
    hass_component_type_e type;
    char icon[ICON_MAXIMUM_LENGTH];
    char unit_of_measurement[UNIT_OF_MEASUREMENT_MAXIMUM_LENGTH];
    char value_template[VALUE_TEMPLATE_MAXIMUM_LENGTH];
    char device_class[DEVICE_CLASS_MAXIMUM_LENGTH];
    char state_topic[STATE_TOPIC_MAXIMUM_LENGTH];
    char config_topic[CONFIG_TOPIC_MAXIMUM_LENGTH];
} hass_component_t;

typedef enum {
    HASS_COMPONENT_IDENTIFIERS,
    HASS_COMPONENT_NAME,
    HASS_COMPONENT_ICON,
    HASS_COMPONENT_UNIT_OF_MEASUREMENT,
    HASS_COMPONENT_VALUE_TEMPLATE,
    HASS_COMPONENT_DEVICE_CLASS,
    HASS_COMPONENT_STATE_TOPIC,
    HASS_COMPONENT_CONFIG_TOPIC
} hass_value_type_e;

/**
 * Create component
 * @param id
 * @param identifiers
 * @param name
 * @param type
 * @param icon
 * @param unit_of_measurement   others do not pass in NULL
 * @param value_template        others do not pass in NULL
 * @param device_class          others do not pass in NULL
 * @return
 */
hass_component_handle_t USER_FUNC hass_component_create(uint32_t id, char *identifiers, char *name, hass_component_type_e type, char *icon,
        char *unit_of_measurement, char *value_template, char *device_class) {
    hass_component_t *component = (hass_component_t *) hfmem_malloc(sizeof(hass_component_t));
    component->id = id;
    strcpy(component->identifiers, identifiers);
    strcpy(component->name, name);
    component->type = type;
    strcpy(component->icon, icon);
    if (unit_of_measurement != NULL)
        strcpy(component->unit_of_measurement, unit_of_measurement);
    if (value_template != NULL)
        strcpy(component->value_template, value_template);
    if (device_class != NULL)
        strcpy(component->device_class, device_class);
    return (hass_component_handle_t) component;
}

/**
 * Delete component
 * @param handle componentHandle
 */
void USER_FUNC hass_component_delete(hass_component_handle_t *handle) {
    if (*handle != NULL) {
        hfmem_free(*handle);
        *handle = NULL;
    }
}

/**
 * get value
 * @param handle componentHandle
 * @param type
 * @return
 */
static char *USER_FUNC get_component_value(hass_component_handle_t handle, hass_value_type_e type) {
    hass_component_t *component = (hass_component_t *) handle;
    switch (type) {
    case HASS_COMPONENT_IDENTIFIERS:
        return component->identifiers;
    case HASS_COMPONENT_NAME:
        return component->name;
    case HASS_COMPONENT_ICON:
        return component->icon;
    case HASS_COMPONENT_UNIT_OF_MEASUREMENT:
        return component->unit_of_measurement;
    case HASS_COMPONENT_VALUE_TEMPLATE:
        return component->value_template;
    case HASS_COMPONENT_DEVICE_CLASS:
        return component->device_class;
    case HASS_COMPONENT_STATE_TOPIC:
        return component->state_topic;
    case HASS_COMPONENT_CONFIG_TOPIC:
        return component->config_topic;
    default:
        return NULL;
    }
}

uint32_t USER_FUNC hass_component_id_get(hass_component_handle_t handle) {
    hass_component_t *component = (hass_component_t *) handle;
    return component->id;
}

char *USER_FUNC hass_component_identifiers_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_IDENTIFIERS);
}

char *USER_FUNC hass_component_name_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_NAME);
}

hass_component_type_e USER_FUNC hass_component_type_get(hass_component_handle_t handle) {
    hass_component_t *component = (hass_component_t *) handle;
    return component->type;
}

char *USER_FUNC hass_component_icon_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_ICON);
}

char *USER_FUNC hass_component_unit_of_measurement_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_UNIT_OF_MEASUREMENT);
}

char *USER_FUNC hass_component_value_template_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_VALUE_TEMPLATE);
}

char *USER_FUNC hass_component_device_class_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_DEVICE_CLASS);
}

char *USER_FUNC hass_component_state_topic_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_STATE_TOPIC);
}

char *USER_FUNC hass_component_config_topic_get(hass_component_handle_t handle) {
    return get_component_value(handle, HASS_COMPONENT_CONFIG_TOPIC);
}
