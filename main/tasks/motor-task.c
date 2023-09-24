// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "utility/timers/timers.h"

// External Dependencies

// Declarations

// TODO: Determine if this is the correct duration
#define MICROSECONDS_PER_MILLISECONDS 1000
#define MILLISECONDS_PER_SECOND 1000
#define SHUTTER_MOTOR_DURATION (3 * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECONDS)

static uint64_t shutterTimeout = 0;

void motor_task(void *pvParameter) {
    while (1) {
        // TODO: Implement logic (needToUpdateShutters) to determine if the shutters need to be updated at this if statement
        if (false == true) {
            if (getShutterStatus() == SHUTTER_STATUS_OPEN) {
                // TODO: Update needToUpdateShutters to false
                setMotorDirection(FORWARD);
                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
            } else if (getShutterStatus() == SHUTTER_STATUS_CLOSED) {
                // TODO: Update needToUpdateShutters to false
                setMotorDirection(BACKWARD);
                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
            }
        } else if (isMotorActive() == true && isTimeoutElapsed(shutterTimeout) == true) {
            setMotorDirection(STOPPED);
            shutterTimeout = 0;
        }
#ifdef DEBUG
// printf("Motor task\n");
// printf("Shutter time amount: %llu\n", shutterTimeout);
// printf("GPTimer count: %llu\n", getGPTimerCount());
#endif

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}