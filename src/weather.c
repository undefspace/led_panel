#include "weather.h"
#include <stdio.h>
#include "fonts/big_clock.h"

void _weather_draw_sun(Olivec_Canvas canvas) {
    olivec_circle(canvas, 4, 4, 2, WEATHER_SUN);
    OLIVEC_PIXEL(canvas, 0, 4) = WEATHER_SUN_RAYS;
    OLIVEC_PIXEL(canvas, 1, 1) = WEATHER_SUN_RAYS;
    OLIVEC_PIXEL(canvas, 1, 7) = WEATHER_SUN_RAYS;
    OLIVEC_PIXEL(canvas, 4, 0) = WEATHER_SUN_RAYS;
    OLIVEC_PIXEL(canvas, 4, 8) = WEATHER_SUN_RAYS;
    OLIVEC_PIXEL(canvas, 7, 1) = WEATHER_SUN_RAYS;
    OLIVEC_PIXEL(canvas, 7, 7) = WEATHER_SUN_RAYS;
    OLIVEC_PIXEL(canvas, 8, 4) = WEATHER_SUN_RAYS;
}

void _weather_draw_cloud(Olivec_Canvas canvas) {
    olivec_circle(canvas, 4, 3, 3, WEATHER_CLOUD);
    olivec_circle(canvas, 9, 4, 2, WEATHER_CLOUD);
    olivec_rect(canvas, 4, 5, 6, 2, WEATHER_CLOUD);
}

void _weather_draw_rain(Olivec_Canvas canvas) {
    for(int i = 0; i < 3; i++)
        olivec_line(canvas, i * 3, 2, 2 + (i * 3), 0, WEATHER_RAIN);
}

void _weather_draw_thunder(Olivec_Canvas canvas) {
    olivec_line(canvas, 2, 0, 0, 2, WEATHER_THUNDER);
    olivec_line(canvas, 0, 2, 2, 2, WEATHER_THUNDER);
    olivec_line(canvas, 2, 2, 0, 4, WEATHER_THUNDER);
}

void weather_draw(Olivec_Canvas canvas) {
    // TODO: get weather
    int sun = 1;
    int cloud = 1;
    int rain = 1;
    int thunder = 1;
    int temp = 99;

    // draw icons
    if(sun) {
        Olivec_Canvas sun = olivec_subcanvas(canvas, cloud ? 6 : 2, cloud ? 1 : 2, 9, 9);
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

    // draw text
    char buf[5];
    sprintf(buf, "%c%02dC", temp > 0 ? '+' : '-', abs(temp));
    olivec_text(canvas, buf, 14, 0, big_clock, WEATHER_TEXT);
}
