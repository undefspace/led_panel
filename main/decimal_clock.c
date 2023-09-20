#include "decimal_clock.h"
#include <time.h>
#include <stdio.h>
#include "fonts/big_clock.h"

void decimal_clock_draw(Olivec_Canvas canvas) {
    // get time
    time_t now;
    time(&now);
    struct tm time;
    localtime_r(&now, &time);
    
    // print time
    char buf[6];
    sprintf(buf, "%02d", time.tm_hour);
    sprintf(buf + 3, "%02d", time.tm_min);
    olivec_text(canvas, buf, 0, 0, big_clock, DECIMAL_CLOCK_FG);
    olivec_text(canvas, buf + 3, 0, 15, big_clock, DECIMAL_CLOCK_FG);
}
