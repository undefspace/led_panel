#pragma once

#include "ui_element.h"

#define SPECTRUM_HEIGHT      17
#define SPECTRUM_VEL_INIT    2
#define SPECTRUM_VEL_ACCEL   1.2

uint32_t angle_to_color(float angle);
void spectrum_draw(Olivec_Canvas canvas);
static const ui_element_t spectrum = {
    .x = 0, .y = 47,
    .width = 64, .height = SPECTRUM_HEIGHT,
    .draw = spectrum_draw
};
