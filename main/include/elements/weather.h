#pragma once

#include "ui_element.h"

#define WEATHER_CLOUD    0xffffffff
#define WEATHER_SUN      0xffffff00
#define WEATHER_SUN_RAYS 0xff4c4c00
#define WEATHER_RAIN(i)  ((i == 0) ? 0xff00abc1 : ((i == 1) ? 0xff017180 : 0xff003d45))
#define WEATHER_THUNDER  0xffff8f00
#define WEATHER_TEXT     0xffffffff

void weather_draw(Olivec_Canvas canvas);
static const ui_element_t weather = {
    .x = 3, .y = 0,
    .width = 58, .height = 17,
    .draw = weather_draw
};
extern uint8_t weather_status;
extern float weather_temp;
