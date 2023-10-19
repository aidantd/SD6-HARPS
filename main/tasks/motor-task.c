// Includes
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "userHAL/dac.h"
#include "utility/audio/audioFile.h"
#include "utility/timers/timers.h"

// External Dependencies

// Declarations

// TODO: Determine if this is the correct duration
#define MICROSECONDS_PER_MILLISECONDS 1000
#define MILLISECONDS_PER_SECOND 1000
#define SHUTTER_MOTOR_DURATION (6 * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECONDS)

static uint64_t shutterTimeout = 0;

// ********************************************************************************
// ********************************************************************************
void motorTask(void *pvParameter) {
    while (1) {
        if (isNeedToUpdateShutterPosition() == true && isMotorActive() == false) {
            if (getShutterStatus() == SHUTTER_STATUS_CLOSED) {
                startDacCosinSignal();

                setMotorDirection(BACKWARD);
                setNeedToUpdateShutterPosition(false);

                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
#ifndef DEBUG
                printf("Motor direction set to forward and speaker turned on\n");
#endif
            } else if (getShutterStatus() == SHUTTER_STATUS_OPEN) {
                startDacCosinSignal();

                setMotorDirection(FORWARD);
                setNeedToUpdateShutterPosition(false);

                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
#ifndef DEBUG
                printf("Motor direction set to backward\n");
#endif
            }
        } else if (isMotorActive() == true && isTimeoutElapsed(shutterTimeout) == true) {
            stopDacCosinSignal();
            setMotorDirection(STOPPED);
            shutterTimeout = 0;
#ifndef DEBUG
            printf("Motor direction set to stopped\n");
#endif
        }
#ifdef DEBUG
        printf("Motor task\n");
        printf("Shutter time amount: %llu\n", shutterTimeout);
        printf("GPTimer count: %llu\n", getGPTimerCount());
#endif

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}