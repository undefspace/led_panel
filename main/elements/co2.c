#include "elements/co2.h"
#include "fonts/big_clock.h"
#include <stdio.h>
#include <esp_timer.h>

int co2_ppm = 9999;

void co2_draw(Olivec_Canvas canvas) {
    // draw ppms
    char buf[16];
    sprintf(buf, "%4dO", co2_ppm);
    uint32_t color =
        (co2_ppm > CO2_HI) ? CO2_HI_COLOR :
        ((co2_ppm > CO2_MED) ? CO2_MED_COLOR :
        CO2_LOW_COLOR);
    olivec_text(canvas, buf, 0, 0, big_clock, color);

    // paint over the exclamation mark triangle if low or med
    // and make it blink if high
    bool phase = (esp_timer_get_time() / 500000) % 2;
    if((color != CO2_HI_COLOR) || phase)
        olivec_rect(canvas, 44, 0, 10, 8, 0xff000000);
}
