#pragma once

#include "ui_element.h"

#define ANALOG_CLOCK_CIRCLE_COLOR   0xffffffff
#define ANALOG_CLOCK_SEC_HAND_COLOR 0xffff0000
#define ANALOG_CLOCK_MIN_HAND_COLOR 0xffff88ff
#define ANALOG_CLOCK_HR_HAND_COLOR  0xffffffff

void analog_clock_draw(Olivec_Canvas canvas);
static const ui_element_t analog_clock = {
    .x = 35, .y = 0,
    .width = 29, .height = 29,
    .draw = analog_clock_draw
};
