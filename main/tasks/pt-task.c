// Includes
#include <stdio.h>
#include <string.h>

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/bme280.h"

// External Dependencies

// Declarations

static int initializationStatus = true;
static int numSecondsPassed;

static struct registerCalibrationMapBME calibrationData;
static uint32_t temperature;
static uint32_t pressure;
static uint32_t humidity;

// ********************************************************************************
// Gets the last recorded temperature
// @return: Temperature in degrees Celsius
// ********************************************************************************
uint32_t getTemperature(void) {
    return temperature;
}

// ********************************************************************************
// Gets the last recorded pressure
// @return: Pressure in Pascals
// ********************************************************************************
uint32_t getPressure(void) {
    return pressure;
}

// ********************************************************************************
// Gets the last recorded humidity
// @return: Humidity in %RH
// ********************************************************************************
uint32_t getHumidity(void) {
    return humidity;
}

// ********************************************************************************
// ********************************************************************************
void pt_task(void *pvParameter) {
    while (1) {
        esp_err_t error = ESP_OK;
        struct registerDataMapBME bmeData;
        numSecondsPassed++;

        if (initializationStatus) {
            initializationStatus = 0;
            bme280_init();

            error |= readFromBME(&calibrationData.dig_T1, BME280_REGISTER_DIG_T1, sizeof(calibrationData));

#ifdef DEBUG
            if (error == ESP_OK) {
                printf("Read from BME280 Calibration Successfully\n");
                printf("dig_T1: %X\n", calibrationData.dig_T1);
                printf("dig_T2: %X\n", calibrationData.dig_T2);
                printf("dig_T3: %X\n", calibrationData.dig_T3);
                printf("dig_P1: %X\n", calibrationData.dig_P1);
                printf("dig_P2: %X\n", calibrationData.dig_P2);
                printf("dig_P3: %X\n", calibrationData.dig_P3);
                printf("dig_P4: %X\n", calibrationData.dig_P4);
                printf("dig_P5: %X\n", calibrationData.dig_P5);
                printf("dig_P6: %X\n", calibrationData.dig_P6);
                printf("dig_P7: %X\n", calibrationData.dig_P7);
                printf("dig_P8: %X\n", calibrationData.dig_P8);
                printf("dig_P9: %X\n", calibrationData.dig_P9);
                printf("dig_H1: %X\n", calibrationData.dig_H1);
                printf("dig_H2: %X\n", calibrationData.dig_H2);
                printf("dig_H3: %X\n", calibrationData.dig_H3);
                printf("dig_H4: %X\n", calibrationData.dig_H4);
                printf("dig_H5: %X\n", calibrationData.dig_H5);
            } else {
                printf("Error reading from BME280: %d\n", error);
            }
#endif
        }

        memset(&bmeData, 0, sizeof(bmeData));

        error |= readFromBME(&bmeData.ctrlHum, BME280_CTRL_HUM, sizeof(bmeData));

        temperature = calculateTemperatureFahrenheit(calibrationData, bmeData.temperatureMSB, bmeData.temperatureLSB, bmeData.temperatureXLSB);
        pressure = calculatePressureMillibars(calibrationData, bmeData.pressureMSB, bmeData.pressureLSB, bmeData.pressureXLSB);
        humidity = calculateHumidityRH(calibrationData, bmeData.humidityMSB, bmeData.humidityLSB);

#ifdef DEMO
        if (error == ESP_OK) {
            printf("Temperature: %ld Fahrenheit\n", temperature);
            printf("Pressure: %ld Millibars\n", pressure);
            printf("Humidity: %ld RH\n\n", humidity);
        } else {
            printf("Error reading from BME280: %d\n", error);
        }
#endif

#ifdef DEBUG
        if (error == ESP_OK) {
            printf("Temperature: %ld Fahrenheit\n", temperature);
            printf("Pressure: %ld Millibars\n", pressure);
            printf("Humidity: %ld RH\n\n", humidity);

            printf("Ctrl Hum: %X\n", bmeData.ctrlHum);
            printf("Status: %X\n", bmeData.status);
            printf("Ctrl Meas: %X\n", bmeData.ctrlMeas);
            printf("Config: %X\n", bmeData.config);
            printf("Pressure MSB: %X\n", bmeData.pressureMSB);
            printf("Pressure LSB: %X\n", bmeData.pressureLSB);
            printf("Pressure XLSB: %X\n", bmeData.pressureXLSB);
            printf("Temperature MSB: %X\n", bmeData.temperatureMSB);
            printf("Temperature LSB: %X\n", bmeData.temperatureLSB);
            printf("Temperature XLSB: %X\n", bmeData.temperatureXLSB);
            printf("Humidity MSB: %X\n", bmeData.humidityMSB);
            printf("Humidity LSB: %X\n\n", bmeData.humidityLSB);
        } else {
            printf("Error reading from BME280: %d\n", error);
        }

#endif

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}