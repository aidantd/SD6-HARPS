idf_component_register(SRCS "main.c" 
                        "peripherals/bme280.c"
                        "peripherals/l289.c"
                        "peripherals/sen0170.c"
                        "tasks/anemometer-task.c"
                        "tasks/decisionMaking-task.c"
                        "tasks/motor-task.c"
                        "tasks/pt-task.c"
                        "tasks/weatherAPI-task.c"
                        "userHal/adc.c"
                        "userHal/dac.c"
                        "userHal/i2c.c"
                        "userHal/uart.c"
                        "utility/timers/timers.c"
                        INCLUDE_DIRS ".")

target_compile_definitions(${COMPONENT_LIB} PRIVATE SENSORS)
