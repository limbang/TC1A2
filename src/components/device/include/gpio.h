/*
 * gpio.h
 *
 *  Created on: 2019年12月26日
 *      Author: limbang-pc
 */

#ifndef SRC_COMPONENTS_DEVICE_INCLUDE_GPIO_H_
#define SRC_COMPONENTS_DEVICE_INCLUDE_GPIO_H_

#include <hsf.h>

typedef enum {
    GPIO25 = HFGPIO_F_USER_DEFINE,  /* key */
    GPIO5,                          /* led */
    GPIO22,                         /* relay1 */
    GPIO24,                         /* relay2 */
    GPIO8,                          /* relay3 */
    GPIO23,                         /* relay4 */
    GPADC0,                         /* relay5 */
    GPIO3,                          /* relay6 */
    GPIO7,                          /* power */
    GPIO12,                         /* current/voltage */
    GPIO4                           /* current/voltage set */
} gpio_number_e;

typedef enum {
    GPIO_LOW = 0, GPIO_HIGH = 1
} gpio_level_e;

bool USER_FUNC gpio_output_set(gpio_number_e gpio_num, gpio_level_e level);

bool USER_FUNC gpio_output_high(gpio_number_e gpio_num);

bool USER_FUNC gpio_output_low(gpio_number_e gpio_num);

gpio_level_e USER_FUNC gpioInputGet(gpio_number_e gpio_num);

#endif /* SRC_COMPONENTS_DEVICE_INCLUDE_GPIO_H_ */
