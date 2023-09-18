#pragma once

#include "ui_element.h"

#define BINARY_CLOCK_BG     0xff202020
#define BINARY_CLOCK_HR_FG  0xffffffff
#define BINARY_CLOCK_MIN_FG 0xffff88ff
#define BINARY_CLOCK_SEC_FG 0xffff0000

void binary_clock_draw(Olivec_Canvas canvas);
static const ui_element_t binary_clock = {
    .x = 25, .y = 13,
    .width = 6, .height = 3,
    .draw = binary_clock_draw
};
