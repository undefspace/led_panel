#pragma once

#include "ui_element.h"

#define SPECTRUM_HEIGHT      17
#define SPECTRUM_HUE_START   0
#define SPECTRUM_HUE_END     360
#define SPECTRUM_EXP         0.75
#define SPECTRUM_VEL_INIT_UP 20
#define SPECTRUM_VEL_INIT    6
#define SPECTRUM_VEL_ACCEL   1.2

void spectrum_draw(Olivec_Canvas canvas);
static const ui_element_t spectrum = {
    .x = 0, .y = 47,
    .width = 64, .height = SPECTRUM_HEIGHT,
    .draw = spectrum_draw
};
