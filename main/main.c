// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// External Dependencies
extern void hello_task(void *pvParameter);

// Declarations

void app_main(void) {
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
}
