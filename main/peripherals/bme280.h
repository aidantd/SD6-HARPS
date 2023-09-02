// Includes
#include <stdint.h>

#include "userHAL/i2c.h"

// External Dependencies

// Declarations

// The slave address of the bme280 is 0x77 when SD0 is connected to VDDIO, and 0x76 when SD0 is connected to GND
// (figure out if we are connected to ground or not)
#define BME280_ADDRESS 0x76

typedef struct registerDataMapBME {
    uint8_t config;
    uint8_t pressureMSB;
    uint8_t pressureLSB;
    uint8_t pressureXLSB;
    uint8_t temperatureMSB;
    uint8_t temperatureLSB;
    uint8_t temperatureXLSB;
    uint8_t humidityMSB;
    uint8_t humidityLSB;
};

typedef struct registerCalibrationMapBME {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
};

enum registerPointerValuesBME {
    // Calibration Registers
    BME280_REGISTER_DIG_T1 = 0x88,
    BME280_REGISTER_DIG_T2 = 0x8A,
    BME280_REGISTER_DIG_T3 = 0x8C,
    BME280_REGISTER_DIG_P1 = 0x8E,
    BME280_REGISTER_DIG_P2 = 0x90,
    BME280_REGISTER_DIG_P3 = 0x92,
    BME280_REGISTER_DIG_P4 = 0x94,
    BME280_REGISTER_DIG_P5 = 0x96,
    BME280_REGISTER_DIG_P6 = 0x98,
    BME280_REGISTER_DIG_P7 = 0x9A,
    BME280_REGISTER_DIG_P8 = 0x9C,
    BME280_REGISTER_DIG_P9 = 0x9E,
    BME280_REGISTER_DIG_H1 = 0xA1,
    BME280_REGISTER_DIG_H2 = 0xE1,
    BME280_REGISTER_DIG_H3 = 0xE3,

    // Overlapping Calibration Registers
    BME280_REGISTER_DIG_H4,
    BME280_REGISTER_DIG_H5,

    // Data Registers
    BME280_REGISTER_CONFIG = 0xF5,
    BME280_REGISTER_PRESSURE_MSB = 0xF7,
    BME280_REGISTER_PRESSURE_LSB,
    BME280_REGISTER_PRESSURE_XLSB,
    BME280_REGISTER_TEMPERATURE_MSB,
    BME280_REGISTER_TEMPERATURE_LSB,
    BME280_REGISTER_TEMPERATURE_XLSB,
    BME280_REGISTER_HUMIDITY_MSB,
    BME280_REGISTER_HUMIDITY_LSB,
};

esp_err_t readFromBME(uint8_t *buffer, uint8_t registerAddress, size_t size);
esp_err_t writeToBME(uint8_t *data, uint8_t registerAddress, size_t size);
esp_err_t bme280_init(void);
uint32_t calculateTemperature(struct registerCalibrationMapBME calibrationData, uint8_t temperatureMSB, uint8_t temperatureLSB, uint8_t temperatureXLSB);