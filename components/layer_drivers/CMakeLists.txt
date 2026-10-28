idf_component_register( SRCS "drv_gpio.c" "drv_timer.c"
                        INCLUDE_DIRS "include"
                        REQUIRES common
                        PRIV_REQUIRES layer_hal)