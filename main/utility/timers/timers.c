// Includes
#include "timers.h"

// External Dependencies

// Declarations
gptimer_handle_t gptimer = NULL;

static time_t currentKnownEpochTime = 0;

// ********************************************************************************
// Initializes the general purpose timer
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
esp_err_t initGPTimer(void) {
    esp_err_t error = ESP_OK;

    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1 * 1000 * 1000,  // 1MHz, 1 tick = 1us
    };

    if (gptimer_new_timer(&timer_config, &gptimer) == ESP_OK) {
        error |= gptimer_enable(gptimer);
        error |= gptimer_start(gptimer);

        return error;
    }

    return ESP_FAIL;
}

// ********************************************************************************
// Gets the current count of the general purpose timer
// @return: Current count of the general purpose timer
// ********************************************************************************
uint64_t getGPTimerCount(void) {
    uint64_t count = 0;

    gptimer_get_raw_count(gptimer, &count);

    return count;
}

// ********************************************************************************
// Creates a timeout for the given timer length based on the current timer count
// @param timerLength: Length of the timer
// @return: Timeout value
// ********************************************************************************
uint64_t createTimeout(uint64_t timerLength) {
    return getGPTimerCount() + timerLength;
}

// ********************************************************************************
// Checks if the given timeout has elapsed
// @param timeout: Timeout to check
// @return: 1 if timeout has elapsed, 0 if timeout has not elapsed
// ********************************************************************************
uint8_t isTimeoutElapsed(uint64_t timeout) {
    if (getGPTimerCount() >= timeout) {
        return 1;
    }

    return 0;
}

// ********************************************************************************
// Sets the current known epoch time
// @param epochTime: Epoch time to set
// ********************************************************************************
void setCurrentKnownEpochTime(time_t epochTime) {
    if (epochTime > currentKnownEpochTime) {
        currentKnownEpochTime = epochTime;
    }
}

// ********************************************************************************
// Increments the current known epoch time by the given amount
// @param amount: Amount to increment the current known epoch time by
// ********************************************************************************
void incrementCurrentKnownEpochTime(time_t amount) {
    currentKnownEpochTime += amount;
}

// ********************************************************************************
// Gets the known time in 24 hour format
// return: Known time in 24 hour format
// ********************************************************************************
struct tm* getKnownEstTime(void) {
    setenv("TZ", "EST5EDT", 1);
    tzset();

    return localtime(&currentKnownEpochTime);
}