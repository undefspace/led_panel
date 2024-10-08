#pragma once

#include "ui_element.h"

#define HEX_CLOCK_0x_FG 0xff404040
#define HEX_CLOCK_FG    0xffffffff

#define HEX_CLOCK_0x1B_ANIMATION_PERIOD 5.0f

void hex_clock_draw(Olivec_Canvas canvas);
static const ui_element_t hex_clock = {
    .x = 25, .y = 1,
    .width = 7, .height = 27,
    .draw = hex_clock_draw
};
