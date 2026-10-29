#include "hal_timer.h"

static timer_callback_t timer_callback_gt [2][2] ={
    {NULL, NULL}, //G0 = {T0, T1}
    {NULL, NULL}  //G1 = {T0, T1}
};

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
