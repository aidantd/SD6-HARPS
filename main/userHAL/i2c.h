// Includes
#include <stdio.h>

#include "driver/i2c.h"
#include "esp_err.h"
#include "hal/gpio_types.h"

// External Dependencies

// Declarations

esp_err_t i2c_master_init(void);
esp_err_t i2c_write_to_device(uint8_t deviceAddress, uint8_t *data, size_t size);
esp_err_t i2c_read_from_device(uint8_t deviceAddress, uint8_t *buffer, size_t size);