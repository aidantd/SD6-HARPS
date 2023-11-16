// Includes
#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "utility/api/weatherAPI.h"
#include "utility/sensorBoard/led.h"

// External Dependencies
extern uint32_t getTemperature(void);
extern uint32_t getPressure(void);
extern uint32_t getHumidity(void);
extern uint32_t getLastRecordedWindSpeedMPH();
extern int getWeatherCondition(char* pCondition);
extern int getWeatherWindSpeedMPH(void);

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
void decisionMakingTask(void* pvParameter) {
    while (1) {
        // Check shutter status and determine if the system needs to update the shutters
        uint32_t lastRecordedSpeed = getLastRecordedWindSpeedMPH();

        int weatherApiWindSpeed = getWeatherWindSpeedMPH();

        char pCondition[MAX_FIELD_LENGTH];
        getWeatherCondition(pCondition);

        switch (getShutterStatus()) {
        case SHUTTER_STATUS_OPEN:
            gpio_set_level(LED_GREEN, 1);

            if (lastRecordedSpeed >= WIND_SPEED_ACTIVATION_THRESHOLD_MPH) {
                setNeedToUpdateShutterPosition(true);
#ifdef DEMO
                printf("\nThe computer has decided to close the shuttering system\n");
#endif
            } else if (getTemperature() >= TEMPERATURE_ACTIVATION_THRESHOLD_F && getPressure() <= PRESSURE_ACTIVATION_THRESHOLD_MB) {
                setNeedToUpdateShutterPosition(true);
#ifdef DEMO
                printf("\nThe computer has decided to open the shuttering system\n");
#endif
            } else if (memcmp(pCondition, "Torrential rain shower", 23) == 0 || weatherApiWindSpeed >= WIND_SPEED_ACTIVATION_THRESHOLD_MPH) {
                setNeedToUpdateShutterPosition(true);
            }

            break;
        case SHUTTER_STATUS_CLOSED:
            gpio_set_level(LED_RED, 1);

            if (lastRecordedSpeed <= WIND_SPEED_DEACTIVATION_THRESHOLD_MPH) {
                setNeedToUpdateShutterPosition(true);
#ifdef DEMO
                printf("\nThe computer has decided to open the shuttering system\n");
#endif
            } else if (getTemperature() <= TEMPERATURE_DEACTIVATION_THRESHOLD_F && getPressure() >= PRESSURE_DEACTIVATION_THRESHOLD_MB) {
                setNeedToUpdateShutterPosition(true);
#ifdef DEMO
                printf("\nThe computer has decided to open the shuttering system\n");
#endif
            } else if (memcmp(pCondition, "Torrential rain shower", 23) != 0 || weatherApiWindSpeed <= WIND_SPEED_DEACTIVATION_THRESHOLD_MPH) {
                setNeedToUpdateShutterPosition(true);
            }

            break;
        default:
            break;
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}