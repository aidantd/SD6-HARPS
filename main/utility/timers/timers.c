// Includes
#include "timers.h"

// External Dependencies

// Declarations
gptimer_handle_t gptimer = NULL;

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

uint64_t getGPTimerCount(void) {
    uint64_t count = 0;

    gptimer_get_raw_count(gptimer, &count);

    return count;
}

uint64_t createTimeout(uint64_t timerLength) {
    return getGPTimerCount() + timerLength;
}

uint8_t isTimeoutElapsed(uint64_t timeout) {
    if (getGPTimerCount() >= timeout) {
        return 1;
    }

    return 0;
}
