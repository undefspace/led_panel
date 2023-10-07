#include "elements/co2.h"
#include "fonts/big_clock.h"
#include "overall_config.h"
#include <stdio.h>
#include <esp_timer.h>
#include <sys/param.h>

uint16_t* co2_ppm_buf = NULL;

void co2_draw(Olivec_Canvas canvas) {
    // draw history
    if(co2_ppm_buf) { 
        // find max
        int co2_max = 1;
        for(int i = 0; i < PANEL_WIDTH; i++)
            co2_max = MAX(co2_ppm_buf[i], co2_max);

        // draw
        int last_y = -1;
        for(int i = 0; i < PANEL_WIDTH; i++) {
            // calculate Y position
            int bar_height = co2.height * co2_ppm_buf[i] / co2_max;
            int y = co2.height - bar_height;

            // draw area under the graph
            olivec_line(canvas, i, y, i, co2.height - 1, CO2_HISTORY_AREA);

            // draw line
            if(i > 0)
                olivec_line_aa(canvas, i, y, i - 1, last_y, CO2_HISTORY_LINE);
            last_y = y;
        }
    }

    // draw current ppms
    uint16_t co2_current = co2_ppm_buf ? co2_ppm_buf[PANEL_WIDTH - 1] : 9999;
    char buf[16];
    sprintf(buf, "%4dO", co2_current);
    uint32_t color =
        (co2_current > CO2_HI) ? CO2_HI_COLOR :
        ((co2_current > CO2_MED) ? CO2_MED_COLOR :
        CO2_LOW_COLOR);
    olivec_text(canvas, buf, 6, 0, big_clock, color);

    // draw blinking exclamation mark triangle on high co2
    bool phase = (esp_timer_get_time() / 500000) % 2;
    if((color == CO2_HI_COLOR) && phase)
        olivec_text(canvas, "!", 50, 0, big_clock, color);
}
