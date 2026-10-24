#include "timer_regs.h"

void drv_timer_start(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel){
    REG32(TIMG_CONFIG_REG(timer_group, timer_sel)) = TIMG_EN; //Habilitar el timer seleccionado del grupo.
}

void drv_timer_stop(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel){
    REG32(TIMG_CONFIG_REG(timer_group, timer_sel)) &= ~TIMG_EN; //Detener timer seleccionado del grupo.
}
void drv_timer_reset(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel, uint64_t time_toLoad){
    REG32(TIMG_CONFIG_REG (timer_group, timer_sel))&= ~TIMG_EN; //Detener el timer del grupo antes de cargar valor.
    REG32(TIMG_LOADLO_REG (timer_group, timer_sel)) = (uint32_t)(time_toLoad & 0x0000FFFF); //Máscara para extraer los 32 LSB de uint64_t.
    REG32(TIMG_LOADHI_REG (timer_group, timer_sel)) = (uint32_t)(time_toLoad >> 32); //Corrimiento para extraer los 32 MSB de uint64_t.
    REG32(TIMG_LOAD_REG   (timer_group, timer_sel)) = 1; //Cargar HI-LO REG
    REG32(TIMG_INT_CLR_REG(timer_group)) = TIMG_INT_T_BIT(timer_sel); //Limpiar interrupción del timer.
    REG32(TIMG_CONFIG_REG (timer_group, timer_sel)) |= (TIMG_ALARM_EN | TIMG_EN); //Habilitar alarma y timer del grupo.
}
void drv_timer_configure(avail_timer_groups_e_t timer_group, avail_timer_inGroups_e_t timer_sel, timer_config_t config_struct){
    //Añadir el intr_alloc para el ISR del timer. Leer puntero a función desde estructura de timer_config_t
    //Aplicar el prescaler y divider al timer. Incluir rutina de validación del divider.
}