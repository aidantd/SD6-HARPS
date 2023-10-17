// Includes
#include "dac.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// External Dependencies

// Declarations

// static dac_continuous_handle_t dac_handle;

dac_cosine_handle_t chan0_handle;

static uint32_t cnt = 1;

// ********************************************************************************
// Initializes the DAC
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initDAC(void) {
    esp_err_t error = ESP_OK;

    dac_cosine_config_t cos0_cfg = {
        .chan_id = DAC_CHAN_0,
        .freq_hz = 1000,  // It will be covered by 8000 in the latter configuration
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT,
        .offset = 0,
        .phase = DAC_COSINE_PHASE_0,
        .atten = DAC_COSINE_ATTEN_DEFAULT,
        .flags.force_set_freq = false,
    };

    error |= dac_cosine_new_channel(&cos0_cfg, &chan0_handle);

    return error;
}

// ********************************************************************************
// Starts the DAC output voltage
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
void startDacCosinSignal(void) {
    dac_cosine_start(chan0_handle);
}

// ********************************************************************************
// Stops the DAC output voltage
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
void stopDacCosinSignal(void) {
    dac_cosine_stop(chan0_handle);
}