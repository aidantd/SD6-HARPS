// Includes
#include "speaker.h"

// External Dependencies

// Declarations

// ********************************************************************************
// Sets the speaker to output a given tone
// @param tone: The tone to output (voltage)
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t setSpeakerTone(uint8_t tone) {
    esp_err_t error = setDACVoltage(tone);

    return error;
}
