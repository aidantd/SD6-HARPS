// Includes
#include "bme280.h"

// External Dependencies

// Declarations
#define CONFIG_STANDBY_TIME_62_MS 0b00100000
#define CONFIG_FILTER_COEFFICIENT_8 0b00001100
#define CONFIG_SPI_DISABLE 0b00000000

#define CTRL_MEAS_OVERSAMPLING_PRESSURE 0b00000100
#define CTRL_MEAS_OVERSAMPLING_TEMPERATURE 0b00100000
#define CTRL_MEAS_FORCED_MODE 0b00000011

#define CTRL_HUM_OVERSAMPLING_HUMIDITY 0b00000001

static int32_t temperature_fine;

// ********************************************************************************
// Reads from the BME280 sensor starting at the given register address for the given size
// @param buffer: The buffer to store the read register data
// @param registerAddress: The register address to start reading from
// @param size: The number of bytes to read
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t readFromBME(uint8_t *buffer, uint8_t registerAddress, size_t size) {
    return i2c_read_from_device(BME280_ADDRESS, buffer, registerAddress, size);
}

// ********************************************************************************
// Writes to the BME280 sensor starting at the given register address for the given size
// @param data: The data to write to the register
// @param registerAddress: The register address to start writing to
// @param size: The number of bytes to write
// ********************************************************************************
esp_err_t writeToBME(uint8_t *data, uint8_t registerAddress, size_t size) {
    return i2c_write_to_device(BME280_ADDRESS, data, registerAddress, size);
}

// ********************************************************************************
// Initializes the BME280 sensor with the given configuration settings
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t bme280_init(void) {
    esp_err_t error = ESP_OK;
    uint8_t writeDataConfig = CONFIG_STANDBY_TIME_62_MS | CONFIG_FILTER_COEFFICIENT_8 | CONFIG_SPI_DISABLE;

    error |= writeToBME(&writeDataConfig, BME280_REGISTER_CONFIG, sizeof(writeDataConfig));

    uint8_t writeCtrlMeas = CTRL_MEAS_OVERSAMPLING_TEMPERATURE | CTRL_MEAS_OVERSAMPLING_PRESSURE | CTRL_MEAS_FORCED_MODE;

    error |= writeToBME(&writeCtrlMeas, BME280_CTRL_MEAS, sizeof(writeCtrlMeas));

    uint8_t writeDataCtrlHum = CTRL_HUM_OVERSAMPLING_HUMIDITY;

    error |= writeToBME(&writeDataCtrlHum, BME280_CTRL_HUM, sizeof(writeDataCtrlHum));

    return error;
}

// ********************************************************************************
// Calculates the temperature read from the BME280 sensor
// @param calibrationData: The calibration data read from the BME280 sensor
// @param temperatureMSB: The most significant byte of the temperature data register
// @param temperatureLSB: The least significant byte of the temperature data register
// @param temperatureXLSB: The extra least significant byte of the temperature data register
// @return: The temperature in degrees Celsius as a scaled whole number
// NOTE:
//      - Returns the temperature in degrees Celsius as a scaled whole number.
//          - For example, an output of 5123 represents 51.23 degrees Celsius.
// ********************************************************************************
int32_t calculateTemperature(struct registerCalibrationMapBME calibrationData, uint8_t temperatureMSB, uint8_t temperatureLSB, uint8_t temperatureXLSB) {
    int32_t temperature32_t = (temperatureMSB << 12) | (temperatureLSB << 4) | (temperatureXLSB >> 4);

    // Based on the BME280 datasheet, the temperature calculation requires the following steps:
    int32_t var1 = ((((temperature32_t >> 3) - ((int32_t)calibrationData.dig_T1 << 1))) * ((int32_t)calibrationData.dig_T2)) >> 11;
    int32_t var2 = (((((temperature32_t >> 4) - ((int32_t)calibrationData.dig_T1)) * ((temperature32_t >> 4) - ((int32_t)calibrationData.dig_T1))) >> 12) * ((int32_t)calibrationData.dig_T3)) >> 14;
    temperature_fine = var1 + var2;

    int32_t temperature = (temperature_fine * 5 + 128) >> 8;

    return temperature;
}

// ********************************************************************************
// Calculates the pressure read from the BME280 sensor
// @param calibrationData: The calibration data read from the BME280 sensor
// @param pressureMSB: The most significant byte of the pressure data register
// @param pressureLSB: The least significant byte of the pressure data register
// @param pressureXLSB: The extra least significant byte of the pressure data register
// @return: The pressure in Pascals as a 32-bit unsigned integer in Q24.8 format
// NOTE:
//      - Returns the pressure in Pascals as a 32-bit unsigned integer in Q24.8 format (24 integer bits and 8 fractional bits).
//          - For example, an output of 24674867 represents 24674867/256 = 96386.2 Pa = 963.862 hPa
// ********************************************************************************
uint32_t calculatePressure(struct registerCalibrationMapBME calibrationData, uint8_t pressureMSB, uint8_t pressureLSB, uint8_t pressureXLSB) {
    int32_t pressure32_t = (pressureMSB << 12) | (pressureLSB << 4) | (pressureXLSB >> 4);

    // Based on the BME280 datasheet, the pressure calculation requires the following steps:
    int64_t var1 = ((int64_t)temperature_fine) - 128000;
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

// ********************************************************************************
// Calculates the humidity read from the BME280 sensor
// @param calibrationData: The calibration data read from the BME280 sensor
// @param humidityMSB: The most significant byte of the humidity data register
// @param humidityLSB: The least significant byte of the humidity data register
// @return: The humidity in %RH as an unsigned 32 bit integer in Q22.10 format
// NOTE:
//      - Returns the humidity in %RH as an unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
//          - For example, an output of 47445 represents 47445/1024 = 46.333 %RH.
//      - Some crazy math is going on here so may want to quadruple check later
// ********************************************************************************
uint32_t calculateHumidity(struct registerCalibrationMapBME calibrationData, uint8_t humidityMSB, uint8_t humidityLSB) {
    int32_t humidity32_t = (humidityMSB << 8) | humidityLSB;

    // Based on the BME280 datasheet, the humidity calculation requires the following steps:
    int32_t var1 = temperature_fine - ((int32_t)76800);
    var1 = (((((humidity32_t << 14) - (((int32_t)calibrationData.dig_H4) << 20) - (((int32_t)calibrationData.dig_H5) * var1)) + ((int32_t)16384)) >> 15) * ((((((var1 * ((int32_t)calibrationData.dig_H6) >> 10) * (((var1 * ((int32_t)calibrationData.dig_H3) >> 11) + ((int32_t)32768)) >> 10)) + ((int32_t)2097152)) * ((int32_t)calibrationData.dig_H2) + 8192) >> 14)));
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)calibrationData.dig_H1)) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);

    return (uint32_t)(var1 >> 12);
}
