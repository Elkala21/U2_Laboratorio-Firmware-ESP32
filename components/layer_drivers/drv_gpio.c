#include "drv_gpio.h"
#include "hal_gpio.h"

/* ========================================================================================================
 * ----------------------------------------------- Helpers ------------------------------------------------
 * ======================================================================================================== */

static hal_gpio_pull_t to_hal_pull(drv_gpio_pull_t pull)
{
    switch (pull){
        case DRV_GPIO_PULL_UP:      return HAL_GPIO_PULL_UP;
        case DRV_GPIO_PULL_DOWN:    return HAL_GPIO_PULL_DOWN;
        case DRV_GPIO_PULL_NONE:    return HAL_GPIO_PULL_NONE;
        case DRV_GPIO_PULL_COUNT:   break;
    }

    return HAL_GPIO_PULL_NONE;
}

static fw_err_t write_validation(uint8_t pin){

    if (hal_gpio_is_valid(pin) == false) {
        return FW_ERR_INVALID_ARG;
    }

    if (hal_gpio_is_output_capable(pin) == false) {
        return FW_ERR_NOT_SUPPORTED;
    }

    if (hal_gpio_is_output_enabled(pin) == false) {
        return FW_ERR_INVALID_STATE;
    }

    return FW_OK;
}

/* ========================================================================================================
 * --------------------------------------- Public Functions in drv_gpio -----------------------------------
 * ======================================================================================================== */

fw_err_t drv_gpio_config(uint8_t pin, const drv_gpio_config_t *config)
{
    /* =============================================== Guard Clauses ========================================== */

    if (config == NULL) {
        return FW_ERR_INVALID_ARG;
    }

    if (hal_gpio_is_valid(pin) == false) {
        return FW_ERR_INVALID_ARG;
    }

    if ((config->mode >= DRV_GPIO_MODE_COUNT) || (config->pull >= DRV_GPIO_PULL_COUNT)) {
        return FW_ERR_INVALID_ARG;
    }

    if ((config->mode == DRV_GPIO_OUTPUT) && !hal_gpio_is_output_capable(pin)) {
        return FW_ERR_NOT_SUPPORTED;
    }

    if ((config->pull != DRV_GPIO_PULL_NONE) && !hal_gpio_has_pulls(pin)) {
        return FW_ERR_NOT_SUPPORTED;
    }

    /* ============================================ Set Pin Configuration ===================================== */

    hal_gpio_select_func(pin);

    /* --------------------------------------------- Input Configuration -------------------------------------- */
    if (config->mode == DRV_GPIO_INPUT) {
        hal_gpio_set_output_enable(pin, false);
        hal_gpio_set_input_enable(pin, true);
        hal_gpio_set_pull(pin, to_hal_pull(config->pull));

        return FW_OK;
    }

    /* --------------------------------------------- Output Configuration ------------------------------------- */
    hal_gpio_set_pull(pin, HAL_GPIO_PULL_NONE);
    hal_gpio_set_input_enable(pin, false);
    hal_gpio_set_level(pin, config->initial_level);
    hal_gpio_route_simple_output(pin);
    hal_gpio_set_output_enable(pin, true);

    return FW_OK;
}

fw_err_t drv_gpio_write(uint8_t pin, bool level)
{
    /* =============================================== Guard Clauses ========================================== */

    fw_err_t err = write_validation(pin);

    if (err != FW_OK){
        return err;
    }

    /* ================================================ Write in pin ========================================== */

    hal_gpio_set_level(pin, level);

    return FW_OK;
}

fw_err_t drv_gpio_read(uint8_t pin, bool *level)
{
    /* =============================================== Guard Clauses ========================================== */

    if (level == NULL){
        return FW_ERR_INVALID_ARG;
    }

    if (!hal_gpio_is_valid(pin)) {
        return FW_ERR_INVALID_ARG;
    }

    bool outputState = hal_gpio_is_output_enabled(pin);

    if ((hal_gpio_is_input_enabled(pin) == false) && (outputState == false)) {
        return FW_ERR_INVALID_STATE;
    }

    /* ================================================ Write in pin ========================================== */

    if (outputState == true) {
        *level = hal_gpio_get_output_level(pin);
        return FW_OK;
    }

    *level = hal_gpio_get_level(pin);

    return FW_OK;
}

fw_err_t drv_gpio_toggle(uint8_t pin){
    /* =============================================== Guard Clauses ========================================== */

    fw_err_t err = write_validation(pin);

    if (err != FW_OK){
        return err;
    }

    /* ================================================ Write in pin ========================================== */

    bool level = hal_gpio_get_output_level(pin);

    hal_gpio_set_level(pin, !level);

    return FW_OK;
}