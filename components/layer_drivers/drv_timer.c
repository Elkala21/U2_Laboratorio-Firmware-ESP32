
#include "drv_timer.h"

#define TICK_US_DIVIDER 80

typedef enum {TIMER1, TIMER2, TIMER3, TIMER4}hal_avail_timers_e;
typedef enum {DISABLED, ENABLED}hal_autoreload_e;

intr_handle_t timer_free_handles[4];

static void sort_group_and_timer(hal_avail_timers_e selected_timer, avail_timer_groups_e_t*group_ptr_out, avail_timer_inGroups_e_t* timer_ptr_out){
    //Timer 1 y 2 de la app son de G0, Timer 3 y 4 son de G1
    *group_ptr_out = ((selected_timer == TIMER1 || selected_timer == TIMER2)? G0 : G1);
    //Timer 1 y 3 de la app serian el timer0 de sus respectivos grupos
    *timer_ptr_out = ((selected_timer == TIMER1 || selected_timer == TIMER3)? T0 : T1);
}

void drv_configure_timer(hal_avail_timers_e selected_timer, uint64_t time_us, hal_autoreload_e autoreload_enable, timer_callback_t app_main_int_func){
    
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (selected_timer, &group, &timer);

    //
    timer_config_t config_struct ={
        .timer_ticks = time_us,
        .autoreload = autoreload_enable,
        .divider = TICK_US_DIVIDER,
        .callback_func = app_main_int_func,
        .free_handle = &timer_free_handles[selected_timer]
    };
    hal_timer_configure(group, timer, config_struct);
}

void drv_stop_timer(hal_avail_timers_e selected_timer){
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (selected_timer, &group, &timer);

    hal_timer_stop(group, timer);
}

void drv_start_timer(hal_avail_timers_e selected_timer){
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (selected_timer, &group, &timer);

    hal_timer_start(group, timer);
}

void drv_restart_timer(hal_avail_timers_e selected_timer){
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (selected_timer, &group, &timer);

    uint64_t restart_time = NaN ;// = 0; //Define where to save and recall times from.

    hal_timer_restart(group, timer, restart_time);
}

void drv_reset_timer(hal_avail_timers_e sel_timer){
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (sel_timer, &group, &timer);

    uint64_t reset_time = NaN; // = 0; //Define where to save and recall timer times from.

    hal_timer_reset(group, timer, reset_time);
}