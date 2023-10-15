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
#define WIND_SPEED_ACTIVATION_THRESHOLD_MPH 30

// Deactivation Thresholds
#define TEMPERATURE_DEACTIVATION_THRESHOLD_F 65
#define PRESSURE_DEACTIVATION_THRESHOLD_MB 1010
#define WIND_SPEED_DEACTIVATION_THRESHOLD_MPH 20

// ********************************************************************************
// ********************************************************************************
void decisionMakingTask(void *pvParameter) {
    while (1) {
        // Check shutter status and determine if the system needs to update the shutters
        switch (getShutterStatus()) {
        case SHUTTER_STATUS_OPEN:
            if (getLastRecordedWindSpeedMPH >= WIND_SPEED_ACTIVATION_THRESHOLD_MPH) {
                setShutterStatus(SHUTTER_STATUS_CLOSED);
                setNeedToUpdateShutterPosition(true);
            }
            break;
        case SHUTTER_STATUS_CLOSED:
            if (getLastRecordedWindSpeedMPH <= WIND_SPEED_DEACTIVATION_THRESHOLD_MPH) {
                setShutterStatus(SHUTTER_STATUS_OPEN);
                setNeedToUpdateShutterPosition(true);
            }
            break;
        default:
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}