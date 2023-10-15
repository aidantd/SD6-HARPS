// Includes
#include <stdio.h>

#include "driver/dac.h"
#include "driver/gpio.h"
#include "esp_err.h"

// External Dependencies

// Declarations
esp_err_t initDAC(void);
void setDacVoltage(uint8_t *audio_data, size_t length);