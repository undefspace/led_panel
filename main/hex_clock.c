#include "hex_clock.h"
#include <time.h>
#include <stdio.h>
#include "fonts/small_clock.h"

void hex_clock_draw(Olivec_Canvas canvas) {
    // get time
    time_t now;
    time(&now);
    struct tm time;
    localtime_r(&now, &time);
    
    // print time
    char buf[6];
    sprintf(buf, "%02X", time.tm_hour);
    sprintf(buf + 3, "%02X", time.tm_min);
    olivec_text(canvas, "0x", 0, 0, small_clock, HEX_CLOCK_0x_FG);
    olivec_text(canvas, buf, 0, 6, small_clock, HEX_CLOCK_FG);
    olivec_text(canvas, "0x", 0, 16, small_clock, HEX_CLOCK_0x_FG);
    olivec_text(canvas, buf + 3, 0, 22, small_clock, HEX_CLOCK_FG);
}
