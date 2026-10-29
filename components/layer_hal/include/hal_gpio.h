#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include <stdbool.h>

/* Internal pull resistor options (IO_MUX FUN_WPU / FUN_WPD)*/
typedef enum {HG_PULL_NONE, HG_PULL_UP, HG_PULL_DOWN }hal_gpio_pull_t;


/* ========================================================================================================
 * -------------------------- Pin capabilities (hardware facts, no register access) -----------------------
 * ======================================================================================================== */

/* True if the pin exists and has an IO_MUX register (not flash pins) */
bool hal_gpio_is_valid(uint8_t pin);

/* True if the pin has an output driver (false for GPIO 34..39) */
bool hal_gpio_is_output_capable(uint8_t pin);

/* --------------------------------------------- IO_MUX --------------------------------------------------- */

/**
 * brief: 
 * param:
 * doxygen
 * 
 */

/* Selects the GPIO function in IO_MUX (MCU_SEL = 2) */
void hal_gpio_select_func(uint8_t pin);

/* Enables/disables the input buffer (FUN_IE) */
void hal_gpio_set_input_enable(uint8_t pin, bool enable);

/* Configures the internal pull resistor (FUN_WPU / FUN_WPD) */
void hal_gpio_set_pull(uint8_t pin, hal_gpio_pull_t pull);

/* ------------------------------------------- GPIO_MATRIX ------------------------------------------------ */

/* Routes the pin to GPIO_OUT / GPIO_ENABLE (OUT_SEL = 0x100, no inversion) */
void hal_gpio_route_simple_output(uint8_t pin);

/* Enables/disables the output driver (GPIO_ENABLE[1]_W1TS / W1TC) */
void hal_gpio_set_output_enable(uint8_t pin, bool enable);

/* ----------------------------------------------- DATA --------------------------------------------------- */

/* Drives the pin high or low (GPIO_OUT[1]_W1TS / W1TC) */
void hal_gpio_set_level(uint8_t pin, bool level);

/* Reads the pin's real level (GPIO_IN / GPIO_IN1) */
bool hal_gpio_get_level(uint8_t pin);

/* Reads the level the pin is being driven to (GPIO_OUT / GPIO_OUT1) */
bool hal_gpio_get_output_level(uint8_t pin);

#endif