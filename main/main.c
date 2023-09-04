// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "userHAL/i2c.h"
#include "userHAL/uart.h"

// External Dependencies
extern void hello_task(void* pvParameter);
void weatherApiTask(void* pvParameter);
extern esp_err_t wifi_init(void);

// Declarations

esp_err_t boardInit(void) {
    esp_err_t error = ESP_OK;

    error |= i2c_master_init();

    error |= uart_master_init();

    error |= wifi_init();

    return error;
}

void app_main(void) {
    if (boardInit() != ESP_OK) {
        printf("Error initializing board\n");

        return;
    } else {
        printf("Board initialized successfully\n");
    }

    xTaskCreate(&weatherApiTask, "weatherAPI", 4096, NULL, 5, NULL);
}
