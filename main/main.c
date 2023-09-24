// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "userHAL/i2c.h"
#include "userHAL/uart.h"
#include "utility/timers/timers.h"

// External Dependencies
extern void weatherApiTask(void* pvParameter);
extern void pt_task(void* pvParameter);
extern void motor_task(void* pvParameter);

extern esp_err_t wifi_init(void);

// Declarations

esp_err_t boardInit(void) {
    esp_err_t error = ESP_OK;

    error |= i2c_master_init();

    error |= uart_master_init();

    error |= wifi_init();

    error |= initGPTimer();

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
    xTaskCreate(&weatherApiTask, "weatherAPI", 4096, NULL, 5, NULL);
    xTaskCreate(&motor_task, "motor_task", 1024, NULL, 5, NULL);
}
