// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "utility/timers/timers.h"

// External Dependencies

// Declarations
#define SHUTTER_MOTOR_DURATION 1000 * 1000 * 3  // 3 seconds

static uint64_t shutterTimeout = 0;

void motor_task(void *pvParameter) {
    while (1) {
        // TODO: Implement logic to determine if the shutters need to be updated at this if statement
        if (true == true) {
            if (getShutterStatus() == SHUTTER_STATUS_OPEN) {
                // TODO: Update the shutter status to closed
                // TODO: Update needToUpdateShutters to false
                setMotorDirection(FORWARD);
                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
            } else if (getShutterStatus() == SHUTTER_STATUS_CLOSED) {
                // TODO: Update the shutter status to open
                // TODO: Update needToUpdateShutters to false
                setMotorDirection(BACKWARD);
                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
            }
        } else if (isMotorActive() == true && isTimeoutElapsed(shutterTimeout) == true) {
            setMotorDirection(STOPPED);
            shutterTimeout = 0;
        }
#ifdef DEBUG
// if (shutterTimeout == 0) {
//     shutterTimeout = createTimeout(1000 * 1000 * 5);  // 5 seconds
// }

// if (isTimeoutElapsed(shutterTimeout) == 1) {
//     printf("Timeout elapsed\n");
//     shutterTimeout = 0;
// }

// printf("Motor task\n");
// printf("Shutter time amount: %llu\n", shutterTimeout);
// printf("GPTimer count: %llu\n", getGPTimerCount());
#endif

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}