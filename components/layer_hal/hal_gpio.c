#include "hal_gpio.h"
#include "gpio_regs.h"
#include <stdint.h>
#include <stdbool.h>

static const uint32_t io_mux_reg[GPIO_PIN_COUNT] = {
    [0]  = IO_MUX_GPIO0_REG,    [1]  = IO_MUX_U0TXD_REG,    [2]  = IO_MUX_GPIO2_REG,    [3]  = IO_MUX_U0RXD_REG,
    [4]  = IO_MUX_GPIO4_REG,    [5]  = IO_MUX_GPIO5_REG,  /*[6]  = flash, do not use,   [7]  = flash, do not use,
    [8]  = flash, do not use,   [9]  = flash, do not use,   [10] = flash, do not use,   [11] = flash, do not use,*/
    [12] = IO_MUX_MTDI_REG,     [13] = IO_MUX_MTCK_REG,     [14] = IO_MUX_MTMS_REG,     [15] = IO_MUX_MTDO_REG,
    [16] = IO_MUX_GPIO16_REG,   [17] = IO_MUX_GPIO17_REG,   [18] = IO_MUX_GPIO18_REG,   [19] = IO_MUX_GPIO19_REG,
 /* [20] =    not bonded,  */   [21] = IO_MUX_GPIO21_REG,   [22] = IO_MUX_GPIO22_REG,   [23] = IO_MUX_GPIO23_REG,
 /* [24] =    not aviable, */   [25] = IO_MUX_GPIO25_REG,   [26] = IO_MUX_GPIO26_REG,   [27] = IO_MUX_GPIO27_REG,
 /* [28] =  does not exist,     [29] =   does not exist,    [30] =   does not exist,    [31] =   does not exist,*/
    [32] = IO_MUX_GPIO32_REG,   [33] = IO_MUX_GPIO33_REG,   [34] = IO_MUX_GPIO34_REG,   [35] = IO_MUX_GPIO35_REG,
    [36] = IO_MUX_GPIO36_REG,   [37] = IO_MUX_GPIO37_REG,   [38] = IO_MUX_GPIO38_REG,   [39] = IO_MUX_GPIO39_REG,
};

/* ========================================================================================================
 * ----------------------------------------------- Helpers ------------------------------------------------
 * ======================================================================================================== */

/* Bit of the pin inside its 32-bit bank: pin % 32 (GPIO32 -> bit 0 of the *1 registers) */
static inline uint32_t pin_mask(uint8_t pin){
    return 1UL << (pin & 31);
}

/* Bank of the pin: pin / 32 -> 0 for GPIO0..31, 1 for GPIO32..39 (pin must be valid) */
static inline uint8_t pin_bank(uint8_t pin){
    return pin >> 5;
}

/* Read-modify-write of a pin's IO_MUX register; pin must be valid */
static inline void io_mux_modify(uint8_t pin, uint32_t clear, uint32_t set){
    uint32_t addr   = io_mux_reg[pin];          /* address of this pin's IO_MUX register (just a number) */
    uint32_t v      = REG32(addr);              /* copy of the register's current content (1st hardware access) */  
    v = v & ~clear;                             /* clear the 'clear' bits on the copy, register untouched */
    v = v | set;                                /* set the 'set' bits on the copy */
    REG32(addr)     = v;                        /* write the copy back in a single access (2nd hardware access) */
}

/* ========================================================================================================
 * --------------------------------------- Public Functions in hal_gpio -----------------------------------
 * ======================================================================================================== */

/* --------------------------------------------- IO_MUX --------------------------------------------------- */

bool hal_gpio_is_valid(uint8_t pin){
    return (pin < GPIO_PIN_COUNT) && (io_mux_reg[pin] != 0);
}

bool hal_gpio_is_output_capable(uint8_t pin){
    return pin < GPIO_FIRST_INPUT_ONLY_PIN;     /*Assuming gpio's pin is valid*/
}

void hal_gpio_select_func(uint8_t pin){
    io_mux_modify(pin, IO_MUX_MCU_SEL_M, (IO_MUX_MCU_SEL_GPIO << IO_MUX_MCU_SEL_S));
}

/* ------------------------------------------- GPIO_MATRIX ------------------------------------------------ */



/* ----------------------------------------------- DATA --------------------------------------------------- */