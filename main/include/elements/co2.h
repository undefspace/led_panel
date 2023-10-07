#pragma once

#include "ui_element.h"

#define CO2_MED          1000
#define CO2_HI           2000
#define CO2_LOW_COLOR    0xff00bb00
#define CO2_MED_COLOR    0xffffff00
#define CO2_HI_COLOR     0xffff0000
#define CO2_HISTORY_LINE 0xffaaaaaa
#define CO2_HISTORY_AREA 0xff101010

void co2_draw(Olivec_Canvas canvas);
static const ui_element_t co2 = {
    .x = 64, .y = 0,
    .width = 64, .height = 14,
    .draw = co2_draw
};
extern uint16_t* co2_ppm_buf;
