// Includes
#include "l289.h"

// External Dependencies

// Declarations

// Currently motor2 pins are the only ones that will be used within our system
#define M1_FORWARD GPIO_NUM_27
#define M1_BACKWARD
#define M2_FORWARD GPIO_NUM_17
#define M2_BACKWARD GPIO_NUM_16

static uint8_t shutterPosition = SHUTTER_STATUS_OPEN;
static uint8_t motorRunning = false;
static uint8_t needToUpdateShutters = false;

// ********************************************************************************
// Initializes the L289 motor driver
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initL289(void) {
    esp_err_t error = ESP_OK;

    gpio_config_t gpioConf = {
        .pin_bit_mask = (1ULL << M2_FORWARD),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en = 1,
    };
    error = gpio_config(&gpioConf);

    gpio_config_t gpioConf2 = {
        .pin_bit_mask = (1ULL << M2_BACKWARD),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en = 1,
    };

    error = gpio_config(&gpioConf2);

    return error;
}

// ********************************************************************************
// Sets the direction of the motor and starts the motor
// @param motorDirection: The direction to set the motor to
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
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

// ********************************************************************************
// Gets the current shutter status
// @return: The current shutter status
// ********************************************************************************
uint8_t getShutterStatus(void) {
    return shutterPosition;
}

// ********************************************************************************
// Sets the current shutter status
// @param shutterStatus: The shutter status to set
// ********************************************************************************
void setShutterStatus(uint8_t shutterStatus) {
    shutterPosition = shutterStatus;
}

// ********************************************************************************
// Gets whether or not the motor is currently active
// @return: Whether or not the motor is currently active
// ********************************************************************************
uint8_t isMotorActive(void) {
    return motorRunning;
}

// ********************************************************************************
// Gets wether or not the systems shutters need to be updated
// @return: Whether or not the systems shutters need to be updated
// ********************************************************************************
uint8_t isNeedToUpdateShutterPosition(void) {
    return needToUpdateShutters;
}

// ********************************************************************************
// Sets whether or not the systems shutters need to be updated
// @param updateShutterStatus: Whether or not the systems shutters need to be updated
// ********************************************************************************
void setNeedToUpdateShutterPosition(uint8_t updateShutterStatus) {
    needToUpdateShutters = updateShutterStatus;
}