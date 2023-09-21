idf_component_register(SRCS "src/i2s_parallel.c"
                            "src/leddisplay.c"
                            "src/val2pwm.c"
                       REQUIRES driver
                       INCLUDE_DIRS "include")
