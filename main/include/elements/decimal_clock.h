#pragma once

#include "ui_element.h"

#define DECIMAL_CLOCK_FG 0xffffffff

void decimal_clock_draw(Olivec_Canvas canvas);
static const ui_element_t decimal_clock = {
    .x = 0, .y = 0,
    .width = 24, .height = 29,
    .draw = decimal_clock_draw
};
