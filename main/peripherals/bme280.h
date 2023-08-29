// Includes
#include <stdint.h>

#include "userHAL/i2c.h"

// External Dependencies

// Declarations

// The slave address of the bme280 is 0x77 when SD0 is connected to VDDIO, and 0x76 when SD0 is connected to GND
// (figure out if we are connected to ground or not)
#define BME280_ADDRESS 0b1110111

struct registerMapBME {
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

enum registerPointerValuesBME {
    BME280_REGISTER_CONFIG = 0xF5,
    BME280_REGISTER_PRESSURE_MSB = 0xF7,
    BME280_REGISTER_PRESSURE_LSB = 0xF8,
    BME280_REGISTER_PRESSURE_XLSB = 0xF9,
    BME280_REGISTER_TEMPERATURE_MSB = 0xFA,
    BME280_REGISTER_TEMPERATURE_LSB = 0xFB,
    BME280_REGISTER_TEMPERATURE_XLSB = 0xFC,
    BME280_REGISTER_HUMIDITY_MSB = 0xFD,
    BME280_REGISTER_HUMIDITY_LSB = 0xFE
};

esp_err_t readFromBME(uint8_t *buffer, size_t size);
esp_err_t writeToBME(uint8_t *data, size_t size);