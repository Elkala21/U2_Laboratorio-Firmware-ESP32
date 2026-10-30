#ifndef DRV_GPIO_H
#define DRV_GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include "fw_err.h"

typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT
} drv_gpio_mode_t;

typedef enum {
    GPIO_PULL_NONE,
    GPIUO_PULL_UP,
    GPIO_PULL_DOWN
} drv_gpio_pull_t;

typedef struct {
    uint8_t             pin;
    drv_gpio_mode_t     mode;
    drv_gpio_pull_t     pull;           /* inputs only: GPIO34-GPIO39 have no pulls */
    bool                initial_level;   /* outputs only: level driven right after config */
} drv_gpio_config_t;

/* Configures a pin as an output or input. Must be called before any other function on that pin. */
fw_err_t drv_gpio_config(uint8_t pin, drv_gpio_config_t *config);

/* Drives an output pin high (true) or low (false) */
fw_err_t drv_gpio_write(uint8_t pin, bool level);

/* Reads the real level of a pin into *level */
fw_err_t drv_gpio_read(uint8_t pin, bool *level);

/* Inverts the current level of an output level */
fw_err_t drv_gpio_toggle(uint8_t pin);

#endif