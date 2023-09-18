#pragma once

#include <stdint.h>
#include <olive.c>

typedef struct {
    uint8_t x, y, width, height;
    void (*draw)(Olivec_Canvas canvas);
} ui_element_t;

void ui_element_draw(Olivec_Canvas canvas, ui_element_t element);
