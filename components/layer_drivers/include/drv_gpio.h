#ifndef DRV_GPIO_H
#define DRV_GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include "fw_err.h"

typedef enum {
    DRV_GPIO_INPUT,
    DRV_GPIO_OUTPUT,
    DRV_GPIO_MODE_COUNT
} drv_gpio_mode_t;

typedef enum {
    DRV_GPIO_PULL_NONE,
    DRV_GPIO_PULL_UP,
    DRV_GPIO_PULL_DOWN,
    DRV_GPIO_PULL_COUNT
} drv_gpio_pull_t;

typedef struct {
    drv_gpio_mode_t     mode;
    drv_gpio_pull_t     pull;               /* inputs only: GPIO34-GPIO39 have no pulls */
    bool                initial_level;      /* outputs only: level driven right after config */
} drv_gpio_config_t;

/* Configures a pin as an output or input. Must be called before any other function on that pin. */
fw_err_t drv_gpio_config(uint8_t pin, const drv_gpio_config_t *config);

/* Drives an output pin high (true) or low (false); electrical level, no active-low inversion */
fw_err_t drv_gpio_write(uint8_t pin, bool level);

/* Level of the pin into *level: real level for inputs, driven level for outputs */
fw_err_t drv_gpio_read(uint8_t pin, bool *level);

/* Inverts the current level of an output pin */
fw_err_t drv_gpio_toggle(uint8_t pin);

#endif /* DRV_GPIO_H */
