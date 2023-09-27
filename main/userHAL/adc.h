// Includes
#include <stdio.h>
#include <stdlib.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_err.h"
#include "hal/gpio_types.h"

// External Depencencies

// Declarations

esp_err_t configureADC(void);
uint32_t readADC(adc1_channel_t channel);