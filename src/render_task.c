#include "render_task.h"
#include "config.h"
#include <olive.c>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "fonts/big_clock.h"

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
bool connected_to_wifi;

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
        }
    }
}

void render_task(void* buffer) {
    uint32_t* buffer_u32 = (uint32_t*)buffer;

    // create queue
    render_task_queue = xQueueCreate(RENDER_QUEUE_LEN, sizeof(render_task_notification_t));

    while(1) {
        // process incoming notifications
        _render_task_process_notifications();

        // render persistent elements
        Olivec_Canvas canvas = olivec_canvas(buffer_u32, PANEL_WIDTH, PANEL_HEIGHT, PANEL_WIDTH);
        olivec_fill(canvas, 0xff000000);
        for(uint8_t i = 0; i < PERSISTENT_ELEMENT_COUNT; i++)
            ui_element_draw(canvas, persistent_elements[i]);

        // darken the screen and draw a Wi-Fi icon if not connected to WiFi yet
        if(!connected_to_wifi) {
            olivec_rect(canvas, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, 0xd0000000);
            olivec_text(canvas, "W", 27, 25, big_clock, 0xffffffff);
        }
        
        // transfer buffer to DMA
        for(uint8_t y = 0; y < PANEL_HEIGHT; y++)
            for(uint8_t x = 0; x < PANEL_WIDTH; x++)
                dma_draw_rgb(x, y, buffer_u32[(y * PANEL_WIDTH) + x]);
        
        // yield
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
