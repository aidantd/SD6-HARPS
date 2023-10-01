// Includes
#include "userHAL/adc.h"

// External Depencencies

// Declarations
static esp_adc_cal_characteristics_t adc1_chars;

// ********************************************************************************
// Configures ADC1 to read with the given settings
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t configureADC(void) {
    esp_err_t error = ESP_OK;

    error |= esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11));

    return error;
}

// ********************************************************************************
// Reads from the given ADC1 channel
// @param channel: ADC1 channel to read from
// @return: ADC reading
// ********************************************************************************
uint32_t readADC(adc1_channel_t channel) {
    return esp_adc_cal_raw_to_voltage(adc1_get_raw(channel), &adc1_chars);
}
