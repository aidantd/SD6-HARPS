// Includes
#include "bme280.h"

// External Dependencies

// Declarations

esp_err_t readFromBME(uint8_t *buffer, size_t size) {
    return i2c_read_from_device(BME280_ADDRESS, buffer, size);
}

esp_err_t writeToBME(uint8_t *data, size_t size) {
    return i2c_write_to_device(BME280_ADDRESS, data, size);
}