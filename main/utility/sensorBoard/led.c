// Includes
#include "led.h"

// External Dependencies

// Declarations

// ********************************************************************************
// Setup LED pins as outputs
// @returns: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initLeds(void) {
    esp_err_t error = ESP_OK;

    gpio_config_t gpioConf = {
        .pin_bit_mask = (1ULL << LED_RED),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en = 1,
    };
    error |= gpio_config(&gpioConf);

    gpio_config_t gpioConf2 = {
        .pin_bit_mask = (1ULL << LED_GREEN),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en = 1,
    };

    error |= gpio_config(&gpioConf2);

    return error;
}