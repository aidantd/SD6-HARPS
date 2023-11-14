#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "font6x9.h"
#include "fontx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "rgb565.h"
#include "utility/haglGraphics/font9x18.h"
#include "utility/haglGraphics/hagl/include/hagl.h"
#include "utility/haglGraphics/hagl_hal/include/hagl_hal.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0

extern EventBits_t getWifiStatus(void);

static int initializationStatus = true;
static hagl_backend_t *display;

void update_pressure(uint32_t pressure) {
    char str[15] = {0};
    sprintf(str,"%ld",pressure);
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);

    hagl_put_text(display, u"00000", 130, 110, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, (const wchar_t*) str, 130, 110, color_white, font6x9);
}
void update_temperature(uint32_t temperature) {
    char str[15] = {0};
    sprintf(str,"%ld",temperature);
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"00000", 130, 90, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, (const wchar_t*) str, 130, 90, color_white, font6x9);
}
void update_wind(uint32_t wind) {
    char str[15] = {0};
    sprintf(str,"%ld",wind);
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"00000", 130, 130, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, (const wchar_t*) str, 130, 130, color_white, font6x9);
}
void update_precipitation(char condition[]) {
    
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"000000000000000000", 20, 170, color_black, font6x9);  // clears previous entry

    hagl_put_text(display, (const wchar_t*) condition, 20, 170, color_white, font6x9);
}
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

void clear_display(void) {
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_fill_rectangle(display, 0, 0, 320, 240, color_black);  // white background
}

void draw_face(bool happy) {
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_green = hagl_color(display, 0x00, 0xff, 0x00);
    hagl_color_t color_red = hagl_color(display, 0x00, 0x00, 0xff);

    // clear smiley area
    hagl_fill_rectangle(display, 220, 65, 300, 145, color_black);
    if (happy) {
        // happy smiley >>
        hagl_fill_rounded_rectangle(display, 230, 65, 250, 95, 2, color_green);    // smiley
        hagl_fill_rounded_rectangle(display, 270, 65, 290, 95, 2, color_green);    // smiley
        hagl_fill_rounded_rectangle(display, 220, 135, 300, 145, 2, color_green);  // smiley

        hagl_fill_rounded_rectangle(display, 220, 124, 225, 140, 2, color_green);  // smiley
        hagl_fill_rounded_rectangle(display, 295, 124, 300, 140, 2, color_green);  // smiley
    } else {
        // danger smiley
        hagl_fill_rounded_rectangle(display, 230, 65, 250, 95, 2, color_red);    // smiley
        hagl_fill_rounded_rectangle(display, 270, 65, 290, 95, 2, color_red);    // smiley
        hagl_fill_rounded_rectangle(display, 220, 135, 300, 145, 2, color_red);  // smiley
    }
}

void draw_menu(void) {
    hagl_color_t color_yellow = hagl_color(display, 0x00, 0xff, 0xff);
    hagl_put_text(display, u"HARPS", 80, 5, color_yellow, font6x9);
    hagl_draw_rectangle(display, 10, 30, 190, 230, color_yellow);  // data box

    hagl_put_text(display, u"Orlando, Fl", 20, 55, color_yellow, font6x9);
    hagl_put_text(display, u"Weather Conditions", 20, 73, color_yellow, font6x9);
    hagl_put_text(display, u"Temperature:", 20, 90, color_yellow, font6x9);
    hagl_put_text(display, u"Pressure:", 20, 110, color_yellow, font6x9);
    hagl_put_text(display, u"Wind Speed:", 20, 130, color_yellow, font6x9);
    hagl_put_text(display, u"Precipitation:", 20, 150, color_yellow, font6x9);
    hagl_put_text(display, u"API Status:", 20, 190, color_yellow, font6x9);
}
void screenTask(void *pvParameter) {
    while (1) {
        if(initializationStatus) {
           display = hagl_init(); 
           initializationStatus = 0;
        }

        char conditions[] = "Rainy";
        printf("writing to screen\n");

        clear_display();

        draw_menu();
        draw_face(true);
        update_temperature(0);
        update_pressure(0);
        update_wind(0);
        update_api_status();
        update_precipitation(conditions);

        draw_face(false);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}