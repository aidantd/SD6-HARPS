// // Includes
#include <stdlib.h>

#include "driver/gpio.h"
#include "esp_err.h"

// External Dependencies

// Declarations

#define LED_RED GPIO_NUM_26
#define LED_GREEN GPIO_NUM_14

esp_err_t initLeds(void);