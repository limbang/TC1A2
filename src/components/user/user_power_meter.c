/*
 * userBL0937.c
 *
 *  Created on: 2020年1月2日
 *      Author: limbang-pc
 */

#include "include/user_power_meter.h"

void user_power_meter_init(pm_run_mode_e run_mode){
    powermeter_init(GPIO7, GPIO12,GPIO4,run_mode);
}


