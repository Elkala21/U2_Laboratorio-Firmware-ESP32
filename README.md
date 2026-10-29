# U2 Laboratorio · Layered Bare-Metal Firmware for ESP32
 
Bare-metal firmware for the ESP32 built on ESP-IDF, accessing the GPIO, IO_MUX and Timer Group
peripherals directly through their registers (no ESP-IDF GPIO/timer drivers). The code is split
into layers so that the application never touches a register or a pin number directly.
 
Software Embebido · Instituto Tecnológico del Estado de Chihuahua (ITECH)
 
---
 
## Architecture
 
Each box is an ESP-IDF component (a layer). Arrows are `#include` dependencies between files;
dashed arrows are direct access to the chip's hardware.
 
```mermaid
flowchart TB
    subgraph APP["main/ · Application"]
        main_c["main.c"]
        app_c["app.c"]
        app_h["app.h"]
    end
 
    subgraph BSP["layer_bsp · Board Support Package"]
        bsp_c["bsp.c"]
        bsp_h["bsp.h"]
    end
 
    subgraph DRV["layer_drivers · Drivers (public API)"]
        drv_gpio_c["drv_gpio.c"]
        drv_gpio_h["drv_gpio.h"]
        drv_timer_c["drv_timer.c"]
        drv_timer_h["drv_timer.h"]
    end
 
    subgraph HAL["layer_hal · Hardware Abstraction Layer"]
        hal_gpio_c["hal_gpio.c"]
        hal_gpio_h["hal_gpio.h"]
        hal_timer_c["hal_timer.c"]
        hal_timer_h["hal_timer.h"]
        subgraph REGS["regs/ · private register map"]
            gpio_regs_h["gpio_regs.h"]
            timer_regs_h["timer_regs.h"]
            reg_access_h["reg_access.h<br/>REG32()"]
        end
    end
 
    subgraph COMMON["common · shared definitions"]
        fw_err_h["fw_err.h"]
    end
 
    subgraph HW["ESP32 hardware"]
        hw_gpio[("GPIO · IO_MUX<br/>0x3FF44000 · 0x3FF49000")]
        hw_timg[("TIMG0 · TIMG1<br/>0x3FF5F000 · 0x3FF60000")]
    end
 
    %% Invisible links: only keep the layers stacked top to bottom
    app_h ~~~ bsp_c
    bsp_h ~~~ drv_gpio_c
    drv_gpio_h ~~~ hal_gpio_c
    drv_timer_h ~~~ hal_timer_c
    reg_access_h ~~~ hw_gpio
 
    %% Application
    main_c --> app_h
    app_c --> app_h
    app_c --> bsp_h
    app_c --> drv_gpio_h
    app_c --> drv_timer_h
 
    %% BSP
    bsp_c --> bsp_h
    bsp_c --> drv_gpio_h
    bsp_c --> drv_timer_h
 
    %% Drivers
    drv_gpio_c --> drv_gpio_h
    drv_gpio_c --> hal_gpio_h
    drv_timer_c --> drv_timer_h
    drv_timer_c --> hal_timer_h
 
    %% HAL
    hal_gpio_c --> hal_gpio_h
    hal_gpio_c --> gpio_regs_h
    hal_timer_c --> hal_timer_h
    hal_timer_c --> timer_regs_h
    gpio_regs_h --> reg_access_h
    timer_regs_h --> reg_access_h
 
    %% Common error codes (used in the public API of drivers and BSP)
    drv_gpio_h --> fw_err_h
    drv_timer_h --> fw_err_h
    bsp_h --> fw_err_h
 
    %% Hardware access
    hal_gpio_c -. "REG32()" .-> hw_gpio
    hal_timer_c -. "REG32()" .-> hw_timg
 
    classDef src fill:#e8f0fe,stroke:#4a6fb5,color:#1a1a1a
    classDef hdr fill:#fff4e0,stroke:#c98a1b,color:#1a1a1a
    classDef hw fill:#eeeeee,stroke:#666666,color:#1a1a1a
    class main_c,app_c,bsp_c,drv_gpio_c,drv_timer_c,hal_gpio_c,hal_timer_c src
    class app_h,bsp_h,drv_gpio_h,drv_timer_h,hal_gpio_h,hal_timer_h,gpio_regs_h,timer_regs_h,reg_access_h,fw_err_h hdr
    class hw_gpio,hw_timg hw
```
 
Blue boxes are source files (`.c`), orange boxes are headers (`.h`).
 
### Layers
 
| Layer | Responsibility | Touches registers? |
|---|---|---|
| **App** (`main/`) | Example application: the APAGADO / STANDBY / RUNNING state machine | No |
| **BSP** (`layer_bsp`) | Knows the board: which pin is each LED and button, and initializes them | No |
| **Drivers** (`layer_drivers`) | General API: validates parameters, returns error codes, decides the order of operations | No |
| **HAL** (`layer_hal`) | One small function per hardware operation, no validation | Yes, only here |
| **regs/** (inside HAL) | Base addresses, offsets, bits and masks from the ESP32 TRM v4.6 | — |
| **common** | Project-wide error type and codes (`fw_err_t`), currently backed by ESP-IDF's `esp_err_t` | No |
 
Rule of thumb: *how it is done on this chip* goes in the HAL; *whether it should be done* goes in
the driver.
 
### Dependency rules (enforced by CMake)
 
```
main           ──REQUIRES──▶  layer_bsp, layer_drivers, common
layer_bsp      ──REQUIRES──▶  common          ──PRIV_REQUIRES──▶  layer_drivers
layer_drivers  ──REQUIRES──▶  common          ──PRIV_REQUIRES──▶  layer_hal
layer_hal      ──PRIV_INCLUDE_DIRS──▶  regs/
common         ──REQUIRES──▶  esp_common (ESP-IDF)
```
 
Because of `PRIV_REQUIRES` and `PRIV_INCLUDE_DIRS`, a file can only include headers from the layer
directly below it. Including `gpio_regs.h` or `hal_gpio.h` from the application fails at compile time.
`common` is required publicly (`REQUIRES`) because `fw_err_t` appears in the public headers of the
drivers and the BSP.
 
---
 
## Repository structure
 
```
.
├── CMakeLists.txt
├── components/
│   ├── common/
│   │   └── include/        fw_err.h
│   ├── layer_hal/
│   │   ├── hal_gpio.c
│   │   ├── hal_timer.c
│   │   ├── include/        hal_gpio.h, hal_timer.h
│   │   └── regs/           reg_access.h, gpio_regs.h, timer_regs.h (private)
│   ├── layer_drivers/
│   │   ├── drv_gpio.c
│   │   ├── drv_timer.c
│   │   └── include/        drv_gpio.h, drv_timer.h
│   └── layer_bsp/
│       ├── bsp.c
│       └── include/        bsp.h
└── main/
    ├── main.c
    ├── app.c
    └── app.h
```
 
---
 
## Build
 
Requires ESP-IDF (VS Code ESP-IDF extension or `idf.py`), target `esp32`.
 
```bash
idf.py set-target esp32
idf.py build flash monitor
```
 
---
 
## References
 
- Espressif Systems, *ESP32 Technical Reference Manual*, v4.6 — Ch. 4 (IO_MUX and GPIO Matrix), Ch. 18 (Timer Group)
