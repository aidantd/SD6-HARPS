// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "userHAL/adc.h"
#include "userHAL/dac.h"
#include "userHAL/i2c.h"
#include "userHAL/uart.h"
#include "utility/sensorBoard/led.h"
#include "utility/timers/timers.h"

// External Dependencies
extern void weatherApiTask(void* pvParameter);
extern void pt_task(void* pvParameter);
extern void motorTask(void* pvParameter);
extern void anemometerTask(void* pvParameter);
extern void decisionMakingTask(void* pvParameter);
extern void screenTask(void* pvParameter);

// Declarations

esp_err_t boardInit(void) {
    esp_err_t error = ESP_OK;

    error |= i2c_master_init();

    error |= uart_master_init();

    error |= configureADC();

    error |= initDAC();

    error |= initGPTimer();

    error |= initL289();

    error |= initLeds();

    return error;
}

void app_main(void) {
    if (boardInit() != ESP_OK) {
        printf("Error initializing board\n");

        return;
    } else {
        printf("Board initialized successfully\n");
    }

    xTaskCreate(&pt_task, "pt_task", 2048, NULL, 5, NULL);
    xTaskCreate(&weatherApiTask, "weatherAPI", 8192, NULL, 3, NULL);
    xTaskCreate(&motorTask, "motorTask", 2048, NULL, 2, NULL);
    xTaskCreate(&anemometerTask, "anemometer_task", 2048, NULL, 4, NULL);
    xTaskCreate(&decisionMakingTask, "decisionMakingTask", 2048, NULL, 1, NULL);
    xTaskCreate(&screenTask, "screenTask", 8192, NULL, 5, NULL);
}
