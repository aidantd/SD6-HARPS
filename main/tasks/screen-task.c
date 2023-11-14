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

void update_pressure(void *const display, uint32_t pressure) {
    char str[15] = {0};
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"00000", 130, 110, color_black, font9x18);  // clears previous entry

    hagl_put_text(display, u"00000", 130, 110, color_white, font9x18);
}
void update_temperature(void *const display, uint32_t temperature) {
    char str[15] = {0};
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"00000", 130, 90, color_black, font9x18);  // clears previous entry

    hagl_put_text(display, u"00000", 130, 90, color_white, font9x18);
}
void update_wind(void *const display, uint32_t temperature) {
    char str[15] = {0};
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"00000", 130, 130, color_black, font9x18);  // clears previous entry

    hagl_put_text(display, u"00000", 130, 130, color_white, font9x18);
}
void update_precipitation(void *const display, uint32_t precipitation) {
    char str[15] = {0};
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"000000000000000000", 20, 170, color_black, font9x18);  // clears previous entry

    hagl_put_text(display, u"000000000000000000", 20, 170, color_white, font9x18);
}
void update_api_status(void *const display, uint32_t pressure) {
    char str[15] = {0};
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_put_text(display, u"000000000000000000", 20, 205, color_black, font9x18);  // clears previous entry

    hagl_put_text(display, u"000000000000000000", 20, 205, color_white, font9x18);
}

void clear_display(void *const display) {
    hagl_color_t color_black = hagl_color(display, 0x00, 0x00, 0x00);
    hagl_color_t color_white = hagl_color(display, 0xff, 0xff, 0xff);
    hagl_fill_rectangle(display, 0, 0, 320, 240, color_black);  // white background
}

void draw_face(void *const display, bool happy) {
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

void draw_menu(void *const display) {
    hagl_color_t color_yellow = hagl_color(display, 0x00, 0xff, 0xff);
    hagl_put_text(display, u"HARPS", 80, 5, color_yellow, font9x18);
    hagl_draw_rectangle(display, 10, 30, 190, 230, color_yellow);  // data box

    hagl_put_text(display, u"Orlando, Fl", 20, 55, color_yellow, font9x18);
    hagl_put_text(display, u"Weather Conditions", 20, 73, color_yellow, font9x18);
    hagl_put_text(display, u"Temperature:", 20, 90, color_yellow, font9x18);
    hagl_put_text(display, u"Pressure:", 20, 110, color_yellow, font9x18);
    hagl_put_text(display, u"Wind Speed:", 20, 130, color_yellow, font9x18);
    hagl_put_text(display, u"Precipitation:", 20, 150, color_yellow, font9x18);
    hagl_put_text(display, u"API Status:", 20, 190, color_yellow, font9x18);
}
void screenTask(void *pvParameter) {
    while (1) {
        // unint32_t is the temp/pressure/wind

        printf("writing to screen");

        hagl_backend_t *display = hagl_init();

        clear_display(display);

        draw_menu(display);
        draw_face(display, true);
        update_temperature(display,0);
        update_pressure(display,0);
        update_wind(display,0);
        update_api_status(display,0);
        update_precipitation(display,0);

        draw_face(display, false);
        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}