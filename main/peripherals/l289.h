// Includes
#include <stdio.h>

#include "/Users/aidan/esp/esp-idf/components/driver/mcpwm/include/driver/mcpwm_types.h"
#include "/Users/aidan/esp/esp-idf/components/driver/mcpwm/mcpwm_private.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "soc/mcpwm_periph.h"

// External Dependencies

// Declarations
#define SHUTTER_STATUS_OPEN 0
#define SHUTTER_STATUS_CLOSED 1

typedef enum motorDirection {
    FORWARD,
    BACKWARD,
} motorDirection_t;

esp_err_t initL289(void);
esp_err_t setMotorDirection(motorDirection_t motorDirection);