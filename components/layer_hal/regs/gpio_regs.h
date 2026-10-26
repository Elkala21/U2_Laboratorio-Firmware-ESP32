#ifndef GPIO_REGS_H
#define GPIO_REGS_H

#include "reg_access.h"

/*===============================================================================================
 * GPIO peripherial
 * TRM v4.6: Chapter 4.12.1 "GPIO Matrix Register Summary"
 *===============================================================================================*/
#define GPIO_BASE                   0x3FF44000UL
#define GPIO_BANK_STRIDE            0x0C        // Distance between bank 0 and bank 1 

/* --- Pins 0 ... 39 (one bit per pin: *_REG -> GPIO 0-31, *1_REG -> GPIO 32-39) --- */

#define GPIO_OUT_REG(c)             (GPIO_BASE + ((c) * 0x0C) + 0x04)
#define GPIO_OUT_W1TS_REG(c)        (GPIO_BASE + ((c) * 0x0C) + 0x08)
#define GPIO_OUT_W1TC_REG(c)        (GPIO_BASE + ((c) * 0x0C) + 0x0C)
#define GPIO_ENABLE_REG(c)          (GPIO_BASE + ((c) * 0x0C) + 0x20)
#define GPIO_ENABLE_W1TS_REG(c)     (GPIO_BASE + ((c) * 0x0C) + 0x24)
#define GPIO_ENABLE_W1TC_REG(c)     (GPIO_BASE + ((c) * 0x0C) + 0x28)
#define GPIO_STRAP_REG              (GPIO_BASE + 0x38)
#define GPIO_IN_REG                 (GPIO_BASE + 0x3C)
#define GPIO_IN1_REG                (GPIO_BASE + 0x40)

/* --- Interrupt status --- */

#define GPIO_STATUS_REG             (GPIO_BASE + 0x44)
#define GPIO_STATUS_W1TS_REG        (GPIO_BASE + 0x48)
#define GPIO_STATUS_W1TC_REG        (GPIO_BASE + 0x4C)
#define GPIO_STATUS1_REG            (GPIO_BASE + 0x50)
#define GPIO_STATUS1_W1TS_REG       (GPIO_BASE + 0x54)
#define GPIO_STATUS1_W1TC_REG       (GPIO_BASE + 0x58)
#define GPIO_ACPU_INT_REG           (GPIO_BASE + 0x60)
#define GPIO_ACPU_NMI_INT_REG       (GPIO_BASE + 0x64)
#define GPIO_PCPU_INT_REG           (GPIO_BASE + 0x68)
#define GPIO_PCPU_NMI_INT_REG       (GPIO_BASE + 0x6C)
#define GPIO_ACPU_INT1_REG          (GPIO_BASE + 0x74)
#define GPIO_ACPU_NMI_INT1_REG      (GPIO_BASE + 0x78)
#define GPIO_PCPU_INT1_REG          (GPIO_BASE + 0x7C)
#define GPIO_PCPU_NMI_INT1_REG      (GPIO_BASE + 0x80)

/* --- Per-pin / per-signal registers (n = index) --- */

#define GPIO_PIN_REG(n)                 (GPIO_BASE + 0x88  + ((n) * 4))    /* n = 0 ... 39  */
#define GPIO_FUNC_IN_SEL_CFG_REG(n)     (GPIO_BASE + 0x130 + ((n) * 4))    /* n = 0 ... 255 */
#define GPIO_FUNC_OUT_SEL_CFG_REG(n)    (GPIO_BASE + 0x530 + ((n) * 4))    /* n = 0 ... 39  */

/*===============================================================================================
 * TRM v4.6: Chapter 4.12.2 "IO MUX Register Summary"
 *===============================================================================================*/

#define IO_MUX_BASE                 0x3FF49000UL

