#include "render_task.h"
#include "overall_config.h"
#include "olive.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "fonts/big_clock.h"
#include <leddisplay.h>

// elements
#include "ui_element.h"
#include "analog_clock.h"
#include "binary_clock.h"
#include "decimal_clock.h"
#include "hex_clock.h"
#include "weather.h"
ui_element_t persistent_elements[] = {
    analog_clock,
    binary_clock,
    decimal_clock,
    hex_clock,
    weather
};
#define PERSISTENT_ELEMENT_COUNT (sizeof(persistent_elements) / sizeof(ui_element_t))

// renderer status
QueueHandle_t render_task_queue;
uint8_t connected_to_wifi;
leddisplay_frame_t leddisplay_frame;
static uint32_t buffer[PANEL_WIDTH * PANEL_HEIGHT];

void _render_task_process_notifications(void) { 
    render_task_notification_t notification;
    while(xQueueReceive(render_task_queue, &notification, 0)) {
        switch(notification.type) {
            // WiFi status
            case rt_notif_wifi_status:
                connected_to_wifi = notification.u.wifi_connected;
                break;
            // weather update
            case rt_notif_weather:
                #define wp notification.u.weather
                weather_status = (wp.sun) | (wp.cloud << 1) | (wp.rain << 2) | (wp.thunder << 3) | (wp.temperature << 8);
                #undef wp
                break;
            // door ring
            case rt_notif_door_ring:
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
        for(uint8_t i = 0; i < PERSISTENT_ELEMENT_COUNT; i++)
            ui_element_draw(canvas, persistent_elements[i]);

        // darken the screen and draw a Wi-Fi icon if not connected to WiFi yet
        if(!connected_to_wifi) {
            olivec_rect(canvas, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, 0xd0000000);
            olivec_text(canvas, "W", 27, 25, big_clock, 0xffffffff);
        }
        
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
        // leddisplay_pixel_update(1);
        
        // yield
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
