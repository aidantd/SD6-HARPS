// Includes
#include "sen0170.h"

// External Depencencies

// Declarations
#define ADC_SEN0170 ADC1_CHANNEL_0
#define ADC_VOLTAGE_TO_METERS_PER_SECOND 6
#define ADC_BASE_VOLTAGE 78
#define ADC_VOLTAGE_TO_METERS_PER_SECOND_CONVERSION_VALUE 100

// Performing the calculation with a conversion values allows us to avoid floating point math.
#define ADC_METERS_PER_SECOND_TO_MILES_PER_HOUR(value) ((value * 2237) / MILES_PER_HOUR_CONVERSION_VALUE)
#define MILES_PER_HOUR_CONVERSION_VALUE 1000

// ********************************************************************************
// Calculates the wind speed in mi/hr from the given read voltage
// @param readVoltage: The voltage read from the ADC
// @return: The wind speed in mi/hr as a 32-bit unsigned integer
// ********************************************************************************
static uint32_t calculateWindSpeed(uint32_t readVoltage) {
    uint32_t voltageToMPS = ((readVoltage - ADC_BASE_VOLTAGE) * ADC_VOLTAGE_TO_METERS_PER_SECOND) / ADC_VOLTAGE_TO_METERS_PER_SECOND_CONVERSION_VALUE;
    return ADC_METERS_PER_SECOND_TO_MILES_PER_HOUR(voltageToMPS);
}

// ********************************************************************************
// Reads the SEN0170 sensor
// @return: The wind speed in mi/hr as a 32-bit unsigned integer
// ********************************************************************************
uint32_t readSen0170(void) {
    uint32_t readVoltage = 0;
    readVoltage = readADC(ADC_SEN0170);

#ifdef DEMO
    printf("SEN0170 Voltage: %ld mV\n", readVoltage);
#endif

    readVoltage = calculateWindSpeed(readVoltage);

    return readVoltage;
}