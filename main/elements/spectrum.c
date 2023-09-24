#include "elements/spectrum.h"
#include "overall_config.h"
#include <math.h>
#include <esp_log.h>
#include <led_strip.h>
#include "images/rzheniye.h"

extern float fft_output[FFT_SIZE * 2];
float bar_target[PANEL_WIDTH], bar_height[PANEL_WIDTH], bar_vel[PANEL_WIDTH];

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
    for(int i = 0; i < FFT_SIZE / 2; i++) {
        int led = i / 2;
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

    olivec_sprite_copy(canvas, 0, 0, spectrum.width, spectrum.height, rzheniye);
    for(int i = 0; i < PANEL_WIDTH; i++) {
        uint32_t rainbow_color = angle_to_color((float)i / (float)PANEL_WIDTH);
        olivec_line(canvas,
            i, 0,
            i, SPECTRUM_HEIGHT - (bar_height[i] * SPECTRUM_HEIGHT),
            0xff000000);
        olivec_line(canvas,
            i, SPECTRUM_HEIGHT,
            i, SPECTRUM_HEIGHT - (bar_height[i] * (SPECTRUM_HEIGHT - 1)),
            (rainbow_color & 0xffffff) | 0x40000000);
    }
}
