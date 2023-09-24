idf_component_register(SRCS "main.c" 
                        "peripherals/bme280.c"
                        "peripherals/sen0170.c"
                        "tasks/weatherAPI-task.c"
                        "tasks/pt-task.c"
                        "userHal/i2c.c"
                        "userHal/uart.c"
                        "userHal/adc.c"
                        INCLUDE_DIRS ".")
