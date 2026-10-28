idf_component_register( SRCS "bsp.c"
                        INCLUDE_DIRS "include"
                        REQUIRES common
                        PRIV_REQUIRES layer_drivers)