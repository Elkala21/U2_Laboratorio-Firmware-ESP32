#ifndef TIMER_REGS_H
#define TIMER_REGS_H

#include "reg_access.h"

/* =====================================================================
 * Timer Group (TIMG)
 * TRM v4.6: Chapter 18.3 "Register Summary" and 18.4 "Registers"
 * ===================================================================== */
#define TIMG0_BASE                  0x3FF5F000UL
#define TIMG1_BASE                  0x3FF60000UL
#define TIMG_BASE(g)                (TIMG0_BASE + ((g) * (TIMG1_BASE - TIMG0_BASE)))

#define TIMG_TIMER_STRIDE           0x24     /* distance between timer 0 and timer 1 */
#define TIMG_T_BASE(g, t)           (TIMG_BASE(g) + (t) * TIMG_TIMER_STRIDE)

/* --- Configuration and control registers --- */
#define TIMG_CONFIG_REG(g, t)       (TIMG_T_BASE(g, t) + 0x00)
#define TIMG_LO_REG(g, t)           (TIMG_T_BASE(g, t) + 0x04)
#define TIMG_HI_REG(g, t)           (TIMG_T_BASE(g, t) + 0x08)
#define TIMG_UPDATE_REG(g, t)       (TIMG_T_BASE(g, t) + 0x0C)
#define TIMG_ALARMLO_REG(g, t)      (TIMG_T_BASE(g, t) + 0x10)
#define TIMG_ALARMHI_REG(g, t)      (TIMG_T_BASE(g, t) + 0x14)
#define TIMG_LOADLO_REG(g, t)       (TIMG_T_BASE(g, t) + 0x18)
#define TIMG_LOADHI_REG(g, t)       (TIMG_T_BASE(g, t) + 0x1C)
#define TIMG_LOAD_REG(g, t)         (TIMG_T_BASE(g, t) + 0x20)

/* --- System Watchdog timer configuration --- */
#define TIMG_WDTCONFIG0_REG(g)      (TIMG_BASE(g) + 0x0048)
#define TIMG_WDTCONFIG1_REG(g)      (TIMG_BASE(g) + 0x004C)
#define TIMG_WDTCONFIG2_REG(g)      (TIMG_BASE(g) + 0x0050)
#define TIMG_WDTCONFIG3_REG(g)      (TIMG_BASE(g) + 0x0054)
#define TIMG_WDTCONFIG4_REG(g)      (TIMG_BASE(g) + 0x0058)
#define TIMG_WDTCONFIG5_REG(g)      (TIMG_BASE(g) + 0x005C)
#define TIMG_WDTFEED_REG(g)         (TIMG_BASE(g) + 0x0060)
#define TIMG_WDTWPROTECT_REG(g)     (TIMG_BASE(g) + 0x0064)

/* --- Configuration and control register for RTC CALI --- */
#define TIMG_RTCCALICFG_REG(g)      (TIMG_BASE(g) + 0x0068)
#define TIMG_RTCCALICFG1_REG(g)     (TIMG_BASE(g) + 0x006C)

/* --- Interrupt registers --- */
#define TIMG_INT_ENA_REG(g)         (TIMG_BASE(g) + 0x0098)
#define TIMG_INT_RAW_REG(g)         (TIMG_BASE(g) + 0x009C)
#define TIMG_INT_ST_REG(g)          (TIMG_BASE(g) + 0x00A0)
#define TIMG_INT_CLR_REG(g)         (TIMG_BASE(g) + 0x00A4)

/* --- Bits of the interrupt registers (INT_ENA / RAW / ST / CLR) --- */
#define TIMG_INT_T_BIT(t)           (1UL << (t))    /* t = 0 ... 1 */
#define TIMG_INT_WDT_BIT            (1UL << 2)

/* --- Bits of TIMG_CONFIG_REG --- */
#define TIMG_EN                     (1UL << 31)
#define TIMG_INCREASE               (1UL << 30)
#define TIMG_AUTORELOAD             (1UL << 29)
#define TIMG_EDGE_INT_EN            (1UL << 12)
#define TIMG_LEVEL_INT_EN           (1UL << 11)
#define TIMG_ALARM_EN               (1UL << 10)

/* --- DIVIDER field (bits 28 ... 13, 16 bits) --- */
#define TIMG_DIVIDER_S              13
#define TIMG_DIVIDER_V              0xFFFFUL                            /* mask before shifting */
#define TIMG_DIVIDER_M              (TIMG_DIVIDER_V << TIMG_DIVIDER_S)  /* mask in register position */

#endif /* TIMER_REGS_H */