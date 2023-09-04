// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// External Dependencies

// Declarations

static int num = 0;

void hello_task(void *pvParameter) {
    while (1) {
        printf("Hello world!\n");
        printf("This is the start of the HARPS project.\n");
        printf("I = %d\n\n", num);
        num++;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}