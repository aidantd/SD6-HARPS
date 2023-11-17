// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "font6x9.h"
#include "fontx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "peripherals/l289.h"
#include "rgb565.h"
#include "utility/haglGraphics/font9x18.h"
#include "utility/haglGraphics/hagl/include/hagl.h"
#include "utility/haglGraphics/hagl_hal/include/hagl_hal.h"
#include "utility/timers/timers.h"

// External Dependencies
extern EventBits_t getWifiStatus(void);
extern uint32_t getLastRecordedWindSpeedMPH();
extern uint32_t getTemperature(void);
extern uint32_t getPressure(void);
extern void getWeatherCondition(char *pCondition);
extern uint8_t getShutterStatus(void);

// Declarations
#define WIFI_CONNECTED_BIT BIT0

static int initializationStatus = true;
static hagl_backend_t *display;

typedef struct {
    int32_t pressure;
    int32_t temperature;
    int32_t wind;
    int8_t shutterStatus;
    char condition[256];
} lastWrittenScreenData_t;

static lastWrittenScreenData_t lastWrittenScreenData = {
    .pressure = -1,
    .temperature = -1,
    .wind = -1,
    .shutterStatus = -1,
    .condition = ""};

// ********************************************************************************
// Updates the time on the screen
// ********************************************************************************
void update_time() {
    wchar_t str[256] = {0};
    if (getCurrentKnownEpochTime() != 0) {
        swprintf(str, 256, L"%s ", asctime(getKnownEstTime()));

        hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
        hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);

        hagl_put_text(display, u"00000", 20, 35, color_black, font6x9);  // clears previous entry

        hagl_put_text(display, str, 20, 35, color_white, font6x9);
    }
}

// ********************************************************************************
// Updates the pressure on the screen
// @param pressure: The pressure to update the screen with
// ********************************************************************************
void update_pressure(uint32_t pressure) {
    wchar_t str[15] = {0};
    swprintf(str, 15, L"%ld ", pressure);
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);

    hagl_put_text(display, u"00000", 130, 110, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, str, 130, 110, color_white, font6x9);
}

// ********************************************************************************
// Updates the temperature on the screen
// @param temperature: The temperature to update the screen with
// ********************************************************************************
void update_temperature(uint32_t temperature) {
    wchar_t str[15] = {0};
    swprintf(str, 15, L"%ld ", temperature);
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"00000", 130, 90, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, str, 130, 90, color_white, font6x9);
}

// ********************************************************************************
// Updates the wind speed on the screen
// @param wind: The wind speed to update the screen with
// ********************************************************************************
void update_wind(uint32_t wind) {
    wchar_t str[15] = {0};
    swprintf(str, 15, L"%ld ", wind);
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"00000", 130, 130, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, str, 130, 130, color_white, font6x9);
}

// ********************************************************************************
// Updates the weather condition on the screen
// @param condition: The weather condition to update the screen with
// ********************************************************************************
void update_condition(char condition[]) {
    wchar_t str[256] = {0};
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);

    if (memcmp(condition, "", 1) != 0) {
        swprintf(str, 256, L"%s", condition);
        printf("Made it to the not NULL\n");
    } else {
        swprintf(str, 256, L"N/A", condition);
        printf("Made it to the NULL");
    }

    hagl_put_text(display, u"000000000000000000", 20, 170, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, str, 20, 170, color_white, font6x9);
}

// ********************************************************************************
// Updates the API status on the screen
// ********************************************************************************
void update_api_status(void) {
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"000000000000000000", 20, 205, color_black, font6x9);  // clears previous entry
    EventBits_t bits = getWifiStatus();
    if (bits & WIFI_CONNECTED_BIT)
        hagl_put_text(display, u"Connected", 20, 205, color_white, font6x9);
    else
        hagl_put_text(display, u"Not Connected", 20, 205, color_white, font6x9);
}

// ********************************************************************************
// Clears the display
// ********************************************************************************
void clear_display(void) {
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_fill_rectangle(display, 0, 0, 320, 240, color_black);  // white background
}

