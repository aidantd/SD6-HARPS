// Includes
#include "bme280.h"

// External Dependencies

// Declarations
#define CONFIG_STANDBY_TIME_62_MS 0b00100000
#define CONFIG_FILTER_COEFFICIENT_8 0b00001100
#define CONFIG_SPI_DISABLE 0b00000000

esp_err_t readFromBME(uint8_t *buffer, uint8_t registerAddress, size_t size) {
    return i2c_read_from_device(BME280_ADDRESS, buffer, registerAddress, size);
}

esp_err_t writeToBME(uint8_t *data, uint8_t registerAddress, size_t size) {
    return i2c_write_to_device(BME280_ADDRESS, data, registerAddress, size);
}

esp_err_t bme280_init(void) {
    uint8_t writeDataConfig = CONFIG_STANDBY_TIME_62_MS | CONFIG_FILTER_COEFFICIENT_8 | CONFIG_SPI_DISABLE;

    return writeToBME(&writeDataConfig, BME280_REGISTER_CONFIG, sizeof(writeDataConfig));
}

uint32_t calculateTemperature(uint8_t temperatureMSB, uint8_t temperatureLSB, uint8_t temperatureXLSB) {
    uint32_t temperature = (temperatureMSB << 11) | (temperatureLSB << 3) | (temperatureXLSB >> 5);
    // uint32_t var1 = ((((temperature >> 3) - ((uint32_t)calibrationData.dig_T1 << 1))) * ((uint32_t)calibrationData.dig_T2)) >> 11;

    return temperature;
}
