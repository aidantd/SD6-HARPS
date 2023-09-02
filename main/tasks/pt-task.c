// Includes
#include <stdio.h>
#include <string.h>

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/bme280.h"

// External Dependencies

// Declarations
#define DEBUG 1

static int initializationStatus = 1;
static int numSecondsPassed = 0;

static struct registerCalibrationMapBME calibrationData;
static uint32_t temperature;
// static uint32_t pressure;
// static uint32_t humidity;

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
        }
#endif

        memset(&bmeData, 0, sizeof(bmeData));

        error |= readFromBME(&bmeData.pressureMSB, BME280_REGISTER_PRESSURE_MSB, sizeof(bmeData));

        temperature = calculateTemperature(bmeData.temperatureMSB, bmeData.temperatureLSB, bmeData.temperatureXLSB);
        printf("Temperature: %ld\n", temperature);

#ifdef DEBUG
        if (numSecondsPassed % 5 == 0) {
            numSecondsPassed = 0;
            if (error == ESP_OK) {
                printf("Read from BME280 successfully\n");

                printf("PressureLSB: %d\n", bmeData.pressureLSB);
                printf("PressureMSB: %d\n", bmeData.pressureMSB);
                printf("PressureXLSB: %d\n", bmeData.pressureXLSB);
                printf("TemperatureLSB: %d\n", bmeData.temperatureLSB);
                printf("TemperatureMSB: %d\n", bmeData.temperatureMSB);
                printf("TemperatureXLSB: %d\n", bmeData.temperatureXLSB);
                printf("HumidityLSB: %d\n", bmeData.humidityLSB);
                printf("HumidityMSB: %d\n", bmeData.humidityMSB);
            } else {
                printf("Error reading from BME280: %d\n", error);
            }
        }
#endif

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}