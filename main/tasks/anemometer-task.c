// Includes
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/sen0170.h"

// External Depencencies

// Declarations

static uint32_t lastRecordedWindSpeed = 0;

uint32_t getLastRecordedWindSpeed() {
    return lastRecordedWindSpeed;
}

void anemometerTask(void* pvParameter) {
    while (1) {
        lastRecordedWindSpeed = readSen0170();

#ifndef DEBUG
        printf("Wind Speed: %ld\n", lastRecordedWindSpeed);
#endif
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}