// ********************************************************************************
// Draws the face on the screen
// @param shutterStatus: The shutter status to draw the face with
// ********************************************************************************
void draw_face(int shutterStatus) {
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_green = hagl_color(display, 0x00, 0xff, 0x00);
    hagl_color_t color_red = hagl_color(display, 0x00, 0x00, 0xff);

    if (shutterStatus == SHUTTER_STATUS_OPEN) {
        // happy smiley
        // clear smiley area
        hagl_fill_rectangle(display, 220, 65, 300, 145, color_black);
        hagl_fill_rounded_rectangle(display, 230, 65, 250, 95, 2, color_green);    // smiley
        hagl_fill_rounded_rectangle(display, 270, 65, 290, 95, 2, color_green);    // smiley
        hagl_fill_rounded_rectangle(display, 220, 135, 300, 145, 2, color_green);  // smiley

        hagl_fill_rounded_rectangle(display, 220, 124, 225, 140, 2, color_green);  // smiley
        hagl_fill_rounded_rectangle(display, 295, 124, 300, 140, 2, color_green);  // smiley

    } else if (shutterStatus == SHUTTER_STATUS_CLOSED) {
        // danger smiley
        // clear smiley area
        hagl_fill_rectangle(display, 220, 65, 300, 145, color_black);
        hagl_fill_rounded_rectangle(display, 230, 65, 250, 95, 2, color_red);    // smiley
        hagl_fill_rounded_rectangle(display, 270, 65, 290, 95, 2, color_red);    // smiley
        hagl_fill_rounded_rectangle(display, 220, 135, 300, 145, 2, color_red);  // smiley
    }
}

// ********************************************************************************
// Draws the menu on the screen
// ********************************************************************************
void draw_menu(void) {
    hagl_color_t color_yellow = hagl_color(display, 0x00, 0xff, 0xff);
    hagl_put_text(display, u"HARPS", 80, 5, color_yellow, font6x9);
    hagl_draw_rectangle(display, 10, 30, 190, 230, color_yellow);  // data box

    hagl_put_text(display, u"Orlando, Fl", 20, 55, color_yellow, font6x9);
    hagl_put_text(display, u"Weather Conditions", 20, 73, color_yellow, font6x9);
    hagl_put_text(display, u"Temperature:", 20, 90, color_yellow, font6x9);
    hagl_put_text(display, u"Pressure:", 20, 110, color_yellow, font6x9);
    hagl_put_text(display, u"Wind Speed:", 20, 130, color_yellow, font6x9);
    hagl_put_text(display, u"Conditions:", 20, 150, color_yellow, font6x9);
    hagl_put_text(display, u"API Status:", 20, 190, color_yellow, font6x9);
}

// ********************************************************************************
// ********************************************************************************
void screenTask(void *pvParameter) {
    while (1) {
        if (initializationStatus) {
            display = hagl_init();

            clear_display();
            draw_menu();

            initializationStatus = 0;
        }

        char pWeatherApiCondition[256];

#ifdef DEBUG
        printf("Updating the screen\n");
#endif

        if (getTemperature() != lastWrittenScreenData.temperature) {
            lastWrittenScreenData.temperature = getTemperature();
            update_temperature(lastWrittenScreenData.temperature);
        }
        if (getPressure() != lastWrittenScreenData.pressure) {
            lastWrittenScreenData.pressure = getPressure();
            update_pressure(lastWrittenScreenData.pressure);
        }
        if (getLastRecordedWindSpeedMPH() != lastWrittenScreenData.wind) {
            lastWrittenScreenData.wind = getLastRecordedWindSpeedMPH();
            update_wind(lastWrittenScreenData.wind);
        }
        if (getShutterStatus() != lastWrittenScreenData.shutterStatus) {
            lastWrittenScreenData.shutterStatus = getShutterStatus();
            draw_face(lastWrittenScreenData.shutterStatus);
        }

        update_api_status();

        getWeatherCondition(pWeatherApiCondition);
        update_condition(pWeatherApiCondition);

        update_time();

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}