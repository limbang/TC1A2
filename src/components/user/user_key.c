//
// Created by limbang-pc on 2019/12/31.
//

#include "user_key.h"



relay_status_e _relay_all_status = RELAY_STATUS_CLOSE;


uint32_t _interrupt_time;
uint32_t _last_interrupt_time;
uint32_t _interrupt_duration;

/**
 * interrupt handler
 *
 * @param state
 * @param fid
 */
static void USER_FUNC interrupt_handler(uint32_t state,uint32_t fid){

    _interrupt_time = hfsys_get_time();

    if(state == 2){
        _last_interrupt_time = _interrupt_time;
    }else{
        _interrupt_duration = hfsys_get_time() - _last_interrupt_time;

        // 按键去抖
        if(_interrupt_duration < 30){
            return;
        }

        // 短按
        if(_interrupt_duration < 1000){
            // 控制全部继电器
            if(!_relay_all_status){
                user_relay_aLL_set(RELAY_STATUS_OPEN);
                _relay_all_status = RELAY_STATUS_OPEN;
                user_led_on();
            }else{
                user_relay_aLL_set(RELAY_STATUS_CLOSE);
                _relay_all_status = RELAY_STATUS_CLOSE;
                user_led_off();
            }
        }

        // 长按
        if(_interrupt_duration > 3000){
            u_printf("[DeBug]:Start smartlink,soft restart system.\r\n");
            hfsmtlk_start();
        }
    }
}

void USER_FUNC user_key_init(void){
    // 初始化按键
    if (hfgpio_configure_fpin_interrupt(GPIO25, HFPIO_IT_EDGE,interrupt_handler, TRUE) != HF_SUCCESS)
        u_printf("[DeBug]:KEY config interrupt fail.\r\n");
}
