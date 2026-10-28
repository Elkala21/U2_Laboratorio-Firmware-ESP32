#include "esp_intr_alloc.h"
#include "esp_intr_types.h"
#include "soc/interrupts.h"

typedef enum {G0, G1} avail_timer_groups_e_t;
typedef enum {T0, T1} avail_timer_inGroups_e_t;

typedef void(*timer_callback_t)(void); //Puntero a función sin valor de retorno ni parámetros.

/*Inicio de estructura de configuración del timer (WIP)*/
typedef struct {
    uint64_t timer_ticks;
    uint32_t autoreload; //Revisar si realmente es necesario que sea de 32 bits.
    uint16_t divider; //Reloj resultante = 80MHz/divider
    uint32_t intr_source; //Establecer el tipo de interrupcion para LEVEL (Llamada CTE a ISR si no se desactiva) y EDGE (Llamada única que debe desactivarse para llamarse nuevamente)
    //intr_handler_t timer_isr; //Revisar el tipo de dato que debería tener el puntero a función.
    timer_callback_t callback_func; //Puntero a función de callback.
    intr_handle_t *free_handle;
} timer_config_t;

void drv_timer_alarm_enable(avail_timer_groups_e_t, avail_timer_inGroups_e_t);

void drv_timer_start(avail_timer_groups_e_t, avail_timer_inGroups_e_t);

void drv_timer_stop(avail_timer_groups_e_t, avail_timer_inGroups_e_t);

void drv_timer_clr_int(avail_timer_groups_e_t, avail_timer_inGroups_e_t);

void drv_timer_restart(avail_timer_groups_e_t, avail_timer_inGroups_e_t, uint64_t);

void drv_timer_reset(avail_timer_groups_e_t, avail_timer_inGroups_e_t, uint64_t);

void drv_timer_configure(avail_timer_groups_e_t, avail_timer_inGroups_e_t, timer_config_t);