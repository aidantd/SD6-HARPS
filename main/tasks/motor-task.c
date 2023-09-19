// Includes
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "utility/timers/timers.h"

// External Dependencies

// Declarations

static uint64_t shutterTimeout = 0;

void motor_task(void *pvParameter) {
    while (1) {
        // TODO: Implement logic to determine if the shutters need to be updated
        // if (needToUpdateShutters() == true) {
        //     if (getShutterStatus() == SHUTTER_STATUS_OPEN) {
        //         // TODO: Update the shutter status to closed
        //         // TODO: Update needToUpdateShutters to false
        //         setMotorDirection(FORWARD);
        //     } else if (getShutterStatus() == SHUTTER_STATUS_CLOSED) {
        //         // TODO: Update the shutter status to open
        //         // TODO: Update needToUpdateShutters to false
        //         setMotorDirection(BACKWARD);
        //     }
        // }
        if (shutterTimeout == 0) {
            shutterTimeout = createTimeout(1000 * 1000 * 5);  // 5 seconds
        }

        if (isTimeoutElapsed(shutterTimeout) == 1) {
            printf("Timeout elapsed\n");
            shutterTimeout = 0;
        }

        printf("Motor task\n");
        printf("Shutter time amount: %llu\n", shutterTimeout);
        printf("GPTimer count: %llu\n", getGPTimerCount());
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}