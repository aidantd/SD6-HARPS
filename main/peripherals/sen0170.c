// Includes
#include "sen0170.h"

// External Depencencies

// Declarations
#define ADC_SEN0170 ADC1_CHANNEL_0
#define ADC_VOLTAGE_TO_METERS_PER_SECOND 6

// Performing the calculation with a conversion values allows us to avoid floating point math.
#define ADC_METERS_PER_SECOND_TO_MILES_PER_HOUR(value) ((value * 223694) / MILES_PER_HOUR_CONVERSION_VALUE)
#define MILES_PER_HOUR_CONVERSION_VALUE 100000

// Returns the wind speed in mi/hr as a 32-bit unsigned integer.
static uint32_t calculateWindSpeed(uint32_t readVoltage) {
    return ADC_METERS_PER_SECOND_TO_MILES_PER_HOUR(readVoltage * ADC_VOLTAGE_TO_METERS_PER_SECOND);
}

// Returns the read wind speed in mi/hr as a 32-bit unsigned integer.
uint32_t readSen0170(void) {
    uint32_t readVoltage = 0;
    readVoltage = readADC(ADC_SEN0170);

    return calculateWindSpeed(readVoltage);
}