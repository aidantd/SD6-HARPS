// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "userHAL/i2c.h"
#include "userHAL/uart.h"

// External Dependencies
extern void hello_task(void *pvParameter);

// Declarations

esp_err_t boardInit(void) {
    esp_err_t error = ESP_OK;

    error |= i2c_master_init();

    error |= hal_master_init();

    if (error != ESP_OK) {
        printf("Error initializing board: %d\n", error);
    } else {
        printf("Board initialized successfully\n");
    }

    return error;
}

void app_main(void) {
    boardInit();

    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
}
