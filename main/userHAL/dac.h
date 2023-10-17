// Includes
#include <stdio.h>

#include "driver/dac_cosine.h"
#include "driver/gpio.h"
#include "esp_err.h"

// External Dependencies

// Declarations
esp_err_t initDAC(void);
void startDacCosinSignal(void);
void stopDacCosinSignal(void);