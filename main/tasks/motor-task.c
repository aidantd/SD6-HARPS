// Includes
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "userHAL/dac.h"
#include "utility/timers/timers.h"

// External Dependencies

// Declarations

// TODO: Determine if this is the correct duration
#define MICROSECONDS_PER_MILLISECONDS 1000
#define MILLISECONDS_PER_SECOND 1000
#define SHUTTER_MOTOR_DURATION (2 * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECONDS)

static uint64_t shutterTimeout = 0;
static int speakerTest = 0;

// ********************************************************************************
// ********************************************************************************
void motor_task(void *pvParameter) {
    while (1) {
        // TODO: Implement logic (needToUpdateShutters) to determine if the shutters need to be updated at this if statement
        if (1) {
            if (speakerTest == 0) {
                // Test the speaker
                setDacVoltage((uint8_t[]){0, 255, 0}, 3);
                speakerTest = 1;
            }
            if (getShutterStatus() == SHUTTER_STATUS_OPEN) {
                // TODO: Update needToUpdateShutters to false
                setMotorDirection(FORWARD);
#ifdef DEBUG
                printf("Motor direction set to forward\n");
#endif
                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
            } else if (getShutterStatus() == SHUTTER_STATUS_CLOSED) {
                // TODO: Update needToUpdateShutters to false
                setMotorDirection(BACKWARD);
#ifdef DEBUG
                printf("Motor direction set to backward\n");
#endif
                shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
            }
        } else if (isMotorActive() == true && isTimeoutElapsed(shutterTimeout) == true) {
            setMotorDirection(STOPPED);
#ifdef DEBUG
            printf("Motor direction set to stopped\n");
#endif
            shutterTimeout = 0;
        }
#ifdef DEBUG
        printf("Motor task\n");
        printf("Shutter time amount: %llu\n", shutterTimeout);
        printf("GPTimer count: %llu\n", getGPTimerCount());
#endif

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}