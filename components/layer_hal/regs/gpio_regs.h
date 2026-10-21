#ifndef GPIO_REGS_H
#define GPIO_REGS_H

#include "reg_access.h"

/*===============================================================================================
 * GPIO peripherial
 * TRM: Chapter 4. "IO_MUX ang GPIO Matrix" -> Register Summary 
===============================================================================================*/
#define GPIO_BASE               0x3FF44000UL

/* --- Pins 0 ... 39 (one bit per pin: *_REG -> GPIO 0-31, *1_REG -> GPIO 32-39) --- */

#define GPIO_OUT_REG            (GPIO_BASE + 0x04)
#define GPIO_OUT_W1TS_REG       (GPIO_BASE + 0x08)
#define GPIO_OUT_W1TC_REG       (GPIO_BASE + 0x0C)
#define GPIO_OUT1_REG           (GPIO_BASE + 0x10)
#define GPIO_OUT1_W1TS_REG      (GPIO_BASE + 0x14)
#define GPIO_OUT1_W1TC_REG      (GPIO_BASE + 0x18)
#define GPIO_ENABLE_REG         (GPIO_BASE + 0x20)
#define GPIO_ENABLE_W1TS_REG    (GPIO_BASE + 0x24)
#define GPIO_ENABLE_W1TC_REG    (GPIO_BASE + 0x28)
#define GPIO_ENABLE1_REG        (GPIO_BASE + 0x2C)
#define GPIO_ENABLE1_W1TS_REG   (GPIO_BASE + 0x30)
#define GPIO_ENABLE1_W1TC_REG   (GPIO_BASE + 0x34)
#define GPIO_STRAP_REG          (GPIO_BASE + 0x38)
#define GPIO_IN_REG             (GPIO_BASE + 0x3C)
#define GPIO_IN1_REG            (GPIO_BASE + 0x40)

/* --- Interrupt status --- */

#define GPIO_STATUS_REG         (GPIO_BASE + 0x44)
#define GPIO_STATUS_W1TS_REG    (GPIO_BASE + 0x48)
#define GPIO_STATUS_W1TC_REG    (GPIO_BASE + 0x4C)
#define GPIO_STATUS1_REG        (GPIO_BASE + 0x50)
#define GPIO_STATUS1_W1TS_REG   (GPIO_BASE + 0x54)
#define GPIO_STATUS1_W1TC_REG   (GPIO_BASE + 0x58)
#define GPIO_ACPU_INT_REG       (GPIO_BASE + 0x60)
#define GPIO_ACPU_NMI_INT_REG   (GPIO_BASE + 0x64)
#define GPIO_PCPU_INT_REG       (GPIO_BASE + 0x68)
#define GPIO_PCPU_NMI_INT_REG   (GPIO_BASE + 0x6C)
#define GPIO_ACPU_INT1_REG      (GPIO_BASE + 0x74)
#define GPIO_ACPU_NMI_INT1_REG  (GPIO_BASE + 0x78)
#define GPIO_PCPU_INT1_REG      (GPIO_BASE + 0x7C)
#define GPIO_PCPU_NMI_INT1_REG  (GPIO_BASE + 0x80)

/* --- Per-pin / per-signal registers (n = index) --- */

#define GPIO_PIN_REG(n)                 (GPIO_BASE + 0x88  + ((n) * 4))    /* n = 0 ... 39  */
#define GPIO_FUNC_IN_SEL_CFG_REG(n)     (GPIO_BASE + 0x130 + ((n) * 4))    /* n = 0 ... 255 */
#define GPIO_FUNC_OUT_SEL_CFG_REG(n)    (GPIO_BASE + 0x530 + ((n) * 4))    /* n = 0 ... 39  */

#endif