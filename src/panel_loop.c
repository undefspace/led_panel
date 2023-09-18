#include "panel_loop.h"
#include "config.h"
#include <olive.c>

// elements
#include "ui_element.h"
#include "analog_clock.h"
#include "binary_clock.h"
#include "decimal_clock.h"
#include "hex_clock.h"
ui_element_t persistent_elements[] = {
    analog_clock,
    binary_clock,
    decimal_clock,
    hex_clock,
};
#define PERSISTENT_ELEMENT_COUNT (sizeof(persistent_elements) / sizeof(ui_element_t))

void panel_loop(uint32_t* buffer) {
    Olivec_Canvas canvas = olivec_canvas(buffer, PANEL_WIDTH, PANEL_HEIGHT, PANEL_WIDTH);
    olivec_fill(canvas, 0xff000000);
    for(uint8_t i = 0; i < PERSISTENT_ELEMENT_COUNT; i++)
        ui_element_draw(canvas, persistent_elements[i]);
}
