// Includes
#include <stdio.h>

#ifdef AIDAN
#include "/Users/aidan/esp/esp-idf/components/driver/gptimer/include/driver/gptimer.h"
#endif

#ifdef SPENCER
#include "/Users/spencer/esp/esp-idf/components/driver/gptimer/include/driver/gptimer.h"
#endif

#include "esp_err.h"

// External Dependencies

// Declarations

esp_err_t initGPTimer(void);
uint64_t getGPTimerCount(void);
uint64_t createTimeout(uint64_t timerLength);
uint8_t isTimeoutElapsed(uint64_t timeout);