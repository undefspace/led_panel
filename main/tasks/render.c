#include "tasks/render.h"
#include "overall_config.h"
#include "olive.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "fonts/big_clock.h"
#include <leddisplay.h>
#include <esp_log.h>

// elements
#include "ui_element.h"
#include "elements/analog_clock.h"
#include "elements/binary_clock.h"
#include "elements/decimal_clock.h"
#include "elements/hex_clock.h"
#include "elements/weather.h"
#include "elements/spectrum.h"

#define TAG "render_task"

// renderer status
QueueHandle_t render_task_queue;
uint8_t connected_to_wifi;
leddisplay_frame_t leddisplay_frame;
static uint32_t buffer[PANEL_WIDTH * PANEL_HEIGHT];
uint8_t brightness = 255;

void _render_task_process_notifications(void) { 
    render_task_notification_t notification;
    while(xQueueReceive(render_task_queue, &notification, 0)) {
        switch(notification.type) {
            // WiFi status
            case rt_notif_wifi_status:
                ESP_LOGI(TAG, "rt_notif_wifi_status: %d", connected_to_wifi);
                connected_to_wifi = notification.u.wifi_connected;
                break;
            // weather update
            case rt_notif_weather:
                #define wp notification.u.weather
                ESP_LOGI(TAG, "rt_notif_weather: sun=%d, cloud=%d, rain=%d, thunder=%d, snow=%d, temperature=%d",
                    wp.sun, wp.cloud, wp.rain, wp.thunder, wp.snow, wp.temperature);
                weather_status = (wp.sun) | (wp.cloud << 1) | (wp.rain << 2) | (wp.thunder << 3)
                    | (wp.snow << 4) | (wp.temperature << 8);
                #undef wp
                break;
            // door ring
            case rt_notif_door_ring:
                ESP_LOGI(TAG, "rt_notif_door_ring");
                break;
            // brightness
            case rt_notif_brightness:
                ESP_LOGI(TAG, "rt_notif_brightness: %d", notification.u.brightness);
                brightness = notification.u.brightness;
                break;
            // fft buffer
            case rt_notif_fft:
                // don't log (too frequent)
                break;
        }
    }
}

void render_task(void* ignored) {
    // create queue
    render_task_queue = xQueueCreate(RENDER_QUEUE_LEN, sizeof(render_task_notification_t));

    while(1) {
        // process incoming notifications
        _render_task_process_notifications();

        // clear buffers
        leddisplay_frame_clear(&leddisplay_frame);
        Olivec_Canvas canvas = olivec_canvas(buffer, PANEL_WIDTH, PANEL_HEIGHT, PANEL_WIDTH);
        olivec_fill(canvas, 0xff000000);

        // render persistent elements
        ui_element_draw(canvas, decimal_clock);
        ui_element_draw(canvas, hex_clock);
        ui_element_draw(canvas, binary_clock);
        ui_element_draw(canvas, analog_clock);
        ui_element_draw(canvas, weather);
        ui_element_draw(canvas, spectrum);

        // darken the screen and draw a Wi-Fi icon if not connected to WiFi yet
        if(!connected_to_wifi) {
            olivec_rect(canvas, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, 0xd0000000);
            olivec_text(canvas, "W", 27, 25, big_clock, 0xffffffff);
        }

        // apply brightness
        olivec_rect(canvas, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, (255 - brightness) << 24);
        
        // update display
        for(uint8_t y = 0; y < PANEL_HEIGHT; y++) {
            for(uint8_t x = 0; x < PANEL_WIDTH; x++) {
                uint32_t rgb = buffer[(y * PANEL_WIDTH) + x];
                uint8_t r = rgb >> 16;
                uint8_t g = rgb >> 8;
                uint8_t b = rgb;
                leddisplay_frame_xy_rgb(&leddisplay_frame, x, y, r, g, b);
            }
        }
        leddisplay_frame_update(&leddisplay_frame);
        
        // yield
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
