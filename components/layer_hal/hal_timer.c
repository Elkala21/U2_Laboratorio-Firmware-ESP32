#include "hal_timer.h"

#define TICK_US_DIVIDER 80

typedef enum {TIMER1, TIMER2, TIMER3, TIMER4}hal_avail_timers_e;
typedef enum {DISABLED, ENABLED}hal_autoreload_e;

intr_handle_t timer_free_handles[4];

void configure_timer(hal_avail_timers_e selected_timer, uint64_t time_us, hal_autoreload_e autoreload_enable, timer_callback_t app_main_int_func){
    //Timer 1 y 2 de la app son de G0, Timer 3 y 4 son de G1
    avail_timer_groups_e_t group = ((selected_timer == TIMER1 || selected_timer == TIMER2)? G0 : G1); 

    //Timer 1 y 3 de la app serian el timer0 de sus respectivos grupos
    avail_timer_inGroups_e_t timer = ((selected_timer == TIMER1 || selected_timer == TIMER3)? T0 : T1); 

    //
    timer_config_t config_struct ={
        .timer_ticks = time_us,
        .autoreload = autoreload_enable,
        .divider = TICK_US_DIVIDER,
        .callback_func = app_main_int_func,
        .free_handle = &timer_free_handles[selected_timer]
    };
    drv_timer_configure(group, timer, config_struct);
}