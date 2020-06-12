/*
 * power_meter.h
 *
 *  Created on: 2020年1月3日
 *      Author: limbang-pc
 */

#ifndef SRC_COMPONENTS_DEVICE_INCLUDE_POWER_METER_H_
#define SRC_COMPONENTS_DEVICE_INCLUDE_POWER_METER_H_

#include "gpio.h"

typedef enum {
    PM_RUN_MODE_1 = 0,   /* 只有 CF 脚可用 */
    PM_RUN_MODE_2,       /* CF CF1 脚可用 SEL脚固定  */
    PM_RUN_MODE_3        /* CF CF1 SEL 都可用  */
} pm_run_mode_e;

typedef enum {
    PM_POWER = 0,
    PM_CURRENT_VOLTAGE,
} pm_value_type_e;

void USER_FUNC set_power_multiplier(float power_multiplier);

void USER_FUNC powermeter_init(gpio_number_e cf_Num, gpio_number_e cf1_Num, gpio_number_e sel_Num, pm_run_mode_e run_mode);


#endif /* SRC_COMPONENTS_DEVICE_INCLUDE_POWER_METER_H_ */
