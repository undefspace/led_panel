#include "elements/binary_clock.h"
#include <time.h>

void _bin_clock_draw_number(Olivec_Canvas canvas, uint8_t n, uint8_t x, uint8_t y, uint32_t bg, uint32_t fg) {
    x += 6;
    for(uint8_t bit = 0; bit < 6; bit++) {
        x--;
        olivec_rect(canvas, x, y, 1, 1, ((n >> bit) & 1) ? fg : bg);
    }
}

void binary_clock_draw(Olivec_Canvas canvas) {
    // get time
    time_t now;
    time(&now);
    struct tm time;
    localtime_r(&now, &time);

    // draw binary numbers
    _bin_clock_draw_number(canvas, time.tm_hour, 0, 0, BINARY_CLOCK_BG, BINARY_CLOCK_HR_FG);
    _bin_clock_draw_number(canvas, time.tm_min, 0, 1, BINARY_CLOCK_BG, BINARY_CLOCK_MIN_FG);
    _bin_clock_draw_number(canvas, time.tm_sec, 0, 2, 0xff000000, BINARY_CLOCK_SEC_FG);
}
