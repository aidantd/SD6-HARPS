// Includes
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "utility/timers/timers.h"

// External Dependencies

// Declarations

// TODO: Determine if this is the correct duration
#define MICROSECONDS_PER_MILLISECONDS 1000
#define MILLISECONDS_PER_SECOND 1000
#define SHUTTER_MOTOR_DURATION (1 * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECONDS)

static uint64_t shutterTimeout = 0;
static int firstTest = 1;
static int init = 0;

// ********************************************************************************
// ********************************************************************************
void motor_task(void *pvParameter) {
    while (1) {
        // setMotorDirection(BACKWARD);
        esp_err_t error;

        if (init == 0) {
            gpio_config_t io_conf = {
                .pin_bit_mask = (1ULL << GPIO_NUM_27),
                .mode = GPIO_MODE_OUTPUT,
                .intr_type = GPIO_INTR_DISABLE,
                .pull_down_en = 0,
                .pull_up_en = 1,
            };
            ESP_ERROR_CHECK(gpio_config(&io_conf));
            // ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_27));
            init = 1;
        }

        printf("GPIO Level: %d\n", gpio_get_level(GPIO_NUM_27));
        if (firstTest == 1) {
            // esp_rom_gpio_pad_select_gpio(GPIO_NUM_27);
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_27, 1));
            printf("Should be going high\n");
            // ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_27));
            firstTest = 0;
        } else {
            // esp_rom_gpio_pad_select_gpio(GPIO_NUM_27);
            // ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_27, 0));
            // ESP_ERROR_CHECK(gpio_reset_pin(GPIO_NUM_27));
            printf("Should be going low\n");
            firstTest = 1;
        }

        // TODO: Implement logic (needToUpdateShutters) to determine if the shutters need to be updated at this if statement
        // if (1) {
        //     if (firstTest == 1) {
        //         // TODO: Update needToUpdateShutters to false
        //         setMotorDirection(FORWARD);
        //         firstTest = 0;
        //         shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
        //     } else if (getShutterStatus() == SHUTTER_STATUS_CLOSED) {
        //         // TODO: Update needToUpdateShutters to false
        //         setMotorDirection(BACKWARD);
        //         shutterTimeout = createTimeout(SHUTTER_MOTOR_DURATION);
        //     }
        // } else if (isMotorActive() == true && isTimeoutElapsed(shutterTimeout) == true) {
        //     setMotorDirection(STOPPED);
        //     shutterTimeout = 0;
        // }
#ifndef DEBUG
        printf("Motor task\n");
        printf("Shutter time amount: %llu\n", shutterTimeout);
        printf("GPTimer count: %llu\n", getGPTimerCount());
#endif

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}