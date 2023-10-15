// Includes
#include "dac.h"

// External Dependencies

// Declarations

// ********************************************************************************
// Initializes the DAC
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initDAC(void) {
    return dac_output_enable(DAC_CHAN_0);
}

// ********************************************************************************
// Sets the DAC output voltage to play audio
// @param audio_data: The audio data to play
// @param length: The length of the audio data
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
void setDacVoltage(uint8_t *audio_data, size_t length) {
    dac_output_voltage(DAC_CHAN_0, 0);

    for (size_t i = 0; i < length; i++) {
        dac_output_voltage(DAC_CHAN_0, audio_data[i]);
    }

    dac_output_voltage(DAC_CHAN_0, 0);
}