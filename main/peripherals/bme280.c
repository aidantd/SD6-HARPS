// Includes
#include "bme280.h"

// External Dependencies

// Declarations
#define CONFIG_STANDBY_TIME_62_MS 0b00100000
#define CONFIG_FILTER_COEFFICIENT_8 0b00001100
#define CONFIG_SPI_DISABLE 0b00000000

static int32_t t_fine;

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

// Returns the temperature in degrees Celsius as a scaled whole number.
// For example, an output of 5123 represents 51.23 degrees Celsius.
int32_t calculateTemperature(struct registerCalibrationMapBME calibrationData, uint8_t temperatureMSB, uint8_t temperatureLSB, uint8_t temperatureXLSB) {
    int32_t temperature32_t = (temperatureMSB << 12) | (temperatureLSB << 4) | (temperatureXLSB >> 4);

    int32_t var1 = ((((temperature32_t >> 3) - ((int32_t)calibrationData.dig_T1 << 1))) * ((int32_t)calibrationData.dig_T2)) >> 11;
    int32_t var2 = (((((temperature32_t >> 4) - ((int32_t)calibrationData.dig_T1)) * ((temperature32_t >> 4) - ((int32_t)calibrationData.dig_T1))) >> 12) * ((int32_t)calibrationData.dig_T3)) >> 14;
    t_fine = var1 + var2;

    int32_t temperature = (t_fine * 5 + 128) >> 8;

    return temperature;
}

// Returns the pressure in Pascals as a 32-bit unsigned integer in Q24.8 format (24 integer bits and 8 fractional bits).
// For example, an output of 24674867 represents 24674867/256 = 96386.2 Pa = 963.862 hPa.
uint32_t calculatePressure(struct registerCalibrationMapBME calibrationData, uint8_t pressureMSB, uint8_t pressureLSB, uint8_t pressureXLSB) {
    int32_t pressure32_t = (pressureMSB << 12) | (pressureLSB << 4) | (pressureXLSB >> 4);

    int64_t var1 = ((int64_t)t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)calibrationData.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calibrationData.dig_P5) << 17);
    var2 = var2 + ((int64_t)calibrationData.dig_P4 << 35);
    var1 = ((var1 * var1 * (int64_t)calibrationData.dig_P3) >> 8) + ((var1 * (int64_t)calibrationData.dig_P2) << 12);
    var1 = ((((uint64_t)1) << 47) + var1) * ((int64_t)calibrationData.dig_P1) >> 33;

    if (var1 == 0) {
        return 0;
    }

    int64_t pressure = 1048576 - pressure32_t;
    pressure = (((pressure << 31) - var2) * 3125) / var1;
    var1 = ((int64_t)calibrationData.dig_P9 * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2 = ((int64_t)calibrationData.dig_P8 * pressure) >> 19;
    pressure = ((pressure + var1 + var2) >> 8) + ((int64_t)calibrationData.dig_P7 << 4);

    return (uint32_t)pressure;
}
