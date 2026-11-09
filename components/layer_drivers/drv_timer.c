#include "drv_timer.h"

/*
TO-DO LIST:

URG|NIMP
- N/A

NURG|IMP
- Añadir validación de selección de opciones del usuario.
- Añadir retorno de errores y revisión parámetros.
- Formato doxygen.

NURG|NIMP
- N/A
*/

#define TICK_US_DIVIDER 80
typedef enum {TIMER1, TIMER2, TIMER3, TIMER4}   hal_avail_timers_e;
typedef enum {DISABLED, ENABLED}                hal_autoreload_e;

static uint64_t saved_reset_times[4];

intr_handle_t timer_free_handles[4];

static void sort_group_and_timer(hal_avail_timers_e selected_timer, avail_timer_groups_e_t*group_ptr_out, avail_timer_inGroups_e_t* timer_ptr_out){
    //Timer 1 y 2 de la app son de G0, Timer 3 y 4 son de G1
    *group_ptr_out = ((selected_timer == TIMER1 || selected_timer == TIMER2)? G0 : G1);
    //Timer 1 y 3 de la app serian el timer0 de sus respectivos grupos
    *timer_ptr_out = ((selected_timer == TIMER1 || selected_timer == TIMER3)? T0 : T1);
}

void drv_configure_timer(hal_avail_timers_e selected_timer, uint64_t time_us, hal_autoreload_e autoreload_enable, bool counts_up, timer_callback_t app_main_intr_func){
    
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (selected_timer, &group, &timer);

    saved_reset_times[selected_timer] = time_us;

    //
    timer_config_t config_struct ={
        .timer_ticks = time_us,
        .autoreload = autoreload_enable,
        .divider = TICK_US_DIVIDER, //Escala de us por defecto
        .ascending_count = counts_up,
        .callback_func = app_main_intr_func,
        .free_handle = &timer_free_handles[selected_timer]
    };
    hal_timer_configure(group, timer, config_struct);
}

void set_base_timer(){
    drv_configure_timer(TIMER1, 0xFFFFFFFF, ENABLED, true, NULL);
}

uint64_t get_millis(){
    uint64_t return_time = 0;
    hal_timer_get_time_us(G0,T0, &return_time);
    return_time = return_time/1000;
    return return_time;
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

void drv_restart_timer(hal_avail_timers_e sel_timer){
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (sel_timer, &group, &timer);

    hal_timer_restart(group, timer, saved_reset_times[sel_timer]);
}

void drv_reset_timer(hal_avail_timers_e sel_timer){
    avail_timer_groups_e_t group = 0;
    avail_timer_inGroups_e_t timer = 0; 
    sort_group_and_timer (sel_timer, &group, &timer);

    hal_timer_reset(group, timer, saved_reset_times[sel_timer]);
}