#include "hal_timer.h"

/*
TO-DO LIST:
URG|IMP
- Bloquear un timer para conteo general.
- Reconfigurar el como se reservan las ISRs de los timers para evitar uso del timer general. 
(Solo permitir al usuario configurar este timer directamente con HAL)

URG|NIMP
- N/A

NURG|IMP
- Añadir validación de selección de opciones del usuario.
- Añadir retorno de errores y revisión parámetros.
- Formato doxygen.

NURG|NIMP
- N/A
*/

static timer_callback_t timer_callback_gt [2][2] ={
    {NULL, NULL}, //G0 = {T0, T1}
    {NULL, NULL}  //G1 = {T0, T1}
};

static void hal_wdt_feed(avail_timer_groups_e_t timer_group){
    //Permitir escritura de WDT.
    REG32(TIMG_WDTWPROTECT_REG(timer_group)) = DISABLE_WDT_WRITE_PROTECTION;

    //Recargar WDT
    REG32(TIMG_WDTFEED_REG(timer_group)) = FEED_UNPROTECTED_WDT;

    //Bloquear escritura de WDT.
    REG32(TIMG_WDTWPROTECT_REG(timer_group)) = ENABLE_WDT_WRITE_PROTECTION;
}

void hal_timer00_isr(void*arg){
    /*Ejecución pre-callback*/

    /*Callback*/
    if(timer_callback_gt[0][0] != NULL) //Si existe un puntero a función, mandarlo llamar.
        timer_callback_gt [0][0]();
    
    /*Ejecución post-callback*/
}

void hal_timer01_isr(void*arg){
    /*Puntero a función específicado por el usuario desde app main*/
    if(timer_callback_gt[0][1] != NULL) //Si existe un puntero a función, mandarlo llamar.
        timer_callback_gt [0][1]();
}

void hal_timer10_isr(void*arg){
    /*Puntero a función específicado por el usuario desde app main*/
    if(timer_callback_gt[1][0] != NULL) //Si existe un puntero a función, mandarlo llamar.
        timer_callback_gt [1][0]();
}

void hal_timer11_isr(void*arg){
    /*Puntero a función específicado por el usuario desde app main*/
    if(timer_callback_gt[1][1] != NULL) //Si existe un puntero a función, mandarlo llamar.
        timer_callback_gt [1][1]();
}

void hal_timer_alarm_enable(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel){
    REG32(TIMG_CONFIG_REG (timer_group, timer_sel)) |= TIMG_ALARM_EN;
}

void hal_timer_start(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel){
    hal_timer_alarm_enable(timer_group, timer_sel);
    REG32(TIMG_CONFIG_REG(timer_group, timer_sel)) |= TIMG_EN; //Habilitar el timer seleccionado del grupo.
}

void hal_timer_stop(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel){
    REG32(TIMG_CONFIG_REG(timer_group, timer_sel)) &= ~TIMG_EN; //Detener timer seleccionado del grupo.
}

void hal_timer_clr_int(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel){
    REG32(TIMG_INT_CLR_REG(timer_group)) = TIMG_INT_T_BIT(timer_sel); //Limpiar interrupción del timer.
}

void hal_timer_restart(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel, uint64_t time_toLoad){
    hal_timer_stop(timer_group, timer_sel); //Detener el timer del grupo antes de cargar valor.
    REG32(TIMG_LOADLO_REG (timer_group, timer_sel)) = (uint32_t)(time_toLoad & 0xFFFFFFFF); //Máscara para extraer los 32 LSB de uint64_t.
    REG32(TIMG_LOADHI_REG (timer_group, timer_sel)) = (uint32_t)(time_toLoad >> 32); //Corrimiento para extraer los 32 MSB de uint64_t.
    REG32(TIMG_LOAD_REG   (timer_group, timer_sel)) = 1; //Cargar HI-LO REG
    hal_timer_clr_int(timer_group, timer_sel); //Limpiar interrupción del timer.
    hal_timer_start(timer_group, timer_sel);
}

