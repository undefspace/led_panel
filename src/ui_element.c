#include "ui_element.h"

void ui_element_draw(Olivec_Canvas canvas, ui_element_t element) {
    Olivec_Canvas subcanvas = olivec_subcanvas(canvas, element.x, element.y, element.width, element.height);
    element.draw(subcanvas);
}
