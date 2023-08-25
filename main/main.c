// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "userHAL/i2c.h"

// External Dependencies
extern void hello_task(void *pvParameter);

// Declarations

esp_err_t boardInit(void) {
    esp_err_t error = ESP_OK;

    error |= i2c_master_init();

    return error;
}

void app_main(void) {
    boardInit();

    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
}
