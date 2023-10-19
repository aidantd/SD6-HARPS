// Includes
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"

// External Dependencies
extern uint32_t getTemperature(void);
extern uint32_t getPressure(void);
extern uint32_t getHumidity(void);
extern uint32_t getLastRecordedWindSpeedMPH();

// Declarations

// Activation Thresholds
#define TEMPERATURE_ACTIVATION_THRESHOLD_F 70
#define PRESSURE_ACTIVATION_THRESHOLD_MB 1000
#define WIND_SPEED_ACTIVATION_THRESHOLD_MPH 150

// Deactivation Thresholds
#define TEMPERATURE_DEACTIVATION_THRESHOLD_F 65
#define PRESSURE_DEACTIVATION_THRESHOLD_MB 1010
#define WIND_SPEED_DEACTIVATION_THRESHOLD_MPH 100

// ********************************************************************************
// ********************************************************************************
void decisionMakingTask(void *pvParameter) {
    while (1) {
        // Check shutter status and determine if the system needs to update the shutters
        printf("Shutter Status: %d\n", getShutterStatus());
        uint32_t lastRecordedSpeed = getLastRecordedWindSpeedMPH();
        printf("Last Recorded Wind Speed: %ld\n", lastRecordedSpeed);
        switch (getShutterStatus()) {
        case SHUTTER_STATUS_OPEN:
            if (lastRecordedSpeed >= WIND_SPEED_ACTIVATION_THRESHOLD_MPH) {
                setNeedToUpdateShutterPosition(true);
#ifndef DEBUG
                printf("The computer has decided to close the shuttering system\n");
#endif
            }
            break;
        case SHUTTER_STATUS_CLOSED:
            if (lastRecordedSpeed <= WIND_SPEED_DEACTIVATION_THRESHOLD_MPH) {
                setNeedToUpdateShutterPosition(true);
#ifndef DEBUG
                printf("The computer has decided to open the shuttering system\n");
#endif
            }
            break;
        default:
            break;
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}