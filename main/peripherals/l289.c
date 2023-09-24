// Includes
#include "l289.h"

// External Dependencies

// Declarations

// Currently motor2 pins are the only ones that will be used within our system
#define M1_FORWARD GPIO_NUM_27
#define M1_BACKWARD
#define M2_FORWARD GPIO_NUM_17
#define M2_BACKWARD GPIO_NUM_13

// Currently the enable pins are not configurable by the chip unless we skyhook a wire
#define M1_ENABLE
#define M2_ENABLE

static uint8_t shutterPosition = SHUTTER_STATUS_OPEN;
static uint8_t motorRunning = false;

esp_err_t initL289(void) {
    esp_err_t error = ESP_OK;

    error |= gpio_set_direction(M2_FORWARD, GPIO_MODE_OUTPUT);
    error |= gpio_set_direction(M2_BACKWARD, GPIO_MODE_OUTPUT);

    return error;
}

esp_err_t setMotorDirection(motorDirection_t motorDirection) {
    esp_err_t error = ESP_OK;

    switch (motorDirection) {
    case FORWARD:
        error |= gpio_set_level(M2_FORWARD, 1);
        error |= gpio_set_level(M2_BACKWARD, 0);
        motorRunning = true;
        shutterPosition = SHUTTER_STATUS_CLOSED;
        break;
    case BACKWARD:
        error |= gpio_set_level(M2_FORWARD, 0);
        error |= gpio_set_level(M2_BACKWARD, 1);
        motorRunning = true;
        shutterPosition = SHUTTER_STATUS_OPEN;
        break;
    case STOPPED:
    default:
        error |= gpio_set_level(M2_FORWARD, 0);
        error |= gpio_set_level(M2_BACKWARD, 0);
        motorRunning = false;
        break;
    }

    return error;
}

uint8_t getShutterStatus(void) {
    return shutterPosition;
}

void setShutterStatus(uint8_t shutterStatus) {
    shutterPosition = shutterStatus;
}

uint8_t isMotorActive(void) {
    return motorRunning;
}
