#include "elements/weather.h"
#include <stdio.h>
#include <math.h>
#include <sys/param.h>
#include "fonts/big_clock.h"
#include "fonts/small_clock.h"
#include <esp_timer.h>
#include <esp_log.h>

float weather_temp = 99.9f;
uint8_t weather_status = 1;

void _weather_safe_set_px(Olivec_Canvas canvas, int x, int y, uint32_t color) {
    if(olivec_in_bounds(canvas, x, y))
        OLIVEC_PIXEL(canvas, x, y) = color;
}

void _weather_draw_sun(Olivec_Canvas canvas) {
    olivec_circle(canvas, 5, 5, 2, WEATHER_SUN);
    _weather_safe_set_px(canvas, 1, 5, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 2, 2, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 2, 8, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 5, 1, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 5, 9, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 8, 2, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 8, 8, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 9, 5, WEATHER_SUN_RAYS);

    // moving ray parts
    uint8_t phase = (esp_timer_get_time() / 300000) % 6;
    int8_t offs = 0, pos_offs = 0, neg_offs = 0;
    switch(phase) {
        case 0:
        case 1:
            offs = -1;
            pos_offs = 0;
            neg_offs = 1;
            break;
        case 3:
        case 4:
            offs = 1;
            pos_offs = 1;
            neg_offs = 0;
            break;
        case 2:
        case 5:
            offs = 0;
            pos_offs = 0;
            neg_offs = 0;
            break;
    }
    _weather_safe_set_px(canvas, 0, 5 - offs, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 1 + pos_offs, 1 + neg_offs, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 1 + neg_offs, 9 - pos_offs, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 5 + offs, 0, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 5 - offs, 10, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 9 - neg_offs, 1 + pos_offs, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 9 - pos_offs, 9 - neg_offs, WEATHER_SUN_RAYS);
    _weather_safe_set_px(canvas, 10, 5 + offs, WEATHER_SUN_RAYS);
}

void _weather_draw_cloud(Olivec_Canvas canvas) {
    olivec_circle(canvas, 4, 3, 3, WEATHER_CLOUD);
    olivec_circle(canvas, 9, 4, 2, WEATHER_CLOUD);
    olivec_rect(canvas, 4, 5, 6, 2, WEATHER_CLOUD);
}

void _weather_draw_rain(Olivec_Canvas canvas) {
    uint8_t phase = (esp_timer_get_time() / 100000) % 10;

    for(int y = 0; y < 3; y++)
        for(int x = 0; x < 3; x++)
            _weather_safe_set_px(canvas, y * 3 + x, 2 - x, WEATHER_RAIN((phase + x - y) % 10));
}

void _weather_draw_thunder(Olivec_Canvas canvas) {
    olivec_line(canvas, 2, 0, 0, 2, WEATHER_THUNDER);
    olivec_line(canvas, 0, 2, 2, 2, WEATHER_THUNDER);
    olivec_line(canvas, 2, 2, 0, 4, WEATHER_THUNDER);
}

void weather_draw(Olivec_Canvas canvas) {
    int sun = weather_status & 1;
    int cloud = weather_status & 2;
    int rain = weather_status & 4;
    int thunder = weather_status & 8;

    // draw icons
    if(sun) {
        Olivec_Canvas sun = olivec_subcanvas(canvas, cloud ? 5 : 1, cloud ? 0 : 2, 11, 11);
        _weather_draw_sun(sun);
    }
    if(cloud) { 
        Olivec_Canvas cloud = olivec_subcanvas(canvas, 0, 4, 12, 7);
        _weather_draw_cloud(cloud);
    }
    if(rain) { 
        Olivec_Canvas rain = olivec_subcanvas(canvas, 2, 11, 9, 3);
        _weather_draw_rain(rain);
    }
    if(thunder) { 
        Olivec_Canvas thunder = olivec_subcanvas(canvas, 4, 11, 3, 5);
        _weather_draw_thunder(thunder);
    }

    // select text color
    uint32_t text_color = WEATHER_TEXT_ZERO;
    olivec_blend_color_fraction(&text_color,
        (weather_temp > 0) ? WEATHER_TEXT_HOT : WEATHER_TEXT_COLD,
        MIN(fabs(weather_temp) * 255.0 / 50.0, 255));

    // draw main text
    char buf[16];
    sprintf(buf, "%c%02dC", weather_temp >= 0 ? '+' : '-', abs((int)weather_temp));
    olivec_text(canvas, buf, 14, 0, big_clock, text_color);

    // draw tenths
    int tenths = abs((int)(weather_temp * 10) % 10);
    sprintf(buf, ".%d", tenths);
    olivec_text(canvas, buf, 47, 9, small_clock, text_color);
}
