#include "hal_gpio.h"
#include "gpio_regs.h"
#include <stdint.h>
#include <stdbool.h>

/* IO_MUX offsets have no pattern (TRM v4.6, Table 18); missing entries stay 0 = pin not usable */
static const uint32_t io_mux_reg[GPIO_PIN_COUNT] = {
    [0]  = IO_MUX_GPIO0_REG,    [1]  = IO_MUX_U0TXD_REG,    [2]  = IO_MUX_GPIO2_REG,    [3]  = IO_MUX_U0RXD_REG,
    [4]  = IO_MUX_GPIO4_REG,    [5]  = IO_MUX_GPIO5_REG,  /*[6]  = flash, do not use,   [7]  = flash, do not use,
    [8]  = flash, do not use,   [9]  = flash, do not use,   [10] = flash, do not use,   [11] = flash, do not use,*/
    [12] = IO_MUX_MTDI_REG,     [13] = IO_MUX_MTCK_REG,     [14] = IO_MUX_MTMS_REG,     [15] = IO_MUX_MTDO_REG,
    [16] = IO_MUX_GPIO16_REG,   [17] = IO_MUX_GPIO17_REG,   [18] = IO_MUX_GPIO18_REG,   [19] = IO_MUX_GPIO19_REG,
 /* [20] =    not bonded,  */   [21] = IO_MUX_GPIO21_REG,   [22] = IO_MUX_GPIO22_REG,   [23] = IO_MUX_GPIO23_REG,
 /* [24] =   not available,*/   [25] = IO_MUX_GPIO25_REG,   [26] = IO_MUX_GPIO26_REG,   [27] = IO_MUX_GPIO27_REG,
 /* [28] =  does not exist,     [29] =   does not exist,    [30] =   does not exist,    [31] =   does not exist,*/
    [32] = IO_MUX_GPIO32_REG,   [33] = IO_MUX_GPIO33_REG,   [34] = IO_MUX_GPIO34_REG,   [35] = IO_MUX_GPIO35_REG,
    [36] = IO_MUX_GPIO36_REG,   [37] = IO_MUX_GPIO37_REG,   [38] = IO_MUX_GPIO38_REG,   [39] = IO_MUX_GPIO39_REG,
};

/* ========================================================================================================
 * ----------------------------------------------- Helpers ------------------------------------------------
 * ======================================================================================================== */

/* Bit of the pin inside its 32-bit register */
static inline uint32_t pin_mask(uint8_t pin){// add inline
    return 1UL << (pin % GPIO_BANK_SIZE);
}

/* Register of the pair the pin lives in: 0 for GPIO0..31, 1 for GPIO32..39 */
static inline uint8_t pin_bank(uint8_t pin){
    return pin / GPIO_BANK_SIZE;
}

/* IO_MUX holds several settings per pin, so only the requested bits may change; pin must be valid */
static inline void io_mux_modify(uint8_t pin, uint32_t clear, uint32_t set){
    uint32_t addr   = io_mux_reg[pin];
    uint32_t v      = REG32(addr);
    v = v & ~clear;
    v = v | set;
    REG32(addr)     = v;                        /* single write: no intermediate state on the pin */
}

/* ========================================================================================================
 * --------------------------------------- Public Functions in hal_gpio -----------------------------------
 * ======================================================================================================== */

/*-------------------------- Pin capabilities (hardware facts, no register access) -----------------------*/

bool hal_gpio_is_valid(uint8_t pin){
    return (pin < GPIO_PIN_COUNT) && (io_mux_reg[pin] != 0);    /* range first: never reads out of bounds */
}

bool hal_gpio_is_output_capable(uint8_t pin){
    return pin < GPIO_FIRST_INPUT_ONLY_PIN;
}

/* --------------------------------------------- IO_MUX ---------------------------------------------------
 *      bit:   14   13   12 | 11   10 |  9 |  8  |  7          (TRM v4.6, Register 4.34)
 *                 MCU_SEL  | FUN_DRV | IE | WPU | WPD
 * -------------------------------------------------------------------------------------------------------- */

void hal_gpio_select_func(uint8_t pin){
    io_mux_modify(pin, IO_MUX_MCU_SEL_M, (IO_MUX_MCU_SEL_GPIO << IO_MUX_MCU_SEL_S));
}

void hal_gpio_set_input_enable(uint8_t pin, bool enable){
    if(enable)  io_mux_modify(pin, IO_MUX_FUN_IE, IO_MUX_FUN_IE);
    else        io_mux_modify(pin, IO_MUX_FUN_IE, 0);
}

void hal_gpio_set_pull(uint8_t pin, hal_gpio_pull_t pull){
    uint32_t set = 0;

    switch (pull){
        case HG_PULL_NONE:
            set = 0;
            break;

        case HG_PULL_UP:
            set = IO_MUX_FUN_WPU;
            break;

        case HG_PULL_DOWN:
            set = IO_MUX_FUN_WPD;
            break;
    }

    io_mux_modify(pin, (IO_MUX_FUN_WPD | IO_MUX_FUN_WPU), set);    /* both cleared so pulls never mix */
}

/* ------------------------------------------- GPIO_MATRIX ------------------------------------------------
 *      bit:       11      |    10   |      9      |   8 ... 0     (TRM v4.6, Register 4.32)
 *             OEN_INV_SEL | OEN_SEL | OUT_INV_SEL | OUT_SEL
 * -------------------------------------------------------------------------------------------------------- */

void hal_gpio_route_simple_output(uint8_t pin){
    uint32_t v      = REG32(GPIO_FUNC_OUT_SEL_CFG_REG(pin));
    uint32_t clear  = GPIO_FUNC_OEN_INV_SEL | GPIO_FUNC_OEN_SEL | GPIO_FUNC_OUT_INV_SEL | GPIO_FUNC_OUT_SEL_M;
    uint32_t set    = GPIO_FUNC_OUT_SEL_GPIO << GPIO_FUNC_OUT_SEL_S;

    v = v & ~clear;                             /* reset value is undefined: every field is set explicitly */
    v = v | set;

    REG32(GPIO_FUNC_OUT_SEL_CFG_REG(pin)) = v;
}

void hal_gpio_set_output_enable(uint8_t pin, bool enable){
    if(enable)  REG32(GPIO_ENABLE_W1TS_REG(pin_bank(pin))) = pin_mask(pin);
    else        REG32(GPIO_ENABLE_W1TC_REG(pin_bank(pin))) = pin_mask(pin);
}

/* ----------------------------------------------- DATA --------------------------------------------------- */

void hal_gpio_set_level(uint8_t pin, bool level){
    if(level)   REG32(GPIO_OUT_W1TS_REG(pin_bank(pin))) = pin_mask(pin);
    else        REG32(GPIO_OUT_W1TC_REG(pin_bank(pin))) = pin_mask(pin);
}

bool hal_gpio_get_level(uint8_t pin){
    uint32_t v = REG32(GPIO_IN_REG(pin_bank(pin)));
    if((pin_mask(pin) & v) == 0){               /* keep only this pin's bit out of the whole bank */
        return false;
    }
    return true;
}

bool hal_gpio_get_output_level(uint8_t pin){
    uint32_t v = REG32(GPIO_OUT_REG(pin_bank(pin)));
    if((pin_mask(pin) & v) == 0){               /* keep only this pin's bit out of the whole bank */
        return false;
    }
    return true;
}