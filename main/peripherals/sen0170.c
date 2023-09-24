// Includes
#include "sen0170.h"

// External Depencencies

// Declarations
#define ADC_SEN0170 ADC1_CHANNEL_0

// Calculates the wind speed based off the formula provided by the manufacturer
static uint32_t calculateWindSpeed(uint32_t readVoltage) {
    return readVoltage * 6;
}

// Reads in the recorded voltage from the anemometer and converts it to a wind speed
uint32_t readSen0170(void) {
    uint32_t readVoltage = 0;
    readVoltage = readADC(ADC_SEN0170);

    return calculateWindSpeed(readVoltage);
}