void hal_timer_reset(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel, uint64_t time_toLoad){
    hal_timer_stop(timer_group, timer_sel); //Detener el timer del grupo antes de cargar valor.
    REG32(TIMG_LOADLO_REG (timer_group, timer_sel)) = (uint32_t)(time_toLoad & 0xFFFFFFFF); //Máscara para extraer los 32 LSB de uint64_t.
    REG32(TIMG_LOADHI_REG (timer_group, timer_sel)) = (uint32_t)(time_toLoad >> 32); //Corrimiento para extraer los 32 MSB de uint64_t.
    REG32(TIMG_LOAD_REG   (timer_group, timer_sel)) = 1; //Cargar HI-LO REG
    hal_timer_clr_int(timer_group, timer_sel); //Limpiar interrupción del timer.
    hal_timer_alarm_enable(timer_group, timer_sel);
}

void hal_timer_configure(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel, timer_config_t config_struct){
    REG32(TIMG_CONFIG_REG (timer_group, timer_sel))&= ~TIMG_EN; //Detener el timer del grupo antes de cargar valor.
    REG32(TIMG_LOADLO_REG (timer_group, timer_sel)) = (uint32_t)(config_struct.timer_ticks & 0xFFFFFFFF); //Máscara para extraer los 32 LSB de uint64_t.
    REG32(TIMG_LOADHI_REG (timer_group, timer_sel)) = (uint32_t)(config_struct.timer_ticks >> 32); //Corrimiento para extraer los 32 MSB de uint64_t.
    REG32(TIMG_LOAD_REG   (timer_group, timer_sel)) = 1; //Cargar HI-LO REG
    hal_timer_clr_int(timer_group, timer_sel); //Limpiar interrupción del timer.
    REG32(TIMG_CONFIG_REG (timer_group, timer_sel))|= TIMG_DIVIDER_VAL(config_struct.divider); //CREAR UNA LIMPIEZA PREVIA DE LOS BITS EN PRESCALER (Creo que no se requiere, ya se hace aqui)

    //Arreglo de los timers disponibles para ISRs
    intr_handler_t selected_timer_isr[2][2] ={
        {hal_timer00_isr, hal_timer01_isr},
        {hal_timer10_isr, hal_timer11_isr}
    };

    //Reservar el puntero a la función de interrupción del timer correspondiente al grupo y timer seleccionados.
    esp_intr_alloc(config_struct.intr_source, ESP_INTR_FLAG_IRAM, selected_timer_isr[timer_group][timer_sel], NULL, config_struct.free_handle);
}



void hal_timer_get_time_us(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel, uint64_t*time_read_ptr_out){
    REG32(TIMG_UPDATE_REG(timer_group, timer_sel)) = 1;
    uint32_t MSB = (REG32(TIMG_HI_REG(timer_group, timer_sel))); //Obtener los 32 bits mas altos del timer
    uint32_t LSB = (REG32(TIMG_LO_REG(timer_group, timer_sel))); //Obtener los 32 bits más bajos del timer
    
    *time_read_ptr_out =  (((uint64_t)MSB<<32)|((uint64_t)LSB)); //Retornar los 64 bits al puntero.
}

void hal_master_timer_get_time_us(uint64_t*time_read_ptr_out){
    hal_timer_get_time_us(G0, T1, time_read_ptr_out);
}

void hal_delay_us_ticks(uint64_t delay_us_ticks){
    //Revisar en que parte del timer iniciamos la cuenta para el delay
    uint64_t count_start_point = 0;
    hal_timer_get_time_us(G0,T1, &count_start_point); //Obtener el tiempo del timer general al que se inicio el delay.

    //Variable a cambiar para ir revisando el tiempo transcurrido durante el delay
    uint64_t current_time = count_start_point;

    //Variable que revisa el último tiempo en el que el WDT se alimentó
    uint64_t last_wdt_feed = count_start_point;

    while(delay_us_ticks > (current_time - count_start_point)){
        hal_timer_get_time_us(G0,T1, &current_time);
        if(current_time-last_wdt_feed >= 20000){
            last_wdt_feed = current_time;
            hal_wdt_feed(G0);
        }
    }
}

void hal_delay_ms_ticks(uint64_t delay_ms){
    uint64_t current_ms = 0;
    while(current_ms < delay_ms){
        hal_delay_us_ticks(1000);
        current_ms++;
    }
}

void hal_delay_s_ticks(uint64_t delay_s_ticks){
    uint64_t current_secs = 0;
    while(current_secs < delay_s_ticks){
        hal_delay_ms_ticks(1000);
        current_secs++;
    }
}