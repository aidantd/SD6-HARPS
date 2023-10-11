// Includes
#include <stdio.h>

#include "driver/dac_oneshot.h"
#include "driver/gpio.h"
#include "esp_err.h"

// External Dependencies

// Declarations
esp_err_t initDAC(void);
esp_err_t setDACVoltage(uint8_t voltage);