#include <stdint.h>
#include <stdbool.h>
#include "include/hal_gpio.h"
#include "regs/gpio_regs.h"

static const uint32_t io_mux_reg[GPIO_PIN_COUNT] = {
    [0]  = IO_MUX_GPIO0_REG,    [1]  = IO_MUX_U0TXD_REG,    [2]  = IO_MUX_GPIO2_REG,    [3]  = IO_MUX_U0RXD_REG,
    [4]  = IO_MUX_GPIO4_REG,    [5]  = IO_MUX_GPIO5_REG,  /*[6]  = flash, do not use,   [7]  = flash, do not use,
    [8]  = flash, do not use,   [9]  = flash, do not use,   [10] = flash, do not use,   [11] = flash, do not use,*/
    [12] = IO_MUX_MTDI_REG,     [13] = IO_MUX_MTCK_REG,     [14] = IO_MUX_MTMS_REG,     [15] = IO_MUX_MTDO_REG,
    [16] = IO_MUX_GPIO16_REG,   [17] = IO_MUX_GPIO17_REG,   [18] = IO_MUX_GPIO18_REG,   [19] = IO_MUX_GPIO19_REG,
 /* [20] =    not bonded,  */   [21] = IO_MUX_GPIO21_REG,   [22] = IO_MUX_GPIO22_REG,   [23] = IO_MUX_GPIO23_REG,
 /* [24] =    not bonded,  */   [25] = IO_MUX_GPIO25_REG,   [26] = IO_MUX_GPIO26_REG,   [27] = IO_MUX_GPIO27_REG,
 /* [28] =  does not exist,     [29] =   does not exist,    [30] =   does not exist,    [31] =   does not exist,*/
    [32] = IO_MUX_GPIO32_REG,   [33] = IO_MUX_GPIO33_REG,   [34] = IO_MUX_GPIO34_REG,   [35] = IO_MUX_GPIO35_REG,
    [36] = IO_MUX_GPIO36_REG,   [37] = IO_MUX_GPIO37_REG,   [38] = IO_MUX_GPIO38_REG,   [39] = IO_MUX_GPIO39_REG,
};

/* Bit of the pin inside its 32-bit bank: pin % 32 (GPIO32 -> bit 0 of the *1 registers) */
static inline uint32_t pin_mask(uint8_t pin){
    return 1UL << (pin & 31);
}

static inline uint32_t bank_reg(uint8_t pin, uint32_t reg_0_31, uint32_t reg_32_39){
    if(pin < 32){

    }
    
    return reg_0_31;
}

bool hal_gpio_is_valid(uint8_t pin){


    return true;
}