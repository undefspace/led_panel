#include "spectrum.h"
#include "overall_config.h"
#include <math.h>
#include <esp_log.h>
#include <led_strip.h>

extern float fft_output[FFT_SIZE * 2];
extern led_strip_t strip;
float bar_target[PANEL_WIDTH], bar_height[PANEL_WIDTH], bar_vel[PANEL_WIDTH];
float led_target[PANEL_WIDTH], led_height[PANEL_WIDTH], led_vel[PANEL_WIDTH];

uint32_t angle_to_color(float angle) {
    float r = 0, g = 0, b = 0;
	
	int i = floor(angle * 6);
	float f = angle * 6 - i;
	float p = 0;
	float q = 1 - f;
	float t = f;
	
	switch (i % 6) {
		case 0: r = 1, g = t, b = p; break;
		case 1: r = q, g = 1, b = p; break;
		case 2: r = p, g = 1, b = t; break;
		case 3: r = p, g = q, b = 1; break;
		case 4: r = t, g = p, b = 1; break;
		case 5: r = 1, g = p, b = q; break;
	}

    r *= 255;
    g *= 255;
    b *= 255;

    return 0xff000000 | ((int)r << 16) | ((int)g << 8) | (int)b;
}

void spectrum_draw(Olivec_Canvas canvas) {
    // convert fft output to bars
    for(int i = 0; i < FFT_SIZE; i++) {
        int led = i / 4;
        bar_target[led] = fft_output[i] / 6;
    }

    // apply smoothing to bars
    for(int i = 0; i < PANEL_WIDTH; i++) {
        if(bar_height[i] < bar_target[i]) {
            bar_height[i] = bar_target[i];
            bar_vel[i] = -SPECTRUM_VEL_INIT;
        } else {
            bar_height[i] = bar_height[i] + (bar_vel[i] / 60);
            bar_vel[i] *= SPECTRUM_VEL_ACCEL;
        }
    }

    for(int i = 0; i < PANEL_WIDTH; i++) {
        olivec_line(canvas,
            i, SPECTRUM_HEIGHT - 1,
            i, SPECTRUM_HEIGHT - 1 - (bar_height[i] * (SPECTRUM_HEIGHT - 1)),
            angle_to_color((float)i / (float)PANEL_WIDTH));
    }

    for(int i = 0; i < STRIP_WIDTH; i++) {
        uint32_t color = 0;
        if(i >= 0 && i < STRIP_WIDTH / 5) color = 0x00ffff;
        if(i >= 1 * STRIP_WIDTH / 5 && i < 2 * STRIP_WIDTH / 5) color = 0xff00ff;
        if(i >= 2 * STRIP_WIDTH / 5 && i < 3 * STRIP_WIDTH / 5) color = 0xffffff;
        if(i >= 3 * STRIP_WIDTH / 5 && i < 4 * STRIP_WIDTH / 5) color = 0xff00ff;
        if(i >= 4 * STRIP_WIDTH / 5 && i < 5 * STRIP_WIDTH / 5) color = 0x00ffff;
        led_strip_set_pixel(&strip, i, rgb_from_code(color));
    }
    led_strip_flush(&strip);
}
