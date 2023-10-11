// Includes
#include "dac.h"

// External Dependencies

// Declarations

static dac_oneshot_handle_t dacHandle;

// ********************************************************************************
// Initializes the DAC
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initDAC(void) {
    dac_oneshot_config_t chan0_cfg = {
        .chan_id = DAC_CHAN_0,
    };

    esp_err_t error = dac_oneshot_new_channel(&chan0_cfg, &dacHandle);

    return error;
}

// ********************************************************************************
// Sets the DAC output voltage
// @param voltage: The voltage to set the DAC to
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t setDACVoltage(uint8_t voltage) {
    esp_err_t error = dac_oneshot_output_voltage(dacHandle, voltage);

    return error;
}