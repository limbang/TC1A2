/*
 * power_meter.c
 *
 *  Created on: 2020年1月3日
 *      Author: limbang-pc
 */

#include "power_meter.h"

#define     POWER_PROBE_TIME    10  // Number of seconds to probe for power before deciding none used

uint8_t _cf_pin;
uint8_t _cf1_pin;
uint8_t _sel_pin;

//static uint32_t clock_count_last = 0;
//static uint32_t clock_count = 0;
//static uint32_t timer_count = 0;
//static uint32_t timer_irq_count = 0;
//
//static float power;

//static void USER_FUNC timer_handler(hftimer_handle_t htimer) {
//    uint32_t timer = 0;
//
//    if ( timer_irq_count > 1 ){
//
//        timer = (clock_count - clock_count_last);
//
//        u_printf("power_irq_handler:%u %u %u ",timer,timer_irq_count,timer_count);
//
//        power = 17100000 * (timer_irq_count - 1) / timer;
//        timer_count = 0;
//        timer_irq_count = 0;
//
//        u_printf("power=%f\n",power);
//    } else{
//        timer_count++;
//    }
//}
//
//static void USER_FUNC cf_interrupt(void) {
//    clock_count = hfsys_get_time( );
//    if ( timer_irq_count == 0 ) clock_count_last = clock_count;
//    timer_irq_count++;
//}

float _power_multiplier;

bool _load_off;

uint32_t _cf_pulse_length = 0;
uint32_t _cf_pulse_last_time = 0;

uint32_t _energy_period_counter = 0;

void USER_FUNC set_power_multiplier(float power_multiplier){
    _power_multiplier = power_multiplier;
}


static void USER_FUNC timer_handler(hftimer_handle_t htimer) {

    if ((hfsys_get_time() - _cf_pulse_last_time) > (POWER_PROBE_TIME * 1000)){
        _cf_pulse_length = 0;    // No load for some time
        _load_off = true;
    }

    u_printf("[PowerMeter]:%u  %f  %f\r\n",_cf_pulse_length,_power_multiplier,(_cf_pulse_length > 0) ? _power_multiplier / _cf_pulse_length : 0);

}


static void USER_FUNC cf_interrupt(void) {
    uint32_t now = hfsys_get_time();
    if(_load_off){
        _cf_pulse_last_time = now;
        _load_off = false;
    }else{
        _cf_pulse_length = now - _cf_pulse_last_time;
        _cf_pulse_last_time = now;
        _energy_period_counter++;
    }
}

static void USER_FUNC intr_handler(uint32_t arg1, uint32_t fid) {
    if (fid == _cf_pin) {
        cf_interrupt();
    }
    if (fid == _cf1_pin) {
    }
}

void USER_FUNC powermeter_init(gpio_number_e cf_Num, gpio_number_e cf1_Num, gpio_number_e sel_Num, pm_run_mode_e run_mode) {

    _cf_pin = cf_Num;

    // Set cf pin to input mode and register interrupt function
    if (hfgpio_configure_fpin(cf_Num, HFM_IO_TYPE_INPUT) != HF_SUCCESS)
        u_printf("[PowerMeter]:config powermeter cf pin fail.\r\n");
    if (hfgpio_configure_fpin_interrupt(cf_Num, HFPIO_IT_FALL_EDGE, intr_handler, false) != HF_SUCCESS)
        u_printf("[PowerMeter]:config powermeter cf pin interrupt fail.\r\n");

    if (run_mode >= PM_RUN_MODE_2) {

        _cf1_pin = cf1_Num;

        // Set cf 1 pin to input mode and register interrupt function
        if (hfgpio_configure_fpin(cf1_Num, HFM_IO_TYPE_INPUT) != HF_SUCCESS)
            u_printf("[PowerMeter]:config powermeter cf1 pin fail.\r\n");
        if (hfgpio_configure_fpin_interrupt(cf1_Num, HFPIO_IT_FALL_EDGE, intr_handler, false) != HF_SUCCESS)
            u_printf("[PowerMeter]:config powermeter cf1 pin interrupt fail.\r\n");

        if (run_mode == PM_RUN_MODE_3) {

            _sel_pin = sel_Num;
            if (hfgpio_configure_fpin(sel_Num, HFPIO_DEFAULT | HFM_IO_OUTPUT_1) != HF_SUCCESS)
                u_printf("[PowerMeter]:config powermeter sel pin fail.\r\n");
        }
    }

    hftimer_handle_t timer = NULL;
    if ((timer = hftimer_create("timer", 1000, true, 1, timer_handler, 0)) == NULL)
        u_printf("[DeBug]:Create timer fail\r\n");
    if (hftimer_start(timer) != HF_SUCCESS)
        u_printf("[DeBug]:start timer fail\r\n");

    if (hfgpio_fenable_interrupt(cf_Num) != HF_SUCCESS)
        u_printf("[PowerMeter]:cf pin enable interrupt fail.\r\n");
    if (run_mode >= PM_RUN_MODE_2) {
        if (hfgpio_fenable_interrupt(cf1_Num) != HF_SUCCESS)
            u_printf("[PowerMeter]:cf1 pin enable interrupt fail.\r\n");
    }
}

