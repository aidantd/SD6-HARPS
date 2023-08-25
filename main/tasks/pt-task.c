// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// External Dependencies

// Declarations

static int num = 0;

void hello_task(void *pvParameter) {
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}