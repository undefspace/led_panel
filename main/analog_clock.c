#include "analog_clock.h"
#include <time.h>
#include <math.h>

#define TWO_PI ((float)(2 * 3.141))

void analog_clock_draw(Olivec_Canvas canvas) {
    // get time
    time_t now;
    time(&now);
    struct tm time;
    localtime_r(&now, &time);

    // derive graphics parameters
    float cx = canvas.width / 2;
    float cy = canvas.height / 2;
    float r = canvas.width / 2;
    float sec_r = r * 4/5;
    float min_r = sec_r * 5/6;
    float hr_r = min_r * 2/3;
    
    // calculate hand angles
    int32_t seconds = ((time.tm_hour % 12) * 3600) + (time.tm_min * 60) + time.tm_sec;
    float sec_angle = TWO_PI * (float)(seconds - 15) / 60;
    float min_angle = TWO_PI * (float)(seconds - 900) / 3600;
    float hr_angle = TWO_PI * (float)(seconds - (12*900)) / (12 * 3600);

    // draw!
    olivec_circle(canvas, cx, cy, r, ANALOG_CLOCK_CIRCLE_COLOR);
    olivec_circle(canvas, cx, cy, r - 1, 0xff000000);
    olivec_line_aa(canvas, cx, cy, cx + (cosf(sec_angle) * sec_r), cy + (sinf(sec_angle) * sec_r), ANALOG_CLOCK_SEC_HAND_COLOR);
    olivec_line_aa(canvas, cx, cy, cx + (cosf(min_angle) * min_r), cy + (sinf(min_angle) * min_r), ANALOG_CLOCK_MIN_HAND_COLOR);
    olivec_line_aa(canvas, cx, cy, cx + (cosf(hr_angle) * hr_r),  cy + (sinf(hr_angle) * hr_r), ANALOG_CLOCK_HR_HAND_COLOR);
}
