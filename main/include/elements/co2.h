#pragma once

#include "ui_element.h"

#define CO2_MED       1000
#define CO2_HI        2000
#define CO2_LOW_COLOR 0xff00ff00
#define CO2_MED_COLOR 0xffffff00
#define CO2_HI_COLOR  0xffff0000

void co2_draw(Olivec_Canvas canvas);
static const ui_element_t co2 = {
    .x = 68, .y = 0,
    .width = 56, .height = 14,
    .draw = co2_draw
};
extern int co2_ppm;