#define IO_MUX_PIN_CTRL             (IO_MUX_BASE + 0x00)
#define IO_MUX_GPIO36_REG           (IO_MUX_BASE + 0x04)
#define IO_MUX_GPIO37_REG           (IO_MUX_BASE + 0x08)
#define IO_MUX_GPIO38_REG           (IO_MUX_BASE + 0x0C)
#define IO_MUX_GPIO39_REG           (IO_MUX_BASE + 0x10)
#define IO_MUX_GPIO34_REG           (IO_MUX_BASE + 0x14)
#define IO_MUX_GPIO35_REG           (IO_MUX_BASE + 0x18)
#define IO_MUX_GPIO32_REG           (IO_MUX_BASE + 0x1C)
#define IO_MUX_GPIO33_REG           (IO_MUX_BASE + 0x20)
#define IO_MUX_GPIO25_REG           (IO_MUX_BASE + 0x24)
#define IO_MUX_GPIO26_REG           (IO_MUX_BASE + 0x28)
#define IO_MUX_GPIO27_REG           (IO_MUX_BASE + 0x2C)
#define IO_MUX_MTMS_REG             (IO_MUX_BASE + 0x30)    // GPIO14
#define IO_MUX_MTDI_REG             (IO_MUX_BASE + 0x34)    // GPIO12
#define IO_MUX_MTCK_REG             (IO_MUX_BASE + 0x38)    // GPIO13
#define IO_MUX_MTDO_REG             (IO_MUX_BASE + 0x3C)    // GPIO15
#define IO_MUX_GPIO2_REG            (IO_MUX_BASE + 0x40)
#define IO_MUX_GPIO0_REG            (IO_MUX_BASE + 0x44)
#define IO_MUX_GPIO4_REG            (IO_MUX_BASE + 0x48)
#define IO_MUX_GPIO16_REG           (IO_MUX_BASE + 0x4C)
#define IO_MUX_GPIO17_REG           (IO_MUX_BASE + 0x50)
#define IO_MUX_SD_DATA2_REG         (IO_MUX_BASE + 0x54)    // GPIO9  (flash, do not use)
#define IO_MUX_SD_DATA3_REG         (IO_MUX_BASE + 0x58)    // GPIO10 (flash, do not use)
#define IO_MUX_SD_CMD_REG           (IO_MUX_BASE + 0x5C)    // GPIO11 (flash, do not use)
#define IO_MUX_SD_CLK_REG           (IO_MUX_BASE + 0x60)    // GPIO6  (flash, do not use)
#define IO_MUX_SD_DATA0_REG         (IO_MUX_BASE + 0x64)    // GPIO7  (flash, do not use)
#define IO_MUX_SD_DATA1_REG         (IO_MUX_BASE + 0x68)    // GPIO8  (flash, do not use)
#define IO_MUX_GPIO5_REG            (IO_MUX_BASE + 0x6C)
#define IO_MUX_GPIO18_REG           (IO_MUX_BASE + 0x70)
#define IO_MUX_GPIO19_REG           (IO_MUX_BASE + 0x74)
#define IO_MUX_GPIO20_REG           (IO_MUX_BASE + 0x78)
#define IO_MUX_GPIO21_REG           (IO_MUX_BASE + 0x7C)
#define IO_MUX_GPIO22_REG           (IO_MUX_BASE + 0x80)
#define IO_MUX_U0RXD_REG            (IO_MUX_BASE + 0x84)    // GPIO3  (UART0 RX, serial monitor)
#define IO_MUX_U0TXD_REG            (IO_MUX_BASE + 0x88)    // GPIO1  (UART0 TX, serial monitor)
#define IO_MUX_GPIO23_REG           (IO_MUX_BASE + 0x8C)
#define IO_MUX_GPIO24_REG           (IO_MUX_BASE + 0x90)

/*===============================================================================================
 * TRM v4.6: Chapter 4.32 "GPIO_FUNCn_OUT_SEL_CFG_REG"
 *===============================================================================================*/

#define GPIO_FUNC_OUT_SEL_S         0
#define GPIO_FUNC_OUT_SEL_M         (0x1FFUL << GPIO_FUNC_OUT_SEL_S) /* bits 8:0 */
#define GPIO_FUNC_OUT_SEL_GPIO      0x100           /*  256: drive pin from GPIO_OUT / GPIO_ENABLE*/
#define GPIO_FUNC_OUT_INV_SEL       (1UL << 9)
#define GPIO_FUNC_OEN_SEL           (1UL << 10)
#define GPIO_FUNC_OEN_INV_SEL       (1UL << 11)

/* --- GPIO_PINn_PAD_DRIVER --- */
#define GPIO_PIN_PAD_DRIVER         (1UL << 2)      /* 1: Open drain, 0: push-pull */

/* --- Pin capabilities --- */
#define GPIO_FIRST_INPUT_ONLY_PIN   34              /* GPIO34 ... 39 Have no output driver nor pulls */

/*===============================================================================================
 * TRM v4.6: Chapter 4.34 "IO_MUX_x_REG fields"
 *===============================================================================================*/
#define IO_MUX_FUN_WPD              (1UL << 7)      /* pull-down enable */
#define IO_MUX_FUN_WPU              (1UL << 8)      /* pull-up enable   */
#define IO_MUX_FUN_IE               (1UL << 9)      /* input enable     */

#define IO_MUX_FUN_DRV_S            10
#define IO_MUX_FUN_DRV_M            (0x3UL << IO_MUX_FUN_DRV_S)     /* bits 11:10 */

#define IO_MUX_MCU_SEL_S            12
#define IO_MUX_MCU_SEL_M            (0x7UL << IO_MUX_MCU_SEL_S)     /* bits 14:12 */
#define IO_MUX_MCU_SEL_GPIO         2

#define GPIO_PIN_COUNT              40

#endif