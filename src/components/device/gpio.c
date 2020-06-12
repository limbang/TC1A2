/*
 * gpio.c
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#include "gpio.h"

/**
 * gpio output set
 * 
 * @param gpio_num
 * @param level
 * @return
 */
bool USER_FUNC gpio_output_set(gpio_number_e gpio_num, gpio_level_e level) {
    if (level == GPIO_LOW)
        return gpio_output_low(gpio_num);
    else if (level == GPIO_HIGH)
        return gpio_output_high(gpio_num);
    else
        return false;
}

/**
 * gpio output high level
 *
 * @param gpio_num
 * @return
 */
bool USER_FUNC gpio_output_high(gpio_number_e gpio_num) {
    if (hfgpio_fset_out_high(gpio_num) == HF_SUCCESS)
        return true;
    else
        return false;
}

/**
 * gpio output low level
 * 
 * @param gpio_num
 * @return
 */
bool USER_FUNC gpio_output_low(gpio_number_e gpio_num) {
    if (hfgpio_fset_out_low(gpio_num) == HF_SUCCESS)
        return true;
    else
        return false;
}

/**
 * gpio input get
 * 
 * @param gpio_num
 * @return
 */
gpio_level_e USER_FUNC gpioInputGet(gpio_number_e gpio_num) {
    if (hfgpio_fpin_is_high(gpio_num) >= 1)
        return GPIO_HIGH;
    else
        return GPIO_LOW;
}
