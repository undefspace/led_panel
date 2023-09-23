#include "tasks/led.h"
#include <led_strip.h>
#include <driver/rmt.h>
#include "overall_config.h"
#include "elements/spectrum.h"
#include <esp_timer.h>

led_strip_t strip;

void led_task(void* ignored) {
    // initialize strip
    strip.type = LED_STRIP_WS2812;
    strip.is_rgbw = false;
    strip.brightness = 255;
    strip.length = STRIP_WIDTH;
    strip.gpio = 21;
    strip.channel = RMT_CHANNEL_0;
    led_strip_install();
    led_strip_init(&strip);

    while(1) {
        // draw animated rainbow
        float time = esp_timer_get_time() / 1000000.0f / STRIP_ANIMATION_PERIOD;
        for(int i = 0; i < STRIP_WIDTH; i++) {
            uint32_t color = angle_to_color(((float)i / (float)STRIP_WIDTH) + time);
            led_strip_set_pixel(&strip, i, rgb_from_code(color));
        }

        // flush to strip
        led_strip_flush(&strip);

        // yield
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
