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
// Checks if the activation threshold has been reached
// @param lastRecordedSpeed: last recorded wind speed in MPH
// @return: 1 if the activation threshold has been reached, 0 otherwise
// ********************************************************************************
static uint8_t isActivationThresholdReached(uint32_t lastRecordedSpeed) {
    if (lastRecordedSpeed >= WIND_SPEED_ACTIVATION_THRESHOLD_MPH) {
        return 1;
    }

    return 0;
}

// ********************************************************************************
// Checks if the deactivation threshold has been reached
// @param lastRecordedSpeed: last recorded wind speed in MPH
// @return: 1 if the activation threshold has been reached, 0 otherwise
// ********************************************************************************
static uint8_t isDeactivationThresholdReached(uint32_t lastRecordedSpeed) {
    if (lastRecordedSpeed <= WIND_SPEED_DEACTIVATION_THRESHOLD_MPH) {
        return 1;
    }

    return 0;
}

// ********************************************************************************
// ********************************************************************************
void decisionMakingTask(void *pvParameter) {
    while (1) {
        // Check shutter status and determine if the system needs to update the shutters
        uint32_t lastRecordedSpeed = getLastRecordedWindSpeedMPH();

        // Check if the activation threshold has been reached
        if (isActivationThresholdReached(lastRecordedSpeed) ||
            isDeactivationThresholdReached(lastRecordedSpeed)) {
#ifdef DEMO
            printf("\nThe computer has decided to update the shutter position\n");
#endif
            setNeedToUpdateShutterPosition(true);
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}