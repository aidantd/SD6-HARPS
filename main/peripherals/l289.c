// Includes
#include "l289.h"

// External Dependencies

// Declarations

// Currently motor2 pins are the only ones that will be used within our system
#define M1_FORWARD GPIO_NUM_27
#define M1_BACKWARD
#define M2_FORWARD GPIO_NUM_17
#define M2_BACKWARD GPIO_NUM_13

static uint8_t shutterPosition = SHUTTER_STATUS_OPEN;
static uint8_t motorRunning = false;

// ********************************************************************************
// Initializes the L289 motor driver
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initL289(void) {
    esp_err_t error = ESP_OK;

    error |= gpio_set_direction(M2_FORWARD, GPIO_MODE_OUTPUT);
    error |= gpio_set_direction(M2_BACKWARD, GPIO_MODE_OUTPUT);

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
        printf("Motor direction set to forward\n");
        printf("M2_FORWARD: %d\n", gpio_get_level(M2_FORWARD));
        printf("M2_BACKWARD: %d\n", gpio_get_level(M2_BACKWARD));
        break;
    case BACKWARD:
        error |= gpio_set_level(M2_FORWARD, 0);
        error |= gpio_set_level(M2_BACKWARD, 1);
        motorRunning = true;
        shutterPosition = SHUTTER_STATUS_OPEN;
        printf("Motor direction set to backward\n");
        printf("M2_FORWARD: %d\n", gpio_get_level(M2_FORWARD));
        printf("M2_BACKWARD: %d\n", gpio_get_level(M2_BACKWARD));
        break;
    case STOPPED:
    default:
        error |= gpio_set_level(M2_FORWARD, 0);
        error |= gpio_set_level(M2_BACKWARD, 0);
        motorRunning = false;
        printf("Motor direction set to stopped\n");
        printf("M2_FORWARD: %d\n", gpio_get_level(M2_FORWARD));
        printf("M2_BACKWARD: %d\n", gpio_get_level(M2_BACKWARD));
        break;
    }

    printf("Error: %d\n", error);
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
