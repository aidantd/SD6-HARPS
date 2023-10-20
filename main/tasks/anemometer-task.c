// Includes
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/sen0170.h"

// External Depencencies

// Declarations

static uint32_t lastRecordedWindSpeedMPH;

// ********************************************************************************
// Gets the last recorded wind speed
// @return: The last recorded wind speed as a 32-bit unsigned integer
// ********************************************************************************
uint32_t getLastRecordedWindSpeedMPH() {
    return lastRecordedWindSpeedMPH;
}

// ********************************************************************************
// ********************************************************************************
void anemometerTask(void* pvParameter) {
    while (1) {
        lastRecordedWindSpeedMPH = readSen0170();

#ifdef DEMO
        printf("Wind Speed: %ld\n", lastRecordedWindSpeedMPH);
#endif
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}