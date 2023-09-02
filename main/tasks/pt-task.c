// Includes
#include <stdio.h>
#include <string.h>

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripherals/bme280.h"

// External Dependencies

// Declarations

void pt_task(void *pvParameter) {
    while (1) {
        esp_err_t error = ESP_OK;
        struct registerMapBME registerData;
        memset(&registerData, 0, sizeof(registerData));

        uint8_t writeDataConfig = 0b01001000;

        error |= i2c_write_to_device(BME280_ADDRESS, &writeDataConfig, BME280_REGISTER_CONFIG, sizeof(writeDataConfig));

        if (error != ESP_OK) {
            printf("Error writing to BME280: %d\n", error);
        } else {
            printf("Wrote to BME280 successfully\n");
        }

        error |= i2c_read_from_device(BME280_ADDRESS, &registerData.pressureMSB, BME280_REGISTER_PRESSURE_MSB, sizeof(registerData));

        if (error != ESP_OK) {
            printf("Error reading from BME280: %d\n", error);
        } else {
            printf("Read from BME280 successfully\n");

            printf("PressureLSB: %d\n", registerData.pressureLSB);
            printf("PressureMSB: %d\n", registerData.pressureMSB);
            printf("PressureXLSB: %d\n", registerData.pressureXLSB);
            printf("TemperatureLSB: %d\n", registerData.temperatureLSB);
            printf("TemperatureMSB: %d\n", registerData.temperatureMSB);
            printf("TemperatureXLSB: %d\n", registerData.temperatureXLSB);
            printf("HumidityLSB: %d\n", registerData.humidityLSB);
            printf("HumidityMSB: %d\n", registerData.humidityMSB);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}