#pragma once

#include <stdint.h>
#include "olive.h"

typedef struct {
    int x, y, width, height;
    void (*draw)(Olivec_Canvas canvas);
} ui_element_t;

void ui_element_draw(Olivec_Canvas canvas, ui_element_t element);
