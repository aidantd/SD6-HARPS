// Includes
#include "dac.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// External Dependencies

// Declarations

// static dac_continuous_handle_t dac_handle;

dac_oneshot_handle_t chan0_handle;

static uint32_t cnt = 1;

// ********************************************************************************
// Initializes the DAC
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initDAC(void) {
    esp_err_t error = ESP_OK;

    dac_oneshot_config_t chan0_cfg = {
        .chan_id = DAC_CHAN_0,
    };
    error |= dac_oneshot_new_channel(&chan0_cfg, &chan0_handle);

    return error;
}

// ********************************************************************************
// Sets the DAC output voltage to play audio
// @param audio_data: The audio data to play
// @param length: The length of the audio data
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
void setDacVoltage(uint8_t *data, size_t data_size) {
    size_t playDuration = 0;

    while (playDuration < data_size) {
        /* Set the voltage every 100 ms */
        ESP_ERROR_CHECK(dac_oneshot_output_voltage(chan0_handle, data[playDuration]));
        printf("Setting speaker voltage to %X\n", data[playDuration]);
        playDuration++;
        vTaskDelay(400 / portTICK_PERIOD_MS);
    }
}